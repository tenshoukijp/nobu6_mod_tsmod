#pragma once

#include "GameDataStruct.h"

// 004D2CB8  41 3A 4D 41 50 44 41 54 41 2E 4E 36 50 00 00 00  A:MAPDATA.N6P...
#define GAMEDATASTRUCT_MAPDATAN6P_NAME_ADDRESS	0x4D2CB8	// mapdata.n6pという名前のアドレス。｢004D2CB8  A:MAPDATA.N6P...｣というNULLが３つあるので、２文字までなら長く出来る。それ以上ながくしたい場合には参照元のアドレス(tenshou.exe内に２か所ある)を動的に変更する必要がある。

#define GAMEDATASTRUCT_MAPDATAN6P_REF_ADDRESS1	0x490B60	// mapdata.n6pという名前のアドレスを参照している場所１つ目
#define GAMEDATASTRUCT_MAPDATAN6P_REF_ADDRESS2	0x490C79	// mapdata.n6pという名前のアドレスを参照している場所２つ目


// mapdata.n6p相当の名前を得る(変えられていれば、それを得る)
char *GetMapDataName();


// mapdata.n6pに変えて、別のものを設定
void SetMapDataName(char *szNewName);

