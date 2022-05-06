#include "CommonGlobalHandle.h"
#include "EXServiceExecute.h"
#include "GameDataStruct.h"
#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "TSModCommand.h"
#include "TSMod.h"

// �����ꂩ�̌R�c���S�C���������Ă���̂�
bool isAnyGudanHasTeppou() {
	int iSumOfGun = 0;
	for (int iGundanID = 0; iGundanID < GAMEDATASTRUCT_GUNDAN_NUM; iGundanID++) {
		iSumOfGun += nb6gundan[iGundanID].gun;
	}

	if (iSumOfGun > 0) {
		return true;
	}
	else {
		return false;
	}
}

// ���Y�i�Ɋ֘A�������ڂ������I�ɑ�����(�S�C�E�n�E���ۍ`)
void EXSuperService_CheckIncreaseSpecialProductByTheThing() {

	// �����͂̓��Y�i�Ɋ֘A�������ڂ������I�ɑ�����(�S�C�E�n�E���ۍ`)
	if ( TSModCommand::MyselfGundan::cmdIncreaseSpecialProductByTheThing > 0 ) {
		for ( int iCastleID=0; iCastleID < GAMEDATASTRUCT_CASTLE_NUM; iCastleID++) {

			// �ǂ��̌R�c�ɏ������Ă��邩�H
			if ( nb6castle[iCastleID].attach == 0xFFFF) {
				continue; // �������ĂȂ���Ύ���
			}

			int iGundanID = nb6castle[iCastleID].attach-1;

			// �l���s��
			if ( !(0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) ) {
				continue;
			}

			// �Ώۂ������͂̌R�c�ł���� �p�X
			if ( !isPlayerGundan(iGundanID) ) {
				continue;
			}
			
			// �S�C�����݂��Ȃ����̒��A�Ƀt���O�������Ă��Ȃ�
			if ( TSModCommand::World::cmdTheTeppouNoExists == 0 ) {

				if (isTeppouDenrai() || isAnyGudanHasTeppou() ) {
					// �Y���̏�ɒb��̑������t���Ă�����ATSModCommand::MyselfGundan::cmdIncreaseSpecialProductByTheThing * 3 �̒l����������
					nb6gundan[iGundanID].gun += nb6castle[iCastleID].bSmith * TSModCommand::MyselfGundan::cmdIncreaseSpecialProductByTheThing * 3;
					if (nb6gundan[iGundanID].gun > GAMEDATASTRUCT_GUN_MAX) {
						nb6gundan[iGundanID].gun = GAMEDATASTRUCT_GUN_MAX;
					}
					// �Y���̏�ɍ��ۍ`�̑������t���Ă�����ATSModCommand::MyselfGundan::cmdIncreaseSpecialProductByTheThing * 3 �̒l����������
					nb6gundan[iGundanID].gun += nb6castle[iCastleID].blPort * TSModCommand::MyselfGundan::cmdIncreaseSpecialProductByTheThing * 3;
					if (nb6gundan[iGundanID].gun > GAMEDATASTRUCT_GUN_MAX) {
						nb6gundan[iGundanID].gun = GAMEDATASTRUCT_GUN_MAX;
					}
				}
			}

			// �Y���̏�ɔn�Y�̑������t���Ă�����ATSModCommand::MyselfGundan::cmdIncreaseSpecialProductByTheThing �̒l����������
			nb6gundan[iGundanID].horse +=  nb6castle[iCastleID].bHorse * TSModCommand::MyselfGundan::cmdIncreaseSpecialProductByTheThing;
			if ( nb6gundan[iGundanID].horse > GAMEDATASTRUCT_KIBA_MAX ) {
				nb6gundan[iGundanID].horse = GAMEDATASTRUCT_KIBA_MAX;
			}

		}
	}


	// �����͂̓��Y�i�Ɋ֘A�������ڂ������I�ɑ�����(�S�C�E�n�E���ۍ`)
	if ( TSModCommand::OthersGundan::cmdIncreaseSpecialProductByTheThing > 0 ) {
		for ( int iCastleID=0; iCastleID < GAMEDATASTRUCT_CASTLE_NUM; iCastleID++) {

			// �ǂ��̌R�c�ɏ������Ă��邩�H
			if ( nb6castle[iCastleID].attach == 0xFFFF) {
				continue; // �������ĂȂ���Ύ���
			}

			int iGundanID = nb6castle[iCastleID].attach-1;

			// �l���s��
			if ( !(0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) ) {
				continue;
			}

			// �Ώۂ������͂̌R�c�ł���� �p�X
			if ( isPlayerGundan(iGundanID) ) {
				continue;
			}
			
			// �S�C�����݂��Ȃ����̒��A�Ƀt���O�������Ă��Ȃ�
			if (TSModCommand::World::cmdTheTeppouNoExists == 0) {

				if (isTeppouDenrai() || isAnyGudanHasTeppou() ) {

					// �Y���̏�ɒb��̑������t���Ă�����ATSModCommand::OthersGundan::cmdIncreaseSpecialProductByTheThing * 3 �̒l����������
					nb6gundan[iGundanID].gun += nb6castle[iCastleID].bSmith * TSModCommand::OthersGundan::cmdIncreaseSpecialProductByTheThing * 3;

					if (nb6gundan[iGundanID].gun > GAMEDATASTRUCT_GUN_MAX) {
						nb6gundan[iGundanID].gun = GAMEDATASTRUCT_GUN_MAX;
					}

					// �Y���̏�ɍ��ۍ`�̑������t���Ă�����ATSModCommand::OthersGundan::cmdIncreaseSpecialProductByTheThing * 3 �̒l����������
					nb6gundan[iGundanID].gun += nb6castle[iCastleID].blPort * TSModCommand::OthersGundan::cmdIncreaseSpecialProductByTheThing * 3;
					if (nb6gundan[iGundanID].gun > GAMEDATASTRUCT_GUN_MAX) {
						nb6gundan[iGundanID].gun = GAMEDATASTRUCT_GUN_MAX;
					}
				}
			}

			// �Y���̏�ɔn�Y�̑������t���Ă�����ATSModCommand::OthersGundan::cmdIncreaseSpecialProductByTheThing �̒l����������
			nb6gundan[iGundanID].horse +=  nb6castle[iCastleID].bHorse * TSModCommand::OthersGundan::cmdIncreaseSpecialProductByTheThing;
			if ( nb6gundan[iGundanID].horse > GAMEDATASTRUCT_KIBA_MAX ) {
				nb6gundan[iGundanID].horse = GAMEDATASTRUCT_KIBA_MAX;
			}

		}
	}

}
