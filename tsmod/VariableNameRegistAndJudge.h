#pragma once

#include "GameDataStruct.h"


#define NAME_VARIATION_NUM	11

// 該当の武将ＩＤの武将は、オリジナル武将（主に名前で判定）かどうか。
bool IsOriginalRegistedBushou(int iBushouID);

// 武将のバリエーション名
extern char *nb6bushou_name_variation[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM][NAME_VARIATION_NUM]; // 1300人が11個のバリエーション名、１つの名前あたり最大27文字まで。
