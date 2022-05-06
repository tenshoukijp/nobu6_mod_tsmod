#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "ScenarioMod.h"


/*
 * ���E�m�F�E�x���Ȃǂ́u�ǔF�n�v�_�C�A���O�́u�m�F�v�{�^������������(�{�^�����Ȃ����̂��܂�)
 */

/*
004A17D1  |. FF75 10        PUSH    DWORD PTR SS:[EBP+10]            ; |Arg4
004A17D4  |. 56             PUSH    ESI                              ; |Arg3
004A17D5  |. FF75 0C        PUSH    DWORD PTR SS:[EBP+C]             ; |Arg2	�� �^�C�g��
004A17D8  |. FF75 08        PUSH    DWORD PTR SS:[EBP+8]             ; |Arg1	�� ���b�Z�[�W
004A17DB  |. E8 89F7FFFF    CALL    TENSHOU.004A0F69                 ; \TENSHOU.004A0F69
004A17E0  |. 85F6           TEST    ESI, ESI
 */

int iPointerOfTitleOnConfirmRatificationDialog = NULL; // �^�C�g���̕ۑ��p
int iPointerOfMsgConfirmRatificationDialog = NULL; // ���b�Z�[�W�̕ۑ��p

int iSaveOfEAXConfirmRatificationDialog = -1; // ��U�ۑ��p

void OnTenshouExeConfirmRatificationDialogExecute() {
	if ( p_snOnRatificationDialog ) {
		char *pszTitle = (char *)iPointerOfTitleOnConfirmRatificationDialog;
		char *pszMessage = (char *)iPointerOfMsgConfirmRatificationDialog;
		p_snOnRatificationDialog( pszMessage, pszTitle );
	}
}



/*
004A17D1  |. FF75 10        PUSH    DWORD PTR SS:[EBP+10]            ; |Arg4
004A17D4  |. 56             PUSH    ESI                              ; |Arg3
004A17D5  |. FF75 0C        PUSH    DWORD PTR SS:[EBP+C]             ; |Arg2	�� �^�C�g��
004A17D8  |. FF75 08        PUSH    DWORD PTR SS:[EBP+8]             ; |Arg1	�� ���b�Z�[�W
004A17DB  |. E8 89F7FFFF    CALL    TENSHOU.004A0F69                 ; \TENSHOU.004A0F69
				�������� JMP TSMod.OnTSExeConfirmRatificationDialog �Ə��������Ă����ۂ̏���
004A17E0  |. 85F6           TEST    ESI, ESI
*/
int pTenshouExeJumpFromToOnTenshouExeConfirmRatificationDialog		=0x4A17DB; // �֐��͂��̃A�h���X����AOnTenshouExeConfirmRatificationDialog�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeConfirmRatificationDialog  =0x4A0F69; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeConfirmRatificationDialog	=0x4A17E0; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeConfirmRatificationDialog() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iSaveOfEAXConfirmRatificationDialog, eax // ��U�ۑ�

		mov eax, DWORD PTR SS:[EBP+0xC]						// �^�C�g���̕ۑ�
		mov iPointerOfTitleOnConfirmRatificationDialog, eax

		mov eax,  DWORD PTR SS:[EBP+0x8]
		mov iPointerOfMsgConfirmRatificationDialog, eax		// ���b�Z�[�W�̕ۑ�

		mov eax, iSaveOfEAXConfirmRatificationDialog		// EAX�����ւƖ߂�

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeConfirmRatificationDialog

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// �Q�[���f�[�^���v���C���[�^�[�����ς�����i�K�œǂݍ��ݍX�V���Ă���
	OnTenshouExeConfirmRatificationDialogExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeConfirmRatificationDialog
	}
}

char cmdOnTenshouExeJumpFromConfirmRatificationDialog[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeConfirmRatificationDialog() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeConfirmRatificationDialog;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeConfirmRatificationDialog + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeConfirmRatificationDialog  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromConfirmRatificationDialog+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeConfirmRatificationDialog), cmdOnTenshouExeJumpFromConfirmRatificationDialog, 5, NULL); //5�o�C�g�̂ݏ�������
}


