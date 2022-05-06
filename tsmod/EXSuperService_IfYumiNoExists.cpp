#include "WinTarget.h"
#include <fstream>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"



/*
 * TSMod.txtで指定すると、すべての武将の弓の適正値はＥとなる。
 */

void EXSuperService_IfYumiNoExists() {

	if ( TSModCommand::World::cmdTheYumiNoExists ) {
		// 念のため、季節がかわる度に、武将全員の弓を０とする。
		for ( int iBushouID = 0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {
			nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = 0;
		}
	}
}
