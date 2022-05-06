#define _CRT_SECURE_NO_WARNINGS

#include "GameDataStruct.h"


void SetMapFlagTarget(int iFlagID, int iValue, int iFlagSlot) {
	int i = iFlagID;
	switch (iFlagSlot) {
		case 0: 
			{
				nb6savedata_ex.mapflag_ex[i].value = iValue;
				break;
			}
		case 1: 
			{
				nb6savedata_ex.mapflag_ex[i].flag1 = iValue;
				break;
			}
		case 2: 
			{
				nb6savedata_ex.mapflag_ex[i].flag2 = iValue;
				break;
			}
		case 3: 
			{
				nb6savedata_ex.mapflag_ex[i].flag3 = iValue;
				break;
			}
		case 4: 
			{
				nb6savedata_ex.mapflag_ex[i].flag4 = iValue;
				break;
			}
		case 5: 
			{
				nb6savedata_ex.mapflag_ex[i].flag5 = iValue;
				break;
			}
		case 6: 
			{
				nb6savedata_ex.mapflag_ex[i].flag6 = iValue;
				break;
			}
		case 7: 
			{
				nb6savedata_ex.mapflag_ex[i].flag7 = iValue;
				break;
			}
		case 8: 
			{
				nb6savedata_ex.mapflag_ex[i].flag8 = iValue;
				break;
			}
	}
}


int WINAPI Extern_SetMapFlag( char *szFlagName, int iFlagValue, int iFlagSlot ) {
	// ���łɑ��݂���t���O�̒��ŒT���B
	for ( int i=0; i < GAMEDATASTRUCT_MAPFLAG_NUM; i++ ) {
		// �����[���͂�����������
		if ( strlen( nb6savedata_ex.mapflag_ex[i].name ) == 0 ) {
			continue;
		}

		// ���̃��x�������݂����B
		if ( strncmp( szFlagName, nb6savedata_ex.mapflag_ex[i].name, 8 )==0 ) { // �W�����݂̂Ŕ���
			// �t���O��ݒ�
			SetMapFlagTarget( i, iFlagValue, iFlagSlot );

			return TRUE;
		}
	}

	// �t���O�����݂��Ȃ��̂ŁA�V�����L�ڂ���B
	for ( int i=0; i < GAMEDATASTRUCT_MAPFLAG_NUM; i++ ) {
		// ��̕�����������B
		if ( strlen( nb6savedata_ex.mapflag_ex[i].name ) == 0 ) {

			strncpy( nb6savedata_ex.mapflag_ex[i].name, szFlagName, 8 ); // �W�����܂ŃR�s�[
			nb6savedata_ex.mapflag_ex[i].name[8] = NULL; // �X�����ڂɔԕ��z�u�B

			// �t���O��ݒ�
			SetMapFlagTarget( i, iFlagValue, iFlagSlot );

			// �����܂ŗ�����ݒ�ł����̂�true��Ԃ��B
			return TRUE;
		}
	}

	// �����܂ŗ����Ƃ������Ƃ͋󂢂Ă�t���O�������B
	return FALSE;
}



int GetMapFlagTarget(int iFlagID, int iFlagSlot) {
	int i = iFlagID;
	switch (iFlagSlot) {
		case 0: 
			{
				return nb6savedata_ex.mapflag_ex[i].value;
				break;
			}
		case 1: 
			{
				return nb6savedata_ex.mapflag_ex[i].flag1;
				break;
			}
		case 2: 
			{
				return nb6savedata_ex.mapflag_ex[i].flag2;
				break;
			}
		case 3: 
			{
				return nb6savedata_ex.mapflag_ex[i].flag3;
				break;
			}
		case 4: 
			{
				return nb6savedata_ex.mapflag_ex[i].flag4;
				break;
			}
		case 5: 
			{
				return nb6savedata_ex.mapflag_ex[i].flag5;
				break;
			}
		case 6: 
			{
				return nb6savedata_ex.mapflag_ex[i].flag6;
				break;
			}
		case 7: 
			{
				return nb6savedata_ex.mapflag_ex[i].flag7;
				break;
			}
		case 8: 
			{
				return nb6savedata_ex.mapflag_ex[i].flag8;
				break;
			}
	}
	return FALSE;
}


int WINAPI Extern_GetMapFlag( char *szFlagName, int iFlagSlot ) {
	// ���łɑ��݂���t���O�̒��ŒT���B
	for ( int i=0; i < GAMEDATASTRUCT_MAPFLAG_NUM; i++ ) {
		// �����[���͂�����������
		if ( strlen( nb6savedata_ex.mapflag_ex[i].name ) == 0 ) {
			continue;
		}

		// ���̃��x�������݂����B
		if ( strncmp( szFlagName, nb6savedata_ex.mapflag_ex[i].name, 8 )==0 ) { // �W�����݂̂Ŕ���
			// �t���O��ݒ�
			return GetMapFlagTarget( i, iFlagSlot );
		}
	}

	// �����܂ŗ����Ƃ������Ƃ͊Y���̃��G���̃t���O�������B
	return FALSE;
}


int WINAPI Extern_DeleteMapFlag( char *szFlagName ) {

	// ���łɑ��݂���t���O�̒��ŒT���B
	for ( int i=0; i < GAMEDATASTRUCT_MAPFLAG_NUM; i++ ) {
		// �����[���͂�����������
		if ( strlen( nb6savedata_ex.mapflag_ex[i].name ) == 0 ) {
			continue;
		}

		// ���̃��x�������݂����B
		if ( strncmp( szFlagName, nb6savedata_ex.mapflag_ex[i].name, 8 )==0 ) { // �W�����݂̂Ŕ���
			// ���g�S������
			memset( (void *)&nb6savedata_ex.mapflag_ex[i], 0, sizeof(nb6savedata_ex.mapflag_ex[i]) );

			return TRUE;
		}
	}

	// �����܂ŗ����Ƃ������Ƃ͊Y���̃��G���̃t���O�������B
	return FALSE;
}