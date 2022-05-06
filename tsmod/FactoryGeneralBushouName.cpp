#define _CRT_SECURE_NO_WARNINGS

#include "GameDataStruct.h"

// �Ƃ��镶���񂪑S���V�t�g�i�h�r�ʏ튿�����ǂ���
int isAllKanji (char *fullname) {
	// �S�p�P�����ȏ�
	if ( strlen(fullname) >= 2 ) {
		// �Q�o�C�g�ɂP���̂P�����B
		for ( int i=0; i<(int)strlen(fullname)/2; i++) {
			if ( 0x88 <= fullname[i*2] && fullname[i*2] <= 0x9F || 0xE0 <= fullname[i*2] && fullname[i*2] <= 0xEE ) {
			} else {
				// �ЂƂł��������false
				return false;
			}
		}
		return true;
	}
	return false;
}

// ���O�Ɩ������`1300�����Ɏ�����������B�����p�Ɩ��O�p�̂Q�̃o�b�t�@�[��n�����ƁB
void getFactoryGeneralName(char *bufFamilyName, char *bufFirstName) { // �x�[�X�ƂȂ镐��ID�B-1�Ȃ�x�[�X�Ȃ��B

	string familyname;
	string firstname;

	vector<string> name1; // �����̂P�����ڂ̔z��
	vector<string> name2; // �����̂Q�����ڂ̔z��
	vector<string> name3; // ���O�̂P�����ڂ̔z��
	vector<string> name4; // ���O�̂Q�����ڂ̔z��

	// ��`�̖��O��T���B
	for ( int iRetsudenID = 0; iRetsudenID < GAMEDATASTRUCT_REGISTED_BUSHOU_NUM; iRetsudenID++ ) {
		if ( nb6bushouretsuden[iRetsudenID].name.size() == 8 ) { // ��{�I�ɖ����{���O���S�p�S�����̕����������Ă���B
			// �t���l�[��
			char *fullname = (char *)nb6bushouretsuden[iRetsudenID].name.c_str();
			// �S���ʏ튿���ł����(�����΍�)
			//if ( isAllKanji(fullname) ) {
				char zen[3] = "";

				strncpy( zen, fullname, 2); // �ŏ��̑S�p�P�������R�s�[
				zen[2] = NULL;
				name1.push_back(string(zen));

				strncpy( zen, fullname+2, 2); // �Q�������R�s�[
				zen[2] = NULL;
				name2.push_back(string(zen));

				strncpy( zen, fullname+4, 2); // �R�������R�s�[
				zen[2] = NULL;
				name3.push_back(string(zen));

				strncpy( zen, fullname+6, 2); // �S�������R�s�[
				zen[2] = NULL;
				name4.push_back(string(zen));
			//}
		}
	}

	// ���߂����O���烉���_���Ŗ����Ɩ��O�𐶐�����
	// �����̂Q�������쐬
	if ( name1.size() > 0 && name2.size() > 0 && name3.size() > 0 && name4.size() > 0 ) {
		int i = rand() % name1.size();
		familyname = name1[i];
		i = rand() % name2.size();
		// �����P�����ڂƓ����Ȃ�����؂�T�C�R���U�蒼��(�ɈɂƂ��ɂȂ�Ȃ��悤��)
		if ( familyname == name2[i] ) {
			i = rand() % name2.size();
		}
		familyname += name2[i];

		// ���O�̂Q�������쐬
		i = rand() % name3.size();
		firstname = name3[i];
		i = rand() % name4.size();
		// �����P�����ڂƓ����Ȃ�����؂�T�C�R���U�蒼��(�M�M�Ƃ��ɂȂ�Ȃ��悤��)
		if ( firstname == name4[i] ) {
			i = rand() % name4.size();
		}
		firstname += name4[i];
	}

	strcpy( bufFamilyName, familyname.c_str() );
	strcpy( bufFirstName, firstname.c_str() );
}


// �Q�[���Ō��X�p�ӂ���Ă��郉���_���ȕP���𓾂�B
// 
struct _HIME_NAME {
	char name[7];
};
char *getRandomHimeName() {
	// �P�|�C���^�̈ʒu
	_HIME_NAME *pHName = (_HIME_NAME *)GAMEDATASTRUCT_HIMENAME_ARRAY;
	int iRnd = rand() % 100; // 100�l�p�ӂ���Ă���B
	return pHName[iRnd].name;
}