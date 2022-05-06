#define _CRT_SECURE_NO_WARNINGS
/*----------------------------------------------------------------*
 HOOK�n�錾
 *----------------------------------------------------------------*/

/////////////////////////////////////////////////////////////////////
// API�t�b�N
/////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN
#define STRICT

#include "WinTarget.h"
#include <windowsx.h>
#include <tchar.h>
#include <tlhelp32.h>
#include <dbghelp.h>
#include <mmsystem.h>
#include <shellapi.h>

#include "CommonGlobalHandle.h"
#include "TSModCommand.h"
#include "CurProcessMemory.h"
#include "HookIntersectRectMessage.h"
#include "HookInvalidateRectMessage.h"
#include "HookDefWindowProcACustom.h"
#include "HookBitBltCustom.h"
#include "HookMessageBoxACustom.h"
#include "HookTextOutACustom.h"
#include "Menu.h"
#include "GetOSVersion.h"


// ImageDirectoryEntryToData
#pragma comment(lib, "dbghelp.lib")



// �ЂƂ̃��W���[���ɑ΂���API�t�b�N���s���֐�
void ReplaceIATEntryInOneMod(
                             PCSTR pszModuleName,
                             PROC pfnCurrent,
                             PROC pfnNew,
                             HMODULE hmodCaller) 
{
    ULONG ulSize;
    PIMAGE_IMPORT_DESCRIPTOR pImportDesc;
    pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToData(
        hmodCaller, TRUE, IMAGE_DIRECTORY_ENTRY_IMPORT, &ulSize);

    if (pImportDesc == NULL)
        return;

    while(pImportDesc->Name) {
        PSTR pszModName = (PSTR) ((PBYTE) hmodCaller + pImportDesc->Name);
        if (lstrcmpiA(pszModName, pszModuleName) == 0) 
            break;
        pImportDesc++;
    }

    if (pImportDesc->Name == 0)
        return;

    PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA) 
        ((PBYTE) hmodCaller + pImportDesc->FirstThunk);

    while(pThunk->u1.Function) {
        PROC *ppfn = (PROC*) &pThunk->u1.Function;
        BOOL fFound = (*ppfn == pfnCurrent);
        if (fFound) {
            DWORD dwDummy;
            VirtualProtect(ppfn, sizeof(ppfn), PAGE_EXECUTE_READWRITE, &dwDummy);
            WriteProcessMemory(
                CommonGlobalHandle::curProcess, ppfn, &pfnNew, sizeof(pfnNew), NULL);
            return;
        }
        pThunk++;
    }
    return;
}

// ���ׂẴ��W���[���ɑ΂���API�t�b�N���s���֐�
void ReplaceIATEntryInAllMods(
                              PCSTR pszModuleName, 
                              PROC pfnCurrent, 
                              PROC pfnNew) 
{
    // ���W���[�����X�g���擾
    HANDLE hModuleSnap = CreateToolhelp32Snapshot(
        TH32CS_SNAPMODULE, GetCurrentProcessId());
    if(hModuleSnap == INVALID_HANDLE_VALUE)
        return;

    MODULEENTRY32 me;
    me.dwSize = sizeof(me);
    BOOL bModuleResult = Module32First(hModuleSnap, &me);
    // ���ꂼ��̃��W���[���ɑ΂���ReplaceIATEntryInOneMod�����s
    while(bModuleResult) { 
        ReplaceIATEntryInOneMod(pszModuleName, pfnCurrent, pfnNew, me.hModule);
        bModuleResult = Module32Next(hModuleSnap, &me);
    }
    CloseHandle(hModuleSnap);
}

// �t�b�N����֐��̃v���g�^�C�v���`
typedef BOOL (WINAPI *PFNINTERSECTRECT)(LPRECT, CONST RECT *, CONST RECT *);

/*----------------------------------------------------------------*
 ��̓I�ȃt�b�N�֐�!!!! 
 *----------------------------------------------------------------*/
