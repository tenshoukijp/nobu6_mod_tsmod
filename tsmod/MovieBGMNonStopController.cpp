#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "FuncPlayMovie.h"


void OnTenshouExeMovieBGMNonStopExecute() {
}


/*
0043CE55  |. 0F84 D7000000  JE      TENSHOU.0043CF32
0043CE5B     E8 23060500    CALL    TENSHOU.0048D483
				└ここを JMP TSMod.OnTSExeMovieBGMNonStop と書き換えてやる実際の処理
0043CE60  |. E8 F8030500    CALL    TENSHOU.0048D25D
*/
int pTenshouExeJumpFromToOnTenshouExeMovieBGMNonStop	 =0x43CE5B; // 関数はこのアドレスから、OnTenshouExeMovieBGMNonStopへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeMovieBGMNonStop =0x48D483; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeMovieBGMNonStop	 =0x43CE60; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeMovieBGMNonStop() {
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
	
	// ゲームデータをプレイヤーターンが変わった段階で読み込み更新しておく
	OnTenshouExeMovieBGMNonStopExecute();

	// ブラックバックするならこっち。
	if ( isMovieWithBGMSTOP ) {
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

			// 元々TENSHOU.EXE内にあったものをここで
			call pTenshouExeJumpCallFromToOnTenshouExeMovieBGMNonStop

			jmp pTenshouExeReturnLblFromOnTenshouExeMovieBGMNonStop
		}
	
	// ブラックバックしないならこっち
	} else {
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

			jmp pTenshouExeReturnLblFromOnTenshouExeMovieBGMNonStop
		}
	}
}

char cmdOnTenshouExeJumpFromMovieBGMNonStop[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeMovieBGMNonStop() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeMovieBGMNonStop;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeMovieBGMNonStop + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeMovieBGMNonStop  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromMovieBGMNonStop+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeMovieBGMNonStop), cmdOnTenshouExeJumpFromMovieBGMNonStop, 5, NULL); //5バイトのみ書き込む
}



