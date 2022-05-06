#pragma once

/*
�y�����̃A���S���Y���z:
0�c�����ƌĂ΂ꂽ�l���B�L�������ɑ����B�M���G�g�ƍN�ق��A�֓����O�⏼�i�v�G���B
�����I�ȃo�����X�̗ǂ��^�C�v�����Acom�̏ꍇ���͂΂��葝�₵�A���܂�푈���Ȃ��B
1�c�ҏ��ɑ����B�ēc���Ƃ�`��i�Ƃ�암������B
2�c�퓬�ɋ��������������B�����ȂǁB
3�c�퓬�����B���쎁�^��B�����D���H
4�c�����]�ᓙ�A�V��E���l���̕����l�n�����B�����D���H
5�c�E�ҕ�����A�v���e���E���n�@���`�ȂǁB�����D���B
6�c�ڗ����Ȃ��U�R�����B���\�^�C�v�H
7�c�I���W�i���P�B���i�͕s���B
255�c�����_���P�B���i�͕s���B
*/

/*
�y�����z:
0 ����			 217 �^�c�K���E���@�䕔���e�E���Ë`�v�E�E�ґS��
1 ��V������	 152 �O�c���ƁE���q���G�E���ԏ@�΁E�������
2 �e�\			 119 �ēc���ƁE���������M�E���ˑ��E���X�؏����Y
3 ���M			 30 �㐙�����E����`���E�����`���E�������
4 ���ӋC		 180 �ɒB���@�E�O�D���c�E��؏d�G�E�R�䐳��
5 �Ќ�			 143 �D�c�M���E����ƍN�E���c�M���E�㐙���M
6 �`��			 145 �L�b�G�g�E�֓����O�E���i�v�G�E�����`��
7 ���			 314 �{�������E�g�쌳�t�E�R�����V���E�j���P�S��
*/

/*
[�����f�[�^(47*532)]

0-1	�O�̕���
2-3 ���̕���
4-5	�H
6	����
7	���
8	�q��
9-10	���n   ( min 500 �` max 2000 )
11-12	��n   ( min 500 �` max 2000 )
13-14	�q�n   ( min 500 �` max 2000 )
15	����
16	��]
17	����
18-19	�M��
20	���� 0xFF�͖���
21-22	�喼(�喼�ԍ�)  �����喼�y�喼�ԍ��z;	//�喼
23-24	�R�c(�R�c�ԍ�)  �����R�c�y�R�c�ԍ��z;	//�R�c
25-26	����		    ��������y��ԍ��z;	//����
27-28	������ FFFF ����
29	����
30	�P��
31	����
32	����(0=���y,1=�R�n,2=�S�C,3=�R�n�S�C,18�܂�)
33	�d��
34	�� ���Ɍ����ς݃t���O����H
35	���N
36-37	�e��NO
38	�v�l
39	aaaabbbb a-���� b-�`��
40	00aabbcc a-�q�K b-��K c-���K
41	�Z�\
42	�E�� 0x00�� 0x10 �E�� 0x20 ���l 0x30 ���� 0x40 �m�� 0x50 �C�� 0x60 ���� 0x70 ���l 0x80 ���� 0x90 �؎x�O 0xA0 �R�m 0xB0 ���p�m 0xC0 ����
43-44	���ȁE�R�n�S�C�E��C�E�S�b�D
0123 4445 5566 6777
0=���֌W�H,1=�S�b�D,2=�R�n�S�C,3=��C
4,5,6,7=���R,�S�C,�R�n,���y�̕��ȁAMAX5
45	aabbbccc a-�Ɨ� b-�E�� c-����
46	1234aaa5 1=� 2-�⍦ 3 ���� 4-�s�� 5�Ɨ��̃r�b�g�̑���
a-����
*/

