#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"
#include "OnChangeWeather.h"

/*
 * 戦闘中にターン表記が減った（内部的にはターン経過数が増えた）場合
 */

// 全ての武将の保存兵数の再計算保存
extern void EXServiceExecuteRegeneration_All_SoldierSaveInBattle();

// 保存兵数から、増加兵数を再計算
extern void EXServiceExecuteRegeneration_All_SoldierCalcInBattle();


// 戦争が終わる時に強制的に-1に設定すること
int iCurIncrementTurnInBattle = -1;
int iPreIncrementTurnInBattle = -1;

int WINAPI Extern_GetCurIncrementTurnInBattle() {
	return iCurIncrementTurnInBattle;
}

extern int arrRemainMoveCountAllBushou[GAMEDATASTRUCT_BUSHOU_NUM];

extern int iNextTurnIsFieldOrCastle;
extern BOOL isMustCallOfDecrementTurnInBatttle;

void OnTenshouExeDecrementTurnInBattleExecute() {

	if ( iCurIncrementTurnInBattle != iPreIncrementTurnInBattle ) {

		//  全ての武将の保存兵数の再計算保存
		EXServiceExecuteRegeneration_All_SoldierSaveInBattle();

		// 保存兵数から、増加兵数を再計算
		EXServiceExecuteRegeneration_All_SoldierCalcInBattle();

		// ターンの情報が代入された時、これを呼び出しなさよというフラグが立っている。
		if (isMustCallOfDecrementTurnInBatttle) {

			// １つ前にここに来た時の経過ターン数を保存
			iPreIncrementTurnInBattle = iCurIncrementTurnInBattle;

			// 戦闘ターンが経過する度に、全ての武将の残り行動数を２でセット(最大３回１ターンに行動するような武将が存在するということ)
			for ( int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {
				arrRemainMoveCountAllBushou[iBushouID] = 2;
			}

			// ScenarioModのターンチェンジメソッドの呼び出し
			if ( p_snOnChangeTurnInBattle ) { // ScenarioModの該当メソッドが存在すれば…
				((PFNSNONCHANGETURNINBATTLE) p_snOnChangeTurnInBattle)( getRemainTurnInBattle() );
			}
		}
	}

}


/*
戦争で１ターン減る度に回ってくる場所
0049630F  /$ A1 D2B44B00    MOV     EAX, DWORD PTR DS:[4BB4D2]
				└ここを JMP TSMod.OnTSExeDecrementTurnInBattle と書き換えてやる実際の処理
00496314  \. C3             RETN
*/
int pTenshouExeJumpFromToOnTenshouExeDecrementTurnInBattle	   =0x49630F; // 関数はこのアドレスから、OnTenshouExeDecrementTurnInBattleへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeDecrementTurnInBattle  =0x496314; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w

// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeDecrementTurnInBattle() {
	// スタックにためておく
	__asm {
		// eaxに経過ターン数が入ってる情報をコピー
		// 元々TENSHOU.EXE内にあった記述そのまま
		mov     eax, dword ptr ds:[GAMEDATASTRUCT_HOWMUCHTURNPROCEED_INBATTLE]

		// 経過ターン数をコピー
		mov iCurIncrementTurnInBattle, eax
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
	OnTenshouExeDecrementTurnInBattleExecute();

	__asm {

		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		jmp pTenshouExeReturnLblFromOnTenshouExeDecrementTurnInBattle
	}
}

/*
戦争で１ターン減る度に回ってくる場所
0049630F  /$ A1 D2B44B00    MOV     EAX, DWORD PTR DS:[4BB4D2]
				└ここを JMP TSMod.OnTSExeDecrementTurnInBattle と書き換えてやる実際の処理
00496314  \. C3             RETN
*/
char cmdOnTenshouExeJumpFromDecrementTurnInBattle[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeDecrementTurnInBattle() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeDecrementTurnInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeDecrementTurnInBattle + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeDecrementTurnInBattle  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromDecrementTurnInBattle+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeDecrementTurnInBattle), cmdOnTenshouExeJumpFromDecrementTurnInBattle, 5, NULL); //5バイトのみ書き込む
}




