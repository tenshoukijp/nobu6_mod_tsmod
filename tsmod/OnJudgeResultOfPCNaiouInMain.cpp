#include "WinTarget.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"
#include "EXBushou_All.h"
#include "ScenarioMod.h"

/*
内応の前に門前払いかどうか。
00402897  |. 85C0           TEST    EAX, EAX


0040288F  |. E8 8D1B0500    CALL    TENSHOU.00454421　← これを実行した後、ESIに仕掛けた側、EBXに仕掛けられた側の部将番号。
													  　 これは、PC/NPCで変わらない。
00402894  |. 83C4 04        ADD     ESP, 4


004028BF  |. 53             PUSH    EBX
004028C0  |. 56             PUSH    ESI
004028C1  |. E8 62FDFFFF    CALL    TENSHOU.00402628 PCの場合   この関数を呼び出すとEDIに結果の値が入る
004028C6  |. 83C4 08        ADD     ESP, 8

00402901  |. E8 612D0A00    CALL    TENSHOU.004A5667 NPCの場合　この関数を呼び出すとEDIに結果の値が入る。
00402906  |. 83C4 04        ADD     ESP, 4


これの結果、EDIの値で成否のランクが決まる。
EDI=0 その場で即座に出奔
EDI=1 内応の取り付けに成功
EDI=2 失敗した。相手に否定された。
EDI=3 逆に内応されてしまい、出奔していった。(大名や軍団長が出奔すると情報が狂うので注意)
EDI=4 失敗した。逆に主に不満はないのか？ と言われた。
*/

int GetJudgeResultOfNaiouInMain(int iActiveBushou, int iPassiveBushou, int iResult) {
	if ( 0 <= iActiveBushou  && iActiveBushou  < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= iPassiveBushou && iPassiveBushou < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ScenarioModのカスタム条件で軍師を設定する
		if ( p_snOnCustomCondition ) {
			// メイン画面での内応の成否																							┌実行者	     ┌受動者	    ┌現在予定されている結果 
			int isScenarioModNaiouResult = ((PFNSNONCUSTOMCONDITION) p_snOnCustomCondition)("AS条件::メイン::内応結果可否", iActiveBushou+1, iPassiveBushou+1, iResult, "", "", 0.0, 0.0);
			if ( isScenarioModNaiouResult != CUSTOM_CONDITION_NOMODIFY ) {

				// 内応の成否を上書きする。
				return isScenarioModNaiouResult;
			}
		}

		// 内応を受ける側の立場が一般の宿老～足軽頭
		if ( nb6bushouname[iPassiveBushou].position >= 2  &&   // 宿老～足軽
			 nb6bushouname[iPassiveBushou].State == 2 ) {      // 立場が「一般」である。

			// 内応する側がランスロット・タルタロスなら1/3の確率ですぐさま相手を出奔させる。
			if ( isLancelotTartare(iActiveBushou) ) {
				if ( rand() % 3 == 0 ) {
					return 0; // 1/3の確率で結果を大成功に上書き。
				}
			}
			// 内応する側がバールゼフォンはさらに巧みだ。1/2の確率ですぐさま相手を出奔させる。
			else if ( strcmp( nb6bushouname[iActiveBushou].familyname, "") == 0 && strcmp( nb6bushouname[iActiveBushou].fastname, "") == 0 ) {
				if ( rand() % 2 == 0 ) {
					return 0; // 1/2の確率で結果を大成功に上書き。
				}
			}
		}

	}

	return -1; // 元の値のまま
}




static int iActiveBushouOfJudgeResultOfPCNaiouInMain = -1;
static int iPassiveBushouOfJudgeResultOfPCNaiouInMain = -1;
static int iResultOfJudgeResultOfPCNaiouInMain = 1; // 結果

void OnTenshouExeJudgeResultOfPCNaiouInMainExecute() {
	int iActiveBushou  = iActiveBushouOfJudgeResultOfPCNaiouInMain - 1;   // 仕掛けた側
	int iPassiveBushou = iPassiveBushouOfJudgeResultOfPCNaiouInMain - 1;  // 仕掛けられた側

	int iOverRide = GetJudgeResultOfNaiouInMain(iActiveBushou, iPassiveBushou, iResultOfJudgeResultOfPCNaiouInMain);
	if (iOverRide != -1) {
		iResultOfJudgeResultOfPCNaiouInMain = iOverRide;
	}
}


/*
004028BF  |. 53             PUSH    EBX
004028C0  |. 56             PUSH    ESI
004028C1  |. E8 62FDFFFF    CALL    TENSHOU.00402628 PCの場合   この関数を呼び出すとEDIに結果の値が入る
				└ここを JMP TSMod.OnTSExeJudgeResultOfPCNaiouInMain と書き換えてやる実際の処理
004028C6  |. 83C4 08        ADD     ESP, 8
*/
int pTenshouExeJumpFromToOnTenshouExeJudgeResultOfPCNaiouInMain	    =0x4028C1; // 関数はこのアドレスから、OnTenshouExeJudgeResultOfPCNaiouInMainへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeJudgeResultOfPCNaiouInMain =0x402628; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeJudgeResultOfPCNaiouInMain  =0x4028C6; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeJudgeResultOfPCNaiouInMain() {
	// スタックにためておく
	__asm {

		mov iActiveBushouOfJudgeResultOfPCNaiouInMain, ESI // 仕掛ける側
		mov iPassiveBushouOfJudgeResultOfPCNaiouInMain, EBX // 仕掛けられる側

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeJudgeResultOfPCNaiouInMain

		mov iResultOfJudgeResultOfPCNaiouInMain, EDI
		/*
		EDI=0 その場で即座に出奔
		EDI=1 内応の取り付けに成功
		EDI=2 失敗した。相手に否定された。
		EDI=3 逆に内応されてしまい、出奔していった。(大名や軍団長が出奔すると情報が狂うので注意)
		EDI=4 失敗した。逆に主に不満はないのか？ と言われた。
		*/

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnTenshouExeJudgeResultOfPCNaiouInMainExecute();

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

		mov EDI, iResultOfJudgeResultOfPCNaiouInMain // 結果の上書き

		jmp pTenshouExeReturnLblFromOnTenshouExeJudgeResultOfPCNaiouInMain
	}
}



char cmdOnTenshouExeJumpFromJudgeResultOfPCNaiouInMain[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeJudgeResultOfPCNaiouInMain() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeJudgeResultOfPCNaiouInMain;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeJudgeResultOfPCNaiouInMain + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeJudgeResultOfPCNaiouInMain  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromJudgeResultOfPCNaiouInMain+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeJudgeResultOfPCNaiouInMain), cmdOnTenshouExeJumpFromJudgeResultOfPCNaiouInMain, 5, NULL); //5バイトのみ書き込む
}


