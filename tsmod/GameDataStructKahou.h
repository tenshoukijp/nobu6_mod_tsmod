#pragma once

/*
[�ƕ�(19*120)]

0-12	���O
13	����
0 ���q     1 ����     2 ����    3 ����        4 �ԓ�    5 ����      6 ��        7 �   8 �n
9 ��       A �w�H�D   B ���q�{  C �����Ԗ{    D ����    E �n���V    F ��؎��v 10 ���n��
11 �\����  12 ����    13 �ʎ蔠 14 �r�[�h���t 15 ���ዾ 16 ���\���� 17 ����
14	�摜
15-16	��������
17	0000AAAA A = �㏸�l
18	AB000000 A = ����  B = ���o��
*/
struct NB6KAHOU
{
	char name[13];	//���O
	byte type;		//����
	byte face;		//�摜
	WORD attach;	//���L��  ���l�Ȃ�0xFFFF;
	byte param : 4;	//�㏸�l
	byte _undef0 : 4;	//0
	byte _undef1 : 6;	//
	byte bMitojo : 1;	//���o�� (���o��=1, �o��=0)
	byte bLost : 1;	//����(1=���� 0=����) 
};


// ���ꂼ��̉ƕ󕪗ޖ��ւ̃|�C���^�̔z��B24��Ԃ���P�P�����o�����߂ɕK�v�ƂȂ�A�h���X�錾 
int GAMEDATASTRUCT_KAHOUTYPELIST_POINTER[];


// �ƕ�^�C�v��ID���當����𓾂�
char* getKahouTypeName(byte type);


// �ƕ��ނ̃J�X�^��������
struct CUSTOM_KAHOU_TYPE_STRING {
	char szKahouTypeString[11]; // �ő�S�p�T�����B�ƕ��ޖ�
};


// �ƕ�𕐏��ɃA�^�b�`����B���X�ʕ����������Ă����ꍇ�́A���̕�������w�肵�������ւƃA�^�b�`���ς��B
// �\�͒l�̕ύX�𔺂��B���l�ɕԏシ��ꍇ��0xFFFF�Ƃ���B
bool ReAttachKahou(int iKahouID, int iNewBushouID);


/*
�ƕ��`�֘A�̏��
*/
struct NB6KAHOURETSUDEN {
	string name;
	string yomigana;	// ��݂���
	string retsuden;	// ��`������
};