PROC pfnOrigIntersectRect = GetProcAddress( GetModuleHandleA("user32.dll"), "IntersectRect");

// �V�ċL�̃��b�Z�[�W�E�B���h�E�n�̃n�b�N
BOOL WINAPI Hook_IntersectRect(
	LPRECT lprcDst,        // �����������i�[�����o�b�t�@
	CONST RECT *lprcSrc1,  // �ŏ��̒����`
	CONST RECT *lprcSrc2   // 2 �Ԗڂ̒����`
)
{
	Hook_IntersectRectMessage(lprcDst, lprcSrc1, lprcSrc2);

	//-------------------------------------------- �������牺�̓I���W�i�����Ăяo������
	// ���̌�A���̂��̂��Ăяo��
    BOOL bResult = ((PFNINTERSECTRECT) pfnOrigIntersectRect)(lprcDst, lprcSrc1, lprcSrc2);
	return bResult;
}

#include "DialogKahou.h"

extern DialogKahou DK;

// �t�b�N����֐��̃v���g�^�C�v���`
typedef BOOL (WINAPI *PFNINVALIDATERECT)(HWND, CONST RECT *, BOOL);

PROC pfnOrigInvalidateRect = GetProcAddress( GetModuleHandleA("user32.dll"), "InvalidateRect");

// �V�ċL�̍ĕ`�悷��K�v�̂���͈͌n�̃n�b�N
BOOL WINAPI Hook_InvalidateRect(
	HWND hWnd,           // �E�B���h�E�̃n���h��
	CONST RECT *lpRect,  // �����`�̍��W
	BOOL bErase          // �������邩�ǂ����̏��
)
{
	Hook_InvalidateRectMessage(hWnd, lpRect, bErase);

	//-------------------------------------------- �������牺�̓I���W�i�����Ăяo������
	// ���̌�A���̂��̂��Ăяo��
	BOOL bResult = false;
	if ( DK.eModeKahou >= KAHOU_MODE3 ) {
		bResult = ((PFNINVALIDATERECT) pfnOrigInvalidateRect)(hWnd, NULL, bErase);
	} else {
		bResult = ((PFNINVALIDATERECT) pfnOrigInvalidateRect)(hWnd, lpRect, bErase);
	}
	return bResult;
}



// �f�����[�h���H
extern bool is_DemoMode();

// �t�b�N����֐��̃v���g�^�C�v���`
typedef LRESULT (WINAPI *PFNDEFWINDOWPROCA)( HWND, UINT, WPARAM, LPARAM );

PROC pfnOrigDefWindowProcA = GetProcAddress( GetModuleHandleA("user32.dll"), "DefWindowProcA");

