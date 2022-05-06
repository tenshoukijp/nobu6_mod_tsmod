#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"
#include "OnChangeWeather.h"

/*
 * �퓬���Ƀ^�[���\�L���������i�����I�ɂ̓^�[���o�ߐ����������j�ꍇ
 */

// �S�Ă̕����̕ۑ������̍Čv�Z�ۑ�
extern void EXServiceExecuteRegeneration_All_SoldierSaveInBattle();

// �ۑ���������A�����������Čv�Z
extern void EXServiceExecuteRegeneration_All_SoldierCalcInBattle();


// �푈���I��鎞�ɋ����I��-1�ɐݒ肷�邱��
int iCurIncrementTurnInBattle = -1;
int iPreIncrementTurnInBattle = -1;

int WINAPI Extern_GetCurIncrementTurnInBattle() {
	return iCurIncrementTurnInBattle;
}

extern int arrRemainMoveCountAllBushou[GAMEDATASTRUCT_BUSHOU_NUM];

extern int iNextTurnIsFieldOrCastle;
extern BOOL isMustCallOfDecrementTurnInBatttle;

void OnTenshouExeDecrementTurnInBattleExecute() {

	if ( iCurIncrementTurnInBattle != iPreIncrementTurnInBattle ) {

		//  �S�Ă̕����̕ۑ������̍Čv�Z�ۑ�
		EXServiceExecuteRegeneration_All_SoldierSaveInBattle();

		// �ۑ���������A�����������Čv�Z
		EXServiceExecuteRegeneration_All_SoldierCalcInBattle();

		// �^�[���̏�񂪑�����ꂽ���A������Ăяo���Ȃ���Ƃ����t���O�������Ă���B
		if (isMustCallOfDecrementTurnInBatttle) {

			// �P�O�ɂ����ɗ������̌o�߃^�[������ۑ�
			iPreIncrementTurnInBattle = iCurIncrementTurnInBattle;

			// �퓬�^�[�����o�߂���x�ɁA�S�Ă̕����̎c��s�������Q�ŃZ�b�g(�ő�R��P�^�[���ɍs������悤�ȕ��������݂���Ƃ�������)
			for ( int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {
				arrRemainMoveCountAllBushou[iBushouID] = 2;
			}

			// ScenarioMod�̃^�[���`�F���W���\�b�h�̌Ăяo��
			if ( p_snOnChangeTurnInBattle ) { // ScenarioMod�̊Y�����\�b�h�����݂���΁c
				((PFNSNONCHANGETURNINBATTLE) p_snOnChangeTurnInBattle)( getRemainTurnInBattle() );
			}
		}
	}

}


/*
�푈�łP�^�[������x�ɉ���Ă���ꏊ
0049630F  /$ A1 D2B44B00    MOV     EAX, DWORD PTR DS:[4BB4D2]
				�������� JMP TSMod.OnTSExeDecrementTurnInBattle �Ə��������Ă����ۂ̏���
00496314  \. C3             RETN
*/
int pTenshouExeJumpFromToOnTenshouExeDecrementTurnInBattle	   =0x49630F; // �֐��͂��̃A�h���X����AOnTenshouExeDecrementTurnInBattle�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeDecrementTurnInBattle  =0x496314; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w

// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeDecrementTurnInBattle() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// eax�Ɍo�߃^�[�����������Ă�����R�s�[
		// ���XTENSHOU.EXE���ɂ������L�q���̂܂�
		mov     eax, dword ptr ds:[GAMEDATASTRUCT_HOWMUCHTURNPROCEED_INBATTLE]

		// �o�߃^�[�������R�s�[
		mov iCurIncrementTurnInBattle, eax
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
	OnTenshouExeDecrementTurnInBattleExecute();

	__asm {

		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		jmp pTenshouExeReturnLblFromOnTenshouExeDecrementTurnInBattle
	}
}

/*
�푈�łP�^�[������x�ɉ���Ă���ꏊ
0049630F  /$ A1 D2B44B00    MOV     EAX, DWORD PTR DS:[4BB4D2]
				�������� JMP TSMod.OnTSExeDecrementTurnInBattle �Ə��������Ă����ۂ̏���
00496314  \. C3             RETN
*/
char cmdOnTenshouExeJumpFromDecrementTurnInBattle[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeDecrementTurnInBattle() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeDecrementTurnInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeDecrementTurnInBattle + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeDecrementTurnInBattle  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromDecrementTurnInBattle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeDecrementTurnInBattle), cmdOnTenshouExeJumpFromDecrementTurnInBattle, 5, NULL); //5�o�C�g�̂ݏ�������
}




