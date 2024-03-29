#pragma once

#pragma pack(1)

#include <string>
#include <vector>

using namespace std;

namespace Filter {
	namespace Gundan {
		enum { 
			軍団名=1,
			軍団長=1<<1,
			居城=1<<2,
			所領=1<<3,
			武将=1<<4,
			兵士=1<<5,
			騎馬=1<<6,
			騎兵=1<<7,
			鉄砲=1<<8,
			砲兵=1<<9,
			金=1<<10,
			金収支=1<<11,
			金収入=1<<12,
			金支出=1<<13,
			米=1<<14,
			米収支=1<<15,
			米収入=1<<16,
			米支出=1<<17,
			行動=1<<18,
			朝廷官位=1<<19,
			幕府役職=1<<20,
			指示内容=1<<21,
			方針=1<<22,
			攻略目標=1<<23,
			内政拠点=1<<24,
			軍事拠点=1<<25,

			デフォルト=0xCD7F
		};
	}
}

// 対象となる軍団リスト。最後のオプションは１つも選択できないオプション。最大数は１固定
int FuncSelectGundanDialog(vector<int> list, int iFilterOption=Filter::Gundan::デフォルト );