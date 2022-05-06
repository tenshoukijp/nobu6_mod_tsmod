#include "WinTarget.h"
#include <fstream>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"

using namespace std;

extern void copyAllBushouSoldiersOriginToCopy();


// (���}�b�v��\�����Ȃ��ۂ�)�푈�J�n���ɁA�����S�����̐퓬�n���l��ۑ��B
int BushouExpBatValueOfStartBattle[GAMEDATASTRUCT_BUSHOU_NUM] = {};

// (���}�b�v��\�����Ȃ��ۂ�)�푈�J�n���ɁA�����S�����̒q�d�n���l��ۑ��B
int BushouExpIntValueOfStartBattle[GAMEDATASTRUCT_BUSHOU_NUM] = {};


// (���}�b�v��\�����Ȃ��ۂ�)�푈�I�����ɁA�����̐퓬�n���l��q�d�n���l�̔��̗}����B
// �܂��́A�퓬�J�n�O�ɕۑ�
void EXServiceExecuteBushouValueControllerOfStartBattle() {
	// �C���t���h�~�t���O���L���̎�
	if ( TSModCommand::AllBushou::cmdSuppressRiseOfExpValueInBattle > 0 ) {

		for ( int iBushouID = 0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {
			// �퓬�O�ɐ�n�ۑ�
			BushouExpBatValueOfStartBattle[iBushouID] = nb6bushouref[iBushouID].expbat;
			// �퓬�O�ɒq�n�ۑ�
			BushouExpIntValueOfStartBattle[iBushouID] = nb6bushouref[iBushouID].expint;
		}
	}
}

// �܂��́A�퓬��ɔ�r
void EXServiceExecuteBushouValueControllerOfEndBattle() {

	// �C���t���h�~�t���O���L���̎�
	if ( TSModCommand::AllBushou::cmdSuppressRiseOfExpValueInBattle > 0 ) {

		// �����̏����ŐV�ɂ���B
		copyAllBushouSoldiersOriginToCopy();

		int coef = 6 - TSModCommand::AllBushou::cmdSuppressRiseOfExpValueInBattle;

		for ( int iBushouID = 0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {
			// �퓬��ɐ�n��r
			int ibatsub = nb6bushouref[iBushouID].expbat - BushouExpBatValueOfStartBattle[iBushouID];

			if ( ibatsub > (coef * 10) ) {

				// ����𕐏��ɃA�b�v�f�[�g
				nb6bushouref[iBushouID].expbat = BushouExpBatValueOfStartBattle[iBushouID] + (coef * 10);
			}

			// �퓬��ɒq�n��r
			int intsub = nb6bushouref[iBushouID].expint - BushouExpIntValueOfStartBattle[iBushouID];
			if ( intsub > (coef * 10) ) {

				// ����𕐏��ɃA�b�v�f�[�g
				nb6bushouref[iBushouID].expint = BushouExpIntValueOfStartBattle[iBushouID] + (coef * 10);
			}
		}
	}
}