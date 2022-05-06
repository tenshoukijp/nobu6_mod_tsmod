#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "TSModCommand.h"

#include "ScenarioMod.h"
#include "PluginMod.h"

/*
 * ウィンドウサイズをどちらにするか決定した時
 */ 


// メニューバーとタイトルバーを無くす
extern void setNoCaptionAndMenuBarAfter();

void OnTenshouExeSelectWindowSizeExecute() {

	CommonGlobalHandle::IsAfterScreenSizeSelected = TRUE;

	// メニューバーとタイトルバーを無くす
	setNoCaptionAndMenuBarAfter();
}

/*
0049945C  |. 50             PUSH    EAX
0049945D  |. 74 07          JE      SHORT TENSHOU.00499466
0049945F  |. E8 B3FCFFFF    CALL    TENSHOU.00499117
				└ここを JMP TSMod.OnTSExeSelectWindowSize と書き換えてやる実際の処理
00499464  |. EB 05          JMP     SHORT TENSHOU.0049946B

*/
int pTenshouExeJumpFromToOnTenshouExeSelectWindowSize		=0x49945F; // 関数はこのアドレスから、OnTenshouExeSelectWindowSizeへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeSelectWindowSize   =0x499117; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeSelectWindowSize	=0x499464; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeSelectWindowSize() {

	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeSelectWindowSize

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
	OnTenshouExeSelectWindowSizeExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeSelectWindowSize
	}
}

char cmdOnTenshouExeJumpFromSelectWindowSize[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeSelectWindowSize() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeSelectWindowSize;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSelectWindowSize + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeSelectWindowSize  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromSelectWindowSize+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSelectWindowSize), cmdOnTenshouExeJumpFromSelectWindowSize, 5, NULL); //5バイトのみ書き込む
}


