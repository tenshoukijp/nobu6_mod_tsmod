#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "ScenarioMod.h"

#include "FuncGundanCastleWindowRepaint.h"

/*
0041050B  |> 6A 00          /PUSH    0
0041050D  |. E8 C4780700    |CALL    TENSHOU.00487DD6
00410512  |. 83C4 04        |ADD     ESP, 4
00410515  |. E8 BDB30600    |CALL    TENSHOU.0047B8D7				← プレイヤがメイン画面で特に何も入力しておらず、控えている状態（メインコマンド控え直前を作る!!)
0041051A  |. 8BD8           |MOV     EBX, EAX
0041051C  |. 85DB           |TEST    EBX, EBX
0041051E  |. 74 2E          |JE      SHORT TENSHOU.0041054E
*/

extern int iEnterOfClickMainScreenMainCommand;
extern void OnLeaveMainPlayerCommand();

void OnTenshouExePrevMainPlayerCommandWaitExecute() {
	if ( iEnterOfClickMainScreenMainCommand ) {
		// 特定のコマンド実行中から出た時
		OnLeaveMainPlayerCommand();
		iEnterOfClickMainScreenMainCommand = 0;

		// 金や兵など操作される可能性があるので、軍団ウィンドウ＋城ウィンドウを描画しなおす
		FuncGundanCastleWindowRepaint();
	}
}


/*

0041064C  |. E8 F6030000    |CALL    TENSHOU.00410A47	← メイン画面の城ウィンドウと軍団ウィンドウの更新
00410651  |. 56             |PUSH    ESI
00410652  |. E8 B7FC0500    |CALL    TENSHOU.0047030E
00410657  |. 83C4 04        |ADD     ESP, 4
0041065A  |. E8 87210400    |CALL    TENSHOU.004527E6
0041065F  |> 56             |PUSH    ESI
00410660  |. E8 783D0400    |CALL    TENSHOU.004543DD
00410665  |. 83C4 04        |ADD     ESP, 4
00410668  |. 85C0           |TEST    EAX, EAX
0041066A  |.^0F84 9BFEFFFF  \JE      TENSHOU.0041050B


0041050B  |> 6A 00          /PUSH    0
0041050D  |. E8 C4780700    |CALL    TENSHOU.00487DD6
00410512  |. 83C4 04        |ADD     ESP, 4
00410515  |. E8 BDB30600    |CALL    TENSHOU.0047B8D7				← プレイヤがメイン画面で特に何も入力しておらず、控えている状態（メインコマンド控え直前を作る!!)
				└ここを JMP TSMod.OnTSExePrevMainPlayerCommandWait と書き換えてやる実際の処理
0041051A  |. 8BD8           |MOV     EBX, EAX
0041051C  |. 85DB           |TEST    EBX, EBX
0041051E  |. 74 2E          |JE      SHORT TENSHOU.0041054E
*/

int pTenshouExeJumpFromToOnTenshouExePrevMainPlayerCommandWait		=0x410515; // 関数はこのアドレスから、OnTenshouExePrevMainPlayerCommandWaitへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExePrevMainPlayerCommandWait  =0x47B8D7; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExePrevMainPlayerCommandWait	=0x41051A; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExePrevMainPlayerCommandWait() {
	// スタックにためておく
	__asm {

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
	OnTenshouExePrevMainPlayerCommandWaitExecute();

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

		mov CommonGlobalHandle::isPlayerMainScreenInputWait, 1

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExePrevMainPlayerCommandWait

		mov CommonGlobalHandle::isPlayerMainScreenInputWait, 0

		jmp pTenshouExeReturnLblFromOnTenshouExePrevMainPlayerCommandWait
	}
}

char cmdOnTenshouExeJumpFromPrevMainPlayerCommandWait[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExePrevMainPlayerCommandWait() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExePrevMainPlayerCommandWait;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExePrevMainPlayerCommandWait + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExePrevMainPlayerCommandWait  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromPrevMainPlayerCommandWait+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExePrevMainPlayerCommandWait), cmdOnTenshouExeJumpFromPrevMainPlayerCommandWait, 5, NULL); //5バイトのみ書き込む
}


