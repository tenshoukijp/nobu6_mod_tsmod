#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


/*
 * �L���ȃo�g���R�}���h�����s�������B���̏����ɗ���B
 * �I�����ăL�����Z����������A���ʂ��Ƃ��Ȃ�Ȃ��ƌ��肵�Ă��鎞(���łɕ��mMAX�Ȃ̂ɒ������悤�Ƃ����Ȃ�)�́A�����͒ʉ߂��Ȃ��B
 */

// �L���ȃR�}���h�����s��������ID
int iExecuteBattleCommandInBattleBushouID = -1;

// �L���ȃR�}���h�����s�������ǂ���
int iExecuteBattleCommandInBattleValidCommand = -1;

// �e�����̂P�^�[��������̎c����s��
extern int arrRemainMoveCountAllBushou[GAMEDATASTRUCT_BUSHOU_NUM];

void OnTenshouExeExecuteBattleCommandInBattleExecute() {
	// �����͈̓`�F�b�N
	if ( 0 <= iExecuteBattleCommandInBattleBushouID-1 && iExecuteBattleCommandInBattleBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// �L���ȃR�}���h�����s���ꂽ�̂ł���΁c
		if ( iExecuteBattleCommandInBattleValidCommand ) {
		}
	}
}


/*
0046AE59  |. E8 71F9FFFF    |CALL    TENSHOU.0046A7CF
				�������� JMP TSMod.OnTSExeExecuteBattleCommandInBattle �Ə��������Ă����ۂ̏���
0046AE5E  |. 85C0           |TEST    EAX, EAX
0046AE60  |. 0F85 4E010000  |JNZ     TENSHOU.0046AFB4
0046AE66  |.^E9 02FFFFFF    |JMP     TENSHOU.0046AD6D
 */
int pTenshouExeJumpFromToOnTenshouExeExecuteBattleCommandInBattle    =0x46AE59; // �֐��͂��̃A�h���X����AOnTenshouExeExecuteBattleCommandInBattle�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeExecuteBattleCommandInBattle=0x46A7CF; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeExecuteBattleCommandInBattle =0x46AE5E; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeExecuteBattleCommandInBattle() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// �R�}���h�����s��������ID���R�s�[
		mov iExecuteBattleCommandInBattleBushouID, ebx

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeExecuteBattleCommandInBattle

		// �L���ȃR�}���h�����s�����Ȃ�Aeax�ɗL���Ȓl�������Ă���Beax��0�Ȃ�΁A�R�}���h�͎��s���ꂽ�Ƃ͂����Ȃ��B
		mov iExecuteBattleCommandInBattleValidCommand, eax;

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
	OnTenshouExeExecuteBattleCommandInBattleExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeExecuteBattleCommandInBattle
	}
}

/*
00444769  |. 50             PUSH    EAX
0044476A  |. E8 3A2C0200    CALL    TENSHOU.004673A9
				�������� JMP TSMod.OnTSExeExecuteBattleCommandInBattle �Ə��������Ă����ۂ̏���
0044476F  |. 83C4 04        ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromExecuteBattleCommandInBattle[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeExecuteBattleCommandInBattle() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeExecuteBattleCommandInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeExecuteBattleCommandInBattle + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeExecuteBattleCommandInBattle  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromExecuteBattleCommandInBattle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeExecuteBattleCommandInBattle), cmdOnTenshouExeJumpFromExecuteBattleCommandInBattle, 5, NULL); //5�o�C�g�̂ݏ�������
}




