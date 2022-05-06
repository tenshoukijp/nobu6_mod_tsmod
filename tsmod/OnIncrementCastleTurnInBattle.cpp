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



0046B5D4   FE05 445D4D00    INC     BYTE PTR DS:[4D5D44]  // ターンのインクリメント
0046B5DA   803D 445D4D00 03 CMP     BYTE PTR DS:[4D5D44], 3 // ３と等しければ～
*/


/*
 * 攻城戦のターンが増えた時
 */


extern int arrRemainMoveCountAllBushou[GAMEDATASTRUCT_BUSHOU_NUM];


// 各ターン開始前
void OnTenshouExeIncrementCastleTurnInBattleBeforeExecute() {
	// 攻城戦ターンが経過する度に、全ての武将の残り行動数を２でセット
	for ( int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {
		arrRemainMoveCountAllBushou[iBushouID] = 2;
	}

	// ScenarioModのターンチェンジメソッドの呼び出し
	if ( p_snOnChangeCastleTurnInBattle ) { // ScenarioModの該当メソッドが存在すれば…
		((PFNSNONCHANGECASTLETURNINBATTLE) p_snOnChangeCastleTurnInBattle)( getCastleTurnInBattle() );
	}
}

// ３ターンの後(画面内の攻城戦が終了した時
void OnTenshouExeIncrementCastleTurnInBattleAfterExecute() {


	// ScenarioModのターンチェンジメソッドの呼び出し
	if ( getCastleTurnInBattle() == 3 ) {
		if ( p_snOnChangeCastleTurnInBattle ) { // ScenarioModの該当メソッドが存在すれば…
			((PFNSNONCHANGECASTLETURNINBATTLE) p_snOnChangeCastleTurnInBattle)( getCastleTurnInBattle()+1 );
		}
	}
}


/*
0046B5C5   . 50             PUSH    EAX                              ; |Arg1
0046B5C6   . E8 29FEFFFF    CALL    TENSHOU.0046B3F4                 ; \TENSHOU.0046B3F4
				└ここを JMP TSMod.OnTSExeIncrementCastleTurnInBattle と書き換えてやる実際の処理
0046B5CB   . 83C4 08        ADD     ESP, 8

*/
int pTenshouExeJumpFromToOnTenshouExeIncrementCastleTurnInBattle	 =0x46B5C6; // 関数はこのアドレスから、OnTenshouExeIncrementCastleTurnInBattleへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeIncrementCastleTurnInBattle =0x46B3F4;
int pTenshouExeReturnLblFromOnTenshouExeIncrementCastleTurnInBattle  =0x46B5CB; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w

// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeIncrementCastleTurnInBattle() {
	// スタックにためておく
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
	
	// 攻城戦のターン動作前
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

		// 攻城戦のとあるターン開始
		call pTenshouExeJumpCallFromToOnTenshouExeIncrementCastleTurnInBattle // TENSHOU.EXEに元々あった処理をここで
		// 攻城戦のとあるターン終了

	// スタックにためておく
		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	// 攻城戦のターン動作後
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
0046B5D4   FE05 445D4D00    INC     BYTE PTR DS:[4D5D44]  // ターンのインクリメント
				└ここを JMP TSMod.OnTSExeIncrementCastleTurnInBattle と書き換えてやる実際の処理
0046B5DA   803D 445D4D00 03 CMP     BYTE PTR DS:[4D5D44], 3 // ３と等しければ～
*/
char cmdOnTenshouExeJumpFromIncrementCastleTurnInBattle[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeIncrementCastleTurnInBattle() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeIncrementCastleTurnInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeIncrementCastleTurnInBattle + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeIncrementCastleTurnInBattle  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromIncrementCastleTurnInBattle+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeIncrementCastleTurnInBattle), cmdOnTenshouExeJumpFromIncrementCastleTurnInBattle, 5, NULL); //5バイトのみ書き込む
}




