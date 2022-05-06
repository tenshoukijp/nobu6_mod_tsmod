#include "WinTarget.h"
#include <fstream>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"



/*
 * TSMod.txtで指定すると、すべての武将の勇猛を特定の値に出来る。登場武将も同様。
 */

void EXSuperService_IfBravesIsTheValue() {

	// まだメインゲームまわってなければ、実行しない
	if ( !CommonGlobalHandle::isMainGameStart ) {
		return;
	}

	// 勇猛値を変化させるコマンドを有効としていなければ何もしない。
	if ( TSModCommand::AllBushou::cmdBraveIsTheValue == -1 ) {
		return;
	}


	for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

		// もし、まだ未修正であれば…
		if ( nb6bushouref[iBushouID].brave != TSModCommand::AllBushou::cmdBraveIsTheValue ) {

			nb6bushouref[iBushouID].brave = TSModCommand::AllBushou::cmdBraveIsTheValue;
		}
	}
}
