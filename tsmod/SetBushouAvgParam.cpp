#include "WinTarget.h"

#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "VariableNameRegistAndJudge.h"

#include "SetBushouAvgParam.h"
#include "EXBushou_All.h"

void CommonSetBushouAvgParamWithRekidaiFlagOn(int iBushouID, int iFaceID);
void CommonSetBushouAvgParam(int iBushouID, int iFaceID);

// �E�Ƃɂ��Ă̓t���O�Ɋ֌W�Ȃ��ݒ�
extern void setJobEXOfCurABushou(int iBushouID);

// �V�i���I�J�n/���[�h���p�̃p�����^���f����
void SetBushouAvgParam(int iFlagValue) {

	// �v�Z�y�ʉ��̂��߂ɁA�ŏ��Ɉ�x�o�^���Ă����B
	bool is_original_bushou_flag[GAMEDATASTRUCT_BUSHOU_NUM] = {false};

	for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

		// ���X�V�ċL�ɋ��镐���ł���΁A�\�͂𔽉f�\
		if ( IsOriginalRegistedBushou(iBushouID) ) {
			// ���̗�`��1300�Ԗ����ɂȂ��Ă���B(���O������v���Ă��āA����`�Ⴄ�ȂǂƂ������Ƃ͂Ȃ�)
			if ( nb6bushouname[iBushouID].number < GAMEDATASTRUCT_REGISTED_BUSHOU_NUM ) {
				is_original_bushou_flag[iBushouID] = true;
			}
		}
	}

	// �ƕ�ɂ��ǉ��l
	byte kahou_add_gov[GAMEDATASTRUCT_BUSHOU_NUM] = {};
	byte kahou_add_bat[GAMEDATASTRUCT_BUSHOU_NUM] = {};
	byte kahou_add_int[GAMEDATASTRUCT_BUSHOU_NUM] = {};
	byte kahou_add_amb[GAMEDATASTRUCT_BUSHOU_NUM] = {};

	// �ƕ�͂��Ȃ炸�A�V�ċL�Ɍ��ݓo�ꂵ�Ă���z(532�����o�[)�������ƂƂȂ�B
	for ( int iKahouID=0; iKahouID < GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {

		// �����҂��������ǂ���
		int iHasID = nb6kahou[iKahouID].attach - 1;
		if ( 0 <= iHasID && iHasID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			// �����҂������̏ꍇ
			// �o�ꂵ�Ă���Ƃ�������

			int ktype = nb6kahou[iKahouID].type;
			int krank = nb6kahou[iKahouID].param;

			// ���������^�C�v�F0 ���q  1 ����  2 ����  3 ����  4 �ԓ�  
			if ( 0x0 <= ktype && ktype <= 0x4 ) {
				// ����̉ƕ�̕����㏸�l���傫���Ȃ�΁c
				if ( krank > kahou_add_gov[iHasID] ) {
					kahou_add_gov[iHasID] = krank;
				}
			}
			// �퓬�����^�C�v�F5 ����  6 ��  7 �  8 �n  9 ��  A �w�H�D   
			else if ( 0x5 <= ktype && ktype <= 0xA ) {
				// ����̉ƕ�̕����㏸�l���傫���Ȃ�΁c
				if ( krank > kahou_add_bat[iHasID] ) {
					kahou_add_bat[iHasID] = krank;
				}
			}
			// �q�d�����^�C�v�FB ���q�{  C �����Ԗ{
			else if ( 0xB <= ktype && ktype <= 0xC ) {
				// ����̉ƕ�̕����㏸�l���傫���Ȃ�΁c
				if ( krank > kahou_add_int[iHasID] ) {
					kahou_add_int[iHasID] = krank;
				}
			}
			// ��]�����^�C�v�FD ����  E �n���V  F ��؎��v  10 ���n��  11 �\����  12 ����  13 �ʎ蔠  14 �r�[�h���t  15 ���ዾ  16 ���\����  17 ����
			else if ( 0xD <= ktype && ktype <= 0x17 ) {
				// ����̉ƕ�̕����㏸�l���傫���Ȃ�΁c
				if ( krank > kahou_add_amb[iHasID] ) {
					kahou_add_amb[iHasID] = krank;
				}
			}
		} else {
			continue;
		}
	}

	// ���ʂɂ�閣�͒l�̒ǉ�
	byte kani_add_chm[GAMEDATASTRUCT_BUSHOU_NUM] = {};

	// ���ʂ͂��Ȃ炸�A�V�ċL�Ɍ��ݓo�ꂵ�Ă���z(532�����o�[)�������ƂƂȂ�B
	for ( int iKaniID=0; iKaniID < GAMEDATASTRUCT_KANI_NUM; iKaniID++) {
		// �����҂��������ǂ���
		int iHasID = nb6kani[iKaniID].attach - 1;
		if ( 0 <= iHasID && iHasID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			// �����҂������̏ꍇ
			// �o�ꂵ�Ă���Ƃ�������

			int krank = nb6kani[iKaniID].param;
			// ����̊��ʂ̕����㏸�l���傫���Ȃ�΁c
			if ( krank > kani_add_chm[iHasID] ) {
				kani_add_chm[iHasID] = krank;
			}
		} else {
			continue;
		}
	}


	// �V�ċL�ɋ��镐���̔\�͂���V���[�Y�ɉ����`�ɂ���Ȃ��(1==�V�K�V�i���I���ɔ��f����)
	for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

		// �V�ċL�Ɍ��X���݂��镐���Ȃ�΁c
		if ( is_original_bushou_flag[iBushouID] ) {
			int iFaceID = nb6bushouname[iBushouID].number;

			// ���V���[�Y�t���O���n�m�ł���A�V�i���I�J�n���܂ł��A�Z�[�u�f�[�^�����邩�͈̔͂��l��
			if ( 0 < TSModCommand::AllBushou::cmdParamConsiderNobunagaSeries &&
				     iFlagValue <= TSModCommand::AllBushou::cmdParamConsiderNobunagaSeries ) {

				// �����E�퓬�E�q�d
				nb6bushouref[iBushouID].maxgov = nb6AvgParam_Gov[iFaceID] + kahou_add_gov[iBushouID];
				nb6bushouref[iBushouID].maxbat = nb6AvgParam_Bat[iFaceID] + kahou_add_bat[iBushouID];
				nb6bushouref[iBushouID].maxint = nb6AvgParam_Int[iFaceID] + kahou_add_int[iBushouID];
				// ��]�E����
				nb6bushouref[iBushouID].ambition = nb6AvgParam_Amb[iFaceID] + kahou_add_amb[iBushouID];
				nb6bushouref[iBushouID].charm = nb6AvgParam_Chm[iFaceID] + kani_add_chm[iBushouID];

				// ���ʂ̃p�����^����
				CommonSetBushouAvgParamWithRekidaiFlagOn(iBushouID, iFaceID);
			}

			CommonSetBushouAvgParam(iBushouID, iFaceID);
		}

	}
}



