#define _CRT_SECURE_NO_WARNINGS

#include "GameDataStruct.h"
#include "ScenarioMod.h"


bool isEnteringFromLoadDialogExecute = FALSE; // ���[�h��ʂ��烁�C���Q�[���ɂ�����

SAVEDATA_EX nb6savedata_ex = {};
SAVEDATA_EX_COMMON nb6savedata_ex_common = {}; // �S�Z�[�u�f�[�^���ʕ�


void SetOnSaveOfSaveDataEx() {
	/*
	 * �g���������Z�[�u����O�ɂ����ŁA�f�[�^�Z�b�e�B���O����B
	 * �l�̃R�s�[����H���K�v�Ȃ��
	 */
}


bool SaveSaveDataEx(int iSelectID ) {

	FILE *fp = fopen( psznb6savedatafilename, "rb+" );
	long sz;

	if( fp == NULL ){
		// "%s�t�@�C�����J���܂���"
		return false;

	} else {

		// �Z�[�u�O�̃f�[�^�ւ̃Z�b�e�B���O
		SetOnSaveOfSaveDataEx();

		fseek( fp, 0, SEEK_END ); // �Ō�̈ʒu��(�T�C�Y�擾�̂��߂���)
		sz = ftell( fp );		  // �T�C�Y���擾����B

		fseek( fp, 0, SEEK_SET ); // �ŏ��̈ʒu�ɖ߂��B

		// ���łɃZ�[�u�f�[�^�͊g������Ă���B
		if ( sz >= GAMEDATASTRUCT_NORMAL_SAVEDATAFILE_SIZE+sizeof(SAVEDATA_EX)*8 ) {

			int seek_pos = GAMEDATASTRUCT_NORMAL_SAVEDATAFILE_SIZE+(iSelectID)*sizeof(SAVEDATA_EX); // �Z�[�u�f�[�^�̑I���ʒu����t�@�C���V�[�N�ʒu�����߂�

			fseek( fp, seek_pos, SEEK_SET );

			// 1��őS����������
			fwrite( (void *)&nb6savedata_ex, sizeof(SAVEDATA_EX), 1, fp);

			// ---�Ō�ɋ��ʕ�
			seek_pos = GAMEDATASTRUCT_NORMAL_SAVEDATAFILE_SIZE+(GAMEDATASTRUCT_SAVEDATA_SLOT_NUM)*sizeof(SAVEDATA_EX); // �Z�[�u�f�[�^�̑I���ʒu����t�@�C���V�[�N�ʒu�����߂�
			fseek( fp, seek_pos, SEEK_SET );

			// 1��őS����������
			fwrite( (void *)&nb6savedata_ex_common, sizeof(SAVEDATA_EX_COMMON), 1, fp);

		// �f�t�H���g�̂܂�
		} else {

			for ( int i=0; i < GAMEDATASTRUCT_SAVEDATA_SLOT_NUM; i++ ) {
				// �I��Ώۂ̃Z�[�u�f�[�^�Ŗ������́A������SAVEDATA_EX���o�́B
				if ( i != iSelectID ) {
					SAVEDATA_EX dummy_savedata_ex = {};
					int seek_pos = GAMEDATASTRUCT_NORMAL_SAVEDATAFILE_SIZE+(i)*sizeof(SAVEDATA_EX); // �Z�[�u�f�[�^�̑I���ʒu����t�@�C���V�[�N�ʒu�����߂�
					fseek( fp, 0, SEEK_END );

					// 1��őS����������
					int a = fwrite( (void *)&dummy_savedata_ex, sizeof(SAVEDATA_EX), 1, fp);

				// �I��Ώۂ����́Anb6savedata_ex�̃f�[�^���o�͂���B
				} else if ( i==iSelectID ) {
					int seek_pos = GAMEDATASTRUCT_NORMAL_SAVEDATAFILE_SIZE+(i)*sizeof(SAVEDATA_EX); // �Z�[�u�f�[�^�̑I���ʒu����t�@�C���V�[�N�ʒu�����߂�

					fseek( fp, 0, SEEK_END );

					// 1��őS����������
					int a = fwrite( (void *)&nb6savedata_ex, sizeof(SAVEDATA_EX), 1, fp);

				}
			}

			// ---�Ō�ɋ��ʕ�
			fseek( fp, 0, SEEK_END );
			// 1��őS����������
			fwrite( (void *)&nb6savedata_ex_common, sizeof(SAVEDATA_EX_COMMON), 1, fp);
		}


		fclose( fp );

		// -------------------------------------
		// ScenarioMod���s
		// -------------------------------------
		if (p_snOnSaveSaveData) {
			p_snOnSaveSaveData(iSelectID, psznb6savedatafilename);
		}

		return true;
	}

	return false;
}


void SetOnLoadOfSaveDataEx() {
	/*
	 * �g���������Z�[�u����O�ɂ����ŁA�f�[�^�Z�b�e�B���O����B
	 * �l�̃R�s�[����H���K�v�Ȃ��
	 */
}

bool LoadSaveDataEx(int iSelectID ) {

	FILE *fp = fopen( psznb6savedatafilename, "rb" );
	long sz;

	if( fp == NULL ){
		// %s�t�@�C�����J���܂���\n"
		return false;

	} else {

		fseek( fp, 0, SEEK_END ); // �Ō�̈ʒu��(�T�C�Y�擾�̂��߂���)
		sz = ftell( fp );		  // �T�C�Y���擾����B

		fseek( fp, 0, SEEK_SET ); // �ŏ��̈ʒu�ɖ߂��B

		// ���łɃZ�[�u�f�[�^�͊g������Ă���B
		if ( sz >= GAMEDATASTRUCT_NORMAL_SAVEDATAFILE_SIZE+sizeof(SAVEDATA_EX)*8 ) {

			int seek_pos = GAMEDATASTRUCT_NORMAL_SAVEDATAFILE_SIZE+(iSelectID)*sizeof(SAVEDATA_EX); // �Z�[�u�f�[�^�̑I���ʒu����t�@�C���V�[�N�ʒu�����߂�

			fseek( fp, seek_pos, SEEK_SET );

			// 1��őS����������
			fread( (void *)&nb6savedata_ex, sizeof(SAVEDATA_EX), 1, fp);

			// ---�Ō�ɋ��ʕ�
			seek_pos = GAMEDATASTRUCT_NORMAL_SAVEDATAFILE_SIZE+(GAMEDATASTRUCT_SAVEDATA_SLOT_NUM)*sizeof(SAVEDATA_EX); // �Z�[�u�f�[�^�̑I���ʒu����t�@�C���V�[�N�ʒu�����߂�
			fseek( fp, seek_pos, SEEK_SET );
			// 1��őS����������
			fread( (void *)&nb6savedata_ex_common, sizeof(SAVEDATA_EX_COMMON), 1, fp);

			// ���[�h��̃f�[�^�ւ̃Z�b�e�B���O
			SetOnLoadOfSaveDataEx();


		}


		fclose( fp );

		// -------------------------------------
		// ScenarioMod���s
		// -------------------------------------
		if (p_snOnLoadSaveData) {
			p_snOnLoadSaveData(iSelectID, psznb6savedatafilename);
		}

		// ���[�h���烁�C���Q�[���ɍs����Ƃ����t���O�𗧂Ă�B
		isEnteringFromLoadDialogExecute = TRUE;

		return true;
	}

	return false;
}