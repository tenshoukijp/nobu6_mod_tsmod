#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


/*
 * 煽動をすでに行ったため、再度行えるかどうか判定する処理。
 */

// 煽動をすでに行ったかどうか
int isSendouDoneOnceJudgement = 0;

// 煽動を行った実行武将ＩＤ
int iSendouDoneOnceBushouID = -1;

void OnTenshouExeSendouOneOnceJudgementExecute() {

	// 煽動弐以上であれば、煽動は何度でも行える。
	// 武将IDが一定の範囲(それ以外の値が入ってくることもあるためチェック)
	if (0 <= iSendouDoneOnceBushouID-1 && iSendouDoneOnceBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM) {
		if (getBushouInt(iSendouDoneOnceBushouID-1) >= 50 &&   // 智謀が50(=100)以上
			nb6bushouref[iSendouDoneOnceBushouID-1].maxint >= 70 &&	 // 智才が70(=140)以上
			nb6bushouref[iSendouDoneOnceBushouID-1].ambition >= 50) {	// 野望が50以上

			// 煽動はまだ行っていないこととする。
			isSendouDoneOnceJudgement = 0;
		}
	}
}


/*
0041B892   . E8 5A270100    CALL    TENSHOU.0042DFF1
				└ここを JMP TSMod.OnTSExeSendouOneOnceJudgement と書き換えてやる実際の処理
0041B897   . 83C4 04        ADD     ESP, 4
0041B89A     85C0           TEST    EAX, EAX
0041B89C     74 24          JE      SHORT TENSHOU.0041B8C2
*/
int pTenshouExeJumpFromToOnTenshouExeSendouOneOnceJudgement	 =0x41B892; // 関数はこのアドレスから、OnTenshouExeSendouOneOnceJudgementへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeSendouOneOnceJudgement =0x42DFF1; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeSendouOneOnceJudgement  =0x41B897; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeSendouOneOnceJudgement() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeSendouOneOnceJudgement

		// 煽動をやったかどうかはEAXに入っている。EAXが非0であれば煽動は行い済み
		mov isSendouDoneOnceJudgement, eax
		
		// 対象の武将ID
		mov iSendouDoneOnceBushouID, esi

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
	OnTenshouExeSendouOneOnceJudgementExecute();

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

		// 状況に応じて、煽動をやったかどうかはEAXに上書き
		mov eax, isSendouDoneOnceJudgement

		jmp pTenshouExeReturnLblFromOnTenshouExeSendouOneOnceJudgement
	}
}

/*
0041B892   . E8 5A270100    CALL    TENSHOU.0042DFF1
				└ここを JMP TSMod.OnTSExeSendouOneOnceJudgement と書き換えてやる実際の処理
0041B897   . 83C4 04        ADD     ESP, 4
0041B89A     85C0           TEST    EAX, EAX
0041B89C     74 24          JE      SHORT TENSHOU.0041B8C2
*/
char cmdOnTenshouExeJumpFromSendouOneOnceJudgement[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeSendouOneOnceJudgement() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeSendouOneOnceJudgement;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSendouOneOnceJudgement + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeSendouOneOnceJudgement  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromSendouOneOnceJudgement+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSendouOneOnceJudgement), cmdOnTenshouExeJumpFromSendouOneOnceJudgement, 5, NULL); //5バイトのみ書き込む
}




