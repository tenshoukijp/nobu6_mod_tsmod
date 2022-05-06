#pragma once

#pragma pack(1)


#define GAMEDATASTRUCT_NORMAL_SAVEDATAFILE_SIZE		844623    // [�ʏ�̃Z�[�u�f�[�^�̃t�@�C���T�C�Y]
#define GAMEDATASTRUCT_SAVEDATA_SLOT_NUM			8		  // [�Z�[�u�f�[�^�̃X���b�g�g�̐�]
#define GAMEDATASTRUCT_MAPFLAG_NUM					3000	  // ���x���t���̃t���O��


struct NB6BUSHOU_SAVEDATA_EX {
	// �ȉ��R�o�C�g
	byte ansatsu_cnt;	// �Y���������P�G�߂ɂǂꂾ���ÎE���Ă��邩
	byte undef_1;	    // ����`
	byte undef_2;	    // ����`

	// �ȉ��S�o�C�g
	byte aptitudeken:4;	// ���̓K�� (0=E,1=D,2=C,3=B,4=A,5=S)
	byte aptitudeyumi:4;	// �|�̓K�� (0=E,1=D,2=C,3=B,4=A,5=S)
	byte aptitudeyari:4;	// ���̓K�� (0=E,1=D,2=C,3=B,4=A,5=S)
	byte aptitudeono:4;	// ���̓K�� (0=E,1=D,2=C,3=B,4=A,5=S)
	byte aptitudemuchi:4;	// �ڂ̓K�� (0=E,1=D,2=C,3=B,4=A,5=S)
	byte aptitudetuchi:4;	// �Ƃ̓K�� (0=E,1=D,2=C,3=B,4=A,5=S)
	byte aptitudetue:4;	// ��̓K�� (0=E,1=D,2=C,3=B,4=A,5=S)
	byte aptitude_undef1:4;	// 

	// �ȉ��S�o�C�g
	byte education;			//���{
	byte command;			//����
	byte undef_4;	    // ����`
	byte undef_5;	    // ����`

	// �ȉ��S�o�C�g
	byte aptitudenougyou:4;	// �_�Ƃ̓K�� (0=E,1=D,2=C,3=B,4=A,5=S)
	byte aptitudesyougyou:4;// ���Ƃ̓K��  (0=E,1=D,2=C,3=B,4=A,5=S)
	byte aptitudechikujo:4;	// �z��̓K�� (0=E,1=D,2=C,3=B,4=A,5=S)
	byte aptitudegaikou:4;	// �O���̓K��  (0=E,1=D,2=C,3=B,4=A,5=S)
	byte aptitudetouyou:4;	// �o�p�̓K��  (0=E,1=D,2=C,3=B,4=A,5=S)
	byte undef_6:4;	    // ����`
	byte undef_7;	    // ����`

	WORD ruiseki_exp_gov;   // �ݐϐ����n���x�B��G�߂��Ƃɔ����B
	WORD ruiseki_exp_bat;   // �ݐϐ퓬�n���x�B��G�߂��Ƃɔ����B
	WORD ruiseki_exp_int;   // �ݐϒq�d�n���x�B��G�߂��Ƃɔ����B
	WORD undef_8;

	int array[44];
};

struct NB6CASTLE_SAVEDATA_EX {
	char name[9];	//�\��
	int array[10];
};

struct NB6GUNDAN_SAVEDATA_EX {
	char name[17];	//�\��
	int array[10];
};

struct NB6DAIMYO_SAVEDATA_EX {
	char name[17];	//�\��
	int array[10];
};

struct NB6KAHOU_SAVEDATA_EX {
	char name[13];	//�\��
	int array[10];
};

struct NB6KANI_SAVEDATA_EX {
	char name[13];	//�\��
	int array[10];
};

struct NB6CHIMEI_SAVEDATA_EX {
    char name[13];	//�\��
	int array[5];
};

struct NB6FLAGS_SAVEDATA_EX {
	byte flag1 : 1;	
	byte flag2 : 1;	
	byte flag3 : 1;	
	byte flag4 : 1;	
	byte flag5 : 1;	
	byte flag6 : 1;	
	byte flag7 : 1;	
	byte flag8 : 1;	
	byte _undef : 6;
	int value : 18;	
};

struct NB6MAPFLAGS_SAVEDATA_EX {
	char name[9];   // �t���O��  
	byte flag1 : 1;	
	byte flag2 : 1;	
	byte flag3 : 1;	
	byte flag4 : 1;	
	byte flag5 : 1;	
	byte flag6 : 1;	
	byte flag7 : 1;	
	byte flag8 : 1;	
	byte _undef : 6;
	int value : 18;	// �t���O�l�B
};

// �p�r����B�\���B�����V�݂���ƁA�����̐�������Č����ăT�C�Y���K�����킹��B
struct NB6UNDEF_SAVEDATA_EX {
	int _undef[10000];
};


// �S���̊g���Z�[�u�f�[�^�̍\����
struct SAVEDATA_EX {
	NB6BUSHOU_SAVEDATA_EX bushou_ex[GAMEDATASTRUCT_BUSHOU_NUM];
	NB6CASTLE_SAVEDATA_EX castle_ex[GAMEDATASTRUCT_CASTLE_NUM];
	NB6GUNDAN_SAVEDATA_EX gundan_ex[GAMEDATASTRUCT_GUNDAN_NUM];
	NB6DAIMYO_SAVEDATA_EX daimyo_ex[GAMEDATASTRUCT_DAIMYO_NUM];
	NB6KAHOU_SAVEDATA_EX kahou_ex[GAMEDATASTRUCT_KAHOU_NUM];
	NB6KANI_SAVEDATA_EX kani_ex[GAMEDATASTRUCT_KANI_NUM];
	NB6CHIMEI_SAVEDATA_EX chimei_ex[GAMEDATASTRUCT_CHIMEI_NUM];
	NB6FLAGS_SAVEDATA_EX flag_ex[10000];
	NB6MAPFLAGS_SAVEDATA_EX mapflag_ex[GAMEDATASTRUCT_MAPFLAG_NUM];
	NB6UNDEF_SAVEDATA_EX _undef;
};

// �g���Z�[�u�f�[�^�̑S�Z�[�u�f�[�^���ʕ�
struct SAVEDATA_EX_COMMON {
	int _undef[1000];
};

extern SAVEDATA_EX nb6savedata_ex;

// �g���������Z�[�u����B
bool SaveSaveDataEx(int iSelectID );

// �g�����������[�h����B
bool LoadSaveDataEx(int iSelectID );


extern SAVEDATA_EX nb6savedata_ex;
extern SAVEDATA_EX_COMMON nb6savedata_ex_common; // �S�Z�[�u�f�[�^���ʕ�
