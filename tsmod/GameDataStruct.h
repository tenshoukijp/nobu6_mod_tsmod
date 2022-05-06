#pragma once
#pragma warning ( 3 : 4706 )

#pragma pack(1)
/*
	�\���̂Ŗ��ߍ��݂��K�v�ȏꍇ�́A�R���p�C�� �f�B���N�e�B�u #pragma pack ���g�p���邱�Ƃ��ł��܂��B
	�������A#pragma pack ���g�p�����ꍇ�A�\���̗̂v�f�͐��񂳂�܂���B
	(�\���̂Ƃ��ŏ���Ɏw��̃r�b�g�Ƃ͂��ƂȂ�悤�Ƀp�b�L���O���Đ��񂵂Ȃ����悤���B
	�����#pragma pack(1)�Ƃ��邱�ƂŁA1bit���̐���ƂȂ邽�߁A������A���񂳂�Ȃ��Ȃ�悤���B
*/


#include "StrFormatMatch.h"
#include "ScenarioDataStruct.h"
#include "SndataN6PDataStruct.h"
#include "MapData.h"
#include "OutputDebugString.h"
#include "OutputDebugStream.h"
#include "OnigRegex.h"
#include "ToCopyClipboard.h"
#include "Menu.h"
#include "OnChangeWeather.h"
#include "3DPosition.h"
#include "2DPosition.h"
#include "FuncGetGunshiBushouID.h"
#include "Todouhuken.h"
#include "FactoryGeneralKaoFace.h"
#include "HexmapNB6.h"




#define GAMEDATASTRUCT_SAVE_OR_LOAD_SCENARIO_ID_ADDRESS  0x4CEFA0 // ���[�h�������̓Z�[�u�������ɁA���̃A�h���X�ɃV�i���I�ԍ�������B�V�i���I�I�����ɂ͓���Ȃ��B

#define GAMEDATASTRUCT_SCENARIO_NAME_POINTER_ARRAY_ADDRESS		0x4D3728 // �U�̃V�i���I���ւ̃|�C���^�z��̐擪�A�h���X

#define GAMEDATASTRUCT_REGISTED_BUSHOU_NUM		1300		// [�o�^�ςݕ�����]
#define GAMEDATASTRUCT_RETSUDEN_NUM				1332		// [��`�̌�]


#define GAMEDATASTRUCT_YEAR_ADDRESSS			0x4CF210	// [�N�E�G��]
#define GAMEDATASTRUCT_START_YEAR_ADDRESS		0x466A8A	// �V�ċL�ɂƂ��Ă̌��N(0�N�j(����1454)

#define GAMEDATASTRUCT_TURN_ADDRESSS			0x4C9355	// [�^�[�����]

#define GAMEDATASTRUCT_SOBA_ADDRESSS			0x4C93FD	// [������]

#define GAMEDATASTRUCT_PLAYERS8_ADDRESSS		0x4C93ED	// [�v���C���[�S���喼�̒S���̌R�c�̔ԍ�]
#define GAMEDATASTRUCT_PLAYERS8_NUM				8			// [��L�̍ő吔�i��������v���C���[�ő吔�j���W]

#define GAMEDATASTRUCT_TOUZYOU_BUSHOU_NUM_ADDRESS       0x4CFC37    // �ҋ@����o�ꂵ����
#define GAMEDATASTRUCT_MITOUZYOU_BUSHOU_NUM_ADDRESS     0x4CFC39    // ���o�ꕐ���̑�����

#define GAMEDATASTRUCT_BUSHOU_ADDRESSS			0x4BFD18	// [�����f�[�^(47*532)]
#define GAMEDATASTRUCT_BUSHOUNAME_ADDRESSS		0x4C5FA8	// [�������O�e�[�u��(18*500�ƕP���p��18*32)]
#define	GAMEDATASTRUCT_BUSHOU_NUM				532

#define GAMEDATASTRUCT_KAMON_REGULAR_NUM		16 *  7			// ���K�Ɩ�̐��B�Q�[�����Ń��[�U�[�ҏW�ȊO�ŏo�Ă��邱�ƂɑÓ��������鐔
#define GAMEDATASTRUCT_KAMON_ALL_NUM			16 * 10			// �Ɩ�̐��B�Ō��16�̓��[�U�[���ҏW��������

#define GAMEDATASTRUCT_CASTLE_ADDRESSS			0x4BC918	// [����(33*214)]
#define	GAMEDATASTRUCT_CASTLE_NUM				214

#define GAMEDATASTRUCT_CASTLEMAX_ADDRESSS		0x4BE778	// [��̈ʒu���Ȃ�(8*214)]

