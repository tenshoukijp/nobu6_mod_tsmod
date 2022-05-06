#include "WinTarget.h"
#include <fstream>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"

using namespace std;

#pragma pack(1)


// 現在のゲームシナリオ上の開始年を得る。
int getStartYearADOfScenarios() {
	int iSnID = getCurScenarioLocation();
	// ちゃんと選択した後である。
	if ( 0 <= iSnID && iSnID <= 5 ) {
		// すでにちゃんと代入されている。
		if ( iSnID < (int)vSnDataN6P.size() ) {
			int ret = 0;
			try {
				ret = vSnDataN6P[iSnID][0xb7];
			} catch (exception e) {
			}
			return ret + getStartYearAD();
		}
	}
	return 0;
}
