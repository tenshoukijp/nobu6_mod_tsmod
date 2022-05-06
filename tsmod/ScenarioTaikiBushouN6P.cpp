#include "WinTarget.h"

#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "ScenarioMessageN6PPatch.h"
#include "ScenarioDataStruct.h"
#include "ls11_mod.h"
#include "ScenarioMod.h"


// sndata.n6p の中身
vector<vector<byte>> vSnTaikiBushouN6P;


/*
1534 0  0
1560 2  1
1571 3  2
1582 4  3
1546 1  4
1599 5  5
*/
// sndata.n6pのデータを、vSnDataN6Pへと格納する。
int LoadSnTaikiBushouN6PToVector() {

	// taiki.n6pをvSnDataN6Pに格納
	int error = ls11_DecodePack("taiki.n6p", NULL, &vSnTaikiBushouN6P);

	// エラーならここで。
	if (error == -1) { return error; }

	// sndata.n6pと、実際の見た目上の並びがことなるため。要するにシナリオのＩＤは見た目どおりの昇順ではないということ。
	swap(vSnTaikiBushouN6P[3], vSnTaikiBushouN6P[4]);
	swap(vSnTaikiBushouN6P[2], vSnTaikiBushouN6P[3]);
	swap(vSnTaikiBushouN6P[1], vSnTaikiBushouN6P[2]);

	if (p_snOnTaikiBushouDataTransfer) {
		p_snOnTaikiBushouDataTransfer(
			&(vSnTaikiBushouN6P[0][0]), vSnTaikiBushouN6P[0].size(),
			&(vSnTaikiBushouN6P[1][0]), vSnTaikiBushouN6P[1].size(),
			&(vSnTaikiBushouN6P[2][0]), vSnTaikiBushouN6P[2].size(),
			&(vSnTaikiBushouN6P[3][0]), vSnTaikiBushouN6P[3].size(),
			&(vSnTaikiBushouN6P[4][0]), vSnTaikiBushouN6P[4].size(),
			&(vSnTaikiBushouN6P[5][0]), vSnTaikiBushouN6P[5].size()
			);
	}

	return 1;
}


// 現在進行形のシナリオの待機武将の登場情報を得る
NB6TAIKIBUSHOUAPPEAR* GetTaikiBushouAppearArrayPointer() {
	int iScenarioLocation = getCurScenarioLocation();
	if (iScenarioLocation < 0) {
		return NULL;
	}

	if (0 <= iScenarioLocation && iScenarioLocation < (int)vSnTaikiBushouN6P.size()) {
		auto varr = vSnTaikiBushouN6P[iScenarioLocation];
		byte *pvarr = &(varr[0]);

		return (NB6TAIKIBUSHOUAPPEAR *)pvarr;
	}
	else {
		return NULL;
	}

}



// 現在進行形のシナリオの待機武将の登場情報を得る
NB6TAIKIBUSHOU* GetTaikiBushouArrayPointer() {
	int iScenarioLocation = getCurScenarioLocation();
	if (iScenarioLocation < 0) {
		return NULL;
	}

	if (0 <= iScenarioLocation && iScenarioLocation < (int)vSnTaikiBushouN6P.size()) {
		auto varr = vSnTaikiBushouN6P[iScenarioLocation];
		byte *pvarr = &(varr[0]);
		int add = sizeof(NB6TAIKIBUSHOUAPPEAR) * getTaikiBushouDataNum(); // 武将情報部分をまるまるパスしたい。
		// そこには確かにデータがある。
		if (add < (int)varr.size()) {
			pvarr = pvarr + add;
			return (NB6TAIKIBUSHOU *)pvarr;
		}
		else {
			return NULL;
		}
	}
	else {
		return NULL;
	}
}



// 現在進行しているシナリオの待機武将の戸籍情報を得る
NB6TAIKIBUSHOUNAME* GetTaikiBushouNameArrayPointer() {
	int iScenarioLocation = getCurScenarioLocation();
	if (iScenarioLocation < 0) {
		return NULL;
	}

	if (0 <= iScenarioLocation && iScenarioLocation < (int)vSnTaikiBushouN6P.size()) {
		auto varr = vSnTaikiBushouN6P[iScenarioLocation];
		byte *pvarr = &(varr[0]);
		int add = (sizeof(NB6TAIKIBUSHOUAPPEAR) + sizeof(NB6TAIKIBUSHOU)) * getTaikiBushouDataNum(); // 武将情報部分をまるまるパスしたい。
		// そこには確かにデータがある。
		if (add < (int)varr.size()) {
			pvarr = pvarr + add;
			return (NB6TAIKIBUSHOUNAME *)pvarr;
		}
		else {
			return NULL;
		}
	}
	else {
		return NULL;
	}
}


