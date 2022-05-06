#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "FuncSelectBushouDialog.h"

// 年・季節がどこかで宣言されている。

extern void writeUpdateGameDataStructSettingsEx();

char pre_check_season = 100;	// １つ前の時にチェックしたシーズンの値を確保しておく。シーズンの切り替わり目をつかむため。 季節は0～3なので、それ以外の正の値で初期化

extern int iEnterOfClickMainScreenMainCommand;

void OnTenshouExePlayerTurnChangeExecute() {

	// 特にこれの必要はないはずだが、一応念のため。
	CommonGlobalHandle::isBattleMode = 0;

	// プレイヤーターン時に行うサービス
	EXSuperService_OnPlayerTurnChange();

	// もしプレイヤーターンチェンジ時に季節も違っていたならば…
	if ( pre_check_season != nb6year.season ) {
		// 該当のシーズンを保存しておいて…
		pre_check_season = nb6year.season;

		// ターン切り替わり かつ シーズンも切り替わりのタイミングの処理を行う
		EXSuperService_OnSeasonChangeAndPlayerTurnChange();

		// ロードせずの季節経過数を1加算
		nb6progressseason++;

		iEnterOfClickMainScreenMainCommand = 0;
	}
}


/*
00410467  |. 66:8BF0        MOV     SI, AX
0041046A  |. BF FFFF0000    MOV     EDI, 0FFFF
				└ここを JMP TSMod.OnTSExePlayerTurnChange と書き換えてやる実際の処理
0041046F  |. E8 71CA0900    CALL    TENSHOU.004ACEE5
*/
int pTenshouExeJumpFromToOnTenshouExePlayerTurnChange    =0x41046A; // 関数はこのアドレスから、OnTenshouExePlayerTurnChangeへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExePlayerTurnChange =0x41046F; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExePlayerTurnChange() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		mov edi, 0xFFFF

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
	OnTenshouExePlayerTurnChangeExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExePlayerTurnChange
	}
}

/*
00410467  |. 66:8BF0        MOV     SI, AX
0041046A  |. BF FFFF0000    MOV     EDI, 0FFFF
				└ここを JMP TSMod.OnTSExePlayerTurnChange と書き換えてやる実際の処理
0041046F  |. E8 71CA0900    CALL    TENSHOU.004ACEE5
*/
char cmdOnTenshouExeJumpFromPlayerTurnChange[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExePlayerTurnChange() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExePlayerTurnChange;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExePlayerTurnChange + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExePlayerTurnChange  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromPlayerTurnChange+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExePlayerTurnChange), cmdOnTenshouExeJumpFromPlayerTurnChange, 5, NULL); //5バイトのみ書き込む
}




