#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"

/*
0040C1FD  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0040C201     E8 EEE5FFFF    CALL    TENSHOU.0040A7F4
0040C206  |. 83C4 04        ADD     ESP, 4
0040C209  |. 66:3D FFFF     CMP     AX, 0FFFF
0040C20D  |. 74 0D          JE      SHORT TENSHOU.0040C21C
0040C20F  |. FF7424 08      PUSH    DWORD PTR SS:[ESP+8]
0040C213  |. 50             PUSH    EAX
0040C214     E8 E9190200    CALL    TENSHOU.0042DC02
0040C219  |. 83C4 08        ADD     ESP, 8
0040C21C  \> C3             RETN
*/

/*
 * �퓬���Ƀ��j�b�g�̃A�j���[�V�����̂n�m�E�n�e�e���؂�ւ��^�C�~���O
 */

// �s�����I����������ID
int iMoveUnitAnimationChangeInBattleBushouID = -1;

// ������ۂɍs�����I���邩�I���Ȃ���
int iMoveUnitAnimationChangeInBattle = 1;

// �e�X�̕����́A����̃^�[���ł̎c��s����
extern int arrRemainMoveCountAllBushou[GAMEDATASTRUCT_BUSHOU_NUM];

void OnTenshouExeMoveUnitAnimationChangeInBattleExecute() {

	// �f�t�H���g�͍s�������������A�����ƃA�j���[�V�����͐؂�ւ��
	iMoveUnitAnimationChangeInBattle = 1;

	// ����ID�͈̔̓`�F�b�N
	if ( 0 <= iMoveUnitAnimationChangeInBattleBushouID-1 && iMoveUnitAnimationChangeInBattleBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ������ID���㋉�E�ł���΁c �������� �����X���b�g�E�^���^���X�ł���΁c �������͎�ɂȂ�΁c
		if ( nb6bushouref[iMoveUnitAnimationChangeInBattleBushouID-1].job >= 0xA0 || isLancelotTartare(iMoveUnitAnimationChangeInBattleBushouID-1) || isShuiHidechiyo(iMoveUnitAnimationChangeInBattleBushouID - 1)) {

			// ����̃^�[���̍s���̎c��J�E���g����1�ȉ��Ȃ�A�A�j���[�V������؂�ւ���(������~�A�j���[�V�����ɂ���)
			if ( arrRemainMoveCountAllBushou[iMoveUnitAnimationChangeInBattleBushouID-1] <= 1 ) {

				// �A�j���[�V������؂�ւ���
				iMoveUnitAnimationChangeInBattle = 1;

			} else {
				// ����̓A�j���[�V�������~�߂Ȃ��B�ēx�s���o���邽�߁B���j�b�g����ɂ�ɂƓ����A�j���[�V�������ێ�����
				iMoveUnitAnimationChangeInBattle = 0;
			}
		}
	}

}


/*
0040C201     E8 EEE5FFFF    CALL    TENSHOU.0040A7F4
				�������� JMP TSMod.OnTSExeMoveUnitAnimationChangeInBattle �Ə��������Ă����ۂ̏���
0040C206  |. 83C4 04        ADD     ESP, 4
0040C209  |. 66:3D FFFF     CMP     AX, 0FFFF
 */
int pTenshouExeJumpFromToOnTenshouExeMoveUnitAnimationChangeInBattle    =0x40C201; // �֐��͂��̃A�h���X����AOnTenshouExeMoveUnitAnimationChangeInBattle�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeMoveUnitAnimationChangeInBattle=0x40A7F4; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeMoveUnitAnimationChangeInBattle =0x40C206; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeMoveUnitAnimationChangeInBattle() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// �A�j���[�V�����؂�ւ�������s��������ID���R�s�[
		mov iMoveUnitAnimationChangeInBattleBushouID, esi

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
	OnTenshouExeMoveUnitAnimationChangeInBattleExecute();

	// �A�j���[�V�������~�߂�̂ł���΁c
	if ( iMoveUnitAnimationChangeInBattle ) {
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
			call pTenshouExeJumpCallFromToOnTenshouExeMoveUnitAnimationChangeInBattle

			jmp pTenshouExeReturnLblFromOnTenshouExeMoveUnitAnimationChangeInBattle
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

			// �A�j���[�V������~�֐��͌Ăяo���Ȃ��A���^�[�����Ă��܂�
			add esp, 4

			retn
		}
	}
}

/*
00444769  |. 50             PUSH    EAX
0044476A  |. E8 3A2C0200    CALL    TENSHOU.004673A9
				�������� JMP TSMod.OnTSExeMoveUnitAnimationChangeInBattle �Ə��������Ă����ۂ̏���
0044476F  |. 83C4 04        ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromMoveUnitAnimationChangeInBattle[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeMoveUnitAnimationChangeInBattle() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeMoveUnitAnimationChangeInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeMoveUnitAnimationChangeInBattle + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeMoveUnitAnimationChangeInBattle  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromMoveUnitAnimationChangeInBattle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeMoveUnitAnimationChangeInBattle), cmdOnTenshouExeJumpFromMoveUnitAnimationChangeInBattle, 5, NULL); //5�o�C�g�̂ݏ�������
}




