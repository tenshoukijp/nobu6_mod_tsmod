#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "SetBushouAvgParam.h"

/*
メインゲーム内でロード決定した
*/
/*
00479E97   . E8 76FDFFFF    CALL    TENSHOU.00479C12← この関数の結果EAXが１だったらゲーム内でロードした。
00479E9C   . 83C4 04        ADD     ESP, 4
00479E9F   . B9 18C24D00    MOV     ECX, TENSHOU.004DC218
00479EA4   . 85C0           TEST    EAX, EAX		← このEAXが１だったらロードした
00479EA6   . 74 2C          JE      SHORT TENSHOU.00479ED4
00479EA8   . E8 3CC3FEFF    CALL    TENSHOU.004661E9 ← ロードすればかならず最初にこの関数実行
*/

//-------------------------------------------------------------------------------------

// 年・季節がどこかで宣言されている。
extern int nb6progressseason;

// １つ前の季節の状態
extern char pre_check_season;

// セーブロードダイアログで選択している時だけ使える。
extern int getSelectSaveOrLoadNo();

void OnTenshouExeLoadSaveDataInGameExecute() {
	nb6progressseason = 1L;

	// １つ前の季節をあり得ない数値にしておく
	pre_check_season = 100;

	// ★★ここに、ロードした時の拡張データ処理
	{
		int iSelectID = getSelectSaveOrLoadNo();

		// エラーではない。
		if ( iSelectID != -1 ) {
			// OutputDebugStream << iSelectID << psznb6savedatafilename << endl;
			LoadSaveDataEx(iSelectID);
		}
	}
}


/*
00479E97   . E8 76FDFFFF    CALL    TENSHOU.00479C12← この関数の結果EAXが１だったらオープニングでロードした。
00479E9C   . 83C4 04        ADD     ESP, 4
00479E9F   . B9 18C24D00    MOV     ECX, TENSHOU.004DC218
00479EA4   . 85C0           TEST    EAX, EAX		← このEAXが１だったらロードした
00479EA6   . 74 2C          JE      SHORT TENSHOU.00479ED4
00479EA8   . E8 3CC3FEFF    CALL    TENSHOU.004661E9
				└ここを JMP TSMod.OnTSExeLoadSaveDataInGame と書き換えてやる実際の処理
00479EAD   . 68 00020000    PUSH    200
*/
int pTenshouExeJumpFromToOnTenshouExeLoadSaveDataInGame		=0x479EA8; // 関数はこのアドレスから、OnTenshouExeLoadSaveDataInGameへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeLoadSaveDataInGame =0x4661E9; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeLoadSaveDataInGame	=0x479EAD; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeLoadSaveDataInGame() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeLoadSaveDataInGame

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// ゲーム内画面でロード時に何かするならば…
	OnTenshouExeLoadSaveDataInGameExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeLoadSaveDataInGame
	}
}

char cmdOnTenshouExeJumpFromLoadSaveDataInGame[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeLoadSaveDataInGame() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeLoadSaveDataInGame;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeLoadSaveDataInGame + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeLoadSaveDataInGame  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromLoadSaveDataInGame+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeLoadSaveDataInGame), cmdOnTenshouExeJumpFromLoadSaveDataInGame, 5, NULL); //5バイトのみ書き込む
}