// �����o�ꎞ�p�̃p�����^���f����
void SetBushouAvgParamAppearanceBushou(int iBushouID) {

	SetExBushouAvgParam(iBushouID); // ���ꕐ���p�̃p�����^�ݒ�

	// ���X�V�ċL�ɋ��镐���ł���΁A�\�͂𔽉f�\
	if ( IsOriginalRegistedBushou(iBushouID) ) {

		// ���̗�`��1300�Ԗ����ɂȂ��Ă���B(���O������v���Ă��āA����`�Ⴄ�ȂǂƂ������Ƃ͂Ȃ�)
		if ( nb6bushouname[iBushouID].number < GAMEDATASTRUCT_REGISTED_BUSHOU_NUM ) {

			int iFaceID = nb6bushouname[iBushouID].number;

			if ( TSModCommand::AllBushou::cmdParamConsiderNobunagaSeries ) { // 1�ł�2�ł��K������B
				// �����E�퓬�E�q�d
				nb6bushouref[iBushouID].maxgov = nb6AvgParam_Gov[iFaceID];
				nb6bushouref[iBushouID].maxbat = nb6AvgParam_Bat[iFaceID];
				nb6bushouref[iBushouID].maxint = nb6AvgParam_Int[iFaceID];
				// ��]�E����
				nb6bushouref[iBushouID].ambition = nb6AvgParam_Amb[iFaceID];
				nb6bushouref[iBushouID].charm = nb6AvgParam_Chm[iFaceID];

				// ���ʂ̃p�����^����
				CommonSetBushouAvgParamWithRekidaiFlagOn(iBushouID, iFaceID);
			}

			// �E�Ƃɂ��Ă̓t���O�Ɋ֌W�Ȃ��ݒ�
			setJobEXOfCurABushou(iBushouID);

			CommonSetBushouAvgParam(iBushouID, iFaceID);

		}

	}
}

