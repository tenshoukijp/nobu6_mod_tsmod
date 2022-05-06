#define _CRT_SECURE_NO_WARNINGS

#include "EXBushou_All.h"
#include "GameDataStruct.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"

// �����X���b�g�^���^���X
EXBushou_LancelotTartare EXB_LancelotTartare;

#define EXB_TempleCommandNum 7
#define EXB_ZenobiaMemberNum 7

// ���ȉ����̏���
EXBushou EXB_TempleCommand[EXB_TempleCommandNum]; // ���H���b�N�`���F���T���A
bool setEXB_TempleCommand = false;
// ���ȏ㉼�̏���


EXBushou EXB_ZenobiaMember[EXB_ZenobiaMemberNum]; // �E�H�[�����`�f�l�u
bool setEXB_ZenobiaMember = false;


EXBushou_ShuiHidechiyo EXB_ShuiHidechiyo;
bool setEXB_NiouMember = false; // �m�������o�[



// �Ώۂ̕��������X���[���A�����ǂ���
bool IsLoslorien(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		char szFullName[20] = "";
		strcat(szFullName, nb6bushouname[iBushouID].familyname);
		strcat(szFullName, nb6bushouname[iBushouID].fastname);
	
		for ( int i=0; i<EXB_TempleCommandNum; i++ ) {
			if ( strcmp(szFullName, EXB_TempleCommand[i].getName())==0 ) {
				return true;
			}
		}
	}
	return false;
}

void EXBushou_All_SetEXBushouInit() {

	// �e���v���R�}���h�����ݒ�
	// ���ȉ����̏���
	if ( !setEXB_TempleCommand ) {
		setEXB_TempleCommand = true;

		// �o�[���[�t�H��
		EXB_TempleCommand[0].setName("�I�J�K�L�M�N");

		// �E�B���U���t
		EXB_TempleCommand[1].setName("�s�t�u�v�w�x");

		// �o���o�X
		EXB_TempleCommand[2].setName("�U�V�W�X�Y�Z");

		// �}���e�B��
		EXB_TempleCommand[3].setName("�c�d�`�a�b");

		// �n�{����
		EXB_TempleCommand[4].setName("�I�J�K�d�e�f");

		// �I�Y�}
		EXB_TempleCommand[5].setName("�v�w�x�y�z�{");

		// �x���T���A
		EXB_TempleCommand[6].setName("�^�_�`�a�b�c");

		// �����X���b�g�^���^���X���X�[�p�[���[�h��
		EXB_LancelotTartare.setSuperMode(true);

		// ���̖ʎq���X�[�p�[���[�h��
		for (int i=0; i<EXB_TempleCommandNum; i++) {
			EXB_TempleCommand[i].setSuperMode(true);
		}
	}

	// �[�m�r�A�������Ă����ҒB
	if ( !setEXB_ZenobiaMember ) {
		setEXB_ZenobiaMember = true;

		// �E�H�[����
		EXB_ZenobiaMember[0].setName("�q�r�n�o�p");
		int iWoBushouID = EXB_ZenobiaMember[0].getNb6BushouIX();
		if ( 0 <= iWoBushouID && iWoBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			nb6savedata_ex.bushou_ex[iWoBushouID].aptitudeyumi = 1;
		}

		// �J�m�[�v�X
		EXB_ZenobiaMember[1].setName("�O�u�[�\�]");
		EXB_ZenobiaMember[1].setLifeTimes(3);

		// �M���o���h
		EXB_ZenobiaMember[2].setName("�g�h�i���");

		// �~���f�B��
		EXB_ZenobiaMember[3].setName("�O�P�Q�R�S�T");

		// �M���_�X
		EXB_ZenobiaMember[4].setName("�G�H�F������");

		// �f�l�u
		EXB_ZenobiaMember[5].setName("�j�k�l�m");
		EXB_ZenobiaMember[5].setLifeTimes(3);

		// �A���r���I�E���h����
		EXB_ZenobiaMember[6].setName("�y�z�{�|�}�~");
		EXB_ZenobiaMember[6].setLifeTimes(3);

		// ���̖ʎq���X�[�p�[���[�h��
		for (int i=0; i<EXB_ZenobiaMemberNum; i++) {
			EXB_TempleCommand[i].setSuperMode(true);
		}

	}

	if (!setEXB_NiouMember) {
		setEXB_NiouMember = true;

		// ��ɏG�����X�[�p�[���[�h��
		EXB_ShuiHidechiyo.setSuperMode(true);

	}

	// ���ȏ㉼�̏���
}

// �΂��Ƃ�̂�x�����鏈���B�����܂�TSMod.txt�ŗL���ɂ��Ă��鎞�����B
void EXBushou_All_CheckLifeTimes() {

	// �����X���b�g�^���^���X
	EXB_LancelotTartare.checkTimesNB6BushouLife();

	// ���ȉ����̏���
	for (int i=0; i<sizeof(EXB_TempleCommand)/sizeof(EXB_TempleCommand[0]); i++) {
		EXB_TempleCommand[i].checkTimesNB6BushouLife();
	}
	for (int i=0; i<sizeof(EXB_ZenobiaMember)/sizeof(EXB_ZenobiaMember[0]); i++) {
		EXB_ZenobiaMember[i].checkTimesNB6BushouLife();
	}
	// ���ȏ㉼�̏���
}




