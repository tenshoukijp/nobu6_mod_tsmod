#include "TSModCommand.h"
#include "SetNeedKoudouryoku.h"

#include "string.h"
#include "OutputDebugStream.h"
#include <windows.h>
#include <map>
using namespace std;

int TSModCommand::gurdian1[1000] = {};

int TSModCommand::World::cmdTheTeppouNoExists							= (int)0;
int TSModCommand::World::cmdTheYumiNoExists								= (int)0;
int TSModCommand::World::cmdCustomKahouExists							= (int)0;
int TSModCommand::World::cmdVirtualBushouAppear							= (int)0;


int TSModCommand::MyselfGundan::cmdGetMoneyAndRiseOnSeasonChangeLv		= (int)0;
int TSModCommand::OthersGundan::cmdGetMoneyAndRiseOnSeasonChangeLv		= (int)0;
int TSModCommand::MyselfGundan::cmdRegenerationSoldierAutomatically		= (int)0;
int TSModCommand::OthersGundan::cmdRegenerationSoldierAutomatically		= (int)0;
int TSModCommand::MyselfGundan::cmdIncreaseSpecialProductByTheThing		= (int)0; // ���Y�i�Ɋ֘A�������ڂ������I�ɑ�����(�S�C�E�n�E���ۍ`)
int TSModCommand::OthersGundan::cmdIncreaseSpecialProductByTheThing		= (int)0; // ���Y�i�Ɋ֘A�������ڂ������I�ɑ�����(�S�C�E�n�E���ۍ`)

int TSModCommand::OthersGundan::cmdPlayerAndTheWholeInfluenceEachOtherData = (int)0;
int TSModCommand::AllGundan::cmdTheNumberOfTheMaxGoToWarFix				= (int)0;

// �u�u�`�v�ɕK�v�ȍs���l
int  TSModCommand::AllGundan::cmdNeedKoudouValueKougi					= CMD_NEED_KOUDOUVALUE_KOUGI_DEFAULT;
// �u��I�v�ɕK�v�ȍs���l
int  TSModCommand::AllGundan::cmdNeedKoudouValueHirou					= CMD_NEED_KOUDOUVALUE_HIROU_DEFAULT;
// �u�Ґ��v�ɕK�v�ȍs���l
int  TSModCommand::AllGundan::cmdNeedKoudouValueHensei					= CMD_NEED_KOUDOUVALUE_HENSEI_DEFAULT;
// �u�����v�ɕK�v�ȍs���l
int  TSModCommand::AllGundan::cmdNeedKoudouValueChohei					= CMD_NEED_KOUDOUVALUE_CHOHEI_DEFAULT;
// �u�o�p�v�ɕK�v�ȍs���l
int  TSModCommand::AllGundan::cmdNeedKoudouValueTouyou					= CMD_NEED_KOUDOUVALUE_TOUYOU_DEFAULT;
// �u�J���v�ɕK�v�ȍs���l
int  TSModCommand::AllGundan::cmdNeedKoudouValueHoubi					= CMD_NEED_KOUDOUVALUE_HOUBI_DEFAULT;
// �u�����v�ɕK�v�ȍs���l
int  TSModCommand::AllGundan::cmdNeedKoudouValueBusshi					= CMD_NEED_KOUDOUVALUE_BUSSHI_DEFAULT;
// �u�푈�v�ɕK�v�ȍs���l
int  TSModCommand::AllGundan::cmdNeedKoudouValueSensou					= CMD_NEED_KOUDOUVALUE_SENSOU_DEFAULT;
// �u�����v�ɕK�v�ȍs���l
int  TSModCommand::AllGundan::cmdNeedKoudouValueDoumei					= CMD_NEED_KOUDOUVALUE_DOUMEI_DEFAULT;
// �u�����v�ɕK�v�ȍs���l
int  TSModCommand::AllGundan::cmdNeedKoudouValueKonin					= CMD_NEED_KOUDOUVALUE_KONIN_DEFAULT;
// �u��؂�v�ɕK�v�ȍs���l
int  TSModCommand::AllGundan::cmdNeedKoudouValueTegire					= CMD_NEED_KOUDOUVALUE_TEGIRE_DEFAULT;
// �u�����v�ɕK�v�ȍs���l
int  TSModCommand::AllGundan::cmdNeedKoudouValueKyohaku					= CMD_NEED_KOUDOUVALUE_KYOHAKU_DEFAULT;
// �u����v�ɕK�v�ȍs���l
int  TSModCommand::AllGundan::cmdNeedKoudouValueChotei					= CMD_NEED_KOUDOUVALUE_CHOTEI_DEFAULT;
// �u�����v�ɕK�v�ȍs���l
int  TSModCommand::AllGundan::cmdNeedKoudouValueNaiou					= CMD_NEED_KOUDOUVALUE_NAIOU_DEFAULT;
// �u�d���v�ɕK�v�ȍs���l
int  TSModCommand::AllGundan::cmdNeedKoudouValueBouryaku				= CMD_NEED_KOUDOUVALUE_BOURYAKU_DEFAULT;
// �u�]��v�ɕK�v�ȍs���l
int  TSModCommand::AllGundan::cmdNeedKoudouValueHyoujo					= CMD_NEED_KOUDOUVALUE_HYOUJO_DEFAULT;
// �u�R�c�v���u�C���v�ɕK�v�ȍs���l
int  TSModCommand::AllGundan::cmdNeedKoudouValueGundanNinmei			= CMD_NEED_KOUDOUVALUE_GUNDAN_NINMEI_DEFAULT;
// �u�R�c�v=>���z�v�u�����v�u�ĔC�v�ɕK�v�ȍs���l
int  TSModCommand::AllGundan::cmdNeedKoudouValueGundanSubCmd			= CMD_NEED_KOUDOUVALUE_GUNDAN_SUBCMD_DEFAULT;
// �u�����E�P���E���i�v�ɕK�v�ȍs���l
int  TSModCommand::AllGundan::cmdNeedKoudouValue5Ningumi				= CMD_NEED_KOUDOUVALUE_5NINGUMI_DEFAULT;


