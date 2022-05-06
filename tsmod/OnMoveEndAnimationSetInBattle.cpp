#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"

/*
 * 戦闘時にユニットが移動したあと、アニメーションを止めるという設定をする関数を呼び出すタイミングでの処理
 */

// 行動を終えた武将のID
int iMoveEndAnimationInBattleBushouID = -1;

// 今回実際に行動を終えるか終えないか
int iMoveEndAnimationInBattle = 1;

// 各々の武将の、今回のターンでの残り行動数
extern int arrRemainMoveCountAllBushou[GAMEDATASTRUCT_BUSHOU_NUM];

void OnTenshouExeMoveEndAnimationInBattleExecute() {

	// デフォルトは行動しをわったら、ちゃんとアニメーションは止まる(行動終了アニメーションとなる)
	iMoveEndAnimationInBattle = 1;

	// 武将IDの範囲チェック
	if ( 0 <= iMoveEndAnimationInBattleBushouID-1 && iMoveEndAnimationInBattleBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 武将のIDが上級職であれば… もしくは ランスロット・タルタロスであれば… もしくは朱伊ならば…
		if ( nb6bushouref[iMoveEndAnimationInBattleBushouID-1].job >= 0xA0 || isLancelotTartare(iMoveEndAnimationInBattleBushouID-1) || isShuiHidechiyo(iMoveEndAnimationInBattleBushouID - 1)) {

			// 今回のターンの行動の残りカウント数が1以上
			if ( arrRemainMoveCountAllBushou[iMoveEndAnimationInBattleBushouID-1] <= 0 ) {

				// アニメーションを止める
				iMoveEndAnimationInBattle = 1;

			} else {
				// 今回はアニメーションを止めない。再度行動出来るため。
				iMoveEndAnimationInBattle = 0;
			}
		}
	}

}


/*
0046AFAC     E8 4C12FAFF    CALL    TENSHOU.0040C1FD
				└ここを JMP TSMod.OnTSExeMoveEndAnimationInBattle と書き換えてやる実際の処理
0046AFB1  |. 83C4 08        ADD     ESP, 8
0046AFB4  |> 6A 01          PUSH    1
0046AFB6  |. B9 78E84D00    MOV     ECX, TENSHOU.004DE878
 */
int pTenshouExeJumpFromToOnTenshouExeMoveEndAnimationInBattle    =0x46AFAC; // 関数はこのアドレスから、OnTenshouExeMoveEndAnimationInBattleへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeMoveEndAnimationInBattle=0x40C1FD; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeMoveEndAnimationInBattle =0x46AFB1; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeMoveEndAnimationInBattle() {
	// スタックにためておく
	__asm {
		// 移動しをえた武将のIDをコピー
		mov iMoveEndAnimationInBattleBushouID, ecx

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// ゲームデータをプレイヤーターンが変わった段階で読み込み更新しておく
	OnTenshouExeMoveEndAnimationInBattleExecute();

	// アニメーションを止めるのであれば…
	if ( iMoveEndAnimationInBattle ) {
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

			// 元々TENSHOU.EXE内にあったものをここで(アニメーションを止める[というか行動がし終わったシグナルを設定する]関数を呼ぶ)
			call pTenshouExeJumpCallFromToOnTenshouExeMoveEndAnimationInBattle

			jmp pTenshouExeReturnLblFromOnTenshouExeMoveEndAnimationInBattle
		}

	// アニメーションを止めるのであれば…
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

			;	// アニメーション停止巻数は呼び出さない

			jmp pTenshouExeReturnLblFromOnTenshouExeMoveEndAnimationInBattle
		}
	}
}

/*
00444769  |. 50             PUSH    EAX
0044476A  |. E8 3A2C0200    CALL    TENSHOU.004673A9
				└ここを JMP TSMod.OnTSExeMoveEndAnimationInBattle と書き換えてやる実際の処理
0044476F  |. 83C4 04        ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromMoveEndAnimationInBattle[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeMoveEndAnimationInBattle() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeMoveEndAnimationInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeMoveEndAnimationInBattle + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeMoveEndAnimationInBattle  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromMoveEndAnimationInBattle+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeMoveEndAnimationInBattle), cmdOnTenshouExeJumpFromMoveEndAnimationInBattle, 5, NULL); //5バイトのみ書き込む
}




