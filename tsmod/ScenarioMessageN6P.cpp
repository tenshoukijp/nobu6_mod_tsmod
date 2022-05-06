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


extern CurProcessMemory CPM;
extern HANDLE g_hModule;


// sndata.n6p の中身
vector<vector<byte>> vSnDataN6P;


/*
1534 0  0
1560 2  1
1571 3  2
1582 4  3
1546 1  4
1599 5  5
*/
// sndata.n6pのデータを、vSnDataN6Pへと格納する。
int LoadSnDataN6PToVector() {

	// sndata.n6pをvSnDataN6Pに格納
	int error = ls11_DecodePack("sndata.n6p", NULL, &vSnDataN6P);

	// エラーならここで。
	if ( error == -1 ) { return error; }

	// sndata.n6pと、実際の見た目上の並びがことなるため。要するにシナリオのＩＤは見た目どおりの昇順ではないということ。
	swap(vSnDataN6P[3],vSnDataN6P[4]);
	swap(vSnDataN6P[2],vSnDataN6P[3]);
	swap(vSnDataN6P[1],vSnDataN6P[2]);

	return 1;
}