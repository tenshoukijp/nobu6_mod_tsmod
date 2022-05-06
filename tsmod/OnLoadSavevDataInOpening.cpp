#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "SetBushouAvgParam.h"

/*
オープニングでロード決定した。
*/
/*
0049D84E  |. E8 BFC3FDFF    |CALL    TENSHOU.00479C12← この関数の結果EAXが１だったらオープニングでロードした。
0049D853  |. 83C4 04        |ADD     ESP, 4
0049D856  |. 85C0           |TEST    EAX, EAX			← このEAXが１だったらロードした
0049D858  |.^0F84 40FFFFFF  |JE      TENSHOU.0049D79E
0049D85E  |. 68 00020000    |PUSH    200
0049D863  |. E8 4B97FCFF    |CALL    TENSHOU.00466FB3
0049D863  |. E8 4B97FCFF    |CALL    TENSHOU.00466FB3
0049D868  |. 83C4 04        |ADD     ESP, 4
*/

//-------------------------------------------------------------------------------------

// 年・季節がどこかで宣言されている。
extern int nb6progressseason;

// １つ前の季節の状態
extern char pre_check_season;

extern int getSelectSaveOrLoadNo();

void OnTenshouExeLoadSaveDataInOpeningExecute() {
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
0049D84E  |. E8 BFC3FDFF    |CALL    TENSHOU.00479C12← この関数の結果EAXが１だったらオープニングでロードした。
0049D853  |. 83C4 04        |ADD     ESP, 4
0049D856  |. 85C0           |TEST    EAX, EAX			← このEAXが１だったらロードした
0049D858  |.^0F84 40FFFFFF  |JE      TENSHOU.0049D79E
0049D85E  |. 68 00020000    |PUSH    200
0049D863  |. E8 4B97FCFF    |CALL    TENSHOU.00466FB3
				└ここを JMP TSMod.OnTSExeLoadSaveDataInOpening と書き換えてやる実際の処理
0049D868  |. 83C4 04        |ADD     ESP, 4
*/
int pTenshouExeJumpFromToOnTenshouExeLoadSaveDataInOpening		=0x49D863; // 関数はこのアドレスから、OnTenshouExeLoadSaveDataInOpeningへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeLoadSaveDataInOpening = 0x466FB3; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeLoadSaveDataInOpening	=0x49D868; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeLoadSaveDataInOpening() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeLoadSaveDataInOpening

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
	OnTenshouExeLoadSaveDataInOpeningExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeLoadSaveDataInOpening
	}
}

char cmdOnTenshouExeJumpFromLoadSaveDataInOpening[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeLoadSaveDataInOpening() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeLoadSaveDataInOpening;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeLoadSaveDataInOpening + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeLoadSaveDataInOpening  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromLoadSaveDataInOpening+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeLoadSaveDataInOpening), cmdOnTenshouExeJumpFromLoadSaveDataInOpening, 5, NULL); //5バイトのみ書き込む
}




