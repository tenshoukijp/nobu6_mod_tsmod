#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "ScenarioMod.h"

/*
 * ���l���̓_�C�A���O�A���s�{�^�����莞
 */
int iMinOfInputNumberDialogExecute = -1;	// �ŏ��l
int iMaxOfInputNumberDialogExecute = -1;	// �ő�l
int iValueOfInputNumberDialogExecute = -1;	// ���ꂽ�l
int iPointerOfMessageOfInputNumberDialogExecute = -1; // ���b�Z�[�W�̃|�C���^

void OnTenshouExeInputNumberDialogExecute() {
	// ���b�Z�[�W�̓��e
	char *pMessage = (char *)iPointerOfMessageOfInputNumberDialogExecute;

	// �V�i���IMod�̌Ăяo��
	if ( p_snOnDecisionInputNumberDialog ) {

		���l���̓_�C�A���O�p�����^�^ param;
		param.�ŏ��l = iMinOfInputNumberDialogExecute;
		param.�ő�l = iMaxOfInputNumberDialogExecute;
		param.���͒l = iValueOfInputNumberDialogExecute;

		p_snOnDecisionInputNumberDialog( pMessage, &param);
	}

}


/*
004A39D8  |. FF75 10        PUSH    DWORD PTR SS:[EBP+10]            ; /Arg3
004A39DB  |. 57             PUSH    EDI                              ; |Arg2		���͂̍ő�l
004A39DC  |. 56             PUSH    ESI                              ; |Arg1		���͂̍ŏ��l
004A39DD  |. 8D8D F8FEFFFF  LEA     ECX, DWORD PTR SS:[EBP-108]      ; |
004A39E3  |. E8 F3F6FFFF    CALL    TENSHOU.004A30DB                 ; \TENSHOU.004A30DB	���̊֐��̌���(EAX)�����͂��Č��肵���l
004A39E8  |. E8 EA7EFDFF    CALL    TENSHOU.0047B8D7
				�������� JMP TSMod.OnTSExeInputNumberDialog �Ə��������Ă����ۂ̏���
004A39ED  |. 8D8D F8FEFFFF  LEA     ECX, DWORD PTR SS:[EBP-108]
*/
int pTenshouExeJumpFromToOnTenshouExeInputNumberDialog		=0x4A39E8; // �֐��͂��̃A�h���X����AOnTenshouExeInputNumberDialog�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeInputNumberDialog  =0x47B8D7; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeInputNumberDialog	=0x4A39ED; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeInputNumberDialog() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		mov iMinOfInputNumberDialogExecute, esi		// �ŏ��l�̕ۑ�
		mov iMaxOfInputNumberDialogExecute, edi		// �ő�l�̕ۑ�

		mov edi, DWORD PTR SS:[EBP+0x10]				// ���b�Z�[�W�̓��e�̃|�C���^��edi�ɕۑ�
		mov iPointerOfMessageOfInputNumberDialogExecute, edi // �ϐ��ɕۑ�
		mov edi, iMaxOfInputNumberDialogExecute		// edi�̒l�����ւƖ߂��B


		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeInputNumberDialog

		mov iValueOfInputNumberDialogExecute, eax

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
	OnTenshouExeInputNumberDialogExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeInputNumberDialog
	}
}

char cmdOnTenshouExeJumpFromInputNumberDialog[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeInputNumberDialog() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeInputNumberDialog;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeInputNumberDialog + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeInputNumberDialog  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromInputNumberDialog+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeInputNumberDialog), cmdOnTenshouExeJumpFromInputNumberDialog, 5, NULL); //5�o�C�g�̂ݏ�������
}


