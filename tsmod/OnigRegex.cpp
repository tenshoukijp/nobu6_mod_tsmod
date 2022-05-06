#pragma pack(1)

#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <map>

#include "OnigRegex.h"
#include "OutputDebugStream.h"

using namespace std;


// 正規表現マッチング用の関数
// @str_target

int OnigMatch( string str_target, string str_regex, Matches* pmatches) {

	// まず、検索結果は削除
	if ( pmatches ) {
		pmatches->clear();
	}

	// どちらかが中身からなら、falseを返す。
	if ( str_target.length() == 0 || str_regex.length() == 0 ) {
		return false;
	}

	int r;
	OnigUChar *start, *range, *end;
	regex_t* reg;
	OnigErrorInfo einfo;
	OnigRegion *region;

	UChar* pattern = (UChar* )str_regex.c_str(); // "あ(.+?)(うえ)お|[あ-う]+";
	UChar* str     = (UChar* )str_target.c_str(); // "きあ\x0Aいう\x0Aえおか";

	// 正規表現の作成。 SJIS
	r = onig_new(&reg, pattern, pattern + strlen((char* )pattern),
		ONIG_OPTION_IGNORECASE|ONIG_OPTION_MULTILINE, ONIG_ENCODING_SJIS, ONIG_SYNTAX_DEFAULT, &einfo);

	if (r != ONIG_NORMAL) {
		char s[ONIG_MAX_ERROR_MESSAGE_LEN];
		onig_error_code_to_str((OnigUChar *)s, r, &einfo);
		OutputDebugStream("正規表現コンパイルエラー: %s\n", s);
		return false;
	}

	region = onig_region_new();

	end   = str + strlen((char* )str);
	start = str;
	range = end;

	// 正規表現で検索する。
	r = onig_search(reg, str, end, start, range, region, ONIG_OPTION_NONE);
	if (r >= 0) {
		int i;

		// マッチした部分全体と、( )の中身の表示。
		// fprintf(stderr, "match at %d\n", r);
		for (i = 0; i < region->num_regs; i++) {

			if ( pmatches ) {
				int len = region->end[i]-region->beg[i];
				// 文字をコピーするための領域を動的に確保
				char *cp = new char[len+1]; // 最初と最後の差分を確保。endが１つ多めなので、これでNULL文字分も確保されている。

				// 文字列をコピー
				// 　　　　　　┌コピー開始位置      ┌コピーバイト数
				strncpy( cp, (char *)(str)+region->beg[i], len );
				cp[len] = '\0'; // 最後にNULL文字を入れる。

				// OutputDebugStream("%s", cp);
				pmatches->insert( Matches::value_type( i, cp ) );

				// 動的な確保領域解放
				delete [] cp;
			}
		}
	}
	else if (r == ONIG_MISMATCH) {
		// OutputDebugStream("search fail\n");
	}
	else { /* error */
		char s[ONIG_MAX_ERROR_MESSAGE_LEN];
		onig_error_code_to_str((OnigUChar *)s, r);
		// OutputDebugStream("正規表現マッチ実行エラー: %s\n", s);
		return false;
	}

	onig_region_free(region, 1 /* 1:free self, 0:free contents only */);
	onig_free(reg);

	if ( r >= 0 ) {
		return true;
	} else {
		return false;
	}
}

// 結果格納用変数用意
static map<int, string> extern_matches;

// 上記関数の外部参照用
int WINAPI Extern_OnigMatchAlloc(char *str_target, char *str_regex, int *pmatches) {

	int ret = false;
	// 結果格納用変数が有効ならば…
	if (pmatches) {
		// 前回の格納結果消去
		extern_matches.clear();

		ret = OnigMatch(str_target, str_regex, &extern_matches); // 結果を格納

		// 何か有効な情報が格納されている。
		if (extern_matches.size() > 0) {

			int imax = -1; // mapに格納されている最大のindex整数を求める。メモリ領域確保数決定のため。
			// マッチしている分･･･
			for (map<int, string>::iterator it = extern_matches.begin(); it != extern_matches.end(); it++) {
				// キーの値が、imaxより大きければ、それが最大のキーの数値
				if (it->first > imax ) {
					imax = it->first;
				}
			}

			// 最大のindexが0以上であれば、mapに中身が１つ以上入ってる。
			if (imax >= 0 ) {

				// 最小から最大まで。
				for ( int i=0; i<=imax; i++) {
					// もし対象のキーが存在しなければ…
					if (extern_matches.count(i) == 0) {
						pmatches[i] = NULL; // もともとそのキーは有効ではない。
					} else {
						pmatches[i] = (int)(extern_matches[i].c_str()); // char *のアドレスをintの数値アドレスへとする。
					}
				}
				pmatches[imax+1] = 0xFFFF; // 最後に番兵用の値
			}
		}

	} else {
		ret = OnigMatch(str_target, str_regex); // マッチのみ
	}

	return ret;
}

void WINAPI Extern_OnigMatchFree(int *pmatches) {
}
