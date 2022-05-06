#pragma once

//--------------------------------------------------HEX 

// �w�b�N�X�̍��W���n�B�w�� �x���@�Œl���������Ă����B
struct HEX_POSITION {
	HEX_POSITION(double _x, double _y) : x(_x), y(_y) {}
	HEX_POSITION() : x(0), y(0) {}
	double x;
	double y;
};
// �t�B�[���h�w�b�N�X�̍��W�����ʂ̂R�������W(����f�J���g�E���Ɠ�����)
D3D_POSITION FieldHexPos_To_WorldPos(HEX_POSITION p);

// ���ʂ̂R�������W(����f�J���g)���t�B�[���h�w�b�N�X�̍��W��
HEX_POSITION WorldPos_To_FieldHexPos(D3D_POSITION p);

// ��w�b�N�X�̍��W�����ʂ̂R�������W(����f�J���g�E���Ɠ�����)
D3D_POSITION CastleHexPos_To_WorldPos(HEX_POSITION p);

// ���ʂ̂R�������W(����f�J���g)����w�b�N�X�̍��W��
HEX_POSITION WorldPos_To_CastleHexPos(D3D_POSITION p);



// �t�B�[���h�̕����x�N�g���B�w�� �x���@�Œl���������Ă����B
struct HEX_VECTOR {
	HEX_VECTOR(double _x, double _y) : x(_x), y(_y) {}
	HEX_VECTOR() : x(0), y(0) {}
	double x;
	double y;
};

// �t�B�[���h���W���u�̌��Z�B���W�`�����W�a�̃x�N�g�������߂�̂ɁA�ʏ�A�a�|�`�����邾�낤����B
HEX_VECTOR operator-(HEX_POSITION &p1, HEX_POSITION &p2);




//const int GAMEDATASTRUCT_CASTLE_HEX_POSX_MAX = GAMEDATASTRUCT_CASTLE_HEX_COLS;	// �U����HEX��HEX_POSITION�^�ɂ����ۂ̂�X�̍ő�l
//const int GAMEDATASTRUCT_CASTLE_HEX_POXY_MAX = GAMEDATASTRUCT_CASTLE_HEX_ROWS;	// �U����HEX��HEX_POSITION�^�ɂ����ۂ̂�Y�̍ő�l


const int GAMEDATASTRUCT_CASTLE_HEX_CENTER_X = 9;
const int GAMEDATASTRUCT_CASTLE_HEX_CENTER_Y = 8;

