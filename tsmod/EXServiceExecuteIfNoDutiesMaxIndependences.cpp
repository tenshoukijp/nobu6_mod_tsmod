#include "WinTarget.h"
#include <fstream>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"



/*
 * TSMod.txtで指定すると、すべての武将の義理を最小に、独立を最大にする。登場武将も同様。

 */

void EXSuperService_IfNoDutiesAndMaxIndependences() {

	// まだメインゲームまわってなければ、実行しない
	if ( !CommonGlobalHandle::isMainGameStart ) {
		return;
	}

	// 義理なしで独立細大のコマンドを有効としていなければ何もしない。
	if ( !TSModCommand::AllBushou::cmdNoDutiesAndMaxIndependences ) {
		return;
	}


	for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

		//--------------まずは独立
		setBushouIndependence( iBushouID, 7 );

		// 義理などナシ!!
		nb6bushouref[iBushouID].duty = 0;
	}
}
