#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "TSModCommand.h"

/*
00413B5C   . 3D 82000000    CMP     EAX, 82								130の国
00413B61   . 73 07          JNB     SHORT TENSHOU.00413B6A								
00413B63   . 33C0           XOR     EAX, EAX								
00413B65   . E9 37030000    JMP     TENSHOU.00413EA1								
00413B6A   > 6A 04          PUSH    4								
00413B6C   . 56             PUSH    ESI								
00413B6D   . E8 0E190900    CALL    TENSHOU.004A5480								
00413B72   . 83C4 08        ADD     ESP, 8								
00413B75   . 85C0           TEST    EAX, EAX								
00413B77   . 74 07          JE      SHORT TENSHOU.00413B80								
00413B79   . 33C0           XOR     EAX, EAX								
00413B7B   . E9 21030000    JMP     TENSHOU.00413EA1								
00413B80   > 56             PUSH    ESI								
00413B81   . E8 251B0900    CALL    TENSHOU.004A56AB								
00413B86   . 66:8BD8        MOV     BX, AX								
00413B89   . 83C4 04        ADD     ESP, 4								
00413B8C   . 56             PUSH    ESI								
00413B8D   . E8 48140900    CALL    TENSHOU.004A4FDA								
00413B92   . 66:A3 907C4D00 MOV     WORD PTR DS:[4D7C90], AX								
00413B98   . 83C4 04        ADD     ESP, 4								
00413B9B   . 66:8BF8        MOV     DI, AX								
00413B9E   . 66:81FB 8000   CMP     BX, 80								128（＝室町御所＝二条所)の支配下
00413BA3   . 74 32          JE      SHORT TENSHOU.00413BD7								
00413BA5   . 66:81FB B500   CMP     BX, 0B5								江戸城(=181)
00413BAA   . 75 0E          JNZ     SHORT TENSHOU.00413BBA								
00413BAC   . 57             PUSH    EDI								
00413BAD   . E8 F65EFFFF    CALL    TENSHOU.00409AA8								
00413BB2   . 83C4 04        ADD     ESP, 4								
*/

/*
 * 征夷大将軍になる条件のうち、最低城数が判定された時
 */
// 引数としてわたってきた判定対象の軍団ID
int iSeiiTaishoConditionCastleNumDaimyoID = -1;
// 一番支配下におさめているところが多い勢力の実際の現在の城数
int iSeiiTaishoConditionCastleNum = -1;
int iSeiiTaishoConditionCastleNeedNum = 130;

// 城数が征夷大将軍として合格しているか？
int iSeiiTaishoConditionCastleNumOver = 0;


void OnTenshouExeSeiiTaishoConditionCastleNumExecute() {

	// 「城数が征夷大将軍として合格しているか？」は毎度リセット
	iSeiiTaishoConditionCastleNumOver = 0;

	// TSMod.txtにて、最低城数が指定されていれば、それを必要数のデフォルト数とするとする
	if ( TSModCommand::AllDaimyo::cmdSeiiTaishogunNeedCastleNum > 0 ) {
		iSeiiTaishoConditionCastleNeedNum = TSModCommand::AllDaimyo::cmdSeiiTaishogunNeedCastleNum;
	}
	// 必要数を満たしていればＯＫ．
	if ( iSeiiTaishoConditionCastleNum >= iSeiiTaishoConditionCastleNeedNum ) {
		iSeiiTaishoConditionCastleNumOver = 1;
	}

}


/*
00413B5C   . 3D 82000000    CMP     EAX, 82								0x82＝130の国を所持	
				└ここを JMP TSMod.OnTSExeSeiiTaishoConditionCastleNum と書き換えてやる実際の処理
00413B61   . 73 07          JNB     SHORT TENSHOU.00413B6A				(より小さくなければ==130以上であれば TENSHOU.00413B6Aへ	
00413B63   . 33C0           XOR     EAX, EAX									
00413B65   . E9 37030000    JMP     TENSHOU.00413EA1									
 */

int pTenshouExeJumpFromToOnTenshouExeSeiiTaishoConditionCastleNum    =0x413B5C; // 関数はこのアドレスから、OnTenshouExeSeiiTaishoConditionCastleNumへとジャンプしてくる。
int pTenshouExeJumpFromToOnTenshouExeSeiiTaishoConditionCastleNumOver=0x413B6A; // 130城以上だった場合のジャンプ先のアドレス
int pTenshouExeReturnLblFromOnTenshouExeSeiiTaishoConditionCastleNum =0x413B63; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w

// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeSeiiTaishoConditionCastleNum() {
	// スタックにためておく
	__asm {
		// 現在の判定の対象となる大名IDを控えておく
		mov iSeiiTaishoConditionCastleNumDaimyoID, esi;

		// 一番多い勢力の城数
		mov iSeiiTaishoConditionCastleNum, eax;

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
	OnTenshouExeSeiiTaishoConditionCastleNumExecute();

	// もし征夷大将軍として必要とされる城数以上であれば…
	if ( iSeiiTaishoConditionCastleNumOver ) {
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

			jmp pTenshouExeJumpFromToOnTenshouExeSeiiTaishoConditionCastleNumOver
		}
	} else {
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

			jmp pTenshouExeReturnLblFromOnTenshouExeSeiiTaishoConditionCastleNum
		}
	}
}

/*
00444769  |. 50             PUSH    EAX
0044476A  |. E8 3A2C0200    CALL    TENSHOU.004673A9
				└ここを JMP TSMod.OnTSExeSeiiTaishoConditionCastleNum と書き換えてやる実際の処理
0044476F  |. 83C4 04        ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromSeiiTaishoConditionCastleNum[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeSeiiTaishoConditionCastleNum() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeSeiiTaishoConditionCastleNum;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSeiiTaishoConditionCastleNum + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeSeiiTaishoConditionCastleNum  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromSeiiTaishoConditionCastleNum+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSeiiTaishoConditionCastleNum), cmdOnTenshouExeJumpFromSeiiTaishoConditionCastleNum, 5, NULL); //5バイトのみ書き込む
}













