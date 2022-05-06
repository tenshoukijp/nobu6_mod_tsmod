#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"

/*
 メイン画面中の城ターンが切り替わった時。
 １軍団ターン内に同じ城が何回か来ることもある
 */


/*
004670FB  |. 77 08          JA      SHORT TENSHOU.00467105
004670FD  |. 66:A3 59934C00 MOV     WORD PTR DS:[4C9359], AX	← ここでnb6turn.castle に値が入れられる。
00467103  |. EB 11          JMP     SHORT TENSHOU.00467116
*/

int iCastleValueOfCastleTurnChange = -1;
void OnTenshouExeCastleTurnChangeExecute() {
	iCastleValueOfCastleTurnChange = (iCastleValueOfCastleTurnChange & 0xFFFF) - 1; // AXが値なので、下位ビットだけ利用する。

	if ( 0 <= iCastleValueOfCastleTurnChange && iCastleValueOfCastleTurnChange < GAMEDATASTRUCT_CASTLE_NUM ) {
		// 城の切り替わり目の処理
		// ◎何か処理を入れる時はココ
	}

}


/*
004670FB  |. 77 08          JA      SHORT TENSHOU.00467105
004670FD  |. 66:A3 59934C00 MOV     WORD PTR DS:[4C9359], AX	← ここでnb6turn.castle に値が入れられる。
				└ここを JMP TSMod.OnTSExeCastleTurnChange と書き換えてやる実際の処理
00467103  |. EB 11          JMP     SHORT TENSHOU.00467116
*/
int pTenshouExeJumpFromToOnTenshouExeCastleTurnChange    =0x4670FD; // 関数はこのアドレスから、OnTenshouExeCastleTurnChangeへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeCastleTurnChange =0x467103; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeCastleTurnChange() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		MOV     WORD PTR DS:[0x4C9359], AX

		// 軍団番号を保存しておく。
		mov iCastleValueOfCastleTurnChange, eax

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
	OnTenshouExeCastleTurnChangeExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeCastleTurnChange
	}
}



char cmdOnTenshouExeJumpFromCastleTurnChange[7]="\xE9\x90\x90\x90\x90\x90";
// 元の命令が5バイト、以後の関数で生まれる命令が合計６バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeCastleTurnChange() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeCastleTurnChange;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeCastleTurnChange + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeCastleTurnChange  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromCastleTurnChange+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeCastleTurnChange), cmdOnTenshouExeJumpFromCastleTurnChange, 6, NULL); //6バイトのみ書き込む
}




