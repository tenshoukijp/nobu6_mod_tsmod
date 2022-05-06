#include "WinTarget.h"
#include <fstream>
#include <time.h>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "FuncMessageDialog.h"
#include "FuncPlayResourceWav.h"
#include "FuncMoveCamera.h"


using namespace std;

char szTeppouSpMakeSmithEventMsg[100] = "";
void EXServiceExecute_TeppouSpMakeSmith() {

	// ���[�h����͔������Ȃ�
	if ( nb6progressseason <= 1 ) { return; }

	// �S�C�`�����Ă��Ȃ���΁A�������Ȃ�
	if ( !isTeppouDenrai() ) { return; }

	// �������̏�ɕ��������l���邩�H
	int iBushouNumInCastle[GAMEDATASTRUCT_CASTLE_NUM] = {0L};
	// �S�Ă̕������Ȃ߂āA���̕������A�S�C�����ӂł���΁A�ǂ̏�ɂ��邩���J�E���g����B
	for ( int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

		// �Q�l�ł͂Ȃ��A�Ώۂ̕������S�C�`�ȏ�ł���΁c
		if (nb6bushouname[iBushouID].State <=2 && getBushouAptitude(iBushouID, APTITUDE_TEPPOU) >= 4 && nb6bushouref[iBushouID].castle <= GAMEDATASTRUCT_CASTLE_NUM) {
			// �Ώۂ̏�ɂ���C���l���J�E���g���P���₷
			iBushouNumInCastle[(nb6bushouref[iBushouID].castle-1)]++;
		}
	}

	// ����Ȃ߂Ă����āA�Ώۂ̏�ɒb�葺�������ꍇ�A�Ǝ��������
	for ( int iCastleID=0; iCastleID<GAMEDATASTRUCT_CASTLE_NUM; iCastleID++ ) {

		// �Ǝ������Ȃ��A�����ď��ƒl��120�ȏ�ɒB�����Ă���B
		if ( !nb6castle[iCastleID].bSmith &&
			 nb6castle[iCastleID].town >= 120 ) {

		    // �S�C���ӂȐl�����̏��2�l�ȏア���ꍇ  �m��2/20, 5�l�Ȃ�m��5/20�Ƃ����悤�ɥ��
			if ( (iBushouNumInCastle[iCastleID] >= 2) && (rand() % 20 < iBushouNumInCastle[iCastleID]) ) {

				nb6castle[iCastleID].bSmith = 1; // �b�葺��1�Ƃ���B

				// �Ώۂ̏�̂Ȃ��ŁA�ł��M���������l��������
				int iBushouIDOfHightTeppou = -1;
				// �S�Ă̕������Ȃ߂āA
				for ( int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

					// �Q�l�ł͂Ȃ��A�Ώۂ̕������S�C�����ӁA���݃^�[�Q�b�g�ƂȂ��Ă����ɂ���B
					if (nb6bushouname[iBushouID].State <= 2 && getBushouAptitude(iBushouID, APTITUDE_TEPPOU) >= 4  && nb6bushouref[iBushouID].castle-1==iCastleID) {

						// ������Ȃ炻�̂܂ܓ����
						if ( iBushouIDOfHightTeppou == -1 ) {
							iBushouIDOfHightTeppou = iBushouID;
						
						// ���M���������l���o�Ă���΂��̐l�ɕύX����(�g���グ�ĂȂ���������Ȃ�����ǁA�܂���������)
						} else if (nb6bushouref[iBushouIDOfHightTeppou].merits < nb6bushouref[iBushouID].merits ) {
							iBushouIDOfHightTeppou = iBushouID;
						}
					}
				}

				// �J���������̏�Ɉړ�
				FuncMoveCamera( iCastleID );

				// ����炷
				FuncPlaySoundWav(602);

				// �Q�[�����̔ėp���_�C�A���O�ŏ����o��				
				wsprintf(szTeppouSpMakeSmithEventMsg, "%s%s��̓����ɂ��A\x0A%s%s�̋ߍx�ɁA�b�葺���o���܂����B", nb6bushouname[iBushouIDOfHightTeppou].familyname, nb6bushouname[iBushouIDOfHightTeppou].fastname,  nb6castle[iCastleID].name, getCastleTypeName(nb6castle[iCastleID].title) );

				FuncConfirmDialog( szTeppouSpMakeSmithEventMsg );
			}
		}
	}
}