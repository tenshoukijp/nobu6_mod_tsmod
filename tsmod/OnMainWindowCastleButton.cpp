#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

// メインウィンドウの「絵」「城」「将」の「絵」のボタンを押した時

BOOL isMainWindowCastleButtonMode = 0;
void OnTenshouExeMainWindowCastleButtonExecute() {

}

/*
00476B59   . E8 D5CBFDFF    CALL    TENSHOU.00453733
00476B5E   . 83C4 08        ADD     ESP, 8
*/

int pTenshouExeJumpFromToOnTenshouExeMainWindowCastleButton		=0x476B59; // 関数はこのアドレスから、OnTenshouExeMainWindowCastleButtonへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeMainWindowCastleButton =0x453733; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeMainWindowCastleButton	=0x476B5E; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeMainWindowCastleButton() {
	// スタックにためておく
	__asm {

		mov isMainWindowCastleButtonMode, 1

		call pTenshouExeJumpCallFromToOnTenshouExeMainWindowCastleButton

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 徴兵条件変更処理をするならば…
	OnTenshouExeMainWindowCastleButtonExecute();

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

		mov isMainWindowCastleButtonMode, 0


		jmp pTenshouExeReturnLblFromOnTenshouExeMainWindowCastleButton
	}
}

char cmdOnTenshouExeJumpFromMainWindowCastleButton[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeMainWindowCastleButton() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeMainWindowCastleButton;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeMainWindowCastleButton + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeMainWindowCastleButton  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromMainWindowCastleButton+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeMainWindowCastleButton), cmdOnTenshouExeJumpFromMainWindowCastleButton, 5, NULL); //5バイトのみ書き込む
}




