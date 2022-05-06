#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


/*
 * 煽動混乱で全員が混乱してしまった旨のメッセージが違和感があるので書き換える
 */

// 実際に何人混乱したのか？
extern int iSendouKonranTargetConranCnt;

void OnTenshouExeSendouKonranAllMessageExecute() {

	// メッセージが転回されているアドレスを得る
	int iSzAddress = CPM.getMemoryAddress(LOC4);
	// アドレスからchar *変数へと変更
	char *szMsgSendouKonranAll = (char *)iSzAddress;

	if ( iSendouKonranTargetConranCnt > 6 ) {

		// 全員→多く
		char *szZenin = strstr(szMsgSendouKonranAll, "全員");
		if (szZenin) {
			memcpy (szZenin, "多く", 4);
		}

		// 導いてしまった→導いた
		szZenin = strstr(szMsgSendouKonranAll, "導いてしまった");
		if (szZenin) {
			memcpy (szZenin, "導いた\0", 7);
		}
	}
	else if ( iSendouKonranTargetConranCnt >= 1 ) {

		// 全員→多く
		char *szZenin = strstr(szMsgSendouKonranAll, "周囲の者たちを次々と巻き込み");
		if (szZenin) {
			memcpy (szZenin, "周囲の数部隊の相手を巻き込み", 28);
		}

		// 全員を大混乱へと導いてしまった→導いた
		szZenin = strstr(szMsgSendouKonranAll, "全員を大混乱");
		if ( szZenin ) {
			memcpy (szZenin, "大混乱へと導いた\0", 17);
		}
	}
	else {
		// 全員→多く
		char *szZenin = strstr(szMsgSendouKonranAll, "周囲の者たちを");
		if ( szZenin ) {
			memcpy (szZenin, "周囲の誰も混乱させなかった\0", 27);
		}
	}
}


/*
00433AF1  |. 68 18100000    PUSH    1018                             ; |Arg1 = 00001018
00433AF6  |. E8 5D890300    CALL    TENSHOU.0046C458                 ; \TENSHOU.0046C458
				└ここを JMP TSMod.OnTSExeSendouKonranAllMessage と書き換えてやる実際の処理
00433AFB  |. 83C4 08        ADD     ESP, 8
*/
int pTenshouExeJumpFromToOnTenshouExeSendouKonranAllMessage	    =0x433AF6; // 関数はこのアドレスから、OnTenshouExeSendouKonranAllMessageへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeSendouKonranAllMessage =0x46C458; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeSendouKonranAllMessage  =0x433AFB; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w

// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeSendouKonranAllMessage() {
	// スタックにためておく
	__asm {
		// TENSHOU.EXEに元々あったもの
		call pTenshouExeJumpCallFromToOnTenshouExeSendouKonranAllMessage

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
	OnTenshouExeSendouKonranAllMessageExecute();

	__asm {

		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		jmp pTenshouExeReturnLblFromOnTenshouExeSendouKonranAllMessage
	}
}

/*
00433AF1  |. 68 18100000    PUSH    1018                             ; |Arg1 = 00001018
00433AF6  |. E8 5D890300    CALL    TENSHOU.0046C458                 ; \TENSHOU.0046C458
				└ここを JMP TSMod.OnTSExeSendouKonranAllMessage と書き換えてやる実際の処理
00433AFB  |. 83C4 08        ADD     ESP, 8
*/
char cmdOnTenshouExeJumpFromSendouKonranAllMessage[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeSendouKonranAllMessage() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeSendouKonranAllMessage;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSendouKonranAllMessage + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeSendouKonranAllMessage  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromSendouKonranAllMessage+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSendouKonranAllMessage), cmdOnTenshouExeJumpFromSendouKonranAllMessage, 5, NULL); //5バイトのみ書き込む
}




