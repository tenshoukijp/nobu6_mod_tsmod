#include "WinTarget.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"
#include "ScenarioMod.h"
#include "DarkModeTitleBarMod.h"



// DDraw�I���W�i���n���h��
extern HINSTANCE hDDrawMod;

#define DDRAW_DLL_ENTRYPOINT_TO_WINPROC							 0x2DE0					// DDraw�̐擪����A��L�́uCMP EAX, 20(WM_SETCURSOR)�v�̎��̃W�����v�ɑ�������A�h���X(�J�[�\���ύX�̂���)

extern const char *DDRAW_DLL_FILENAME;





//-----------------------------------------------------------------------------------------------------------
int static uMsgOfDDrawDllWindowProcA = 0;
int static lParamOfDDrawDllWindowProcA = 0;
int static wParamOfDDrawDllWindowProcA = 0;
int static hWndOfDDrawDllWindowProcA = 0;


extern PFMDARKMODETITLEBAR_ONWMSIZE pDarkModeTitleBar_OnWmSize;

void OnTenshouExeDDrawDllWindowProcAExecute() {
	HWND hWnd = (HWND)hWndOfDDrawDllWindowProcA;
	UINT Msg = (UINT)uMsgOfDDrawDllWindowProcA;
	WPARAM wParam = (WPARAM)wParamOfDDrawDllWindowProcA;
	LPARAM lParam = (LPARAM)lParamOfDDrawDllWindowProcA;

	/*
	char szClass[512];
    GetClassName(  hWnd, szClass, sizeof(szClass) );    // �N���X������̎擾 �� Tenshouki�̃��C���E�B���h�E�Ȃ�hWnd�ƂȂ�͂��������
	*/

	if ( Msg == WM_COMMAND ) {
	// �����͗���B

	} else if ( Msg == WM_SYSCOMMAND ) {
	// �����͗��Ȃ��B

	} else if ( Msg == WM_MENUSELECT ) {
		// ���j���[��\�����Ȃ��B
		if ( TSModCommand::Environment::cmdNoTitleBarAndMenuBar && CommonGlobalHandle::IsAfterScreenSizeSelected ) {
			// ���j���[�\���ێ��^�C�}�[���㏑������B�T�b�ێ�
			SetTimer( CommonGlobalHandle::tenshouWND, TIMER_EVENT_ID_MENU_HIDE, 3000, TenshouMenuHide );
		}

	} else if ( Msg == WM_TIMER ) {
		// �����͗���B
		/*
		wParam
		wParam�p�����[�^�̓^�C�}�[ID�ł��B
		lParam
		lParam�p�����[�^�́A�^�C�}�[�o�^����SetTimer�֐��ɓn���ꂽ�A�v���P�[�V������`�̃R�[���o�b�N�֐��ւ̃|�C���^�ł��B
		*/

	} else if ( Msg == WM_PAINT ) {
		// �����͗���B
	}
	else if (Msg == WM_SIZE) {
		if (pDarkModeTitleBar_OnWmSize) {
			pDarkModeTitleBar_OnWmSize();
		}
		/*
		int iWidth = LOWORD(lParam); // ��
		if (iWidth >= 780) {
			// MessageBox(NULL, "WM_SETTINGCHANGE", "WM_SETTINGCHANGE", NULL);
		}
		*/


		// �����͗���B

		// �T�C�Y�ύX�̃^�C�v���w�肳��܂��B�ȉ��̒l�̂����ꂩ�ɂȂ�܂��B
		/*

		�l �Ӗ�

		0 (SIZE_RESTORED) 		�E�B���h�E���T�C�Y�ύX����܂����B�������ŏ����܂��͍ő剻�ł͂���܂���B
		1 (SIZE_MINIMIZED)		�E�B���h�E���ŏ�������܂����B
		2 (SIZE_MAXIMIZED)		�E�B���h�E���ő剻����܂����B
		3 (SIZE_MAXSHOW) 		���鑼�̃E�B���h�E�����̃T�C�Y�ɖ߂��ꂽ�Ƃ��A���ׂẴ|�b�v�A�b�v�E�B���h�E�ɑ����܂��B
		4 (SIZE_MAXHIDE) 		���鑼�̃E�B���h�E���ő剻���ꂽ�Ƃ��A���ׂẴ|�b�v�A�b�v�E�B���h�E�ɑ����܂��B

		WPARAM fwSizeType = wParam;   // �p�����[�^�̒l�B
		int iWidth  = LOWORD(lParam); // ��
		int iHeight = HIWORD(lParam); // ����
		*/
	} else if ( Msg == WM_SETTINGCHANGE) {

	} else if ( Msg == WM_LBUTTONDOWN ) {
		// �����͗���
	}
}

