#include "WinTarget.h"
#include <fstream>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"

// �Q�l�̌M�������G�߃_�E������
void EXSuperService_RoninKunkouDecrement() {
	
	if ( ! TSModCommand::AllBushou::cmdNonMeritoriousReset ) {
		return;
	}

	for ( int iRoninID = 0; iRoninID < GAMEDATASTRUCT_BUSHOU_NUM; iRoninID++ ) {
		if ( nb6bushouname[iRoninID].State == 4 ) { // �Q�l��
			// �M����95���Ɍ��炷
			int curMerits = nb6bushouref[iRoninID].merits;
			curMerits = (int)(curMerits * 0.95);
			nb6bushouref[iRoninID].merits = curMerits;
		}
	}
}

