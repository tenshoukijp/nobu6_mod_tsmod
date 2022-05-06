#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"


/*
セーブデータをセーブする際に、名前(savedat.n6p)が決まる時。

0049D57E  |. 68 708D4C00    PUSH    TENSHOU.004C8D70                 ;  ASCII "A:SAVEDAT.N6P"
0049D583  |. 8D45 C0        LEA     EAX, DWORD PTR SS:[EBP-40]
*/


void OnTenshouExePushSaveFileNameOfUnknownExecute() {
	// セーブファイルの選択。しかし不明なタイミング
}



/*
0049D57E  |. 68 708D4C00    PUSH    TENSHOU.004C8D70                 ;  ASCII "A:SAVEDAT.N6P"
				└ここを JMP TSMod.OnTenshouExePushSaveFileNameOfUnknown と書き換えてやる実際の処理
0049D583  |. 8D45 C0        LEA     EAX, DWORD PTR SS:[EBP-40]
*/
int pTenshouExeJumpFromToOnTenshouExePushSaveFileNameOfUnknown	  = 0x49D57E; // 関数はこのアドレスから、OnTenshouExePushSaveFileNameOfUnknownへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExePushSaveFileNameOfUnknown  = 0x49D583; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExePushSaveFileNameOfUnknown() {
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
	
	OnTenshouExePushSaveFileNameOfUnknownExecute();

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

		// 元々tenshou.exe にあった処理をここに。
		PUSH    0x4C8D70

		jmp pTenshouExeReturnLblFromOnTenshouExePushSaveFileNameOfUnknown
	}
}




char cmdOnTenshouExeJumpFromPushSaveFileNameOfUnknown[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計5バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExePushSaveFileNameOfUnknown() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExePushSaveFileNameOfUnknown;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExePushSaveFileNameOfUnknown + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExePushSaveFileNameOfUnknown  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromPushSaveFileNameOfUnknown+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExePushSaveFileNameOfUnknown), cmdOnTenshouExeJumpFromPushSaveFileNameOfUnknown, 5, NULL); //5バイトのみ書き込む
}