struct NB6BUSHOU
{
	WORD prev;
	WORD next;
	WORD _undef2;		//must 0xFFFF
	byte maxgov;		//����
	byte maxbat;		//���
	byte maxint;		//�q��
	WORD expgov;		//���n
	WORD expbat;		//��n
	WORD expint;		//�q�n
	char charm;			//����
	char ambition;		//��]
	char loyal;			//����
	WORD merits;		//�M��
	byte blood;			//���� 0xFF�͖���
	WORD attach;		//�喼  �����喼�y�喼�ԍ��z;	//�喼
	WORD gundan;		//�R�c  �����R�c�y�R�c�ԍ��z;	//�R�c
	WORD castle;		//����  ��������y��ԍ��z;	//����
	WORD rise;			//������ 0xFFFF ������̑喼�ԍ�
	byte soldier;		//����
	byte training;		//�P��
	byte solloyal;		//����
	byte form;			//����(0=���y,1=�R�n,2=�S�C,3=�R�n�S�C,18�܂�)
	byte work;			//�d���N��
	byte spouse;		//�� (�����z��ԍ���500�Ԉȏオ�Ȃ�P�ƂȂ�����ł���B���肪500�Ԃ̎��ɁA������0�Ƃ������l������B0��0+500�ˋA�� �Ƃ�������B
	//�� ���̔ԍ���255�Ȃ�Ȃ͂��Ȃ��B255�ȊO�̎�(�Ⴆ��3)�́A���̔ԍ���500�������y�����ԍ��z(�Ⴆ��503)�̏������ȂƂ�������)
	// �����ĊY���̕���(�ȕP����)��_�Ȃ�����(�Ⴆ��3)�̒l�ɂȂ��Ă��邱�ƁB
	byte birth;			//���N - 1454
	WORD parent;		//�e��NO
	byte algo;			//�v�l
	byte duty : 4;		//�`��
	byte affinity : 4;	//����
	//00aa bbcc a-�q�K b-��K c-���K
	byte aptitudegov : 2;	//c-���K (2=A,1=B,0=C)
	byte aptitudebat : 2;	//b-��K (2=A,1=B,0=C)
	byte aptitudeint : 2;	//a-�q�K (2=A,1=B,0=C)
	byte undef_5 : 2;		//
	byte tech_ikkatsu : 1;	//�Z�\�E�ꊅ
	byte tech_ansatsu : 1;	//�Z�\�E�ÎE
	byte tech_ryusyutsu : 1;	//�Z�\�E���o
	byte tech_sendou : 1;		//�Z�\�E����
	byte tech_ryugen : 1;		//�Z�\�E����
	byte tech_benzetsu : 1;	//�Z�\�E�ِ�
	byte tech_yakiuchi : 1;	//�Z�\�E�ē�
	byte tech_chohatsu : 1;	//�Z�\�E����
	byte job;			//�E�� 0x00 �� 0x10 �E�� 0x20 ���l 0x30 ���� 0x40 �m�� 0x50 �C�� 0x60 ���� 0x70 ���l 0x80 ���� 0x90 �؎x�O 0xA0 �R�m 0xB0 ���p�m 0xC0 ����

	byte aptitudeasi : 3;		//���y�K��
	byte aptitudekib : 3;		//�R�n�K��
	byte _aptitudegun_hi : 2; //�S�C�K����ʃr�b�g
	byte _aptitudegun_lo : 1; //�S�C�K�����ʃr�b�g
	byte aptitudesui : 3;		//���R�K��

	byte biggun : 1;		//��C
	byte horsegun : 1;	//�R�n�S�C
	byte steelship : 1;	//�S�b�D
	byte _undef : 1;		//0
	byte life : 3;		//����
	byte brave : 3;		//�E��
	byte _independence_hi : 2; //�Ɨ���ʃr�b�g
	byte _independence_lo : 1; //�Ɨ����ʃr�b�g
	byte tone : 3;		//����
	byte bEnd : 1;		//�s��
	byte bRise : 1;		//����
	byte grudge : 1;		//�⍦
	byte kaiken : 1;		//�
};


struct NB6BUSHOUCOPY
{
	byte soldier;		//����
};


// �����̔N��𓾂�
int getYearsOld(int iBushouID);

/// <summary>
/// �Ώۂ̕����̔N���ݒ肷��B
/// �����N��͉��΂ł����R�ɐݒ�o����킯�ł͂Ȃ��A
/// �v�Z���ʂƂ��āA�u1455�N���܂��葁�����܂ꂽ���Ɓv�ɂȂ�N��͐ݒ�ł��Ȃ�
/// �v�Z���ʂƂ��āA�u1708�N���܂���x�����܂ꂽ���Ɓv�ɂȂ�N����ݒ�ł��Ȃ�
/// <para>�Ԃ�l�F�N��ݒ�ɐ�������ΐ^�E���s����΋U</para>
/// </summary>
/// <param name="�����ԍ��y�z��p�z">�ΏۂƂȂ镐���́u�z��p�v�̔ԍ�</param>
/// <param name="�N��">�V���ȔN��</param>
/// <returns>�Ԃ�l�F�N��ݒ�ɐ�������ΐ^�E���s����΋U</returns>
bool setYearsOld(int iBushouID, int iYearsOld);

enum APTITUDE { APTITUDE_ASIGARU = 1, APTITUDE_KIBA = 2, APTITUDE_TEPPOU = 3, APTITUDE_SUIGUN = 4, APTITUDE_YUMI = 5, APTITUDE_YARI = 6 };

// ���y�E�R�n�E�S�C�E���R�̓K���𐔒l�œ���B(0=E,1=D,2=C,3=B,4=A,5=S)
int getBushouAptitude(int iBushouID, APTITUDE eAptitude);

