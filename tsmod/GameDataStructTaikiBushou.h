#pragma once

/*
struct �ҋ@�������P�^
{
byte �o��N;				/// <summary>�o��N�B�l�Ƃ��ẮA1454(�V�ċL��N)����̍����̒l�B
WORD ��������y��ԍ��z;	/// <summary>����</summary>
WORD �����喼�y�喼�ԍ��z;	/// <summary>�喼</summary>
} �~�l��
*/
// �ҋ@�����̓o��^(ScenarioMod�Ƌ���)
struct NB6TAIKIBUSHOUAPPEAR {
	byte appear;			/// <summary>�o��N�B�l�Ƃ��ẮA1454(�V�ċL��N)����̍����̒l�B
	WORD castle;		//����  ��������y��ԍ��z;	//����
	WORD attach;		//�喼  �����喼�y�喼�ԍ��z;	//�喼
};

struct NB6TAIKIBUSHOU : NB6BUSHOU {
	// �ǉ��t�B�[���h�Ȃ��A�S������
};


// �ҋ@�����ːЏ��^(ScenarioMod�Ƌ���)
struct NB6TAIKIBUSHOUNAME
{
	char familyname[7];	//����
	char fastname[7];	//���O
	WORD number;		//�����i���o�[(����12bit�͗�`�H)
	byte State : 4;		//0�喼,1�R��,2����,3�B��,4�Q�l,5�P,6,�ҋ@,7���S
	byte position : 4;	//�g��		(�B��=0, �喼=1, �h�V=2, �ƘV=3, ����=4, ���叫=5, ���y��=6)
	byte sex : 1;			//�� 0-�j 1-��
};



// ���ݐi�s�`�̃V�i���I�̑ҋ@�����̓o����𓾂�
extern NB6TAIKIBUSHOUAPPEAR* GetTaikiBushouAppearArrayPointer();


// ���ݐi�s�`�̃V�i���I�̑ҋ@�����̌ːЏ��𓾂�
extern NB6TAIKIBUSHOUNAME* GetTaikiBushouNameArrayPointer();


// ���ݎ��s���̃Q�[���őҋ@����u�o��v�ւƐ؂�ւ����������
extern int getTouzyouBushouNum();

// ���ݎ��s���̃Q�[��(=�V�i���I)�́u�ҋ@�����v�̃f�[�^���B
// ���̐��̓V�i���I���ƂɌ��܂����Œ萔�B��x�Q�[�����n�܂�ƁA�r���ő������Ȃ��B
extern int getTaikiBushouDataNum();

