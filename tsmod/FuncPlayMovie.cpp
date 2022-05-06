#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"

#include "FuncPlayMovie.h"
#include "GameDataStruct.h"
#include "SetExtensionMovie.h"
#include "OnigRegex.h"
#include "time.h"


int pCallFromFuncPlayMovie = 0x43CD64;
void FuncPlayMovieAsm() {
	__asm {
		PUSH    1
		PUSH    1						// これがムービー番号である。
		CALL    pCallFromFuncPlayMovie
		ADD     ESP, 8
	}
}

char *pOriginalAdditionalPoiter = NULL;
BOOL isMovieWithBlackBack = TRUE; // 　ブラックバックするべきでない。デフォルトではブラックバックするので、これはFALSE
BOOL isMovieWithBGMSTOP = TRUE;	 // 　ＢＧＭは止めるべきでない。デフォルトではＢＧＭは止めるので、これはFALSE

void FuncPlayMovie(string strMovie, BOOL isBlackBack, BOOL isBGMStop) {

	// オリジナルの１番目(コーエーロゴ)のムービーポインタを控えておく
	pOriginalAdditionalPoiter = nb6_moviepointer_ex[1].pAddressMovieName;

	// 振ってきた文字列のポインタを１番目のムービーとしてセット
	nb6_moviepointer_ex[1].pAddressMovieName = (char *)strMovie.c_str();

	isMovieWithBlackBack = isBlackBack; // ブラックバックするかどうかを上書きする。
	isMovieWithBGMSTOP = isBGMStop; // ＢＧＭを止めるかどうかを上書きする。

	FuncPlayMovieAsm(); // ムービーを再生する。
	// 再生が終わったのでポインタを元へと戻す

	isMovieWithBlackBack = TRUE; // ブラックバックをする状態へと戻す。
	isMovieWithBGMSTOP = TRUE; // ＢＧＭを止める状態へと戻す。

	// ムービーの名前を元へと戻す。
	nb6_moviepointer_ex[1].pAddressMovieName = pOriginalAdditionalPoiter;

}



// 動画指定が、正規表現パターンマッチで行われている場合は、１つを選択する。HDD内のmovieフォルダ内が対象となる。
string ChoiceMovieFromRegex(string strMovieRegexPattern) {

	// 対象となるムービーのリスト
	vector<string> movie_list;

	WIN32_FIND_DATA ffd;

	// movieフォルダにある*.aviという名前のファイル。
	HANDLE hFileList = FindFirstFile("movie\\*.avi", &ffd);

	// 存在した場合、読み込み対象とする
	if ( hFileList != INVALID_HANDLE_VALUE ) {

		do {
			// 文字揺れ防止のため、小文字化する。
			char filename[512] = "";
			strcpy(filename, _strlwr(ffd.cFileName));

			Matches movie_matches;

			// ".avi"の部分を削除
			if ( strlen(filename)-4 >= 0 ) {
				filename[strlen(filename)-4] = NULL; // 最後の.aviを削除
			}

			// パターンにマッチするものを探す
			if ( OnigMatch( filename, strMovieRegexPattern, &movie_matches ) ) {

				// マッチするものはリストに追加
				movie_list.push_back(string(filename));
			} 

		} while (FindNextFile(hFileList, &ffd));
	}
	FindClose(hFileList);

	// nb6moviepointer_exの0番目はNULLなので、1からスタート。
	for ( int i=1; i< GAMEDATASTRUCT_MOVIE_NUM; i++ ) {
		// ＣＤ－ＲＯＭのムービーリストとマッチしたら、文字列を返す。
		// パターンにマッチするものを探す
		if ( OnigMatch( nb6_moviepointer_ex[i].pAddressMovieName, strMovieRegexPattern ) ) {

			// マッチするものはリストに追加
			movie_list.push_back(string(nb6_moviepointer_ex[i].pAddressMovieName));
		}
	}

	// パターンにマッチするものが１つもなければ、
	if ( movie_list.size() == 0 ) {

		// 対応するものが無かった場合は、stringを空相当にする。
		return string("");

	} else {
		int irandom = rand() % movie_list.size();

		return movie_list[irandom];
	}
}


// 外部からの呼び出し用の口。
void WINAPI Extern_FuncPlayMovie(char *szMovieName, int iOptions) {

	// ムービーの指定は正規表現パターンかもしれないので、それがあるかどうかチェック。
	string strMovieRegexPattern = ChoiceMovieFromRegex(szMovieName);

	// ストリングが空相当だったら、ムービーは再生しない。不正な場合も(ChoiceMovieFromRegex)内で空相当になる。
	if ( strMovieRegexPattern.empty() ) { return; }

	// 黒バック系のScenarioModから渡ってくるオプション。ビット演算して値を反対にする必要がある。
	int isBlackBackOption = !(iOptions & 0x01);
	// ＢＧＭ停止系のScenarioModから渡ってくるオプション。ビット演算して値を反対にする必要がある。
	int isBGMSTOPOption = !(iOptions & 0x02);

	FuncPlayMovie(strMovieRegexPattern, isBlackBackOption, isBGMSTOPOption);
}