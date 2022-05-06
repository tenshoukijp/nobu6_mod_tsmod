#include "WinTarget.h"

#include "EXBushou.h"
#include "EXBushou_All.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"

#include <fstream>

using namespace std;

// �����ɂ���Ē����x���㉺������B
void EXSuperService_AishoCalc() {

	// �u�㒷�Ƃ̑����̗ǂ��������A���G�߁A�����ɉe������v��L���ɂ��Ă��Ȃ���΁A�������Ȃ��B
	if ( TSModCommand::AllBushou::cmdAffinityWithSuperiorInfluenceLoyalty == 0 ) { return; }

	// �S�Ă̕������Ȃ߂āA
	for ( int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

		// �����(��ʕ���)��łࢌR�c����łȂ���΃p�X
		if ( nb6bushouname[iBushouID].State != 2 && nb6bushouname[iBushouID].State != 1 ) { continue; }

		// �����̃��[�_�[�Ƃ́B
		int iLeaderID = 0;

		// ��R�c����̏ꍇ
		if ( nb6bushouname[iBushouID].State == 1 ) { 


			// �����̑喼�ԍ������߂�
			int iDaimyoID = nb6bushouref[iBushouID].attach-1;

			// �喼�ԍ�����喼ID�����߂�
			iLeaderID = nb6daimyo[iDaimyoID].attach-1;

			// �L���ȕ���ID�łȂ���Ύ�
			if ( !(0 <= iLeaderID && iLeaderID < GAMEDATASTRUCT_BUSHOU_NUM) ) {
				continue;
			}
		}

		// �����(=��ʕ���)��̏ꍇ
		if ( nb6bushouname[iBushouID].State == 2 ) {  

			// �Y���̕����̌R�cID�𓾂�
			int iGundanID = nb6bushouref[iBushouID].gundan-1;

			// �L���ȌR�cID�łȂ���Ύ�
			if ( !(0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM) ) {
				continue;
			}

			// �R�c���̕���ID�����߂�
			iLeaderID = nb6gundan[iGundanID].leader-1;

			// �L���ȕ���ID�łȂ���Ύ�
			if ( !(0 <= iLeaderID && iLeaderID < GAMEDATASTRUCT_BUSHOU_NUM) ) {
				continue;
			}
		}

		// 2�l�̑����̍��������߂�
		int iDiffer = getBushouAishoDiff( iBushouID, iLeaderID );

		// 2�l�̑������傫���قǁA�}�C�i�X����
		iDiffer = -iDiffer;

		// �ō���+4, �ň���-4�ƂȂ�悤�ɒ�������B
		iDiffer += 4;

		// ���Z��0�Ȃ�Ή������Ȃ��Ă��ǂ�
		if ( iDiffer == 0 ) { continue; }

		// ����ȊO�Ȃ�v�Z����B
		int cLoyal = nb6bushouref[iBushouID].loyal + iDiffer;

		// 100�𒴂��Ă�����100�Ƃ���B
		if ( cLoyal > 100 ) { cLoyal = 100; }
		// 0�����Ȃ�0�Ƃ���B
		if ( cLoyal <  0 )  { cLoyal =   0; }

		// ����𕐏��ɃA�b�v�f�[�g
		nb6bushouref[iBushouID].loyal = (char)cLoyal;
	}
}