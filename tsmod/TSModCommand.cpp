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
int TSModCommand::MyselfGundan::cmdIncreaseSpecialProductByTheThing		= (int)0; // 特産品に関連した項目が自動的に増える(鉄砲・馬・国際港)
int TSModCommand::OthersGundan::cmdIncreaseSpecialProductByTheThing		= (int)0; // 特産品に関連した項目が自動的に増える(鉄砲・馬・国際港)

int TSModCommand::OthersGundan::cmdPlayerAndTheWholeInfluenceEachOtherData = (int)0;
int TSModCommand::AllGundan::cmdTheNumberOfTheMaxGoToWarFix				= (int)0;

// 「講義」に必要な行動値
int  TSModCommand::AllGundan::cmdNeedKoudouValueKougi					= CMD_NEED_KOUDOUVALUE_KOUGI_DEFAULT;
// 「披露」に必要な行動値
int  TSModCommand::AllGundan::cmdNeedKoudouValueHirou					= CMD_NEED_KOUDOUVALUE_HIROU_DEFAULT;
// 「編成」に必要な行動値
int  TSModCommand::AllGundan::cmdNeedKoudouValueHensei					= CMD_NEED_KOUDOUVALUE_HENSEI_DEFAULT;
// 「徴兵」に必要な行動値
int  TSModCommand::AllGundan::cmdNeedKoudouValueChohei					= CMD_NEED_KOUDOUVALUE_CHOHEI_DEFAULT;
// 「登用」に必要な行動値
int  TSModCommand::AllGundan::cmdNeedKoudouValueTouyou					= CMD_NEED_KOUDOUVALUE_TOUYOU_DEFAULT;
// 「褒美」に必要な行動値
int  TSModCommand::AllGundan::cmdNeedKoudouValueHoubi					= CMD_NEED_KOUDOUVALUE_HOUBI_DEFAULT;
// 「物資」に必要な行動値
int  TSModCommand::AllGundan::cmdNeedKoudouValueBusshi					= CMD_NEED_KOUDOUVALUE_BUSSHI_DEFAULT;
// 「戦争」に必要な行動値
int  TSModCommand::AllGundan::cmdNeedKoudouValueSensou					= CMD_NEED_KOUDOUVALUE_SENSOU_DEFAULT;
// 「同盟」に必要な行動値
int  TSModCommand::AllGundan::cmdNeedKoudouValueDoumei					= CMD_NEED_KOUDOUVALUE_DOUMEI_DEFAULT;
// 「婚姻」に必要な行動値
int  TSModCommand::AllGundan::cmdNeedKoudouValueKonin					= CMD_NEED_KOUDOUVALUE_KONIN_DEFAULT;
// 「手切れ」に必要な行動値
int  TSModCommand::AllGundan::cmdNeedKoudouValueTegire					= CMD_NEED_KOUDOUVALUE_TEGIRE_DEFAULT;
// 「脅迫」に必要な行動値
int  TSModCommand::AllGundan::cmdNeedKoudouValueKyohaku					= CMD_NEED_KOUDOUVALUE_KYOHAKU_DEFAULT;
// 「朝廷」に必要な行動値
int  TSModCommand::AllGundan::cmdNeedKoudouValueChotei					= CMD_NEED_KOUDOUVALUE_CHOTEI_DEFAULT;
// 「内応」に必要な行動値
int  TSModCommand::AllGundan::cmdNeedKoudouValueNaiou					= CMD_NEED_KOUDOUVALUE_NAIOU_DEFAULT;
// 「謀略」に必要な行動値
int  TSModCommand::AllGundan::cmdNeedKoudouValueBouryaku				= CMD_NEED_KOUDOUVALUE_BOURYAKU_DEFAULT;
// 「評定」に必要な行動値
int  TSModCommand::AllGundan::cmdNeedKoudouValueHyoujo					= CMD_NEED_KOUDOUVALUE_HYOUJO_DEFAULT;
// 「軍団」→「任命」に必要な行動値
int  TSModCommand::AllGundan::cmdNeedKoudouValueGundanNinmei			= CMD_NEED_KOUDOUVALUE_GUNDAN_NINMEI_DEFAULT;
// 「軍団」=>分配」「合流」「再任」に必要な行動値
int  TSModCommand::AllGundan::cmdNeedKoudouValueGundanSubCmd			= CMD_NEED_KOUDOUVALUE_GUNDAN_SUBCMD_DEFAULT;
// 「内政・訓練・昇進」に必要な行動値
int  TSModCommand::AllGundan::cmdNeedKoudouValue5Ningumi				= CMD_NEED_KOUDOUVALUE_5NINGUMI_DEFAULT;


