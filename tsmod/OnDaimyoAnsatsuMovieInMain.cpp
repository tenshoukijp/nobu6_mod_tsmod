#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "FuncPlayBGM.h"

// 大名暗殺成功時のムービーが流れた時


/*
0045B934  |> 6A 01          PUSH    1                                ; /Arg2 = 00000001
0045B936  |. 6A 1B          PUSH    1B                               ; |Arg1 = 0000001B
0045B938  |. E8 2714FEFF    CALL    TENSHOU.0043CD64                 ; \TENSHOU.0043CD64
0045B93D  |. 83C4 08        ADD     ESP, 8
*/

extern int iAnsatuBGMOfPatchInAnsatsuBushouInMain;
void OnTenshouExeDaimyoAnsatsuMovieInMainExecute() {
	// これはパッチ。大名暗殺時にムービーが流れると、BGMが再生されなくなってしまうので、
	// それまで再生されていたBGMを強制的に再生する。
	if (iAnsatuBGMOfPatchInAnsatsuBushouInMain) {
		FuncPlayBGM(iAnsatuBGMOfPatchInAnsatsuBushouInMain);
	}
}


int pTenshouExeJumpFromToOnTenshouExeDaimyoAnsatsuMovieInMain    =0x45B938; // 関数はこのアドレスから、OnTenshouExeDaimyoAnsatsuMovieInMainへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeDaimyoAnsatsuMovieInMain=0x43CD64; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeDaimyoAnsatsuMovieInMain =0x45B93D; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeDaimyoAnsatsuMovieInMain() {
	// スタックにためておく
	__asm {

		call pTenshouExeJumpCallFromToOnTenshouExeDaimyoAnsatsuMovieInMain

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
	OnTenshouExeDaimyoAnsatsuMovieInMainExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeDaimyoAnsatsuMovieInMain
	}
}


char cmdOnTenshouExeJumpFromDaimyoAnsatsuMovieInMain[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeDaimyoAnsatsuMovieInMain() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeDaimyoAnsatsuMovieInMain;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeDaimyoAnsatsuMovieInMain + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeDaimyoAnsatsuMovieInMain  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromDaimyoAnsatsuMovieInMain+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeDaimyoAnsatsuMovieInMain), cmdOnTenshouExeJumpFromDaimyoAnsatsuMovieInMain, 5, NULL); //5バイトのみ書き込む
}




