#include "WinTarget.h"
#include <fstream>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"



/*
 * TSMod.txt�Ŏw�肷��ƁA���ׂĂ̕����̗E�҂����̒l�ɏo����B�o�ꕐ�������l�B
 */

void EXSuperService_IfBravesIsTheValue() {

	// �܂����C���Q�[���܂���ĂȂ���΁A���s���Ȃ�
	if ( !CommonGlobalHandle::isMainGameStart ) {
		return;
	}

	// �E�Ғl��ω�������R�}���h��L���Ƃ��Ă��Ȃ���Ή������Ȃ��B
	if ( TSModCommand::AllBushou::cmdBraveIsTheValue == -1 ) {
		return;
	}


	for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

		// �����A�܂����C���ł���΁c
		if ( nb6bushouref[iBushouID].brave != TSModCommand::AllBushou::cmdBraveIsTheValue ) {

			nb6bushouref[iBushouID].brave = TSModCommand::AllBushou::cmdBraveIsTheValue;
		}
	}
}
