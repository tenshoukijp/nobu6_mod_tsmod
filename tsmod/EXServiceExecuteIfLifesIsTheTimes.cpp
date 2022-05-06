#include "WinTarget.h"
#include <fstream>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"



/*
 * TSMod.txt�Ŏw�肷��ƁA���ׂĂ̕����̎��������̒l�ɏo����B�o�ꕐ�������l�B
 */
void EXSuperService_IfLifesIsTheTimes() {

	// �܂����C���Q�[���܂���ĂȂ���΁A���s���Ȃ�
	if ( !CommonGlobalHandle::isMainGameStart ) {
		return;
	}


	// �΂����̂��x���Ȃ��{����������R�}���h���Q�{�ȏ�ŗL���B�P�ȉ��ł͖����B
	if ( TSModCommand::AllBushou::cmdLifeIsTheTimes <= 1 ) {
		return;
	}

	// �t�ȊO�͖���(�����̂��͂₷���āA�܂��G�ߏ�񂪂P�O�̓~�ɂȂ��Ă���
	if ( nb6year.season != 3 ) {
		return;
	}

	for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

		// ���{�Ƃ��Ă�������year�����������A0�łȂ���΁A�΂����Ȃ��B�������{�Ƃ����̂������قǁA�΂����Ȃ�
		// �΂����Ȃ����߂ɂ́A��a���N����P���₳�Ȃ���΂Ȃ�Ȃ��B
		if ( (nb6year.year % TSModCommand::AllBushou::cmdLifeIsTheTimes) != 0 ) {

			// �a���N���P�B255���Ƒ��₳�Ȃ�(0�ɂȂ��Ă܂�)
			if ( nb6bushouref[iBushouID].birth <= 254 ) {

				int old = getYearsOld(iBushouID);

				// �P�Ȃ��12�˂̎��͏������Ȃ��y�P���o�ꂵ�ĕP�ɂȂ邩�����ɂȂ邩�̔���C�x���g������A���ꂪ���N�J��Ԃ��Ȃ��悤�ɂ���z
				if (nb6bushouname[iBushouID].State == 5 && old == 12) {
					;

				// ����ӊO�͐��܂ꂽ�N�����炵�Ă������ƂŁA�΂����ʂƂ��Ď��Ȃ�����B
				} else {
					int birth = nb6bushouref[iBushouID].birth;
					birth++;
					nb6bushouref[iBushouID].birth = birth;
				}
			}
		}
	}
}
