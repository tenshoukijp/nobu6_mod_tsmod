
#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "TSModCommand.h"


/*
 武将が浪人になった時の勲功や仕官年数の操作。戦争時版
*/
/*
00429438  |. 53             PUSH    EBX
00429439  |. 57             PUSH    EDI
0042943A  |. E8 6605FEFF    CALL    TENSHOU.004099A5		EDIが対象の武将
0042943F  |. 83C4 08        ADD     ESP, 8
*/


int iCurBushouIDInUpdateToRoninOfWarKunkouParam = -1;
int iCurBushouKunkouValueInUpdateToRoninOfWarKunkouParam = -1;

void OnTenshouExeUpdateToRoninOfWarKunkouParamExecuteBefore() { // 前

	int iBushouID = iCurBushouIDInUpdateToRoninOfWarKunkouParam-1;

	// 勲功リセットしない
	if ( TSModCommand::AllBushou::cmdNonMeritoriousReset ) {
		// 武将が一定の範囲であること
		if (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {
			if ( nb6bushouname[iBushouID].State == 4 ) { // 浪人にされた時
				iCurBushouKunkouValueInUpdateToRoninOfWarKunkouParam = nb6bushouref[iBushouID].merits; // 勲功値を一端保存
			}
		}
	}
}

void OnTenshouExeUpdateToRoninOfWarKunkouParamExecuteAfter() { // 後

	int iBushouID = iCurBushouIDInUpdateToRoninOfWarKunkouParam-1;

	// 勲功リセットしない
	if ( TSModCommand::AllBushou::cmdNonMeritoriousReset ) {
		// 武将が一定の範囲であること
		if (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {
			if ( nb6bushouname[iBushouID].State == 4 ) { // 浪人にされた時
				nb6bushouref[iBushouID].merits = (WORD)(iCurBushouKunkouValueInUpdateToRoninOfWarKunkouParam * 0.7); // 勲功値を復元。0.65倍
			}
		}
	}
}

/*
00429438  |. 53             PUSH    EBX
00429439  |. 57             PUSH    EDI
0042943A  |. E8 6605FEFF    CALL    TENSHOU.004099A5		EDIが対象の武将
				└ここを JMP TSMod.OnTSExeUpdateToRoninOfWarKunkouParam と書き換えてやる実際の処理
0042943F  |. 83C4 08        ADD     ESP, 8
*/
int pTenshouExeJumpFromToOnTenshouExeUpdateToRoninOfWarKunkouParam	    =0x42943A; // 関数はこのアドレスから、OnTenshouExeUpdateToRoninOfWarKunkouParamへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeUpdateToRoninOfWarKunkouParam  =0x4099A5; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeUpdateToRoninOfWarKunkouParam   =0x42943F; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeUpdateToRoninOfWarKunkouParam() {
	// スタックにためておく
	__asm {
		// 武将IDを保存
		mov iCurBushouIDInUpdateToRoninOfWarKunkouParam, edi

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
	OnTenshouExeUpdateToRoninOfWarKunkouParamExecuteBefore();

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
		call pTenshouExeJumpCallFromToOnTenshouExeUpdateToRoninOfWarKunkouParam

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

	OnTenshouExeUpdateToRoninOfWarKunkouParamExecuteAfter();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeUpdateToRoninOfWarKunkouParam
	}
}

/*
*/
char cmdOnTenshouExeJumpFromUpdateToRoninOfWarKunkouParam[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeUpdateToRoninOfWarKunkouParam() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeUpdateToRoninOfWarKunkouParam;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeUpdateToRoninOfWarKunkouParam + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeUpdateToRoninOfWarKunkouParam  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromUpdateToRoninOfWarKunkouParam+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeUpdateToRoninOfWarKunkouParam), cmdOnTenshouExeJumpFromUpdateToRoninOfWarKunkouParam, 5, NULL); //5バイトのみ書き込む
}




