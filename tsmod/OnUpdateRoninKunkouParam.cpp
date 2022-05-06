
#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "TSModCommand.h"


/*
 武将が浪人になった時の勲功や仕官年数の操作
*/
/*
0040B421  |. 56             PUSH    ESI
0040B422  |. 57             PUSH    EDI
0040B423  |. E8 7DE5FFFF    CALL    TENSHOU.004099A5		EDIが対象の武将
0040B428  |. 83C4 08        ADD     ESP, 8
*/


int iCurBushouIDInUpdateToRoninKunkouParam = -1;
int iCurBushouKunkouValueInUpdateToRoninKunkouParam = -1;

void OnTenshouExeUpdateToRoninKunkouParamExecuteBefore() { // 前

	int iBushouID = iCurBushouIDInUpdateToRoninKunkouParam-1;

	// 勲功リセットしない
	if ( TSModCommand::AllBushou::cmdNonMeritoriousReset ) {

		// 武将が一定の範囲であること
		if (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {
			if ( nb6bushouname[iBushouID].State == 4 ) { // 浪人にされた時
				iCurBushouKunkouValueInUpdateToRoninKunkouParam = nb6bushouref[iBushouID].merits; // 勲功値を一端保存
			}
		}
	}
}

void OnTenshouExeUpdateToRoninKunkouParamExecuteAfter() { // 後

	int iBushouID = iCurBushouIDInUpdateToRoninKunkouParam-1;

	// 勲功リセットしない
	if ( TSModCommand::AllBushou::cmdNonMeritoriousReset ) {

		// 武将が一定の範囲であること
		if (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {
			if ( nb6bushouname[iBushouID].State == 4 ) { // 浪人にされた時
				nb6bushouref[iBushouID].merits = (WORD)(iCurBushouKunkouValueInUpdateToRoninKunkouParam * 0.7); // 勲功値を復元。0.7倍
			}
		}
	}
}

/*
0040B421  |. 56             PUSH    ESI
0040B422  |. 57             PUSH    EDI
0040B423  |. E8 7DE5FFFF    CALL    TENSHOU.004099A5
				└ここを JMP TSMod.OnTSExeUpdateToRoninKunkouParam と書き換えてやる実際の処理
0040B428  |. 83C4 08        ADD     ESP, 8
*/
int pTenshouExeJumpFromToOnTenshouExeUpdateToRoninKunkouParam	   =0x40B423; // 関数はこのアドレスから、OnTenshouExeUpdateToRoninKunkouParamへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeUpdateToRoninKunkouParam  =0x4099A5; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeUpdateToRoninKunkouParam   =0x40B428; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeUpdateToRoninKunkouParam() {
	// スタックにためておく
	__asm {
		// 武将IDを保存
		mov iCurBushouIDInUpdateToRoninKunkouParam, edi

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
	OnTenshouExeUpdateToRoninKunkouParamExecuteBefore();

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
		call pTenshouExeJumpCallFromToOnTenshouExeUpdateToRoninKunkouParam

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

	OnTenshouExeUpdateToRoninKunkouParamExecuteAfter();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeUpdateToRoninKunkouParam
	}
}

/*
*/
char cmdOnTenshouExeJumpFromUpdateToRoninKunkouParam[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeUpdateToRoninKunkouParam() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeUpdateToRoninKunkouParam;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeUpdateToRoninKunkouParam + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeUpdateToRoninKunkouParam  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromUpdateToRoninKunkouParam+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeUpdateToRoninKunkouParam), cmdOnTenshouExeJumpFromUpdateToRoninKunkouParam, 5, NULL); //5バイトのみ書き込む
}




