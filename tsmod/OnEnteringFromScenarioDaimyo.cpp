#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#include <vector>
#include <string>

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "FuncPlayMovie.h"
#include "OnigRegex.h"
#include "FuncFadeInOut.h"

using namespace std;

/*
 * シナリオ大名選択時の処理
 */
bool isEnteringFromScenarioDaimyoExecute = false;

void OnTenshouExePlayScenarioDetailMovie() ;
void OnTenshouExeEnteringFromScenarioDaimyoExecute() {

	isEnteringFromScenarioDaimyoExecute = true;
}




/*
0049D814  |. 68 00040000    |PUSH    400
0049D819  |. E8 9597FCFF    |CALL    TENSHOU.00466FB3
				└ここを JMP TSMod.OnTSExeEnteringFromScenarioDaimyo と書き換えてやる実際の処理
0049D81E  |. 83C4 04        |ADD     ESP, 4
*/
int pTenshouExeJumpFromToOnTenshouExeEnteringFromScenarioDaimyo	 =0x49D819; // 関数はこのアドレスから、OnTenshouExeEnteringFromScenarioDaimyoへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeEnteringFromScenarioDaimyo =0x466FB3; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeEnteringFromScenarioDaimyo  =0x49D81E; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeEnteringFromScenarioDaimyo() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeEnteringFromScenarioDaimyo

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
	OnTenshouExeEnteringFromScenarioDaimyoExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeEnteringFromScenarioDaimyo
	}
}





char cmdOnTenshouExeJumpFromEnteringFromScenarioDaimyo[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeEnteringFromScenarioDaimyo() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeEnteringFromScenarioDaimyo;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeEnteringFromScenarioDaimyo + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeEnteringFromScenarioDaimyo  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromEnteringFromScenarioDaimyo+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeEnteringFromScenarioDaimyo), cmdOnTenshouExeJumpFromEnteringFromScenarioDaimyo, 5, NULL); //5バイトのみ書き込む
}




//----------------------------------------------------------------------------------------------------------------------
//------------------------------------------
// 上のすぐ後。ここは、シナリオ選択実行時共通とロード時と共通で流れてくる。フェイドアウト直前となる。

void OnTenshouExePlayScenarioDetailMovie(); // シナリオ解説用の動画を再生する。

void OnTenshouExeEnteringAfterScenarioDaimyoExecute() {

	// このタイミングで、シナリオ選択からやってきた場合のみシナリオ解説ムービーを流す。
	if ( isEnteringFromScenarioDaimyoExecute ) {
		OnTenshouExePlayScenarioDetailMovie();
	}
}

extern int iSelectingScenarioLocation; // シナリオの位置の番号。上から０．一番下が５

