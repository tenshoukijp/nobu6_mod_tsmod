#include "WinTarget.h"
#include <fstream>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"



/*
 * TSMod.txtで指定すると、デフォ武将の熟練度をＭＡＸとする。
 */

// シナリオ選択画面時にＭＡＸとする。
void EXSuperService_IfFixExperienceMaxOnChangeTiming() {
	
	if ( ! TSModCommand::AllBushou::cmdFixExperienceMax ) {
		return;
	}

	// 政治・戦闘・智謀 の熟練度をＭＡＸの2000とする。
	for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

		nb6bushouref[iBushouID].expgov = 2000; 
		nb6bushouref[iBushouID].expbat = 2000; 
		nb6bushouref[iBushouID].expint = 2000; 
	}
}




// ゲームスタート時にもＭＡＸとする。セーブからのロードなどに対応するため。
void EXSuperService_IfFixExperienceMaxOnAlwaysMainGame(int iBushouID) {

	// 技能が未開発をＯＮにするコマンドを有効としていなければ何もしない。
	if ( ! TSModCommand::AllBushou::cmdFixExperienceMax ) {
		return;
	}

	// まだメインゲームまわってなければ、実行しない
	if ( !CommonGlobalHandle::isMainGameStart ) {
		return;
	}

	nb6bushouref[iBushouID].expgov = 2000; 
	nb6bushouref[iBushouID].expbat = 2000; 
	nb6bushouref[iBushouID].expint = 2000; 
}