int TSModCommand::AllDaimyo::cmdSeiiTaishogunNeedCastleNum				= (int)0;
int TSModCommand::AllDaimyo::cmdTheSengokouPeriodIsEndless				= (int)0;
int TSModCommand::MyselfDaimyo::cmdGameEndConditionIsGettingPosition	= (int)0;
int TSModCommand::MyselfDaimyo::cmdCanInstructDirectOtherGundan			= (int)0; // ��Q�R�c�ȉ��ɒ��ڎw�����\�B
int TSModCommand::MyselfDaimyo::cmdCanMultiHyoujoInSeason				= (int)0;
int TSModCommand::OtherDaimyo::cmdNeverThreat							= (int)0; // �b�o�t�͋����R�}���h�����s���Ȃ��B

int TSModCommand::AllCastle::cmdIncreaseRatioCanDraftSoldier			= (int)0; // �����\����������
int TSModCommand::AllCastle::cmdCastleDefenceCoefOfNPC					= (int)0; // �m�o�b�퓬���̏�h��W��
int TSModCommand::AllCastle::cmdShortCutFallCastle						= (int)0;



int TSModCommand::OtherBushou::cmdCanUseAllAbilityInBattle				= (int)0;

int TSModCommand::AllBushou::cmdNinjaMustBeAShadow						= (int)0;
int TSModCommand::AllBushou::cmdNinjaNotHaveAWayOfPromotion				= (int)0;
int TSModCommand::AllBushou::cmdPositionInfluenceSoldierMax				= (int)0;
int TSModCommand::AllBushou::cmdExtendedJob								= (int)0;
int TSModCommand::AllBushou::cmdNoDeclineTalentParam					= (int)0;		// �������V��Ă��\�͂��������Ȃ�
int TSModCommand::AllBushou::cmdBraveIsTheValue							= -1; // �E�҂�0�`7�܂ŗL���Ȃ̂Łc
int TSModCommand::AllBushou::cmdLifeIsTheValue							= -1; // ������0�`7�܂ŗL���Ȃ̂Łc
int TSModCommand::AllBushou::cmdAbilityIsUnderdeveloped					= (int)0;
int TSModCommand::AllBushou::cmdFixExperienceMax						= (int)0; // ���������n�E��n�E�q�n���l�`�w�ł������Ȃ�c
int TSModCommand::AllBushou::cmdAutoUpExperienceAndTech					= (int)0; // �n���x�����b�B�����邩�ǂ���
int TSModCommand::AllBushou::cmdNotGrowUpSuddenly						= (int)0; // �}�ɂ͐������Ȃ��B
int TSModCommand::AllBushou::cmdLifeIsTheTimes							= (int)0;
int TSModCommand::AllBushou::cmdNoDutiesAndMaxIndependences				= (int)0;
int TSModCommand::AllBushou::cmdAntiAssassinationInBattle				= (int)0;
int TSModCommand::AllBushou::cmdAffinityWithSuperiorInfluenceLoyalty	= (int)0;
int TSModCommand::AllBushou::cmdSetWorkToDieEverySeason					= (int)0;
int TSModCommand::AllBushou::cmdAppearanceHimeBecomeBushou				= (int)0; // �b�o�t�o��P���P�����ɂ���B
int TSModCommand::AllBushou::cmdNonFaceBushouIndependences				= (int)0; // ��Ȃ����������g�����邩�ۂ��B
int TSModCommand::AllBushou::cmdNonMeritoriousReset						= (int)0; // �M�������Z�b�g���邩�B


int TSModCommand::AllBushou::cmdSuppressRiseOfExpValueInBattle			= (int)0;
int TSModCommand::AllBushou::cmdParamConsiderNobunagaSeries				= (int)0;

int TSModCommand::AllBushou::cmdSoldierBaseMaxOfDaimy					= (int)0;	
int TSModCommand::AllBushou::cmdSoldierBaseMaxOfSyuku					= (int)0;
int TSModCommand::AllBushou::cmdSoldierBaseMaxOfKarou					= (int)0;
int TSModCommand::AllBushou::cmdSoldierBaseMaxOfBusyo					= (int)0;
int TSModCommand::AllBushou::cmdSoldierBaseMaxOfSamra					= (int)0;	
int TSModCommand::AllBushou::cmdSoldierBaseMaxOfAshig					= (int)0;	

int TSModCommand::AllBushou::cmdReceiveDamageOfBreakDoor				= (int)0; // ����󂷍ۂɃ_���[�W���󂯂邩�ǂ����B
int TSModCommand::AllBushou::cmdDeathFrequencyNotInfluenceLife			= (int)0; // ���S�p�x�̎����ւ̉e���͂��J�b�g����B�f�t�H���g�ł́B�e�����Ă���B(=�J�b�g���Ȃ�)
int TSModCommand::AllBushou::cmdDegreeLoyaltyOnCaptiveLibra				= (int)0; // ����������ɉƐb�̒����x���~

int TSModCommand::UltraBushou::cmdExists								= (int)0;

int TSModCommand::Environment::cmdWideResolution						= (int)0;
int TSModCommand::Environment::cmdMobileResolution						= (int)0;

int TSModCommand::Environment::cmdBGMMode								= (int)0;

int TSModCommand::Environment::cmdNoScreenSaverInDemo					= (int)0;

int TSModCommand::Environment::cmdNoTitleBarAndMenuBar					= (int)0;

int TSModCommand::Environment::cmdInternetConnectPermission				= (int)0; // �C���^�[�l�b�g�ւ̐ڑ��̋���

int TSModCommand::Debug::cmdDebugWindowOpen								= (int)0;

int TSModCommand::gurdian2[1000] = {};


