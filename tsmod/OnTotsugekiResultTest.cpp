#include "WinTarget.h"
#include <time.h>

#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"

/*
 * 戦争において、突撃系をした際(NPC含む)、突撃された側の死亡判定の操作
 */

int iTotsugekiAttackTestBushouID = -1; // 突撃した武将ID
int iTotsugekiDefendTestBushouID = -1; // 突撃された武将ID
int iIsTotsugekiResultDeath = -1;	   // 突撃した結果、死亡したのかしていないのか。

void OnTenshouExeTotsugekiResultTestExecute() { 
	int iBushouID = iTotsugekiAttackTestBushouID - 1;
	int iTargetID = iTotsugekiDefendTestBushouID - 1;
	// 範囲チェック
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM && 0 <= iTargetID && iTargetID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 突撃したのはランスロットタルタロスである。
		if (isLancelotTartare(iBushouID)) {

			// n/50の確率(n=TSMod.txt)で相手は死亡。１突撃あたりなので、n=3なら、相手は1/10ほどの確率で死ぬ。
			if (rand() % 30 == 0) {
				iIsTotsugekiResultDeath = 1;
			}

		// 突撃したのは朱伊である。
		} else if (isShuiHidechiyo(iBushouID)) {

			// n/50の確率(n=TSMod.txt)で相手は死亡。１突撃あたりなので、n=3なら、相手は1/10ほどの確率で死ぬ。
			if (rand() % 30 == 0) {
				iIsTotsugekiResultDeath = 1;
			}

		// 突撃側のジョブが騎士であり、騎馬か騎馬鉄砲である
		} else if ( nb6bushouref[iBushouID].job == 0xA0 && (nb6bushouref[iBushouID].form == 1 || nb6bushouref[iBushouID].form == 3 ) ) {
			// 相手側のジョブが上級未満
			if (nb6bushouref[iTargetID].job < 0xA0) {
				if ( rand() % 30 == 0 ) {
					iIsTotsugekiResultDeath = 1;
				}
			}
		}

		// 相手がランスロットタルタロスなら突撃による死亡がない。
		if ( isLancelotTartare( iTargetID ) ) {
			iIsTotsugekiResultDeath = 0;
		} 

		// 相手が朱伊なら突撃による死亡がない。
		else if (isShuiHidechiyo(iTargetID)) {
			iIsTotsugekiResultDeath = 0;
		}

		// 関数がDLLで定義されていたら、CONDITIONとして呼び出し、その結果がTRUEであれば、死亡させ、FALSEであれば、死亡しない。
		// ScenarioModで特に何もしないということを可能とするため、
		if ( p_snOnCustomCondition ) {																						          // ┌突撃した側               ┌突撃された側                 ┌ 予定されてる死亡結果
			int isScenarioModDeathResult = ((PFNSNONCUSTOMCONDITION) p_snOnCustomCondition)("AS条件::戦争::突撃受動者死亡可否", iTotsugekiAttackTestBushouID, iTotsugekiDefendTestBushouID, iIsTotsugekiResultDeath, "", "", 0.0, 0.0);
			// 非修正要綱でなければ…
			if ( isScenarioModDeathResult != CUSTOM_CONDITION_NOMODIFY ) {
				iIsTotsugekiResultDeath = isScenarioModDeathResult;
			}
		}
	}
}
/* 
004324A6  |. E8 6AF1FFFF    |CALL    TENSHOU.00431615                ; \TENSHOU.00431615　←突撃系
004324AB  |. 83C4 0C        |ADD     ESP, 0C
004324AE  |. 85C0           |TEST    EAX, EAX							EAXが1なら相手が死亡した。ＥＢＸが相手の武将番号

*/ 
int pTenshouExeJumpFromToOnTenshouExeTotsugekiResultTest =0x4324A6; 
int pTenshouExeJumpCallFromToOnTenshouExeTotsugekiResultTest=0x431615; 
int pTenshouExeReturnLblFromOnTenshouExeTotsugekiResultTest =0x4324AB; 
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeTotsugekiResultTest() { 
	__asm { 
		// TENSHOU.EXE内のcall
		call pTenshouExeJumpCallFromToOnTenshouExeTotsugekiResultTest 

		// 
		mov iIsTotsugekiResultDeath, eax				// EAXに1が入っていれば、相手武将は死亡した
		mov iTotsugekiAttackTestBushouID, edi			// 突撃した側の武将ID 
		mov iTotsugekiDefendTestBushouID, ebx			// 突撃された側の武将ID 
		push eax 
		push ebx 
		push ecx 
		push edx 
		push esp 
		push ebp 
		push esi 
		push edi 
	} 

	OnTenshouExeTotsugekiResultTestExecute(); 

	__asm { 
		pop edi 
		pop esi 
		pop ebp 
		pop esp 
		pop edx 
		pop ecx 
		pop ebx 
		pop eax 

		// 操作された突撃結果による死亡判定結果をeaxに戻す。
		mov eax, iIsTotsugekiResultDeath 

		jmp pTenshouExeReturnLblFromOnTenshouExeTotsugekiResultTest 
	} 
} 
char cmdOnTenshouExeJumpFromTotsugekiResultTest[6]="\xE9"; 
void WriteAsmJumperOnTenshouExeTotsugekiResultTest() { 
	int iAddress = (int)OnTenshouExeTotsugekiResultTest; 
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeTotsugekiResultTest + 5 ) ; 
	memcpy(cmdOnTenshouExeJumpFromTotsugekiResultTest+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。 
	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeTotsugekiResultTest), cmdOnTenshouExeJumpFromTotsugekiResultTest, 5, NULL); // 5バイト書き換え
} 