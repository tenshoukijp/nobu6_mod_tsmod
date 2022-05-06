
#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "TSModCommand.h"


/*
 旗揚げ系の時、勲功等の操作
*/
/*
00442911  |> 50             PUSH    EAX
00442912  |. 56             PUSH    ESI
00442913  |. E8 8D70FCFF    CALL    TENSHOU.004099A5		ESIが対象武将
00442918  |. 83C4 08        ADD     ESP, 8
*/


int iCurBushouIDInUpdateToHataageKunkouParam = -1;
int iCurBushouKunkouValueInUpdateToHataageKunkouParam = -1;

void OnTenshouExeUpdateToHataageKunkouParamExecuteBefore() { // 前

	int iBushouID = iCurBushouIDInUpdateToHataageKunkouParam-1;

	// 勲功リセットしない
	if ( TSModCommand::AllBushou::cmdNonMeritoriousReset ) {
		// 特になし
	}
}

void OnTenshouExeUpdateToHataageKunkouParamExecuteAfter() { // 後

	int iBushouID = iCurBushouIDInUpdateToHataageKunkouParam-1;

	// 勲功リセットしない
	if ( TSModCommand::AllBushou::cmdNonMeritoriousReset ) {
		// 特になし
	}
}

/*
00442911  |> 50             PUSH    EAX
00442912  |. 56             PUSH    ESI
00442913  |. E8 8D70FCFF    CALL    TENSHOU.004099A5		ESIが対象武将
				└ここを JMP TSMod.OnTSExeUpdateToHataageKunkouParam と書き換えてやる実際の処理
00442918  |. 83C4 08        ADD     ESP, 8
*/
int pTenshouExeJumpFromToOnTenshouExeUpdateToHataageKunkouParam	     =0x442913; // 関数はこのアドレスから、OnTenshouExeUpdateToHataageKunkouParamへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeUpdateToHataageKunkouParam  =0x4099A5; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeUpdateToHataageKunkouParam   =0x442918; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeUpdateToHataageKunkouParam() {
	// スタックにためておく
	__asm {
		// 武将IDを保存
		mov iCurBushouIDInUpdateToHataageKunkouParam, esi

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
	OnTenshouExeUpdateToHataageKunkouParamExecuteBefore();

	__asm {
		// スタックを元に戻して実行
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeUpdateToHataageKunkouParam

		// 再びスタックにためる
		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnTenshouExeUpdateToHataageKunkouParamExecuteAfter();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeUpdateToHataageKunkouParam
	}
}

/*
*/
char cmdOnTenshouExeJumpFromUpdateToHataageKunkouParam[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeUpdateToHataageKunkouParam() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeUpdateToHataageKunkouParam;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeUpdateToHataageKunkouParam + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeUpdateToHataageKunkouParam  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromUpdateToHataageKunkouParam+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeUpdateToHataageKunkouParam), cmdOnTenshouExeJumpFromUpdateToHataageKunkouParam, 5, NULL); //5バイトのみ書き込む
}




