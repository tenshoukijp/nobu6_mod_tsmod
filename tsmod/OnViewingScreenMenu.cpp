#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


void OnTenshouExeViewingScreenMenuExecute() {
	// ゲームははじまった(いろいろなメモリ等処理してＯＫかどうかのフラグとなる)
	CommonGlobalHandle::isNobunaganoYabouGameStart = true;
}


/*
0049902C  |. 8BCE           MOV     ECX, ESI
0049902E  |. 68 B8334D00    PUSH    TENSHOU.004D33B8                 ;  ASCII "画面サイズを選択してください"
00499033  |. E8 16550100    CALL    TENSHOU.004AE54E
				└ここを JMP TSMod.OnTSExeViewingScreenMenu と書き換えてやる実際の処理
00499038  |. 5E             POP     ESI
*/
int pTenshouExeJumpFromToOnTenshouExeViewingScreenMenu		=0x499033; // 関数はこのアドレスから、OnTenshouExeViewingScreenMenuへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeViewingScreenMenu  =0x4AE54E; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeViewingScreenMenu	=0x499038; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeViewingScreenMenu() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeViewingScreenMenu

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
	OnTenshouExeViewingScreenMenuExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingScreenMenu
	}
}

/*
0049902C  |. 8BCE           MOV     ECX, ESI
0049902E  |. 68 B8334D00    PUSH    TENSHOU.004D33B8                 ;  ASCII "画面サイズを選択してください"
00499033  |. E8 16550100    CALL    TENSHOU.004AE54E
				└ここを JMP TSMod.OnTSExeViewingScreenMenu と書き換えてやる実際の処理
00499038  |. 5E             POP     ESI
*/
char cmdOnTenshouExeJumpFromViewingScreenMenu[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeViewingScreenMenu() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeViewingScreenMenu;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingScreenMenu + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingScreenMenu  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromViewingScreenMenu+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingScreenMenu), cmdOnTenshouExeJumpFromViewingScreenMenu, 5, NULL); //5バイトのみ書き込む
}