#define	GAMEDATASTRUCT_CASTLETYPE_NUM			7			// [��̃^�C�v(�䏊�Ƃ��قƂ�)]�̐�

#define GAMEDATASTRUCT_CASTLE_HEX_ROLE_ADDRESS	0x4D5182	// ����̖����̊T�O�̃f�[�^�̃A�h���X(18*16�o�C�g)
#define GAMEDATASTRUCT_CASTLE_HEX_HIGH_ADDRESS	0x4D555A	// ����̍����̊T�O�̃f�[�^�̃A�h���X(18*16�o�C�g)


#define GAMEDATASTRUCT_FIELD_HEX_ROLE_ADDRESS	0x4D5182	// ���̖����̊T�O�̃f�[�^�̃A�h���X(41*24�o�C�g)
#define GAMEDATASTRUCT_FIELD_HEX_UNIT_POS_ADDRESS	0x4D5932	// ���Ń��j�b�g������ʒu���|�c���|�c���B���ݕ\������HEX�}�b�v�łقƂ�ǂ�FF�A���j�b�g����Ƃ���́A���j�b�g�ԍ��������Ă���B
#define GAMEDATASTRUCT_CASTLE_HEX_UNIT_POS_ADDRESS	0x4D5932	// �U���Ń��j�b�g������ʒu���|�c���|�c���B���ݕ\������HEX�}�b�v�łقƂ�ǂ�FF�A���j�b�g����Ƃ���́A���j�b�g�ԍ��������Ă���B
																// ���Ƌ��ʂ̃A�h���X�B
#define GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_ADDRESS	0x4D5D58	// HEX�}�b�v�ɂ�����e���j�b�g�̏��(1�l32bit) x 1�邩��o�Ă���W�l �~ �P��킠����X��ō��v 72�lx32bit
#define GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_NUM	72			// HEX�}�b�v�ɂ����郆�j�b�g�̍ő吔�B1�邩��o�Ă���W�l �~ �P��킠����X��

#define GAMEDATASTRUCT_CASTLE_HEX_FIRE_ROLE_ADDRESS	0x4D4DAA	// ����̉΂̎�֘A�̊T�O�̃f�[�^�̃A�h���X(18*16�o�C�g)

#define GAMEDATASTRUCT_FIELD_HEX_COLS			41	// ����HEX�̉��̐�
#define GAMEDATASTRUCT_FIELD_HEX_ROWS			24	// ����HEX�̏c�̐�

#define GAMEDATASTRUCT_CASTLE_HEX_COLS			18	// �U����HEX�̉��̐�
#define GAMEDATASTRUCT_CASTLE_HEX_ROWS			16	// �U����HEX�̏c�̐�


#define GAMEDATASTRUCT_WAR_HEX_TURN_ADDRESS		0x4D5D48	// HEX�}�b�v�ɂ����āA���݂ǂ̌R�c�̏��ԂƂȂ��Ă���̂��B�e���Ԃ͉����Ŋ��蓖�Ă��Ă���B���̏��ԂōU�߂�Ƃ����킯�͂Ȃ��A�e�ԍ��ɌR�c�����蓖�Ă���͗l�B

#define GAMEDATASTRUCT_WAR_JOINT_DAIMYO_HEX_GROUP_INFO_ADDRESS	0x4D4D48	//  �w�b�N�X�}�b�v�ɂ����āA���̐푈�Ɋւ���Ă���u�喼�E�R�c�E���叫�E[�U�ߎ�E�󂯎�E����]�v���
#define GAMEDATASTRUCT_WAR_JOINT_DAIMYO_HEX_GROUP_INFO_NUM		9			//  �푈�Ɋ֌W����喼�͍ő�X�喼�܂ŁB

#define	GAMEDATASTRUCT_GUNDAN_ADDRESS			0x4C9640	// [�R�c���]
#define	GAMEDATASTRUCT_GUNDAN_NUM				214


#define	GAMEDATASTRUCT_DAIMYO_ADDRESS			0x4CE7B8	// [�喼���] 
#define	GAMEDATASTRUCT_DAIMYO_NUM				72

#define GAMEDATASTRUCT_DOUMEI_ADDRESS			0x4CF213	// [�������] (A�喼-B�喼�Ԃ̓����l)

#define	GAMEDATASTRUCT_CHIMEI_ADDRESS			0x4BF278	// [�n�����]
#define	GAMEDATASTRUCT_CHIMEI_NUM				74


