#pragma once

#pragma pack(1)

#include "FuncSelectBushouDialog.h"

// 対象となる武将リスト。選択可能最大人数を指定。一応８人まで。デフォルトはよくありがちな５人。
vector<int> FuncSelectBushouMultiDialog(vector<int> list, __int64 iFilterOption=Filter::Bushou::デフォルト, int iSelectableMaxBushouNum=5 );

