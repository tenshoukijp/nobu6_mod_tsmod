#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "SetBushouAvgParam.h"

//-------------------------------------------------------------------------------------

// �I�����Ă���V�i���I�ԍ�
extern int iSelectingScenarioLocation;
extern int iSelectingScenarioID;

/*

00422D72   MOVZX   ECX, WORD PTR DS:[EAX*2+4D3718]
00427B6A   MOVZX   EAX, WORD PTR DS:[EDI*2+4D3718]
00436F98   MOVZX   EAX, WORD PTR DS:[EBP*2+4D3718]
00478A21   MOVZX   EAX, WORD PTR DS:[EAX*2+4D3718]

004D3718  00 00 02 00 03 00 04 00 01 00 05 00              .......�@�� �V�i���I��Location�ƃV�i���IID�̑Ή��\


00478A29  |. FF3485 28374D0>PUSH    DWORD PTR DS:[EAX*4+4D3728]                          ;  TENSHOU.004D36A4�@������EAX�̓V�i���I�ԍ�

00479A71   . E8 8BFEFFFF    CALL    TENSHOU.00479901
00479A76   . 85C0           TEST    EAX, EAX												EAX���P��������A���[�h�����Ƃ������ƁB
00479A78   . 74 0F          JE      SHORT TENSHOU.00479A89
*/

// �{���ɃZ�[�u�⃍�[�h�Łu���s�v�����������H
BOOL isExecuteOfOnSaveOrLoad = FALSE;

void OnTenshouExeDecisionScenarioIDOnSaveOrLoadExecute() {

	if ( isExecuteOfOnSaveOrLoad ) {
		// �I�������V�i���I
		byte *iLoadScenario = (byte *)GAMEDATASTRUCT_SAVE_OR_LOAD_SCENARIO_ID_ADDRESS;
		iSelectingScenarioID = *iLoadScenario;

		// �V�i���I�͂����ƑI�����ꂽ��
		if ( 0 <= iSelectingScenarioID && iSelectingScenarioID <= 5 ) {
			// �I���V�i���I���P�[�V������ݒ�
			if ( iSelectingScenarioID == 0 ) {
				iSelectingScenarioLocation = 0;
			} else if ( iSelectingScenarioID == 1 ) {
				iSelectingScenarioLocation = 2;
			} else if ( iSelectingScenarioID == 2 ) {
				iSelectingScenarioLocation = 3;
			} else if ( iSelectingScenarioID == 3 ) {
				iSelectingScenarioLocation = 4;
			} else if ( iSelectingScenarioID == 4 ) {
				iSelectingScenarioLocation = 1;
			} else if ( iSelectingScenarioID == 5 ) {
				iSelectingScenarioLocation = 5;
			} else {
				iSelectingScenarioLocation = -1;
			}
		}
	}
}


/*
00479A71   . E8 8BFEFFFF    CALL    TENSHOU.00479901
				�������� JMP TSMod.OnTSExeDecisionScenarioIDOnSaveOrLoad �Ə��������Ă����ۂ̏���
00479A76   . 85C0           TEST    EAX, EAX												EAX���P��������A���[�h�����Ƃ������ƁB
00479A78   . 74 0F          JE      SHORT TENSHOU.00479A89
*/
int pTenshouExeJumpFromToOnTenshouExeDecisionScenarioIDOnSaveOrLoad		=0x479A71; // �֐��͂��̃A�h���X����AOnTenshouExeDecisionScenarioIDOnSaveOrLoad�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeDecisionScenarioIDOnSaveOrLoad =0x479901; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeDecisionScenarioIDOnSaveOrLoad	=0x479A76; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeDecisionScenarioIDOnSaveOrLoad() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeDecisionScenarioIDOnSaveOrLoad

		mov isExecuteOfOnSaveOrLoad, eax // EAX�̓�����1�Ȃ�{���Ɏ��s���ꂽ�B

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// �Q�[������ʂŃ��[�h���ɉ�������Ȃ�΁c
	OnTenshouExeDecisionScenarioIDOnSaveOrLoadExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeDecisionScenarioIDOnSaveOrLoad
	}
}

char cmdOnTenshouExeJumpFromDecisionScenarioIDOnSaveOrLoad[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeDecisionScenarioIDOnSaveOrLoad() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeDecisionScenarioIDOnSaveOrLoad;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeDecisionScenarioIDOnSaveOrLoad + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeDecisionScenarioIDOnSaveOrLoad  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromDecisionScenarioIDOnSaveOrLoad+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeDecisionScenarioIDOnSaveOrLoad), cmdOnTenshouExeJumpFromDecisionScenarioIDOnSaveOrLoad, 5, NULL); //5�o�C�g�̂ݏ�������
}