#define GAMEDATASTRUCT_SOLDIERMAX_ADDRESS		0x4BB7A0	// [�ő啺��]
#define GAMEDATASTRUCT_SOLDIERMAX_NUM			7

#define GAMEDATASTRUCT_PARAMMAX_ADDRESS			0x4BB6E0	// [�e��p�����^�̍ő�l�B�����n]
#define GAMEDATASTRUCT_PARAMMAX_MAIN_ADDRESS	0x4CD8A0	// [�e��p�����^�̍ő�l�B���C���n]

#define	GAMEDATASTRUCT_KANI_ADDRESS				0x4CC2F0	// [���ʏ��]
#define	GAMEDATASTRUCT_KANI_NUM					250
#define	GAMEDATASTRUCT_KANITYPE_NUM				23			// [���ʎ�ނ̐�] ����ʁ`�]���ʉ� �܂�

#define	GAMEDATASTRUCT_POSITIONTYPELIST_POINTER_ADDRESS	0x4D1554	// [��E���]�̂��ꂼ��̕�����ւ̃|�C���^�̃��X�g�̐擪�A�h���X
#define	GAMEDATASTRUCT_POSITIONTYPELIST_ADDRESS	0x4CFC3B			// [��E���]
#define	GAMEDATASTRUCT_POSITIONTYPE_NUM			8

#define GAMEDATASTRUCT_KAHOU_ADDRESSS			0x4D2108	// [�ƕ���]
#define	GAMEDATASTRUCT_KAHOU_NUM				120

#define GAMEDATASTRUCT_KAHOUTYPELIST_POINTER_ADDRESS	0x4D1420	// [�ƕ���]�̂��ꂼ��̕�����ւ̃|�C���^�̃��X�g�̐擪�A�h���X
#define	GAMEDATASTRUCT_KAHOUTYPELIST_ADDRESS	0x4CFDCC			// [�ƕ���]
#define	GAMEDATASTRUCT_KAHOUTYPE_NUM			24


#define GAMEDATASTRUCT_SAVEDATAFILENAME_ADDRESS 0x4C8D72    // [�Z�[�u�f�[�^�̃t�@�C�����̃A�h���X]

#define GAMEDATASTRUCT_REQUIREDMERITS_ADDRESS   0x4D1950    // [�K�v�M���l�̃A�h���X]
#define GAMEDATASTRUCT_REQUIREDMERITS_NUM		5			// [�h�V�E�ƘV�E�����E���叫�E���y��]�̂T��ނ���`����Ă���B

#define GAMEDATASTRUCT_MONEY_MAX				60000		// ������MAX
#define GAMEDATASTRUCT_RISE_MAX					60000		// ���Ă�MAX
#define GAMEDATASTRUCT_GUN_MAX					60000		// �S�C��MAX
#define GAMEDATASTRUCT_KIBA_MAX					60000		// �R�n��MAX


#define GAMEDATASTRUCT_AISHOU_NUM				16			// �����̌� 0�`15


#define GAMEDATASTRUCT_HOWMUCHTURNPROCEED_INBATTLE	0x4BB4D2	// �푈�����^�[���o�߂�����
#define GAMEDATASTRUCT_HOWMUCHTURNPROCEED_INCASTLEBATTLE	0x4D5D44	// ���ݕ\�����̍U���͉��^�[���ڂ��i�O�`�Q�j



#define GAMEDATASTRUCT_WEATHER_ADDRESSS			0x4BB4D6	// �V�C�̃A�h���X

#define GAMEDATASTRUCT_EVENT_GROUP1_ADDRESSS		0x4CFC33	// �C�x���g�A�h���X���̂P b00000001 �S�C�`��(=1) b00000010(=2 �L���X�g���`���A�����낢�낱�̃t���O�ŊǗ�����Ă�����ۂ��B
#define GAMEDATASTRUCT_EVENT_GROUP2_ADDRESSS		0x4CFC34	// �C�x���g�A�h���X���̂Q b01000000 ����z��(=1)

#define GAMEDATASTRUCT_SETTINGS_ADDRESSS		0x4C9351	// ���ݒ�̃A�h���X

#define GAMEDATASTRUCT_SETTINGSEX_ADDRESSS		0x4DEE00	// ���ݒ�g��(�ؑ�)�̃A�h���X


#define GAMEDATASTRUCT_SELECTING_BUSHOU_LIST	0x4DBD40	// �����I�𓙂̎��ɁA�������X�g�̕���ID�������Ă���B0xFFFF�܂ł��ΏہB


