#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"

/*
 * �퓬���Ƀ��j�b�g���ړ��������ƁA�A�j���[�V�������~�߂�Ƃ����ݒ������֐����Ăяo���^�C�~���O�ł̏���
 */

// �s�����I����������ID
int iMoveEndAnimationInBattleBushouID = -1;

// ������ۂɍs�����I���邩�I���Ȃ���
int iMoveEndAnimationInBattle = 1;

// �e�X�̕����́A����̃^�[���ł̎c��s����
extern int arrRemainMoveCountAllBushou[GAMEDATASTRUCT_BUSHOU_NUM];

void OnTenshouExeMoveEndAnimationInBattleExecute() {

	// �f�t�H���g�͍s�������������A�����ƃA�j���[�V�����͎~�܂�(�s���I���A�j���[�V�����ƂȂ�)
	iMoveEndAnimationInBattle = 1;

	// ����ID�͈̔̓`�F�b�N
	if ( 0 <= iMoveEndAnimationInBattleBushouID-1 && iMoveEndAnimationInBattleBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ������ID���㋉�E�ł���΁c �������� �����X���b�g�E�^���^���X�ł���΁c �������͎�ɂȂ�΁c
		if ( nb6bushouref[iMoveEndAnimationInBattleBushouID-1].job >= 0xA0 || isLancelotTartare(iMoveEndAnimationInBattleBushouID-1) || isShuiHidechiyo(iMoveEndAnimationInBattleBushouID - 1)) {

			// ����̃^�[���̍s���̎c��J�E���g����1�ȏ�
			if ( arrRemainMoveCountAllBushou[iMoveEndAnimationInBattleBushouID-1] <= 0 ) {

				// �A�j���[�V�������~�߂�
				iMoveEndAnimationInBattle = 1;

			} else {
				// ����̓A�j���[�V�������~�߂Ȃ��B�ēx�s���o���邽�߁B
				iMoveEndAnimationInBattle = 0;
			}
		}
	}

}


/*
0046AFAC     E8 4C12FAFF    CALL    TENSHOU.0040C1FD
				�������� JMP TSMod.OnTSExeMoveEndAnimationInBattle �Ə��������Ă����ۂ̏���
0046AFB1  |. 83C4 08        ADD     ESP, 8
0046AFB4  |> 6A 01          PUSH    1
0046AFB6  |. B9 78E84D00    MOV     ECX, TENSHOU.004DE878
 */
int pTenshouExeJumpFromToOnTenshouExeMoveEndAnimationInBattle    =0x46AFAC; // �֐��͂��̃A�h���X����AOnTenshouExeMoveEndAnimationInBattle�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeMoveEndAnimationInBattle=0x40C1FD; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeMoveEndAnimationInBattle =0x46AFB1; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeMoveEndAnimationInBattle() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// �ړ���������������ID���R�s�[
		mov iMoveEndAnimationInBattleBushouID, ecx

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
	OnTenshouExeMoveEndAnimationInBattleExecute();

	// �A�j���[�V�������~�߂�̂ł���΁c
	if ( iMoveEndAnimationInBattle ) {
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

			// ���XTENSHOU.EXE���ɂ��������̂�������(�A�j���[�V�������~�߂�[�Ƃ������s�������I������V�O�i����ݒ肷��]�֐����Ă�)
			call pTenshouExeJumpCallFromToOnTenshouExeMoveEndAnimationInBattle

			jmp pTenshouExeReturnLblFromOnTenshouExeMoveEndAnimationInBattle
		}

	// �A�j���[�V�������~�߂�̂ł���΁c
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

			;	// �A�j���[�V������~�����͌Ăяo���Ȃ�

			jmp pTenshouExeReturnLblFromOnTenshouExeMoveEndAnimationInBattle
		}
	}
}

/*
00444769  |. 50             PUSH    EAX
0044476A  |. E8 3A2C0200    CALL    TENSHOU.004673A9
				�������� JMP TSMod.OnTSExeMoveEndAnimationInBattle �Ə��������Ă����ۂ̏���
0044476F  |. 83C4 04        ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromMoveEndAnimationInBattle[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeMoveEndAnimationInBattle() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeMoveEndAnimationInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeMoveEndAnimationInBattle + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeMoveEndAnimationInBattle  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromMoveEndAnimationInBattle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeMoveEndAnimationInBattle), cmdOnTenshouExeJumpFromMoveEndAnimationInBattle, 5, NULL); //5�o�C�g�̂ݏ�������
}




