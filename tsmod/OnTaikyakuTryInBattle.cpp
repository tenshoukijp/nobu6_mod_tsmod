#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"

/*
 */

int iTaikyakuTryBushouID = -1; // 退却しようとした武将ID
int iIsSuccessTaikyaku = -1; // このうちAX部分のみ 

void OnTenshouExeTaikyakuTryExecute() { 
	int iBushouID = iTaikyakuTryBushouID-1; 

	if ( 0<= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) { // 532は手抜き 

		// 関数がDLLで定義されていたら、CONDITIONとして呼び出し、その結果がFALSEであれば… 退却することは出来ない。
		if ( p_snOnCustomCondition ) {

			int isCanTaikyakuScenarioMod = ((PFNSNONCUSTOMCONDITION) p_snOnCustomCondition)("AND条件::戦争::退却可否", iTaikyakuTryBushouID, 0, 0, "", "", 0.0, 0.0);
			if ( isCanTaikyakuScenarioMod == FALSE ) {
				iIsSuccessTaikyaku = iIsSuccessTaikyaku | 0xFFFF;  // 退却が出来ない
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
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeTaikyakuTry() { 
	__asm { 
		// TENSHOU.EXE内のcall
		call pTenshouExeJumpCallFromToOnTenshouExeTaikyakuTry 

		// 
		mov iIsSuccessTaikyaku, eax				// 成功したかどうか eaxまるまるではなく、ax==0xffffなら退却できない状況
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
	memcpy(cmdOnTenshouExeJumpFromTaikyakuTry+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。 
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeTaikyakuTry), cmdOnTenshouExeJumpFromTaikyakuTry, 5, NULL); // 一応安全のため 
} 