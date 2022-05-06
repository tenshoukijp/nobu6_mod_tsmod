/*
0041B37D  |> E8 55050600    CALL    TENSHOU.0047B8D7 ← 評定開始
0041B382  |. E8 677F0000    CALL    TENSHOU.004232EE ← 評定終了
*/


#include "WinTarget.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"


// 軍議中かどうかの判定表。軍議開始～終了まで、｢isModeGungiDoing」が1となる。
int isModeGungiDoing = 0L;

/*
0041B37D  |> E8 55050600    CALL    TENSHOU.0047B8D7 ← 評定開始
				└ここを JMP TSMod.OnTSExeGundiDoing と書き換えてやる実際の処理
0041B382  |. E8 677F0000    CALL    TENSHOU.004232EE ← 評定終了
*/
int pTenshouExeJumpFromToOnTenshouExeGundiDoing	 =0x41B37D; // 関数はこのアドレスから、OnTenshouExeGundiDoingへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeGundiDoing =0x47B8D7; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeGundiDoing  =0x41B382; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeGundiDoing() {
	// スタックにためておく
	__asm {

		mov isModeGungiDoing, 1  // 軍議モードをTRUEにする。

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeGundiDoing

		mov isModeGungiDoing, 0  // 軍議モードをFALSEにする。

		jmp pTenshouExeReturnLblFromOnTenshouExeGundiDoing
	}
}



char cmdOnTenshouExeJumpFromGundiDoing[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeGundiDoing() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeGundiDoing;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeGundiDoing + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGundiDoing  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromGundiDoing+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeGundiDoing), cmdOnTenshouExeJumpFromGundiDoing, 5, NULL); //5バイトのみ書き込む
}


