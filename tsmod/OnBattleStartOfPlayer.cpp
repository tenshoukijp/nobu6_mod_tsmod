#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"
#include "OnChangeWeather.h"

/*
 * �v���C�����������߂Đ푈���͂��܂���
 */

// ����͕K���푈�J�n���ɌĂԂ���
// �S�Ă̕����̕ۑ������̃��Z�b�g
extern void EXServiceExecuteRegeneration_All_SoldierResetInBattle();
// �S�Ă̕����̕ۑ������̍Čv�Z�ۑ�
extern void EXServiceExecuteRegeneration_All_SoldierSaveInBattle();
// �ۑ���������A�����������Čv�Z
extern void EXServiceExecuteRegeneration_All_SoldierCalcInBattle();

// �푈���I��鎞�ɋ����I��-1�ɐݒ肷�邱��
extern int iCurIncrementTurnInBattle;
extern int iPreIncrementTurnInBattle;
extern int isMustCallOfDecrementTurnInBatttle;

// �S�Ă̕����̐��ł̌��݂̃t�H�[���`��
extern int iCurTekkousenForm[];

// ���z�u���Ɂu�s���ς݁v�ƂȂ镔���̑Ώ��B
extern bool arrIsFirstHaichiAction[];

// �퓬�ɂ����āA�U�ߍ��܂ꂽ���̏�ԍ�(��z��ԍ�+1�̔ԍ�)
int iTargetCastleIDInStartOfPlayerExecute = -1;

// NPC��PC���ʂŁA�s���̗ǂ��p�����[�^�ŌĂяo���B
extern void OnTenshouExeBattleStartExecute(int iRecieveCastleID );

void OnTenshouExeBattleStartOfPlayerExecute() {
	// �푈���d�|����ꂽ��
	// OutputDebugStream( "%s", nb6castle[iTargetCastleIDInStartOfPlayerExecute-1].name);
	// �푈���d�|�����l�̓^�[���R�c
	// OutputDebugStream( "%s", nb6bushouname[ nb6gundan[ nb6turn.gundan-1 ].leader-1 ].fastname );

	// �푈�J�n
	CommonGlobalHandle::isBattleMode = 1;

	// �V��ω��̗\��͍폜
	iReservedWeatherOnChangeWeather.clear();

	// �푈�J�n���ɑS�Ă̕����̐��ł̌��݂̃t�H�[���`�Ԃ�-1�Ƃ��Ă���
	for ( int ix=0; ix<GAMEDATASTRUCT_BUSHOU_NUM; ix++) {
		iCurTekkousenForm[ix]=-1;
	}

	// �푈�J�n���ɑS�Ă̏��z�u��0�ɂ��Ă���
	for ( int ix=0; ix<GAMEDATASTRUCT_BUSHOU_NUM; ix++ ) {
		arrIsFirstHaichiAction[ix] = 0;
	}

	// �����ɂ�������������NPC�̏ꏊ�ł��S�������v�Z���K�v�Ȃ̂Œ���!!
	// �S�Ă̕����̕ۑ������̃��Z�b�g
	EXServiceExecuteRegeneration_All_SoldierResetInBattle();
	//  �S�Ă̕����̕ۑ������̍Čv�Z�ۑ�
	EXServiceExecuteRegeneration_All_SoldierSaveInBattle();
	// �ۑ���������A�����������Čv�Z
	EXServiceExecuteRegeneration_All_SoldierCalcInBattle();

	// �G�����Ȃ�A�퓬���S�Ă̋Z�\���g����(�p�����^IF�̈��)
	EXSuperService_IfCanAllAlibityInBattleStart();

	// �s���̗ǂ��p�����[�^�ɂ����쓮�֐����Ăяo���B
	OnTenshouExeBattleStartExecute(iTargetCastleIDInStartOfPlayerExecute-1 );

}

void OnTenshouExeBattleEndOfPlayerExecute() {
	// �G�����Ȃ�A�퓬���S�Ă̋Z�\���g����(�p�����^IF�̈��)�ŁA�퓬�J�n�O�̌��̋Z�\�ɖ߂��B
	EXSuperService_IfCanAllAlibityInBattleEnd();

	// �V��ω��̗\��͍폜
	iReservedWeatherOnChangeWeather.clear();

	// ScenarioMod�̐푈�I�����̃��\�b�h�ďo���B
	if ( p_snOnBattleEnd ) {
		((PFNSNONBATTLEEND) p_snOnBattleEnd)();
	}
}


/*
004A0E39  |. E8 0BFBFFFF    CALL    TENSHOU.004A0949
				�������� JMP TSMod.OnTSExeBattleStartOfPlayer �Ə��������Ă����ۂ̏���
004A0E3E  |. B8 01000000    MOV     EAX, 1
*/
int pTenshouExeJumpFromToOnTenshouExeBattleStartOfPlayer	   =0x4A0E39; // �֐��͂��̃A�h���X����AOnTenshouExeBattleStartOfPlayer�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeBattleStartOfPlayer   =0x4A0949; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeBattleStartOfPlayer    =0x4A0E3E; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w

// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeBattleStartOfPlayer() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// �U�ߍ��܂ꂽ���̏�ID���T���Ă���
		mov iTargetCastleIDInStartOfPlayerExecute, esi

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
	OnTenshouExeBattleStartOfPlayerExecute();

	__asm {

		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		// ���XTENSHOU.EXE���ɂ������L�q���̂܂�
		call  pTenshouExeJumpCallFromToOnTenshouExeBattleStartOfPlayer
	}
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

	// 
	OnTenshouExeBattleEndOfPlayerExecute();

	__asm {

		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		// �푈�I���
		mov CommonGlobalHandle::isBattleMode, 0

		// �푈�I������̂Ń^�[����-1��
		mov iCurIncrementTurnInBattle, -1
		mov iPreIncrementTurnInBattle, -1
		mov isMustCallOfDecrementTurnInBatttle, 0

		jmp pTenshouExeReturnLblFromOnTenshouExeBattleStartOfPlayer
	}
}

/*
004A0E39  |. E8 0BFBFFFF    CALL    TENSHOU.004A0949
				�������� JMP TSMod.OnTSExeBattleStartOfPlayer �Ə��������Ă����ۂ̏���
004A0E3E  |. B8 01000000    MOV     EAX, 1
*/
char cmdOnTenshouExeJumpFromBattleStartOfPlayer[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeBattleStartOfPlayer() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeBattleStartOfPlayer;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeBattleStartOfPlayer + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeBattleStartOfPlayer  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromBattleStartOfPlayer+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeBattleStartOfPlayer), cmdOnTenshouExeJumpFromBattleStartOfPlayer, 5, NULL); //5�o�C�g�̂ݏ�������
}




