#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"

/*
0040C1FD  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0040C201     E8 EEE5FFFF    CALL    TENSHOU.0040A7F4
0040C206  |. 83C4 04        ADD     ESP, 4
0040C209  |. 66:3D FFFF     CMP     AX, 0FFFF
0040C20D  |. 74 0D          JE      SHORT TENSHOU.0040C21C
0040C20F  |. FF7424 08      PUSH    DWORD PTR SS:[ESP+8]
0040C213  |. 50             PUSH    EAX
0040C214     E8 E9190200    CALL    TENSHOU.0042DC02
0040C219  |. 83C4 08        ADD     ESP, 8
0040C21C  \> C3             RETN
*/

/*
 * 戦闘時にユニットのアニメーションのＯＮ・ＯＦＦが切り替わるタイミング
 */

// 行動を終えた武将のID
int iMoveUnitAnimationChangeInBattleBushouID = -1;

// 今回実際に行動を終えるか終えないか
int iMoveUnitAnimationChangeInBattle = 1;

// 各々の武将の、今回のターンでの残り行動数
extern int arrRemainMoveCountAllBushou[GAMEDATASTRUCT_BUSHOU_NUM];

void OnTenshouExeMoveUnitAnimationChangeInBattleExecute() {

	// デフォルトは行動しをわったら、ちゃんとアニメーションは切り替わる
	iMoveUnitAnimationChangeInBattle = 1;

	// 武将IDの範囲チェック
	if ( 0 <= iMoveUnitAnimationChangeInBattleBushouID-1 && iMoveUnitAnimationChangeInBattleBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 武将のIDが上級職であれば… もしくは ランスロット・タルタロスであれば… もしくは朱伊ならば…
		if ( nb6bushouref[iMoveUnitAnimationChangeInBattleBushouID-1].job >= 0xA0 || isLancelotTartare(iMoveUnitAnimationChangeInBattleBushouID-1) || isShuiHidechiyo(iMoveUnitAnimationChangeInBattleBushouID - 1)) {

			// 今回のターンの行動の残りカウント数が1以下なら、アニメーションを切り替える(即ち停止アニメーションにする)
			if ( arrRemainMoveCountAllBushou[iMoveUnitAnimationChangeInBattleBushouID-1] <= 1 ) {

				// アニメーションを切り替える
				iMoveUnitAnimationChangeInBattle = 1;

			} else {
				// 今回はアニメーションを止めない。再度行動出来るため。ユニットがゆにゆにと動くアニメーションを維持する
				iMoveUnitAnimationChangeInBattle = 0;
			}
		}
	}

}


/*
0040C201     E8 EEE5FFFF    CALL    TENSHOU.0040A7F4
				└ここを JMP TSMod.OnTSExeMoveUnitAnimationChangeInBattle と書き換えてやる実際の処理
0040C206  |. 83C4 04        ADD     ESP, 4
0040C209  |. 66:3D FFFF     CMP     AX, 0FFFF
 */
int pTenshouExeJumpFromToOnTenshouExeMoveUnitAnimationChangeInBattle    =0x40C201; // 関数はこのアドレスから、OnTenshouExeMoveUnitAnimationChangeInBattleへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeMoveUnitAnimationChangeInBattle=0x40A7F4; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeMoveUnitAnimationChangeInBattle =0x40C206; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeMoveUnitAnimationChangeInBattle() {
	// スタックにためておく
	__asm {
		// アニメーション切り替え判定を行う武将のIDをコピー
		mov iMoveUnitAnimationChangeInBattleBushouID, esi

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
	OnTenshouExeMoveUnitAnimationChangeInBattleExecute();

	// アニメーションを止めるのであれば…
	if ( iMoveUnitAnimationChangeInBattle ) {
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
			call pTenshouExeJumpCallFromToOnTenshouExeMoveUnitAnimationChangeInBattle

			jmp pTenshouExeReturnLblFromOnTenshouExeMoveUnitAnimationChangeInBattle
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

			// アニメーション停止関数は呼び出さない、リターンしてしまう
			add esp, 4

			retn
		}
	}
}

/*
00444769  |. 50             PUSH    EAX
0044476A  |. E8 3A2C0200    CALL    TENSHOU.004673A9
				└ここを JMP TSMod.OnTSExeMoveUnitAnimationChangeInBattle と書き換えてやる実際の処理
0044476F  |. 83C4 04        ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromMoveUnitAnimationChangeInBattle[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeMoveUnitAnimationChangeInBattle() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeMoveUnitAnimationChangeInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeMoveUnitAnimationChangeInBattle + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeMoveUnitAnimationChangeInBattle  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromMoveUnitAnimationChangeInBattle+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeMoveUnitAnimationChangeInBattle), cmdOnTenshouExeJumpFromMoveUnitAnimationChangeInBattle, 5, NULL); //5バイトのみ書き込む
}




