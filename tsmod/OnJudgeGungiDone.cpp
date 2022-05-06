#include "WinTarget.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"


/*
  今季節、すでに軍議を行ったかどうかの判定
/*
0041B267  |. E8 ACA20800    CALL    TENSHOU.004A5518
0041B26C  |. 83C4 04        ADD     ESP, 4
0041B26F  |. 85C0           TEST    EAX, EAX			← このEAXが1ならば、すでに軍議を行った
*/

// 今季節軍議を行ったかどうかを上書きするフラグ用
int isOverFlagOnJudgeGungiDone = 0L;

void OnTenshouExeJudgeGungiDoneExecute() {
	// 一季節に複数回の評定が可能であれば
	if ( TSModCommand::MyselfDaimyo::cmdCanMultiHyoujoInSeason ) {
		// 担当大名がプレイヤー大名であれば
		if ( isPlayerDaimyo(nb6turn.daimyo-1) ) {
			// 評定済みフラグを一時的に偽装
			isOverFlagOnJudgeGungiDone = 0;
		}
	}
}



/*
0041B267  |. E8 ACA20800    CALL    TENSHOU.004A5518
				└ここを JMP TSMod.OnTSExeJudgeGungiDone と書き換えてやる実際の処理
0041B26C  |. 83C4 04        ADD     ESP, 4
0041B26F  |. 85C0           TEST    EAX, EAX			← このEAXが1ならば、すでに軍議を行った
0041B271  |. 74 41          JE      SHORT TENSHOU.0041B2B4
*/
int pTenshouExeJumpFromToOnTenshouExeJudgeGungiDone	   =0x41B267; // 関数はこのアドレスから、OnTenshouExeJudgeGungiDoneへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeJudgeGungiDone =0x4A5518; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeJudgeGungiDone  =0x41B271; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeJudgeGungiDone() {
	// スタックにためておく
	__asm {

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeJudgeGungiDone

		mov isOverFlagOnJudgeGungiDone, eax    // 今季節軍議を行ったかどうかの結果フラグを保存しておく。

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnTenshouExeJudgeGungiDoneExecute();

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

		mov eax, isOverFlagOnJudgeGungiDone // 今季節軍議を行ったかどうかの結果フラグを上書きする。

		ADD     ESP, 4				// TENSHOU.EXEに元々あった処理
		TEST    EAX, EAX			// TENSHOU.EXEに元々あった処理

		jmp pTenshouExeReturnLblFromOnTenshouExeJudgeGungiDone
	}
}



char cmdOnTenshouExeJumpFromJudgeGungiDone[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeJudgeGungiDone() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeJudgeGungiDone;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeJudgeGungiDone + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeJudgeGungiDone  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromJudgeGungiDone+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeJudgeGungiDone), cmdOnTenshouExeJumpFromJudgeGungiDone, 5, NULL); //5バイトのみ書き込む
}


