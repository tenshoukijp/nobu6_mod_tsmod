#pragma once

#include "EXBushou_LancelotTartare.h"
#include "EXBushou_ShuiHidechiyo.h"


void EXBushou_All_Execute();

// 特殊武将用のパラメタ反映
void SetExBushouAvgParam(int iBushouID);

// 対象の武将がロスローリアンかどうか
bool IsLoslorien(int iBushouID);
