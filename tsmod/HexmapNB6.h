#pragma once

#pragma pack(1)

int setHexMapNB6ToHexMapArray();
char *GetHexMapName();


// 004C8DE2  48 45 58 4D 41 50 2E 4E 42 36 00 00 00 00 41 3A  HEXMAP.NB6....A:
#define GAMEDATASTRUCT_HEXMAPNB6_NAME_ADDRESS	0x4C8DE2	// hexmap.nb6という名前のアドレス。｢004D2CB8  A:MAPDATA.N6P...｣というNULLが３つあるので、２文字までなら長く出来る。それ以上ながくしたい場合には参照元のアドレス(tenshou.exe内に２か所ある)を動的に変更する必要がある。

#define GAMEDATASTRUCT_HEXMAPNB6_REF_ADDRESS	0x498999	// hexmap.nb6という名前のアドレスを参照している場所１つ目
