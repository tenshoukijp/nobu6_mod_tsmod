#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"

/*
 * �퓬���Ƀ��j�b�g���ړ����������̂��ǂ����̂��𔻒肷�邽�߂̊֐����Ăяo�������̃^�C�~���O
 */

// �S�Ă̕����̎c��s����(�e�X�̕�����������s���p�̎c��J�E���g�������Ă���)�B
// �퓬���Ɏc��^�[�����ω�����ƁA�Ăу`���[�W�����B
int arrRemainMoveCountAllBushou[GAMEDATASTRUCT_BUSHOU_NUM] = {};

// �ړ������������̃`�F�b�N�Ώۂ̕�����ID
int iMoveEndJudgeInBattleBushouID = -1;

// ������ۂɍs�����I���邩�I���Ȃ���
int iMoveEndJudgeInBattle = 1;

void OnTenshouExeMoveEndJudgeInBattleExecute() {

	// ����ID�͈̔̓`�F�b�N
	if ( 0 <= iMoveEndJudgeInBattleBushouID-1 && iMoveEndJudgeInBattleBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ������ID���㋉�E�ł���΁c  �������� �����X���b�g�E�^���^���X�ł���΁c �������͎�ɂȂ�΁c
		if ( nb6bushouref[iMoveEndJudgeInBattleBushouID-1].job >= 0xA0 || isLancelotTartare(iMoveEndJudgeInBattleBushouID-1) || isShuiHidechiyo(iMoveEndJudgeInBattleBushouID - 1)) {

			if ( arrRemainMoveCountAllBushou[iMoveEndJudgeInBattleBushouID-1] >= 1 ) {
				// �ړ��͂܂��\�Ƃ���B
				iMoveEndJudgeInBattle = 0;
			}
		}
	}
}


/*
0046AD22  |. E8 8332FCFF    CALL    TENSHOU.0042DFAA		�ړ��ςł���΁A����CALL�ɂ����EAX�ɂP�ȏ�̒l������B�ړ����ĂȂ����0
				�������� JMP TSMod.OnTSExeMoveEndJudgeInBattle �Ə��������Ă����ۂ̏���
0046AD27  |. 83C4 04        ADD     ESP, 4
0046AD2A     85C0           TEST    EAX, EAX
0046AD2C     0F85 8E020000  JNZ     TENSHOU.0046AFC0
 */
int pTenshouExeJumpFromToOnTenshouExeMoveEndJudgeInBattle    =0x46AD22; // �֐��͂��̃A�h���X����AOnTenshouExeMoveEndJudgeInBattle�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeMoveEndJudgeInBattle=0x42DFAA; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeMoveEndJudgeInBattle =0x46AD27; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeMoveEndJudgeInBattle() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// �ړ������������̃`�F�b�N�Ώۂ̕�����ID���R�s�[
		mov iMoveEndJudgeInBattleBushouID, esi

		// ���XTENSHOU.EXE���ɂ��������̂�������		�ړ��ςł���΁A����CALL�ɂ����EAX�ɂP�ȏ�̒l������B�ړ����ĂȂ����0
		call pTenshouExeJumpCallFromToOnTenshouExeMoveEndJudgeInBattle

		// �s�����������Ă��邩�ǂ����BEAX�ɂP�ȏ�̒l������B�ړ����ĂȂ����0
		mov iMoveEndJudgeInBattle, eax

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	//
	OnTenshouExeMoveEndJudgeInBattleExecute();

	// �ړ��������Ă���Ȃ�Ε��i�ǂ���B�������Ȃ��B
	if ( iMoveEndJudgeInBattle ) {
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

			jmp pTenshouExeReturnLblFromOnTenshouExeMoveEndJudgeInBattle
		}

	// �ړ������𓾂Ă��Ȃ��Ƃ�������(�ړ����𓾂Ă��Ȃ��Ƃ����㏑������������K�v������ꍇ���܂�)�͂�����
	} else {
		__asm {
			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			mov eax, 0		// 0�Ȃ�Έړ��͂��𓾂Ă��Ȃ��B

			jmp pTenshouExeReturnLblFromOnTenshouExeMoveEndJudgeInBattle
		}
	}
}

/*
00444769  |. 50             PUSH    EAX
0044476A  |. E8 3A2C0200    CALL    TENSHOU.004673A9
				�������� JMP TSMod.OnTSExeMoveEndJudgeInBattle �Ə��������Ă����ۂ̏���
0044476F  |. 83C4 04        ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromMoveEndJudgeInBattle[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeMoveEndJudgeInBattle() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeMoveEndJudgeInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeMoveEndJudgeInBattle + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeMoveEndJudgeInBattle  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromMoveEndJudgeInBattle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeMoveEndJudgeInBattle), cmdOnTenshouExeJumpFromMoveEndJudgeInBattle, 5, NULL); //5�o�C�g�̂ݏ�������
}




