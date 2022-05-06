#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "ScenarioMod.h"

#include "FuncSelectMenuDialog.h"

#include <algorithm>

using namespace std;


/*
セーブデータをロードする際に、名前(savedat.n6p)が決まる時。

00479C3B  |. 68 708D4C00    PUSH    TENSHOU.004C8D70                 ;  ASCII "A:SAVEDAT.N6P"
00479C40  |. 56             PUSH    ESI
00479C41  |. E8 A7350300    CALL    TENSHOU.004AD1ED
00479C46  |. 83C4 08        ADD     ESP, 8
*/

#define SAVEDATA_MULTI_NUM  10
#define SAVEDATA_FILENAME_MAX_LEN	80
char szMultiSaveDataFileName[SAVEDATA_MULTI_NUM][SAVEDATA_FILENAME_MAX_LEN] = {
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
};

int iPushFilePointerAddressOfSaveFileName = NULL;
void OnTenshouExePushSaveFileNameExecute() {

	// シナリオタイトル名の更新。ScenarioModの関数を呼び出す
	if ( p_snOnViewingScenarioTitleList ) {
		 p_snOnViewingScenarioTitleList();
	}

	// まず０番目は常にデフォルトと同じ(A:SAVEDATA.N6P)とする。
	strcpy( szMultiSaveDataFileName[0], ((char *)GAMEDATASTRUCT_SAVEDATAFILENAME_ADDRESS)-2 ); // A:SAVEDATA.N6Pなので2文字分アドレス引く

	// デフォルトではこれを選択したファイル名としておく
	iPushFilePointerAddressOfSaveFileName = (int)szMultiSaveDataFileName[0]; // セーブデータファイル名配列の0番目の配列(デフォルト名)

	// savedat*.n6pというファイル名を探す
	vector<string> listTargetSaveFiles;


	WIN32_FIND_DATA ffd;
	// *Mod.dllという名前のファイル。
	HANDLE hFileList = FindFirstFile("savedat*.n6p", &ffd);

	// 存在した場合、読み込み対象とする
	if ( hFileList != INVALID_HANDLE_VALUE ) {
		do {
			char *p = ffd.cFileName;
			_strupr(p);

			// コピーという文字列が含まれたsavedat.n6p以外の時
			if ( ! strstr(p, "コピー") ){ 
				string filename = string("A:")+p;
				listTargetSaveFiles.push_back(filename);
			}

		} while (FindNextFile(hFileList, &ffd));
	}
	FindClose(hFileList);

	// szMultiSaveDataFileNameの最初の１つを除いてリセット
	for ( int i=1; i<SAVEDATA_MULTI_NUM; i++) {
		strcpy( szMultiSaveDataFileName[i], "");
	}

	// 見つかったファイル群の中から、デフォルト名である、A:SAVEDATA.N6Pを除くイタレータ
	vector<string>::iterator end_it = remove( listTargetSaveFiles.begin(), listTargetSaveFiles.end(), szMultiSaveDataFileName[0] );

	// まず、見つかったものにsavedat.n6pが含まれるかどうか
	int iSumOfList=1; // szMultiSaveDataFileNameへの配列。デフォルト以外なので1からスタート
	for ( vector<string>::iterator it = listTargetSaveFiles.begin(); 
		  it != end_it && iSumOfList<SAVEDATA_MULTI_NUM; // end_it という上で計算した特殊なイタレータまで
		  it++, iSumOfList++ ) { // szMultiSaveDataFileNameの配列をはみ出さないように。

		strncpy( szMultiSaveDataFileName[iSumOfList], (*it).c_str(), SAVEDATA_FILENAME_MAX_LEN-2);
		szMultiSaveDataFileName[iSumOfList][SAVEDATA_FILENAME_MAX_LEN-1] = NULL; // アホみたいな長い名前でもOKなように番兵
	}

	// 結局存在するセーブデータが１つより大きいならばメニュー化
	if ( iSumOfList > 1 ) {
		vector<string>  menu;
		for ( int i=0; i<SAVEDATA_MULTI_NUM; i++ ) {
			// 有効な文字列であれば…
			if ( strlen(szMultiSaveDataFileName[i]) > 3 ) {

				// メニューを作成する。

				// メニュー用の文字列に加工
				string menu_title = ((char *)szMultiSaveDataFileName[i])+2; // 最初の"A:"を除去するため。
				menu_title = "　" + menu_title + "　"; // 前後に空白
				// プッシュ
				menu.push_back( menu_title );
			}
		}

		// メニュー表示
		int iSelect = 0;
		
		while ( 1 ) {
			iSelect = SelectMenuDialog(menu);
			if ( iSelect != 0xFFFF ) { break; } // 有効なメニューを選んだ

		}

		// 有効なメニュー番号を選んだ
		if ( iSelect < (int)menu.size() ) {
			// 選択した文字列へのポインタを数字として、push用として格納
			iPushFilePointerAddressOfSaveFileName = (int)szMultiSaveDataFileName[iSelect];

			// セーブデータのファイル名
			psznb6savedatafilename = (char *)iPushFilePointerAddressOfSaveFileName+2; // セーブデータエディタ用。ファイル名だけなので+2

		}
	}
}



/*
00479C3B  |. 68 708D4C00    PUSH    TENSHOU.004C8D70                 ;  ASCII "A:SAVEDAT.N6P"
				└ここを JMP TSMod.OnTenshouExePushSaveFileName と書き換えてやる実際の処理
00479C40  |. 56             PUSH    ESI
00479C41  |. E8 A7350300    CALL    TENSHOU.004AD1ED
00479C46  |. 83C4 08        ADD     ESP, 8
*/
int pTenshouExeJumpFromToOnTenshouExePushSaveFileName	  = 0x479C3B; // 関数はこのアドレスから、OnTenshouExePushSaveFileNameへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExePushSaveFileName  = 0x479C40; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExePushSaveFileName() {
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
	
	OnTenshouExePushSaveFileNameExecute();

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

		// 元々tenshou.exe にあった処理をここに。
		PUSH    iPushFilePointerAddressOfSaveFileName

		jmp pTenshouExeReturnLblFromOnTenshouExePushSaveFileName
	}
}




char cmdOnTenshouExeJumpFromPushSaveFileName[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計5バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExePushSaveFileName() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExePushSaveFileName;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExePushSaveFileName + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExePushSaveFileName  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromPushSaveFileName+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExePushSaveFileName), cmdOnTenshouExeJumpFromPushSaveFileName, 5, NULL); //5バイトのみ書き込む
}




