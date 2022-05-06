#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"
#include "OnChangeWeather.h"

/*
 * プレイヤ操作をからめて戦争がはじまった
 */

// これは必ず戦争開始時に呼ぶこと
// 全ての武将の保存兵数のリセット
extern void EXServiceExecuteRegeneration_All_SoldierResetInBattle();
// 全ての武将の保存兵数の再計算保存
extern void EXServiceExecuteRegeneration_All_SoldierSaveInBattle();
// 保存兵数から、増加兵数を再計算
extern void EXServiceExecuteRegeneration_All_SoldierCalcInBattle();

// 戦争が終わる時に強制的に-1に設定すること
extern int iCurIncrementTurnInBattle;
extern int iPreIncrementTurnInBattle;
extern int isMustCallOfDecrementTurnInBatttle;

// 全ての武将の戦場での現在のフォーム形態
extern int iCurTekkousenForm[];

// 初配置時に「行動済み」となる部分の対処。
extern bool arrIsFirstHaichiAction[];

// 戦闘において、攻め込まれた側の城番号(城配列番号+1の番号)
int iTargetCastleIDInStartOfPlayerExecute = -1;

// NPCとPC共通で、都合の良いパラメータで呼び出す。
extern void OnTenshouExeBattleStartExecute(int iRecieveCastleID );

void OnTenshouExeBattleStartOfPlayerExecute() {
	// 戦争を仕掛けられた城
	// OutputDebugStream( "%s", nb6castle[iTargetCastleIDInStartOfPlayerExecute-1].name);
	// 戦争を仕掛けた人はターン軍団
	// OutputDebugStream( "%s", nb6bushouname[ nb6gundan[ nb6turn.gundan-1 ].leader-1 ].fastname );

	// 戦争開始
	CommonGlobalHandle::isBattleMode = 1;

	// 天候変化の予約は削除
	iReservedWeatherOnChangeWeather.clear();

	// 戦争開始時に全ての武将の戦場での現在のフォーム形態を-1としておく
	for ( int ix=0; ix<GAMEDATASTRUCT_BUSHOU_NUM; ix++) {
		iCurTekkousenForm[ix]=-1;
	}

	// 戦争開始時に全ての初配置を0にしておく
	for ( int ix=0; ix<GAMEDATASTRUCT_BUSHOU_NUM; ix++ ) {
		arrIsFirstHaichiAction[ix] = 0;
	}

	// ★他にもういっかしょNPCの場所でも全く同じ計算が必要なので注意!!
	// 全ての武将の保存兵数のリセット
	EXServiceExecuteRegeneration_All_SoldierResetInBattle();
	//  全ての武将の保存兵数の再計算保存
	EXServiceExecuteRegeneration_All_SoldierSaveInBattle();
	// 保存兵数から、増加兵数を再計算
	EXServiceExecuteRegeneration_All_SoldierCalcInBattle();

	// 敵武将なら、戦闘中全ての技能が使える(パラメタIFの一種)
	EXSuperService_IfCanAllAlibityInBattleStart();

	// 都合の良いパラメータにした駆動関数を呼び出す。
	OnTenshouExeBattleStartExecute(iTargetCastleIDInStartOfPlayerExecute-1 );

}

void OnTenshouExeBattleEndOfPlayerExecute() {
	// 敵武将なら、戦闘中全ての技能が使える(パラメタIFの一種)で、戦闘開始前の元の技能に戻す。
	EXSuperService_IfCanAllAlibityInBattleEnd();

	// 天候変化の予約は削除
	iReservedWeatherOnChangeWeather.clear();

	// ScenarioModの戦争終了時のメソッド呼出し。
	if ( p_snOnBattleEnd ) {
		((PFNSNONBATTLEEND) p_snOnBattleEnd)();
	}
}


/*
004A0E39  |. E8 0BFBFFFF    CALL    TENSHOU.004A0949
				└ここを JMP TSMod.OnTSExeBattleStartOfPlayer と書き換えてやる実際の処理
004A0E3E  |. B8 01000000    MOV     EAX, 1
*/
int pTenshouExeJumpFromToOnTenshouExeBattleStartOfPlayer	   =0x4A0E39; // 関数はこのアドレスから、OnTenshouExeBattleStartOfPlayerへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeBattleStartOfPlayer   =0x4A0949; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeBattleStartOfPlayer    =0x4A0E3E; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w

// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeBattleStartOfPlayer() {
	// スタックにためておく
	__asm {

		// 攻め込まれた側の城IDを控えておく
		mov iTargetCastleIDInStartOfPlayerExecute, esi

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
	OnTenshouExeBattleStartOfPlayerExecute();

	__asm {

		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		// 元々TENSHOU.EXE内にあった記述そのまま
		call  pTenshouExeJumpCallFromToOnTenshouExeBattleStartOfPlayer
	}
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
	OnTenshouExeBattleEndOfPlayerExecute();

	__asm {

		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		// 戦争終わり
		mov CommonGlobalHandle::isBattleMode, 0

		// 戦争終わったのでターンを-1に
		mov iCurIncrementTurnInBattle, -1
		mov iPreIncrementTurnInBattle, -1
		mov isMustCallOfDecrementTurnInBatttle, 0

		jmp pTenshouExeReturnLblFromOnTenshouExeBattleStartOfPlayer
	}
}

/*
004A0E39  |. E8 0BFBFFFF    CALL    TENSHOU.004A0949
				└ここを JMP TSMod.OnTSExeBattleStartOfPlayer と書き換えてやる実際の処理
004A0E3E  |. B8 01000000    MOV     EAX, 1
*/
char cmdOnTenshouExeJumpFromBattleStartOfPlayer[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeBattleStartOfPlayer() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeBattleStartOfPlayer;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeBattleStartOfPlayer + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeBattleStartOfPlayer  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromBattleStartOfPlayer+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeBattleStartOfPlayer), cmdOnTenshouExeJumpFromBattleStartOfPlayer, 5, NULL); //5バイトのみ書き込む
}