// �摜�n�̃n�b�N
LRESULT WINAPI Hook_DefWindowProcA(
	HWND hWnd,      // �E�B���h�E�̃n���h��
	UINT Msg,       // ���b�Z�[�W�̎��ʎq
	WPARAM wParam,  // ���b�Z�[�W�̍ŏ��̃p�����[�^
	LPARAM lParam   // ���b�Z�[�W�� 2 �Ԗڂ̃p�����[�^
)
{
	Hook_DefWindowProcACustom( hWnd, Msg, wParam, lParam );

	// �X�N���[���Z�C�o�[�̋N����h�~����B�X�N���[���Z�C�o�[�ɂȂ肻���Ȏ��ɁA�N�����Ă���A�v���̂����ꂩ���A1��Ԃ���
	// �X�N���[���Z�C�o�[�̋N���͖h�~�����悤���B
	if ( Msg == WM_SYSCOMMAND && wParam == SC_SCREENSAVE ) {
		// �u�f�����[�h�Ȃ�X�N���[���Z�C�o�[���N�����Ȃ��v�̃t���O�������Ă��āA���݃f�����[�h�Ȃ�΁A
		if ( TSModCommand::Environment::cmdNoScreenSaverInDemo == 1 && is_DemoMode() ) {
			// �X�N���[���Z�C�o�[�͋N�����Ȃ��B
			return 1;

		}
	}

    // �S�Ẵ}�E�X���b�Z�[�W�̍ŏ��ɂ��ꂪ��������B
	// �}�E�X�{�^������Ƀ^�C�g���o�[��ŉ����ꂽ���Ƃɂ���
 	if ( Msg == WM_NCHITTEST ) {

		// ����ȊO�̓E�B���h�E�𓮂����B(�^�C�g���Ƀ}�E�X��u���Ă���Ɠ�������������)
		
		// �ȉ��A�{�[�_�[�t�߂̍��W�̓^�C�g���o�[���}�E�X�ŃN���b�N���Ă��邱�ƂƓ������ƂƂ���B
		// ����ɂ��A���������Ƃ��\�ƂȂ�B

		HWND hWnd = CommonGlobalHandle::tenshouWND;

		// �X�N���[����̍��W
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);

		// �E�B���h�E�̃X�N���[���̈�𓾂�
		RECT rect_move;
		GetWindowRect( hWnd, &rect_move );

		// �{�[�_�[���ق�̓��������ӂƂ݂Ȃ��B�{�[�_�[����1�����Ȃ��Ă��ŏ��ł�5�s�N�Z���͓������镝������
		int iBorderX = GetSystemMetrics( SM_CXBORDER );
		int iBorderY = GetSystemMetrics( SM_CYBORDER );
		iBorderX = max( iBorderX, 8 );
		iBorderY = max( iBorderY, 8 );
		rect_move.left += iBorderX;
		rect_move.right -= iBorderX;
		rect_move.top += iBorderY;
		rect_move.bottom -= iBorderY;

		// �}�E�X�̃N���b�N�_���A�{�[�_�[�g�t�߂ɂ�������A�^�C�g���o�[���N���b�N�������̂Ƃ݂Ȃ��B
		if ( !PtInRect( &rect_move, pt ) ) {

			// ���j���[�o�[�ƃ^�C�g���o�[���B�����[�h�ŁA���A�X�N���[���̃T�C�Y�I�����I���Ă���(���j���[�́u�I���v��Valild�ɂȂ�����
			if ( TSModCommand::Environment::cmdNoTitleBarAndMenuBar && CommonGlobalHandle::IsAfterScreenSizeSelected ) {

				// �E�B���h�E�̃X�N���[���̈�𓾂�
				RECT rect_menu;
				GetWindowRect( hWnd, &rect_menu );

				// �g�b�v�Ɋւ��Ă̂݃{�[�_�[���ق�̓��������ӂƂ݂Ȃ��B
				rect_menu.top += iBorderY;

				// �㕔�̃o�[�t�߂ŃN���b�N���Ă���B
				if ( !PtInRect( &rect_menu, pt ) ) {

					// ���݂̃��j���[�́H
					HMENU curMenu = GetMenu(hWnd);

					if ( GetAsyncKeyState(VK_LBUTTON) < 0 ) {

						// ���j���[������
						if ( !curMenu ) {
							SetMenu( CommonGlobalHandle::tenshouWND, CommonGlobalHandle::hMenu);
							// ���j���[�\���ێ��^�C�}�[���㏑������B�T�b�ێ�
							SetTimer( CommonGlobalHandle::tenshouWND, TIMER_EVENT_ID_MENU_HIDE, 3000, TenshouMenuHide );
						} else {
							SetMenu( CommonGlobalHandle::tenshouWND, NULL);
						}

					}
				}
			}

			// ���N���b�N���Ă����祥�
			if (GetAsyncKeyState(VK_LBUTTON) < 0 ) {

				return HTCAPTION;
			// �E�N���b�N���Ă����祥�
			} else if (GetAsyncKeyState(VK_RBUTTON) < 0 ) {
				// �����A�C�R�������Ă����猳�ɖ߂�
				if ( IsIconic(hWnd) ) {

					// ���ɖ߂�
					OpenIcon( hWnd );
				} else {
					ShowWindow( hWnd, SW_MINIMIZE);
				}

				return HTCAPTION;
			}
		}

	}

	//-------------------------------------------- �������牺�̓I���W�i�����Ăяo������
	// ���̌�A���̂��̂��Ăяo��
    LRESULT LPResult = ((PFNDEFWINDOWPROCA) pfnOrigDefWindowProcA)(hWnd, Msg, wParam, lParam );

    return LPResult;
}

