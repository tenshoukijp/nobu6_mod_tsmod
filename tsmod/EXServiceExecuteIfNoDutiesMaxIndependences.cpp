#include "WinTarget.h"
#include <fstream>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"



/*
 * TSMod.txt�Ŏw�肷��ƁA���ׂĂ̕����̋`�����ŏ��ɁA�Ɨ����ő�ɂ���B�o�ꕐ�������l�B

 */

void EXSuperService_IfNoDutiesAndMaxIndependences() {

	// �܂����C���Q�[���܂���ĂȂ���΁A���s���Ȃ�
	if ( !CommonGlobalHandle::isMainGameStart ) {
		return;
	}

	// �`���Ȃ��œƗ��ב�̃R�}���h��L���Ƃ��Ă��Ȃ���Ή������Ȃ��B
	if ( !TSModCommand::AllBushou::cmdNoDutiesAndMaxIndependences ) {
		return;
	}


	for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

		//--------------�܂��͓Ɨ�
		setBushouIndependence( iBushouID, 7 );

		// �`���Ȃǃi�V!!
		nb6bushouref[iBushouID].duty = 0;
	}
}
