#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


/*
 * �L���ȃo�g���U�������s�������B���̏����ɗ���B
 * �I�����ăL�����Z����������A���ʂ��Ƃ��Ȃ�Ȃ��ƌ��肵�Ă��鎞(���łɕ��mMAX�Ȃ̂ɒ������悤�Ƃ����Ȃ�)�́Aeax��0�������͒ʉ߂��Ȃ��B
 */

// �L���ȃR�}���h�����s��������ID
int iExecuteBattleAttackInBattleBushouID = -1;

// �L���ȃR�}���h�����s�������ǂ���
int iExecuteBattleAttackInBattleValidCommand = -1;

// �e�����̂P�^�[��������̎c����s��
extern int arrRemainMoveCountAllBushou[GAMEDATASTRUCT_BUSHOU_NUM];

void OnTenshouExeExecuteBattleAttackInBattleExecute() {

	// �����͈̓`�F�b�N
	if ( 0 <= iExecuteBattleAttackInBattleBushouID-1 && iExecuteBattleAttackInBattleBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// �L���ȃR�}���h�����s���ꂽ�̂ł���΁c
		if ( iExecuteBattleAttackInBattleValidCommand ) {
		}
	}
}


/*
0046AECE  |. E8 ADFAFFFF    |CALL    TENSHOU.0046A980                ; \TENSHOU.0046A980
0046AED3  |. 83C4 04        |ADD     ESP, 4
				�������� JMP TSMod.OnTSExeExecuteBattleAttackInBattle �Ə��������Ă����ۂ̏���
0046AED6  |. 85C0           |TEST    EAX, EAX						EAX���P�ł�������A�Ȃ�炩�̍U���s�������ۂɍs�����Ƃ������ƁB
 */
int pTenshouExeJumpFromToOnTenshouExeExecuteBattleAttackInBattle    =0x46AECE; // �֐��͂��̃A�h���X����AOnTenshouExeExecuteBattleAttackInBattle�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeExecuteBattleAttackInBattle=0x46A980; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeExecuteBattleAttackInBattle =0x46AED3; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeExecuteBattleAttackInBattle() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// �R�}���h�����s��������ID���R�s�[
		mov iExecuteBattleAttackInBattleBushouID, esi

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeExecuteBattleAttackInBattle

		// �L���ȃR�}���h�����s�����Ȃ�Aeax�ɗL���Ȓl�������Ă���Beax��0�Ȃ�΁A�R�}���h�͎��s���ꂽ�Ƃ͂����Ȃ��B
		mov iExecuteBattleAttackInBattleValidCommand, eax;

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
	OnTenshouExeExecuteBattleAttackInBattleExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeExecuteBattleAttackInBattle
	}
}

/*
00444769  |. 50             PUSH    EAX
0044476A  |. E8 3A2C0200    CALL    TENSHOU.004673A9
				�������� JMP TSMod.OnTSExeExecuteBattleAttackInBattle �Ə��������Ă����ۂ̏���
0044476F  |. 83C4 04        ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromExecuteBattleAttackInBattle[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeExecuteBattleAttackInBattle() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeExecuteBattleAttackInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeExecuteBattleAttackInBattle + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeExecuteBattleAttackInBattle  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromExecuteBattleAttackInBattle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeExecuteBattleAttackInBattle), cmdOnTenshouExeJumpFromExecuteBattleAttackInBattle, 5, NULL); //5�o�C�g�̂ݏ�������
}




