#pragma once

#pragma pack(1)

#include <string>
#include <vector>

using namespace std;

namespace Filter {
	namespace Kahou {
		enum { 
			名称=1,
			種類=1<<1,
			等級=1<<2,
			所有者=1<<3,
			効果=1<<4,

			デフォルト=0x1F
		};
	}
}

// 対象となる家宝リスト。
// iFilterOptionはどれを表示するのか？ という情報
int FuncSelectKahouDialog(vector<int> list, int iFilterOption=Filter::Kahou::デフォルト );