map<string, int*> AllTSModTxtFlags;

// �ו��Q�Ɨp
int WINAPI Extern_getTSModTxtFlag(char *szflagname) {

    if (AllTSModTxtFlags.find(szflagname) == AllTSModTxtFlags.end()) {
        // �L�[�� "Key" �̗v�f���Ȃ��ꍇ�̏���
		return -0xFFFF;
    } else {
        // �L�[�� "Key" �̗v�f������ꍇ�̏���
		// �}�b�v�̓t���O�ւ̃|�C���^�������Ă���̂ŁA�f���t�@�����X(�]���l)��Ԃ�
		return *(AllTSModTxtFlags[szflagname]);
    }
}



// TSMod.txt��:cmd:�n�̃X�C�b�`�𔽉f������B
void TSModCommand::setCommandSwitch(char *szCurCmdTokenBuf, int byteCurCmdLv) {

	// ���Α叫�R�ɂȂ�̂ɕK�v�ȏ�̐��̕ύX
	if( strcmp(szCurCmdTokenBuf, "all_daimyo_seiitaishogun_need_castle_num")==0 ) {
		// LV �� MIN, MAX �I�[�o�[�t���[����
		if (byteCurCmdLv < 0 ) { byteCurCmdLv = 0; }
		if (byteCurCmdLv > 214 ) { byteCurCmdLv = 214; }

		TSModCommand::AllDaimyo::cmdSeiiTaishogunNeedCastleNum = byteCurCmdLv;
		AllTSModTxtFlags["all_daimyo_seiitaishogun_need_castle_num"] = &TSModCommand::AllDaimyo::cmdSeiiTaishogunNeedCastleNum;

	// ���������ׂĂ̕����̗E�Ғl������̒l�ł������Ȃ�c
	} else if (strcmp(szCurCmdTokenBuf, "all_bushou_brave_is_the_value")==0 ) {
		// LV �� MIN, MAX �I�[�o�[�t���[����
		if (byteCurCmdLv < 0 ) { byteCurCmdLv = 0; }
		if (byteCurCmdLv > 7 ) { byteCurCmdLv = 7; }

		TSModCommand::AllBushou::cmdBraveIsTheValue = byteCurCmdLv;
		AllTSModTxtFlags["all_bushou_brave_is_the_value"] = &TSModCommand::AllBushou::cmdBraveIsTheValue;

	// ���������ׂĂ̕����̎���������̒l�ł������Ȃ�c
	} else if (strcmp(szCurCmdTokenBuf, "all_bushou_life_is_the_value")==0 ) {
		// LV �� MIN, MAX �I�[�o�[�t���[����
		if (byteCurCmdLv < 0 ) { byteCurCmdLv = 0; }
		if (byteCurCmdLv > 7 ) { byteCurCmdLv = 7; }

		TSModCommand::AllBushou::cmdLifeIsTheValue = byteCurCmdLv;
		AllTSModTxtFlags["all_bushou_life_is_the_value"] = &TSModCommand::AllBushou::cmdLifeIsTheValue;

	// �����\�����������B
	} else if (strcmp(szCurCmdTokenBuf, "all_castle_increase_ratio_can_draft_soldier")==0 ) {
		if (byteCurCmdLv < 0 ) { byteCurCmdLv = 0; }
		if (byteCurCmdLv > 10 ) { byteCurCmdLv = 10; }

		TSModCommand::AllCastle::cmdIncreaseRatioCanDraftSoldier = byteCurCmdLv;
		AllTSModTxtFlags["all_castle_increase_ratio_can_draft_soldier"] = &TSModCommand::AllCastle::cmdIncreaseRatioCanDraftSoldier;

	// �����͂̓��Y�i�Ɋ֘A�������ڂ������I�ɑ�����(�S�C�E�n�E���ۍ`)
	} else if (strcmp(szCurCmdTokenBuf, "myself_gundan_increase_special_product_by_the_thing")==0 ) {
		if (byteCurCmdLv < 0 ) { byteCurCmdLv = 0; }
		if (byteCurCmdLv > 10 ) { byteCurCmdLv = 10; }
		TSModCommand::MyselfGundan::cmdIncreaseSpecialProductByTheThing = byteCurCmdLv;
		AllTSModTxtFlags["myself_gundan_increase_special_product_by_the_thing"] = &TSModCommand::MyselfGundan::cmdIncreaseSpecialProductByTheThing;

	// �����͂̓��Y�i�Ɋ֘A�������ڂ������I�ɑ�����(�S�C�E�n�E���ۍ`)
	} else if (strcmp(szCurCmdTokenBuf, "others_gundan_increase_special_product_by_the_thing")==0 ) {
		if (byteCurCmdLv < 0 ) { byteCurCmdLv = 0; }
		if (byteCurCmdLv > 10 ) { byteCurCmdLv = 10; }
		TSModCommand::OthersGundan::cmdIncreaseSpecialProductByTheThing = byteCurCmdLv;
		AllTSModTxtFlags["others_gundan_increase_special_product_by_the_thing"] = &TSModCommand::OthersGundan::cmdIncreaseSpecialProductByTheThing;


	// �n���x��Z�\�̎����b�B����
	} else if (strcmp(szCurCmdTokenBuf, "all_bushou_autoup_experience_and_tech")==0 ) {
		if (byteCurCmdLv < 0 ) { byteCurCmdLv = 0; }
		if (byteCurCmdLv > 10 ) { byteCurCmdLv = 10; }
		TSModCommand::AllBushou::cmdAutoUpExperienceAndTech = byteCurCmdLv;
		AllTSModTxtFlags["all_bushou_autoup_experience_and_tech"] = &TSModCommand::AllBushou::cmdAutoUpExperienceAndTech;

	// �喼�̕��m��
	} else if (strcmp(szCurCmdTokenBuf, "all_bushou_soldier_base_maxof_daimy")==0 ) {
		if (byteCurCmdLv > 100 ) { byteCurCmdLv = 100; }
		TSModCommand::AllBushou::cmdSoldierBaseMaxOfDaimy = byteCurCmdLv;
		AllTSModTxtFlags["all_bushou_soldier_base_maxof_daimy"] = &TSModCommand::AllBushou::cmdSoldierBaseMaxOfDaimy;
	// �h�V�̕��m��
	} else if (strcmp(szCurCmdTokenBuf, "all_bushou_soldier_base_maxof_syuku")==0 ) {
		if (byteCurCmdLv > 100 ) { byteCurCmdLv = 100; }
		TSModCommand::AllBushou::cmdSoldierBaseMaxOfSyuku = byteCurCmdLv;
		AllTSModTxtFlags["all_bushou_soldier_base_maxof_syuku"] = &TSModCommand::AllBushou::cmdSoldierBaseMaxOfSyuku;
	// �ƘV�̕��m��
	} else if (strcmp(szCurCmdTokenBuf, "all_bushou_soldier_base_maxof_karou")==0 ) {
		if (byteCurCmdLv > 100 ) { byteCurCmdLv = 100; }
		TSModCommand::AllBushou::cmdSoldierBaseMaxOfKarou = byteCurCmdLv;
		AllTSModTxtFlags["all_bushou_soldier_base_maxof_karou"] = &TSModCommand::AllBushou::cmdSoldierBaseMaxOfKarou;
	// �����̕��m��
	} else if (strcmp(szCurCmdTokenBuf, "all_bushou_soldier_base_maxof_busyo")==0 ) {
		if (byteCurCmdLv > 100 ) { byteCurCmdLv = 100; }
		TSModCommand::AllBushou::cmdSoldierBaseMaxOfBusyo = byteCurCmdLv;
		AllTSModTxtFlags["all_bushou_soldier_base_maxof_busyo"] = &TSModCommand::AllBushou::cmdSoldierBaseMaxOfBusyo;
	// ���叫�̕��m��
	} else if (strcmp(szCurCmdTokenBuf, "all_bushou_soldier_base_maxof_samra")==0 ) {
		if (byteCurCmdLv > 100 ) { byteCurCmdLv = 100; }
		TSModCommand::AllBushou::cmdSoldierBaseMaxOfSamra = byteCurCmdLv;
		AllTSModTxtFlags["all_bushou_soldier_base_maxof_samra"] = &TSModCommand::AllBushou::cmdSoldierBaseMaxOfSamra;
	// ���y���̕��m��
	} else if (strcmp(szCurCmdTokenBuf, "all_bushou_soldier_base_maxof_ashig")==0 ) {
		if (byteCurCmdLv > 100 ) { byteCurCmdLv = 100; }
		TSModCommand::AllBushou::cmdSoldierBaseMaxOfAshig = byteCurCmdLv;
		AllTSModTxtFlags["all_bushou_soldier_base_maxof_ashig"] = &TSModCommand::AllBushou::cmdSoldierBaseMaxOfAshig;

	} else if ( strstr(szCurCmdTokenBuf,  "all_gundan_need_koudou_value" ) ) {
		if (byteCurCmdLv < 1  )  { byteCurCmdLv = 1; }
		if (byteCurCmdLv > 100 ) { byteCurCmdLv = 100; }

		// �u�`�̍s����
		if ( strcmp(szCurCmdTokenBuf, "all_gundan_need_koudou_value_kougii")==0 ) {
			TSModCommand::AllGundan::cmdNeedKoudouValueKougi = byteCurCmdLv;
			AllTSModTxtFlags["all_gundan_need_koudou_value_kougii"] = &TSModCommand::AllGundan::cmdNeedKoudouValueKougi;
		} else if ( strcmp(szCurCmdTokenBuf, "all_gundan_need_koudou_value_hirouu")==0 ) {
			TSModCommand::AllGundan::cmdNeedKoudouValueHirou = byteCurCmdLv;
			AllTSModTxtFlags["all_gundan_need_koudou_value_hirouu"] = &TSModCommand::AllGundan::cmdNeedKoudouValueHirou;
		} else if ( strcmp(szCurCmdTokenBuf, "all_gundan_need_koudou_value_hensei")==0 ) {
			TSModCommand::AllGundan::cmdNeedKoudouValueHensei = byteCurCmdLv;
			AllTSModTxtFlags["all_gundan_need_koudou_value_hensei"] = &TSModCommand::AllGundan::cmdNeedKoudouValueHensei;
		} else if ( strcmp(szCurCmdTokenBuf, "all_gundan_need_koudou_value_chohei")==0 ) {
			TSModCommand::AllGundan::cmdNeedKoudouValueChohei = byteCurCmdLv;
			AllTSModTxtFlags["all_gundan_need_koudou_value_chohei"] = &TSModCommand::AllGundan::cmdNeedKoudouValueChohei;
		} else if ( strcmp(szCurCmdTokenBuf, "all_gundan_need_koudou_value_touyou")==0 ) {
			TSModCommand::AllGundan::cmdNeedKoudouValueTouyou = byteCurCmdLv;
			AllTSModTxtFlags["all_gundan_need_koudou_value_touyou"] = &TSModCommand::AllGundan::cmdNeedKoudouValueTouyou;
		} else if ( strcmp(szCurCmdTokenBuf, "all_gundan_need_koudou_value_houbii")==0 ) {
			TSModCommand::AllGundan::cmdNeedKoudouValueHoubi = byteCurCmdLv;
			AllTSModTxtFlags["all_gundan_need_koudou_value_houbii"] = &TSModCommand::AllGundan::cmdNeedKoudouValueHoubi;
		} else if ( strcmp(szCurCmdTokenBuf, "all_gundan_need_koudou_value_busshi")==0 ) {
			TSModCommand::AllGundan::cmdNeedKoudouValueBusshi = byteCurCmdLv;
			AllTSModTxtFlags["all_gundan_need_koudou_value_busshi"] = &TSModCommand::AllGundan::cmdNeedKoudouValueBusshi;
		} else if ( strcmp(szCurCmdTokenBuf, "all_gundan_need_koudou_value_sensou")==0 ) {
			TSModCommand::AllGundan::cmdNeedKoudouValueSensou = byteCurCmdLv;
			AllTSModTxtFlags["all_gundan_need_koudou_value_sensou"] = &TSModCommand::AllGundan::cmdNeedKoudouValueSensou;
		} else if ( strcmp(szCurCmdTokenBuf, "all_gundan_need_koudou_value_doumei")==0 ) {
			TSModCommand::AllGundan::cmdNeedKoudouValueDoumei = byteCurCmdLv;
			AllTSModTxtFlags["all_gundan_need_koudou_value_doumei"] = &TSModCommand::AllGundan::cmdNeedKoudouValueDoumei;
		} else if ( strcmp(szCurCmdTokenBuf, "all_gundan_need_koudou_value_koninn")==0 ) {
			TSModCommand::AllGundan::cmdNeedKoudouValueKonin = byteCurCmdLv;
			AllTSModTxtFlags["all_gundan_need_koudou_value_koninn"] = &TSModCommand::AllGundan::cmdNeedKoudouValueKonin;
		} else if ( strcmp(szCurCmdTokenBuf, "all_gundan_need_koudou_value_tegire")==0 ) {
			TSModCommand::AllGundan::cmdNeedKoudouValueTegire = byteCurCmdLv;
			AllTSModTxtFlags["all_gundan_need_koudou_value_tegire"] = &TSModCommand::AllGundan::cmdNeedKoudouValueTegire;
		} else if ( strcmp(szCurCmdTokenBuf, "all_gundan_need_koudou_value_kyohak")==0 ) {
			TSModCommand::AllGundan::cmdNeedKoudouValueKyohaku = byteCurCmdLv;
			AllTSModTxtFlags["all_gundan_need_koudou_value_kyohak"] = &TSModCommand::AllGundan::cmdNeedKoudouValueKyohaku;
		} else if ( strcmp(szCurCmdTokenBuf, "all_gundan_need_koudou_value_chotei")==0 ) {
			TSModCommand::AllGundan::cmdNeedKoudouValueChotei = byteCurCmdLv;
			AllTSModTxtFlags["all_gundan_need_koudou_value_chotei"] = &TSModCommand::AllGundan::cmdNeedKoudouValueChotei;
		} else if ( strcmp(szCurCmdTokenBuf, "all_gundan_need_koudou_value_naiouu")==0 ) {
			TSModCommand::AllGundan::cmdNeedKoudouValueNaiou = byteCurCmdLv;
			AllTSModTxtFlags["all_gundan_need_koudou_value_naiouu"] = &TSModCommand::AllGundan::cmdNeedKoudouValueNaiou;
		} else if ( strcmp(szCurCmdTokenBuf, "all_gundan_need_koudou_value_bourya")==0 ) {
			TSModCommand::AllGundan::cmdNeedKoudouValueBouryaku = byteCurCmdLv;
			AllTSModTxtFlags["all_gundan_need_koudou_value_bourya"] = &TSModCommand::AllGundan::cmdNeedKoudouValueBouryaku;
		} else if ( strcmp(szCurCmdTokenBuf, "all_gundan_need_koudou_value_hyoujo")==0 ) {
			TSModCommand::AllGundan::cmdNeedKoudouValueHyoujo = byteCurCmdLv;
			AllTSModTxtFlags["all_gundan_need_koudou_value_hyoujo"] = &TSModCommand::AllGundan::cmdNeedKoudouValueHyoujo;
		} else if ( strcmp(szCurCmdTokenBuf, "all_gundan_need_koudou_value_gunnin")==0 ) {
			TSModCommand::AllGundan::cmdNeedKoudouValueGundanNinmei = byteCurCmdLv;
			AllTSModTxtFlags["all_gundan_need_koudou_value_gunnin"] = &TSModCommand::AllGundan::cmdNeedKoudouValueGundanNinmei;
		} else if ( strcmp(szCurCmdTokenBuf, "all_gundan_need_koudou_value_gunsub")==0 ) {
			TSModCommand::AllGundan::cmdNeedKoudouValueGundanSubCmd = byteCurCmdLv;
			AllTSModTxtFlags["all_gundan_need_koudou_value_gunsub"] = &TSModCommand::AllGundan::cmdNeedKoudouValueGundanSubCmd;
		} else if ( strcmp(szCurCmdTokenBuf, "all_gundan_need_koudou_value_5ningu")==0 ) {
			TSModCommand::AllGundan::cmdNeedKoudouValue5Ningumi = byteCurCmdLv;
			AllTSModTxtFlags["all_gundan_need_koudou_value_5ningu"] = &TSModCommand::AllGundan::cmdNeedKoudouValue5Ningumi;
		}

	} else {

		// LV �� MIN, MAX �I�[�o�[�t���[����
		if (byteCurCmdLv < 0 ) { byteCurCmdLv = 0; }
		if (byteCurCmdLv > 5 ) { byteCurCmdLv = 5; }

		// �����͂ɋ���Ă����Z����R�}���h
		if (strcmp(szCurCmdTokenBuf, "myself_gundan_get_money_and_rise_on_season_change")==0 ) {
			TSModCommand::MyselfGundan::cmdGetMoneyAndRiseOnSeasonChangeLv = byteCurCmdLv;
			AllTSModTxtFlags["myself_gundan_get_money_and_rise_on_season_change"] = &TSModCommand::MyselfGundan::cmdGetMoneyAndRiseOnSeasonChangeLv;

		// �����͂ɋ���Ă����Z����R�}���h
		} else if (strcmp(szCurCmdTokenBuf, "others_gundan_get_money_and_rise_on_season_change")==0 ) {
			TSModCommand::OthersGundan::cmdGetMoneyAndRiseOnSeasonChangeLv = byteCurCmdLv;
			AllTSModTxtFlags["others_gundan_get_money_and_rise_on_season_change"] = &TSModCommand::OthersGundan::cmdGetMoneyAndRiseOnSeasonChangeLv;

		// �����͂̕��m�������񕜂���R�}���h
		} else if (strcmp(szCurCmdTokenBuf, "myself_gundan_regeneration_soldier_automatically")==0 ) {
			TSModCommand::MyselfGundan::cmdRegenerationSoldierAutomatically = byteCurCmdLv;
			AllTSModTxtFlags["myself_gundan_regeneration_soldier_automatically"] = &TSModCommand::MyselfGundan::cmdRegenerationSoldierAutomatically;

		// �����͂̕��m�������񕜂���R�}���h
		} else if (strcmp(szCurCmdTokenBuf, "others_gundan_regeneration_soldier_automatically")==0 ) {
			TSModCommand::OthersGundan::cmdRegenerationSoldierAutomatically = byteCurCmdLv;
			AllTSModTxtFlags["others_gundan_regeneration_soldier_automatically"] = &TSModCommand::OthersGundan::cmdRegenerationSoldierAutomatically;

		// �v���C���̐i���ƁA�v���C���ȊO�̑S�̂̌R�c�̐i�������A�v���C���ȊO�̌R�c�݂͌��Ɏ����o�����X����낤�Ƃ��܂��B
		} else if (strcmp(szCurCmdTokenBuf, "others_gundan_player_and_the_whole_influence_each_other_data")==0 ) {
			TSModCommand::OthersGundan::cmdPlayerAndTheWholeInfluenceEachOtherData = byteCurCmdLv;
			AllTSModTxtFlags["others_gundan_player_and_the_whole_influence_each_other_data"] = &TSModCommand::OthersGundan::cmdPlayerAndTheWholeInfluenceEachOtherData;

		// �퓬�}�b�v�𢌩�Ă��Ȃ����ԂŐ푈���N�������A�����̏n���x���ߓx�ɏ㏸���邱�Ƃɂ��퓬�l��q�d�l�̃C���t����h���܂��B
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_suppress_rise_of_exp_value_in_battle")==0 ) {
			TSModCommand::AllBushou::cmdSuppressRiseOfExpValueInBattle = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_suppress_rise_of_exp_value_in_battle"] = &TSModCommand::AllBushou::cmdSuppressRiseOfExpValueInBattle;

		// �o�w���͏��8���ۏႳ��Ă���B
		} else if ( strcmp(szCurCmdTokenBuf, "all_gundan_the_number_of_the_max_go_to_war_fix")==0 ) {
			// LV �� MIN, MAX �I�[�o�[�t���[����
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllGundan::cmdTheNumberOfTheMaxGoToWarFix = byteCurCmdLv;
			AllTSModTxtFlags["all_gundan_the_number_of_the_max_go_to_war_fix"] = &TSModCommand::AllGundan::cmdTheNumberOfTheMaxGoToWarFix;

		// �������S�Ă̕����̍΂����̂��x���Ȃ����Ȃ�c
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_life_is_the_times")==0 ) {
			TSModCommand::AllBushou::cmdLifeIsTheTimes = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_life_is_the_times"] = &TSModCommand::AllBushou::cmdLifeIsTheTimes;

		// �������S�Ă̕������V��Ă��\�͂��������Ȃ�
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_no_decline_talent_param")==0 ) {
			TSModCommand::AllBushou::cmdNoDeclineTalentParam = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_no_decline_talent_param"] = &TSModCommand::AllBushou::cmdNoDeclineTalentParam;

		// �����͋}�ɂ͐������Ȃ��B
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_not_grow_up_suddenly")==0 ) {
			TSModCommand::AllBushou::cmdNotGrowUpSuddenly = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_not_grow_up_suddenly"] = &TSModCommand::AllBushou::cmdNotGrowUpSuddenly;

		// �E�҂͉e������̂ł���ׂ�(�E�҂̕��m����1/5�̐���������)
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_ninja_must_be_a_shadow")==0 ) {
			if (byteCurCmdLv > 3 ) { byteCurCmdLv = 3; }
			TSModCommand::AllBushou::cmdNinjaMustBeAShadow = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_ninja_must_be_a_shadow"] = &TSModCommand::AllBushou::cmdNinjaMustBeAShadow;

		// �E�҂͌M�����㏸�����A�o���ł��Ȃ�
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_ninja_not_have_a_way_of_promotion")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllBushou::cmdNinjaNotHaveAWayOfPromotion = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_ninja_not_have_a_way_of_promotion"] = &TSModCommand::AllBushou::cmdNinjaNotHaveAWayOfPromotion;

		// ���ʂ��E������΁A���ꂪ���m�̍ő吔�̑����Ɍq����
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_position_influence_soldier_max")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllBushou::cmdPositionInfluenceSoldierMax = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_position_influence_soldier_max"] = &TSModCommand::AllBushou::cmdPositionInfluenceSoldierMax;

		// �g�����ꂽ�E��(�o��)�𗘗p����B
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_extended_job")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllBushou::cmdExtendedJob = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_extended_job"] = &TSModCommand::AllBushou::cmdExtendedJob;

		// �Z�\�����J���ł������Ȃ�
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_ability_is_underdeveloped")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllBushou::cmdAbilityIsUnderdeveloped = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_ability_is_underdeveloped"] = &TSModCommand::AllBushou::cmdAbilityIsUnderdeveloped;

		// ���������n�E��n�E�q�n���l�`�w�ł������Ȃ�c
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_fix_experience_max")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllBushou::cmdFixExperienceMax = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_fix_experience_max"] = &TSModCommand::AllBushou::cmdFixExperienceMax;

		// �����̔\�͂͗��V���[�Y���l���������̂ł���B
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_param_consider_nobunaga_series")==0 ) {
			if (byteCurCmdLv > 2 ) { byteCurCmdLv = 2; }
			TSModCommand::AllBushou::cmdParamConsiderNobunagaSeries = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_param_consider_nobunaga_series"] = &TSModCommand::AllBushou::cmdParamConsiderNobunagaSeries;
			
		// �S�Ă̕����ɋ`���ȂǂȂ��A�Ɨ�����ł������Ȃ�c
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_noduty_and_maxindependence")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllBushou::cmdNoDutiesAndMaxIndependences = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_noduty_and_maxindependence"] = &TSModCommand::AllBushou::cmdNoDutiesAndMaxIndependences;

		// �S�Ă̕����͈ÎE�Ԃ������邱�Ƃ�����B
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_anti_assassination_in_battle")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllBushou::cmdAntiAssassinationInBattle = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_anti_assassination_in_battle"] = &TSModCommand::AllBushou::cmdAntiAssassinationInBattle;

		// �S�Ă̕����́A���G�߁A�㒷�Ƃ̑����̉e�����󂯂āA�ω�����B
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_affinity_with_superior_influence_loyalty")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllBushou::cmdAffinityWithSuperiorInfluenceLoyalty = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_affinity_with_superior_influence_loyalty"] = &TSModCommand::AllBushou::cmdAffinityWithSuperiorInfluenceLoyalty;
		
		// (���S�p�x�Ɋ֌W�Ȃ�)���G�߁A�����̎��S�������s����B
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_set_work_to_die_every_season")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllBushou::cmdSetWorkToDieEverySeason = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_set_work_to_die_every_season"] = &TSModCommand::AllBushou::cmdSetWorkToDieEverySeason;

		// �b�o�t�o��P���P�����ɂ���B
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_appearance_hime_become_bushou")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllBushou::cmdAppearanceHimeBecomeBushou = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_appearance_hime_become_bushou"] = &TSModCommand::AllBushou::cmdAppearanceHimeBecomeBushou;

		// ��Ȃ����������g�����邩�ۂ��B
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_non_face_bushou_independences")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllBushou::cmdNonFaceBushouIndependences = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_non_face_bushou_independences"] = &TSModCommand::AllBushou::cmdNonFaceBushouIndependences;

		// ��Ȃ����������g�����邩�ۂ��B
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_non_meritorious_reset")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllBushou::cmdNonMeritoriousReset = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_non_meritorious_reset"] = &TSModCommand::AllBushou::cmdNonMeritoriousReset;

		// ����V���[�g�J�b�g�������L���Ȃ��
		} else if (strcmp(szCurCmdTokenBuf, "all_castle_shortcut_fall_castle")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllCastle::cmdShortCutFallCastle = byteCurCmdLv;
			AllTSModTxtFlags["all_castle_shortcut_fall_castle"] = &TSModCommand::AllCastle::cmdShortCutFallCastle;

		// ����󂷍ۂɃ_���[�W���󂯂�Ȃ��
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_receive_damage_of_break_door")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllBushou::cmdReceiveDamageOfBreakDoor = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_receive_damage_of_break_door"] = &TSModCommand::AllBushou::cmdReceiveDamageOfBreakDoor;

		// �m�o�b�퓬���̏�h��W��
		} else if (strcmp(szCurCmdTokenBuf, "all_castle_castle_defence_coef_of_npc")==0 ) {
			if (byteCurCmdLv > 7 ) { byteCurCmdLv = 7; }
			TSModCommand::AllCastle::cmdCastleDefenceCoefOfNPC = byteCurCmdLv;
			AllTSModTxtFlags["all_castle_castle_defence_coef_of_npc"] = &TSModCommand::AllCastle::cmdCastleDefenceCoefOfNPC;

		// ���S�p�x�̎����ւ̉e���͂��J�b�g����B�f�t�H���g�ł́B�e�����Ă���B(=�J�b�g���Ȃ�)
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_death_frequency_not_influence_life")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllBushou::cmdDeathFrequencyNotInfluenceLife = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_death_frequency_not_influence_life"] = &TSModCommand::AllBushou::cmdDeathFrequencyNotInfluenceLife;

		// �m�o�b���͂͋����R�}���h�����s���Ȃ��B
		} else if (strcmp(szCurCmdTokenBuf, "all_other_daimyo_never_thread")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::OtherDaimyo::cmdNeverThreat = byteCurCmdLv;
			AllTSModTxtFlags["all_other_daimyo_never_thread"] = &TSModCommand::OtherDaimyo::cmdNeverThreat;

		// �����͂̑S�Ă̕����́A�퓬���S�Ă̋Z�\���g������
		} else if (strcmp(szCurCmdTokenBuf, "other_bushou_can_use_all_ability_in_battle")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::OtherBushou::cmdCanUseAllAbilityInBattle = byteCurCmdLv;
			AllTSModTxtFlags["other_bushou_can_use_all_ability_in_battle"] = &TSModCommand::OtherBushou::cmdCanUseAllAbilityInBattle;

		// �Q�[���I��������ύX���A��E�A�C�܂łƂ���B
		} else if (strcmp(szCurCmdTokenBuf, "myself_daimyo_game_end_condition_is_getting_position")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::MyselfDaimyo::cmdGameEndConditionIsGettingPosition = byteCurCmdLv;
			AllTSModTxtFlags["myself_daimyo_game_end_condition_is_getting_position"] = &TSModCommand::MyselfDaimyo::cmdGameEndConditionIsGettingPosition;

		// ��Q�R�c�ȉ��ɒ��ڎw�����\�B
		} else if (strcmp(szCurCmdTokenBuf, "myself_daimyo_can_instruct_direct_other_gundan")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::MyselfDaimyo::cmdCanInstructDirectOtherGundan = byteCurCmdLv;
			AllTSModTxtFlags["myself_daimyo_can_instruct_direct_other_gundan"] = &TSModCommand::MyselfDaimyo::cmdCanInstructDirectOtherGundan;

		// ��G�߂ɕ�����]�肪�\�B
		} else if (strcmp(szCurCmdTokenBuf, "myself_daimyo_can_multi_hyoujo_in_season")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::MyselfDaimyo::cmdCanMultiHyoujoInSeason = byteCurCmdLv;
			AllTSModTxtFlags["myself_daimyo_can_multi_hyoujo_in_season"] = &TSModCommand::MyselfDaimyo::cmdCanMultiHyoujoInSeason;

		// ����������ɉƐb�̒����x���~�B
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_degree_loyalty_on_captive_libra")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllBushou::cmdDegreeLoyaltyOnCaptiveLibra = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_degree_loyalty_on_captive_libra"] = &TSModCommand::AllBushou::cmdDegreeLoyaltyOnCaptiveLibra;

		// �Q�[���I��������ύX���A�G���h���X�Ƃ���B
		} else if (strcmp(szCurCmdTokenBuf, "all_daimyo_the_sengoku_period_is_endless")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllDaimyo::cmdTheSengokouPeriodIsEndless = byteCurCmdLv;
			AllTSModTxtFlags["all_daimyo_the_sengoku_period_is_endless"] = &TSModCommand::AllDaimyo::cmdTheSengokouPeriodIsEndless;

		// �E���g������������Ȃ�΁c
		} else if (strcmp(szCurCmdTokenBuf, "ultra_bushou_exists")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::UltraBushou::cmdExists = byteCurCmdLv;
			AllTSModTxtFlags["ultra_bushou_exists"] = &TSModCommand::UltraBushou::cmdExists;

		// �𑜓x�����o�C���p�̎w��Ȃ�΁c
		} else if (strcmp(szCurCmdTokenBuf, "environment_wide_resolution")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::Environment::cmdWideResolution = byteCurCmdLv;
			AllTSModTxtFlags["environment_wide_resolution"] = &TSModCommand::Environment::cmdWideResolution;

			// �𑜓x�����o�C���p�̎w��Ȃ�΁c
		} else if (strcmp(szCurCmdTokenBuf, "environment_mobile_resolution")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::Environment::cmdMobileResolution = byteCurCmdLv;
			AllTSModTxtFlags["environment_mobile_resolution"] = &TSModCommand::Environment::cmdMobileResolution;

		// �a�f�l���[�h
		} else if (strcmp(szCurCmdTokenBuf, "environment_bgm_mode")==0 ) {
			TSModCommand::Environment::cmdBGMMode = byteCurCmdLv;
			AllTSModTxtFlags["environment_bgm_mode"] = &TSModCommand::Environment::cmdBGMMode;

		// �X�N���[���Z�C�o�[�N���h�~
		} else if (strcmp(szCurCmdTokenBuf, "environment_no_screensaver_in_demo")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::Environment::cmdNoScreenSaverInDemo = byteCurCmdLv;
			AllTSModTxtFlags["environment_no_screensaver_in_demo"] = &TSModCommand::Environment::cmdNoScreenSaverInDemo;

		// �^�C�g���o�[�g���j���[�o�[�͕s�v
		} else if (strcmp(szCurCmdTokenBuf, "environment_no_titlebar_and_menubar")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::Environment::cmdNoTitleBarAndMenuBar = byteCurCmdLv;
			AllTSModTxtFlags["environment_no_titlebar_and_menubar"] = &TSModCommand::Environment::cmdNoTitleBarAndMenuBar;
			
		// �C���^�[�l�b�g�ւ̐ڑ�����
		} else if (strcmp(szCurCmdTokenBuf, "environment_internet_connect_permission")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::Environment::cmdInternetConnectPermission = byteCurCmdLv;
			AllTSModTxtFlags["environment_internet_connect_permission"] = &TSModCommand::Environment::cmdInternetConnectPermission;

		// �f�o�b�O�E�B���h�E�̃I�[�v���w��Ȃ�΁c
		} else if (strcmp(szCurCmdTokenBuf, "debug_debug_window_open")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::Debug::cmdDebugWindowOpen = byteCurCmdLv;
			AllTSModTxtFlags["debug_debug_window_open"] = &TSModCommand::Debug::cmdDebugWindowOpen;

		// ���̐��ɓS�C�����݂��Ȃ������Ȃ�c
		} else if (strcmp(szCurCmdTokenBuf, "world_the_teppou_no_exists")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::World::cmdTheTeppouNoExists = byteCurCmdLv;
			AllTSModTxtFlags["world_the_teppou_no_exists"] = &TSModCommand::World::cmdTheTeppouNoExists;

		// ���̐��ɋ|�����݂��Ȃ������Ȃ�c
		} else if (strcmp(szCurCmdTokenBuf, "world_the_yumi_no_exists")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::World::cmdTheYumiNoExists = byteCurCmdLv;
			AllTSModTxtFlags["world_the_yumi_no_exists"] = &TSModCommand::World::cmdTheYumiNoExists;

		// ���z������o�ꂳ����Ȃ�c
		} else if (strcmp(szCurCmdTokenBuf, "world_virtual_bushou_appear")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::World::cmdVirtualBushouAppear = byteCurCmdLv;
			AllTSModTxtFlags["world_virtual_bushou_appear"] = &TSModCommand::World::cmdVirtualBushouAppear;
			
		// ���̐���.jp��p�̃J�X�^���ƕ󂪑��݂���Ȃ�΁c
		} else if (strcmp(szCurCmdTokenBuf, "world_custom_kahou_exists")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::World::cmdCustomKahouExists = byteCurCmdLv;
			AllTSModTxtFlags["world_custom_kahou_exists"] = &TSModCommand::World::cmdCustomKahouExists;
		}
	}
}
