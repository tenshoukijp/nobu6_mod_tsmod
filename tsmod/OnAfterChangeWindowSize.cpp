#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "TSModCommand.h"

#include "ScenarioMod.h"
#include "PluginMod.h"

/*
 * 選択、もしくは、自動選択の結果、ウインドウサイズが変更されようとしている時(された直後)。
 */ 



/*
00499625  |. E8 7AEFFFFF    CALL    TENSHOU.004985A4 ← このメソッドで変更される。
0049962A  |. E8 A790FBFF    CALL    TENSHOU.004526D6
0049962F  |. E8 EC9CF8FF    CALL    TENSHOU.00423320
*/

void OnTenshouExeAfterChangeWindowSizeExecute() {

	// PluginMod系の０個以上（複数可能）なＤＬＬのメインゲーム開始時のメソッドを呼び出す。
	// 実行対象となっているDLLを順に捜査して…
	for ( vector<HMODULE>::iterator it = listOfTargetDLL.begin(); it != listOfTargetDLL.end(); it++ ) {

		// 対象のDLLにMainGameStart関数が存在するかをチェック。
		PFNTARGETMODAFTERCHANGEWINDOWSIZE p_AfterChangeWindowSize = (PFNTARGETMODAFTERCHANGEWINDOWSIZE)GetProcAddress(*it, "OnAfterChangeWindowSize");

		if ( !p_AfterChangeWindowSize ) {
			continue;
		}

		((PFNTARGETMODAFTERCHANGEWINDOWSIZE) p_AfterChangeWindowSize)();
	}

	// ScenarioModの同じメソッド
	if ( p_snOnAfterChangeWindowSize ) {
		p_snOnAfterChangeWindowSize();
	}

	// 最後のい変更矩形を更新しておく。
	GetWindowRect( CommonGlobalHandle::tenshouWND, &CommonGlobalHandle::latestCenterWindowRect );
}

/*
00499625  |. E8 7AEFFFFF    CALL    TENSHOU.004985A4 ← このメソッドで変更される。
				└ここを JMP TSMod.OnTSExeAfterChangeWindowSize と書き換えてやる実際の処理
0049962A  |. E8 A790FBFF    CALL    TENSHOU.004526D6
*/
int pTenshouExeJumpFromToOnTenshouExeAfterChangeWindowSize		=0x499625; // 関数はこのアドレスから、OnTenshouExeAfterChangeWindowSizeへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeAfterChangeWindowSize  =0x4985A4; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeAfterChangeWindowSize	=0x49962A; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeAfterChangeWindowSize() {

	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeAfterChangeWindowSize

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// ゲーム内画面でロード時に何かするならば…
	OnTenshouExeAfterChangeWindowSizeExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeAfterChangeWindowSize
	}
}

char cmdOnTenshouExeJumpFromAfterChangeWindowSize[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeAfterChangeWindowSize() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeAfterChangeWindowSize;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeAfterChangeWindowSize + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeAfterChangeWindowSize  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromAfterChangeWindowSize+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeAfterChangeWindowSize), cmdOnTenshouExeJumpFromAfterChangeWindowSize, 5, NULL); //5バイトのみ書き込む
}