// ���y�E�R�n�E�S�C�E���R�̓K���𐔒l�œ���B(0=E,1=D,2=C,3=B,4=A,5=S)
void setBushouAptitude(int iBushouID, APTITUDE eAptitude, int iTekisei);

// �|�̓K���l��ݒ肷��(ScenarioMod������̐ݒ�) (0=E,1=D,2=C,3=B,4=A,5=S) �l�̎擾�����˂�B
int WINAPI Extern_setYumiAptitude(int iBushouID, APTITUDE eAptitude, int iTekisei);

// ���̓K���l��ݒ肷��(ScenarioMod������̐ݒ�) (0=E,1=D,2=C,3=B,4=A,5=S) �l�̎擾�����˂�B
int WINAPI Extern_setYariAptitude(int iBushouID, APTITUDE eAptitude, int iTekisei);


// �����̐퓬�l
int getBushouBat(int iBushouID);
// �����̒q�d�l
int getBushouInt(int iBushouID);
// �����̐����l
int getBushouGov(int iBushouID);

// �ƕ�Ȃǂ��܂߂Ȃ��f�̐����˔\
int getBushouPureGovMax(int iBushouID);

// �ƕ�Ȃǂ��܂߂Ȃ��f�̐퓬�˔\
int getBushouPureBatMax(int iBushouID);

// �ƕ�Ȃǂ��܂߂Ȃ��f�̒q�d�˔\
int getBushouPureIntMax(int iBushouID);

// �ƕ�Ȃǂ��܂߂Ȃ��f�̖�]
int getBushouPureAmbition(int iBushouID);

// ���ʂȂǂ��܂߂Ȃ��f�̖���
int getBushouPureCharm(int iBushouID);

// �ƕ�ɂ�鐭���̏グ��
int getBushouGovMaxUpByKahou(int iBushouID);

// �ƕ�ɂ��퓬�̏グ��
int getBushouBatMaxUpByKahou(int iBushouID);

// �ƕ�ɂ��q�d�̏グ��
int getBushouIntMaxUpByKahou(int iBushouID);

// �ƕ�ɂ���]�̏グ��
int getBushouAmbitionUpByKahou(int iBushouID);

// ���ʂɂ�閣�͂̏グ��
int getBushouCharmUpByKani(int iBushouID);


// �����̓Ɨ��l�𓾂�
int getBushouIndependence(int iBushouID);

// �����̓Ɨ��l��ݒ肷��
void setBushouIndependence(int iBushouID, int iIndependence);


// ��ID���畐��ID�����߂�B(�z��p�ł͂Ȃ���+1���ꂽ�l���Ԃ��Ă���)
int getBushouIDFromFaceID(int iFaceID);

// �Ώۂ̕���ID�����ݓS�b�D��Ԃ��ǂ���(�S�b�D�������Ă��邩�ǂ����ł͂Ȃ��A���ɂ����āA���S�b�D�̌`�󂩂ǂ���)
extern int iCurTekkousenForm[GAMEDATASTRUCT_BUSHOU_NUM];

// bfile.nb6�ɑ��݂����`�̐�
extern int iExistRetudenNumOfBFileN6P;



struct NB6KAIKENIKOU {
	byte _undef1; // ��{0xFF
	byte _undef2; // ��{0
	byte _undef3; // ��{0
	byte ai; // ��{0x24������(0x24�͐��͂`�h���p������H)�B
	// 0:�Ê� 1:�J�� 2:���� 3:�z�� 4:�{�� 5:�푈 6:���� 7:�P�� 8:�Ґ����� 9:�R�n�Ґ� 10:�S�C�Ґ�
	byte depth; // ���0, 8�B ���܂ɁA9, A
	byte _undef4; // ��{0
	byte _undef5; // ��{0
	byte _undef6; // ��{0
};

// �Ώە����̉�ӌ���AI�𓾂�
// 0:�Ê� 1:�J�� 2:���� 3:�z�� 4:�{�� 5:�푈 6:���� 7:�P�� 8:�Ґ����� 9:�R�n�Ґ� 10:�S�C�Ґ�
// 11:�O���ړ�,12:����ړ�,13:�čw��,14:�S�C�w��,15:�R�n�w��,16:�ƕ�w��,17:�Ĕ��p,18:�u�`�t��
// 19:�u�`�K��,20:�o�p,21:���i,22:�J��,23:��������,24:�������:25:�F�D�e�P,26:��؂�,27:����
// 28:����H��,29:����,30�Z�����o,31:�Ꝅ�,32:�������,33:�����ÎE,34:�Ă�����,35:����U��
extern int getKaikenIkouAI(int iBushouID);

