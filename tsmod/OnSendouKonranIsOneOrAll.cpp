#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


/*
 * 煽動混乱で１人だけか、多人数混乱か
 */

// 煽動を行った実行武将ＩＤ
int iSendouKonranIsOneOrAllBushouID = -1;

// 煽動混乱が多人数になるかどうかフラグ
int iSendouKonranIsOneOrAllMultiNumber = -1;

// 前回の煽動時に混乱したカウント
extern int iSendouKonranTargetConranCnt;

void OnTenshouExeSendouKonranIsOneOrAllExecute() {

	// 前回の煽動で混乱した人数をここで念のためリセット
	iSendouKonranTargetConranCnt = 0;

	// 煽動四以上であれば、煽動混乱対象が多人数となる。
	// 武将IDが一定の範囲(それ以外の値が入ってくることもあるためチェック)
	if (0 <= iSendouKonranIsOneOrAllBushouID-1 && iSendouKonranIsOneOrAllBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM) {
		if (getBushouInt(iSendouKonranIsOneOrAllBushouID-1) >= 90 &&   // 智謀が90(=180)以上
			nb6bushouref[iSendouKonranIsOneOrAllBushouID-1].maxint >= 100 &&	 // 智才が100(=200)以上
			nb6bushouref[iSendouKonranIsOneOrAllBushouID-1].ambition >= 90) {	// 野望が90以上

			// マルチ人数強制
			iSendouKonranIsOneOrAllMultiNumber = 1;
		}
	}
}


/*
00433A9B  |. E8 EB940700    CALL    TENSHOU.004ACF8B									
				└ここを JMP TSMod.OnTSExeSendouKonranIsOneOrAll と書き換えてやる実際の処理
00433AA0  |. 83C4 04        ADD     ESP, 4									
00433AA3  |. 85C0           TEST    EAX, EAX		ここでEAXが0だと自分が混乱
*/
int pTenshouExeJumpFromToOnTenshouExeSendouKonranIsOneOrAll	  =0x433A9B; // 関数はこのアドレスから、OnTenshouExeSendouKonranIsOneOrAllへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeSendouKonranIsOneOrAll =0x4ACF8B; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeSendouKonranIsOneOrAll  =0x433AA0; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeSendouKonranIsOneOrAll() {
	// スタックにためておく
	__asm {

		// 元々TENSHOU.EXEにあった処理をここで実行
		call pTenshouExeJumpCallFromToOnTenshouExeSendouKonranIsOneOrAll

		// 実行武将のID
		mov iSendouKonranIsOneOrAllBushouID, edi

		// eaxが0だと多人数にならず自分混乱 
		mov iSendouKonranIsOneOrAllMultiNumber, eax;

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
	OnTenshouExeSendouKonranIsOneOrAllExecute();

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

		// 多人数になるかどうかの情報を上書き
		mov eax, iSendouKonranIsOneOrAllMultiNumber

		jmp pTenshouExeReturnLblFromOnTenshouExeSendouKonranIsOneOrAll
	}
}

/*
0041B892   . E8 5A270100    CALL    TENSHOU.0042DFF1
				└ここを JMP TSMod.OnTSExeSendouKonranIsOneOrAll と書き換えてやる実際の処理
0041B897   . 83C4 04        ADD     ESP, 4
0041B89A     85C0           TEST    EAX, EAX
0041B89C     74 24          JE      SHORT TENSHOU.0041B8C2
*/
char cmdOnTenshouExeJumpFromSendouKonranIsOneOrAll[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeSendouKonranIsOneOrAll() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeSendouKonranIsOneOrAll;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSendouKonranIsOneOrAll + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeSendouKonranIsOneOrAll  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromSendouKonranIsOneOrAll+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSendouKonranIsOneOrAll), cmdOnTenshouExeJumpFromSendouKonranIsOneOrAll, 5, NULL); //5バイトのみ書き込む
}




