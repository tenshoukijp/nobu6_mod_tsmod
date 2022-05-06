//
#define _CRT_SECURE_NO_WARNINGS
#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "ls11_mod.h"
#include "CnvHankakuZenkaku.h"
#include "GetOSVersion.h"


extern std::string HankakuKatakanaToZentakuKatakana(std::string szBufOriginal);

// message.n6p の 情報のうち、家宝名や家宝列伝をNB6KahouExoptionに格納する。
int setBKahouN6PToNB6KAHOUEXOPTION() {

	// ちょうど vBufDecodedData[0]=１番目の分割ファイルのデータ列、vBufDecodedData[1]=２番目の分割ファイルのデータ列、みたいな感じ
	vector<vector<byte>> vBufDecodedData;

	// 圧縮されたLS11フォーマットをデコードする関数
	ls11_DecodePack("message.n6p", NULL, &vBufDecodedData );


	int iKahouNo = -1;

	int vBufDecodedDataSize = (int)vBufDecodedData.size();

	// デコードしたデータの(内部的)ファイルでまわしていく。0番は対象ではない。 // 15～17が家宝データ
	for ( int ifile = 15; ifile < vBufDecodedDataSize && ifile <= 17 ; ifile++ ) {

		// ちょうど vSplittedData[0]=１番目の要素のデータ列、vSplittedData[1]=２番目の要素のデータ列、みたいな感じ
		vector<vector<byte>> vSplittedData;

		// さらに要素分割。
		ls11_SplitData( vBufDecodedData[ifile], &vSplittedData );

		int ivSplittedDataSize = (int)vSplittedData.size();

		continue;

		// 各要素ごとに･･･ まず名前、読み仮名
		for ( int ielem=0; ielem < ivSplittedDataSize; ielem++ ) {

			if ( ielem%2==0 ) { iKahouNo++; }

			if ( iKahouNo < 0 ) { iKahouNo = 0; }; // 最悪のための番兵

			// 元来定義されている列伝数を超えたものは、無視
			if ( iKahouNo >= GAMEDATASTRUCT_KAHOU_NUM ) { break; }

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
				// 家宝列伝から要素を抽出する。
				if ( OnigMatch(szBufCurrent, "^\x1b\x6b(.*?)\x1b\x48(.*?)\n?\x05\x05\x05$", &matches) ) {

					/*
					string name;		// 代表名
					string yomigana;	// よみがな
					string retsuden;	// 列伝文字列
					*/
					nb6kahouretsuden[iKahouNo].name = matches[2];
					if ( !matches[1].empty() ) {

						// 半角カタカナを全角ひらがなに
						char szBufZenHiranaga[128] = "";
						hankana2zenhira((unsigned char *)matches[1].c_str(), (unsigned char *)szBufZenHiranaga);
						szBufZenHiranaga[100] = NULL; // 下手な修正しても暴走しないよう念のため

						nb6kahouretsuden[iKahouNo].yomigana = string(szBufZenHiranaga);
					}
				}

			// 奇数は家宝列伝。
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


				// この特殊な記号は天翔記において、半角カタカナを全角カタカナにするという意味
				szBufCurrent = HankakuKatakanaToZentakuKatakana(szBufCurrent);

				Matches matches;
				// 最後の終了記号以降は消す。
				if ( OnigMatch(szBufCurrent, "^(.*?)\x05\x05\x05$", &matches) ) {

					if ( !matches[1].empty() ) {
						// 残りは、半角カタカナを全角ひらがなに
						char szBufZenHiragana[256] = "";
						hankana2zenhira((unsigned char *)matches[1].c_str(), (unsigned char *)szBufZenHiragana);
						szBufZenHiragana[255] = NULL; // 下手な修正しても暴走しないよう念のため
						nb6kahouretsuden[iKahouNo].retsuden = string(szBufZenHiragana);
					}
				}

			}
		}
	}
	return true; //
}
