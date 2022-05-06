#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

#include "FuncImagePngDialog.h"

//---- GDI+�w�b�_�֘A
#include <gdiplus.h>
#pragma comment (lib, "gdiplus.lib")        // �����N���ɕK�v�ȃ��C�u����

using namespace Gdiplus;

static ULONG_PTR gdiplusToken;
static HDC hDC;
static int iPngLeft=-1, iPngTop=-1;

BOOL isModePngDialog = FALSE;

int StartPngDialog(char *szPngName, int posX, int posY) {

    // �����R�[�h�����C�h������ɕϊ�
    Gdiplus::GdiplusStartupInput    gdiplusStartupInput;
 
    //---- GDI+�̏����ݒ�
    if( Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL) !=  Gdiplus::Ok ) {
        return FALSE;
    }

 	// �E�B���h�E�̃f�o�C�X�R���e�L�X�g�n���h�����擾����
	hDC = GetDC( CommonGlobalHandle::tenshouWND );

	Graphics pngGraphics(hDC);

	WCHAR wc_szPngName[256];
	mbstowcs(wc_szPngName, szPngName, strlen(szPngName)+1);

	Image pngImage(wc_szPngName);
	if ( pngImage.GetLastStatus() != Ok ) {
		return FALSE;
	}

	// �^�񒆂����߂邽�߁B
	RECT rc; // �N���C�A���g�̈�̃T�C�Y
	::GetClientRect(CommonGlobalHandle::tenshouWND, &rc);

	int width = rc.right-rc.left;
	int height = rc.bottom-rc.top;

	iPngLeft = width/2 - pngImage.GetWidth()/2;
	iPngTop = height/2 - pngImage.GetHeight()/2;

	// ���W���w�肳��Ă���Ώ㏑��
	if ( posX != -1 ) {
		iPngLeft = posX;
	}
	if ( posY != -1 ) {
		iPngTop = posY;
	}

	pngGraphics.DrawImage(&pngImage, iPngLeft, iPngTop, pngImage.GetWidth(), pngImage.GetHeight());

	return TRUE;
}

void EndPngDialog() {
	//---- GDI+�̉��
    Gdiplus::GdiplusShutdown(gdiplusToken);

	ReleaseDC( CommonGlobalHandle::tenshouWND, hDC );   // �E�B���h�E�̃f�o�C�X�R���e�L�X�g�n���h�����������
}

