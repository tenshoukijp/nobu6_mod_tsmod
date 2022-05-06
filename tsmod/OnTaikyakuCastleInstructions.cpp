#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"

// �ދp�ɏ�Q�����邩�ǂ���
// �����݂����͖��������Ă���(�Ăяo���Ă��Ȃ�)

int iTaikyakuCastleInstructionBushouID = -1; // �ދp���悤�Ƃ�������ID
static int iInstructionCastleNoOfTaikyaku = -1; // ���̂���AX�����̂� 

void OnTenshouExeTaikyakuCastleInstructionExecute() { 
	int iBushouID = iTaikyakuCastleInstructionBushouID-1; 

	// �K��������
	iInstructionCastleNoOfTaikyaku = -1;

	if ( 0<= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) { // 532�͎蔲�� 

		// �֐���DLL�Œ�`����Ă�����ACONDITION�Ƃ��ČĂяo���A���̌��ʂ�FALSE�ł���΁c �ދp���邱�Ƃ͏o���Ȃ��B
		if ( p_snOnCustomCondition ) {

			int isTaikyakuCastleNoScenarioMod = ((PFNSNONCUSTOMCONDITION) p_snOnCustomCondition)("AS����::�푈::�ދp��ԍ�", iTaikyakuCastleInstructionBushouID, 0, 0, "", "", 0.0, 0.0);
			if ( 0 < isTaikyakuCastleNoScenarioMod && isTaikyakuCastleNoScenarioMod <= GAMEDATASTRUCT_CASTLE_NUM) {
				iInstructionCastleNoOfTaikyaku = iInstructionCastleNoOfTaikyaku & 0xFFFF0000;  // ��ʃr�b�g�݂̂��ɂ��āc
				iInstructionCastleNoOfTaikyaku = iInstructionCastleNoOfTaikyaku | isTaikyakuCastleNoScenarioMod;  // ���ʃr�b�g�͑ދp�̏�ԍ�
			}
		}
	} 
} 
/* 
00428EE4  |. 56             PUSH    ESI
00428EE5  |. 57             PUSH    EDI
00428EE6  |. 8B7D 0C        MOV     EDI, DWORD PTR SS:[EBP+C]
00428EE9  |. 85FF           TEST    EDI, EDI
*/ 
int pTenshouExeJumpFromToOnTenshouExeTaikyakuCastleInstruction    =0x428EE4; 
int pTenshouExeReturnLblFromOnTenshouExeTaikyakuCastleInstruction =0x428EE9; 
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeTaikyakuCastleInstruction() { 
	__asm { 
		mov iTaikyakuCastleInstructionBushouID, ebx			// ����
		push eax 
		push ebx 
		push ecx 
		push edx 
		push esp 
		push ebp 
		push esi 
		push edi 
	} 

	OnTenshouExeTaikyakuCastleInstructionExecute(); 

	// ���Ɏw�肪�Ȃ��܂܂ł������B
	if ( iInstructionCastleNoOfTaikyaku == -1 ) {
		__asm { 
			pop edi 
			pop esi 
			pop ebp 
			pop esp 
			pop edx 
			pop ecx 
			pop ebx 
			pop eax 

			// ���X�̏�����������
			PUSH    ESI
			PUSH    EDI
			MOV     EDI, DWORD PTR SS:[EBP+0xC]					// �ދp�\�������s�̉\������������AEDI���P�Ƃ��ɂȂ��Ă���B

			jmp pTenshouExeReturnLblFromOnTenshouExeTaikyakuCastleInstruction 
		} 
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

			// �ދp�̎w�肪�������ꍇ�́A�錟���ɂ䂾�˂�
			PUSH    ESI
			PUSH    EDI
			MOV     EDI, 0

			jmp pTenshouExeReturnLblFromOnTenshouExeTaikyakuCastleInstruction 
		} 
	}

} 
char cmdOnTenshouExeJumpFromTaikyakuCastleInstruction[6]="\xE9"; 
void WriteAsmJumperOnTenshouExeTaikyakuCastleInstruction() { 
	int iAddress = (int)OnTenshouExeTaikyakuCastleInstruction; 
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeTaikyakuCastleInstruction + 5 ) ; 
	memcpy(cmdOnTenshouExeJumpFromTaikyakuCastleInstruction+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B 
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeTaikyakuCastleInstruction), cmdOnTenshouExeJumpFromTaikyakuCastleInstruction, 5, NULL); // �ꉞ���S�̂��� 
} 