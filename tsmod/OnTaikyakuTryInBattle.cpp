#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"

/*
 */

int iTaikyakuTryBushouID = -1; // �ދp���悤�Ƃ�������ID
int iIsSuccessTaikyaku = -1; // ���̂���AX�����̂� 

void OnTenshouExeTaikyakuTryExecute() { 
	int iBushouID = iTaikyakuTryBushouID-1; 

	if ( 0<= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) { // 532�͎蔲�� 

		// �֐���DLL�Œ�`����Ă�����ACONDITION�Ƃ��ČĂяo���A���̌��ʂ�FALSE�ł���΁c �ދp���邱�Ƃ͏o���Ȃ��B
		if ( p_snOnCustomCondition ) {

			int isCanTaikyakuScenarioMod = ((PFNSNONCUSTOMCONDITION) p_snOnCustomCondition)("AND����::�푈::�ދp��", iTaikyakuTryBushouID, 0, 0, "", "", 0.0, 0.0);
			if ( isCanTaikyakuScenarioMod == FALSE ) {
				iIsSuccessTaikyaku = iIsSuccessTaikyaku | 0xFFFF;  // �ދp���o���Ȃ�
			}
		}

	} 

} 
/* 
0041C042 . E8 99CE0000 CALL TENSHOU.00428EE0 ; \TENSHOU.00428EE0 
0041C047 . 83C4 08 ADD ESP, 8 
0041C04A . 66:3D FFFF CMP AX, 0FFFF 
*/ 
int pTenshouExeJumpFromToOnTenshouExeTaikyakuTry =0x41C042; 
int pTenshouExeJumpCallFromToOnTenshouExeTaikyakuTry=0x428EE0; 
int pTenshouExeReturnLblFromOnTenshouExeTaikyakuTry =0x41C047; 
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeTaikyakuTry() { 
	__asm { 
		// TENSHOU.EXE����call
		call pTenshouExeJumpCallFromToOnTenshouExeTaikyakuTry 

		// 
		mov iIsSuccessTaikyaku, eax				// �����������ǂ��� eax�܂�܂�ł͂Ȃ��Aax==0xffff�Ȃ�ދp�ł��Ȃ���
		mov iTaikyakuTryBushouID, ebx			// 
		push eax 
		push ebx 
		push ecx 
		push edx 
		push esp 
		push ebp 
		push esi 
		push edi 
	} 

	OnTenshouExeTaikyakuTryExecute(); 

	__asm { 
		pop edi 
		pop esi 
		pop ebp 
		pop esp 
		pop edx 
		pop ecx 
		pop ebx 
		pop eax 

		mov eax, iIsSuccessTaikyaku 
		jmp pTenshouExeReturnLblFromOnTenshouExeTaikyakuTry 
	} 
} 
char cmdOnTenshouExeJumpFromTaikyakuTry[6]="\xE9"; 
void WriteAsmJumperOnTenshouExeTaikyakuTry() { 
	int iAddress = (int)OnTenshouExeTaikyakuTry; 
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeTaikyakuTry + 5 ) ; 
	memcpy(cmdOnTenshouExeJumpFromTaikyakuTry+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B 
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeTaikyakuTry), cmdOnTenshouExeJumpFromTaikyakuTry, 5, NULL); // �ꉞ���S�̂��� 
} 