#include "WinTarget.h"
#include <fstream>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"



/*
 * TSMod.txt�Ŏw�肷��ƁA���ׂĂ̕����̎��������̒l�ɏo����B�o�ꕐ�������l�B
 */

void EXSuperService_IfLifesIsTheValue() {

	// �܂����C���Q�[���܂���ĂȂ���΁A���s���Ȃ�
	if ( !CommonGlobalHandle::isMainGameStart ) {
		return;
	}

	// �����l��ω�������R�}���h��L���Ƃ��Ă��Ȃ���Ή������Ȃ��B
	if ( TSModCommand::AllBushou::cmdLifeIsTheValue == -1 ) {
		return;
	}


	for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

		// �������w�肳�ꂽ�l�łȂ���΁c
		if ( nb6bushouref[iBushouID].life != TSModCommand::AllBushou::cmdLifeIsTheValue ) {

			nb6bushouref[iBushouID].life = TSModCommand::AllBushou::cmdLifeIsTheValue;
		}
	}
}