void OnTenshouExePlayScenarioDetailMovie() {

	// シナリオの位置番号が正当な範囲にあるか？
	int iSnNo = iSelectingScenarioLocation+1;
	
	if ( 1 <= iSnNo && iSnNo <= 6 ) {

		bool isScenarioMoviePlay = false; // フェイドアウトのため。
		// --------------------------------------------------
		// まずはシナリオ固有の(SN*_OPENING.avi)を再生する。
		struct stat  st;

		char szMovieName[256] = "";
		wsprintf(szMovieName, "SN%d_OPENING", iSnNo);

		char szMovieFullName[256] = "";
		wsprintf(szMovieFullName, "Movie\\SN%d_OPENING.avi", iSnNo);

		// そのムービーはハードディスクに存在するか
		if (stat(szMovieFullName, &st) == 0) {
			FuncPlayMovie(szMovieName);
			isScenarioMoviePlay = true;
		}


		// --------------------------------------------------
		// 次にシナリオ固有の(SN*_RANDOM??.avi)、ついで、シナリオ共通で(OPENING_SN_RND??.avi)を再生する。
		
		vector<string> ident_scenario_movie_file; // 特定のシナリオの中でのランダム再生ファイル
		vector<string> common_scenario_movie_file; // 特定のシナリオの中でのランダム再生ファイル

		WIN32_FIND_DATA ffd;
		// OPENING_SN*.aviという名前のファイル。
		HANDLE hFileList = FindFirstFile("movie\\*.avi", &ffd);

		// 存在した場合、読み込み対象とするが、正規表現でチェック。
		if ( hFileList != INVALID_HANDLE_VALUE ) {

			do {
				char filename[512] = "";
				strcpy(filename, _strlwr(ffd.cFileName));
				char szSnNo[5] = "";
				_itoa( iSnNo, szSnNo,10 );

				// 特定のシナリオ用のランダムムービー
				string strIdentMovieRegex = string("^(sn")+string(szSnNo)+string("_random.+?).avi$");

				// シナリオ用にかかわらない共通のランダムムービー
				string strCommonMovieRegex = string("^(sn")+string("_random.+?).avi$");

				Matches movie_matches;
				// 特定のシナリオ用のファイル名の正規表現パターンを調べる
				if ( OnigMatch( filename, strIdentMovieRegex, &movie_matches ) ) {
					// 存在していたなら、読んじゃったわけでして、後で走査するため登録しておく。
					ident_scenario_movie_file.push_back(string(movie_matches[1]));
				} 

				// シナリオ用にかかわらない共通のランダムのファイル名の正規表現パターンを調べる
				if ( OnigMatch( filename, strCommonMovieRegex, &movie_matches ) ) {
					// 存在していたなら、読んじゃったわけでして、後で走査するため登録しておく。
					common_scenario_movie_file.push_back(string(movie_matches[1]));
				}

			} while (FindNextFile(hFileList, &ffd));
		}
		FindClose(hFileList);

		// 特定のシナリオ用のランダムムービー(SN*_RANDOM??.avi)を再生する。
		if ( ident_scenario_movie_file.size() > 0 ) {
			int irand = rand() % ident_scenario_movie_file.size(); // 大きな整数乱数に対して、該当の正規表現があてはまったムービーの個数の余剰が乱数となる。
			FuncPlayMovie( ident_scenario_movie_file[irand] ); // ランダムで再生

			isScenarioMoviePlay = true;
		}

		// --------------------------------------------------
		// 次にシナリオ共通の(SN_RANDOM.avi)を再生する。
		if ( common_scenario_movie_file.size() > 0 ) {
			int irand = rand() % common_scenario_movie_file.size(); // 大きな整数乱数に対して、該当の正規表現があてはまったムービーの個数の余剰が乱数となる。
			FuncPlayMovie( common_scenario_movie_file[irand] ); // ランダムで再生

			isScenarioMoviePlay = true;
		}

		// ムービーを再生していると、フェイドアウトしていても、パッと画面が出てしまうので、改めてフェイドアウト。
		if ( isScenarioMoviePlay ) {
			FuncFadeOut();
		}
	}
}



/*
0049D81E  |. 83C4 04        |ADD     ESP, 4
0049D821  |> E8 C85AF8FF    |CALL    TENSHOU.004232EE
				└ここを JMP TSMod.OnTSExeEnteringAfterScenarioDaimyo と書き換えてやる実際の処理
0049D826  |. 8D4D A4        |LEA     ECX, DWORD PTR SS:[EBP-5C]
*/
int pTenshouExeJumpFromToOnTenshouExeEnteringAfterScenarioDaimyo	 =0x49D821; // 関数はこのアドレスから、OnTenshouExeEnteringAfterScenarioDaimyoへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeEnteringAfterScenarioDaimyo =0x4232EE; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeEnteringAfterScenarioDaimyo  =0x49D826; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeEnteringAfterScenarioDaimyo() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeEnteringAfterScenarioDaimyo // これは事実上フェイドアウト

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
	OnTenshouExeEnteringAfterScenarioDaimyoExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeEnteringAfterScenarioDaimyo
	}
}



char cmdOnTenshouExeJumpFromEnteringAfterScenarioDaimyo[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeEnteringAfterScenarioDaimyo() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeEnteringAfterScenarioDaimyo;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeEnteringAfterScenarioDaimyo + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeEnteringAfterScenarioDaimyo  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromEnteringAfterScenarioDaimyo+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeEnteringAfterScenarioDaimyo), cmdOnTenshouExeJumpFromEnteringAfterScenarioDaimyo, 5, NULL); //5バイトのみ書き込む
}

