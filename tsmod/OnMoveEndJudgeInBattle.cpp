#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"

/*
 * 戦闘時にユニットが移動しをえたのかどうかのかを判定するための関数を呼び出す処理のタイミング
 */

// 全ての武将の残り行動数(各々の武将が複数回行動用の残りカウントを持っている)。
// 戦闘中に残りターンが変化すると、再びチャージされる。
int arrRemainMoveCountAllBushou[GAMEDATASTRUCT_BUSHOU_NUM] = {};

// 移動しをえたかのチェック対象の武将のID
int iMoveEndJudgeInBattleBushouID = -1;

// 今回実際に行動を終えるか終えないか
int iMoveEndJudgeInBattle = 1;

void OnTenshouExeMoveEndJudgeInBattleExecute() {

	// 武将IDの範囲チェック
	if ( 0 <= iMoveEndJudgeInBattleBushouID-1 && iMoveEndJudgeInBattleBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 武将のIDが上級職であれば…  もしくは ランスロット・タルタロスであれば… もしくは朱伊ならば…
		if ( nb6bushouref[iMoveEndJudgeInBattleBushouID-1].job >= 0xA0 || isLancelotTartare(iMoveEndJudgeInBattleBushouID-1) || isShuiHidechiyo(iMoveEndJudgeInBattleBushouID - 1)) {

			if ( arrRemainMoveCountAllBushou[iMoveEndJudgeInBattleBushouID-1] >= 1 ) {
				// 移動はまだ可能とする。
				iMoveEndJudgeInBattle = 0;
			}
		}
	}
}


/*
0046AD22  |. E8 8332FCFF    CALL    TENSHOU.0042DFAA		移動済であれば、このCALLによってEAXに１以上の値が入る。移動してなければ0
				└ここを JMP TSMod.OnTSExeMoveEndJudgeInBattle と書き換えてやる実際の処理
0046AD27  |. 83C4 04        ADD     ESP, 4
0046AD2A     85C0           TEST    EAX, EAX
0046AD2C     0F85 8E020000  JNZ     TENSHOU.0046AFC0
 */
int pTenshouExeJumpFromToOnTenshouExeMoveEndJudgeInBattle    =0x46AD22; // 関数はこのアドレスから、OnTenshouExeMoveEndJudgeInBattleへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeMoveEndJudgeInBattle=0x42DFAA; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeMoveEndJudgeInBattle =0x46AD27; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeMoveEndJudgeInBattle() {
	// スタックにためておく
	__asm {
		// 移動しをえたかのチェック対象の武将のIDをコピー
		mov iMoveEndJudgeInBattleBushouID, esi

		// 元々TENSHOU.EXE内にあったものをここで		移動済であれば、このCALLによってEAXに１以上の値が入る。移動してなければ0
		call pTenshouExeJumpCallFromToOnTenshouExeMoveEndJudgeInBattle

		// 行動をしをえているかどうか。EAXに１以上の値が入る。移動してなければ0
		mov iMoveEndJudgeInBattle, eax

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
	OnTenshouExeMoveEndJudgeInBattleExecute();

	// 移動しをえているならば普段どおり。何もしない。
	if ( iMoveEndJudgeInBattle ) {
		// スタックに引き出す
		__asm {
			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			jmp pTenshouExeReturnLblFromOnTenshouExeMoveEndJudgeInBattle
		}

	// 移動をしを得ていないという結果(移動しを得ていないという上書きをしたする必要がある場合も含む)はこちら
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

			mov eax, 0		// 0ならば移動はしを得ていない。

			jmp pTenshouExeReturnLblFromOnTenshouExeMoveEndJudgeInBattle
		}
	}
}

/*
00444769  |. 50             PUSH    EAX
0044476A  |. E8 3A2C0200    CALL    TENSHOU.004673A9
				└ここを JMP TSMod.OnTSExeMoveEndJudgeInBattle と書き換えてやる実際の処理
0044476F  |. 83C4 04        ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromMoveEndJudgeInBattle[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeMoveEndJudgeInBattle() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeMoveEndJudgeInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeMoveEndJudgeInBattle + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeMoveEndJudgeInBattle  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromMoveEndJudgeInBattle+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeMoveEndJudgeInBattle), cmdOnTenshouExeJumpFromMoveEndJudgeInBattle, 5, NULL); //5バイトのみ書き込む
}




