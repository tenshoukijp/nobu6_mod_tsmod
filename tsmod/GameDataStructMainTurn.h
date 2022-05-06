#pragma once

////////////////////////////////////////////////////////////////////////////
// NB6 �V�ċL
/*
[�N�����(3*1)]

0-1		�N��(1454����̌o�ߐ�)
2		�G��(0=�t,1=��,2=�H,3=�~)
*/
struct NB6YEAR
{
	WORD year;			// �N��(1454����̌o�ߐ�)	 �� �������ڂł͂Ȃ��AgetYearAD�œ��邱��
	char season;		// �G��(0=�t,1=��,2=�H,3=�~) �� �������ڂł͂Ȃ�getSeasonName�œ��邱��
};

// 1454�N����̍����ƂȂ��Ă���N�𐼗�ɒ���
int getYearAD();

// �Q�[���̊J�n�N�B�f�t�H���g��1454�N
int getStartYearAD();

// �N�E���E����茳��������𓾂�B
// �N�����������w�肵�Ȃ������ꍇ�A�Q�[�����̌��݂̐���N�̔N�����w�肵�����̂Ƃ݂Ȃ��B
// �N�����������Ƃ��ēn�����ꍇ�A�w��N�̔N�����w�肵�����̂Ƃ݂Ȃ��B
// �N�E�������������Ƃ��Ďw�肵���ꍇ�A�Y������31�����w�肵�����̂Ƃ݂Ȃ��B(���̂悤�ȓ��t�����݂��Ȃ��Ƃ��s�s���͂Ȃ�)
//   (�V�ċL�ł́A�u�N�̓r���Ō������ς���Ă���v�ꍇ�A���N���ł͂Ȃ��A�V�����̕�����ɍ̗p���邽�߁A100���V�N�����Ԃ�N���x�[�X�ƂȂ��Ă���B)
struct NB6WAREKI {
	char gengou[9]; // �N��
	char kanjiyear[15]; // �a��̔N(����)
	int iyear;	// �a��̔N
};
NB6WAREKI getWarekiFromYMD(int iYear = -1, int iMonth = -1, int iDay = -1);

// �G�ߔԍ�����G�ߖ��𓾂�
char* getSeasonName();

extern int nb6progressseason;

// �I�������V�i���I�̈�ԍ�(�ォ��0�ԁA��ԉ���5��)
extern int iSelectingScenarioLocation;

// ���݃v���C���̃V�i���I���P�[�V�����ԍ�(0�`5, �s���ȏꍇ��-1);
int getCurScenarioLocation();

// �Ώۂh�c�̃V�i���I�̃^�C�g�����𓾂�
char* getScenarioTitle(int iLocationID);

/*
[ ���݂̃^�[���̏��]

W �喼
W �R�c
W ��
*/
struct NB6TURN
{
	WORD daimyo;			// ���݂̃^�[���̑喼�ԍ�
	WORD gundan;			// ���݂̃^�[���̌R�c�ԍ�
	WORD castle;			// ���݂̃^�[���̏�ԍ�
};


/*
B �đ���
B �S�C����
B �n����
B ������
*/
struct NB6SOBA
{
	byte rise;			// �đ���  ����@�\������Ă��鑊��̏\�{�̐��l �P�̎��A����0.1�ƂȂ�B
	byte gun;			// �S�C���� ��� ��ʏ�ł��l�̒ʂ�B
	byte horse;			// �n���� ��� ��ʏ�ł��l�̒ʂ�B
	byte soldier;		// ������ ��� ��ʏ�ł��l�̒ʂ�B
};

/*
�v���C���[�̒S���R�c(�喼�ԍ��ł͂Ȃ��R�c�ԍ�)
*/
struct NB6PLAYERS8
{
	WORD gundan;
};

// �Ώۂ̌R�cID�������ꂩ�̃v���C���[���͉��̌R�c�ł���(�ő�W�l)
bool isPlayerGundan(WORD iTargetGundanID);

// �Ώۂ̕���ID�������ꂩ�̃v���C���[���͉��̕����ł���
bool isPlayerBushou(WORD iTargetBushouID);

// �Ώۂ̑喼ID�������ꂩ�̃v���C���[�S���̑喼�ł���
bool isPlayerDaimyo(WORD iTargetDaimyoID);
