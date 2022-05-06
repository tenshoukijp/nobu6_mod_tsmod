#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"


/*
0046B5C5   . 50             PUSH    EAX                              ; |Arg1
0046B5C6   . E8 29FEFFFF    CALL    TENSHOU.0046B3F4                 ; \TENSHOU.0046B3F4
0046B5CB   . 83C4 08        ADD     ESP, 8



0046B5D4   FE05 445D4D00    INC     BYTE PTR DS:[4D5D44]  // �^�[���̃C���N�������g
0046B5DA   803D 445D4D00 03 CMP     BYTE PTR DS:[4D5D44], 3 // �R�Ɠ�������΁`
*/


/*
 * �U���̃^�[������������
 */


extern int arrRemainMoveCountAllBushou[GAMEDATASTRUCT_BUSHOU_NUM];


// �e�^�[���J�n�O
void OnTenshouExeIncrementCastleTurnInBattleBeforeExecute() {
	// �U���^�[�����o�߂���x�ɁA�S�Ă̕����̎c��s�������Q�ŃZ�b�g
	for ( int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {
		arrRemainMoveCountAllBushou[iBushouID] = 2;
	}

	// ScenarioMod�̃^�[���`�F���W���\�b�h�̌Ăяo��
	if ( p_snOnChangeCastleTurnInBattle ) { // ScenarioMod�̊Y�����\�b�h�����݂���΁c
		((PFNSNONCHANGECASTLETURNINBATTLE) p_snOnChangeCastleTurnInBattle)( getCastleTurnInBattle() );
	}
}

// �R�^�[���̌�(��ʓ��̍U��킪�I��������
void OnTenshouExeIncrementCastleTurnInBattleAfterExecute() {


	// ScenarioMod�̃^�[���`�F���W���\�b�h�̌Ăяo��
	if ( getCastleTurnInBattle() == 3 ) {
		if ( p_snOnChangeCastleTurnInBattle ) { // ScenarioMod�̊Y�����\�b�h�����݂���΁c
			((PFNSNONCHANGECASTLETURNINBATTLE) p_snOnChangeCastleTurnInBattle)( getCastleTurnInBattle()+1 );
		}
	}
}


/*
0046B5C5   . 50             PUSH    EAX                              ; |Arg1
0046B5C6   . E8 29FEFFFF    CALL    TENSHOU.0046B3F4                 ; \TENSHOU.0046B3F4
				�������� JMP TSMod.OnTSExeIncrementCastleTurnInBattle �Ə��������Ă����ۂ̏���
0046B5CB   . 83C4 08        ADD     ESP, 8

*/
int pTenshouExeJumpFromToOnTenshouExeIncrementCastleTurnInBattle	 =0x46B5C6; // �֐��͂��̃A�h���X����AOnTenshouExeIncrementCastleTurnInBattle�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeIncrementCastleTurnInBattle =0x46B3F4;
int pTenshouExeReturnLblFromOnTenshouExeIncrementCastleTurnInBattle  =0x46B5CB; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w

// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeIncrementCastleTurnInBattle() {
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
	
	// �U���̃^�[������O
	OnTenshouExeIncrementCastleTurnInBattleBeforeExecute();

	__asm {

		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		// �U���̂Ƃ���^�[���J�n
		call pTenshouExeJumpCallFromToOnTenshouExeIncrementCastleTurnInBattle // TENSHOU.EXE�Ɍ��X������������������
		// �U���̂Ƃ���^�[���I��

	// �X�^�b�N�ɂ��߂Ă���
		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	// �U���̃^�[�������
	OnTenshouExeIncrementCastleTurnInBattleAfterExecute();

	__asm {

		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	
		jmp pTenshouExeReturnLblFromOnTenshouExeIncrementCastleTurnInBattle
	}
}

/*
0046B5D4   FE05 445D4D00    INC     BYTE PTR DS:[4D5D44]  // �^�[���̃C���N�������g
				�������� JMP TSMod.OnTSExeIncrementCastleTurnInBattle �Ə��������Ă����ۂ̏���
0046B5DA   803D 445D4D00 03 CMP     BYTE PTR DS:[4D5D44], 3 // �R�Ɠ�������΁`
*/
char cmdOnTenshouExeJumpFromIncrementCastleTurnInBattle[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeIncrementCastleTurnInBattle() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeIncrementCastleTurnInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeIncrementCastleTurnInBattle + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeIncrementCastleTurnInBattle  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromIncrementCastleTurnInBattle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeIncrementCastleTurnInBattle), cmdOnTenshouExeJumpFromIncrementCastleTurnInBattle, 5, NULL); //5�o�C�g�̂ݏ�������
}




