#include "WinTarget.h"
#include <fstream>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"

using namespace std;

extern void copyAllBushouSoldiersOriginToCopy();


// (戦場マップを表示しない際の)戦争開始時に、部将全員分の戦闘熟練値を保存。
int BushouExpBatValueOfStartBattle[GAMEDATASTRUCT_BUSHOU_NUM] = {};

// (戦場マップを表示しない際の)戦争開始時に、部将全員分の智謀熟練値を保存。
int BushouExpIntValueOfStartBattle[GAMEDATASTRUCT_BUSHOU_NUM] = {};


// (戦場マップを表示しない際の)戦争終了時に、武将の戦闘熟練値や智謀熟練値の飛躍の抑える。
// まずは、戦闘開始前に保存
void EXServiceExecuteBushouValueControllerOfStartBattle() {
	// インフレ防止フラグが有効の時
	if ( TSModCommand::AllBushou::cmdSuppressRiseOfExpValueInBattle > 0 ) {

		for ( int iBushouID = 0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {
			// 戦闘前に戦熟保存
			BushouExpBatValueOfStartBattle[iBushouID] = nb6bushouref[iBushouID].expbat;
			// 戦闘前に智熟保存
			BushouExpIntValueOfStartBattle[iBushouID] = nb6bushouref[iBushouID].expint;
		}
	}
}

// まずは、戦闘後に比較
void EXServiceExecuteBushouValueControllerOfEndBattle() {

	// インフレ防止フラグが有効の時
	if ( TSModCommand::AllBushou::cmdSuppressRiseOfExpValueInBattle > 0 ) {

		// 武将の情報を最新にする。
		copyAllBushouSoldiersOriginToCopy();

		int coef = 6 - TSModCommand::AllBushou::cmdSuppressRiseOfExpValueInBattle;

		for ( int iBushouID = 0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {
			// 戦闘後に戦熟比較
			int ibatsub = nb6bushouref[iBushouID].expbat - BushouExpBatValueOfStartBattle[iBushouID];

			if ( ibatsub > (coef * 10) ) {

				// それを武将にアップデート
				nb6bushouref[iBushouID].expbat = BushouExpBatValueOfStartBattle[iBushouID] + (coef * 10);
			}

			// 戦闘後に智熟比較
			int intsub = nb6bushouref[iBushouID].expint - BushouExpIntValueOfStartBattle[iBushouID];
			if ( intsub > (coef * 10) ) {

				// それを武将にアップデート
				nb6bushouref[iBushouID].expint = BushouExpIntValueOfStartBattle[iBushouID] + (coef * 10);
			}
		}
	}
}