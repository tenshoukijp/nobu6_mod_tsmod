#pragma once

#pragma pack(1)

#include <string>
#include <vector>

using namespace std;

namespace Filter {
	namespace Daimyo {
		enum { 
			大名=1,
			本城=1<<1,
			友好=1<<2,
			同盟=1<<3,
			婚姻=1<<4,
			共敵=1<<5,
			敵対大名=1<<6,
			軍団=1<<7,
			所領=1<<8,
			武将=1<<9,
			兵士=1<<10,
			騎馬=1<<11,
			騎兵=1<<12,
			鉄砲=1<<13,
			砲兵=1<<14,
			金=1<<15,
			金収支=1<<16,
			金収入=1<<17,
			金支出=1<<18,
			米=1<<19,
			米収支=1<<20,
			米収入=1<<21,
			米支出=1<<22,
			布教=1<<23,
			商人=1<<24,

			デフォルト=0x311079F
		};
	}
}


// 対象となる大名リスト。
// iMainDaimyoIDは、どの大名視点の情報を出すのか？ ということである。省略した場合は、ターン大名のものとなる。
int FuncSelectDaimyoDialog(vector<int> list, int iFilterOption=Filter::Daimyo::デフォルト, int iMainDaimyoID=-1 );