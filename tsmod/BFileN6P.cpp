//
#define _CRT_SECURE_NO_WARNINGS
#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "ls11_mod.h"
#include "CnvHankakuZenkaku.h"
#include "GetOSVersion.h"

std::string HankakuKatakanaToZentakuKatakana(std::string szBufOriginal) {
	std::string szBufCurrent = szBufOriginal;

	Matches matches;

	// この特殊な記号は天翔記において、半角カタカナを全角カタカナにするという意味
	for (int i=0; OnigMatch(szBufCurrent, "^(.*?)\x1b\x4b(.*?)\x1b\x48(.*?)$", &matches) && i<(int)szBufCurrent.size(); i++ ) { 
		// 列伝から要素を抽出する。
		if ( !matches[2].empty() ) {
			char szBufZenKatakana[256] = "";
			hankana2zenkana((unsigned char *)matches[2].c_str(), (unsigned char *)szBufZenKatakana);
			szBufZenKatakana[255] = NULL; // 下手な修正しても暴走しないよう念のため

			szBufCurrent = matches[1] + szBufZenKatakana + matches[3];
		}
	}

	// 最後の閉じる記号がないまま行末を迎える場合がある。
	if ( OnigMatch(szBufCurrent, "^(.*?)\x1b\x4b(.+)$", &matches) ) {

		// 列伝から要素を抽出する。
		if ( !matches[2].empty() ) {
			char szBufZenKatakana[256] = "";
			hankana2zenkana((unsigned char *)matches[2].c_str(), (unsigned char *)szBufZenKatakana);
			szBufZenKatakana[255] = NULL; // 下手な修正しても暴走しないよう念のため

			szBufCurrent = matches[1] + szBufZenKatakana;
		}
	}

	return szBufCurrent;
}


// bfile.n6p の 情報を NB6BushouExoptionに格納する。
int setBfileN6PToNB6BUSHOUEXOPTION() {

	// ちょうど vBufDecodedData[0]=１番目の分割ファイルのデータ列、vBufDecodedData[1]=２番目の分割ファイルのデータ列、みたいな感じ
	vector<vector<byte>> vBufDecodedData;

	// 圧縮されたLS11フォーマットをデコードする関数
	ls11_DecodePack("bfile.n6p", NULL, &vBufDecodedData );

	int iRetsudenNo = -1;

	// デコードしたデータの(内部的)ファイルでまわしていく。0番は対象ではない。武将データは1番から。
	int ivBufDecodedDataSize = vBufDecodedData.size();

	for ( int ifile = 1; ifile < (int)ivBufDecodedDataSize ; ifile++ ) {

		// ちょうど vSplittedData[0]=１番目の要素のデータ列、vSplittedData[1]=２番目の要素のデータ列、みたいな感じ
		vector<vector<byte>> vSplittedData;

		ls11_SplitData( vBufDecodedData[ifile], &vSplittedData );

		int ivSplittedDataSize = vSplittedData.size();

		// 各要素ごとに･･･ まず名前、読み仮名 生没年の類
		for ( int ielem=0; ielem < (int)ivSplittedDataSize; ielem++ ) {

			if ( ielem%2==0 ) { iRetsudenNo++; }

			if ( iRetsudenNo < 0 ) { iRetsudenNo = 0; }; // 最悪のための番兵

			// 元来定義されている列伝数を超えたものは、無視
			if ( iRetsudenNo >= GAMEDATASTRUCT_RETSUDEN_NUM ) { continue; } // 1332人を超えると、ここより下にはいかない。カウンタだけまわす。

			// 偶数が読み仮名とか
			if ( ielem%2==0 ) {

				string szBufCurrent;
				int datasize = vSplittedData[ielem].size();
				try {
					if ( datasize > 0 ) {

						char *bytedata = new char[datasize+1];
						memcpy( bytedata, &vSplittedData[ielem][0], datasize );
						bytedata[datasize] = NULL; // 番兵
						szBufCurrent = string(bytedata); 
						delete[] bytedata;
					}
				} catch (exception e) {
				}

				Matches matches;
				// 列伝から要素を抽出する。
				if ( OnigMatch(szBufCurrent, "^(.*?) \x1b\x6b(.*?)\x1b\x48 ((.+)～(.+?))?\x05\x05\x05", &matches) ) {

					/*
					string name;		// 代表名
					string yomigana;	// よみがな
					string born;	    // 誕生年文字列(????)とかもあるため
					string death;	    // 死亡年文字列(????)とかもあるため
					string retsuden;	// 列伝文字列
					*/

					nb6bushouretsuden[iRetsudenNo].name = matches[1];
					if ( !matches[2].empty() ) {

						// 半角カタカナを全角ひらがなに
						char szBufZenHiranaga[128] = "";
						hankana2zenhira((unsigned char *)matches[2].c_str(), (unsigned char *)szBufZenHiranaga);
						szBufZenHiranaga[100] = NULL; // 下手な修正しても暴走しないよう念のため

						nb6bushouretsuden[iRetsudenNo].yomigana = string(szBufZenHiranaga);
					}
					if ( !matches[4].empty() ) {
						nb6bushouretsuden[iRetsudenNo].born = matches[4];
					}
					if ( !matches[5].empty() ) {
						nb6bushouretsuden[iRetsudenNo].death = matches[5];
					}
				}

			// 奇数は列伝。
			} else { 

				string szBufCurrent;

				int datasize = vSplittedData[ielem].size();
				try {
					if ( datasize > 0 ) {

						char *bytedata = new char[datasize+1];
						memcpy( bytedata, &vSplittedData[ielem][0], datasize );
						bytedata[datasize] = NULL; // 番兵
						szBufCurrent = string(bytedata); 
						delete[] bytedata;
					}
				} catch (exception e) {
				}

				Matches matches;

				// 半角カタカナを全角カタカナにする。
				szBufCurrent = HankakuKatakanaToZentakuKatakana(szBufCurrent);

				// 最後の終了記号以降は消す。
				if ( OnigMatch(szBufCurrent, "^(.+?)\x05\x05\x05", &matches) ) {

					if ( !matches[1].empty() ) {
						// 残りは、半角カタカナを全角ひらがなに
						char szBufZenHiragana[256] = "";
						hankana2zenhira((unsigned char *)matches[1].c_str(), (unsigned char *)szBufZenHiragana);
						szBufZenHiragana[255] = NULL; // 下手な修正しても暴走しないよう念のため
						nb6bushouretsuden[iRetsudenNo].retsuden = string(szBufZenHiragana);

					}
				}

			}

		}
	}


	// 合計列伝数を返す。
	return iRetsudenNo+1;
}





// 列伝の人数のところのニーモニックをbfile.n6pを元に書き換え。これはJumperではない。
void WriteAsmOnTenshouExeRetsuDenExistNumOfBFileN6P() {
	// TENSHOU.EXEの列伝人数制限を変更する。妥当な範囲にあるかどうかチェックしておく。
	if ( 1300 <= iExistRetudenNumOfBFileN6P && iExistRetudenNumOfBFileN6P <= 0xFFFF ) { // 1932だとヤヴァイ。
		WORD bExistNum = (WORD)iExistRetudenNumOfBFileN6P;

		WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(GAMEDATASTRUCT_BUSHOU_RETSUDEN_NUM_PATCH_ADDRESS), &bExistNum, 2, NULL);
	}
}