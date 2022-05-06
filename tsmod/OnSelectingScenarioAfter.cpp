#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


/*
* シナリオ大名説明文表示で確認ボタンを押した後
*/
void OnTenshouExeSelectingScenarioAfterExecute() {
	// シナリオ大名選択時の特殊処理を行う。
	EXSuperService_OnSelectingScenarioAfter();
}


/*
0049D7E9   E8 DF03FDFF      |CALL    TENSHOU.0046DBCD ← これ説明文表示中。抜けるとブラックアウト
0049D7EE   83C4 04          |ADD     ESP, 4
0049D7F1   E8 D2FBFFFF      |CALL    TENSHOU.0049D3C8 ← これフェイドインして大名選択。
*/
int pTenshouExeJumpFromToOnTenshouExeSelectingScenarioAfter = 0x49D7E9; // 関数はこのアドレスから、OnTenshouExeSelectingScenarioAfterへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeSelectingScenarioAfter = 0x46DBCD; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeSelectingScenarioAfter = 0x49D7EE; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec(naked) void WINAPI OnTenshouExeSelectingScenarioAfter() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeSelectingScenarioAfter

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
	OnTenshouExeSelectingScenarioAfterExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeSelectingScenarioAfter
	}
}

/*
0049D289  |. E8 87A7F8FF    CALL    TENSHOU.00427A15                 ; \TENSHOU.00427A15
0049D28E  |. E8 4160F8FF    CALL    TENSHOU.004232D4
└ここを JMP TSMod.OnTSExeSelectingScenarioAfter と書き換えてやる実際の処理
0049D293  |. E8 3FE6FDFF    CALL    TENSHOU.0047B8D7
*/
char cmdOnTenshouExeJumpFromSelectingScenarioAfter[6] = "\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeSelectingScenarioAfter() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeSelectingScenarioAfter;
	int SubAddress = iAddress - (pTenshouExeJumpFromToOnTenshouExeSelectingScenarioAfter + 5);
	// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeSelectingScenarioAfter  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromSelectingScenarioAfter + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory(CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSelectingScenarioAfter), cmdOnTenshouExeJumpFromSelectingScenarioAfter, 5, NULL); //5バイトのみ書き込む
}




