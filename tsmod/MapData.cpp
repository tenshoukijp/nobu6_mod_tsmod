#define _CRT_SECURE_NO_WARNINGS

#include "MapData.h"


// A:MAPDATA.N6Pみたいな名前が返ってくる。A:MAPDATA.N6Pが実際にはどこに書かれているかは定かではないが、デフォルトでは、GAMEDATASTRUCT_MAPDATAN6P_NAME_ADDRESS のアドレスに存在する。
// MapDataObgk.dllなどでは、この場所をDLL内を指し示すように変えている。
// GetMapDataName関数なら、どこにあろうとも、該当の名前を取得可能。
char *GetMapDataName() {
	char *szMapDataName = NULL;

	// まず、GAMEDATASTRUCT_MAPDATAN6P_REF_ADDRESS1 のアドレスのINT値が、szMapDataNameポインタのアドレスとなるべき。即ち、そのアドレスに、mapdata.n6pといった文字へのポインタが入っている。
	int *iAddress = NULL;
	iAddress = (int *)GAMEDATASTRUCT_MAPDATAN6P_REF_ADDRESS1;
	szMapDataName = (char *)(*iAddress);

	return szMapDataName;
}

// A:MAPDATA.N6Pみたいに｢A:｣を先頭に付けること
void SetMapDataName(char *szNewName) {
	// ポインタを得る
	char *pMapDataName = GetMapDataName();
	// 該当のポインタに元々存在している文字列数をMAX数として入れ替える。
	strcpy( pMapDataName, szNewName); // デフォルトのTENSHOU.EXE内なら13文字まで厳守、MapDataObgkModに対してなら、31文字までいける。
}