// �Ώە����̉�ӌ��`�h��ݒ肷��B
extern void setKaikenIkouAI(int iBushouID, int iAI);


/*
�������O�e�[�u��(18*500�ƕP���p��18*32)

[18�o�C�g]
0-6		����
7-13	���O
14-15	�����i���o�[(����12bit�͗�`�H)
16	���4�r�b�g�͐g��(b0000=�B��,b0001=�喼
�@b0010=�h�V,b0011=�ƘV
 �@b0100=����,b0101=���叫
  �@b0110=���y��)
   ����4�r�b�g	0�喼,1�R��,2����,3�B��,4�Q�l,5�P,6?,7���S

   17	�� 0-�j 1-��
   */
struct NB6BUSHOUNAME
{
	char familyname[7];	//����
	char fastname[7];	//���O
	WORD number;		//�����i���o�[(����12bit�͗�`�H)
	byte State : 4;		//0�喼,1�R��,2����,3�B��,4�Q�l,5�P,6,�ҋ@,7���S
	byte position : 4;	//�g��		(�B��=0, �喼=1, �h�V=2, �ƘV=3, ����=4, ���叫=5, ���y��=6)
	byte sex : 1;			//�� 0-�j 1-��
	byte battleRetreat : 1;	//1:���őދp�����B0:�ʏ�ʂ�
	byte battleDeath : 1;	//1:���Ő펀�����B0:�ʏ�ʂ�
	byte _undef : 5;
};


// State ���� �� State �Q�l��
bool setGenekiToRounin(int iBushouID);

// State �Q�l �� State ������
bool setRouninToGeneki(int iBushouID);

// ���������̏�̏��Ƃ���B
// ����́u�����v�̕������A���̏�̏��ɂ���B
// ���̏�傪�u�R�c���v��u�喼�v���ƁA���̐l���̗���ƂȂ�B
bool setGenekiToJyosyu(int iBushouID);

// �������w���Ɉړ�����B
bool setBushouTransport(int iBushouID, int iNewCastleID);

// �w�蕐���̔�����iBushouID�𓾂�B���������Ȃ��ꍇ�ɂ́A0xFFFF���Ԃ��Ă���B
int getHanryoBushouID(int iBushouID);

// �P(���̕����̍Ȋ܂�)���A�w��̕����ƌ���������A�����֌W���͈�ؑ��삳��Ȃ��B
// withKodokuDeath�́A�N�����������ēV�W�ǓƂɂȂ����玀�S�������s������
bool setHimeMarriage(int iHimeID, int iOttoID, BOOL withKodokuDeath = true);

// �w��̕P�𗣍�������B
// withKodokuDeath�́A�N�����������ēV�W�ǓƂɂȂ����玀�S�������s������
bool setHimeDivorce(int iHimeID, BOOL withKodokuDeath = true);



// 2�l�̕����̑����̍��𓾂�
int getBushouAishoDiff(int iBushouAID, int iBushouBID);


/*
* ���o���Ȃǂɂ�����A���́E�q�́E��R�ҁv��ID
*/
struct NB6FUKIDASHIRELATEPERSONS {
	WORD i1stPersonID;	// ������ID(�ʏ핐��ID)
	WORD i2ndPersonID;	// �����ID(�ʏ핐��ID�B�C�x���g��⏤�l���531,�_�����532�ƂȂ�)
	WORD i3rdPersonID;	// ��R�҂�ID(�ʏ�1stPerson��2ndPerson�̉�b�̒��ŏo�Ă���l��ID);
	DWORD _undef1;
	DWORD _undef2;
	DWORD _undef3;
	DWORD _undef4;
	DWORD _undef5;
};


/*
������`�֘A�̏��
*/
struct NB6BUSHOURETSUDEN {
	string name;
	string yomigana;	// ��݂���
	string born;	    // �a���N������(????)�Ƃ������邽��
	string death;	    // ���S�N������(????)�Ƃ������邽��
	string retsuden;	// ��`������
};




/*
�ő啺�� [0:�B��,1:�喼,2:�h�V�`6:���y]�܂łƂ��Ċi�[���Ă䂭
*/
struct NB6SOLDIERMAX
{
	WORD soldiermax;
};

/*
�ő啺�� [0:�B��,1:�喼,2:�h�V�`6:���y]�܂łƂ��Ċi�[���Ă䂭�B���ʂ�g���Ȃǂ��ӂ܂����ό`�p�����^
*/
struct NB6SOLDIERMAX_CUSTOM
{
	WORD soldiermax;
};


/*
�g�����i�̂��߂ɕK�v�ƂȂ�M���l
*/
struct NB6REQUIREDMERITS
{
	DWORD merits;
};

