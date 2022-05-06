#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "ScenarioMod.h"

#include "FuncGundanCastleWindowRepaint.h"

/*
0041050B  |> 6A 00          /PUSH    0
0041050D  |. E8 C4780700    |CALL    TENSHOU.00487DD6
00410512  |. 83C4 04        |ADD     ESP, 4
00410515  |. E8 BDB30600    |CALL    TENSHOU.0047B8D7				�� �v���C�������C����ʂœ��ɉ������͂��Ă��炸�A�T���Ă����ԁi���C���R�}���h�T�����O�����!!)
0041051A  |. 8BD8           |MOV     EBX, EAX
0041051C  |. 85DB           |TEST    EBX, EBX
0041051E  |. 74 2E          |JE      SHORT TENSHOU.0041054E
*/

extern int iEnterOfClickMainScreenMainCommand;
extern void OnLeaveMainPlayerCommand();

void OnTenshouExePrevMainPlayerCommandWaitExecute() {
	if ( iEnterOfClickMainScreenMainCommand ) {
		// ����̃R�}���h���s������o����
		OnLeaveMainPlayerCommand();
		iEnterOfClickMainScreenMainCommand = 0;

		// ���╺�ȂǑ��삳���\��������̂ŁA�R�c�E�B���h�E�{��E�B���h�E��`�悵�Ȃ���
		FuncGundanCastleWindowRepaint();
	}
}


/*

0041064C  |. E8 F6030000    |CALL    TENSHOU.00410A47	�� ���C����ʂ̏�E�B���h�E�ƌR�c�E�B���h�E�̍X�V
00410651  |. 56             |PUSH    ESI
00410652  |. E8 B7FC0500    |CALL    TENSHOU.0047030E
00410657  |. 83C4 04        |ADD     ESP, 4
0041065A  |. E8 87210400    |CALL    TENSHOU.004527E6
0041065F  |> 56             |PUSH    ESI
00410660  |. E8 783D0400    |CALL    TENSHOU.004543DD
00410665  |. 83C4 04        |ADD     ESP, 4
00410668  |. 85C0           |TEST    EAX, EAX
0041066A  |.^0F84 9BFEFFFF  \JE      TENSHOU.0041050B


0041050B  |> 6A 00          /PUSH    0
0041050D  |. E8 C4780700    |CALL    TENSHOU.00487DD6
00410512  |. 83C4 04        |ADD     ESP, 4
00410515  |. E8 BDB30600    |CALL    TENSHOU.0047B8D7				�� �v���C�������C����ʂœ��ɉ������͂��Ă��炸�A�T���Ă����ԁi���C���R�}���h�T�����O�����!!)
				�������� JMP TSMod.OnTSExePrevMainPlayerCommandWait �Ə��������Ă����ۂ̏���
0041051A  |. 8BD8           |MOV     EBX, EAX
0041051C  |. 85DB           |TEST    EBX, EBX
0041051E  |. 74 2E          |JE      SHORT TENSHOU.0041054E
*/

int pTenshouExeJumpFromToOnTenshouExePrevMainPlayerCommandWait		=0x410515; // �֐��͂��̃A�h���X����AOnTenshouExePrevMainPlayerCommandWait�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExePrevMainPlayerCommandWait  =0x47B8D7; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExePrevMainPlayerCommandWait	=0x41051A; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExePrevMainPlayerCommandWait() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

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
	OnTenshouExePrevMainPlayerCommandWaitExecute();

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

		mov CommonGlobalHandle::isPlayerMainScreenInputWait, 1

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExePrevMainPlayerCommandWait

		mov CommonGlobalHandle::isPlayerMainScreenInputWait, 0

		jmp pTenshouExeReturnLblFromOnTenshouExePrevMainPlayerCommandWait
	}
}

char cmdOnTenshouExeJumpFromPrevMainPlayerCommandWait[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExePrevMainPlayerCommandWait() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExePrevMainPlayerCommandWait;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExePrevMainPlayerCommandWait + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExePrevMainPlayerCommandWait  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromPrevMainPlayerCommandWait+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExePrevMainPlayerCommandWait), cmdOnTenshouExeJumpFromPrevMainPlayerCommandWait, 5, NULL); //5�o�C�g�̂ݏ�������
}