void _WaitPngDialog(int iTime) {
	// �}�E�X�N���b�N�ҋ@
	DWORD preTimeGetTime = timeGetTime();
	// ���b�Z�[�W���x�ɂ���āA�C���[�W�_�C�A���O�������I�ɏ�����b�����قȂ�B
	int RefleshTimeArray[] = { 300, 800, 2000, 4000, INT_MAX };
	int RefleshTime = RefleshTimeArray[ nb6settings.messagespd ]; // ���b�Z�[�W���x�O�`�S �S����

	// ���[�U�[�w�肳��Ă���΁A���̒l�ŏ㏑��
	if ( iTime > 0 ) { 
		RefleshTime = iTime;
	}

	// �����Ń}�E�X���͑҂�
	MSG msg;
	while(1) {
		BOOL ret = GetMessage( &msg, NULL, 0, 0 );  // ���b�Z�[�W���擾����

		// �Q�[���I���B
		if ( CommonGlobalHandle::isNobunaganoYabouGameEnd ) {
			break;
		}

		// �}�E�X���������B������
		if ( msg.message == WM_LBUTTONDOWN || msg.message == WM_RBUTTONDOWN ) {
			break; // ������
		}

		if ( iTime >= ImageDialogCloseTime::AutoClose ) { // AutoClose�������́A���Ԏw��
			DWORD curTimeGetTime = timeGetTime();
			// �K��̊Ԋu���Ԃ��߂���
			if ( int(curTimeGetTime - preTimeGetTime) > int(RefleshTime) ) {
				break;
			} 
		}

		// ���b�Z�[�W����������
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
}

int OpenPngDialog(char *szPngName, int posX, int posY, int iTime) {

	// �܂��͕\��
	BOOL ret = StartPngDialog(szPngName, posX, posY);
	if ( ret == FALSE ) { return FALSE; }

	_WaitPngDialog(iTime);

	// ���
	EndPngDialog();

	return TRUE;
}


// �O���Q�Ɨp
int WINAPI Extern_OpenImageDialog(char *szPictureName, int posX, int posY, int iTime ) {
	return OpenPngDialog( szPictureName, posX, posY, iTime);
}
int WINAPI Extern_StartImageDialog(char *szPictureName, int posX, int posY ) {
	return StartPngDialog( szPictureName, posX, posY);
}
void WINAPI Extern_EndImageDialog() {
	EndPngDialog();
}
//---------------------------------------------------------------------------------------------------------------

/*
 ���\�[�X����ǂݍ��ރ^�C�v
*/ 
 // ���\�[�X����摜��ǂݍ��ފ֐��ł�
Gdiplus::Bitmap *LoadImageFromResource(
    HMODULE hinst,  // �ǂݍ��݌��̃C���X�^���X�̃n���h���ł�
    LPCTSTR pszName,  // �ǂݍ��ރ��\�[�X�̖��O�ł�
    LPCTSTR pszType   // �ǂݍ��ރ��\�[�X�̎�ނł�
)
{
    // �w�肳�ꂽ���\�[�X��T���܂�
    HRSRC hRes = FindResource(hinst, pszName, pszType);
	if (hRes == NULL) {
        return NULL;
	}
    // ���\�[�X�̃T�C�Y���擾���܂�
    DWORD Size = SizeofResource(hinst, hRes);
	if (Size == 0) {
        return NULL;
	}

    // ���\�[�X��ǂݍ��݂܂�
    HGLOBAL hData = LoadResource(hinst, hRes);
	if (hData == NULL) {
        return NULL;
	}
    const void *pData = LockResource(hData);
	if (pData == NULL) {
        return NULL;
	}

    // ���\�[�X�̃f�[�^���R�s�[���܂�
    HGLOBAL hBuffer = GlobalAlloc(GMEM_MOVEABLE, Size);
	if (hBuffer == NULL) {
        return NULL;
	}
    void *pBuffer = GlobalLock(hBuffer);
    if (pBuffer == NULL) {
        GlobalFree(hBuffer);
        return NULL;
    }
    CopyMemory(pBuffer, pData, Size);
    GlobalUnlock(hBuffer);

    // �摜��ǂݍ��݂܂�
    IStream *pStream;
    if (CreateStreamOnHGlobal(hBuffer, TRUE, &pStream) != S_OK) {
        GlobalFree(hBuffer);
        return NULL;
    }
    Gdiplus::Bitmap *pPngImage = Gdiplus::Bitmap::FromStream(pStream);
    pStream->Release();

    return pPngImage;
}


static Gdiplus::Bitmap *pPngImage = NULL;
int StartPngResourceDialog(char *szResourceName, HMODULE hModule, int posX, int posY) {
    // �����R�[�h�����C�h������ɕϊ�
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
 
    //---- GDI+�̏����ݒ�
    if( Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL) !=  Gdiplus::Ok ) {
        return FALSE;
    }

 	// �E�B���h�E�̃f�o�C�X�R���e�L�X�g�n���h�����擾����
	hDC = GetDC( CommonGlobalHandle::tenshouWND );

	Graphics pngGraphics(hDC);

	Gdiplus::Bitmap *pPngImage = NULL;

    // �摜��ǂݍ��݂܂�
    pPngImage = LoadImageFromResource(hModule, szResourceName, "IMAGE");
    if (pPngImage == NULL)
        return -1;  // �摜�̓ǂݍ��ݎ��s

	// �^�񒆂����߂邽�߁B
	RECT rc; // �N���C�A���g�̈�̃T�C�Y
	::GetClientRect(CommonGlobalHandle::tenshouWND, &rc);

	int width = rc.right-rc.left;
	int height = rc.bottom-rc.top;

	iPngLeft = width/2 - pPngImage->GetWidth()/2;
	iPngTop = height/2 - pPngImage->GetHeight()/2;

	// ���W���w�肳��Ă���Ώ㏑��
	if ( posX != -1 ) {
		iPngLeft = posX;
	}
	if ( posY != -1 ) {
		iPngTop = posY;
	}

	pngGraphics.DrawImage(pPngImage, iPngLeft, iPngTop, pPngImage->GetWidth(), pPngImage->GetHeight());

	return TRUE;
}

void EndPngResourceDialog() {
	if ( pPngImage ) {
		delete pPngImage;
	}
	//---- GDI+�̉��
    Gdiplus::GdiplusShutdown(gdiplusToken);

	ReleaseDC( CommonGlobalHandle::tenshouWND, hDC );   // �E�B���h�E�̃f�o�C�X�R���e�L�X�g�n���h�����������
}

int OpenPngResourceDialog(char *szResourceName, HMODULE hModule, int posX, int posY, int iTime) {

	// �܂��͕\��
	BOOL ret = StartPngResourceDialog(szResourceName, hModule, posX, posY);
	if ( ret == FALSE ) { return FALSE; }

	_WaitPngDialog(iTime);

	// ���
	EndPngResourceDialog();

	return TRUE;
}

// �O���Q�Ɨp
int WINAPI Extern_OpenResourceImageDialog(char *szPictureName, HMODULE hModule, int posX, int posY, int iTime ) {
	return OpenPngResourceDialog( szPictureName, hModule, posX, posY, iTime);
}
int WINAPI Extern_StartResourceImageDialog(char *szPictureName, HMODULE hModule, int posX, int posY ) {
	return StartPngResourceDialog( szPictureName, hModule, posX, posY );
}
void WINAPI Extern_EndResourceImageDialog() {
	EndPngResourceDialog();
}

// �r���ŃQ�[�����A�I�������ꍇ�ł����
BOOL FreeImageDialog() {
	EndPngDialog();
	EndPngResourceDialog();

	return TRUE;
}