#include "CommonGlobalHandle.h"
#include "EXServiceExecute.h"
#include "GameDataStruct.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "TSModCommand.h"
#include "TSMod.h"





// ���V�[�Y���A�v���C���̃Q�[���i���Ɉ����񂹂��鏈��
void EXSuperService_PlayerDaimyoDataInfluenceOtherDaimyoData() {

	// �v���C���̐i�x�Ɉ����񂹂���
	if ( TSModCommand::OthersGundan::cmdPlayerAndTheWholeInfluenceEachOtherData > 0 ) {

		int lvl = TSModCommand::OthersGundan::cmdPlayerAndTheWholeInfluenceEachOtherData;

		int iPlayerGundanCnt = 0;
		int iPlayerGundanGold = 0;
		int iPlayerGundanRise = 0;
		int iPlayerGundanTeppou = 0;
		int iPlayerGundanKiba = 0;
		int iPlayerGundanSoldier = 0;

		int iOtherGundanCnt = 0;
		int iOtherGundanGold = 0;
		int iOtherGundanRise = 0;
		int iOtherGundanTeppou = 0;
		int iOtherGundanKiba = 0;
		int iOtherGundanSoldier = 0;

		int iGundanTeppou[GAMEDATASTRUCT_GUNDAN_NUM] = {};
		int iGundanKiba[GAMEDATASTRUCT_GUNDAN_NUM] = {};

		// �v���C�����͂̋��ƕĂ�����
		for (int iGundanNum=0; iGundanNum < GAMEDATASTRUCT_GUNDAN_NUM; iGundanNum++) {
			// ���閼�i���o�[��0xffff���Ƃ��̌R�c�͑��݂��Ă��Ȃ��B���ցB
			if (nb6gundan[iGundanNum].residence == 0xFFFF ) {
				continue;
			}
			// �Ώۂ��v���C���[�̌R�c�ł���΋��E�āE�S�C�E�R�n���J�E���g����B
			if ( isPlayerGundan(iGundanNum) ) {
				iPlayerGundanCnt++;
				iPlayerGundanGold += nb6gundan[iGundanNum].money;
				iPlayerGundanRise += nb6gundan[iGundanNum].rise;
				iPlayerGundanTeppou += nb6gundan[iGundanNum].gun;
				iPlayerGundanKiba += nb6gundan[iGundanNum].horse;
			// �Ώۂ��v���C���[�ȊO�̌R�c�ł���΋��E�āE�S�C�E�R�n���J�E���g����B
			} else {
				iOtherGundanCnt++;
				iOtherGundanGold += nb6gundan[iGundanNum].money;
				iOtherGundanRise += nb6gundan[iGundanNum].rise;
				iOtherGundanTeppou += nb6gundan[iGundanNum].gun;
				iOtherGundanKiba += nb6gundan[iGundanNum].horse;
			}
		}

		for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {
			int iGundanNum = nb6bushouref[iBushouID].gundan-1;

			// �Ώە������v���C���[�̌R�c�ł���Ε����ɂԂ牺�����Ă镺�m�̕�����A�����E���E�āE�S�C�E�R�n���J�E���g����B
			if ( isPlayerGundan(iGundanNum) ) {
				// �������v���X
				iPlayerGundanSoldier += nb6bushouref[iBushouID].soldier;
				// �����R�n���Ȃ�΁A�R�n���v���X
				if ( nb6bushouref[iBushouID].form == 1 ) {
					iPlayerGundanKiba += nb6bushouref[iBushouID].soldier;
				// �����S�C���Ȃ�΁A�S�C���v���X
				} else if ( nb6bushouref[iBushouID].form == 2 ) {
					iPlayerGundanTeppou += nb6bushouref[iBushouID].soldier;
				// �����R�n�S�C���Ȃ�΁A�S�C�ƋR�n���v���X
				} else if ( nb6bushouref[iBushouID].form == 3 ) {
					iPlayerGundanKiba += nb6bushouref[iBushouID].soldier;
					iPlayerGundanTeppou += nb6bushouref[iBushouID].soldier;
				}
			} else {
				// �������v���X
				iOtherGundanSoldier += nb6bushouref[iBushouID].soldier;
				// �����R�n���Ȃ�΁A�R�n���v���X
				if ( nb6bushouref[iBushouID].form == 1 ) {
					iOtherGundanKiba += nb6bushouref[iBushouID].soldier;
				// �����S�C���Ȃ�΁A�S�C���v���X
				} else if ( nb6bushouref[iBushouID].form == 2 ) {
					iOtherGundanTeppou += nb6bushouref[iBushouID].soldier;
				// �����R�n�S�C���Ȃ�΁A�S�C�ƋR�n���v���X
				} else if ( nb6bushouref[iBushouID].form == 3 ) {
					iOtherGundanKiba += nb6bushouref[iBushouID].soldier;
					iOtherGundanTeppou += nb6bushouref[iBushouID].soldier;

					iGundanKiba[iGundanNum] += nb6bushouref[iBushouID].soldier;
					iGundanTeppou[iGundanNum] += nb6bushouref[iBushouID].soldier;
				}
			}
		}	

		// �v���C�����͂����݂��Ă���ꍇ�B
		if ( iPlayerGundanCnt > 0 ) {
			iPlayerGundanGold /= iPlayerGundanCnt;
			iPlayerGundanRise /= iPlayerGundanCnt;
			iPlayerGundanTeppou /= iPlayerGundanCnt;
			iPlayerGundanKiba /= iPlayerGundanCnt;
			iPlayerGundanSoldier /= iPlayerGundanCnt;
		}
		if ( iOtherGundanCnt > 0 ) {
			iOtherGundanGold /= iOtherGundanCnt;
			iOtherGundanRise /= iOtherGundanCnt;
			iOtherGundanTeppou /= iOtherGundanCnt;
			iOtherGundanKiba /= iOtherGundanCnt;
			iOtherGundanSoldier /= iOtherGundanCnt;
		}

		int iAvgGundanGold = iPlayerGundanGold + iOtherGundanGold;
		int iAvgGundanRise = iPlayerGundanRise + iOtherGundanRise;
		int iAvgGundanTeppou = iPlayerGundanTeppou + iOtherGundanTeppou;
		int iAvgGundanKiba = iPlayerGundanKiba + iOtherGundanKiba;
		int iAvgGundanSoldier = iPlayerGundanSoldier + iOtherGundanSoldier;

		// �e�R�c�ɑ΂��āA�����ƕĂ𑝂₷�B
		for (int iGundanNum=0; iGundanNum < GAMEDATASTRUCT_GUNDAN_NUM; iGundanNum++) {

			// ���閼�i���o�[��0xffff���Ƃ��̌R�c�͑��݂��Ă��Ȃ��B���ցB
			if (nb6gundan[iGundanNum].residence == 0xFFFF ) {
				continue;
			}
			// �Ώۂ��v���C���[�̌R�c�ł���� �p�X
			if ( isPlayerGundan(iGundanNum) ) {
				continue;
			}

			// �R�c�i���o�[(1�Ԃ͉e����A2�Ԉȍ~�͉e������)
			int iGNumber = nb6gundan[iGundanNum].number;

			// �����֘A
			// �I�[�o�[�t���[�h�~�̂���INT�^�փR�s�[
			int money = nb6gundan[iGundanNum].money;
			// ���ςƌ��̊Ԃ����
			if ( iGNumber == 1 ) {
				money = (5*money + lvl*iAvgGundanGold)/(5+lvl); 
			} else {
				money = (15*money + lvl*iAvgGundanGold)/(15+lvl); 
			}
			// �I�[�o�[�t���[��}����
			if ( money < 0 ) { money = 0; }
			if ( money > GAMEDATASTRUCT_MONEY_MAX) { money = GAMEDATASTRUCT_MONEY_MAX; }
			// �i�[
			nb6gundan[iGundanNum].money = money;


			// ���Ċ֘A
			// �I�[�o�[�t���[�h�~�̂���INT�^�փR�s�[
			int rise = nb6gundan[iGundanNum].rise;
			// ���ςƌ��̊Ԃ����
			if ( iGNumber == 1 ) {
				rise = (5*rise + lvl*iAvgGundanRise)/(5+lvl); 
			} else {
				rise = (15*rise + lvl*iAvgGundanRise)/(15+lvl); 
			}
			// �I�[�o�[�t���[��}����
			if ( rise < 0 ) { rise = 0; }
			if ( rise > GAMEDATASTRUCT_RISE_MAX) { rise = GAMEDATASTRUCT_RISE_MAX; }
			// �i�[
			nb6gundan[iGundanNum].rise = rise;

			// �S�C�֘A
			// �I�[�o�[�t���[�h�~�̂���INT�^�փR�s�[
			int teppou = iGundanTeppou[iGundanNum];  // �Y���R�c�̕��m�����S�C�{�]���Ă�S�C�̍��v

			// �Y���R�c�̍��v�S�C�� - ���ϓS�C��
			int subTeppou = teppou - iAvgGundanTeppou; 
				
			// ������Y���̌R�c�ɑ�����������

			int gun = nb6gundan[iGundanNum].gun;
			// ���ςƌ��̊Ԃ����
			if ( iGNumber == 1 ) {
				gun = gun - subTeppou/(2*(6-lvl)); 
			} else {
				gun = gun - subTeppou/(4*(6-lvl));
			}
			// �I�[�o�[�t���[��}����
			if ( gun < 0 ) { gun = 0; }
			if ( gun > GAMEDATASTRUCT_GUN_MAX) { gun = GAMEDATASTRUCT_GUN_MAX; }
			// �i�[
			nb6gundan[iGundanNum].gun = gun;


			// �S�C�֘A
			// �I�[�o�[�t���[�h�~�̂���INT�^�փR�s�[
			int kiba = iGundanKiba[iGundanNum];  // �Y���R�c�̕��m�����S�C�{�]���Ă�S�C�̍��v

			// �Y���R�c�̍��v�S�C�� - ���ϓS�C��
			int subKiba = kiba - iAvgGundanKiba; 
				
			// ������Y���̌R�c�ɑ�����������

			int horse = nb6gundan[iGundanNum].horse;
			// ���ςƌ��̊Ԃ����
			if ( iGNumber == 1 ) {
				horse = horse - subKiba/(2*(6-lvl)); 
			} else {
				horse = horse - subKiba/(4*(6-lvl));
			}
			// �I�[�o�[�t���[��}����
			if ( horse < 0 ) { horse = 0; }
			if ( horse > GAMEDATASTRUCT_KIBA_MAX) { horse = GAMEDATASTRUCT_KIBA_MAX; }
			// �i�[
			nb6gundan[iGundanNum].horse = horse;


		}
	}
}

