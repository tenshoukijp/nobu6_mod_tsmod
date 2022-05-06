#define _CRT_SECURE_NO_WARNINGS

#pragma pack(1)

#include "WinTarget.h"

#include "EXBushou.h"
#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"



extern int getCustomSoldierMax(int, int);


EXBushou::EXBushou() {
	// ���O�͋�ŃX�^�[�g
	m_name[0] = NULL;

	// �����f�[�^�z���ł�IX(����)��-1�ŃZ�b�g
	m_nb6bushou_ix = -1;

	m_curTimeGetTime = ::timeGetTime();
	m_preTimeGetTime = m_curTimeGetTime;

	// �΂̎��ɂ���
	m_lifetimes = 1;
}

void EXBushou::setName(char *name) {
	strcpy(m_name, name);
}


void EXBushou::setNb6BushouIX(int iBushouID) {
	m_nb6bushou_ix = iBushouID;
}

int EXBushou::getNb6BushouIX() {


	// ���łɕ����f�[�^�z���ł�Index���������Ă���Ȃ�A�����Ԃ�
	if (m_nb6bushou_ix >= 0) {
		// ���S���Ă���Ȃ�Ζ��O��������-1��Ԃ�
		if ( 0 <= m_nb6bushou_ix && m_nb6bushou_ix < GAMEDATASTRUCT_BUSHOU_NUM ) {
			if ( nb6bushouname[m_nb6bushou_ix].State == 7 ) {
				strcpy(m_name, "");
				return -1;
			}
		}
 
		return m_nb6bushou_ix;

	// ���O���ݒ肳��Ă���΁Aix����������B
	} else if (strlen(m_name) > 2) {

		for (int b_ix=0; b_ix<GAMEDATASTRUCT_BUSHOU_NUM; b_ix++) {
			// �c��+���O����v������A���ꂪ�����̔ԍ���!!

			// �c��+���O�̘g��p��
			char fullname[100];
			// �c��+���O���Z�b�g
			wsprintf(fullname, "%s%s", nb6bushouname[b_ix].familyname, nb6bushouname[b_ix].fastname);

			fullname[12] = NULL;

			// ��r���Ĉ�v������A�z��ԍ���Ԃ�
			if (strcmp( m_name, fullname) == 0) {
				// �Ȍ㌟�����Ȃ��Ă��ǂ��悤�ɃZ�b�g
				m_nb6bushou_ix = b_ix;
				
				// �l��Ԃ��Ă���
				return b_ix;
			}
		}
		return m_nb6bushou_ix;

	// 
	} else {
		return m_nb6bushou_ix;
	}
}

// �e�X�̕��������G�߃��C����ʂő��������Ă��ǂ����m��
extern int ServiceExecuteRegenerationAllIncrSoldierInMain[GAMEDATASTRUCT_BUSHOU_NUM];
// �e�X�̕���������̐퓬��ʂő��������Ă��ǂ����m��
extern int ServiceExecuteRegenerationAllIncrSoldierInBattle[GAMEDATASTRUCT_BUSHOU_NUM];
// �e�X�̕������푈�J�n�O�ɕۑ��������w��(�푈���ɒ�������Ȃ肵�čő傷����������ƈꉞ�푈���^�[���؂�ւ����ɑ�����);
extern int ServiceExecuteRegenerationAllSaveBeginSoldier[GAMEDATASTRUCT_BUSHOU_NUM];



