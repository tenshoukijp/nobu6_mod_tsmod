#include "WinTarget.h"
#include <fstream>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"



/*
 * TSMod.txt�Ŏw�肷��ƁA���ׂĂ̕����̋|�̓K���l�͂d�ƂȂ�B
 */

void EXSuperService_IfYumiNoExists() {

	if ( TSModCommand::World::cmdTheYumiNoExists ) {
		// �O�̂��߁A�G�߂������x�ɁA�����S���̋|���O�Ƃ���B
		for ( int iBushouID = 0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {
			nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = 0;
		}
	}
}