#define GAMEDATASTRUCT_FUKIDASHI_RELATE_PERSONS	0x4CD770	// ��́E����E��b���ɏo�Ă���֘A���Ă���l���B

#define GAMEDATASTRUCT_VARIABLE_VALUE_GROUP1_DIALOG	0x4DDF38	// ��́u�l�v��u���l�v�̓��ꕨ�̓��͌��ʕ\�����g�B��P�O���[�v
#define GAMEDATASTRUCT_VARIABLE_VALUE_GROUP2_DIALOG	0x4DDF58	// ���i���́u�l�v���I���̓��͌��ʕ\�����g�B��Q�O���[�v


#define GAMEDATASTRUCT_PLAYING_BGM_ID_ADDRESS	0x4D2A7C	// �Đ�����BGM��ID�̃A�h���X(WORD)

#define GAMEDATASTRUCT_WINMM_DLL_NAME_ADDRESS	0x4E7CC0	// WINMM.DLL��_INMM.DLL�Ȃ̂��B


#define GAMEDATASTRUCT_BUSHOU_RETSUDEN_NUM_PATCH_ADDRESS	0x40CB5A	// TENSHOU.EXE�̗�`�l���������蕔����1932�ȂǁA�ύX���ׂ��ꏊ�B�j�[���j�b�N�̒��Ȃ̂Œ���	

#define GAMEDATASTRUCT_FONTNAME_ADDRESS			0x4BC884    // �t�H���g�����i�[����Ă���A�h���X�B

#define GAMEDATASTRUCT_MOVIE_NUM				36			// �b�c�|�q�n�l�ɂ��郀�[�r�[�̌��B
#define GAMEDATASTRUCT_MOVIELISTPOINTER_ADDRESS				0x4C9588	// TENSHOU.EXE��koeilogo���̊e���[�r�[�̃|�C���^���z��ƂȂ��Ċi�[����Ă���ŏ��̏ꏊ�B

#define GAMEDATASTRUCT_MAINMAP_DATA_ADDRESS		0x4CF212	// ���C���}�b�v�̃p���b�g�{���C���}�b�v�̃f�[�^�̊i�[�ꏊ

#define GAMEDATASTRUCT_TENSHOUKI_DRIVENAME_ADDRESS	0x4C9580 // �V�ċL�̃h���C�u���̕����񂪓����Ă�A�h���X�B
#define GAMEDATASTRUCT_TENSHOUKI_DISKLABEL_ADDRESS	0x4CC278 // �V�ċL�̂b�c�f�B�X�N�̃��x���̕����񂪓����Ă�A�h���X�B

#define GAMEDATASTRUCT_HIMENAME_ARRAY			0x4CA9B0	// �P�������񂳂�Ă���ꏊ�B1�l7�o�C�g�B(�S�p3�����{NULL)��100�l���B

#define GAMEDATASTRUCT_SYOUNIN_NUM				10			// �Q�[�����̏��l�B10�l�B
#define GAMEDATASTRUCT_GOYOUSYOUNINNAME_ARRAY	0x4D1490	// ���l���ւ̃|�C���^�����񂵂Ă���ꏊ�B

#define GAMEDATASTRUCT_RESOLUTION_ADDRESS		0x4DFD40	// ��������int�ŘA�����ĉ��E�c�̏�񂪊i�[����Ă���B

#define GAMEDATASTRUCT_FUKIDASHI_DIALOG_LAYOUT_ADDRESS	0x4DA940	// �_�C�A���O�n���C�A�E�g�́u����v�Ɓu�E���v�̊�_���f�ڂ���Ă���B


#include "SaveDataStructEx.h"


#include "GameDataStructMainTurn.h"

#include "GameDataStructBushou.h"

#include "GameDataStructTaikiBushou.h"

#include "GameDataStructBattle.h"

#include "GameDataStructCastle.h"

#include "GameDataStructChimei.h"

#include "GameDataStructGundan.h"

#include "GameDataStructDaimyo.h"

#include "GameDataStructYakusyoku.h"

#include "GameDataStructKani.h"

#include "GameDataStructKahou.h"

#include "GameDataStructMaxmum.h"


#include "GameDataStructMainCommand.h"


#include "GameDataStructSettings.h"

#include "GameDataStructUI.h"

