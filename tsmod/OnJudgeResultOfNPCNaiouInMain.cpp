#include "WinTarget.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"
#include "EXBushou_All.h"

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


//----------------------------------------------------------------------------------------------
// 仕掛ける側と仕掛けられる側の武将番号を拾うため。

static int iActiveBushouOfJudgeResultOfNPCNaiouInMain = -1;
static int iPassiveBushouOfJudgeResultOfNPCNaiouInMain = -1;

void OnTenshouExeJudgeResultOfNPCNaiouInMain_BeforeExecute() {
}

/*
0040288F  |. E8 8D1B0500    CALL    TENSHOU.00454421
				└ここを JMP TSMod.OnTSExeJudgeResultOfNPCNaiouInMain_Before と書き換えてやる実際の処理
00402894  |. 83C4 04        ADD     ESP, 4
*/
int pTenshouExeJumpFromToOnTenshouExeJudgeResultOfNPCNaiouInMain_Before	    =0x40288F; // 関数はこのアドレスから、OnTenshouExeJudgeResultOfNPCNaiouInMain_Beforeへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeJudgeResultOfNPCNaiouInMain_Before =0x454421; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeJudgeResultOfNPCNaiouInMain_Before  =0x402894; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeJudgeResultOfNPCNaiouInMain_Before() {
	// スタックにためておく
	__asm {

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeJudgeResultOfNPCNaiouInMain_Before

		mov iActiveBushouOfJudgeResultOfNPCNaiouInMain, ESI // 仕掛ける側
		mov iPassiveBushouOfJudgeResultOfNPCNaiouInMain, EBX // 仕掛けられる側

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnTenshouExeJudgeResultOfNPCNaiouInMain_BeforeExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeJudgeResultOfNPCNaiouInMain_Before
	}
}



char cmdOnTenshouExeJumpFromJudgeResultOfNPCNaiouInMain_Before[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeJudgeResultOfNPCNaiouInMain_Before() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeJudgeResultOfNPCNaiouInMain_Before;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeJudgeResultOfNPCNaiouInMain_Before + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeJudgeResultOfNPCNaiouInMain_Before  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromJudgeResultOfNPCNaiouInMain_Before+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeJudgeResultOfNPCNaiouInMain_Before), cmdOnTenshouExeJumpFromJudgeResultOfNPCNaiouInMain_Before, 5, NULL); //5バイトのみ書き込む
}







//----------------------------------------------------------------------------------------------




static int iResultOfJudgeResultOfNPCNaiouInMain = 1; // 結果

extern int GetJudgeResultOfNaiouInMain(int iActiveBushou, int iPassiveBushou, int iResult);

void OnTenshouExeJudgeResultOfNPCNaiouInMainExecute() {
	int iActiveBushou  = iActiveBushouOfJudgeResultOfNPCNaiouInMain - 1;   // 仕掛けた側
	int iPassiveBushou = iPassiveBushouOfJudgeResultOfNPCNaiouInMain - 1;  // 仕掛けられた側

	int iOverRide = GetJudgeResultOfNaiouInMain(iActiveBushou, iPassiveBushou, iResultOfJudgeResultOfNPCNaiouInMain);
	if (iOverRide != -1) {
		iResultOfJudgeResultOfNPCNaiouInMain = iOverRide;
	}
}


/*
00402901  |. E8 612D0A00    CALL    TENSHOU.004A5667
				└ここを JMP TSMod.OnTSExeJudgeResultOfNPCNaiouInMain と書き換えてやる実際の処理
00402906  |. 83C4 04        ADD     ESP, 4
*/
int pTenshouExeJumpFromToOnTenshouExeJudgeResultOfNPCNaiouInMain	 =0x402901; // 関数はこのアドレスから、OnTenshouExeJudgeResultOfNPCNaiouInMainへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeJudgeResultOfNPCNaiouInMain =0x4A5667; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeJudgeResultOfNPCNaiouInMain  =0x402906; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeJudgeResultOfNPCNaiouInMain() {
	// スタックにためておく
	__asm {

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeJudgeResultOfNPCNaiouInMain

		mov iResultOfJudgeResultOfNPCNaiouInMain, EDI
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

	OnTenshouExeJudgeResultOfNPCNaiouInMainExecute();

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

		mov EDI, iResultOfJudgeResultOfNPCNaiouInMain // 結果の上書き

		jmp pTenshouExeReturnLblFromOnTenshouExeJudgeResultOfNPCNaiouInMain
	}
}



char cmdOnTenshouExeJumpFromJudgeResultOfNPCNaiouInMain[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeJudgeResultOfNPCNaiouInMain() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeJudgeResultOfNPCNaiouInMain;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeJudgeResultOfNPCNaiouInMain + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeJudgeResultOfNPCNaiouInMain  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromJudgeResultOfNPCNaiouInMain+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeJudgeResultOfNPCNaiouInMain), cmdOnTenshouExeJumpFromJudgeResultOfNPCNaiouInMain, 5, NULL); //5バイトのみ書き込む
}