int TSModCommand::AllDaimyo::cmdSeiiTaishogunNeedCastleNum				= (int)0;
int TSModCommand::AllDaimyo::cmdTheSengokouPeriodIsEndless				= (int)0;
int TSModCommand::MyselfDaimyo::cmdGameEndConditionIsGettingPosition	= (int)0;
int TSModCommand::MyselfDaimyo::cmdCanInstructDirectOtherGundan			= (int)0; // 第２軍団以下に直接指示が可能。
int TSModCommand::MyselfDaimyo::cmdCanMultiHyoujoInSeason				= (int)0;
int TSModCommand::OtherDaimyo::cmdNeverThreat							= (int)0; // ＣＰＵは脅迫コマンドを実行しない。

int TSModCommand::AllCastle::cmdIncreaseRatioCanDraftSoldier			= (int)0; // 徴兵可能数増加割合
int TSModCommand::AllCastle::cmdCastleDefenceCoefOfNPC					= (int)0; // ＮＰＣ戦闘時の城防御係数
int TSModCommand::AllCastle::cmdShortCutFallCastle						= (int)0;



int TSModCommand::OtherBushou::cmdCanUseAllAbilityInBattle				= (int)0;

int TSModCommand::AllBushou::cmdNinjaMustBeAShadow						= (int)0;
int TSModCommand::AllBushou::cmdNinjaNotHaveAWayOfPromotion				= (int)0;
int TSModCommand::AllBushou::cmdPositionInfluenceSoldierMax				= (int)0;
int TSModCommand::AllBushou::cmdExtendedJob								= (int)0;
int TSModCommand::AllBushou::cmdNoDeclineTalentParam					= (int)0;		// もしも老齢しても能力が減衰しない
int TSModCommand::AllBushou::cmdBraveIsTheValue							= -1; // 勇猛は0～7まで有効なので…
int TSModCommand::AllBushou::cmdLifeIsTheValue							= -1; // 寿命は0～7まで有効なので…
int TSModCommand::AllBushou::cmdAbilityIsUnderdeveloped					= (int)0;
int TSModCommand::AllBushou::cmdFixExperienceMax						= (int)0; // もしも政熟・戦熟・智熟がＭＡＸであったなら…
int TSModCommand::AllBushou::cmdAutoUpExperienceAndTech					= (int)0; // 熟練度自動鍛錬があるかどうか
int TSModCommand::AllBushou::cmdNotGrowUpSuddenly						= (int)0; // 急には成長しない。
int TSModCommand::AllBushou::cmdLifeIsTheTimes							= (int)0;
int TSModCommand::AllBushou::cmdNoDutiesAndMaxIndependences				= (int)0;
int TSModCommand::AllBushou::cmdAntiAssassinationInBattle				= (int)0;
int TSModCommand::AllBushou::cmdAffinityWithSuperiorInfluenceLoyalty	= (int)0;
int TSModCommand::AllBushou::cmdSetWorkToDieEverySeason					= (int)0;
int TSModCommand::AllBushou::cmdAppearanceHimeBecomeBushou				= (int)0; // ＣＰＵ登場姫も姫武将にする。
int TSModCommand::AllBushou::cmdNonFaceBushouIndependences				= (int)0; // 顔なし武将が旗揚げするか否か。
int TSModCommand::AllBushou::cmdNonMeritoriousReset						= (int)0; // 勲功をリセットするか。


int TSModCommand::AllBushou::cmdSuppressRiseOfExpValueInBattle			= (int)0;
int TSModCommand::AllBushou::cmdParamConsiderNobunagaSeries				= (int)0;

int TSModCommand::AllBushou::cmdSoldierBaseMaxOfDaimy					= (int)0;	
int TSModCommand::AllBushou::cmdSoldierBaseMaxOfSyuku					= (int)0;
int TSModCommand::AllBushou::cmdSoldierBaseMaxOfKarou					= (int)0;
int TSModCommand::AllBushou::cmdSoldierBaseMaxOfBusyo					= (int)0;
int TSModCommand::AllBushou::cmdSoldierBaseMaxOfSamra					= (int)0;	
int TSModCommand::AllBushou::cmdSoldierBaseMaxOfAshig					= (int)0;	

int TSModCommand::AllBushou::cmdReceiveDamageOfBreakDoor				= (int)0; // 門を壊す際にダメージを受けるかどうか。
int TSModCommand::AllBushou::cmdDeathFrequencyNotInfluenceLife			= (int)0; // 死亡頻度の寿命への影響力をカットする。デフォルトでは。影響している。(=カットしない)
int TSModCommand::AllBushou::cmdDegreeLoyaltyOnCaptiveLibra				= (int)0; // 武将解放時に家臣の忠誠度下降

