#include "WinTarget.h"
#include <math.h>

#include "EXBushou.h"
#include "EXBushou_All.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"

extern int  getCustomSoldierMax( int iBushouID, int soldiermax );

// ���݂̃��C����ʂ̃^�[���ł̊e�X�̕������������Ă��ǂ����m�̐� 
int ServiceExecuteRegenerationAllIncrSoldierInMain[GAMEDATASTRUCT_BUSHOU_NUM] = {};

// ��L�̊e�X�̕����ɂ��āA���C����ʂ̢������������v�Z����B
void EXServiceExecuteRegeneration_All_SoldierCalcInMain() {

	// �����͂̎����񕜂�0���傫��
	if ( TSModCommand::OthersGundan::cmdRegenerationSoldierAutomatically > 0) {

		// �������͉񕜃T�[�r�X���󂯂镐��ID��S�����Z�b�g
		for (int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {

			if (nb6bushouref[iBushouID].gundan == 0xFFFF) {
				continue;
			}

			// �v���C���[�̔z���R�c�łȂ���΁c
			if (! isPlayerGundan(nb6bushouref[iBushouID].gundan-1)) {

				//�@���݂̐g���𓾂�
				int position = nb6bushouname[iBushouID].position;

				//�@�g������ő啺���𓾂�
				int soldiermax = nb6soldiermax[position].soldiermax;

				// �ő啺�m���́A�E�҂������芯�ʁE��E���ɂ���ăt�B���^�[��������
				soldiermax = getCustomSoldierMax( iBushouID, soldiermax );

				// �������Ă��ǂ�����(MAX�̕��m��-���݂̕��� / 9-�w�背�x��(0�`5) = �X�`�S)
				ServiceExecuteRegenerationAllIncrSoldierInMain[iBushouID] = 
					(int)ceil
					(
						( 
							(double)soldiermax - (double)nb6bushouref[iBushouID].soldier
						) / (9-TSModCommand::OthersGundan::cmdRegenerationSoldierAutomatically) 
					);

				// ���������P��菭�Ȃ��ꍇ�͂P�Ƃ���B
				if ( ServiceExecuteRegenerationAllIncrSoldierInMain[iBushouID] < 1 ) {
					ServiceExecuteRegenerationAllIncrSoldierInMain[iBushouID] = 1;
				}
			}
		}
	}

	// �����͂̎����񕜂�0���傫��
	if ( TSModCommand::MyselfGundan::cmdRegenerationSoldierAutomatically > 0) {

		// �������͉񕜃T�[�r�X���󂯂镐��ID��S�����Z�b�g
		for (int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {

			if (nb6bushouref[iBushouID].gundan == 0xFFFF) {
				continue;
			}

			// �v���C���[�̔z���R�c�ł���΁c
			if (isPlayerGundan(nb6bushouref[iBushouID].gundan-1)) {

				//�@���݂̐g���𓾂�
				int position = nb6bushouname[iBushouID].position;

				//�@�g������ő啺���𓾂�
				int soldiermax = nb6soldiermax[position].soldiermax;

				// �ő啺�m���́A�E�҂������芯�ʁE��E���ɂ���ăt�B���^�[��������
				soldiermax = getCustomSoldierMax( iBushouID, soldiermax );

				// �������Ă��ǂ�����(MAX�̕��m��-���݂̕��� / 9-�w�背�x��(0�`5) = �X�`�S)
				ServiceExecuteRegenerationAllIncrSoldierInMain[iBushouID] = 
					(int)ceil
					(
						( 
							(double)soldiermax - (double)nb6bushouref[iBushouID].soldier
						) / (9-TSModCommand::MyselfGundan::cmdRegenerationSoldierAutomatically) 
					);

				// ���������P��菭�Ȃ��ꍇ�͂P�Ƃ���B
				if ( ServiceExecuteRegenerationAllIncrSoldierInMain[iBushouID] < 1 ) {
					ServiceExecuteRegenerationAllIncrSoldierInMain[iBushouID] = 1;
				}
			}
		}
	}
}




// ���݂̐푈��ʂ֓���ۂ̊e�X�̕����̕ۑ�����(�������푈���ɑ���������A�푈�����ł̃^�[���P�ʂő�������B 
int ServiceExecuteRegenerationAllSaveBeginSoldier[GAMEDATASTRUCT_BUSHOU_NUM] = {};

// ����͕K���푈�J�n���ɌĂԂ���
void EXServiceExecuteRegeneration_All_SoldierResetInBattle() {

	for (int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {
		// �S�Ă̕����̕ۑ����������Z�b�g
		ServiceExecuteRegenerationAllSaveBeginSoldier[iBushouID] = 0;
	}
}

void EXServiceExecuteRegeneration_All_SoldierSaveInBattle() {
	// �������񕜂̂ǂ��炩�̃I�v�V�������n�m
	if ( TSModCommand::OthersGundan::cmdRegenerationSoldierAutomatically > 0 || TSModCommand::MyselfGundan::cmdRegenerationSoldierAutomatically > 0) {

		for (int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {

			// ���݂̕������ۑ�����������葽���Ȃ��Ă���΁A�����V���ȕۑ������Ƃ���B
			if ( ServiceExecuteRegenerationAllSaveBeginSoldier[iBushouID] < nb6bushouref[iBushouID].soldier ) {
				ServiceExecuteRegenerationAllSaveBeginSoldier[iBushouID] = nb6bushouref[iBushouID].soldier;
			}
		}
	}
}


int EXServiceExecuteDaimyoHasCastleCnt[GAMEDATASTRUCT_DAIMYO_NUM] = {}; // �e�X�̑喼�����鐔

//�ő吨��(�ł������鐔������)�喼�𓾂�
void EXServiceExecuteSetDaimyoHasCastleCnt() {
	// �܂��A�ꉞ�Ă΂��x�Ƀ��Z�b�g
	for (int iDaimyoID=0; iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM; iDaimyoID++ ) {
		EXServiceExecuteDaimyoHasCastleCnt[iDaimyoID] = 0;
	}

	for (int iCastleID=0; iCastleID < GAMEDATASTRUCT_CASTLE_NUM; iCastleID++ ) {
		// �����喼�ԍ��ɏ]���āA�J�E���g�𑝂₵�Ă䂭�B
		int iGundanID = nb6castle[iCastleID].attach-1;
		if ( iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) { // �N�ɂ��������ĂȂ���͂��̏����Œe�����
			int iDaimyoID = nb6gundan[iGundanID].attach-1;
			EXServiceExecuteDaimyoHasCastleCnt[iDaimyoID]++;
		}
	}
}



// ���݂̐푈��ʂ̃^�[���ł̊e�X�̕������������Ă��ǂ����m�̐�(�e�X�̕����̕�����1+Lv%[�J�グ] ���ۂɂ͎w���6�`Lv%�ʂ�[�J�グ]))
int ServiceExecuteRegenerationAllIncrSoldierInBattle[GAMEDATASTRUCT_BUSHOU_NUM] = {};

// ��L�̊e�X�̕����ɂ��āA�푈��ʂ̢������������v�Z����B
void EXServiceExecuteRegeneration_All_SoldierCalcInBattle() {

	// �����͂̎����񕜂�0���傫��
	if ( TSModCommand::OthersGundan::cmdRegenerationSoldierAutomatically > 0) {

		// �v���C���S���喼�̏�����̕��ϐ������߂�
		int iPlayerTantoDaimyoCnt = 0;
		int iPlayerTantoCastleCnt = 0;

		// �v���C���̐i���ƁA�v���C���ȊO�̑S�̂̌R�c�̐i�������A�v���C���ȊO�̌R�c�݂͌��Ɏ����o�����X����낤�Ƃ��܂��B
		// ����Ă����ł́A�v���C���Ƃ̑Δ�ŁA�邪���Ȃ��قǁA�����W���������A�����قǑ����W�������Ȃ�
		if ( TSModCommand::OthersGundan::cmdPlayerAndTheWholeInfluenceEachOtherData > 0 ) {

			// �S�Ă̑喼�̏鐔���Z�b�e�B���O����B����p�^�C�v�B
			EXServiceExecuteSetDaimyoHasCastleCnt();

			for ( int iDaimyoID=0; iDaimyoID<GAMEDATASTRUCT_DAIMYO_NUM; iDaimyoID++ ) {

				// �Y���̓v���C���S���̑喼��
				if ( isPlayerDaimyo( iDaimyoID ) ) {
					iPlayerTantoDaimyoCnt++;
					iPlayerTantoCastleCnt += EXServiceExecuteDaimyoHasCastleCnt[iDaimyoID];
				}
			}
			// �v���C�����S�����Ă���喼�̕��Ϗ����鐔�����߂�
			if ( iPlayerTantoDaimyoCnt > 0 ) {
				iPlayerTantoCastleCnt = iPlayerTantoCastleCnt / iPlayerTantoDaimyoCnt;
			}
		}

		if ( iPlayerTantoCastleCnt <= 0 ) {
			// �v�Z���₷���悤�ɁA�Œ�ł��P�͎����Ă邱�Ƃɂ��Ă��܂��B
			iPlayerTantoCastleCnt = 1;
		}

		// �������͉񕜃T�[�r�X���󂯂镐��ID��S�����Z�b�g
		for (int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {

			if (nb6bushouref[iBushouID].gundan == 0xFFFF) {
				continue;
			}

			// �v���C���[�̔z���R�c�łȂ���΁c
			if (! isPlayerGundan(nb6bushouref[iBushouID].gundan-1)) {

				float fCoef = 1.0;
				if ( TSModCommand::OthersGundan::cmdPlayerAndTheWholeInfluenceEachOtherData > 0 ) {
					// �Y����������������喼ID(�喼�ԍ�)
					int iTargetDaimyoID = nb6bushouref[iBushouID].attach -1;
					// �Y���̑喼�����鐔
					int iTargetCastleNum = EXServiceExecuteDaimyoHasCastleCnt[iTargetDaimyoID];

					// �v���C���[�̎����Ă�鐔 / �Y���喼�������Ă�鐔���A�����W���̂P�Ƃ���B
					// ����ɂ��v���C���[���͂��傫���قǁA�P�^�[��������̕��͑��������傫���Ȃ�B
					if ( iTargetCastleNum > 0 ) {
						fCoef = (float)iPlayerTantoCastleCnt/(float)iTargetCastleNum;
						fCoef = fCoef*TSModCommand::OthersGundan::cmdPlayerAndTheWholeInfluenceEachOtherData;
						if ( fCoef > 25 ) { fCoef = 25; }
						if ( fCoef < 1 ) { fCoef = 1; }
					}
				}

				// �������Ă��ǂ�����(�푈�J�n���`���݂܂ł̍ő啺����6%)
				ServiceExecuteRegenerationAllIncrSoldierInBattle[iBushouID] = 
					(int)ceil(
						(double)ServiceExecuteRegenerationAllSaveBeginSoldier[iBushouID] * (double)(TSModCommand::OthersGundan::cmdRegenerationSoldierAutomatically+fCoef) / 100
					);
			}
		}
	}

	// �����͂̎����񕜂�0���傫��
	if ( TSModCommand::MyselfGundan::cmdRegenerationSoldierAutomatically > 0) {

		// �������͉񕜃T�[�r�X���󂯂镐��ID��S�����Z�b�g
		for (int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {

			if (nb6bushouref[iBushouID].gundan == 0xFFFF) {
				continue;
			}

			// �v���C���[�̔z���R�c�ł���΁c
			if (isPlayerGundan(nb6bushouref[iBushouID].gundan-1)) {

				// �������Ă��ǂ�����(�푈�J�n���`���݂܂ł̍ő啺����6%)
				ServiceExecuteRegenerationAllIncrSoldierInBattle[iBushouID] = 
					(int)ceil(
						(double)ServiceExecuteRegenerationAllSaveBeginSoldier[iBushouID] * (double)(1+TSModCommand::MyselfGundan::cmdRegenerationSoldierAutomatically) / 100
					);
			}
		}
	}
}





// �����񕜃T�[�r�X���󂯂�S����
EXBushou EXB_RegenerationBushou[GAMEDATASTRUCT_BUSHOU_NUM];

// 1�񂾂��ݒ肷��̂�
bool bEXServiceExecuteRegeneration_All_SetBushouID = false;

void EXServiceExecuteRegeneration_All_SetBushouID() {
	// ���ݒ�Ȃ�
	if ( !bEXServiceExecuteRegeneration_All_SetBushouID ) {
		// �ݒ�ς݂Ƃ���
		bEXServiceExecuteRegeneration_All_SetBushouID = true;

		// �������͉񕜃T�[�r�X���󂯂镐��ID��S�����Z�b�g
		for (int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {
			EXB_RegenerationBushou[iBushouID].setNb6BushouIX(iBushouID);
		}
	}
}


void EXServiceExecuteRegeneration_All_CheckRegeneration(int isForce = 0) {


	// �����͂̎����񕜂�0���傫��
	if ( TSModCommand::OthersGundan::cmdRegenerationSoldierAutomatically > 0) {

		// �v���C���S���喼�̏�����̕��ϐ������߂�
		int iPlayerTantoDaimyoCnt = 0;
		int iPlayerTantoCastleCnt = 0;

		// �v���C���̐i���ƁA�v���C���ȊO�̑S�̂̌R�c�̐i�������A�v���C���ȊO�̌R�c�݂͌��Ɏ����o�����X����낤�Ƃ��܂��B
		// ����Ă����ł́A�v���C���Ƃ̑Δ�ŁA�邪���Ȃ��قǁA�����W���������A�����قǑ����W�������Ȃ�
		if ( TSModCommand::OthersGundan::cmdPlayerAndTheWholeInfluenceEachOtherData > 0 ) {

			// �S�Ă̑喼�̏鐔���Z�b�e�B���O����B����p�^�C�v�B
			EXServiceExecuteSetDaimyoHasCastleCnt();

			for ( int iDaimyoID=0; iDaimyoID<GAMEDATASTRUCT_DAIMYO_NUM; iDaimyoID++ ) {

				// �Y���̓v���C���S���̑喼��
				if ( isPlayerDaimyo( iDaimyoID ) ) {
					iPlayerTantoDaimyoCnt++;
					iPlayerTantoCastleCnt += EXServiceExecuteDaimyoHasCastleCnt[iDaimyoID];
				}
			}
			// �v���C�����S�����Ă���喼�̕��Ϗ����鐔�����߂�
			if ( iPlayerTantoDaimyoCnt > 0 ) {
				iPlayerTantoCastleCnt = iPlayerTantoCastleCnt / iPlayerTantoDaimyoCnt;
			} 
		}

		if ( iPlayerTantoCastleCnt <= 0 ) {
			// �v�Z���₷���悤�ɁA�Œ�ł��P�͎����Ă邱�Ƃɂ��Ă��܂��B
			iPlayerTantoCastleCnt = 1;
		}

		// �������͉񕜃T�[�r�X���󂯂镐��ID��S�����Z�b�g
		for (int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {

			if (nb6bushouref[iBushouID].gundan == 0xFFFF) {
				continue;
			}

			// �v���C���[�̔z���R�c�łȂ���΁c
			if (! isPlayerGundan(nb6bushouref[iBushouID].gundan-1)) {

				float fCoef = 1.0;
				byte lv = TSModCommand::OthersGundan::cmdRegenerationSoldierAutomatically;
				fCoef = float(lv * lv); // �f�t�H���g�Ȃ炱�̒l

				if ( TSModCommand::OthersGundan::cmdPlayerAndTheWholeInfluenceEachOtherData > 0 ) {
					// �Y����������������喼ID(�喼�ԍ�)
					int iTargetDaimyoID = nb6bushouref[iBushouID].attach -1;
					// �Y���̑喼�����鐔
					int iTargetCastleNum = EXServiceExecuteDaimyoHasCastleCnt[iTargetDaimyoID];

					// �v���C���[�̎����Ă�鐔 / �Y���喼�������Ă�鐔���A�����W���̂P�Ƃ���B
					// ����ɂ��v���C���[���͂��傫���قǁA�P�^�[��������̕��͑��������傫���Ȃ�B
					if ( iTargetCastleNum > 0 ) {
						// ���x���������قǒZ�����Ԃŉ񕜂���B���x���̂Q��ŒZ���Ȃ��Ă䂭�B
						fCoef = (float)iPlayerTantoCastleCnt/(float)iTargetCastleNum;
						if (fCoef > 20*TSModCommand::OthersGundan::cmdPlayerAndTheWholeInfluenceEachOtherData ) { 
							fCoef = float(20*TSModCommand::OthersGundan::cmdPlayerAndTheWholeInfluenceEachOtherData);
						}
						fCoef = (float)lv*(float)lv*fCoef;
						if ( fCoef > 100 ) { fCoef = 100; }
						if ( fCoef < 1 ) { fCoef = 1; }
					}
				}
				if ( fCoef > 0 ) {
					EXB_RegenerationBushou[iBushouID].checkRegenerationNB6BushouSoldier((int)((float)250000/fCoef), isForce); 
				}
			}
		}
	}

	// �����͂̎����񕜂�0���傫��
	if ( TSModCommand::MyselfGundan::cmdRegenerationSoldierAutomatically > 0) {

		// �������͉񕜃T�[�r�X���󂯂镐��ID��S�����Z�b�g
		for (int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {

			if (nb6bushouref[iBushouID].gundan == 0xFFFF) {
				continue;
			}

			// �v���C���[�̔z���R�c�ł���΁c
			if (isPlayerGundan(nb6bushouref[iBushouID].gundan-1)) {

				// ���x���������قǒZ�����Ԃŉ񕜂���B���x���̂Q��ŒZ���Ȃ��Ă䂭�B
				byte lv = TSModCommand::MyselfGundan::cmdRegenerationSoldierAutomatically;
				EXB_RegenerationBushou[iBushouID].checkRegenerationNB6BushouSoldier(250000/(lv*lv), isForce); 
			}

		}
	}

}


void EXSuperService_Regeneration(int isForce) {

	// ����ID����ʂ�o�^
	EXServiceExecuteRegeneration_All_SetBushouID();

	// ���W�F�l�`�F�b�N
	EXServiceExecuteRegeneration_All_CheckRegeneration(isForce);

}