// �t�b�N����֐��̃v���g�^�C�v���`
typedef LRESULT (WINAPI *PFNSENDMESSAGEA)( HWND, UINT, WPARAM, LPARAM );

PROC pfnOrigSendMessageA = GetProcAddress( GetModuleHandleA("user32.dll"), "SendMessageA");

// �摜�n�̃n�b�N
LRESULT WINAPI Hook_SendMessageA(
	HWND hWnd,      // �E�B���h�E�̃n���h��
	UINT Msg,       // ���b�Z�[�W�̎��ʎq
	WPARAM wParam,  // ���b�Z�[�W�̍ŏ��̃p�����[�^
	LPARAM lParam   // ���b�Z�[�W�� 2 �Ԗڂ̃p�����[�^
)
{
	//-------------------------------------------- �������牺�̓I���W�i�����Ăяo������
	// ���̌�A���̂��̂��Ăяo��
    LRESULT LPResult = ((PFNSENDMESSAGEA) pfnOrigSendMessageA)(hWnd, Msg, wParam, lParam );

    return LPResult;
}




// �t�b�N����֐��̃v���g�^�C�v���`
typedef BOOL (WINAPI *PFNBITBLT)( HDC, int, int, int, int, HDC, int, int, DWORD );

PROC pfnOrigBitBlt = GetProcAddress( GetModuleHandleA("gdi32.dll"), "BitBlt");

// �摜�n�̃n�b�N
BOOL WINAPI Hook_BitBlt(
	HDC hdcDest, // �R�s�[��f�o�C�X�R���e�L�X�g�̃n���h��
	int nXDest,  // �R�s�[�撷���`�̍������ x ���W
	int nYDest,  // �R�s�[�撷���`�̍������ y ���W
	int nWidth,  // �R�s�[�撷���`�̕�
	int nHeight, // �R�s�[�撷���`�̍���
	HDC hdcSrc,  // �R�s�[���f�o�C�X�R���e�L�X�g�̃n���h��
	int nXSrc,   // �R�s�[�������`�̍������ x ���W
	int nYSrc,   // �R�s�[�������`�̍������ y ���W
	DWORD dwRop  // ���X�^�I�y���[�V�����R�[�h
)
{
	//-------------------------------------------- �������牺�̓I���W�i�����Ăяo������
	// ���̌�A���̂��̂��Ăяo��
    BOOL bResult = ((PFNBITBLT) pfnOrigBitBlt)(hdcDest, nXDest, nYDest, nWidth, nHeight, hdcSrc, nXSrc, nYSrc, dwRop );

	return bResult;
}

extern CurProcessMemory CPM;

// �t�b�N����֐��̃v���g�^�C�v���`
typedef BOOL (WINAPI *PFNPATBLT)( HDC, int, int, int, int, DWORD );

PROC pfnOrigPatBlt = GetProcAddress( GetModuleHandleA("gdi32.dll"), "PatBlt");

// �摜�n�̃n�b�N
BOOL WINAPI Hook_PatBlt(
	HDC hdc,      // �f�o�C�X�R���e�L�X�g�̃n���h��
	int nXLeft,   // �����`�̍������ x ���W
	int nYLeft,   // �����`�̍������ y ���W
	int nWidth,   // �����`�̕�
	int nHeight,  // �����`�̍���
	DWORD dwRop   // ���X�^�I�y���[�V�����R�[�h
)
{
	// ���݂̃f�o�C�X�R���e�L�X�g�̃n���h���l���R�s�[
	//-------------------------------------------- �������牺�̓I���W�i�����Ăяo������
    BOOL bResult = ((PFNPATBLT) pfnOrigPatBlt)(hdc, nXLeft, nYLeft, nWidth, nHeight, dwRop );

	return bResult;
}


