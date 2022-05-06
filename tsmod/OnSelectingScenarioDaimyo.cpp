#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


/*
 * シナリオ大名選択時の処理
 */
void OnTenshouExeSelectingScenarioDaimyoExecute() {
	// シナリオ大名選択時の特殊処理を行う。
	EXSuperService_OnSelectingScenarioDaimyo();
}


/*
0049D289  |. E8 87A7F8FF    CALL    TENSHOU.00427A15                 ; \TENSHOU.00427A15
0049D28E  |. E8 4160F8FF    CALL    TENSHOU.004232D4
				└ここを JMP TSMod.OnTSExeSelectingScenarioDaimyo と書き換えてやる実際の処理
0049D293  |. E8 3FE6FDFF    CALL    TENSHOU.0047B8D7
*/
int pTenshouExeJumpFromToOnTenshouExeSelectingScenarioDaimyo	 =0x49D28E; // 関数はこのアドレスから、OnTenshouExeSelectingScenarioDaimyoへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeSelectingScenarioDaimyo =0x4232D4; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeSelectingScenarioDaimyo  =0x49D293; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeSelectingScenarioDaimyo() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeSelectingScenarioDaimyo

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
	OnTenshouExeSelectingScenarioDaimyoExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeSelectingScenarioDaimyo
	}
}

/*
0049D289  |. E8 87A7F8FF    CALL    TENSHOU.00427A15                 ; \TENSHOU.00427A15
0049D28E  |. E8 4160F8FF    CALL    TENSHOU.004232D4
				└ここを JMP TSMod.OnTSExeSelectingScenarioDaimyo と書き換えてやる実際の処理
0049D293  |. E8 3FE6FDFF    CALL    TENSHOU.0047B8D7
*/
char cmdOnTenshouExeJumpFromSelectingScenarioDaimyo[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeSelectingScenarioDaimyo() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeSelectingScenarioDaimyo;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSelectingScenarioDaimyo + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeSelectingScenarioDaimyo  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromSelectingScenarioDaimyo+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSelectingScenarioDaimyo), cmdOnTenshouExeJumpFromSelectingScenarioDaimyo, 5, NULL); //5バイトのみ書き込む
}




