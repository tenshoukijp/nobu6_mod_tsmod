
#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "SetBushouAvgParam.h"

/*
メインゲーム内でセーブした
*/
/*
00479E61  |. E8 ACFDFFFF    CALL    TENSHOU.00479C12 ← この関数の結果EAXが１だったらセーブした。
00479E66  |. 83C4 04        ADD     ESP, 4
00479E69  |. 33C0           XOR     EAX, EAX			← このEAXが１だったらセーブした
00479E6B  \. C3             RETN
*/

//-------------------------------------------------------------------------------------
// セーブロードダイアログで選択している時だけ使える。
extern int getSelectSaveOrLoadNo();

int isSaveExecuteOfSaveSaveDataInGame = 0;
void OnTenshouExeSaveSaveDataInGameExecute() {
	if ( isSaveExecuteOfSaveSaveDataInGame ) {
		// ★★ここに、セーブした時の拡張データ処理
		{
	
			int iSelectID = getSelectSaveOrLoadNo();

			// エラーではない。
			if ( iSelectID != -1 ) {
				// OutputDebugStream << iSelectID << psznb6savedatafilename << endl;
				SaveSaveDataEx(iSelectID );
			}
		}
	}
}


/*
00479E61  |. E8 ACFDFFFF    CALL    TENSHOU.00479C12 ← この関数の結果EAXが１だったらセーブした。
				└ここを JMP TSMod.OnTSExeSaveSaveDataInGame と書き換えてやる実際の処理
00479E66  |. 83C4 04        ADD     ESP, 4
00479E69  |. 33C0           XOR     EAX, EAX			← このEAXが１だったらセーブした
00479E6B  \. C3             RETN
*/
int pTenshouExeJumpFromToOnTenshouExeSaveSaveDataInGame		=0x479E61; // 関数はこのアドレスから、OnTenshouExeSaveSaveDataInGameへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeSaveSaveDataInGame =0x479C12; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeSaveSaveDataInGame	=0x479E66; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeSaveSaveDataInGame() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeSaveSaveDataInGame

		mov isSaveExecuteOfSaveSaveDataInGame, EAX // ちゃんとセーブしたなら1が、キャンセル等でセーブしてないなら0が入る。

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
	OnTenshouExeSaveSaveDataInGameExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeSaveSaveDataInGame
	}
}

char cmdOnTenshouExeJumpFromSaveSaveDataInGame[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeSaveSaveDataInGame() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeSaveSaveDataInGame;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSaveSaveDataInGame + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeSaveSaveDataInGame  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromSaveSaveDataInGame+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSaveSaveDataInGame), cmdOnTenshouExeJumpFromSaveSaveDataInGame, 5, NULL); //5バイトのみ書き込む
}