// �t�b�N����֐��̃v���g�^�C�v���`
typedef int (WINAPI *PFNMESSAGEBOXA)(HWND, PCSTR, PCSTR, UINT);

PROC pfnOrigMessageBoxA = GetProcAddress( GetModuleHandleA("user32.dll"), "MessageBoxA");

int WINAPI Hook_MessageBoxA(
	HWND hWnd,          // �I�[�i�[�E�B���h�E�̃n���h��
	PCTSTR lpText,     // ���b�Z�[�W�{�b�N�X���̃e�L�X�g
	PCTSTR lpCaption,  // ���b�Z�[�W�{�b�N�X�̃^�C�g��
	UINT uType          // ���b�Z�[�W�{�b�N�X�̃X�^�C��
)
{
    // ���̂��̂��Ăяo��
    int nResult = ((PFNMESSAGEBOXA) pfnOrigMessageBoxA)(hWnd, lpText, lpCaption, uType );
	
	// ���ʂɌ�ŌĂяo���B(�������Ƃ�m�邽��)
	Hook_MessageBoxACustom( hWnd, lpText, lpCaption, uType, nResult );
	
	return nResult;
}

// �t�b�N����֐��̃v���g�^�C�v���`
typedef int (WINAPI *PFNTIMEGETTIME)(VOID);

PROC pfnOrigtimeGetTime = GetProcAddress( GetModuleHandleA("winmm.dll"), "timeGetTime");

int WINAPI Hook_TimeGetTime(VOID)
{

    // ���̂��̂��Ăяo��
    int nResult = ((PFNTIMEGETTIME) pfnOrigtimeGetTime)();
	
	return nResult;
}

// �t�b�N����֐��̃v���g�^�C�v���`
typedef HANDLE (WINAPI *PFNCREATEFILEA)(LPCTSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);

PROC pfnCreateFileA = GetProcAddress( GetModuleHandleA("kernel32.dll"), "CreateFileA");