void EXBushou_All_CheckRegeneration() {

	// �����ɗ��邽�тɁA���m�̃��W�F�l�`�F�b�N
	EXB_LancelotTartare.checkRegenerationNB6BushouSoldier(3000);

	// ���ȉ����̏���
	for (int i=0; i<sizeof(EXB_TempleCommand)/sizeof(EXB_TempleCommand[0]); i++) {
		EXB_TempleCommand[i].checkRegenerationNB6BushouSoldier(20000);
	}
	// ���ȏ㉼�̏���

	// �����X���b�g�^���^���X�͑�C�����Ă��⋋������
	EXB_LancelotTartare.checkRestoreNB6BushouTaihou();
}



void EXBushou_All_Execute() {
	
	if ( TSModCommand::UltraBushou::cmdExists != 0) {

		// ���O�̓o�^
		EXBushou_All_SetEXBushouInit();

		// ���W�F�l�`�F�b�N
		EXBushou_All_CheckRegeneration();
	}
}


// ���ꕐ���p�̃p�����^���f
void SetExBushouAvgParam(int iSigBushouID) {
	// iBushouID == -1 �Ȃ�S�������`�F�b�N

	if ( iSigBushouID == -1 ) {

		for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {
			// �E�H�[�����Ȃ�΁c��𓾈ӂ�
			if ( strcmp(nb6bushouname[iBushouID].familyname, "�q�r")==0 && strcmp(nb6bushouname[iBushouID].fastname, "�n�o�p")==0 ) {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetue = 5; // �񂾂��r��

				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeono = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudemuchi = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetuchi = 0;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeken = 0;

			// �x���T���A�c��𓾈ӂ�
			} else if ( strcmp(nb6bushouname[iBushouID].familyname, "�^�_�`")==0 && strcmp(nb6bushouname[iBushouID].fastname, "�a�b�c")==0 ) {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetue = 4; // �񂾂��`��

				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = 3; // �|��������
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeono = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudemuchi = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetuchi = 0;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeken = 0;

			// �f�l�u�Ȃ�΁c��𓾈ӂ�
			} else if ( strcmp(nb6bushouname[iBushouID].familyname, "�j�k")==0 && strcmp(nb6bushouname[iBushouID].fastname, "�l�m")==0 ) {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetue = 5; // �񂾂��r��

				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeono = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudemuchi = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetuchi = 0;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeken = 0;

			// ���h�����Ȃ�΁c��𓾈ӂ�
			} else if ( strcmp(nb6bushouname[iBushouID].familyname, "�y�z�{")==0 && strcmp(nb6bushouname[iBushouID].fastname, "�|�}�~")==0 ) {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetue = 5; // �񂾂��r��

				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeono = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudemuchi = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetuchi = 0;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeken = 0;

			// �M���o���h�Ȃ�΁c�ڂ𓾈ӂ�
			} else if ( strcmp(nb6bushouname[iBushouID].familyname, "�g�h�i")==0 && strcmp(nb6bushouname[iBushouID].fastname, "���")==0 ) {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudemuchi = 5;

				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeono = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetuchi = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetue = 0;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeken = 1;

			// �J�m�[�v�X�Ȃ�΁c���Ƒ��𓾈ӂ�
			} else if ( strcmp(nb6bushouname[iBushouID].familyname, "�O�u")==0 && strcmp(nb6bushouname[iBushouID].fastname, "�[�\�]")==0 ) {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari = 4; // ���͂`
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeono = 4; // �����`

				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudemuchi = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetuchi = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetue = 0; 
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeken = 2;

			// �^���^���X�Ȃ�΁c���𓾈ӂ�
			} else if ( isLancelotTartare(iBushouID) ) {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeken = 5; // ���͂r ���͕��ʂ͐ݒ肵�Ȃ��B

				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = 2; 
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeono = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudemuchi = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetuchi = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetue = 0; 

			// �o�[���[�t�H���Ȃ�΁c���𓾈ӂ�
			} else if ( strcmp(nb6bushouname[iBushouID].familyname, "�I�J�K")==0 && strcmp(nb6bushouname[iBushouID].fastname, "�L�M�N")==0 ) {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari = 5; // ���͂r

				nb6savedata_ex.bushou_ex[iBushouID].aptitudeken = 3;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeono = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudemuchi = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetuchi = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetue = 0; 

			// �o���o�X�Ȃ�΁c�n���}�[(��)�𓾈ӂ�
			} else if ( strcmp(nb6bushouname[iBushouID].familyname, "�U�V�W")==0 && strcmp(nb6bushouname[iBushouID].fastname, "�X�Y�Z")==0 ) {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetuchi = 5; // �Ƃ͂r
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeono = 4; // ���͂`

				nb6savedata_ex.bushou_ex[iBushouID].aptitudeken = 3;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudemuchi = 0;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetue = 0; 

			// �I�Y�}�E���[�E�O���V���X�Ȃ�΁c�ڂ����ӂ�
			} else if (	strcmp( nb6bushouname[iBushouID].familyname, "�v�w�x") == 0 && strcmp( nb6bushouname[iBushouID].fastname, "�y�z�{") == 0 ) {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudemuchi = 5;

				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = 3; // �|��������
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeken = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeono = 0;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetuchi = 0;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetue = 0;

			// ��ɏG���Ȃ�΁c�|�����ӂ�
			} else if (isShuiHidechiyo(iBushouID)) {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeken = 1; 

				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari = 3; // ���͂a
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = 5; // �|�͂r
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeono = 0;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudemuchi = 0;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetuchi = 0;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetue = 2; // ��͂b

			}
		}
	}
}
