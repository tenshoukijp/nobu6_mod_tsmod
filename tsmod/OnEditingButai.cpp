#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"



/*
 * 部隊編成画面に入った時
 */ 


/*
0047A022  |. E8 B0180000    CALL    TENSHOU.0047B8D7
0047A027  |. 8D8D 58FFFFFF  LEA     ECX, DWORD PTR SS:[EBP-A8]

*/


int isOnEditingButaiMode = 0L;

void OnTenshouExeEditingButaiExecute() {
}



/*
0047A022  |. E8 B0180000    CALL    TENSHOU.0047B8D7				←部隊編成画面に入った。この関数が呼ばれている間が部隊編成画面開いてる間。
0047A027  |. 8D8D 58FFFFFF  LEA     ECX, DWORD PTR SS:[EBP-A8]
*/
int pTenshouExeJumpFromToOnTenshouExeEditingButai	  = 0x47A022; // 関数はこのアドレスから、OnTenshouExeEditingButaiへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeEditingButai = 0x47B8D7; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeEditingButai  = 0x47A027; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeEditingButai() {
	// スタックにためておく
	__asm {

		mov isOnEditingButaiMode, 1	// 部隊編成モードＯＮ

			// 元の処理をここに記載
		call pTenshouExeJumpCallFromToOnTenshouExeEditingButai

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
	OnTenshouExeEditingButaiExecute();

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

		mov isOnEditingButaiMode, 0	// 部隊編成モードＯＦＦ

		jmp pTenshouExeReturnLblFromOnTenshouExeEditingButai
	}
}




char cmdOnTenshouExeJumpFromEditingButai[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計5バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeEditingButai() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeEditingButai;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeEditingButai + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeEditingButai  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromEditingButai+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeEditingButai), cmdOnTenshouExeJumpFromEditingButai, 5, NULL); //5バイトのみ書き込む
}