HANDLE WINAPI Hook_CreateFileA(
	LPCTSTR lpFileName,                         // �t�@�C����
	DWORD dwDesiredAccess,                      // �A�N�Z�X���[�h
	DWORD dwShareMode,                          // ���L���[�h
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, // �Z�L�����e�B�L�q�q
	DWORD dwCreationDisposition,                // �쐬���@
	DWORD dwFlagsAndAttributes,                 // �t�@�C������
	HANDLE hTemplateFile                        // �e���v���[�g�t�@�C���̃n���h��
)
{

    // ���̂��̂��Ăяo��
    HANDLE nResult = ((PFNCREATEFILEA) pfnCreateFileA)(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	
	return nResult;
}



// �t�b�N����֐��̃v���g�^�C�v���`
typedef BOOL (WINAPI *PFNTEXTOUTA)(HDC, int, int, LPCTSTR, int );

PROC pfnOrigTextOutA = GetProcAddress( GetModuleHandleA("gdi32.dll"), "TextOutA");

BOOL WINAPI Hook_TextOutA(
	HDC hdc,           // �f�o�C�X�R���e�L�X�g�̃n���h��
	int nXStart,       // �J�n�ʒu�i��_�j�� x ���W
	int nYStart,       // �J�n�ʒu�i��_�j�� y ���W
	LPCTSTR lpString,  // ������
	int cbString       // ������
) {
	// ��ɃJ�X�^���̕������s�B
	Hook_TextOutACustom(hdc, nXStart, nYStart, lpString, cbString);

    // ���̂��̂��Ăяo��
    int nResult = ((PFNTEXTOUTA) pfnOrigTextOutA)(hdc, nXStart, nYStart, lpString, cbString);
	
	return nResult;
}

// �t�b�N����֐��̃v���g�^�C�v���`
typedef BOOL (WINAPI *PFGETVOLUMEINFORMATIONA)(LPCTSTR, LPTSTR, DWORD, LPDWORD, LPDWORD, LPDWORD, LPTSTR, DWORD );

PROC pfnOrigGetVolumeInformationA = GetProcAddress( GetModuleHandleA("kernel32.dll"), "GetVolumeInformationA");

BOOL WINAPI Hook_GetVolumeInformationA(
  LPCTSTR lpRootPathName,           // ���[�g�f�B���N�g��
  LPTSTR lpVolumeNameBuffer,        // �{�����[�����o�b�t�@
  DWORD nVolumeNameSize,            // ���O�o�b�t�@�̃T�C�Y
  LPDWORD lpVolumeSerialNumber,     // �{�����[���̃V���A���ԍ�
  LPDWORD lpMaximumComponentLength, // �t�@�C�����̍ő�̒���
  LPDWORD lpFileSystemFlags,        // �t�@�C���V�X�e���̃I�v�V����
  LPTSTR lpFileSystemNameBuffer,    // �t�@�C���V�X�e�������i�[����o�b�t�@
  DWORD nFileSystemNameSize         // �t�@�C���V�X�e�������i�[����o�b�t�@�̃T�C�Y
) {
    // ���̂��̂��Ăяo��
    int nResult = ((PFGETVOLUMEINFORMATIONA) pfnOrigGetVolumeInformationA)(
		lpRootPathName,
		lpVolumeNameBuffer, nVolumeNameSize,
		lpVolumeSerialNumber,
		lpMaximumComponentLength,
		lpFileSystemFlags,
		lpFileSystemNameBuffer,
		nFileSystemNameSize
		);
	
	if ( lpVolumeNameBuffer && strcmp(lpVolumeNameBuffer, "Audio CD") == 0) {
		strncpy(lpVolumeNameBuffer, "TENSHOUKI95", nVolumeNameSize-1);
	}

	return nResult;
}



// �t�b�N����֐��̃v���g�^�C�v���`
typedef BOOL (WINAPI *PFWINHELPA)(HWND, LPCTSTR, UINT, DWORD );

PROC pfnOrigWinHelpA = GetProcAddress( GetModuleHandleA("user32.dll"), "WinHelpA");

BOOL WINAPI Hook_WinHelpA(
    HWND hWndMain, // �E�B���h�E�̃n���h��
    LPCTSTR lpszHelp, // �w���v�t�@�C����
    UINT uCommand, // �w���v�̎��
    DWORD dwData // �ǉ��̃f�[�^
) {
    // ���̂��̂��Ăяo��
    int nResult = ((PFWINHELPA) pfnOrigWinHelpA)(
		hWndMain,
		lpszHelp,
		uCommand,
		dwData
		);

	if (nResult == 0) {
		if (getOSVersion() >= 6.0 ) { // Vista�ȍ~
			ShellExecute(CommonGlobalHandle::tenshouWND, "open", "http://xn--rssu31gj1g.jp/?page=nobu_install_winhlp32", NULL, NULL, SW_SHOWNORMAL);
		}
	}
	
	return nResult;
}


/*----------------------------------------------------------------*
 HOOK�n����
 *----------------------------------------------------------------*/
bool isHookIntersectRect = false;
bool isHookInvalidateRect = false;
bool isHookDefWindowProcA = false;
bool isHookSendMessageA = false;
bool isHookBitBlt = false;
bool isHookPatBlt = false;
bool isHookMessageBoxA = false;
bool isHookTimeGetTime = false;
bool isHookCreateFileA = false;
bool isHookTextOutA = false;
bool isHookGetVolumeInformationA = false;
bool isHookWinHelpA = false;


void HookFunctions() {
	PROC pfnOrig;
	if ( !isHookIntersectRect ) {
		isHookIntersectRect = true;

		pfnOrig = ::GetProcAddress( GetModuleHandleA("user32.dll"), "IntersectRect");
		ReplaceIATEntryInAllMods( "user32.dll", pfnOrig, (PROC)Hook_IntersectRect);
	}
	if ( !isHookDefWindowProcA ) {
		isHookDefWindowProcA = true;
		pfnOrig = ::GetProcAddress( GetModuleHandleA("user32.dll"), "DefWindowProcA");
		ReplaceIATEntryInAllMods( "user32.dll", pfnOrig, (PROC)Hook_DefWindowProcA);
	}
	/*
	if ( !isHookInvalidateRect ) {
		isHookInvalidateRect = true;

		pfnOrig = ::GetProcAddress( GetModuleHandleA("user32.dll"), "InvalidateRect");
		ReplaceIATEntryInAllMods( "user32.dll", pfnOrig, (PROC)Hook_InvalidateRect);
	}
	*/
	/*
	if ( !isHookSendMessageA ) {
		isHookSendMessageA = true;
		pfnOrig = ::GetProcAddress( GetModuleHandleA("user32.dll"), "SendMessageA");
		ReplaceIATEntryInAllMods( "user32.dll", pfnOrig, (PROC)Hook_SendMessageA);
	}
	*/
	/*
	if ( !isHookBitBlt ) {
		isHookBitBlt = true;
		pfnOrig = ::GetProcAddress( GetModuleHandleA("gdi32.dll"), "BitBlt");
		ReplaceIATEntryInAllMods( "gdi32.dll", pfnOrig, (PROC)Hook_BitBlt);
	}
	*/
	/*
	if ( !isHookPatBlt ) {
		isHookPatBlt = true;
		pfnOrig = ::GetProcAddress( GetModuleHandleA("gdi32.dll"), "PatBlt");
		ReplaceIATEntryInAllMods( "gdi32.dll", pfnOrig, (PROC)Hook_PatBlt);
	}
	*/
	if ( !isHookMessageBoxA ) {
		isHookMessageBoxA = true;
	    pfnOrig = ::GetProcAddress( GetModuleHandleA("user32.dll"), "MessageBoxA");
		ReplaceIATEntryInAllMods( "user32.dll", pfnOrig, (PROC)Hook_MessageBoxA);
	}
	/*
	if ( !isHookTimeGetTime ) {
		isHookTimeGetTime = true;
	    pfnOrig = ::GetProcAddress( GetModuleHandleA("winmm.dll"), "timeGetTime");
		ReplaceIATEntryInAllMods( "winmm.dll", pfnOrig, (PROC)Hook_TimeGetTime);
	}
	*/
	/*
	if ( !isHookCreateFileA ) {
		isHookCreateFileA = true;
	    pfnOrig = ::GetProcAddress( GetModuleHandleA("kernel32.dll"), "CreateFileA");
		ReplaceIATEntryInAllMods( "kernel32.dll", pfnOrig, (PROC)Hook_CreateFileA);
	}
	*/
	/*
	if ( !isHookTextOutA ) {
		isHookTextOutA = true;
		pfnOrig = ::GetProcAddress( GetModuleHandleA("gdi32.dll"), "TextOutA");
		ReplaceIATEntryInAllMods( "gdi32.dll", pfnOrig, (PROC)Hook_TextOutA);
	}
	*/
	/*
	if ( !isHookGetVolumeInformationA ) {
		isHookGetVolumeInformationA = true;
	    pfnOrig = ::GetProcAddress( GetModuleHandleA("kernel32.dll"), "GetVolumeInformationA");
		ReplaceIATEntryInAllMods( "kernel32.dll", pfnOrig, (PROC)Hook_GetVolumeInformationA);
	}
	*/
	if ( !isHookWinHelpA ) {
		isHookWinHelpA = true;
	    pfnOrig = ::GetProcAddress( GetModuleHandleA("user32.dll"), "WinHelpA");
		ReplaceIATEntryInAllMods( "user32.dll", pfnOrig, (PROC)Hook_WinHelpA);
	}
}





