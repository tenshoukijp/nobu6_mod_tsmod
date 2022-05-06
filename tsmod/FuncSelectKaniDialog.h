#pragma once

#pragma pack(1)

#include <string>
#include <vector>

using namespace std;

namespace Filter {
	namespace Kani {
		enum { 
			官位名=1,
			位=1<<1,
			所有者=1<<2,
			効果=1<<3,

			デフォルト=0xF
		};
	}
}

// 対象となる家宝リスト。
// iFilterOptionはどれを表示するのか？ という情報
int FuncSelectKaniDialog(vector<int> list, int iFilterOption=Filter::Kani::デフォルト );