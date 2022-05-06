#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"

// 退却に障害があるかどうか
// ★現在ここは無効化している(呼び出していない)

int iTaikyakuCastleInstructionBushouID = -1; // 退却しようとした武将ID
static int iInstructionCastleNoOfTaikyaku = -1; // このうちAX部分のみ 

void OnTenshouExeTaikyakuCastleInstructionExecute() { 
	int iBushouID = iTaikyakuCastleInstructionBushouID-1; 

	// 必ず初期化
	iInstructionCastleNoOfTaikyaku = -1;

	if ( 0<= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) { // 532は手抜き 

		// 関数がDLLで定義されていたら、CONDITIONとして呼び出し、その結果がFALSEであれば… 退却することは出来ない。
		if ( p_snOnCustomCondition ) {

			int isTaikyakuCastleNoScenarioMod = ((PFNSNONCUSTOMCONDITION) p_snOnCustomCondition)("AS条件::戦争::退却城番号", iTaikyakuCastleInstructionBushouID, 0, 0, "", "", 0.0, 0.0);
			if ( 0 < isTaikyakuCastleNoScenarioMod && isTaikyakuCastleNoScenarioMod <= GAMEDATASTRUCT_CASTLE_NUM) {
				iInstructionCastleNoOfTaikyaku = iInstructionCastleNoOfTaikyaku & 0xFFFF0000;  // 上位ビットのみいにして…
				iInstructionCastleNoOfTaikyaku = iInstructionCastleNoOfTaikyaku | isTaikyakuCastleNoScenarioMod;  // 下位ビットは退却の城番号
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
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeTaikyakuCastleInstruction() { 
	__asm { 
		mov iTaikyakuCastleInstructionBushouID, ebx			// 武将
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

	// 得に指定がないままであった。
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

			// 元々の処理をここに
			PUSH    ESI
			PUSH    EDI
			MOV     EDI, DWORD PTR SS:[EBP+0xC]					// 退却可能性が失敗の可能性があったら、EDIが１とかになっている。

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

			// 退却の指定があった場合は、城検索にゆだねる
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
	memcpy(cmdOnTenshouExeJumpFromTaikyakuCastleInstruction+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。 
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeTaikyakuCastleInstruction), cmdOnTenshouExeJumpFromTaikyakuCastleInstruction, 5, NULL); // 一応安全のため 
} 