int TSModCommand::UltraBushou::cmdExists								= (int)0;

int TSModCommand::Environment::cmdWideResolution						= (int)0;
int TSModCommand::Environment::cmdMobileResolution						= (int)0;

int TSModCommand::Environment::cmdBGMMode								= (int)0;

int TSModCommand::Environment::cmdNoScreenSaverInDemo					= (int)0;

int TSModCommand::Environment::cmdNoTitleBarAndMenuBar					= (int)0;

int TSModCommand::Environment::cmdInternetConnectPermission				= (int)0; // インターネットへの接続の許可

int TSModCommand::Debug::cmdDebugWindowOpen								= (int)0;

int TSModCommand::gurdian2[1000] = {};


map<string, int*> AllTSModTxtFlags;

// 細部参照用
int WINAPI Extern_getTSModTxtFlag(char *szflagname) {

    if (AllTSModTxtFlags.find(szflagname) == AllTSModTxtFlags.end()) {
        // キーが "Key" の要素がない場合の処理
		return -0xFFFF;
    } else {
        // キーが "Key" の要素がある場合の処理
		// マップはフラグへのポインタを持っているので、デリファレンス(評価値)を返す
		return *(AllTSModTxtFlags[szflagname]);
    }
}



// TSMod.txtの:cmd:系のスイッチを反映させる。
void TSModCommand::setCommandSwitch(char *szCurCmdTokenBuf, int byteCurCmdLv) {

	// 征夷大将軍になるのに必要な城の数の変更
	if( strcmp(szCurCmdTokenBuf, "all_daimyo_seiitaishogun_need_castle_num")==0 ) {
		// LV の MIN, MAX オーバーフロー処理
		if (byteCurCmdLv < 0 ) { byteCurCmdLv = 0; }
		if (byteCurCmdLv > 214 ) { byteCurCmdLv = 214; }

		TSModCommand::AllDaimyo::cmdSeiiTaishogunNeedCastleNum = byteCurCmdLv;
		AllTSModTxtFlags["all_daimyo_seiitaishogun_need_castle_num"] = &TSModCommand::AllDaimyo::cmdSeiiTaishogunNeedCastleNum;

	// もしもすべての武将の勇猛値が特定の値であったなら…
	} else if (strcmp(szCurCmdTokenBuf, "all_bushou_brave_is_the_value")==0 ) {
		// LV の MIN, MAX オーバーフロー処理
		if (byteCurCmdLv < 0 ) { byteCurCmdLv = 0; }
		if (byteCurCmdLv > 7 ) { byteCurCmdLv = 7; }

		TSModCommand::AllBushou::cmdBraveIsTheValue = byteCurCmdLv;
		AllTSModTxtFlags["all_bushou_brave_is_the_value"] = &TSModCommand::AllBushou::cmdBraveIsTheValue;

	// もしもすべての武将の寿命が特定の値であったなら…
	} else if (strcmp(szCurCmdTokenBuf, "all_bushou_life_is_the_value")==0 ) {
		// LV の MIN, MAX オーバーフロー処理
		if (byteCurCmdLv < 0 ) { byteCurCmdLv = 0; }
		if (byteCurCmdLv > 7 ) { byteCurCmdLv = 7; }

		TSModCommand::AllBushou::cmdLifeIsTheValue = byteCurCmdLv;
		AllTSModTxtFlags["all_bushou_life_is_the_value"] = &TSModCommand::AllBushou::cmdLifeIsTheValue;

	// 徴兵可能数増加割合。
	} else if (strcmp(szCurCmdTokenBuf, "all_castle_increase_ratio_can_draft_soldier")==0 ) {
		if (byteCurCmdLv < 0 ) { byteCurCmdLv = 0; }
		if (byteCurCmdLv > 10 ) { byteCurCmdLv = 10; }

		TSModCommand::AllCastle::cmdIncreaseRatioCanDraftSoldier = byteCurCmdLv;
		AllTSModTxtFlags["all_castle_increase_ratio_can_draft_soldier"] = &TSModCommand::AllCastle::cmdIncreaseRatioCanDraftSoldier;

	// 自勢力の特産品に関連した項目が自動的に増える(鉄砲・馬・国際港)
	} else if (strcmp(szCurCmdTokenBuf, "myself_gundan_increase_special_product_by_the_thing")==0 ) {
		if (byteCurCmdLv < 0 ) { byteCurCmdLv = 0; }
		if (byteCurCmdLv > 10 ) { byteCurCmdLv = 10; }
		TSModCommand::MyselfGundan::cmdIncreaseSpecialProductByTheThing = byteCurCmdLv;
		AllTSModTxtFlags["myself_gundan_increase_special_product_by_the_thing"] = &TSModCommand::MyselfGundan::cmdIncreaseSpecialProductByTheThing;

	// 他勢力の特産品に関連した項目が自動的に増える(鉄砲・馬・国際港)
	} else if (strcmp(szCurCmdTokenBuf, "others_gundan_increase_special_product_by_the_thing")==0 ) {
		if (byteCurCmdLv < 0 ) { byteCurCmdLv = 0; }
		if (byteCurCmdLv > 10 ) { byteCurCmdLv = 10; }
		TSModCommand::OthersGundan::cmdIncreaseSpecialProductByTheThing = byteCurCmdLv;
		AllTSModTxtFlags["others_gundan_increase_special_product_by_the_thing"] = &TSModCommand::OthersGundan::cmdIncreaseSpecialProductByTheThing;


	// 熟練度や技能の自動鍛錬あり
	} else if (strcmp(szCurCmdTokenBuf, "all_bushou_autoup_experience_and_tech")==0 ) {
		if (byteCurCmdLv < 0 ) { byteCurCmdLv = 0; }
		if (byteCurCmdLv > 10 ) { byteCurCmdLv = 10; }
		TSModCommand::AllBushou::cmdAutoUpExperienceAndTech = byteCurCmdLv;
		AllTSModTxtFlags["all_bushou_autoup_experience_and_tech"] = &TSModCommand::AllBushou::cmdAutoUpExperienceAndTech;

	// 大名の兵士数
	} else if (strcmp(szCurCmdTokenBuf, "all_bushou_soldier_base_maxof_daimy")==0 ) {
		if (byteCurCmdLv > 100 ) { byteCurCmdLv = 100; }
		TSModCommand::AllBushou::cmdSoldierBaseMaxOfDaimy = byteCurCmdLv;
		AllTSModTxtFlags["all_bushou_soldier_base_maxof_daimy"] = &TSModCommand::AllBushou::cmdSoldierBaseMaxOfDaimy;
	// 宿老の兵士数
	} else if (strcmp(szCurCmdTokenBuf, "all_bushou_soldier_base_maxof_syuku")==0 ) {
		if (byteCurCmdLv > 100 ) { byteCurCmdLv = 100; }
		TSModCommand::AllBushou::cmdSoldierBaseMaxOfSyuku = byteCurCmdLv;
		AllTSModTxtFlags["all_bushou_soldier_base_maxof_syuku"] = &TSModCommand::AllBushou::cmdSoldierBaseMaxOfSyuku;
	// 家老の兵士数
	} else if (strcmp(szCurCmdTokenBuf, "all_bushou_soldier_base_maxof_karou")==0 ) {
		if (byteCurCmdLv > 100 ) { byteCurCmdLv = 100; }
		TSModCommand::AllBushou::cmdSoldierBaseMaxOfKarou = byteCurCmdLv;
		AllTSModTxtFlags["all_bushou_soldier_base_maxof_karou"] = &TSModCommand::AllBushou::cmdSoldierBaseMaxOfKarou;
	// 部将の兵士数
	} else if (strcmp(szCurCmdTokenBuf, "all_bushou_soldier_base_maxof_busyo")==0 ) {
		if (byteCurCmdLv > 100 ) { byteCurCmdLv = 100; }
		TSModCommand::AllBushou::cmdSoldierBaseMaxOfBusyo = byteCurCmdLv;
		AllTSModTxtFlags["all_bushou_soldier_base_maxof_busyo"] = &TSModCommand::AllBushou::cmdSoldierBaseMaxOfBusyo;
	// 侍大将の兵士数
	} else if (strcmp(szCurCmdTokenBuf, "all_bushou_soldier_base_maxof_samra")==0 ) {
		if (byteCurCmdLv > 100 ) { byteCurCmdLv = 100; }
		TSModCommand::AllBushou::cmdSoldierBaseMaxOfSamra = byteCurCmdLv;
		AllTSModTxtFlags["all_bushou_soldier_base_maxof_samra"] = &TSModCommand::AllBushou::cmdSoldierBaseMaxOfSamra;
	// 足軽頭の兵士数
	} else if (strcmp(szCurCmdTokenBuf, "all_bushou_soldier_base_maxof_ashig")==0 ) {
		if (byteCurCmdLv > 100 ) { byteCurCmdLv = 100; }
		TSModCommand::AllBushou::cmdSoldierBaseMaxOfAshig = byteCurCmdLv;
		AllTSModTxtFlags["all_bushou_soldier_base_maxof_ashig"] = &TSModCommand::AllBushou::cmdSoldierBaseMaxOfAshig;

	} else if ( strstr(szCurCmdTokenBuf,  "all_gundan_need_koudou_value" ) ) {
		if (byteCurCmdLv < 1  )  { byteCurCmdLv = 1; }
		if (byteCurCmdLv > 100 ) { byteCurCmdLv = 100; }

		// 講義の行動力
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

		// LV の MIN, MAX オーバーフロー処理
		if (byteCurCmdLv < 0 ) { byteCurCmdLv = 0; }
		if (byteCurCmdLv > 5 ) { byteCurCmdLv = 5; }

		// 自勢力に金を米を加算するコマンド
		if (strcmp(szCurCmdTokenBuf, "myself_gundan_get_money_and_rise_on_season_change")==0 ) {
			TSModCommand::MyselfGundan::cmdGetMoneyAndRiseOnSeasonChangeLv = byteCurCmdLv;
			AllTSModTxtFlags["myself_gundan_get_money_and_rise_on_season_change"] = &TSModCommand::MyselfGundan::cmdGetMoneyAndRiseOnSeasonChangeLv;

		// 他勢力に金を米を加算するコマンド
		} else if (strcmp(szCurCmdTokenBuf, "others_gundan_get_money_and_rise_on_season_change")==0 ) {
			TSModCommand::OthersGundan::cmdGetMoneyAndRiseOnSeasonChangeLv = byteCurCmdLv;
			AllTSModTxtFlags["others_gundan_get_money_and_rise_on_season_change"] = &TSModCommand::OthersGundan::cmdGetMoneyAndRiseOnSeasonChangeLv;

		// 自勢力の兵士が自動回復するコマンド
		} else if (strcmp(szCurCmdTokenBuf, "myself_gundan_regeneration_soldier_automatically")==0 ) {
			TSModCommand::MyselfGundan::cmdRegenerationSoldierAutomatically = byteCurCmdLv;
			AllTSModTxtFlags["myself_gundan_regeneration_soldier_automatically"] = &TSModCommand::MyselfGundan::cmdRegenerationSoldierAutomatically;

		// 他勢力の兵士が自動回復するコマンド
		} else if (strcmp(szCurCmdTokenBuf, "others_gundan_regeneration_soldier_automatically")==0 ) {
			TSModCommand::OthersGundan::cmdRegenerationSoldierAutomatically = byteCurCmdLv;
			AllTSModTxtFlags["others_gundan_regeneration_soldier_automatically"] = &TSModCommand::OthersGundan::cmdRegenerationSoldierAutomatically;

		// プレイヤの進捗と、プレイヤ以外の全体の軍団の進捗を見つつ、プレイヤ以外の軍団は互いに資源バランスを取ろうとします。
		} else if (strcmp(szCurCmdTokenBuf, "others_gundan_player_and_the_whole_influence_each_other_data")==0 ) {
			TSModCommand::OthersGundan::cmdPlayerAndTheWholeInfluenceEachOtherData = byteCurCmdLv;
			AllTSModTxtFlags["others_gundan_player_and_the_whole_influence_each_other_data"] = &TSModCommand::OthersGundan::cmdPlayerAndTheWholeInfluenceEachOtherData;

		// 戦闘マップを｢見ていない｣状態で戦争が起きた時、武将の熟練度が過度に上昇することによる戦闘値や智謀値のインフレを防ぎます。
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_suppress_rise_of_exp_value_in_battle")==0 ) {
			TSModCommand::AllBushou::cmdSuppressRiseOfExpValueInBattle = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_suppress_rise_of_exp_value_in_battle"] = &TSModCommand::AllBushou::cmdSuppressRiseOfExpValueInBattle;

		// 出陣数は常に8が保障されている。
		} else if ( strcmp(szCurCmdTokenBuf, "all_gundan_the_number_of_the_max_go_to_war_fix")==0 ) {
			// LV の MIN, MAX オーバーフロー処理
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllGundan::cmdTheNumberOfTheMaxGoToWarFix = byteCurCmdLv;
			AllTSModTxtFlags["all_gundan_the_number_of_the_max_go_to_war_fix"] = &TSModCommand::AllGundan::cmdTheNumberOfTheMaxGoToWarFix;

		// もしも全ての武将の歳を取るのが遅くなったなら…
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_life_is_the_times")==0 ) {
			TSModCommand::AllBushou::cmdLifeIsTheTimes = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_life_is_the_times"] = &TSModCommand::AllBushou::cmdLifeIsTheTimes;

		// もしも全ての武将が老齢しても能力が減衰しない
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_no_decline_talent_param")==0 ) {
			TSModCommand::AllBushou::cmdNoDeclineTalentParam = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_no_decline_talent_param"] = &TSModCommand::AllBushou::cmdNoDeclineTalentParam;

		// 武将は急には成長しない。
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_not_grow_up_suddenly")==0 ) {
			TSModCommand::AllBushou::cmdNotGrowUpSuddenly = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_not_grow_up_suddenly"] = &TSModCommand::AllBushou::cmdNotGrowUpSuddenly;

		// 忍者は影たるものであるべし(忍者の兵士数は1/5の制限が入る)
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_ninja_must_be_a_shadow")==0 ) {
			if (byteCurCmdLv > 3 ) { byteCurCmdLv = 3; }
			TSModCommand::AllBushou::cmdNinjaMustBeAShadow = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_ninja_must_be_a_shadow"] = &TSModCommand::AllBushou::cmdNinjaMustBeAShadow;

		// 忍者は勲功が上昇せず、出世できない
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_ninja_not_have_a_way_of_promotion")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllBushou::cmdNinjaNotHaveAWayOfPromotion = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_ninja_not_have_a_way_of_promotion"] = &TSModCommand::AllBushou::cmdNinjaNotHaveAWayOfPromotion;

		// 官位や役職があれば、それが兵士の最大数の増加に繋がる
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_position_influence_soldier_max")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllBushou::cmdPositionInfluenceSoldierMax = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_position_influence_soldier_max"] = &TSModCommand::AllBushou::cmdPositionInfluenceSoldierMax;

		// 拡張された職業(出自)を利用する。
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_extended_job")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllBushou::cmdExtendedJob = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_extended_job"] = &TSModCommand::AllBushou::cmdExtendedJob;

		// 技能が未開発であったなら
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_ability_is_underdeveloped")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllBushou::cmdAbilityIsUnderdeveloped = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_ability_is_underdeveloped"] = &TSModCommand::AllBushou::cmdAbilityIsUnderdeveloped;

		// もしも政熟・戦熟・智熟がＭＡＸであったなら…
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_fix_experience_max")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllBushou::cmdFixExperienceMax = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_fix_experience_max"] = &TSModCommand::AllBushou::cmdFixExperienceMax;

		// 武将の能力は歴代シリーズを考慮したものである。
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_param_consider_nobunaga_series")==0 ) {
			if (byteCurCmdLv > 2 ) { byteCurCmdLv = 2; }
			TSModCommand::AllBushou::cmdParamConsiderNobunagaSeries = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_param_consider_nobunaga_series"] = &TSModCommand::AllBushou::cmdParamConsiderNobunagaSeries;
			
		// 全ての武将に義理などなく、独立が常であったなら…
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_noduty_and_maxindependence")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllBushou::cmdNoDutiesAndMaxIndependences = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_noduty_and_maxindependence"] = &TSModCommand::AllBushou::cmdNoDutiesAndMaxIndependences;

		// 全ての武将は暗殺返しをすることがある。
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_anti_assassination_in_battle")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllBushou::cmdAntiAssassinationInBattle = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_anti_assassination_in_battle"] = &TSModCommand::AllBushou::cmdAntiAssassinationInBattle;

		// 全ての武将は、毎季節、上長との相性の影響を受けて、変化する。
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_affinity_with_superior_influence_loyalty")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllBushou::cmdAffinityWithSuperiorInfluenceLoyalty = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_affinity_with_superior_influence_loyalty"] = &TSModCommand::AllBushou::cmdAffinityWithSuperiorInfluenceLoyalty;
		
		// (死亡頻度に関係なく)毎季節、武将の死亡処理が行われる。
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_set_work_to_die_every_season")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllBushou::cmdSetWorkToDieEverySeason = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_set_work_to_die_every_season"] = &TSModCommand::AllBushou::cmdSetWorkToDieEverySeason;

		// ＣＰＵ登場姫も姫武将にする。
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_appearance_hime_become_bushou")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllBushou::cmdAppearanceHimeBecomeBushou = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_appearance_hime_become_bushou"] = &TSModCommand::AllBushou::cmdAppearanceHimeBecomeBushou;

		// 顔なし武将が旗揚げするか否か。
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_non_face_bushou_independences")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllBushou::cmdNonFaceBushouIndependences = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_non_face_bushou_independences"] = &TSModCommand::AllBushou::cmdNonFaceBushouIndependences;

		// 顔なし武将が旗揚げするか否か。
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_non_meritorious_reset")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllBushou::cmdNonMeritoriousReset = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_non_meritorious_reset"] = &TSModCommand::AllBushou::cmdNonMeritoriousReset;

		// 落城ショートカット処理が有効ならば
		} else if (strcmp(szCurCmdTokenBuf, "all_castle_shortcut_fall_castle")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllCastle::cmdShortCutFallCastle = byteCurCmdLv;
			AllTSModTxtFlags["all_castle_shortcut_fall_castle"] = &TSModCommand::AllCastle::cmdShortCutFallCastle;

		// 門を壊す際にダメージを受けるならば
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_receive_damage_of_break_door")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllBushou::cmdReceiveDamageOfBreakDoor = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_receive_damage_of_break_door"] = &TSModCommand::AllBushou::cmdReceiveDamageOfBreakDoor;

		// ＮＰＣ戦闘時の城防御係数
		} else if (strcmp(szCurCmdTokenBuf, "all_castle_castle_defence_coef_of_npc")==0 ) {
			if (byteCurCmdLv > 7 ) { byteCurCmdLv = 7; }
			TSModCommand::AllCastle::cmdCastleDefenceCoefOfNPC = byteCurCmdLv;
			AllTSModTxtFlags["all_castle_castle_defence_coef_of_npc"] = &TSModCommand::AllCastle::cmdCastleDefenceCoefOfNPC;

		// 死亡頻度の寿命への影響力をカットする。デフォルトでは。影響している。(=カットしない)
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_death_frequency_not_influence_life")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllBushou::cmdDeathFrequencyNotInfluenceLife = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_death_frequency_not_influence_life"] = &TSModCommand::AllBushou::cmdDeathFrequencyNotInfluenceLife;

		// ＮＰＣ勢力は脅迫コマンドを実行しない。
		} else if (strcmp(szCurCmdTokenBuf, "all_other_daimyo_never_thread")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::OtherDaimyo::cmdNeverThreat = byteCurCmdLv;
			AllTSModTxtFlags["all_other_daimyo_never_thread"] = &TSModCommand::OtherDaimyo::cmdNeverThreat;

		// 他勢力の全ての武将は、戦闘中全ての技能が使えたら
		} else if (strcmp(szCurCmdTokenBuf, "other_bushou_can_use_all_ability_in_battle")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::OtherBushou::cmdCanUseAllAbilityInBattle = byteCurCmdLv;
			AllTSModTxtFlags["other_bushou_can_use_all_ability_in_battle"] = &TSModCommand::OtherBushou::cmdCanUseAllAbilityInBattle;

		// ゲーム終了条件を変更し、役職就任までとする。
		} else if (strcmp(szCurCmdTokenBuf, "myself_daimyo_game_end_condition_is_getting_position")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::MyselfDaimyo::cmdGameEndConditionIsGettingPosition = byteCurCmdLv;
			AllTSModTxtFlags["myself_daimyo_game_end_condition_is_getting_position"] = &TSModCommand::MyselfDaimyo::cmdGameEndConditionIsGettingPosition;

		// 第２軍団以下に直接指示が可能。
		} else if (strcmp(szCurCmdTokenBuf, "myself_daimyo_can_instruct_direct_other_gundan")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::MyselfDaimyo::cmdCanInstructDirectOtherGundan = byteCurCmdLv;
			AllTSModTxtFlags["myself_daimyo_can_instruct_direct_other_gundan"] = &TSModCommand::MyselfDaimyo::cmdCanInstructDirectOtherGundan;

		// 一季節に複数回評定が可能。
		} else if (strcmp(szCurCmdTokenBuf, "myself_daimyo_can_multi_hyoujo_in_season")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::MyselfDaimyo::cmdCanMultiHyoujoInSeason = byteCurCmdLv;
			AllTSModTxtFlags["myself_daimyo_can_multi_hyoujo_in_season"] = &TSModCommand::MyselfDaimyo::cmdCanMultiHyoujoInSeason;

		// 武将解放時に家臣の忠誠度下降。
		} else if (strcmp(szCurCmdTokenBuf, "all_bushou_degree_loyalty_on_captive_libra")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllBushou::cmdDegreeLoyaltyOnCaptiveLibra = byteCurCmdLv;
			AllTSModTxtFlags["all_bushou_degree_loyalty_on_captive_libra"] = &TSModCommand::AllBushou::cmdDegreeLoyaltyOnCaptiveLibra;

		// ゲーム終了条件を変更し、エンドレスとする。
		} else if (strcmp(szCurCmdTokenBuf, "all_daimyo_the_sengoku_period_is_endless")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::AllDaimyo::cmdTheSengokouPeriodIsEndless = byteCurCmdLv;
			AllTSModTxtFlags["all_daimyo_the_sengoku_period_is_endless"] = &TSModCommand::AllDaimyo::cmdTheSengokouPeriodIsEndless;

		// ウルトラ武将がいるならば…
		} else if (strcmp(szCurCmdTokenBuf, "ultra_bushou_exists")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::UltraBushou::cmdExists = byteCurCmdLv;
			AllTSModTxtFlags["ultra_bushou_exists"] = &TSModCommand::UltraBushou::cmdExists;

		// 解像度がモバイル用の指定ならば…
		} else if (strcmp(szCurCmdTokenBuf, "environment_wide_resolution")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::Environment::cmdWideResolution = byteCurCmdLv;
			AllTSModTxtFlags["environment_wide_resolution"] = &TSModCommand::Environment::cmdWideResolution;

			// 解像度がモバイル用の指定ならば…
		} else if (strcmp(szCurCmdTokenBuf, "environment_mobile_resolution")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::Environment::cmdMobileResolution = byteCurCmdLv;
			AllTSModTxtFlags["environment_mobile_resolution"] = &TSModCommand::Environment::cmdMobileResolution;

		// ＢＧＭモード
		} else if (strcmp(szCurCmdTokenBuf, "environment_bgm_mode")==0 ) {
			TSModCommand::Environment::cmdBGMMode = byteCurCmdLv;
			AllTSModTxtFlags["environment_bgm_mode"] = &TSModCommand::Environment::cmdBGMMode;

		// スクリーンセイバー起動防止
		} else if (strcmp(szCurCmdTokenBuf, "environment_no_screensaver_in_demo")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::Environment::cmdNoScreenSaverInDemo = byteCurCmdLv;
			AllTSModTxtFlags["environment_no_screensaver_in_demo"] = &TSModCommand::Environment::cmdNoScreenSaverInDemo;

		// タイトルバートメニューバーは不要
		} else if (strcmp(szCurCmdTokenBuf, "environment_no_titlebar_and_menubar")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::Environment::cmdNoTitleBarAndMenuBar = byteCurCmdLv;
			AllTSModTxtFlags["environment_no_titlebar_and_menubar"] = &TSModCommand::Environment::cmdNoTitleBarAndMenuBar;
			
		// インターネットへの接続許可
		} else if (strcmp(szCurCmdTokenBuf, "environment_internet_connect_permission")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::Environment::cmdInternetConnectPermission = byteCurCmdLv;
			AllTSModTxtFlags["environment_internet_connect_permission"] = &TSModCommand::Environment::cmdInternetConnectPermission;

		// デバッグウィンドウのオープン指定ならば…
		} else if (strcmp(szCurCmdTokenBuf, "debug_debug_window_open")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::Debug::cmdDebugWindowOpen = byteCurCmdLv;
			AllTSModTxtFlags["debug_debug_window_open"] = &TSModCommand::Debug::cmdDebugWindowOpen;

		// この世に鉄砲が存在しなかったなら…
		} else if (strcmp(szCurCmdTokenBuf, "world_the_teppou_no_exists")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::World::cmdTheTeppouNoExists = byteCurCmdLv;
			AllTSModTxtFlags["world_the_teppou_no_exists"] = &TSModCommand::World::cmdTheTeppouNoExists;

		// この世に弓が存在しなかったなら…
		} else if (strcmp(szCurCmdTokenBuf, "world_the_yumi_no_exists")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::World::cmdTheYumiNoExists = byteCurCmdLv;
			AllTSModTxtFlags["world_the_yumi_no_exists"] = &TSModCommand::World::cmdTheYumiNoExists;

		// 仮想武将を登場させるなら…
		} else if (strcmp(szCurCmdTokenBuf, "world_virtual_bushou_appear")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::World::cmdVirtualBushouAppear = byteCurCmdLv;
			AllTSModTxtFlags["world_virtual_bushou_appear"] = &TSModCommand::World::cmdVirtualBushouAppear;
			
		// この世に.jp専用のカスタム家宝が存在するならば…
		} else if (strcmp(szCurCmdTokenBuf, "world_custom_kahou_exists")==0 ) {
			if (byteCurCmdLv > 1 ) { byteCurCmdLv = 1; }
			TSModCommand::World::cmdCustomKahouExists = byteCurCmdLv;
			AllTSModTxtFlags["world_custom_kahou_exists"] = &TSModCommand::World::cmdCustomKahouExists;
		}
	}
}
