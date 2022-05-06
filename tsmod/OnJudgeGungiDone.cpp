#include "WinTarget.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"


/*
  ���G�߁A���łɌR�c���s�������ǂ����̔���
/*
0041B267  |. E8 ACA20800    CALL    TENSHOU.004A5518
0041B26C  |. 83C4 04        ADD     ESP, 4
0041B26F  |. 85C0           TEST    EAX, EAX			�� ����EAX��1�Ȃ�΁A���łɌR�c���s����
*/

// ���G�ߌR�c���s�������ǂ������㏑������t���O�p
int isOverFlagOnJudgeGungiDone = 0L;

void OnTenshouExeJudgeGungiDoneExecute() {
	// ��G�߂ɕ�����̕]�肪�\�ł����
	if ( TSModCommand::MyselfDaimyo::cmdCanMultiHyoujoInSeason ) {
		// �S���喼���v���C���[�喼�ł����
		if ( isPlayerDaimyo(nb6turn.daimyo-1) ) {
			// �]��ς݃t���O���ꎞ�I�ɋU��
			isOverFlagOnJudgeGungiDone = 0;
		}
	}
}



/*
0041B267  |. E8 ACA20800    CALL    TENSHOU.004A5518
				�������� JMP TSMod.OnTSExeJudgeGungiDone �Ə��������Ă����ۂ̏���
0041B26C  |. 83C4 04        ADD     ESP, 4
0041B26F  |. 85C0           TEST    EAX, EAX			�� ����EAX��1�Ȃ�΁A���łɌR�c���s����
0041B271  |. 74 41          JE      SHORT TENSHOU.0041B2B4
*/
int pTenshouExeJumpFromToOnTenshouExeJudgeGungiDone	   =0x41B267; // �֐��͂��̃A�h���X����AOnTenshouExeJudgeGungiDone�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeJudgeGungiDone =0x4A5518; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeJudgeGungiDone  =0x41B271; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeJudgeGungiDone() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeJudgeGungiDone

		mov isOverFlagOnJudgeGungiDone, eax    // ���G�ߌR�c���s�������ǂ����̌��ʃt���O��ۑ����Ă����B

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnTenshouExeJudgeGungiDoneExecute();

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

		mov eax, isOverFlagOnJudgeGungiDone // ���G�ߌR�c���s�������ǂ����̌��ʃt���O���㏑������B

		ADD     ESP, 4				// TENSHOU.EXE�Ɍ��X����������
		TEST    EAX, EAX			// TENSHOU.EXE�Ɍ��X����������

		jmp pTenshouExeReturnLblFromOnTenshouExeJudgeGungiDone
	}
}



char cmdOnTenshouExeJumpFromJudgeGungiDone[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeJudgeGungiDone() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeJudgeGungiDone;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeJudgeGungiDone + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeJudgeGungiDone  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromJudgeGungiDone+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeJudgeGungiDone), cmdOnTenshouExeJumpFromJudgeGungiDone, 5, NULL); //5�o�C�g�̂ݏ�������
}