void EXBushou::checkRegenerationNB6BushouSoldier(int RefleshTime, int isForce ) {

	// �����z��f�[�^���ɁA���g���m�����ꂽ��Ԃł����
	int iBushouID = getNb6BushouIX();

	// �����z��f�[�^���ɁA���g���m�����ꂽ��Ԃł����
	if ( !(0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) ) {
		return;
	}

	m_curTimeGetTime = timeGetTime();

	// RefleshTime�~���b�ɂP�������񕜁A�܂��A�����t���O�ɂP�������Ă鎞
	if ( int(m_curTimeGetTime - m_preTimeGetTime) > int(RefleshTime) || isForce ) { // RefleshTime�~���b��1�������񕜂��Ă䂭

		m_preTimeGetTime = m_curTimeGetTime;



		int iSoldierNum    = nb6bushouref[iBushouID].soldier;  // ����
		int iSoldierTrain  = nb6bushouref[iBushouID].training; // �P��
		int iSoldierLoyal  = nb6bushouref[iBushouID].solloyal; // ����


		//�@���݂̐g���𓾂�
		int position = nb6bushouname[iBushouID].position;
		//�@�g������ő啺���𓾂�
		int soldiermax = nb6soldiermax[position].soldiermax;

		// �ő啺�m���́A�E�҂������芯�ʁE��E���ɂ���ăt�B���^�[��������
		soldiermax = getCustomSoldierMax( iBushouID, soldiermax );

		// ���m���O�̎������I�ɑ��₷(��ɑ���ύX����Q�[���X�^�[�g��)
		if ( iSoldierNum == 0 && isForce ) {

			// ���͂��P���₷�B
			iSoldierNum++;

			nb6bushouref[iBushouID].soldier = iSoldierNum;

			// ���͂O���瑝�₷�̂ŌP���l�̐ݒ�B�P���l��35+���(100���Z)/3 �Ƃ���B
			iSoldierTrain = 35 + (nb6bushouref[iBushouID].maxbat / 3);

			nb6bushouref[iBushouID].training = iSoldierTrain;

			// ���͂O���瑝�₷�̂Ŏm�C�l�̐ݒ�B�m�C�l��35+����(100���Z)/3 �Ƃ���B
			iSoldierLoyal = 35 + (nb6bushouref[iBushouID].charm / 3);

			nb6bushouref[iBushouID].solloyal = iSoldierLoyal;


		// �ő啺������葽���Ȃ��Ă��܂��Ă���ꍇ�������f�B���O������
		} else if (iSoldierNum > soldiermax) {

			// ���͂��P���炷�B
			if ( iSoldierNum > 0 ) { iSoldierNum--; } else { iSoldierNum = 0; }

			nb6bushouref[iBushouID].soldier = iSoldierNum;


		// 1�ȏ�̕��������āA�Y���̍ő啺����菭�Ȃ��ꍇ�̂ݏ������s�� (����������؂�ւ����̂݁A0�ȏ�Ȃ瑝������������)
		} else if (1 <= iSoldierNum && iSoldierNum < soldiermax ) {

			// �s���`�̎��́c
			if (iSoldierNum < soldiermax*0.3 ) { // ������3�������ł��������o�C
				m_preTimeGetTime -= DWORD(RefleshTime*0.8); // 5000�̏ꍇ�A4000�������ƂŁA����̍X�V��1�b�Ԋu�Ƃ���B(���Ԃ��S�b�o�߂������Ƃɂ��Ă��܂�)
			} else if ( iSoldierNum < soldiermax*0.6 ) { // ������6���� �ł�����ƌ����Ă���
				m_preTimeGetTime -= DWORD(RefleshTime*0.4); // 5000�̏ꍇ�A2000�������ƂŁA����̍X�V��3�b�Ԋu�Ƃ���B(���Ԃ��Q�b�o�߂������Ƃɂ��Ă��܂�)
			}

			if (  m_isUltraBushou ) {
				// ���͂��P���₷�B
				iSoldierNum++;

				nb6bushouref[iBushouID].soldier = iSoldierNum;

			// ���ݐ푈���N���Ă��炸�A���C����ʂ̎c�����Z�����c���Ă���Ȃ�΁c
			} else if ( !CommonGlobalHandle::isBattleMode && ServiceExecuteRegenerationAllIncrSoldierInMain[iBushouID] > 0 ) { // ���C����ʗp�̑������Ă��ǂ��������c���Ă���

				// ���͂��P���₷�B
				iSoldierNum++;

				// ���₵���������炷�B
				ServiceExecuteRegenerationAllIncrSoldierInMain[iBushouID]--;

				nb6bushouref[iBushouID].soldier = iSoldierNum;

			// ���ݐ푈���ŁA�푈�p�̎c�����Z�����c���Ă���Ȃ�΁c
			} else if ( CommonGlobalHandle::isBattleMode && ServiceExecuteRegenerationAllIncrSoldierInBattle[iBushouID] > 0 ) { // �퓬��ʗp�̑������Ă��ǂ��������c���Ă���

				// ���݂̕��m�����A�푈�O��菭�Ȃ��Ȃ�΁c
				if ( iSoldierNum < ServiceExecuteRegenerationAllSaveBeginSoldier[iBushouID] ) {
					// ���͂��P���₷�B
					iSoldierNum++;

					// ���₵���������炷�B
					ServiceExecuteRegenerationAllIncrSoldierInBattle[iBushouID]--;

					nb6bushouref[iBushouID].soldier = iSoldierNum;
				}
			}


		}

		
	}

}


// ��C�̏C��
void EXBushou::checkRestoreNB6BushouTaihou() {
	
	// �����z��f�[�^���ɁA���g���m�����ꂽ��Ԃł����
	int iBushouID = getNb6BushouIX();

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		// ��C���C��
		nb6bushouref[iBushouID].biggun = 1;

	}
}


/*
 * �����̓f�t�H���g�Ő��{�ł���B
 */
void EXBushou::checkTimesNB6BushouLife() {

	// �܂����C���Q�[���܂���ĂȂ���΁A���s���Ȃ�
	if ( !CommonGlobalHandle::isMainGameStart ) {
		return;
	}

	// �΂����̂��x���Ȃ��{����������R�}���h���L���̎���������
	if ( TSModCommand::AllBushou::cmdLifeIsTheTimes >= 2 ) {
		return;
	}

	// �~����t�ɂ���鎞�ȊO�ȊO�͖���
	if ( nb6year.season != 3 ) {
		return;
	}

	int iBushouID = getNb6BushouIX();

	// �����z��f�[�^���ɁA���g���m�����ꂽ��Ԃł����
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// �R�{�Ƃ��Ă�������year�����������A0�łȂ���΁A�΂����Ȃ��B
		// �΂����Ȃ����߂ɂ́A��a���N����P���₳�Ȃ���΂Ȃ�Ȃ��B
		if ( (nb6year.year % m_lifetimes) != 0 ) {

			// �a���N���P�B255���Ƒ��₳�Ȃ�(0�ɂȂ��Ă܂�)
			if ( nb6bushouref[iBushouID].birth <= 254 ) {

				int old = getYearsOld(iBushouID);
				if (old >= 13) {
					int birth = nb6bushouref[iBushouID].birth;
					birth++;
					nb6bushouref[iBushouID].birth = birth;
				}
			}
		}
	}
}