// hexmap.n6p の 情報を NB6HexMapDataに格納する。
#include "GameDataStruct.h"
#include "ls11_mod.h"

#define HEXMAP_CASTLE_GRAPH_STARTID 952 // hexmap.nb6 の中は、野戦マップと攻城戦マップに分かれているが、城マップの始まる番号の開始点を示す。
									  // 最初の0～(34*14)-1までが野戦マップのグラ構成、3*14～3*14*2-1までが役割データ
									  // よって34*14*2=952が城マップデータの開始点である。
									  // 例外的に1594版に大阪 兼 安土のグラ構成マップが入っている

#define HEXMAP_CASTLE_ROLE_STARTID HEXMAP_CASTLE_GRAPH_STARTID + GAMEDATASTRUCT_CASTLE_NUM
									  // 城の役割の始まるポイント。城グラフィック構成番号の最初に、城数を足したもの

#define HEXMAP_CASTLE_HIGH_STARTID HEXMAP_CASTLE_GRAPH_STARTID + GAMEDATASTRUCT_CASTLE_NUM*2
									  // 城の高さの始まるポイント。城グラフィック構成番号の最初に、城数*2を足したもの


int CastleHighSum[GAMEDATASTRUCT_CASTLE_NUM] = {}; // 各城マップの全チップの高さの合計。
int SuperCastleHighSum = 0; // 各城マップの全チップの高さの合計。安土 兼 大阪城 専用。

// この項目はデフォルトのhexmap.nb6からだとメモリ展開が大きくなりすぎてtenshou.exeが動かなくなる
int setHexMapNB6ToHexMapArray() {

	// HexMap.NB6の全データを格納
	vector<vector<byte>> vHexMapNB6Data;

	// 圧縮されたLS11フォーマットをデコードする関数
	char *szHexMapName = GetHexMapName() + 2; // 先頭にある「A:HEXMAP.NB6」の「A:」の分をカット

	int result = ls11_DecodePack(szHexMapName, NULL, &vHexMapNB6Data );

	if ( vHexMapNB6Data.size() >= 1597 ) { // 大坂城の高さがhexmap.1596なので、hexmap.000～hexmap.1596 の計1597個が必要。
		for ( int i=0; i<GAMEDATASTRUCT_CASTLE_NUM;i++ ) {

			vector<byte> vCastleHigh = vHexMapNB6Data[HEXMAP_CASTLE_HIGH_STARTID+i]; // 高さのデータは[952+214*2]=1380 からスタート

			int iHigh=0; // 該当の城の高さの平均
			// 該当マップの高さを全て集計する
			for ( vector<byte>::iterator it=vCastleHigh.begin(); it != vCastleHigh.end(); it++ ) {
				iHigh += *it;
			}
			CastleHighSum[i] = iHigh; // 該当城の情報として格納
		}

		vector<byte> vOOsakaCastleHigh = vHexMapNB6Data[1596]; // 安土 兼 大阪の高さ情報。特別にここに存在する。

		int iHigh=0; // 該当の城の高さの平均
		// 該当マップの高さを全て集計する
		for ( vector<byte>::iterator it=vOOsakaCastleHigh.begin(); it != vOOsakaCastleHigh.end(); it++ ) {
			iHigh += *it;
		}
		SuperCastleHighSum = iHigh; // 該当城の情報として格納
	}

	return result;
}


// 城マップが持つ堅牢度を出す。
int getCastleMapSolidDegre(int iCastleID) {
	if ( 0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM ) {
		// 石山本願寺の場所で、大坂城になっている
		if (iCastleID==167 && strcmp(nb6castle[iCastleID].name, "大坂")==0 ) {
			return SuperCastleHighSum;
		}
		// 観音寺の場所で安土になっている
		else if (iCastleID==136 && strcmp(nb6castle[iCastleID].name, "安土")==0 ) {
			return SuperCastleHighSum;
		}
		else {
			// 堅牢度を返す。
			return CastleHighSum[iCastleID];
		}
	}

	return 0;
}




// A:HEXMAP.NB6みたいな名前が返ってくる。A:HEXMAP.N6Pが実際にはどこに書かれているかは定かではないが、デフォルトでは、GAMEDATASTRUCT_HEXMAPN6P_REF_ADDRESS のアドレスに存在する。
// MapDataObgk.dllなどでは、この場所をDLL内を指し示すように変えている。
// GetHexMapName関数なら、どこにあろうとも、該当の名前を取得可能。
char *GetHexMapName() {
	char *szHexMapName = NULL;

	// まず、GAMEDATASTRUCT_MAPDATAN6P_REF_ADDRESS1 のアドレスのINT値が、szHexMapNameポインタのアドレスとなるべき。即ち、そのアドレスに、mapdata.n6pといった文字へのポインタが入っている。
	int *iAddress = NULL;
	iAddress = (int *)GAMEDATASTRUCT_HEXMAPNB6_REF_ADDRESS;
	szHexMapName = (char *)(*iAddress);

	return szHexMapName;
}