// ���V���[�Y���n�m�̎��������B���ϒl���f�̋��ʏ���(�V�i���I�J�n���ƁA�����o�ꎞ�̋���)
void CommonSetBushouAvgParamWithRekidaiFlagOn(int iBushouID, int iFaceID) {

	// �`��
	nb6bushouref[iBushouID].duty = nb6AvgParam_Gir[iFaceID];

	// ���y
	setBushouAptitude( iBushouID, APTITUDE_ASIGARU, nb6AvgParam_Bat_Ashigaru[iFaceID] );
	// �R�n
	setBushouAptitude( iBushouID, APTITUDE_KIBA, nb6AvgParam_Bat_Kiba[iFaceID] );
	// �S�C
	setBushouAptitude( iBushouID, APTITUDE_TEPPOU, nb6AvgParam_Bat_Teppou[iFaceID] );
	// ���R
	setBushouAptitude( iBushouID, APTITUDE_SUIGUN, nb6AvgParam_Bat_Suigun[iFaceID] );
}

// ���ƁE���l �ȂǂƂ������u�������悤�̕�����1300�l���̃��X�g
extern char *pOriginalBushouJobExList[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM];

// ���V���[�Y���n�m���ǂ����֌W�Ȃ��B���ϒl���f�̋��ʏ���
void CommonSetBushouAvgParam(int iBushouID, int iFaceID) {


	// �|��TSMod.txt�̐ݒ�ɂ͊֌W�Ȃ��ݒ肷��B
	// �|�̓K�����R�s�[
	// �|�������Ƃ����t���O��ON�Ȃ̂ł���΁A0�ŏ㏑������B
	if ( TSModCommand::World::cmdTheYumiNoExists ) {
		nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = 0;
	} else {
		nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = nb6AvgParam_Bat_Yumi[iFaceID];
		if (0 <= iFaceID && iFaceID < GAMEDATASTRUCT_REGISTED_BUSHOU_NUM) {
			if (strstr(pOriginalBushouJobExList[iFaceID], "�|")) {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = max(3, nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi); // B�ȏ�ɂ͂��ċ|�͎g����悤��
			}
		}
	}
	// ����TSMod.txt�̐ݒ�ɂ͊֌W�Ȃ��ݒ肷��B
	// ���̓K�����R�s�[
	nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari = nb6AvgParam_Bat_Yari[iFaceID];

	// ���ꑮ���́A��ʕ����͎������킹�Ȃ��̂ŁA�S�ăN���A
	nb6savedata_ex.bushou_ex[iBushouID].aptitudeono = 0;
	nb6savedata_ex.bushou_ex[iBushouID].aptitudemuchi = 0;
	nb6savedata_ex.bushou_ex[iBushouID].aptitudetuchi = 0;
	nb6savedata_ex.bushou_ex[iBushouID].aptitudetue = 0;
	nb6savedata_ex.bushou_ex[iBushouID].aptitudeken = 0;

	// ���{�E����
	nb6savedata_ex.bushou_ex[iBushouID].education = nb6AvgParam_Edu[iFaceID];
	nb6savedata_ex.bushou_ex[iBushouID].command = nb6AvgParam_Cmd[iFaceID];
	// �_��
	nb6savedata_ex.bushou_ex[iBushouID].aptitudenougyou = nb6AvgParam_Nougyou[iFaceID];
	// ����
	nb6savedata_ex.bushou_ex[iBushouID].aptitudesyougyou = nb6AvgParam_Syougyou[iFaceID];
	// �z��
	nb6savedata_ex.bushou_ex[iBushouID].aptitudechikujo = nb6AvgParam_Chikujo[iFaceID];
	// �O��
	nb6savedata_ex.bushou_ex[iBushouID].aptitudegaikou = nb6AvgParam_Gaikou[iFaceID];
	// �o�p
	nb6savedata_ex.bushou_ex[iBushouID].aptitudetouyou = nb6AvgParam_Touyou[iFaceID];
}

