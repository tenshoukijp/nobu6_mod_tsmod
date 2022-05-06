#include "WinTarget.h"
#include <fstream>
#include <time.h>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "FuncMessageDialog.h"
#include "FuncPlayResourceWav.h"
#include "FuncMoveCamera.h"



using namespace std;

char szPirateMakePortEventMsg[100] = "";
void EXServiceExecute_PirateMakePort() {

	// ���[�h����͔������Ȃ�
	if ( nb6progressseason <= 1 ) { return; }

	// �������̏�ɕ��������l���邩�H
	int iBushouNumInCastle[GAMEDATASTRUCT_CASTLE_NUM] = {0L};
	// �S�Ă̕������Ȃ߂āA���̕������A�C���ł���΁A�ǂ̏�ɂ��邩���J�E���g����B
	for ( int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {
		// �Ώۂ̕������C���ł���΁c ���A�ǂ����̏�ɂ����Ə������Ă���΁c
		if (nb6bushouname[iBushouID].State <=2 && nb6bushouref[iBushouID].job == 0x50 && nb6bushouref[iBushouID].castle <= GAMEDATASTRUCT_CASTLE_NUM) {
			// �Ώۂ̏�ɂ���C���l���J�E���g���P���₷
			iBushouNumInCastle[(nb6bushouref[iBushouID].castle-1)]++;
		}
	}

	// ����Ȃ߂Ă����āA�Ώۂ̏邪���ɍ`�������āA���ۍ`���Ȃ��ꍇ�A���ۍ`�����
	for ( int iCastleID=0; iCastleID<GAMEDATASTRUCT_CASTLE_NUM; iCastleID++ ) {

		// �`�͂���A�������A���ۍ`���Ȃ��A�����ď��ƒl��150�ȏ�ɒB�����Ă���B
		if ( nb6castle[iCastleID].bPort  &&
			!nb6castle[iCastleID].blPort &&
			 nb6castle[iCastleID].town >= 150 ) {

		    // �C�������̏��2�l�ȏア���ꍇ  �m��2/20, 5�l�Ȃ�m��5/20�Ƃ����悤�ɥ��
			if ( (iBushouNumInCastle[iCastleID] >= 2) && (rand() % 20 < iBushouNumInCastle[iCastleID]) ) {

				// ���ۍ`���P�Ƃ���B
				nb6castle[iCastleID].blPort = 1;

				// �Ώۏ�̍ő古�ƒl�𑝂₷�B�ő古�ƒl���������Ă���l�����邩������Ȃ��̂ŐT�d�ȑ���
				byte iTownMax = nb6castlemax[iCastleID].maxtown;
				// 200��菬�����Ȃ�Α��₷
				if (iTownMax < 200) {
					iTownMax += 30;
				}
				// 200�𒴂����Ȃ�΁A200�Ƃ���B
				if (iTownMax > nb6parammax_main.town && nb6parammax_main.town <= 255 ) {
					iTownMax = (byte)nb6parammax_main.town;
				}	
				// �ő古�ƒl�̃A�h���X
				nb6castlemax[iCastleID].maxtown = iTownMax;


				// �Ώۂ̏�̂Ȃ��ŁA�ł��M���������l��������
				int iBushouIDOfHightSuigun = -1;
				// �S�Ă̕������Ȃ߂āA
				for ( int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {
					// �Ώۂ̕������C���ł���A���݃^�[�Q�b�g�ƂȂ��Ă����ɂ���B
					if (nb6bushouname[iBushouID].State <= 2 && nb6bushouref[iBushouID].job == 0x50 && nb6bushouref[iBushouID].castle-1==iCastleID) {
						// ������Ȃ炻�̂܂ܓ����
						if ( iBushouIDOfHightSuigun == -1 ) {
							iBushouIDOfHightSuigun = iBushouID;
						
						// ���M���������l���o�Ă���΂��̐l�ɕύX����(�g���グ�ĂȂ���������Ȃ�����ǁA�܂���������)
						} else if (nb6bushouref[iBushouIDOfHightSuigun].merits < nb6bushouref[iBushouID].merits ) {
							iBushouIDOfHightSuigun = iBushouID;
						}
					}
				}

				// �J���������̏�Ɉړ�
				FuncMoveCamera( iCastleID );

				// ����炷
				FuncPlaySoundWav(602);

				// �Q�[�����̔ėp���_�C�A���O�ŏ����o��				
				wsprintf(szPirateMakePortEventMsg, "%s%s�炪�����鐅�R�O�̓����ɂ��A\x0A%s%s�ߍx�̍`���A���ۍ`�ɔ��W���܂����B", nb6bushouname[iBushouIDOfHightSuigun].familyname, nb6bushouname[iBushouIDOfHightSuigun].fastname,  nb6castle[iCastleID].name, getCastleTypeName(nb6castle[iCastleID].title) );
				FuncConfirmDialog( szPirateMakePortEventMsg );
			}
		}
	}
}