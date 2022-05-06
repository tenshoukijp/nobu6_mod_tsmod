#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"
#include "OnChangeWeather.h"

extern int iCurIncrementTurnInBattle;
BOOL isMustCallOfDecrementTurnInBatttle = FALSE;
void OnTenshouExeSetTurnInBattleExecute() {
	if ( iCurIncrementTurnInBattle == -1 ) {
		isMustCallOfDecrementTurnInBatttle = FALSE;
	} else {
		isMustCallOfDecrementTurnInBatttle = TRUE;
	}
		
}


/*
�푈�łP�^�[������x�A�^�[�����Z�b�g���Ă���ꏊ
00496315  /$ 8B4424 04      MOV     EAX, DWORD PTR SS:[ESP+4]
00496319  |. A3 D2B44B00    MOV     DWORD PTR DS:[4BB4D2], EAX
0049631E  \. C3             RETN
*/
int pTenshouExeJumpFromToOnTenshouExeSetTurnInBattle	 =0x496319; // �֐��͂��̃A�h���X����AOnTenshouExeSetTurnInBattle�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeSetTurnInBattle  =0x49631E; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w

// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeSetTurnInBattle() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ������L�q���̂܂�
		MOV     DWORD PTR DS : [GAMEDATASTRUCT_HOWMUCHTURNPROCEED_INBATTLE], EAX

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
	OnTenshouExeSetTurnInBattleExecute();

	__asm {

		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		jmp pTenshouExeReturnLblFromOnTenshouExeSetTurnInBattle
	}
}

char cmdOnTenshouExeJumpFromSetTurnInBattle[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeSetTurnInBattle() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeSetTurnInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSetTurnInBattle + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeSetTurnInBattle  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromSetTurnInBattle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSetTurnInBattle), cmdOnTenshouExeJumpFromSetTurnInBattle, 5, NULL); //5�o�C�g�̂ݏ�������
}