/*
LRESULT Hook_DefWindowProcACustom(
	HWND hWnd,      // �E�B���h�E�̃n���h��
	UINT Msg,       // ���b�Z�[�W�̎��ʎq
	WPARAM wParam,  // ���b�Z�[�W�̍ŏ��̃p�����[�^
	LPARAM lParam   // ���b�Z�[�W�� 2 �Ԗڂ̃p�����[�^
)
*/

/*
52982DDF   53               PUSH    EBX


52982DE0   56               PUSH    ESI
52982DE1   8B75 10          MOV     ESI, DWORD PTR SS:[EBP+10]
52982DE4   57               PUSH    EDI
				�����̂R�s�� JMP TSMod.OnTSExeDDrawDllWindowProcA �Ə��������Ă����ۂ̏���
52982DE5   83F9 7E          CMP     ECX, 7E
*/
int pTenshouExeJumpFromToOnTenshouExeDDrawDllWindowProcA = NULL; // DDraw���ǂݍ��܂�Ă���ꏊ�v�Z
int pTenshouExeReturnLblFromOnTenshouExeDDrawDllWindowProcA = NULL; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeDDrawDllWindowProcA() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// DDraw.dll�Ɍ��X�������������L�q
		PUSH ESI
		MOV ESI, DWORD PTR SS:[EBP+0x10]
		PUSH EDI

		mov wParamOfDDrawDllWindowProcA, ESI // wParam�̃R�s�[

		mov uMsgOfDDrawDllWindowProcA, ECX   // uMsg�̃R�s�[

		mov lParamOfDDrawDllWindowProcA, EDX // lParam�̃R�s�[

		mov EDX, DWORD PTR SS:[EBP+0x8]		// hWnd��EDX�ɏ㏑������B
		mov hWndOfDDrawDllWindowProcA, EDX // hWnd�̃R�s�[

		mov EDX, lParamOfDDrawDllWindowProcA // EDX�̒l�����ւƖ߂��B

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnTenshouExeDDrawDllWindowProcAExecute();

	// �X�^�b�N�Ɉ����o��
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		jmp pTenshouExeReturnLblFromOnTenshouExeDDrawDllWindowProcA
	}
}



char cmdOnTenshouExeJumpFromDDrawDllWindowProcA[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeDDrawDllWindowProcA() {

	pTenshouExeJumpFromToOnTenshouExeDDrawDllWindowProcA = (int)hDDrawMod + DDRAW_DLL_ENTRYPOINT_TO_WINPROC; // DDraw�̐擪����AWinProc�ɑ�������֐��̃A�h���X
	pTenshouExeReturnLblFromOnTenshouExeDDrawDllWindowProcA = pTenshouExeJumpFromToOnTenshouExeDDrawDllWindowProcA + 5; // ���^�[����͂T�o�C�g��
	// �J�[�\����ύX����̂���߂�

	int iAddress = (int)OnTenshouExeDDrawDllWindowProcA;

	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeDDrawDllWindowProcA + 5 )	;

	// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeDDrawDllWindowProcA  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromDDrawDllWindowProcA+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeDDrawDllWindowProcA), cmdOnTenshouExeJumpFromDDrawDllWindowProcA, 5, NULL); //5�o�C�g�̂ݏ�������
}

