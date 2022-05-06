#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"




/*
 * �m�o�b�݂̂ő����͂��܂邿����ƑO�B�U�߂�ꂽ��ԍ����m�ۂ��邽�߂݂̂ł������t�b�N
 */

// �퓬�ɂ����āA�U�ߍ��܂ꂽ���̏�ԍ�(��z��ԍ�+1�̔ԍ�)
extern int iTargetCastleIDInStartOfNPCExecute;


/*
0043CC3B  |. 66:A1 38B64C00 MOV     AX, WORD PTR DS:[4CB638]
0043CC41  |. 50             PUSH    EAX
0043CC42  |. E8 C00B0300    CALL    TENSHOU.0046D807
				�������� JMP TSMod.OnTSExeBattleStartOfNPCPrevious �Ə��������Ă����ۂ̏���
0043CC47  |. 83C4 04        ADD     ESP, 4
*/
int pTenshouExeJumpFromToOnTenshouExeBattleStartOfNPCPrevious		=0x43CC42; // �֐��͂��̃A�h���X����AOnTenshouExeBattleStartOfNPCPrevious�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeBattleStartOfNPCPrevious   =0x46D807; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeBattleStartOfNPCPrevious	=0x43CC47; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeBattleStartOfNPCPrevious() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// �U�߂�ꂽ��ԍ���ێ����Ă����B
		mov iTargetCastleIDInStartOfNPCExecute, eax

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeBattleStartOfNPCPrevious

		jmp pTenshouExeReturnLblFromOnTenshouExeBattleStartOfNPCPrevious
	}
}

/*
0043CC47  |. 83C4 04        ADD     ESP, 4
0043CC4A  |. 50             PUSH    EAX                              ; |Arg1
0043CC4B  |. E8 9E220300    CALL    TENSHOU.0046EEEE                 ; \TENSHOU.0046EEEE
				�������� JMP TSMod.OnTSExeBattleStartOfNPCPrevious �Ə��������Ă����ۂ̏���
0043CC50  |. 83C4 0C        ADD     ESP, 0C
*/
char cmdOnTenshouExeJumpFromBattleStartOfNPCPrevious[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeBattleStartOfNPCPrevious() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeBattleStartOfNPCPrevious;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeBattleStartOfNPCPrevious + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeBattleStartOfNPCPrevious  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromBattleStartOfNPCPrevious+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeBattleStartOfNPCPrevious), cmdOnTenshouExeJumpFromBattleStartOfNPCPrevious, 5, NULL); //5�o�C�g�̂ݏ�������
}



//-------------------------------------------------------------------------------------------------------------




/*
 * �m�o�b�݂̂ő����͂��܂���
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
extern BOOL isMustCallOfDecrementTurnInBatttle;

// �S�Ă̕����̐��ł̌��݂̃t�H�[���`��
extern int iCurTekkousenForm[];

// ���z�u���Ɂu�s���ς݁v�ƂȂ镔���̑Ώ��B
extern bool arrIsFirstHaichiAction[];

// �퓬�ɂ����āA�U�ߍ��܂ꂽ���̏�ԍ�(��z��ԍ�+1�̔ԍ�)
int iTargetCastleIDInStartOfNPCExecute = -1;

// NPC��PC���ʂŁA�s���̗ǂ��p�����[�^�ŌĂяo���B
extern void OnTenshouExeBattleStartExecute(int iRecieveCastleID );

void OnTenshouExeBattleStartOfNPCExecute() {
	// �푈�J�n
	CommonGlobalHandle::isBattleMode = 1;

	// �푈�J�n���ɑS�Ă̕����̐��ł̌��݂̃t�H�[���`�Ԃ�-1�Ƃ��Ă���
	for ( int ix=0; ix<GAMEDATASTRUCT_BUSHOU_NUM; ix++) {
		iCurTekkousenForm[ix]=-1;
	}

	// �푈�J�n���ɑS�Ă̏��z�u��0�ɂ��Ă���
	for ( int ix=0; ix<GAMEDATASTRUCT_BUSHOU_NUM; ix++ ) {
		arrIsFirstHaichiAction[ix] = 0;
	}

	// �푈�����Ă��Ȃ��ꍇ�A�n���l���ُ�ɏオ��̂�h�����߂̏����B
	// �퓬�J�n���ɁA�l��ۑ����Ă����B
	EXServiceExecuteBushouValueControllerOfStartBattle();


	// �����ɂ�������������Player�̏ꏊ�ł��S�������v�Z���K�v�Ȃ̂Œ���!!
	// �S�Ă̕����̕ۑ������̃��Z�b�g
	EXServiceExecuteRegeneration_All_SoldierResetInBattle();
	//  �S�Ă̕����̕ۑ������̍Čv�Z�ۑ�
	EXServiceExecuteRegeneration_All_SoldierSaveInBattle();
	// �ۑ���������A�����������Čv�Z
	EXServiceExecuteRegeneration_All_SoldierCalcInBattle();

	// �G�����Ȃ�A�퓬���S�Ă̋Z�\���g����(�p�����^IF�̈��)
	EXSuperService_IfCanAllAlibityInBattleStart();

	// �s���̗ǂ��p�����[�^�ɂ����쓮�֐����Ăяo���B
	// OutputDebugStream("%d", iTargetCastleIDInStartOfNPCExecute);
	OnTenshouExeBattleStartExecute(iTargetCastleIDInStartOfNPCExecute-1 );

}

void OnTenshouExeBattleEndOfNPCExecute() {
	// �G�����Ȃ�A�퓬���S�Ă̋Z�\���g����(�p�����^IF�̈��)
	EXSuperService_IfCanAllAlibityInBattleEnd();
}

/*
0043CC53  |. E8 F13C0600    CALL    TENSHOU.004A0949						
				�������� JMP TSMod.OnTSExeBattleStartOfNPC �Ə��������Ă����ۂ̏���
0043CC58  |. 57             PUSH    EDI						
*/
int pTenshouExeJumpFromToOnTenshouExeBattleStartOfNPC	    =0x43CC53; // �֐��͂��̃A�h���X����AOnTenshouExeBattleStartOfNPC�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeBattleStartOfNPC   =0x4A0949; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeBattleStartOfNPC    =0x43CC58; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w

// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeBattleStartOfNPC() {
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
	
	// 
	OnTenshouExeBattleStartOfNPCExecute();

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
		call  pTenshouExeJumpCallFromToOnTenshouExeBattleStartOfNPC
	}

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

	// 
	OnTenshouExeBattleEndOfNPCExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeBattleStartOfNPC
	}
}

/*
0043CC53  |. E8 F13C0600    CALL    TENSHOU.004A0949						
				�������� JMP TSMod.OnTSExeBattleStartOfNPC �Ə��������Ă����ۂ̏���
0043CC58  |. 57             PUSH    EDI						
*/
char cmdOnTenshouExeJumpFromBattleStartOfNPC[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeBattleStartOfNPC() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeBattleStartOfNPC;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeBattleStartOfNPC + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeBattleStartOfNPC  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromBattleStartOfNPC+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeBattleStartOfNPC), cmdOnTenshouExeJumpFromBattleStartOfNPC, 5, NULL); //5�o�C�g�̂ݏ�������
}




//----------------------------------------------------------------------------------------------


/*
 * �m�o�b�݂̂ő����I�����������ƌ�
 */

void OnTenshouExeBattleAfterOfNPCExecute() {
	// �푈�����Ă��Ȃ��ꍇ�A�n���l���ُ�ɏオ��̂�h�����߂̏����B
	// �퓬�I����ɁA�l���r���Đ��䂷��B
	EXServiceExecuteBushouValueControllerOfEndBattle();

	// ScenarioMod�̐푈�I�����̃��\�b�h�ďo���B
	if ( p_snOnBattleEnd ) {
		((PFNSNONBATTLEEND) p_snOnBattleEnd)();
	}
}

/*
0043CC65  |. 56             PUSH    ESI
0043CC66  |. E8 72770100    CALL    TENSHOU.004543DD
				�������� JMP TSMod.OnTSExeBattleAfterOfNPC �Ə��������Ă����ۂ̏���
0043CC6B  |. 83C4 04        ADD     ESP, 4
0043CC6E  |. 85C0           TEST    EAX, EAX
*/
int pTenshouExeJumpFromToOnTenshouExeBattleAfterOfNPC	    =0x43CC66; // �֐��͂��̃A�h���X����AOnTenshouExeBattleAfterOfNPC�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeBattleAfterOfNPC   =0x4543DD; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeBattleAfterOfNPC    =0x43CC6B; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w

// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeBattleAfterOfNPC() {

	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// ���XTENSHOU.EXE���ɂ������L�q���̂܂�
		call  pTenshouExeJumpCallFromToOnTenshouExeBattleAfterOfNPC

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
	OnTenshouExeBattleAfterOfNPCExecute();

	__asm {

		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		jmp pTenshouExeReturnLblFromOnTenshouExeBattleAfterOfNPC
	}
}


char cmdOnTenshouExeJumpFromBattleAfterOfNPC[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeBattleAfterOfNPC() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeBattleAfterOfNPC;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeBattleAfterOfNPC + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeBattleAfterOfNPC  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromBattleAfterOfNPC+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeBattleAfterOfNPC), cmdOnTenshouExeJumpFromBattleAfterOfNPC, 5, NULL); //5�o�C�g�̂ݏ�������
}