namespace CASTLE_HEX_ROLE {
	enum { �g�O = 0x00, ������n = 0x10, ������n = 0x11, ��O���n = 0x12, �X = 0x13, �x = 0x14, �C = 0x14, �� = 0x14, �x�� = 0x15, ��� = 0x16, �� = 0x17, �J�� = 0x18, �E = 0x19, �� = 0x1A, �{�� = 0x1B };
}


// �퓬��ʂ̏���w�b�N�X�̖����̃o�C�g��
union NB6CASTLE_HEX_ROLE {
	BYTE index[GAMEDATASTRUCT_CASTLE_HEX_COLS * GAMEDATASTRUCT_CASTLE_HEX_ROWS];				// �o�C�g��̃C���f�b�N�X�Ƃ��Ă݂��ꍇ
	struct {
		BYTE pos_x[GAMEDATASTRUCT_CASTLE_HEX_COLS];				// 18���A16�s�Ƃ��Č����ꍇ;
	} pos_y[GAMEDATASTRUCT_CASTLE_HEX_ROWS];
};

// �퓬��ʂ̏���w�b�N�X�̍����̃o�C�g��
union NB6CASTLE_HEX_HIGH {
	BYTE index[GAMEDATASTRUCT_CASTLE_HEX_COLS * GAMEDATASTRUCT_CASTLE_HEX_ROWS];				// �o�C�g��̃C���f�b�N�X�Ƃ��Ă݂��ꍇ
	struct {
		BYTE pos_x[GAMEDATASTRUCT_CASTLE_HEX_COLS];				// 18���A16�s�Ƃ��Č����ꍇ;
	} pos_y[GAMEDATASTRUCT_CASTLE_HEX_ROWS];
};

// ��U���̃��j�b�g�̃}�b�v���̃|�W�V����
union NB6CASTLE_HEX_UNIT_POS {
	BYTE index[GAMEDATASTRUCT_CASTLE_HEX_COLS * GAMEDATASTRUCT_CASTLE_HEX_ROWS];				// �o�C�g��̃C���f�b�N�X�Ƃ��Ă݂��ꍇ
	struct {
		BYTE pos_x[GAMEDATASTRUCT_CASTLE_HEX_COLS];				// 18���A16�s�Ƃ��Č����ꍇ;
	} pos_y[GAMEDATASTRUCT_CASTLE_HEX_ROWS];
};

// ����w�b�N�X�ꏊ�̃`�b�v�̖���(CASTLE_HEX_ROLE)�𓾂�
int GetCastleHexRole(double x, double y);
int GetCastleHexRole(HEX_POSITION p);

// ����w�b�N�X�ꏊ�̍���
int GetCastleHexHigh(double x, double y);
int GetCastleHexHigh(HEX_POSITION p);


namespace FIRE_HEX_ROLE {
	enum { ���� = 0x00, ���� = 0x01 };
}

// ����w�b�N�X�ꏊ�̃`�b�v�̉����
int GetFireHexRole(double x, double y);
int GetFireHexRole(HEX_POSITION p);

const int GAMEDATASTRUCT_FIELD_HEX_CENTER_X = 20;
const int GAMEDATASTRUCT_FIELD_HEX_CENTER_Y = 12;

namespace FIELD_HEX_ROLE {
	enum { �g�O = 0x00, ���n = 0x1, �� = 0x2, ���n = 0x3, �r��n = 0x4, �X = 0x5, ��R = 0x6, ���R = 0x7, ���R = 0x8, �� = 0x9, �� = 0xA, �C = 0xB, �� = 0xB, �� = 0xC /*�Q�A��*/ };
}


// ��펞�̃w�b�N�X�̖����̃o�C�g��
union NB6FIELD_HEX_ROLE {
	BYTE index[GAMEDATASTRUCT_FIELD_HEX_COLS * GAMEDATASTRUCT_FIELD_HEX_ROWS];				// �o�C�g��̃C���f�b�N�X�Ƃ��Ă݂��ꍇ
	struct {
		BYTE pos_x[GAMEDATASTRUCT_FIELD_HEX_COLS];				// 41���A24�s�Ƃ��Č����ꍇ;
	} pos_y[GAMEDATASTRUCT_FIELD_HEX_ROWS];
};

// ��펞�̃��j�b�g�̃}�b�v���̃|�W�V����
union NB6FIELD_HEX_UNIT_POS {
	BYTE index[GAMEDATASTRUCT_FIELD_HEX_COLS * GAMEDATASTRUCT_FIELD_HEX_ROWS];				// �o�C�g��̃C���f�b�N�X�Ƃ��Ă݂��ꍇ
	struct {
		BYTE pos_x[GAMEDATASTRUCT_FIELD_HEX_COLS];				// 41���A24�s�Ƃ��Č����ꍇ;
	} pos_y[GAMEDATASTRUCT_FIELD_HEX_ROWS];
};

// ��펞�̃w�b�N�X�n�̍��W���A�z��Ƃ��Ă̎��l��index�ւƕύX����B
void ConvertFieldHexPosToFieldIndexPos(double x, double y, int *out_x, int *out_y);

// �U��펞�̃w�b�N�X�n�̍��W���A�z��Ƃ��Ă̎��l��index�ւƕύX����B
void ConvertCastleHexPosToCastleIndexPos(double x, double y, int *out_x, int *out_y);



// ����w�b�N�X�ꏊ�̃`�b�v�̖���(FIELD_HEX_ROLE)�𓾂�
int GetFieldHexRole(double x, double y);
int GetFieldHexRole(HEX_POSITION p);

// �t�ɐݒ肷��B(�Ƃ����Ă��A�ݒ肵���Ƃ��Ă��A�G���ς��Ȃ��̂ŏ�肭�����Ȃ���)
void SetFieldHexRole(double x, double y, int eHexRole);
void SetFieldHexRole(HEX_POSITION p, int eHexRole);


// ����w�b�N�X�ꏊ�̍���
int GetFieldHexHigh(double x, double y);
int GetFieldHexHigh(HEX_POSITION p);



// �푈�ɏo�w���Ă���iBushouID�̈ꗗ�𓾂�B
vector<int> GetSyutujinBushouArray();
// ���ݕ\������Ă���}�b�v����ŁA�o�w���Ă���iBushouID�̈ꗗ�𓾂�B
vector<int> GetCurMapSyutujinBushouArray();

// �w��̕����h�c�̕������ǂ��ɂ���̂��H ���Ȃ��ꍇ�Ap(x=0xFF, y=0xFF) ���Ԃ��Ă���B
HEX_POSITION GetFieldHexUnitPos(int iBushouID);

// �w��̕����h�c�̕������ǂ��ɂ���̂��H ���Ȃ��ꍇ�Ap(x=0xFF, y=0xFF) ���Ԃ��Ă���B
HEX_POSITION GetCastleHexUnitPos(int iBushouID);

// �w�b�N�X�̂`�_�Ƃa�_�̊Ԃɂ���`�b�v�̃A���C��Ԃ��B
vector<int> RoleArrayOfBetweenPosToPos(HEX_POSITION pos1, HEX_POSITION pos2);


// �w�b�N�X�Ԋu�̎擾�Bp1��p2�܂ŉ��e�ړ�����K�v������̂�(�S�Ă̞e�͍s������̂Ɖ��肷��)
int getHexDistance(HEX_POSITION p1, HEX_POSITION p2);

bool Is_FieldWar();          // ���݉�ʂɏo�Ă���́A���
bool Is_CastleWar();         // ���݉�ʂɏo�Ă���́A�U���

int getCastleTurnInBattle();  // �U���ŉ��^�[���ڂ��B�P�`�R�D�U���łȂ��ꍇ�͂O�D

// ���̃��j�b�g�ԍ�
struct NB6BUSHOU_HEX_UNIT_INFO {
	WORD bushou;					// �����ԍ��B1�傫�����̕��B�����z��ԍ��Ɏ����ɂ�-1���邱�ƁB
	WORD junban;					// ���̒l�̓^�[�����ԁB�푈�ɂ����āA���̐��l���Ⴂ�R�c���珇�Ԃ����Ƃ������ƁB
	int	 pos_x;						// �s���ɂ�����pos_x�B�߂܂�����A�ދp�����肵�āA��ꂩ�狏�Ȃ��Ȃ�ƁA0xFFFF
	int	 pos_y;						// �s���ɂ�����pos_y�B�߂܂�����A�ދp�����肵�āA��ꂩ�狏�Ȃ��Ȃ�ƁA0xFFFF
	BYTE movepower;					// �ړ��͎c�ʁB0x1�`0xF���炢�̐��l�������Ă���B
	WORD _undef2;					// �s��2 ���j�b�g���݂��Ă�̂�FFFF�A���Ă��Ȃ��̂�
	BYTE _undef3;					// �s��1
	BYTE _undef4;					// �s��1
	BYTE _undef5;					// �s��1
	WORD _what1;					// �����Ӗ������肻���B1�`8���炢�̐��l�������Ă���B
	WORD _undef6;					// �ق�0x0000
	WORD _undef7;					// �ق�0xFFFF
	BYTE konran : 4;					// 1=�ʏ퍬�� 2=�卬�� 3=����
	BYTE bEnd : 4;					// �s���σt���O�B0x2=�s���ς�, 0x0=���s��
	BYTE _what4;					// �����Ӗ������肻���B���݂��Ă镐���́A�0x2�������
	WORD _what5;					// �����Ӗ������肻���B���݂��Ă镐���́A�0x70�������
	int	 _undef8;					// �s��
};
/*
�ȉ��A�퓬�֘A�̊e��p�����[�^�ւ̃A�N�Z�X�ESet�n�֐��Ǝv����
0042DBF1  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0042DBF5  |. E8 DFFEFFFF    CALL    TENSHOU.0042DAD9
0042DBFA  |. 0FB640 0C      MOVZX   EAX, BYTE PTR DS:[EAX+C]
0042DBFE  |. 83C4 04        ADD     ESP, 4
0042DC01  \. C3             RETN
0042DC02  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0042DC06  |. E8 CEFEFFFF    CALL    TENSHOU.0042DAD9
0042DC0B  |. 8A4C24 0C      MOV     CL, BYTE PTR SS:[ESP+C]
0042DC0F  |. 83C4 04        ADD     ESP, 4
0042DC12  |. 8848 0C        MOV     BYTE PTR DS:[EAX+C], CL
0042DC15  \. C3             RETN
0042DC16  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0042DC1A  |. E8 BAFEFFFF    CALL    TENSHOU.0042DAD9
0042DC1F  |. 83C4 04        ADD     ESP, 4
0042DC22  |. 8B40 0D        MOV     EAX, DWORD PTR DS:[EAX+D]
0042DC25  \. C3             RETN
0042DC26  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0042DC2A  |. E8 AAFEFFFF    CALL    TENSHOU.0042DAD9
0042DC2F  |. 8B4C24 0C      MOV     ECX, DWORD PTR SS:[ESP+C]
0042DC33  |. 83C4 04        ADD     ESP, 4
0042DC36  |. 66:8948 0D     MOV     WORD PTR DS:[EAX+D], CX
0042DC3A  \. C3             RETN
0042DC3B  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0042DC3F  |. E8 95FEFFFF    CALL    TENSHOU.0042DAD9
0042DC44  |. 0FB640 11      MOVZX   EAX, BYTE PTR DS:[EAX+11]
0042DC48  |. 83C4 04        ADD     ESP, 4
0042DC4B  \. C3             RETN
0042DC4C  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0042DC50  |. E8 84FEFFFF    CALL    TENSHOU.0042DAD9
0042DC55  |. 8A4C24 0C      MOV     CL, BYTE PTR SS:[ESP+C]
0042DC59  |. 83C4 04        ADD     ESP, 4
0042DC5C  |. 8848 11        MOV     BYTE PTR DS:[EAX+11], CL
0042DC5F  \. C3             RETN
0042DC60  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0042DC64  |. E8 70FEFFFF    CALL    TENSHOU.0042DAD9
0042DC69  |. 8B4C24 0C      MOV     ECX, DWORD PTR SS:[ESP+C]
0042DC6D  |. 83C4 04        ADD     ESP, 4
0042DC70  |. 66:8948 12     MOV     WORD PTR DS:[EAX+12], CX
0042DC74  \. C3             RETN
0042DC75  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0042DC79  |. E8 5BFEFFFF    CALL    TENSHOU.0042DAD9
0042DC7E  |. 83C4 04        ADD     ESP, 4
0042DC81  |. 8B40 12        MOV     EAX, DWORD PTR DS:[EAX+12]
0042DC84  \. C3             RETN
0042DC85  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0042DC89  |. E8 4BFEFFFF    CALL    TENSHOU.0042DAD9
0042DC8E  |. 8B4C24 0C      MOV     ECX, DWORD PTR SS:[ESP+C]
0042DC92  |. 83C4 04        ADD     ESP, 4
0042DC95  |. 0948 18        OR      DWORD PTR DS:[EAX+18], ECX
0042DC98  \. C3             RETN
0042DC99  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0042DC9D  |. E8 37FEFFFF    CALL    TENSHOU.0042DAD9
0042DCA2  |. 8B4C24 0C      MOV     ECX, DWORD PTR SS:[ESP+C]
0042DCA6  |. 83C4 04        ADD     ESP, 4
0042DCA9  |. F7D1           NOT     ECX
0042DCAB  |. 2148 18        AND     DWORD PTR DS:[EAX+18], ECX
0042DCAE  \. C3             RETN
0042DCAF  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0042DCB3  |. E8 21FEFFFF    CALL    TENSHOU.0042DAD9
0042DCB8  |. 83C4 04        ADD     ESP, 4
0042DCBB  |. 8B40 18        MOV     EAX, DWORD PTR DS:[EAX+18]
0042DCBE  |. 234424 08      AND     EAX, DWORD PTR SS:[ESP+8]
0042DCC2  \. C3             RETN
0042DCC3  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0042DCC7  |. E8 0DFEFFFF    CALL    TENSHOU.0042DAD9
0042DCCC  |. 8B4C24 0C      MOV     ECX, DWORD PTR SS:[ESP+C]
0042DCD0  |. 83C4 04        ADD     ESP, 4
0042DCD3  |. 0948 1C        OR      DWORD PTR DS:[EAX+1C], ECX
0042DCD6  \. C3             RETN
0042DCD7  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0042DCDB  |. E8 F9FDFFFF    CALL    TENSHOU.0042DAD9
0042DCE0  |. 8B4C24 0C      MOV     ECX, DWORD PTR SS:[ESP+C]
0042DCE4  |. 83C4 04        ADD     ESP, 4
0042DCE7  |. F7D1           NOT     ECX
0042DCE9  |. 2148 1C        AND     DWORD PTR DS:[EAX+1C], ECX
0042DCEC  \. C3             RETN
0042DCED  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0042DCF1  |. E8 E3FDFFFF    CALL    TENSHOU.0042DAD9
0042DCF6  |. 83C4 04        ADD     ESP, 4
0042DCF9  |. 8B40 1C        MOV     EAX, DWORD PTR DS:[EAX+1C]
0042DCFC  |. 234424 08      AND     EAX, DWORD PTR SS:[ESP+8]
0042DD00  \. C3             RETN
*/
/*
��L�l�̏����������B�푈�J�n��
0042DD59  /$ 56             PUSH    ESI
0042DD5A  |. 57             PUSH    EDI
0042DD5B  |. 66:BE 0100     MOV     SI, 1
0042DD5F  |. BF FFFF0000    MOV     EDI, 0FFFF
0042DD64  |> 57             /PUSH    EDI
0042DD65  |. 56             |PUSH    ESI
0042DD66  |. E8 A2FDFFFF    |CALL    TENSHOU.0042DB0D
0042DD6B  |. 83C4 08        |ADD     ESP, 8
0042DD6E  |. 57             |PUSH    EDI
0042DD6F  |. 56             |PUSH    ESI
0042DD70  |. E8 BEFDFFFF    |CALL    TENSHOU.0042DB33
0042DD75  |. 83C4 08        |ADD     ESP, 8
0042DD78  |. 57             |PUSH    EDI                                                        ; /Arg3
0042DD79  |. 57             |PUSH    EDI                                                        ; |Arg2
0042DD7A  |. 56             |PUSH    ESI                                                        ; |Arg1
0042DD7B  |. E8 3AFEFFFF    |CALL    TENSHOU.0042DBBA                                           ; \TENSHOU.0042DBBA
0042DD80  |. 83C4 0C        |ADD     ESP, 0C
0042DD83  |. 6A 00          |PUSH    0
0042DD85  |. 56             |PUSH    ESI
0042DD86  |. E8 77FEFFFF    |CALL    TENSHOU.0042DC02
0042DD8B  |. 83C4 08        |ADD     ESP, 8
0042DD8E  |. 57             |PUSH    EDI
0042DD8F  |. 56             |PUSH    ESI
0042DD90  |. E8 91FEFFFF    |CALL    TENSHOU.0042DC26
0042DD95  |. 83C4 08        |ADD     ESP, 8
0042DD98  |. 6A 00          |PUSH    0
0042DD9A  |. 56             |PUSH    ESI
0042DD9B  |. E8 ACFEFFFF    |CALL    TENSHOU.0042DC4C
0042DDA0  |. 83C4 08        |ADD     ESP, 8
0042DDA3  |. 57             |PUSH    EDI
0042DDA4  |. 56             |PUSH    ESI
0042DDA5  |. E8 B6FEFFFF    |CALL    TENSHOU.0042DC60
0042DDAA  |. 83C4 08        |ADD     ESP, 8
0042DDAD  |. 57             |PUSH    EDI
0042DDAE  |. 56             |PUSH    ESI
0042DDAF  |. E8 E1000000    |CALL    TENSHOU.0042DE95
0042DDB4  |. 83C4 08        |ADD     ESP, 8
0042DDB7  |. 57             |PUSH    EDI
0042DDB8  |. 56             |PUSH    ESI
0042DDB9  |. 66:46          |INC     SI
0042DDBB  |. E8 D9FEFFFF    |CALL    TENSHOU.0042DC99
0042DDC0  |. 83C4 08        |ADD     ESP, 8
0042DDC3  |. 66:83FE 48     |CMP     SI, 48
0042DDC7  |.^76 9B          \JBE     SHORT TENSHOU.0042DD64
0042DDC9  |. 5F             POP     EDI
0042DDCA  |. 5E             POP     ESI
0042DDCB  \. C3             RETN
*/


// �w�蕐�������ɂ����āA�s���ς݂��ǂ���
BOOL IsActionEndInBattle(int iBushouID);
// �w�蕐�����s���ς݂Ƃ���
void setIsActionEndInBattle(int iBushouID, int isEnd);

namespace KONRAN_STATUS {
	enum { ���� = 0, ���� = 1, �卬�� = 2, �������� = 3 };
}
// �Ώۂ̕������������Ă��邩�B
int GetKonranStatus(int iBushouID);
// �Ώۂ̕���������������B(�ĕ`��𔺂�)
BOOL SetKonranStatus(int iBushouID, int iKonranStatus);
// �Ώۂ̕������w���KONRAN_STATUS�ɂ���

// �w�b�N�X�}�b�v�ɂ�����B�S���^�[������A�喼�A�R�c�ȂǁB
struct NB6GUNDAN_HEX_INFO {
	int _undef;			// �s��
	int daimyo;			// ���݃^�[���̑喼�ԍ�
	int gundan;			// ���݃^�[���̌R�c�ԍ�
	int junban;		    // ���݃^�[���̌R�c�́u���ԁv�ԍ��B
};


// �w�b�N�X�}�b�v�ɂ����āA���̐푈�Ɋւ���Ă���u�喼�E�R�c�E���叫�E[�U�ߎ�E�󂯎�E����]�v���
struct NB6WAR_JOINT_DAIMYO_HEX_GROUP_INFO {
	WORD daimyo;		// �喼�ԍ�
	WORD gundan;		// �R�c�ԍ�
	WORD soudaisho;		// ���叫�̕����ԍ�
	WORD group;			// ���Ԃ��U�����A�󑤂��A�Ȃǂ́A�s��B�����܂ł��A�퓬�J�n���̍U�ߑ��喼�Ɠ����O���[�v���U�ߑ��̉��R�A�󑤂̑喼�Ɠ����O���[�v���󑤁A����ȊO�������ł���B
	WORD _undef;		// ���ɂȂɂ��Ȃ��Bmust 0000
};

// ���ݐi�s���̐푈�Ɋւ���Ă���iDaimyoID�̃��X�g�B(�o�w���Ă��邩�ǂ����͊֌W�Ȃ�)
vector<int> GetCurWarJointDaimyoIDList();

// ���ݐi�s���̐푈�Ɋւ���Ă���iGundanID�̃��X�g�B(�o�w���Ă��邩�ǂ����͊֌W�Ȃ�)
vector<int> GetCurWarJointGundanIDList();



namespace WarStandPoint {
	//	   �s��          ���ڍU����   ���ڎ����   �U�����̉��R�@      ������̉��R�@�@�����R 
	enum { unknown = 0, attack = 1, defend = 2, attack_enngun = 3, defend_enngun = 4, chuuritsu = 5 };
};

// �Y���̌R�c�̐��ɂ����闧���Ԃ�(�s��=0, ���ڍU�߂鑤=1, ���ڎ�鑤=2, �U�߂鑤�̉��R=3, ��鑤�̉��R=4, �����R=5)
int GetWarStandPointTheGundan(int iGundanID);

// �Y���̕����̐��ɂ����闧���Ԃ�(�s��=0, ���ڍU�߂鑤=1, ���ڎ�鑤=2, �U�߂鑤�̉��R=3, ��鑤�̉��R=4, �����R=5)
int GetWarStandPointTheBushou(int iBushouID);

// ���݂̐�ɂ����āA�w��R�c�̑��叫(�R�c����喼�̂��Ƃł͂Ȃ��A���ɂ����Ă̊Y���R�c�̑��叫�B
int GetWarSoudaishoTheGundan(int iGundanID);





// �푈�ł��łɉ��^�[���o�߂�����
byte getHowMuchTurnProceesInBattle();

// �푈�ŉ��^�[���c���Ă��邩�B�������ł͂Ȃ��A�����v�Z
// �퓬�^�[�����łȂ���� -1 ���Ԃ�B
int getRemainTurnInBattle();




// �V�C���𓾂� 0:���� 1:�܂� 2:�J 3:��
byte getWeather();

void reserveWeather(int eTenki, int iContinueTurn = 2);