// extern����̃����h�N�T�C�̂ł����ł܂Ƃ߂Ă��Ă���
// �ǂ����Ă�TSMod.dll�̎������c��ݏd���Ȃ��Ă��܂����ꍇ�́A
// ��ϐ��ւ̃|�C���^���]����������̂ւ̢�Q�ƌ^��ɂ��āAextern ������΁A�L�q���z���������ɏo����B
// nb6bushou�ɂ��Ă̓����������b�N���Ă���u�Ԃ�TSMod.dll�������������Ƃ��Ă��ӂ��Ƃ΂Ȃ��悤�ɑS�ʓI�ɋL�q����Ă���B(���̑���A�܂�肭�ǂ����d��)
extern NB6YEAR			*const _nb6year;
#define							nb6year		_nb6year[0]
extern NB6TURN			*const _nb6turn;
#define							nb6turn		_nb6turn[0]
extern NB6SOBA			*const _nb6soba;
#define							nb6soba		_nb6soba[0]
extern NB6PLAYERS8		*const nb6players8;
extern NB6BUSHOUCOPY	nb6bushoucopy[GAMEDATASTRUCT_BUSHOU_NUM];
extern NB6BUSHOU		*const nb6bushouref;	// �Q�Ɛ�p
extern NB6BUSHOUNAME	*const nb6bushouname;
extern NB6BUSHOURETSUDEN   nb6bushouretsuden[GAMEDATASTRUCT_RETSUDEN_NUM];
extern NB6KAHOURETSUDEN    nb6kahouretsuden[GAMEDATASTRUCT_KAHOU_NUM];
extern NB6CASTLE		*const nb6castle;
extern NB6CASTLEMAX		*const nb6castlemax;

extern NB6GUNDAN		*const nb6gundan;
extern NB6DAIMYO		*const nb6daimyo;
extern NB6CHIMEI		*const nb6chimei;
extern NB6KANI			*const nb6kani;
extern NB6KAHOU			*const nb6kahou;

extern NB6SOLDIERMAX	nb6soldiermax[GAMEDATASTRUCT_SOLDIERMAX_NUM];
extern NB6PARAMMAX		*const _nb6parammax;
#define							nb6parammax		_nb6parammax[0]
extern NB6PARAMMAX_MAIN		*const _nb6parammax_main;
#define							nb6parammax_main	_nb6parammax_main[0]
extern NB6REQUIREDMERITS   *const nb6requiredmerits;
extern NB6SETTINGS		*const _nb6settings;
#define						    nb6settings		_nb6settings[0]
extern NB6SETTINGSEX	*const _nb6settingsex;
#define						    nb6settingsex	_nb6settingsex[0]

extern char *psznb6savedatafilename;

extern int GAMEDATASTRUCT_KAHOUTYPELIST_POINTER[GAMEDATASTRUCT_KAHOUTYPE_NUM];
extern int GAMEDATASTRUCT_POSITIONTYPELIST_POINTER[GAMEDATASTRUCT_POSITIONTYPE_NUM];

extern NB6SOLDIERMAX_CUSTOM nb6soldiermax_custom[GAMEDATASTRUCT_SOLDIERMAX_NUM];

extern NB6FUKIDASHIRELATEPERSONS *const nb6fukidashirelatepersons;


extern NB6CASTLE_HEX_ROLE *const _nb6castle_hex_role;
#define							  nb6castle_hex_role		_nb6castle_hex_role[0]
extern NB6CASTLE_HEX_HIGH *const _nb6castle_hex_high;
#define							  nb6castle_hex_high		_nb6castle_hex_high[0]

extern NB6FIELD_HEX_ROLE *const _nb6field_hex_role;
#define							 nb6field_hex_role			_nb6field_hex_role[0]

extern NB6CASTLE_HEX_ROLE *const _nb6fire_hex_role;
#define							  nb6fire_hex_role			_nb6fire_hex_role[0]

extern NB6CASTLE_HEX_UNIT_POS *const _nb6castle_hex_unit_pos;
#define							      nb6castle_hex_unit_pos		_nb6castle_hex_unit_pos[0]
extern NB6FIELD_HEX_UNIT_POS *const  _nb6field_hex_unit_pos;
#define							      nb6field_hex_unit_pos			_nb6field_hex_unit_pos[0]

extern NB6BUSHOU_HEX_UNIT_INFO *const nb6bushou_hex_unit_info;
extern NB6GUNDAN_HEX_INFO	   *const _nb6gundan_hex_info;
#define							      nb6gundan_hex_info		_nb6gundan_hex_info[0]

extern NB6WAR_JOINT_DAIMYO_HEX_GROUP_INFO *const nb6war_joint_daimyo_hex_group_info;

extern NB6FUKIDASHI_DIALOG_LAYOUT *const _nb6fukidashi_dialog_layout;
#define						nb6fukidashi_dialog_layout		_nb6fukidashi_dialog_layout[0]


