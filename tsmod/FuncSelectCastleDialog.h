#pragma once

#pragma pack(1)

#include <string>
#include <vector>

using namespace std;

namespace Filter {
	namespace Castle {
		enum { 
			城名=1,
			国=1<<1,
			大名=1<<2,
			軍団=1<<3,
			城主=1<<4,
			武将=1<<5,
			行動可=1<<6,
			連結=1<<7,
			石高=1<<8,
			最石=1<<9,
			商業=1<<10,
			最商=1<<11,
			城郭=1<<12,
			最城=1<<13,
			民忠=1<<14,
			人口=1<<15,
			徴兵可=1<<16,
			兵士=1<<17,
			騎兵=1<<18,
			砲兵=1<<19,
			金収入=1<<20,
			米収入=1<<21,
			内貿=1<<22,
			外貿=1<<23,
			馬産=1<<24,
			砲産=1<<25,
			金山=1<<26,

			デフォルト=0x33D57B
		};
	}
}

// 対象となる城リスト。最後のオプションは１つも選択できないオプション。最大数は１固定
int FuncSelectCastleDialog(vector<int> list, int iFilterOption=Filter::Castle::デフォルト );