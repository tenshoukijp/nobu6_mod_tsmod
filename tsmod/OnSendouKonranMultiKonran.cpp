#include "WinTarget.h"
#include <time.h>

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


/*
 * 煽動混乱で多人数混乱時に、誰を混乱させるかどうっか
 */

// 煽動混乱を行った実行武将ＩＤ
int iSendouKonranMultiKonranBushouID = -1;

// 煽動混乱の判断対象となるターゲット武将ＩＤ
int iSendouKonranMultiKonranTargetID = -1;

// 今回のターゲットを煽動混乱させるかどうか
int iSendouKonranCurTargetBushouMustKonran = -1;

// 何人混乱したのか？ でその後のメッセージを変更する必要がある。
int iSendouKonranTargetConranCnt = 0;

int iSendouKonranMultiKonranSeed = 0;

void OnTenshouExeSendouKonranMultiKonranExecute() {

	iSendouKonranCurTargetBushouMustKonran = 1;

	iSendouKonranMultiKonranSeed++;

	// 全員の時、混乱対象は自群は含まれない。
	// 武将IDが一定の範囲(それ以外の値が入ってくることもあるためチェック)
	if (0 <= iSendouKonranMultiKonranBushouID-1 && iSendouKonranMultiKonranBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM) {

		if (0 <= iSendouKonranMultiKonranTargetID-1 && iSendouKonranMultiKonranTargetID-1 < GAMEDATASTRUCT_BUSHOU_NUM) {

			/*
			// 該当の武将の戦場における立場を返す(不明=0, 直接攻める側=1, 直接守る側=2, 攻める側の援軍=3, 守る側の援軍=4, 中立軍=5)
			int GetWarStandPointTheBushou(int iBushouID);
			*/
			int i1stStdPnt = GetWarStandPointTheBushou(iSendouKonranMultiKonranBushouID-1);
			int i2ndStdPnt = GetWarStandPointTheBushou(iSendouKonranMultiKonranTargetID-1);

			// もし実行者の所属大名とターゲットの所属大名が同じであれば、混乱させない
			if ( nb6bushouref[iSendouKonranMultiKonranBushouID-1].attach == nb6bushouref[iSendouKonranMultiKonranTargetID-1].attach ) {
				iSendouKonranCurTargetBushouMustKonran = 0;

			// 仕掛け人が、「直接攻撃側」で、仕掛けられる側が「その援軍」であれば、混乱しない。
			} else if ( i1stStdPnt == 1 && i2ndStdPnt == 3 ) {
				iSendouKonranCurTargetBushouMustKonran = 0;

			// 仕掛け人が、「攻撃側の援軍」で、仕掛けられる側が「直接攻撃側」であれば、混乱しない。
			} else if ( i1stStdPnt == 3 && i2ndStdPnt == 1 ) {
				iSendouKonranCurTargetBushouMustKonran = 0;

			// 仕掛け人が、「直接の守備陣」で、仕掛けられる側が「その援軍」であれば、混乱しない。
			} else if ( i1stStdPnt == 2 && i2ndStdPnt == 4 ) {
				iSendouKonranCurTargetBushouMustKonran = 0;

			// 仕掛け人が、「守備側の援軍」で、仕掛けられる側が「直接の守備軍」であれば、混乱しない。
			} else if ( i1stStdPnt == 4 && i2ndStdPnt == 2 ) {
				iSendouKonranCurTargetBushouMustKonran = 0;

			// 混乱者数が増えるほど、混乱する確率を減らす
			} else if ((iSendouKonranTargetConranCnt >= 1) && (rand() % 2 == 0) ) {
				iSendouKonranCurTargetBushouMustKonran = 0;

			} else {
				// 混乱した人数を増やす
				iSendouKonranTargetConranCnt++;
			}
		}
	}
}


/*
00433AC4  |. 6A 02          |PUSH    2
00433AC6  |. 50             |PUSH    EAX
00433AC7  |. E8 27A3FFFF    |CALL    TENSHOU.0042DDF3
				└ここを JMP TSMod.OnTSExeSendouKonranMultiKonran と書き換えてやる実際の処理
00433ACC  |. 66:8B06        |MOV     AX, WORD PTR DS:[ESI]
00433ACF  |. 83C4 08        |ADD     ESP, 8

*/
int pTenshouExeJumpFromToOnTenshouExeSendouKonranMultiKonran	  =0x433AC7; // 関数はこのアドレスから、OnTenshouExeSendouKonranMultiKonranへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeSendouKonranMultiKonran =0x42DDF3; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeSendouKonranMultiKonran  =0x433ACC; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeSendouKonranMultiKonran() {
	// スタックにためておく
	__asm {

		// 実行武将のID
		mov iSendouKonranMultiKonranBushouID, edi

		// 混乱対象となるターゲットの武将のID
		mov iSendouKonranMultiKonranTargetID, eax

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
	OnTenshouExeSendouKonranMultiKonranExecute();

	if ( iSendouKonranCurTargetBushouMustKonran ) {
		__asm {

			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			// 混乱させるべきなら関数呼び出し
			call pTenshouExeJumpCallFromToOnTenshouExeSendouKonranMultiKonran
			jmp pTenshouExeReturnLblFromOnTenshouExeSendouKonranMultiKonran
		}

	} else {

		__asm {

			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			jmp pTenshouExeReturnLblFromOnTenshouExeSendouKonranMultiKonran
		}
	}
}

/*
00433AC4  |. 6A 02          |PUSH    2
00433AC6  |. 50             |PUSH    EAX
00433AC7  |. E8 27A3FFFF    |CALL    TENSHOU.0042DDF3
				└ここを JMP TSMod.OnTSExeSendouKonranMultiKonran と書き換えてやる実際の処理
00433ACC  |. 66:8B06        |MOV     AX, WORD PTR DS:[ESI]
00433ACF  |. 83C4 08        |ADD     ESP, 8
*/
char cmdOnTenshouExeJumpFromSendouKonranMultiKonran[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeSendouKonranMultiKonran() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeSendouKonranMultiKonran;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSendouKonranMultiKonran + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeSendouKonranMultiKonran  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromSendouKonranMultiKonran+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSendouKonranMultiKonran), cmdOnTenshouExeJumpFromSendouKonranMultiKonran, 5, NULL); //5バイトのみ書き込む
}




