#include "WinTarget.h"
#include <fstream>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"



/*
 * TSMod.txtで指定すると、すべての武将の寿命を特定の値に出来る。登場武将も同様。
 */

void EXSuperService_IfLifesIsTheValue() {

	// まだメインゲームまわってなければ、実行しない
	if ( !CommonGlobalHandle::isMainGameStart ) {
		return;
	}

	// 寿命値を変化させるコマンドを有効としていなければ何もしない。
	if ( TSModCommand::AllBushou::cmdLifeIsTheValue == -1 ) {
		return;
	}


	for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

		// 寿命が指定された値でなければ…
		if ( nb6bushouref[iBushouID].life != TSModCommand::AllBushou::cmdLifeIsTheValue ) {

			nb6bushouref[iBushouID].life = TSModCommand::AllBushou::cmdLifeIsTheValue;
		}
	}
}
