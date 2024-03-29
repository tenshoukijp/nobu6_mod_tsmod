#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"
#include <fstream>
#include <math.h>
#include <algorithm>


#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TMath.h"
#include "OnChangeWeather.h"
#include "ScenarioMod.h"
#include "FuncBattleUnitStateRepaint.h"

using namespace std;

#pragma pack(1)

NB6YEAR				*const _nb6year = (NB6YEAR *)GAMEDATASTRUCT_YEAR_ADDRESSS;
NB6TURN				*const _nb6turn = (NB6TURN *)GAMEDATASTRUCT_TURN_ADDRESSS;
NB6SOBA				*const _nb6soba = (NB6SOBA *)GAMEDATASTRUCT_SOBA_ADDRESSS;
NB6PLAYERS8			*const nb6players8 = (NB6PLAYERS8 *)GAMEDATASTRUCT_PLAYERS8_ADDRESSS;
NB6BUSHOUCOPY		nb6bushoucopy[GAMEDATASTRUCT_BUSHOU_NUM]		= {};
NB6BUSHOU			*const nb6bushouref = (NB6BUSHOU *)GAMEDATASTRUCT_BUSHOU_ADDRESSS;

NB6BUSHOUNAME		*const nb6bushouname = (NB6BUSHOUNAME *)GAMEDATASTRUCT_BUSHOUNAME_ADDRESSS;
NB6BUSHOURETSUDEN   nb6bushouretsuden[GAMEDATASTRUCT_RETSUDEN_NUM] = {};
NB6KAHOURETSUDEN    nb6kahouretsuden[GAMEDATASTRUCT_KAHOU_NUM] = {};
NB6CASTLE			*const nb6castle = (NB6CASTLE *)GAMEDATASTRUCT_CASTLE_ADDRESSS;
NB6CASTLEMAX		*const nb6castlemax = (NB6CASTLEMAX *)GAMEDATASTRUCT_CASTLEMAX_ADDRESSS;
NB6PARAMMAX			*const _nb6parammax = (NB6PARAMMAX *)GAMEDATASTRUCT_PARAMMAX_ADDRESS;
NB6PARAMMAX_MAIN	*const _nb6parammax_main = (NB6PARAMMAX_MAIN *)GAMEDATASTRUCT_PARAMMAX_MAIN_ADDRESS;
NB6DAIMYO			*const nb6daimyo = (NB6DAIMYO *)GAMEDATASTRUCT_DAIMYO_ADDRESS;
NB6GUNDAN			*const nb6gundan = (NB6GUNDAN *)GAMEDATASTRUCT_GUNDAN_ADDRESS;
NB6CHIMEI			*const nb6chimei = (NB6CHIMEI *)GAMEDATASTRUCT_CHIMEI_ADDRESS;
NB6KANI				*const nb6kani   = (NB6KANI *)GAMEDATASTRUCT_KANI_ADDRESS;
NB6KAHOU			*const nb6kahou  = (NB6KAHOU *)GAMEDATASTRUCT_KAHOU_ADDRESSS;
NB6SOLDIERMAX		nb6soldiermax[GAMEDATASTRUCT_SOLDIERMAX_NUM]= {};
NB6REQUIREDMERITS   *const nb6requiredmerits = (NB6REQUIREDMERITS *)GAMEDATASTRUCT_REQUIREDMERITS_ADDRESS;
NB6SETTINGS			*const _nb6settings = (NB6SETTINGS *)GAMEDATASTRUCT_SETTINGS_ADDRESSS;
NB6SETTINGSEX		*const _nb6settingsex = (NB6SETTINGSEX *)GAMEDATASTRUCT_SETTINGSEX_ADDRESSS;
NB6FUKIDASHIRELATEPERSONS *const nb6fukidashirelatepersons = (NB6FUKIDASHIRELATEPERSONS *)GAMEDATASTRUCT_FUKIDASHI_RELATE_PERSONS;
NB6CASTLE_HEX_ROLE	*const _nb6castle_hex_role = (NB6CASTLE_HEX_ROLE *)GAMEDATASTRUCT_CASTLE_HEX_ROLE_ADDRESS;
NB6CASTLE_HEX_HIGH	*const _nb6castle_hex_high = (NB6CASTLE_HEX_HIGH *)GAMEDATASTRUCT_CASTLE_HEX_HIGH_ADDRESS;

NB6FIELD_HEX_ROLE  *const _nb6field_hex_role = (NB6FIELD_HEX_ROLE *)GAMEDATASTRUCT_FIELD_HEX_ROLE_ADDRESS;

NB6CASTLE_HEX_ROLE *const _nb6fire_hex_role = (NB6CASTLE_HEX_ROLE *)GAMEDATASTRUCT_CASTLE_HEX_FIRE_ROLE_ADDRESS;


NB6CASTLE_HEX_UNIT_POS *const _nb6castle_hex_unit_pos = (NB6CASTLE_HEX_UNIT_POS *)GAMEDATASTRUCT_CASTLE_HEX_UNIT_POS_ADDRESS;
NB6FIELD_HEX_UNIT_POS  *const _nb6field_hex_unit_pos  = (NB6FIELD_HEX_UNIT_POS *)GAMEDATASTRUCT_FIELD_HEX_UNIT_POS_ADDRESS;


NB6BUSHOU_HEX_UNIT_INFO *const nb6bushou_hex_unit_info = (NB6BUSHOU_HEX_UNIT_INFO *)GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_ADDRESS; // 戦場における各武将の情報
NB6GUNDAN_HEX_INFO *const _nb6gundan_hex_info = (NB6GUNDAN_HEX_INFO *)GAMEDATASTRUCT_WAR_HEX_TURN_ADDRESS; 

NB6WAR_JOINT_DAIMYO_HEX_GROUP_INFO *const nb6war_joint_daimyo_hex_group_info = (NB6WAR_JOINT_DAIMYO_HEX_GROUP_INFO *)GAMEDATASTRUCT_WAR_JOINT_DAIMYO_HEX_GROUP_INFO_ADDRESS; // 戦場に参加した大名や総大将の情報

NB6FUKIDASHI_DIALOG_LAYOUT *const _nb6fukidashi_dialog_layout = (NB6FUKIDASHI_DIALOG_LAYOUT *)GAMEDATASTRUCT_FUKIDASHI_DIALOG_LAYOUT_ADDRESS; // 

// 現在プレイ中のシナリオロケーションを得る(0〜5, 不明な場合-1)

int getCurScenarioLocation() {
	if ( 0 <= iSelectingScenarioLocation && iSelectingScenarioLocation <= 5 ) {
		return iSelectingScenarioLocation;
	}
	return -1;
}

int WINAPI Extern_getCurScenarioLocation() {
	return getCurScenarioLocation();
}

/*
004D3728  [E4 36 4D 00] [C4 36 4D 00] [A4 36 4D 00] [84 36 4D 00]  ・M.ﾄ6M.､6M.・M.
004D3738  [64 36 4D 00] [44 36 4D 00]                          d6M.D6M.
*/
char* getScenarioTitle(int iLocation) {
	if ( 0 <= iLocation && iLocation <= 5 ) {
		int *p = (int *)GAMEDATASTRUCT_SCENARIO_NAME_POINTER_ARRAY_ADDRESS;
		int iTargetAddress = *(p+iLocation);
		char *pTitle = (char *)iTargetAddress;
		return pTitle;
	} else {
		return NULL;
	}
}


// 現在のゲーム進行中の年数を得る。
int getYearAD() {
	return nb6year.year + getStartYearAD();
}

// ゲームの開始年。デフォルトは1454年であるが、改造した場合に備えてアドレスから得るようにする。
int getStartYearAD() {
	return *((WORD *)GAMEDATASTRUCT_START_YEAR_ADDRESS);
}

// 現在のゲーム進行中の季節を得る
char szSeasonName[5][3] = {"春", "夏", "秋", "冬", "無"};
char* getSeasonName() {
	char season = nb6year.season;

	if (season==0) {
		return szSeasonName[0];
	} else if (season==1) {
		return szSeasonName[1];
	} else if (season==2) {
		return szSeasonName[2];
	} else if (season==3) {
		return szSeasonName[3];
	}
	return szSeasonName[4];
}

// 武将の年齢を得る
int getYearsOld( int iBushouID ) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		return nb6year.year - nb6bushouref[iBushouID].birth + 1;
	}
	return 0xFF;
}

/*
* 武将の年齢を設定する。
*/
bool setYearsOld(int iBushouID, int iYearsOld) {

	// 範囲チェック
	if (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		int check = nb6year.year + 1 - iYearsOld;
		if (check < 0) {
			OutputDebugStream("指定年齢は範囲外。誕生年が結果として、天翔記基準年(1454)より早い生まれとなってしまっている。");
			check = 0;
		}
		if (check > 255) {
			OutputDebugStream("指定年齢は範囲外。誕生年が結果として、天翔記最終年(1708)より遅い生まれとなってしまっている。");
			check = 255;
		}
		nb6bushouref[iBushouID].birth = check;
		return true;
	}
	else {
		OutputDebugStream("iBushouIDは範囲外");
		return false;
	}
}




// 対象の軍団IDがいずれかのプレイヤー勢力下の軍団である(最大８人)
// iTargetGundanIDには配列上の番号(attach-1の数)を渡すこと
bool isPlayerGundan( WORD iTargetGundanID ) {

	if (iTargetGundanID >= GAMEDATASTRUCT_GUNDAN_NUM) { 
		return false;
	}

	WORD iGundanID = 0;
	for ( int iPG=0; iPG<GAMEDATASTRUCT_PLAYERS8_NUM; iPG++) {
		// プレイヤーが担当している軍団のＩＤを確保
		iGundanID = nb6players8[iPG].gundan;

		// 0xFFFFならばプレイヤーではない
		if ( iGundanID == 0xFFFF ) {
			continue;
		}

		// プレイヤー担当の軍団の大名が、ターゲットとしている対象の軍団の大名と一致するならば、
		// それはプレイヤーの勢力下の軍団である。
		// どうもnb6player8のgundanの値は、内部管理のgundanの値よりも+1されているようである。
		// よってここでは-1する。
		if (iGundanID >= 1 && nb6gundan[iGundanID-1].attach == nb6gundan[iTargetGundanID].attach ) {
			return true;
		}
	}
	
	return false;
}

// 対象の武将IDがいずれかのプレイヤー勢力下の武将である
bool isPlayerBushou( WORD iTargetBushouID ) {
	// 武将IDの範囲に収まっていること
	if ( 0 <= iTargetBushouID && iTargetBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		// ターゲットの武将の軍団IDがプレイヤーの軍団であるかを返す
		return isPlayerGundan(nb6bushouref[iTargetBushouID].gundan-1);
	} else {
		return false;
	}
}

// 対象の大名IDがいずれかのプレイヤー担当の大名である
bool isPlayerDaimyo( WORD iTargetDaimyoID ) {
	// 武将IDの範囲に収まっていること
	if ( 0 <= iTargetDaimyoID && iTargetDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM ) {
		// ターゲットの武将の軍団IDがプレイヤーの軍団であるかを返す
		return isPlayerGundan(nb6daimyo[iTargetDaimyoID].gundan-1);
	} else {
		return false;
	}
}


// 足軽・騎馬・鉄砲・水軍の適正を数値で得る。(0=E,1=D,2=C,3=B,4=A,5=S)
int getBushouAptitude( int iBushouID, APTITUDE eAptitude ) {
	if (eAptitude==APTITUDE_ASIGARU) {
		return nb6bushouref[iBushouID].aptitudeasi;
	} else if (eAptitude==APTITUDE_KIBA) {
		return nb6bushouref[iBushouID].aptitudekib;
	} else if (eAptitude==APTITUDE_TEPPOU) {
		// 鉄砲は２つのフィットフィールドに分かれてしまっているので、計算
		int ret = nb6bushouref[iBushouID]._aptitudegun_lo << 2;
		ret += nb6bushouref[iBushouID]._aptitudegun_hi;
		return ret;
	} else if (eAptitude==APTITUDE_SUIGUN) {
		return nb6bushouref[iBushouID].aptitudesui;

	} else if (eAptitude == APTITUDE_YARI) {
		return nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari;
	}
	else if (eAptitude == APTITUDE_YUMI) {
		return nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi;
	}
	return -1;
}


extern int WINAPI Extern_setBushouExAptitude(int iBushouID, APTITUDE eAptitude, int iTekisei);

// 足軽・騎馬・鉄砲・水軍の適正を数値で設定する。(0=E,1=D,2=C,3=B,4=A,5=S)
void setBushouAptitude( int iBushouID, APTITUDE eAptitude, int iTekisei ) {
	if (eAptitude==APTITUDE_ASIGARU) {
		nb6bushouref[iBushouID].aptitudeasi = iTekisei;
	} else if (eAptitude==APTITUDE_KIBA) {
		nb6bushouref[iBushouID].aptitudekib = iTekisei;
	} else if (eAptitude==APTITUDE_TEPPOU) {
		// 鉄砲は２つのフィットフィールドに分かれてしまっているので、計算
		nb6bushouref[iBushouID]._aptitudegun_hi = iTekisei & 0x0003;  // 0b000000HH
		nb6bushouref[iBushouID]._aptitudegun_lo = iTekisei >> 2;      // 0b00000L00
	} else if (eAptitude==APTITUDE_SUIGUN) {
		nb6bushouref[iBushouID].aptitudesui = iTekisei;

	}
	else {
		Extern_setBushouExAptitude(iBushouID, eAptitude, iTekisei);
	}
}

// 弓の適性値を設定する(ScenarioMod等からの設定) (0=E,1=D,2=C,3=B,4=A,5=S) 値の取得も兼ねる。
int WINAPI Extern_setBushouExAptitude(int iBushouID, APTITUDE eAptitude, int iTekisei ) {
	if ( eAptitude==APTITUDE_YUMI) {
		// -1ならば設定ではなく、値の取得
		if ( iTekisei == -1 ) {
			return nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi;
		}
		// 有効な値なら代入
		else {
			// もし弓がないのであれば、０で上書き
			if ( TSModCommand::World::cmdTheYumiNoExists ) {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = 0;
			} else {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = iTekisei;
			}
			return iTekisei;
		}

	} else if (eAptitude==APTITUDE_YARI) {
		// -1ならば設定ではなく、値の取得
		if ( iTekisei == -1 ) {
			return nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari;
		}
		else {
			nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari = iTekisei;
			return iTekisei;
		}
	}
	return -1;
}




// 武将の独立値を得る
int getBushouIndependence( int iBushouID ) {
	// 鉄砲は２つのフィットフィールドに分かれてしまっているので、計算
	int ret = nb6bushouref[iBushouID]._independence_lo << 2;
	ret += nb6bushouref[iBushouID]._independence_hi;
	return ret;
}

// 武将の独立値を設定する
void setBushouIndependence( int iBushouID, int iIndependence ) {
	nb6bushouref[iBushouID]._independence_hi = iIndependence & 0x0003;  // 0b000000HH
	nb6bushouref[iBushouID]._independence_lo = iIndependence >> 2;      // 0b00000L00
}

// 武将の戦闘値
int getBushouBat(int iBushouID) {
	// 戦才×戦塾/2000が
	return int(nb6bushouref[iBushouID].maxbat * (float(nb6bushouref[iBushouID].expbat) / float(2000)));
}
// 武将の智謀値
int getBushouInt(int iBushouID) {
	// 戦才×戦塾/2000が
	return int(nb6bushouref[iBushouID].maxint * (float(nb6bushouref[iBushouID].expint) / float(2000)));
}
// 武将の政治値
int getBushouGov(int iBushouID) {
	// 戦才×戦塾/2000が
	return int(nb6bushouref[iBushouID].maxgov * (float(nb6bushouref[iBushouID].expgov) / float(2000)));
}

// 家宝などを含めない素の政治才能
int getBushouPureGovMax(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int upparam = 0; // 上昇値
		for (int iKahouID =0; iKahouID < GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {
			// 政治増加タイプ：0 茶碗  1 茶壺  2 茶入  3 茶釜  4 花入  
			if ( 0 <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 4 ) {
				// 持ち主と指定武将が一致した
				if (nb6kahou[iKahouID].attach == iBushouID + 1) {
					// 今までで最大の上昇値
					if (nb6kahou[iKahouID].param > upparam ) {
						upparam = nb6kahou[iKahouID].param;
					}
				}
			}
		}

		int maxgov = nb6bushouref[iBushouID].maxgov;
		return (maxgov - upparam) > 0 ? (maxgov - upparam) : 1;
	} else {
		return 0;
	}
}

// 家宝などを含めない素の戦闘才能
int getBushouPureBatMax(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int upparam = 0; // 上昇値
		for (int iKahouID =0; iKahouID < GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {
			// 戦闘増加タイプ：5 刀剣  6 槍  7 具足  8 馬  9 鞍  A 陣羽織   
			if ( 5 <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 0xA ) {
				// 持ち主と指定武将が一致した
				if (nb6kahou[iKahouID].attach == iBushouID + 1) {
					// 今までで最大の上昇地
					if (nb6kahou[iKahouID].param > upparam ) {
						upparam = nb6kahou[iKahouID].param;
					}
				}
			}
		}

		int maxbat = nb6bushouref[iBushouID].maxbat;
		return (maxbat - upparam) > 0 ? (maxbat - upparam) : 1;
	} else {
		return 0;
	}
}

// 家宝などを含めない素の智謀才能
int getBushouPureIntMax(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int upparam = 0; // 上昇値
		for (int iKahouID =0; iKahouID < GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {
			// 智謀増加タイプ：B 巻子本  C 明朝綴本
			if ( 0xB <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 0xC ) {
				// 持ち主と指定武将が一致した
				if (nb6kahou[iKahouID].attach == iBushouID + 1) {
					// 今までで最大の上昇地
					if (nb6kahou[iKahouID].param > upparam ) {
						upparam = nb6kahou[iKahouID].param;
					}
				}
			}
		}

		int maxint = nb6bushouref[iBushouID].maxint;
		return (maxint - upparam) > 0 ? (maxint - upparam) : 1;
	} else {
		return 0;
	}
}

// 家宝などを含めない素の野望
int getBushouPureAmbition(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int upparam = 0; // 上昇値
		for (int iKahouID =0; iKahouID < GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {
			// 野望増加タイプ：D 香木  E 地球儀  F 南蛮時計  10 水墨画  11 十字架  12 聖書  13 玉手箱  14 ビードロ杯  15 遠眼鏡  16 ルソン壺  17 煙草
			if ( 0xD <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 0x17 ) {
				// 持ち主と指定武将が一致した
				if (nb6kahou[iKahouID].attach == iBushouID + 1) {
					// 今までで最大の上昇地
					if (nb6kahou[iKahouID].param > upparam ) {
						upparam = nb6kahou[iKahouID].param;
					}
				}
			}
		}

		int ambition = nb6bushouref[iBushouID].ambition;
		return (ambition - upparam) > 0 ? (ambition - upparam) : 1;
	} else {
		return 0;
	}
}

// 官位などを含めない素の魅力
int getBushouPureCharm(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int upparam = 0; // 上昇値
		for (int iKaniID =0; iKaniID < GAMEDATASTRUCT_KANI_NUM; iKaniID++) {
			// 持ち主と指定武将が一致した
			if (nb6kani[iKaniID].attach == iBushouID + 1) {
				// 今までで最大の上昇地
				if (nb6kani[iKaniID].param > upparam ) {
					upparam = nb6kani[iKaniID].param;
				}
			}
		}

		int charm = nb6bushouref[iBushouID].charm;
		return (charm - upparam) > 0 ? (charm - upparam) : 1;
	} else {
		return 0;
	}
}



// 家宝による政治の上げ幅
int getBushouGovMaxUpByKahou(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int upparam = 0; // 上昇値
		for (int iKahouID =0; iKahouID < GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {
			// 政治増加タイプ：0 茶碗  1 茶壺  2 茶入  3 茶釜  4 花入  
			if ( 0 <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 4 ) {
				// 持ち主と指定武将が一致した
				if (nb6kahou[iKahouID].attach == iBushouID + 1) {
					// 今までで最大の上昇値
					if (nb6kahou[iKahouID].param > upparam ) {
						upparam = nb6kahou[iKahouID].param;
					}
				}
			}
		}
		return upparam;
	} else {
		return 0;
	}
}

// 家宝による戦闘の上げ幅
int getBushouBatMaxUpByKahou(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int upparam = 0; // 上昇値
		for (int iKahouID =0; iKahouID < GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {
			// 戦闘増加タイプ：5 刀剣  6 槍  7 具足  8 馬  9 鞍  A 陣羽織   
			if ( 5 <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 0xA ) {
				// 持ち主と指定武将が一致した
				if (nb6kahou[iKahouID].attach == iBushouID + 1) {
					// 今までで最大の上昇地
					if (nb6kahou[iKahouID].param > upparam ) {
						upparam = nb6kahou[iKahouID].param;
					}
				}
			}
		}
		return upparam;
	} else {
		return 0;
	}
}

// 家宝による智謀の上げ幅
int getBushouIntMaxUpByKahou(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int upparam = 0; // 上昇値
		for (int iKahouID =0; iKahouID < GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {
			// 智謀増加タイプ：B 巻子本  C 明朝綴本
			if ( 0xB <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 0xC ) {
				// 持ち主と指定武将が一致した
				if (nb6kahou[iKahouID].attach == iBushouID + 1) {
					// 今までで最大の上昇地
					if (nb6kahou[iKahouID].param > upparam ) {
						upparam = nb6kahou[iKahouID].param;
					}
				}
			}
		}

		return upparam;
	} else {
		return 0;
	}
}

// 家宝による野望の上げ幅
int getBushouAmbitionUpByKahou(int iBushouID) {
		if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int upparam = 0; // 上昇値
		for (int iKahouID =0; iKahouID < GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {
			// 野望増加タイプ：D 香木  E 地球儀  F 南蛮時計  10 水墨画  11 十字架  12 聖書  13 玉手箱  14 ビードロ杯  15 遠眼鏡  16 ルソン壺  17 煙草
			if ( 0xD <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 0x17 ) {
				// 持ち主と指定武将が一致した
				if (nb6kahou[iKahouID].attach == iBushouID + 1) {
					// 今までで最大の上昇地
					if (nb6kahou[iKahouID].param > upparam ) {
						upparam = nb6kahou[iKahouID].param;
					}
				}
			}
		}
		return upparam;
	} else {
		return 0;
	}
}


// 官位による魅力の上げ幅
int getBushouCharmUpByKani(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int upparam = 0; // 上昇値
		for (int iKaniID =0; iKaniID < GAMEDATASTRUCT_KANI_NUM; iKaniID++) {
			// 持ち主と指定武将が一致した
			if (nb6kani[iKaniID].attach == iBushouID + 1) {
				// 今までで最大の上昇地
				if (nb6kani[iKaniID].param > upparam ) {
					upparam = nb6kani[iKaniID].param;
				}
			}
		}
		return upparam;
	} else {
		return 0;
	}
}


// 官位を武将にアタッチする。元々別武将が持っていた場合は、元の武将から指定した武将へとアタッチが変わる。
// 能力値の変更を伴う。朝廷に戻す場合は、0xFFFFを指定すること。
bool ReAttachKani(int iKaniID, int iNewBushouID) {
	if ( !(0 <= iKaniID && iKaniID < GAMEDATASTRUCT_KANI_NUM) ) {
		return false;
	}

	int iOldBushouID = nb6kani[iKaniID].attach - 1;
	// 本人だ。何もする必要がない。
	if ( iOldBushouID == iNewBushouID ) {
		return true;
	}
	// 元々朝廷が持っていて、新たな指定も朝廷だ。何もする必要がない。
	if ( iOldBushouID >= 0xFFFE && iNewBushouID >= 0xFFFE )  {
		return true;
	}


	// 元の武将の政治能力の素値
	int iOldBushouPureCharm = 0;
	// 新しい指定の武将は正規の武将であり、朝廷ではない。
	if ( 0 <= iOldBushouID && iOldBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		iOldBushouPureCharm = getBushouPureCharm(iOldBushouID);
	// 不正な番号は朝廷ということにする。
	} else {
		iOldBushouID = 0xFFFF;
	}

	int iNewBushouPureCharm = 0;
	// 新しい指定の武将は正規の武将であり、朝廷ではない。
	if ( 0 <= iNewBushouID && iNewBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		iNewBushouPureCharm = getBushouPureCharm(iNewBushouID);

	// 不正な番号は朝廷ということにする。
	} else {
		iNewBushouID = 0xFFFF;
	}

	// 対象の家宝の所持者を変更する。
	nb6kani[iKaniID].attach = (iNewBushouID == 0xFFFF) ? 0xFFFF : (iNewBushouID + 1);

	// 元の武将は正規の武将であり、朝廷ではない。
	if ( 0 <= iOldBushouID && iOldBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int iOldBushouCharmUpByKani = getBushouCharmUpByKani(iOldBushouID);
		nb6bushouref[iOldBushouID].charm = iOldBushouPureCharm + iOldBushouCharmUpByKani;
	}

	// 新しい指定の武将は正規の武将であり、朝廷ではない。
	if ( 0 <= iNewBushouID && iNewBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int iNewBushouCharmUpByKani = getBushouCharmUpByKani(iNewBushouID);
		nb6bushouref[iNewBushouID].charm = iNewBushouPureCharm + iNewBushouCharmUpByKani;
	}

	return true;
}


// 家宝を武将にアタッチする。元々別武将が持っていた場合は、元の武将から指定した武将へとアタッチが変わる。
// 能力値の変更を伴う。商人に戻す場合は、0xFFFFを指定すること。
bool ReAttachKahou(int iKahouID, int iNewBushouID) {
	if ( !(0 <= iKahouID && iKahouID < GAMEDATASTRUCT_KAHOU_NUM) ) {
		return false;
	}

	// 消失は降ろす
	nb6kahou[iKahouID].bLost = false;
	// 未登場は降ろす。
	nb6kahou[iKahouID].bMitojo = false;

	int iOldBushouID = nb6kahou[iKahouID].attach - 1;
	// 本人だ。何もする必要がない。
	if ( iOldBushouID == iNewBushouID ) {
		return true;
	}
	// 元々商人が持っていて、新たな指定も商人だ。何もする必要がない。
	if ( iOldBushouID >= 0xFFFE && iNewBushouID >= 0xFFFE )  {
		return true;
	}

	// 政治増加タイプ：0 茶碗  1 茶壺  2 茶入  3 茶釜  4 花入  
	if ( 0 <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 4 ) {

		// 元の武将の政治能力の素値
		int iOldBushouPureGovMax = 0;
		// 新しい指定の武将は正規の武将であり、商人ではない。
		if ( 0 <= iOldBushouID && iOldBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			iOldBushouPureGovMax = getBushouPureGovMax(iOldBushouID);
		// 不正な番号は商人ということにする。
		} else {
			iOldBushouID = 0xFFFF;
		}

		int iNewBushouPureGovMax = 0;
		// 新しい指定の武将は正規の武将であり、商人ではない。
		if ( 0 <= iNewBushouID && iNewBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			iNewBushouPureGovMax = getBushouPureGovMax(iNewBushouID);

		// 不正な番号は商人ということにする。
		} else {
			iNewBushouID = 0xFFFF;
		}

		// 対象の家宝の所持者を変更する。
		nb6kahou[iKahouID].attach = (iNewBushouID == 0xFFFF) ? 0xFFFF : (iNewBushouID + 1);

		// 新しい指定の武将は正規の武将であり、商人ではない。
		if ( 0 <= iOldBushouID && iOldBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			int iOldBushouGovMaxUpByKahou = getBushouGovMaxUpByKahou(iOldBushouID);
			nb6bushouref[iOldBushouID].maxgov = iOldBushouPureGovMax + iOldBushouGovMaxUpByKahou;
		}

		// 新しい指定の武将は正規の武将であり、商人ではない。
		if ( 0 <= iNewBushouID && iNewBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			int iNewBushouGovMaxUpByKahou = getBushouGovMaxUpByKahou(iNewBushouID);
			nb6bushouref[iNewBushouID].maxgov = iNewBushouPureGovMax + iNewBushouGovMaxUpByKahou;
		}

	// 戦闘増加タイプ：5 刀剣  6 槍  7 具足  8 馬  9 鞍  A 陣羽織   
	} else if ( 5 <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 0xA ) {

		// 元の武将の政治能力の素値
		int iOldBushouPureBatMax = 0;
		// 新しい指定の武将は正規の武将であり、商人ではない。
		if ( 0 <= iOldBushouID && iOldBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			iOldBushouPureBatMax = getBushouPureBatMax(iOldBushouID);
		// 不正な番号は商人ということにする。
		} else {
			iOldBushouID = 0xFFFF;
		}

		int iNewBushouPureBatMax = 0;
		// 新しい指定の武将は正規の武将であり、商人ではない。
		if ( 0 <= iNewBushouID && iNewBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			iNewBushouPureBatMax = getBushouPureBatMax(iNewBushouID);

		// 不正な番号は商人ということにする。
		} else {
			iNewBushouID = 0xFFFF;
		}

		// 対象の家宝の所持者を変更する。
		nb6kahou[iKahouID].attach = (iNewBushouID == 0xFFFF) ? 0xFFFF : (iNewBushouID + 1);

		// 新しい指定の武将は正規の武将であり、商人ではない。
		if ( 0 <= iOldBushouID && iOldBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			int iOldBushouBatMaxUpByKahou = getBushouBatMaxUpByKahou(iOldBushouID);
			nb6bushouref[iOldBushouID].maxbat = iOldBushouPureBatMax + iOldBushouBatMaxUpByKahou;
		}

		// 新しい指定の武将は正規の武将であり、商人ではない。
		if ( 0 <= iNewBushouID && iNewBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			int iNewBushouBatMaxUpByKahou = getBushouBatMaxUpByKahou(iNewBushouID);
			nb6bushouref[iNewBushouID].maxbat = iNewBushouPureBatMax + iNewBushouBatMaxUpByKahou;
		}


	// 智謀増加タイプ：B 巻子本  C 明朝綴本
	} else if ( 0xB <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 0xC ) {

		// 元の武将の政治能力の素値
		int iOldBushouPureIntMax = 0;
		// 新しい指定の武将は正規の武将であり、商人ではない。
		if ( 0 <= iOldBushouID && iOldBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			iOldBushouPureIntMax = getBushouPureIntMax(iOldBushouID);
		// 不正な番号は商人ということにする。
		} else {
			iOldBushouID = 0xFFFF;
		}

		int iNewBushouPureIntMax = 0;
		// 新しい指定の武将は正規の武将であり、商人ではない。
		if ( 0 <= iNewBushouID && iNewBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			iNewBushouPureIntMax = getBushouPureIntMax(iNewBushouID);

		// 不正な番号は商人ということにする。
		} else {
			iNewBushouID = 0xFFFF;
		}

		// 対象の家宝の所持者を変更する。
		nb6kahou[iKahouID].attach = (iNewBushouID == 0xFFFF) ? 0xFFFF : (iNewBushouID + 1);

		// 新しい指定の武将は正規の武将であり、商人ではない。
		if ( 0 <= iOldBushouID && iOldBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			int iOldBushouIntMaxUpByKahou = getBushouIntMaxUpByKahou(iOldBushouID);
			nb6bushouref[iOldBushouID].maxint = iOldBushouPureIntMax + iOldBushouIntMaxUpByKahou;
		}

		// 新しい指定の武将は正規の武将であり、商人ではない。
		if ( 0 <= iNewBushouID && iNewBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			int iNewBushouIntMaxUpByKahou = getBushouIntMaxUpByKahou(iNewBushouID);
			nb6bushouref[iNewBushouID].maxint = iNewBushouPureIntMax + iNewBushouIntMaxUpByKahou;
		}

	// 野望増加タイプ：D 香木  E 地球儀  F 南蛮時計  10 水墨画  11 十字架  12 聖書  13 玉手箱  14 ビードロ杯  15 遠眼鏡  16 ルソン壺  17 煙草
	} else if  ( 0xD <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 0x17 ) {

		// 元の武将の政治能力の素値
		int iOldBushouPureAmbition = 0;
		// 新しい指定の武将は正規の武将であり、商人ではない。
		if ( 0 <= iOldBushouID && iOldBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			iOldBushouPureAmbition = getBushouPureAmbition(iOldBushouID);
		// 不正な番号は商人ということにする。
		} else {
			iOldBushouID = 0xFFFF;
		}

		int iNewBushouPureAmbition = 0;
		// 新しい指定の武将は正規の武将であり、商人ではない。
		if ( 0 <= iNewBushouID && iNewBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			iNewBushouPureAmbition = getBushouPureAmbition(iNewBushouID);

		// 不正な番号は商人ということにする。
		} else {
			iNewBushouID = 0xFFFF;
		}

		// 対象の家宝の所持者を変更する。
		nb6kahou[iKahouID].attach = (iNewBushouID == 0xFFFF) ? 0xFFFF : (iNewBushouID + 1);

		// 新しい指定の武将は正規の武将であり、商人ではない。
		if ( 0 <= iOldBushouID && iOldBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			int iOldBushouAmbitionUpByKahou = getBushouAmbitionUpByKahou(iOldBushouID);
			nb6bushouref[iOldBushouID].ambition = iOldBushouPureAmbition + iOldBushouAmbitionUpByKahou;
		}

		// 新しい指定の武将は正規の武将であり、商人ではない。
		if ( 0 <= iNewBushouID && iNewBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			int iNewBushouAmbitionUpByKahou = getBushouAmbitionUpByKahou(iNewBushouID);
			nb6bushouref[iNewBushouID].ambition = iNewBushouPureAmbition + iNewBushouAmbitionUpByKahou;
		}

	}

	return true;
}




// 顔IDから武将IDを求める。失敗時は0xFFFF
int getBushouIDFromFaceID(int iFaceID) {
	for ( int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {
		if ( nb6bushouname[iBushouID].number == iFaceID ) {
			return iBushouID;
		}
	}
	return 0xFFFF;
}


// 指定武将の伴侶のiBushouIDを得る。伴侶が居ない場合には、0xFFFFが返ってくる。
int getHanryoBushouID(int iBushouID) {

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 指定の武将の伴侶情報
		int iBushouIDSpouse = nb6bushouref[iBushouID].spouse; //

		// 結婚していない
		if ( iBushouIDSpouse == 0xFF ) {
			return 0xFFFF;
		}

		int iTsumaBushouID = iBushouIDSpouse+500;

		// 伴侶の番号が自分自身だったら、自分は妻側の立場
		if ( iTsumaBushouID == iBushouID ) {
			for ( int i=0; i<GAMEDATASTRUCT_BUSHOU_NUM; i++ ) {
				// 自分自身以外で、同じ伴侶番号を持ってるのが、伴侶の武将
				if ( i != iBushouID &&  nb6bushouref[i].spouse == iBushouIDSpouse ) {
					return i;
				}
			}
			return 0xFFFF;

		// 自分は夫側。妻にも同じ番号が設定されているか確認。
		} else {
			if ( nb6bushouref[iTsumaBushouID].spouse == iBushouIDSpouse ) {
				return iTsumaBushouID;
			// なぜか設定されていない…。改造等でデータ壊れている。
			} else {
				return 0xFFFF;
			}
		}
	} else {
		return 0xFFFF;
	}
}

// 姫(他の武将の妻含む)を、指定の武将と結婚させる、同盟関係等は一切操作されない。
bool setHimeMarriage( int iHimeID, int iOttoID, BOOL withKodokuDeath ) {

	// 同一人物。アウト。
	if ( iHimeID == iOttoID ) { return false; }

	// 夫と姫が規定の範囲に入っていなければアウト
	if (   0 <= iOttoID && iOttoID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 500 <= iHimeID && iHimeID < GAMEDATASTRUCT_BUSHOU_NUM) {

		// もし姫側のＩＤの武将が、姫でないならば、アウト
		 if ( nb6bushouname[iHimeID].State != 5 ) { // 
			return false;
		}

		int _new_spouse = iHimeID-500; // 妻に入るべき値は、姫武将番号【配列用】-500

		// すでに両者は夫婦である、何もする必要が無い。
		if ( nb6bushouref[iOttoID].spouse == _new_spouse &&  nb6bushouref[iHimeID].spouse == _new_spouse ) {
			return true;
		}

		// 夫側がすでに結婚しているか、結婚していたら離婚
		int _otto_old_spouse = nb6bushouref[iOttoID].spouse; //
		// 離婚
		setHimeDivorce( _otto_old_spouse+500, withKodokuDeath );

		nb6bushouname[iHimeID].State = 5; // 天蓋孤独処理で死亡したかもしれないので姫に戻す。

		int _tuma_old_spouse = nb6bushouref[iHimeID].spouse; //
		setHimeDivorce( _tuma_old_spouse+500, withKodokuDeath );

		nb6bushouname[iHimeID].State = 5; // 天蓋孤独処理で死亡したかもしれないので姫に戻す。

		// 身が綺麗になったので改めて結婚
		nb6bushouref[iOttoID].spouse = _new_spouse;
		nb6bushouref[iHimeID].spouse = _new_spouse;

		// 旧大名家を覚えておいて
		int OldDaimyoID = nb6bushouref[iHimeID].attach;

		// 夫側の大名の方に身を寄せる
		nb6bushouref[iHimeID].attach = nb6bushouref[iOttoID].attach;

		// 名字を夫のもので上書きする。
		strcpy( nb6bushouname[iHimeID].familyname, nb6bushouname[iOttoID].familyname );

		// 結果、大名家が変化したのであれば、士官年数もリセット
		if ( OldDaimyoID != nb6bushouref[iHimeID].attach) {
			nb6bushouref[iHimeID].work = 0;
		}

		return true;
	}

	return false;
}


// 指定の姫を離婚させる。
bool setHimeDivorce( int iHimeID, BOOL withKodokuDeath ) {

	// 夫と姫が規定の範囲に入っていなければアウト
	if ( 500 <= iHimeID && iHimeID < GAMEDATASTRUCT_BUSHOU_NUM ) {


		// もし姫側のＩＤの武将が、姫でないならば、アウト
		if ( nb6bushouname[iHimeID].State != 5 ) {
			return false;
		}

		int _tuma_spouse = nb6bushouref[iHimeID].spouse; //

		// 元々結婚していない。
		if ( _tuma_spouse == 0xFF ) {
			return false;
		}

		// 両者の結婚情報をクリア
		for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {
			// 夫もしくは妻である
			if ( nb6bushouref[iBushouID].spouse == _tuma_spouse ) {
				// 離婚
				nb6bushouref[iBushouID].spouse = 0xFF;
				// 夫と妻2人両方とも値を0xFFにする必要があるので、breakしないこと。
				// ついでにデータにゴミが入っている場合でもクリア
			}
		}


		// 旧大名家を覚えておいて
		int OldDaimyoID = nb6bushouref[iHimeID].attach;

		// 以下の身元引受人系の処理は、プログラム側で出来るだけやっておこうという趣旨。
		// 仮にここで適切な大名に帰属転移できなかったとしても、天翔記のプログラム本体が次季節時には、適切な大名に帰属させる処理を行う。

		// 離婚したので所属大名を変更する必要性がある。まず、親が大名をしているかどうか。
		for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

			// 大名のうち…
			if ( nb6bushouname[iBushouID].State == 0 ) { // 大名

				// 自分の親が大名やってる。
				if ( nb6bushouref[iHimeID].parent < 5000 ) {
					// その大名は姫の親
					if ( nb6bushouname[iBushouID].number == nb6bushouref[iHimeID].parent ) {

						// そこに身を寄せる
						nb6bushouref[iHimeID].attach = nb6bushouref[iBushouID].attach;

						strcpy( nb6bushouname[iHimeID].familyname, nb6bushouname[iBushouID].familyname );
						break;
					}
				}
			}
		}


		// 親が大名をやっていなくとも、親が同じ人(即ち兄弟姉妹)が大名をやっているかどうか。
		for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

			// 大名のうち…
			if ( nb6bushouname[iBushouID].State == 0 ) { // 大名

				// 親が同じだ。
				if ( nb6bushouref[iHimeID].parent < 5000 ) {
					if ( nb6bushouref[iBushouID].parent == nb6bushouref[iHimeID].parent ) {

						// そこに身を寄せる
						nb6bushouref[iHimeID].attach = nb6bushouref[iBushouID].attach;

						strcpy( nb6bushouname[iHimeID].familyname, nb6bushouname[iBushouID].familyname );
						break;
					}
				}
			}
		}

		// 親兄弟が大名やっていなくても、一門が大名をやっているのではないか。
		for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

			// 大名のうち…
			if ( nb6bushouname[iBushouID].State == 0 ) { // 大名

				// 親が同じだ。
				if ( nb6bushouref[iHimeID].blood != 0xFF ) {
					if ( nb6bushouref[iBushouID].blood == nb6bushouref[iHimeID].blood ) {

						// そこに身を寄せる
						nb6bushouref[iHimeID].attach = nb6bushouref[iBushouID].attach;

						strcpy( nb6bushouname[iHimeID].familyname, nb6bushouname[iBushouID].familyname );
						break;
					}
				}
			}
		}

		bool isTengaiKodoku = true;
		// 大名でなくとも、親がどこかにいるのではないか？
		for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

			// 自分の親
			if ( nb6bushouref[iHimeID].parent < 5000 ) {
				if ( nb6bushouname[iBushouID].number == nb6bushouref[iHimeID].parent ) {

					// そこに身を寄せる
					nb6bushouref[iHimeID].attach = nb6bushouref[iBushouID].attach;

					strcpy( nb6bushouname[iHimeID].familyname, nb6bushouname[iBushouID].familyname );

					// 身よりが居たので天蓋孤独ではない
					isTengaiKodoku = false;
					break;
				}
			}
		}

		// 大名でなくとも、親が同じ人(即ち兄弟姉妹)がどこかにいるのではないか？
		for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

			// 親が同じだ。
			if ( nb6bushouref[iHimeID].parent < 5000 ) {
				if ( nb6bushouref[iBushouID].parent == nb6bushouref[iHimeID].parent ) {

					// そこに身を寄せる
					nb6bushouref[iHimeID].attach = nb6bushouref[iBushouID].attach;

					strcpy( nb6bushouname[iHimeID].familyname, nb6bushouname[iBushouID].familyname );

					// 身よりが居たので天蓋孤独ではない
					isTengaiKodoku = false;
					break;
				}
			}
		}


		// 結果、大名家が変化したのであれば、士官年数もリセット
		if ( OldDaimyoID != nb6bushouref[iHimeID].attach) {
			nb6bushouref[iHimeID].work = 0;
		}

		if ( withKodokuDeath ) {
			if ( isTengaiKodoku ) {
				nb6bushouname[iHimeID].State = 7; // 天蓋孤独になったら死亡とする。
			}
		}

		return true;
	}

	return false;
}




// State 現役 → State 浪人へ
bool setGenekiToRounin(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		if ( nb6bushouname[iBushouID].State == 2 ) { // 現役武将の場合だけ処理をする。

			// 該当武将の居城がちゃんとした情報が格納されている。(ユーザー改造等によって壊れていない)
			int iCastleID = nb6bushouref[iBushouID].castle-1;
			if (0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM ) {
				if ( nb6castle[iCastleID].master == 0xFFFF ) { return false; } // リンクリストが壊れているのか城に誰も所属していない。何もしない。return;

				// メモリ上の値変更
				{
					nb6bushouname[iBushouID].State = 4; // 状態を浪人とする。
						
					nb6bushouname[iBushouID].position = 6; // 身分を足軽とする。

					nb6bushouref[iBushouID].attach = 0xFFFF; // 大名に所属しない。
					nb6bushouref[iBushouID].gundan = 0xFFFF; // 軍団に所属しない。
					nb6bushouref[iBushouID].loyal = 50; // 忠誠を50とする。
					nb6bushouref[iBushouID].work = 0; // 仕官年数を0とする。

					nb6bushouref[iBushouID].rise = 0xFFFF; // 内応先をなしとする。
					nb6bushouref[iBushouID].bRise = 0; // 内応なし。

					nb6bushouref[iBushouID].soldier = 0; // 兵数を0とする。
					nb6bushouref[iBushouID].training = 0; // 訓練を0とする。
					nb6bushouref[iBushouID].solloyal = 0; // 兵忠を0とする。
					nb6bushouref[iBushouID].form = 0; // 足軽隊とする。

					// 勲功リセットしない
					if ( TSModCommand::AllBushou::cmdNonMeritoriousReset ) {
						int merits = nb6bushouref[iBushouID].merits; // 勲功値を復元。0.7倍
						nb6bushouref[iBushouID].merits = (WORD)(0.7 * merits);
					} else {
						nb6bushouref[iBushouID].merits = 0; // 勲功を0とする。
					}
				}

				/* 　自分
				      ↓
				   ○-●-○-○

				   というのを

				   ○-○-○

				   とする。
				*/


				// 浪人にしたい武将は、いま城主だ
				if ( nb6castle[iCastleID].master-1 == iBushouID ) {
					nb6castle[iCastleID].master =  nb6bushouref[iBushouID].next; // 自分のnextを城主にする。
				}

				// 自分のprevは有効な武将だ。
				if ( 0 < nb6bushouref[iBushouID].prev && nb6bushouref[iBushouID].prev <= GAMEDATASTRUCT_BUSHOU_NUM ) {

					// 自分が城主ではない武将で、
					// １つ前の人の次のリンク先が自分自身の場合
					if ( nb6bushouref[iBushouID].prev != 0xFFFF && nb6bushouref[(nb6bushouref[iBushouID].prev-1)].next == iBushouID+1 ) {
						// 自分ではなく、次の人へと繋ぎかえる。
						nb6bushouref[(nb6bushouref[iBushouID].prev-1)].next = nb6bushouref[iBushouID].next;
					}
				}

				// 自分のnextは有効な武将だ。
				if ( 0 < nb6bushouref[iBushouID].next && nb6bushouref[iBushouID].next <= GAMEDATASTRUCT_BUSHOU_NUM ) {
					// １つ後の人の前のリンク先が自分自身の場合
					if ( nb6bushouref[iBushouID].next != 0xFFFF && nb6bushouref[(nb6bushouref[iBushouID].next-1)].prev == iBushouID+1 ) {
						// 自分ではなく、前の人へと繋ぎかえる。
						nb6bushouref[(nb6bushouref[iBushouID].next-1)].prev = nb6bushouref[iBushouID].prev;
					}
				}


				/* 　
				     
				   ○-○

				   というのを

				   ●-○-○

				   とする。
				*/

				// 浪人としてみなした時、自分のnextは、現在浪人リストの先頭の人
				nb6bushouref[iBushouID].next = nb6castle[iCastleID].ronin; // 現在の先頭の人を自分の次の人に

				// nextが有効な武将ならば…
				if ( 0 < nb6castle[iCastleID].ronin && nb6castle[iCastleID].ronin <= GAMEDATASTRUCT_BUSHOU_NUM ) {
					// 次の人のprevを自分に
					nb6bushouref[ nb6castle[iCastleID].ronin-1].prev = iBushouID+1;

				}
				nb6bushouref[iBushouID].prev = 0xFFFF; // prevは居ない

				nb6castle[iCastleID].ronin = iBushouID+1; // 城の浪人の先頭を自分にする。

				return true;

			}
		}
	}

	return false;
}


// State 浪人 → State 現役へ
bool setRouninToGeneki(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		if ( nb6bushouname[iBushouID].State == 4 ) { // 浪人武将の場合だけ処理をする。
			// 該当武将の居城がちゃんとした情報が格納されている。(ユーザー改造等によって壊れていない)
			int iCastleID = nb6bushouref[iBushouID].castle-1;
			if (0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM ) {

				if ( nb6castle[iCastleID].ronin == 0xFFFF ) { return false; } // リンクリストが壊れているのか浪人が誰も居ないことになっちゃってる。何もしない。return;
				if ( nb6castle[iCastleID].attach == 0xFFFF ) { return false; } // 該当の城がどこの軍団にも所属していない場合、浪人を現役とは出来ない。
																		       // なぜなら、それは大名化することを意味するから。
				int iAttachGundanID = nb6castle[iCastleID].attach-1;
				int iAttachDaimyoID = -1;
				if ( 0 <= iAttachGundanID && iAttachGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
					iAttachDaimyoID = nb6gundan[iAttachGundanID].attach - 1;
				} else {
					return false;
				}
				if ( ! (0 <= iAttachDaimyoID && iAttachDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM ) ) {
					return false;
				}

				// 現在の城の城主から、リンクリストを辿っていく
				int iLinkEndBusyouID = nb6castle[iCastleID].master;
				int iSafetyCnt = 0;
				while ( true ) {
					// セーフティカウントが暴走している。
					if ( iSafetyCnt > GAMEDATASTRUCT_BUSHOU_NUM ) { 
						return false;
					}

					// 城主が居ないならば、終わり
					if ( iLinkEndBusyouID == 0xFFFF ) {
						break;
					}

					// 現在の武将の、nextが有効ではなければ、現在の武将が最後の人。終わり。
					if ( nb6bushouref[iLinkEndBusyouID-1].next == 0xFFFF ) {
						break;

					// 現在の人が有効である。
					} else {
						// リンクリストを次の人とする。
						if ( 0 < iLinkEndBusyouID && iLinkEndBusyouID <= GAMEDATASTRUCT_BUSHOU_NUM ) {
							iLinkEndBusyouID = nb6bushouref[iLinkEndBusyouID-1].next;
						}
					}
				}

				// メモリ上の値変更
				{
					nb6bushouname[iBushouID].State = 2; // 状態を現役とする。
						
					nb6bushouref[iBushouID].attach = iAttachDaimyoID+1; // 大名に所属する。
					nb6bushouref[iBushouID].gundan = iAttachGundanID+1; // 軍団に所属する。
					nb6bushouref[iBushouID].loyal = 50; // 忠誠を50とする。

					nb6bushouref[iBushouID].rise = 0xFFFF; // 内応先をなしとする。
					nb6bushouref[iBushouID].bRise = 0; // 内応なし。

					nb6bushouref[iBushouID].soldier = 0; // 兵数を0とする。
					nb6bushouref[iBushouID].training = 0; // 訓練を0とする。
					nb6bushouref[iBushouID].solloyal = 0; // 兵忠を0とする。
					nb6bushouref[iBushouID].form = 0; // 足軽隊とする。

					//------------------------------------------------------
					// 勲功リセットしないモードだと、残り勲功に合わせて昇進
					if ( TSModCommand::AllBushou::cmdNonMeritoriousReset ) {

						if ( nb6bushouref[iBushouID].merits >= nb6requiredmerits[0].merits ) { // 宿老級勲功を持つ。
							nb6bushouname[iBushouID].position = 2; // 宿老へ
						} else if ( nb6bushouref[iBushouID].merits >= nb6requiredmerits[1].merits ) { // 家老級勲功を持つ。
							nb6bushouname[iBushouID].position = 3; // 家老へ
						} else if ( nb6bushouref[iBushouID].merits >= nb6requiredmerits[2].merits ) { // 部将級勲功を持つ。
							nb6bushouname[iBushouID].position = 4; // 部将へ
						} else if ( nb6bushouref[iBushouID].merits >= nb6requiredmerits[3].merits ) { // 侍大将級勲功を持つ。
							nb6bushouname[iBushouID].position = 5; // 侍大将へ
						}
					// デフォルトだと勲功はリセット
					} else {
						nb6bushouname[iBushouID].position = 6; // 身分を足軽とする。
					}
					//------------------------------------------------------
				}

				/* 　自分
				      ↓
				   ○-●-○-○

				   というのを

				   ○-○-○

				   とする。
				*/


				// 現役にしたい武将は、いま先頭だ
				if ( nb6castle[iCastleID].ronin-1 == iBushouID ) {
					nb6castle[iCastleID].ronin =  nb6bushouref[iBushouID].next; // 自分のnextを城主にする。
				}

				// 自分のprevは有効な武将だ。
				if ( 0 < nb6bushouref[iBushouID].prev && nb6bushouref[iBushouID].prev <= GAMEDATASTRUCT_BUSHOU_NUM ) {

					// 自分が先頭ではない武将で、
					// １つ前の人の次のリンク先が自分自身の場合
					if ( nb6bushouref[iBushouID].prev != 0xFFFF && nb6bushouref[(nb6bushouref[iBushouID].prev-1)].next == iBushouID+1 ) {
						// 自分ではなく、次の人へと繋ぎかえる。
						nb6bushouref[(nb6bushouref[iBushouID].prev-1)].next = nb6bushouref[iBushouID].next;
					}
				}

				// 自分のnextは有効な武将だ。
				if ( 0 < nb6bushouref[iBushouID].next && nb6bushouref[iBushouID].next <= GAMEDATASTRUCT_BUSHOU_NUM ) {
					// １つ後の人の前のリンク先が自分自身の場合
					if ( nb6bushouref[iBushouID].next != 0xFFFF && nb6bushouref[(nb6bushouref[iBushouID].next-1)].prev == iBushouID+1 ) {
						// 自分ではなく、前の人へと繋ぎかえる。
						nb6bushouref[(nb6bushouref[iBushouID].next-1)].prev = nb6bushouref[iBushouID].prev;
					}
				}


				/* 　
				     
				   ○-○

				   というのを

				   ○-○-●

				   とする。
				*/


				// 自分は現役として一番最後にくっつく。自分のnextは居ない
				nb6bushouref[iBushouID].next = 0xFFFF;

				// もし自分は、現役になるとともに、城主にもなるべきならば…
				if ( iLinkEndBusyouID == 0xFFFF ) {
					nb6bushouref[iBushouID].prev = 0xFFFF; // 自分の前の人は居ない

					nb6castle[iCastleID].master = iBushouID+1; // 城主は自分

				// 武将リンクリストの一番最後にぶら下がるべき。
				} else {

					nb6bushouref[iLinkEndBusyouID-1].next = iBushouID+1; // リンクリストの最後のnextを自分に

					nb6bushouref[iBushouID].prev = iLinkEndBusyouID; // 自分の前の人は、城のリンクリストの最後の人物
				}
				return true;

			}
		}
	}

	return false;
}


bool _setBushouTransportRonin(int iBushouID, int iNewCastleID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		if ( nb6bushouname[iBushouID].State == 4 ) { // 浪人武将の場合だけ処理をする。
			// 該当武将の居城がちゃんとした情報が格納されている。(ユーザー改造等によって壊れていない)
			int iCastleID = nb6bushouref[iBushouID].castle-1;

			if ( iCastleID == iNewCastleID ) { return true; } // 引っ越し先が同じだったら何もする必要はなく成功したとみなす。

			if (0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM ) {

				if ( nb6castle[iCastleID].ronin == 0xFFFF ) { return false; } // リンクリストが壊れているのか浪人が誰も居ないことになっちゃってる。何もしない。return;

				// メモリ上の値変更
				{
					// 所属居城を転送先に変更する。
					nb6bushouref[iBushouID].castle = iNewCastleID + 1;				
				}

				/* 　自分
				      ↓
				   ○-●-○-○

				   というのを

				   ○-○-○

				   とする。
				*/


				// 転送したい武将は、いま先頭だ
				if ( nb6castle[iCastleID].ronin-1 == iBushouID ) {
					nb6castle[iCastleID].ronin =  nb6bushouref[iBushouID].next; // 自分のnextを城主にする。
				}

				// 自分のprevは有効な武将だ。
				if ( 0 < nb6bushouref[iBushouID].prev && nb6bushouref[iBushouID].prev <= GAMEDATASTRUCT_BUSHOU_NUM ) {

					// 自分が先頭ではない武将で、
					// １つ前の人の次のリンク先が自分自身の場合
					if ( nb6bushouref[iBushouID].prev != 0xFFFF && nb6bushouref[(nb6bushouref[iBushouID].prev-1)].next == iBushouID+1 ) {
						// 自分ではなく、次の人へと繋ぎかえる。
						nb6bushouref[(nb6bushouref[iBushouID].prev-1)].next = nb6bushouref[iBushouID].next;
					}
				}

				// 自分のnextは有効な武将だ。
				if ( 0 < nb6bushouref[iBushouID].next && nb6bushouref[iBushouID].next <= GAMEDATASTRUCT_BUSHOU_NUM ) {
					// １つ後の人の前のリンク先が自分自身の場合
					if ( nb6bushouref[iBushouID].next != 0xFFFF && nb6bushouref[(nb6bushouref[iBushouID].next-1)].prev == iBushouID+1 ) {
						// 自分ではなく、前の人へと繋ぎかえる。
						nb6bushouref[(nb6bushouref[iBushouID].next-1)].prev = nb6bushouref[iBushouID].prev;
					}
				}

				/* 　
				     
				   ○-○

				   というのを

				   ●-○-○

				   とする。
				*/

				// 浪人としてみなした時、自分のnextは、現在浪人リストの先頭の人
				nb6bushouref[iBushouID].next = nb6castle[iNewCastleID].ronin; // 転送先の現在の先頭の人を自分の次の人に

				// nextが有効な武将ならば…
				if ( 0 < nb6castle[iNewCastleID].ronin && nb6castle[iNewCastleID].ronin <= GAMEDATASTRUCT_BUSHOU_NUM ) {
					// 次の人のprevを自分に
					nb6bushouref[ nb6castle[iNewCastleID].ronin-1].prev = iBushouID+1;
				}
				nb6bushouref[iBushouID].prev = 0xFFFF; // prevは居ない

				nb6castle[iNewCastleID].ronin = iBushouID+1; // 城の浪人の先頭を自分にする。

				return true;
			}
		}
	}

	return false;
}


bool setBushouTransport(int iBushouID, int iNewCastleID) {

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 今の居城と新しい居城
		int iCastleID = nb6bushouref[iBushouID].castle-1;
		if ( iCastleID == iNewCastleID ) { return true; } // 今居る場所と、転居先が同じなら何もする必要はない。

		// 浪人の場合は移転するだけ
		if ( nb6bushouname[iBushouID].State == 4 ) { // 浪人武将の場合だけ処理をする。
			return _setBushouTransportRonin(iBushouID, iNewCastleID);

		// 現役の場合は、浪人化してから、移転して、また(可能であれば)現役に戻す。
		// 未統治城だと浪人のまま移転することとなる。
		} else if ( nb6bushouname[iBushouID].State == 2 ) { // 現役武将である
			int _merits = nb6bushouref[iBushouID].merits;
			int _work = nb6bushouref[iBushouID].work;
			int _position = nb6bushouname[iBushouID].position;
			int _loyal = nb6bushouref[iBushouID].loyal;
			int _soldier = nb6bushouref[iBushouID].soldier;
			int _training = nb6bushouref[iBushouID].training;
			int _solloyal = nb6bushouref[iBushouID].solloyal;
			int _form = nb6bushouref[iBushouID].form;
			int _attach = nb6bushouref[iBushouID].attach;
			int _rise = nb6bushouref[iBushouID].rise;
			int _bRise = nb6bushouref[iBushouID].bRise;

			// 一端、浪人にして
			bool isRouninka = setGenekiToRounin(iBushouID);
			if (! isRouninka) { return false; }

			// 位置移動
			bool isTranska = _setBushouTransportRonin(iBushouID, iNewCastleID);
			if (! isTranska) { return false; }

			bool isGenekika = setRouninToGeneki(iBushouID);
			// 現役にできなかったらそのまま浪人として終わる。(未支配城とかだと、そのまま浪人のままとなる。)
			if (! isGenekika) { return false; }

			// 元々現役なので、浪人化する前の値で復元するものを戻す。
			nb6bushouref[iBushouID].merits = _merits; // 勲功
			nb6bushouname[iBushouID].position = _position; // 身分
			nb6bushouref[iBushouID].loyal = _loyal; // 忠誠
			nb6bushouref[iBushouID].soldier = _soldier; // 兵数
			nb6bushouref[iBushouID].training = _training; // 訓練
			nb6bushouref[iBushouID].solloyal = _solloyal; // 士気
			nb6bushouref[iBushouID].form = _form; // 兵態

			// 転居前と転居後で大名が変化していない場合のみ、士官年数や内応系も元の値を復元
			if (_attach == nb6bushouref[iBushouID].attach) {
				nb6bushouref[iBushouID].work = _work; // 仕官年数
				nb6bushouref[iBushouID].rise = _rise; // 内応先
				nb6bushouref[iBushouID].bRise = _bRise; // 内応状態
			} else {
				nb6bushouref[iBushouID].grudge = 0; // 大名が変わった場合、恨みは消す
			}

		}
	}
	return false;
}


// 現役をその城の城主とする。
// 特定の「現役」の武将を、その城の城主にする。
// 元の城主が「軍団長」や「大名」だと、その人物の立場となる。
bool setGenekiToJyosyu(int iBushouID) {

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		//0大名,1軍長,2現役
		if ( nb6bushouname[iBushouID].State == 2 ) { // 現役武将の場合だけ処理をする。

			// 該当武将の居城がちゃんとした情報が格納されている。(ユーザー改造等によって壊れていない)
			int iCastleID = nb6bushouref[iBushouID].castle-1;

			if (0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM ) {

				if ( nb6castle[iCastleID].master == 0xFFFF ) { return false; } // リンクリストが壊れているのか城に誰も所属していない。何もしない。return;

				// ここまで来たら城主が居るので…
				int iJyosyuBushouID = nb6castle[iCastleID].master-1;

				// 指定武将がすでに城主なら何もしなくても城主である。
				if ( iBushouID== iJyosyuBushouID ) { return true; }

				// 城主は居るが(改造等で)おかしな破たんデータになっていたら、何もしない。
				if ( ! (0 <= iJyosyuBushouID && iJyosyuBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) ) {
					return false;
				}

				//0大名,1軍長,2現役

				// 城主が現役でも軍団長でもないならば(大名)、何もしない。入れ替えると変なことになる。
				// 現役ならば
				if ( nb6bushouname[iJyosyuBushouID].State == 2 ) {	

					; // 特に何もしない
				// 軍団長ならば
				} else if ( nb6bushouname[iJyosyuBushouID].State == 1 ) {
					// 今の軍団長を現役とする
					nb6bushouname[iJyosyuBushouID].State = 2; // 現役とする
					
					// 逆に指定武将を軍団長化する
					nb6bushouname[iBushouID].State = 1; // 軍団長
					int iGundanID = nb6bushouref[iJyosyuBushouID].gundan-1;
					// 決して要らぬ判定のはずであるが一応
					if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
						// 軍団情報の軍団長の番号を、指定の武将にする。
						nb6gundan[iGundanID].leader = iBushouID + 1;
					}
				// 大名ならば…
				} else if ( nb6bushouname[iJyosyuBushouID].State == 0 ) {
					// 今の大名を現役とする
					nb6bushouname[iJyosyuBushouID].State = 2;
					// 大名だった人の身分を宿老とする。
					nb6bushouname[iJyosyuBushouID].position = 2; //(隠居=0, 大名=1, 宿老=2, 家老=3, 部将=4, 侍大将=5, 足軽頭=6)
					// 逆に指定武将を大名とする
					nb6bushouname[iBushouID].State = 0;
					nb6bushouname[iBushouID].position = 1;

					int iGundanID = nb6bushouref[iJyosyuBushouID].gundan-1;
					int iDaimyoID = nb6bushouref[iJyosyuBushouID].attach-1;
					// 決して要らぬ判定のはずであるが一応
					if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
						// 軍団情報の軍団長の番号を、指定の武将にする。
						nb6gundan[iGundanID].leader = iBushouID + 1;
					}
					if ( 0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM ) {
						// nb6daimyoの大名の番号を、指定の武将にする。
						nb6daimyo[iDaimyoID].attach = iBushouID + 1;
					}

				} else {
					return false;
				}




				/*城情報の武将リスト

				　 　自分
				   ↓
				   ○-●-○-○

				   というのを

				   ○-○-○

				   とする。
				   */


				// パージしたい武将は、いま城主だ
				if ( nb6castle[iCastleID].master-1 == iBushouID ) {
					nb6castle[iCastleID].master =  nb6bushouref[iBushouID].next; // 自分のnextを城主にする。
				}

				// 自分のprevは有効な武将だ。
				if ( 0 < nb6bushouref[iBushouID].prev && nb6bushouref[iBushouID].prev <= GAMEDATASTRUCT_BUSHOU_NUM ) {

					// 自分が城主ではない武将で、
					// １つ前の人の次のリンク先が自分自身の場合
					if ( nb6bushouref[iBushouID].prev != 0xFFFF && nb6bushouref[(nb6bushouref[iBushouID].prev-1)].next == iBushouID+1 ) {
						// 自分ではなく、次の人へと繋ぎかえる。
						nb6bushouref[(nb6bushouref[iBushouID].prev-1)].next = nb6bushouref[iBushouID].next;
					}
				}

				// 自分のnextは有効な武将だ。
				if ( 0 < nb6bushouref[iBushouID].next && nb6bushouref[iBushouID].next <= GAMEDATASTRUCT_BUSHOU_NUM ) {
					// １つ後の人の前のリンク先が自分自身の場合
					if ( nb6bushouref[iBushouID].next != 0xFFFF && nb6bushouref[(nb6bushouref[iBushouID].next-1)].prev == iBushouID+1 ) {
						// 自分ではなく、前の人へと繋ぎかえる。
						nb6bushouref[(nb6bushouref[iBushouID].next-1)].prev = nb6bushouref[iBushouID].prev;
					}
				}



				/* 城情報の城主リスト　

				○-○

				というのを

				●-○-○

				とする。
				*/

				// 自分が城主となるので、自分の次の人を、今の城主の人とする。
				int iJosyu = nb6castle[iCastleID].master;
				nb6bushouref[iBushouID].next = iJosyu; // 現在の先頭の人を自分の次の人に

				// nextが有効な武将ならば…
				if ( 0 < iJosyu && iJosyu <= GAMEDATASTRUCT_BUSHOU_NUM ) {
					// 次の人のprevを自分に
					nb6bushouref[iJosyu-1].prev = iBushouID+1;
				}
				
				nb6bushouref[iBushouID].prev = 0xFFFF; // 本人が先頭になるので、prevは居ない

				// そして最後に自分を城主に
				nb6castle[iCastleID].master = iBushouID+1; // 城の城主を自分にする。

				return true;
			}
		}
	}

	return false;
}



// bfile.nb6に存在する列伝の数
int iExistRetudenNumOfBFileN6P = -1;



// 対象の武将IDが現在鉄甲船状態かどうか(鉄甲船を持っているかどうかではなく、戦場において、今鉄甲船の形状かどうか)
int iCurTekkousenForm[GAMEDATASTRUCT_BUSHOU_NUM] = {-1};


int getTouzyouBushouNum() {
	WORD *pTouzyouBushouNum = (WORD *)GAMEDATASTRUCT_TOUZYOU_BUSHOU_NUM_ADDRESS;
	return *pTouzyouBushouNum;
}

int getTaikiBushouDataNum() {
	WORD *pMitouzyouBushouNum = (WORD *)GAMEDATASTRUCT_MITOUZYOU_BUSHOU_NUM_ADDRESS;
	return *pMitouzyouBushouNum;
}



// ロードしないでやってる経過季節。毎季節の変わり目で加算されてゆく。
int nb6progressseason = 1L;


void setCastleName(int iCastleID, string name) {
	if (0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM) {

		strncpy_s(nb6castle[iCastleID].name, name.c_str(), 8);
		nb6castle[iCastleID].name[8] = NULL;
		nb6castle[iCastleID].nameEdited = true;
	}
}


// 勝手に文字列を決めうちせずに、TENSHOU.EXE内のポインタ配列から引っ張るようにした方が、他者の改造への耐久力が増すかも。
char nb6CastleTypeName[GAMEDATASTRUCT_CASTLETYPE_NUM][5] = {
	"城",
	"御坊",
	"寺",
	"御所",
	"館",
	"",
	"城"
};

// 城のタイプIDから具体的な文字列を得る
char *getCastleTypeName(int iCastleType) {
	if ( iCastleType == 0x00 ) {
		return nb6CastleTypeName[0];
	}
	else if (iCastleType == 0x01 ) {
		return nb6CastleTypeName[1];
	}
	else if (iCastleType == 0x02 ) {
		return nb6CastleTypeName[2];
	}
	else if (iCastleType == 0x04 ) {
		return nb6CastleTypeName[3];
	}
	else if (iCastleType == 0x08 ) {
		return nb6CastleTypeName[4];
	}
	else if (iCastleType == 0x10 ) {
		return nb6CastleTypeName[5];
	}
	else if (iCastleType == 0x20 ) {
		return nb6CastleTypeName[6];
	}
	return "";
}


// 指定の軍団が所持している城の数。
int getGundanHasCastleNum(int iGundanID) {
	int iHasCastleCnt = 0;
	if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
		for ( int iCastleID=0; iCastleID < GAMEDATASTRUCT_CASTLE_NUM; iCastleID++ ) {
			// 軍団番号が一致するものをカウント
			if ( iGundanID == nb6castle[iCastleID].attach-1 ) {
				iHasCastleCnt++;
			}
		}
	}
	return iHasCastleCnt;
}

// 指定の大名が所持している城の数。
int getDaimyoHasCastleNum(int iDaimyoID) {
	int iHasCastleCnt = 0;
	
	if ( 0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM ) {
		for ( int iCastleID=0; iCastleID < GAMEDATASTRUCT_CASTLE_NUM; iCastleID++ ) {
			int iGundanID = nb6castle[iCastleID].attach-1;

			if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
				// 大名番号が一致するものをカウント
				if ( iDaimyoID == nb6gundan[iGundanID].attach-1 ) {
					iHasCastleCnt++;
				}
			}
		}
	}
	return iHasCastleCnt;
}

// 指定の大名が所持している城の番号【配列用】のリスト
vector<int> getDaimyoHasCastleList(int iDaimyoID) {
	vector<int> castlelist;

	if (0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM) {
		for (int iCastleID = 0; iCastleID < GAMEDATASTRUCT_CASTLE_NUM; iCastleID++) {
			int iGundanID = nb6castle[iCastleID].attach - 1;

			if (0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM) {
				// 大名番号が一致するものをカウント
				if (iDaimyoID == nb6gundan[iGundanID].attach - 1) {
					castlelist.push_back(iCastleID);
				}
			}
		}
	}
	return castlelist;
}



// 野戦時のヘックス系の座標を、配列としての実値のindexへと変更する。
void ConvertFieldHexPosToFieldIndexPos(double x, double y, int *out_x, int *out_y) {

	// 座標値は妥当な範囲か？
	if ( 0<=x && x< GAMEDATASTRUCT_FIELD_HEX_COLS && 0<=y && y< GAMEDATASTRUCT_FIELD_HEX_ROWS) {
		// yを座標実値にして、整数化
		int _y = round_整数化(y);
		// 野戦の偶数行目か奇数行目かで、半身ずらす。偶数行なら、"半HEX"多い値がxに入ってくるため。
		if (_y % 2==1) {
			x = x - 0.5;
		}
		// xを最も近い値の整数化
		int _x = round_整数化(x);
		// ０より小さくなることもあるので０に
		if ( _x < 0 ) { _x = 0; }
		if ( _x >= GAMEDATASTRUCT_FIELD_HEX_COLS ) { _x = GAMEDATASTRUCT_FIELD_HEX_COLS-1; }
		if ( _y >= GAMEDATASTRUCT_FIELD_HEX_ROWS ) { _y = GAMEDATASTRUCT_FIELD_HEX_ROWS-1; }

		// 配列アクセス値としてのxとyが出た
		*out_x = _x;
		*out_y = _y;
	} else {
		*out_x = 0xFFFF;
		*out_y = 0xFFFF;
	}
}


// 攻城戦時のヘックス系の座標を、配列としての実値のindexへと変更する。
void ConvertCastleHexPosToCastleIndexPos(double x, double y, int *out_x, int *out_y) {

	// 座標値は妥当な範囲か？
	if ( 0<=x && x< GAMEDATASTRUCT_CASTLE_HEX_COLS && 0<=y && y< GAMEDATASTRUCT_CASTLE_HEX_ROWS) {
		// yを座標実値にして、整数化
		int _y = round_整数化(y);
		// 野戦の偶数行目か奇数行目かで、半身ずらす。偶数行なら、"半HEX"多い値がxに入ってくるため。
		if (_y % 2==1) {
			x = x - 0.5;
		}
		// xを最も近い値の整数化
		int _x = round_整数化(x);
		// ０より小さくなることもあるので０に
		if ( _x < 0 ) { _x = 0; }
		if ( _x >= GAMEDATASTRUCT_CASTLE_HEX_COLS ) { _x = GAMEDATASTRUCT_CASTLE_HEX_COLS-1; }
		if ( _y >= GAMEDATASTRUCT_CASTLE_HEX_ROWS ) { _y = GAMEDATASTRUCT_CASTLE_HEX_ROWS-1; }

		// 配列アクセス値としてのxとyが出た
		*out_x = _x;
		*out_y = _y;
	} else {
		*out_x = 0xFFFF;
		*out_y = 0xFFFF;
	}
}

// あるヘックス場所のチップの役割(FIELD_HEX_ROLE)を得る
int GetFieldHexRole(double x, double y) {
	// 座標値は妥当な範囲か？
	if ( 0<=x && x<= GAMEDATASTRUCT_FIELD_HEX_COLS && 0<=y && y<=GAMEDATASTRUCT_FIELD_HEX_ROWS) {
		// yを座標実値にして、整数化
		int _y = round_整数化(y);
		// 野戦の偶数行目か奇数行目かで、半身ずらす。偶数行なら、"半HEX"多い値がxに入ってくるため。
		if (_y % 2==1) {
			x = x - 0.5;
		}
		// xを最も近い値の整数化
		int _x = round_整数化(x);
		// ０より小さくなることもあるので０に
		if ( _x < 0 ) { _x = 0; }
		if ( _x >= GAMEDATASTRUCT_FIELD_HEX_COLS ) { _x = GAMEDATASTRUCT_FIELD_HEX_COLS-1; }
		if ( _y >= GAMEDATASTRUCT_FIELD_HEX_ROWS ) { _y = GAMEDATASTRUCT_FIELD_HEX_ROWS-1; }
		// 該当の場所にあるチップの役割番号(FIELD_HEX_ROLE)を返す
		return nb6field_hex_role.pos_y[_y].pos_x[_x];
	} else {
		return -1;
	}
}

int GetFieldHexRole(HEX_POSITION p) {
	return GetFieldHexRole(p.x, p.y);
}

// 指定の野戦の役割と合致する、位置リストを得る
// role には FIELD_HEX_ROLE::* の値を指定する
vector<HEX_POSITION> GetFieldHexPosOfHexRole(int role) {
	vector<HEX_POSITION> vec;
	for (int x = 0; x < GAMEDATASTRUCT_FIELD_HEX_COLS; x++) {
		for (int y = 0; y < GAMEDATASTRUCT_FIELD_HEX_ROWS; y++) {
			if (nb6field_hex_role.pos_y[y].pos_x[x] == role) {

				HEX_POSITION pos;
				if (y % 2 == 1) {
					pos.x = (double)x + 0.5;
					pos.y = y;
				}
				else {
					pos.x = x;
					pos.y = y;
				}

				vec.push_back(pos);
			}
		}
	}

	return vec;
}



// あるヘックス場所のチップの役割(CASTLE_HEX_ROLE)を得る
int GetCastleHexRole(double x, double y) {
	// 座標値は妥当な範囲か？
	if ( 0<=x && x< GAMEDATASTRUCT_CASTLE_HEX_COLS && 0<=y && y< GAMEDATASTRUCT_CASTLE_HEX_ROWS) {
		// yを座標実値にして、整数化
		int _y = round_整数化(y);
		// 野戦の偶数行目か奇数行目かで、半身ずらす。偶数行なら、"半HEX"多い値がxに入ってくるため。
		if (_y % 2==1) {
			x = x - 0.5;
		}
		// xを最も近い値の整数化
		int _x = round_整数化(x);
		// ０より小さくなることもあるので０に
		if ( _x < 0 ) { _x = 0; }
		if ( _x >= GAMEDATASTRUCT_CASTLE_HEX_COLS ) { _x = GAMEDATASTRUCT_CASTLE_HEX_COLS-1; }
		if ( _y >= GAMEDATASTRUCT_CASTLE_HEX_ROWS ) { _y = GAMEDATASTRUCT_CASTLE_HEX_ROWS-1; }
		// 該当の場所にあるチップの役割番号(CASTLE_HEX_ROLE)を返す
		return nb6castle_hex_role.pos_y[_y].pos_x[_x];
	} else {
		return -1;
	}
}


int GetCastleHexRole(HEX_POSITION p) {
	return GetCastleHexRole(p.x, p.y);
}


// 指定の攻城戦の役割と合致する、位置リストを得る
// role には CASTLE_HEX_ROLE::* の値を指定する
vector<HEX_POSITION> GetCastleHexPosOfHexRole(int role) {
	vector<HEX_POSITION> vec;
	for (int x = 0; x < GAMEDATASTRUCT_CASTLE_HEX_COLS; x++) {
		for (int y = 0; y < GAMEDATASTRUCT_CASTLE_HEX_ROWS; y++) {
			if (nb6castle_hex_role.pos_y[y].pos_x[x] == role) {

				HEX_POSITION pos;
				if (y % 2 == 1) {
					pos.x = (double)x + 0.5;
					pos.y = y;
				}
				else {
					pos.x = x;
					pos.y = y;
				}

				vec.push_back(pos);
			}
		}
	}

	return vec;
}



/* 下で右１バイトが１の所は炎が付いている
004D4DAA                          10 10 10 10 10 10 10 10          
004D4DBA  10 00 00 00 00 00 00 00 00 10 10 10 11 98 90 10  ........・
004D4DCA  10 10 10 00 00 00 00 00 00 00 00 10 10 10 10 90  ........・
004D4DDA  90 90 10 10 10 10 00 00 00 00 00 00 00 10 10 10  瑞.......
004D4DEA  10 91 90 10 10 10 10 00 00 00 00 00 00 00 00 00  草.........
004D4DFA  10 10 10 11 10 10 10 10 10 00 00 00 00 00 00 00  .......
004D4E0A  00 00 10 10 11 11 11 10 10 10 00 00 00 00 00 00  ........
004D4E1A  00 00 00 00 00 10 10 10 11 10 10 10 00 00 00 00  .........
004D4E2A  00 00 00 00 00 00 00 10 10 10 10 10 10 00 00 00  ..........
*/
// あるヘックス場所のチップの炎の役割(CASTLE_HEX_ROLE)を得る
// 炎が付いている所は、右の１バイトが1になる。
int GetFireHexRole(double x, double y) {
	// 座標値は妥当な範囲か？
	if ( 0<=x && x< GAMEDATASTRUCT_CASTLE_HEX_COLS && 0<=y && y< GAMEDATASTRUCT_CASTLE_HEX_ROWS) {
		// yを座標実値にして、整数化
		int _y = round_整数化(y);
		// 野戦の偶数行目か奇数行目かで、半身ずらす。偶数行なら、"半HEX"多い値がxに入ってくるため。
		if (_y % 2==1) {
			x = x - 0.5;
		}
		// xを最も近い値の整数化
		int _x = round_整数化(x);
		// ０より小さくなることもあるので０に
		if ( _x < 0 ) { _x = 0; }
		if ( _x >= GAMEDATASTRUCT_CASTLE_HEX_COLS ) { _x = GAMEDATASTRUCT_CASTLE_HEX_COLS-1; }
		if ( _y >= GAMEDATASTRUCT_CASTLE_HEX_ROWS ) { _y = GAMEDATASTRUCT_CASTLE_HEX_ROWS-1; }
		// 該当の場所にあるチップの役割番号(CASTLE_HEX_ROLE)を返す
		return (nb6fire_hex_role.pos_y[_y].pos_x[_x] & 0xF) == 1;
	} else {
		return -1;
	}
}


int GetFireHexRole(HEX_POSITION p) {
	return GetFireHexRole(p.x, p.y);
}


// 指定の攻城戦の炎上状態と合致する、位置リストを得る
// role には CASTLE_HEX_ROLE::* の値を指定する。指定しなければ炎上状態のものを集める
vector<HEX_POSITION> GetFireeHexPosOfHexRole(int role = FIRE_HEX_ROLE::炎上) {
	vector<HEX_POSITION> vec;
	for (int x = 0; x < GAMEDATASTRUCT_CASTLE_HEX_COLS; x++) {
		for (int y = 0; y < GAMEDATASTRUCT_CASTLE_HEX_ROWS; y++) {
			if ((nb6fire_hex_role.pos_y[y].pos_x[x] & 0xF) == role) {

				HEX_POSITION pos;
				if (y % 2 == 1) {
					pos.x = (double)x + 0.5;
					pos.y = y;
				}
				else {
					pos.x = x;
					pos.y = y;
				}

				vec.push_back(pos);
			}
		}
	}

	return vec;
}



// あるヘックス場所のチップの高さを得る
int GetFieldHexHigh(double x, double y) {
	int role = GetFieldHexRole(x, y);
	if ( role == FIELD_HEX_ROLE::川 || FIELD_HEX_ROLE::湖 || FIELD_HEX_ROLE::海 ) {
		return 0;
	} else if ( role == FIELD_HEX_ROLE::低山 ) {
		return 2;
	} else if ( role == FIELD_HEX_ROLE::中山 ) {
		return 3;
	} else if ( role == FIELD_HEX_ROLE::高山 ) {
		return 4;
	} else {
		return 1;
	}
}

int GetFieldHexHigh(HEX_POSITION p) {
	return GetFieldHexHigh(p.x, p.y);
}

// あるヘックス場所のチップの高さを得る
int GetCastleHexHigh(double x, double y) {
	// 座標値は妥当な範囲か？
	if ( 0<=x && x< GAMEDATASTRUCT_CASTLE_HEX_COLS && 0<=y && y< GAMEDATASTRUCT_CASTLE_HEX_ROWS) {
		// yを座標実値にして、整数化
		int _y = round_整数化(y);
		// 野戦の偶数行目か奇数行目かで、半身ずらす。偶数行なら、"半HEX"多い値がxに入ってくるため。
		if (_y % 2==1) {
			x = x - 0.5;
		}
		// xを最も近い値の整数化
		int _x = round_整数化(x);
		// ０より小さくなることもあるので０に
		if ( _x < 0 ) { _x = 0; }
		if ( _x >= GAMEDATASTRUCT_CASTLE_HEX_COLS ) { _x = GAMEDATASTRUCT_CASTLE_HEX_COLS-1; }
		if ( _y >= GAMEDATASTRUCT_CASTLE_HEX_ROWS ) { _y = GAMEDATASTRUCT_CASTLE_HEX_ROWS-1; }
		// 該当の場所にあるチップの役割番号(CASTLE_HEX_ROLE)を返す
		return nb6castle_hex_high.pos_y[_y].pos_x[_x];
	} else {
		return -1;
	}
}

int GetCastleHexHigh(HEX_POSITION p) {
	return GetCastleHexHigh(p.x, p.y);
}


// 逆に設定する。(といっても、設定したとしても、絵が変わらないので上手くいかないが)
void SetFieldHexRole(double x, double y, int eHexRole) {
	// 座標値は妥当な範囲か？
	if ( 0<=x && x< GAMEDATASTRUCT_FIELD_HEX_COLS && 0<=y && y< GAMEDATASTRUCT_FIELD_HEX_ROWS) {
		// yを座標実値にして、整数化
		int _y = round_整数化(y);
		// 野戦の偶数行目か奇数行目かで、半身ずらす。偶数行なら、"半HEX"多い値がxに入ってくるため。
		if (_y % 2==1) {
			x = x - 0.5;
		}
		// xを最も近い値の整数化
		int _x = round_整数化(x);
		// ０より小さくなることもあるので０に
		if ( _x < 0 ) { _x = 0; }
		if ( _x >= GAMEDATASTRUCT_FIELD_HEX_COLS ) { _x = GAMEDATASTRUCT_FIELD_HEX_COLS-1; }
		if ( _y >= GAMEDATASTRUCT_FIELD_HEX_ROWS ) { _y = GAMEDATASTRUCT_FIELD_HEX_ROWS-1; }
		// 該当の場所のチップの役割番号(FIELD_HEX_ROLE)を設定する。
		nb6field_hex_role.pos_y[_y].pos_x[_x] = eHexRole;
	}
}

void SetFieldHexRole(HEX_POSITION p, int eHexRole) {
	SetFieldHexRole(p.x, p.y, eHexRole);
}

// フィールドヘックスの座標→普通の３次元座標(左手デカルト・音と同じへ)
D3D_POSITION FieldHexPos_To_WorldPos(HEX_POSITION p) {
	D3D_POSITION p_real;
	p_real.x = p.x - GAMEDATASTRUCT_FIELD_HEX_CENTER_X;
	p_real.z = (p.y -GAMEDATASTRUCT_FIELD_HEX_CENTER_Y) * ( sqrt(3.0f) / 2.0f ) * -1; // sin60°
	if ( p_real.z == 0 ) { p_real.z = 0; } // アホみたいだが、"-0"とならないようにする。

	// yの高さを決めるにあたって、低山なら1, 中山なら2, 高山なら3とする。
	int iHexRole = GetFieldHexRole( p.x, p.y );

	if ( iHexRole == FIELD_HEX_ROLE::高山 ) {
		p_real.y = 3;
	} else if ( iHexRole == FIELD_HEX_ROLE::中山 ) {
		p_real.y = 2;
	} else if ( iHexRole == FIELD_HEX_ROLE::低山 ) {
		p_real.y = 1;
	} else {
		p_real.y = 0;
	}
	return p_real;
}

// 普通の３次元座標(左手デカルト)→フィールドヘックスの座標へ
HEX_POSITION WorldPos_To_FieldHexPos(D3D_POSITION p) {
	HEX_POSITION p_hex;
	p_hex.x = round_整数化(p.x + GAMEDATASTRUCT_FIELD_HEX_CENTER_X);
	p_hex.y = round_二分の一吸着化((2.0f / sqrt(3.0f) ) * (p.z) * -1 + GAMEDATASTRUCT_FIELD_HEX_CENTER_Y);
	return p_hex;
}

// 城ヘックスの座標→普通の３次元座標(左手デカルト・音と同じへ)
D3D_POSITION CastleHexPos_To_WorldPos(HEX_POSITION p) {
	D3D_POSITION p_real;
	p_real.x = p.x - GAMEDATASTRUCT_CASTLE_HEX_CENTER_X;
	p_real.z = (p.y -GAMEDATASTRUCT_CASTLE_HEX_CENTER_Y) * ( sqrt(3.0f) / 2.0f ) * -1; // sin60°
	if ( p_real.z == 0 ) { p_real.z = 0; } // アホみたいだが、"-0"とならないようにする。

	// yの高さを決めるにあたって城の高さ情報を当てはめる。
	int iHexHigh = GetCastleHexHigh( p.x, p.y );
	p_real.y = iHexHigh;
	return p_real;
}

// 普通の３次元座標(左手デカルト)→城ヘックスの座標へ
HEX_POSITION WorldPos_To_CastleHexPos(D3D_POSITION p) {
	HEX_POSITION p_hex;
	p_hex.x = round_整数化(p.x + GAMEDATASTRUCT_CASTLE_HEX_CENTER_X);
	p_hex.y = round_二分の一吸着化((2.0f / sqrt(3.0f) ) * (p.z) * -1 + GAMEDATASTRUCT_CASTLE_HEX_CENTER_Y);
	return p_hex;
}


// ヘックス間隔の取得。p1→p2まで何枡移動する必要があるのか(全ての枡は行けるものと仮定する)
int getHexDistance( HEX_POSITION p1, HEX_POSITION p2 ) {
	double ax = double(abs(p1.x-p2.x));
	double ay = double(abs(p1.y-p2.y));
	if ( p1.y == p2.y ) { // 同じ行に居るならば…
		return round_整数化(ax);
	} else if ( ax*2 <= ay ) { // 2点間のX座標の差が2点間のY座標の差の半分以下の場合、Y座標の移動だけで該当ポイントまで到達できる。
		return round_整数化(ay);
	} else {
		// Xの差 + Yの差 - Yの差の半分
		return round_整数化( ax + ay - 0.5*ay );
	}
}

// 現在出陣しているiBushouIDの一覧を得る。
vector<int> GetSyutujinBushouArray() {
	vector<int> bArray;

	// 戦場にいる武将から条件を満たすものの移動残量を増やす。
	for ( int i=0; i<GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_NUM; i++ ) {
		if ( nb6bushou_hex_unit_info[i].bushou == 0xFFFF ) { continue; } // 0xFFFFだとそのスロットに武将は居ない。次の人
		if ( nb6bushou_hex_unit_info[i].bushou > GAMEDATASTRUCT_BUSHOU_NUM ) { continue; } // ここで範囲保障しておいてあげる。次の人
		
		int iBushouID = nb6bushou_hex_unit_info[i].bushou - 1;

		if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			// 戦死や退却していない
			if ( nb6bushouname[iBushouID].battleDeath == 0 && nb6bushouname[iBushouID].battleRetreat == 0 ) {
				bArray.push_back( iBushouID );
			}
		}
	}

	return bArray;
}

// 現在表示されているマップ限定で、出陣しているiBushouIDの一覧を得る。
vector<int> GetCurMapSyutujinBushouArray() {
	vector<int> bArray;

	if ( Is_CastleWar() ) {
		for ( int i=0; i<GAMEDATASTRUCT_CASTLE_HEX_COLS * GAMEDATASTRUCT_CASTLE_HEX_ROWS; i++ ) {
			int iBango = nb6castle_hex_unit_pos.index[i];
			if ( 1 <= iBango && iBango <= GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_NUM ) {
				if ( nb6bushou_hex_unit_info[iBango-1].bushou != 0xFFFF ) {
					int iBushouID = nb6bushou_hex_unit_info[iBango-1].bushou-1;

					if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
						// 戦死や退却していない
						if ( nb6bushouname[iBushouID].battleDeath == 0 && nb6bushouname[iBushouID].battleRetreat == 0 ) {
							bArray.push_back( iBushouID );
						}
					}
				}
			}
		}
	}
	if ( Is_FieldWar() ) {
		for ( int i=0; i<GAMEDATASTRUCT_FIELD_HEX_COLS * GAMEDATASTRUCT_FIELD_HEX_ROWS; i++ ) {
			int iBango = nb6field_hex_unit_pos.index[i];
			if ( 1 <= iBango && iBango <= GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_NUM ) {
				if ( nb6bushou_hex_unit_info[iBango-1].bushou != 0xFFFF ) {
					int iBushouID = nb6bushou_hex_unit_info[iBango-1].bushou-1;

					if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
						// 戦死や退却していない
						if ( nb6bushouname[iBushouID].battleDeath == 0 && nb6bushouname[iBushouID].battleRetreat == 0 ) {
							bArray.push_back( iBushouID );
						}
					}
				}
			}
		}
	}
	return bArray;
}


// 指定の武将ＩＤの武将がどこにいるのか？ 居ない場合、p(x=0xFFFF, y=0xFFFF) が返ってくる。
HEX_POSITION GetFieldHexUnitPos(int iBushouID) {

	HEX_POSITION pos( 0xFFFF, 0xFFFF );

	if ( !(0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) ) {
		return pos;
	}
	if ( !Is_FieldWar() ) {
		return pos;
	}

	// 戦場にいる武将から条件を満たすものの移動残量を増やす。
	for ( int i=0; i<GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_NUM; i++ ) {
		if ( nb6bushou_hex_unit_info[i].bushou == 0xFFFF ) { continue; } // 0xFFFFだとそのスロットに武将は居ない。次の人
		if ( nb6bushou_hex_unit_info[i].bushou > GAMEDATASTRUCT_BUSHOU_NUM ) { continue; } //

		// 探してる武将が、表示中の野戦に居た!!
		if ( iBushouID == nb6bushou_hex_unit_info[i].bushou - 1 ) {
			int x = nb6bushou_hex_unit_info[i].pos_x;
			int y = nb6bushou_hex_unit_info[i].pos_y;

			if ( x==0xFFFF ) {
				return pos;
			}

			// yを座標実値にして、整数化
			pos.y = y;

			// 野戦の偶数行目か奇数行目かで、半身ずらす。偶数行なら、"座標を0.5分右にずらす。
			if (y % 2==1) {
				pos.x = x + 0.5;
			} else {
				pos.x = x;
			}

			return pos;
		}
	}

	return pos;
}




// 指定の武将ＩＤの武将がどこにいるのか？ 居ない場合、p(x=0xFFFF, y=0xFFFF) が返ってくる。
HEX_POSITION GetCastleHexUnitPos(int iBushouID) {


	HEX_POSITION pos( 0xFFFF, 0xFFFF );

	if ( !(0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) ) {
		return pos;
	}
	if ( !Is_CastleWar() ) {
		return pos;
	}

	// 戦場にいる武将から条件を満たすものの移動残量を増やす。
	for ( int i=0; i<GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_NUM; i++ ) {
		if ( nb6bushou_hex_unit_info[i].bushou == 0xFFFF ) { continue; } // 0xFFFFだとそのスロットに武将は居ない。次の人
		if ( nb6bushou_hex_unit_info[i].bushou > GAMEDATASTRUCT_BUSHOU_NUM ) { continue; } //

		// 探してる武将が、表示中の攻城戦闘に居た!!
		if ( iBushouID == nb6bushou_hex_unit_info[i].bushou - 1 ) {
			int x = nb6bushou_hex_unit_info[i].pos_x;
			int y = nb6bushou_hex_unit_info[i].pos_y;

			if ( x==0xFFFF ) {
				return pos;
			}

			// yを座標実値にして、整数化
			pos.y = y;

			// 野戦の偶数行目か奇数行目かで、半身ずらす。偶数行なら、"座標を0.5分右にずらす。
			if (y % 2==1) {
				pos.x = x + 0.5;
			} else {
				pos.x = x;
			}

			return pos;
		}
	}

	return pos;
}

HEX_VECTOR operator-(HEX_POSITION &p1, HEX_POSITION &p2) {
	HEX_VECTOR p;
	p.x = p1.x - p2.x;
	p.y = p1.y - p2.y;
	return p;
}


// 現在進行中の戦争に関わっているiDaimyoIDのリスト。(出陣しているかどうかは関係ない)
vector<int> GetCurWarJointDaimyoIDList() {
	vector<int> list;
	if ( Is_FieldWar() || Is_CastleWar() ) {
		// 参加大名番号を検索して、
		for ( int ix=0; ix < GAMEDATASTRUCT_WAR_JOINT_DAIMYO_HEX_GROUP_INFO_NUM; ix++ ) {

			int iDaimyoID = (int)nb6war_joint_daimyo_hex_group_info[ix].daimyo - 1;
			// 範囲内であれば、リストに追加
			if ( 0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM ) {
				// 該当の値がすでにlist内にあるか
				vector<int>::iterator iter = std::find( list.begin(), list.end() , iDaimyoID );
				// 存在しなければ…
				if ( iter == list.end() ) {
					// 追加
					list.push_back( iDaimyoID );
				}
			}
		}
	}

	
	return list;
}

// 現在進行中の戦争に関わっているiGundanIDのリスト。(出陣しているかどうかは関係ない)
vector<int> GetCurWarJointGundanIDList() {
	vector<int> list;
	if ( Is_FieldWar() || Is_CastleWar() ) {
		// 参加大名番号を検索して、
		for ( int ix=0; ix < GAMEDATASTRUCT_WAR_JOINT_DAIMYO_HEX_GROUP_INFO_NUM; ix++ ) {

			int iGundanID = (int)nb6war_joint_daimyo_hex_group_info[ix].gundan - 1;
			// 範囲内であれば、リストに追加
			if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
				list.push_back( iGundanID );
			}
		}
	}

	return list;
}

extern 戦争開始パラメタ war_start_param;
// 該当の軍団の戦場における立場を返す(直接守る側=0, 直接攻める側=1, 攻める側の援軍=3, 守る側の援軍=4, 中立軍=5)
int GetWarStandPointTheGundan(int iGundanID) {

	if ( !(0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) ) {
		return WarStandPoint::unknown;
	}

	// enum { unknown = -1,  攻撃親軍 = 1,  攻撃子軍 = 5,  守備親軍=0, 守備子軍 = 4, 中立親軍=2, 中立子軍=6, 攻撃援軍親軍=1, 攻撃援軍子軍=5, 守備援軍親軍=0, 守備援軍子軍=4 };

	if ( Is_FieldWar() || Is_CastleWar() ) {

		//--------------------------------- 直接攻撃側、もしくは、直接防御側であれば、戦争開始パラメタからはじき出せる
		// 指定の軍団の大名ID
		int iDaimyoID = nb6gundan[iGundanID].attach-1;

		// 攻撃側の軍団の大名ID
		int iAttackDaimyoID = -1; 
		if ( 0 <= war_start_param.攻撃側軍団_軍団番号-1 && war_start_param.攻撃側軍団_軍団番号-1 < GAMEDATASTRUCT_GUNDAN_NUM ) {
			iAttackDaimyoID = nb6gundan[war_start_param.攻撃側軍団_軍団番号-1].attach-1;
		}

		// 守備側の軍団の大名ID
		int iDefendDaimyoID = -1; 
		if ( 0 <= war_start_param.守備側軍団_軍団番号-1 && war_start_param.守備側軍団_軍団番号-1 < GAMEDATASTRUCT_GUNDAN_NUM ) {
			iDefendDaimyoID = nb6gundan[war_start_param.守備側軍団_軍団番号-1].attach-1;
		}

		// 指定の軍団の大名は直接攻撃の大名
		if ( iDaimyoID == iAttackDaimyoID ) {
			return WarStandPoint::attack; // 直接攻撃側
		}
		// 指定の軍団の大名は直接守備の大名
		else if ( iDaimyoID == iDefendDaimyoID ) {
			return WarStandPoint::defend; // 直接防御側
		}

		//--------------------------------- それ以外の場合は、WAR_JOINT_DAIMYO_HEX_GROUP_INFO 系の情報から辿っていく

		// 参加大名番号を検索して、攻撃側のグループID、守備側のグループIDを出す。
		for ( int ix=0; ix < GAMEDATASTRUCT_WAR_JOINT_DAIMYO_HEX_GROUP_INFO_NUM; ix++ ) {
			// 援軍や中立軍かチェック
			int JoinGundanID = (int)nb6war_joint_daimyo_hex_group_info[ix].gundan - 1; // 参加軍団ID
			int JoinDaimyoID = (int)nb6war_joint_daimyo_hex_group_info[ix].daimyo - 1; // 参加大名ID
			// 指定の軍団と同じ軍団IDである場合のみ値を返す。
			if ( iGundanID == JoinGundanID ) {

				// 指定の軍団の大名は直接攻撃の大名
				if ( JoinDaimyoID == iAttackDaimyoID ) {
					return WarStandPoint::attack; // 直接攻撃側
				}
				// 指定の軍団の大名は直接守備の大名
				else if ( JoinDaimyoID == iDefendDaimyoID ) {
					return WarStandPoint::defend; // 直接防御側
				}

				// 2か6なら中立軍。2が中立の親軍で、6が中立の子軍
				else if ( nb6war_joint_daimyo_hex_group_info[ix].group == 2 || nb6war_joint_daimyo_hex_group_info[ix].group == 6 ) {
					// 中立軍
					return WarStandPoint::chuuritsu;
				}
				// 1か5なら援軍の攻撃軍。1が援軍の親軍で、5が援軍の子軍
				else if ( nb6war_joint_daimyo_hex_group_info[ix].group == 1 || nb6war_joint_daimyo_hex_group_info[ix].group == 5 ) {
					// 援軍の攻撃軍
					return WarStandPoint::attack_enngun;
				}
				// 0か4なら援軍の守備軍。0が援軍の親軍で、4が援軍の子軍
				else if ( nb6war_joint_daimyo_hex_group_info[ix].group == 0 || nb6war_joint_daimyo_hex_group_info[ix].group == 4 ) {
					// 援軍の守備軍
					return WarStandPoint::defend_enngun;
				}
			}
		}
	}

	return WarStandPoint::unknown; // 不明
}

// 該当の武将の戦場における立場を返す(不明=0, 直接攻める側=1, 直接守る側=2, 攻める側の援軍=3, 守る側の援軍=4, 中立軍=5)
int GetWarStandPointTheBushou(int iBushouID) {

	// 範囲チェック
	if ( !(0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) ) {
		return WarStandPoint::unknown;
	}

	// 該当武将の軍団ID入手
	int iGundanID = nb6bushouref[iBushouID].gundan-1;
	if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
		return GetWarStandPointTheGundan(iGundanID);
	}

	return WarStandPoint::unknown;
}

// 現在の戦において、指定軍団の総大将(軍団長や大名のことではなく、戦場においての該当軍団の総大将。
int GetWarSoudaishoTheGundan(int iGundanID) {
	if ( !(0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) ) {
		return 0xFFFF;
	}

	if ( Is_FieldWar() || Is_CastleWar() ) {
		// 参加大名番号を検索して、攻撃側のグループID、守備側のグループIDを出す。
		for ( int ix=0; ix < GAMEDATASTRUCT_WAR_JOINT_DAIMYO_HEX_GROUP_INFO_NUM; ix++ ) {

			int iJointGundanID = (int)nb6war_joint_daimyo_hex_group_info[ix].gundan-1;

			// 指定の軍団を見つけた。
			if ( iGundanID == iJointGundanID ) {
				// 武将番号(+1の方)の範囲に収まっている。
				if ( 0 < nb6war_joint_daimyo_hex_group_info[ix].soudaisho && nb6war_joint_daimyo_hex_group_info[ix].soudaisho <= GAMEDATASTRUCT_BUSHOU_NUM ) {
					return nb6war_joint_daimyo_hex_group_info[ix].soudaisho-1;
				// 武将番号の範囲に収まっていないということは、大名や軍団は決まっているが、総大将が決まっていない。ユニット配置前だと総大将が決まらないので、このような値となる。
				} else {
					return 0xFFFF;
				}
			}
		}
	}

	return 0xFFFF;
}

// 野戦なのか攻城戦闘なのか？
// 現在画面に出てる戦場は、野戦
extern int iNextTurnCastleID;
// 現在の戦場におけるターン経過数。戦場以外では-1となっている。
extern int iCurIncrementTurnInBattle;
bool Is_FieldWar() {
	return (iCurIncrementTurnInBattle >= 0) && iNextTurnCastleID == 0xFFFF && CommonGlobalHandle::isBattleMode;
}

// 現在画面に出てる戦場は、攻城戦
bool Is_CastleWar() {
	return (iCurIncrementTurnInBattle >= 0) && ( 0 <= iNextTurnCastleID-1 && iNextTurnCastleID-1 < GAMEDATASTRUCT_CASTLE_NUM ) && CommonGlobalHandle::isBattleMode; 
}


// 表示中の攻城戦で何ターン目か。１〜３．攻城戦でない場合は-1．
int getCastleTurnInBattle() {

	if ( Is_CastleWar() ) {

		byte *turn = (byte *)GAMEDATASTRUCT_HOWMUCHTURNPROCEED_INCASTLEBATTLE;
		return *turn + 1;

	} else {
		return -1;
	}
}

// 外部参照用
int WINAPI Extern_getCastleTurnInBattle() {
	return getCastleTurnInBattle();
}

//--------------

// 指定武将が戦場において、行動済みかどうか
BOOL IsActionEndInBattle(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		for ( int i=0; i < GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_NUM; i++ ) {
			if ( nb6bushou_hex_unit_info[i].bushou == iBushouID+1 ) {
				if ( nb6bushou_hex_unit_info[i].bEnd == 0x2 ) { // 行動済である。
					return TRUE;
				} else {
					return FALSE;
				}
				break;
			}
		}
	}
	return FALSE;
}


// 対象の武将が混乱しているか。
int GetKonranStatus(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		for ( int i=0; i < GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_NUM; i++ ) {
			if ( nb6bushou_hex_unit_info[i].bushou == iBushouID+1 ) {
				return nb6bushou_hex_unit_info[i].konran; 
			}
		}
	}
	return KONRAN_STATUS::無し;
}

// 対象の武将を混乱させる。(描画が伴っていない)
BOOL SetKonranStatus(int iBushouID, int iKonranStatus ) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		for ( int i=0; i < GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_NUM; i++ ) {
			if ( nb6bushou_hex_unit_info[i].bushou == iBushouID+1 ) {
				nb6bushou_hex_unit_info[i].konran = iKonranStatus;
				// 混乱・大混乱・挑発 の状態になったのなら…
				if ( iKonranStatus > 0 ) {
					nb6bushou_hex_unit_info[i].bEnd   = 0x2; // 行動済
				}
				FuncBattleUnitStatusRepaint(iBushouID); // 描画更新
				return TRUE;
			}
		}
	}
	return FALSE;
}

// 指定武将を行動済みとする
extern int arrRemainMoveCountAllBushou[GAMEDATASTRUCT_BUSHOU_NUM];
void setIsActionEndInBattle(int iBushouID, int isEnd=TRUE) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		for ( int i=0; i < GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_NUM; i++ ) {
			if ( nb6bushou_hex_unit_info[i].bushou == iBushouID+1 ) {
				if ( isEnd ) {
					nb6bushou_hex_unit_info[i].bEnd = 0x2;  // 行動済とする。
					arrRemainMoveCountAllBushou[iBushouID] = 0;
				} else {
					nb6bushou_hex_unit_info[i].bEnd = 0;  // 未行動とする。
				}
				break;
			}
		}
	}
}


//--------------MAINのグリッド系

// グリッドポジションクラス
GRID_POSITION::GRID_POSITION(int _x,int _y) {
	x=_x,
	y=_y;

	// 各隣接城は無い状態で初期化
	nw= 0xFFFF;
	no= 0xFFFF;
	ne= 0xFFFF;
	we= 0xFFFF;
	ea= 0xFFFF;
	sw= 0xFFFF;
	so= 0xFFFF;
	se= 0xFFFF;

}

// グリッド座標同志の減算。座標Ａ→座標Ｂのベクトルを求めるのに、通常、Ｂ−Ａをするだろうから。
GRID_VECTOR operator-(GRID_POSITION &p1, GRID_POSITION &p2) {
	GRID_VECTOR p;
	p.x = p1.x - p2.x;
	p.y = p1.y - p2.y;
	return p;
}

// 城ＩＤから、メイングリッド上のポジションを得る。左上グリッドが0,0。mapdataobgkのグリッドを見ればわかりやすい。
// １つのグリッドには、１つの城しかない。
GRID_POSITION getCastlePosInMainGrid(int iCastleID) {
	GRID_POSITION pos(0, 0);
	if ( 0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM ) {
		int iGridX = (nb6castlemax[iCastleID].x + 1) / 5; // ０オリジンなので＋１して５で割る。Ｘ方向は１つのグリッドの中に５つの場所がある。
		int iGridY = (nb6castlemax[iCastleID].y + 1) / 3; // ０オリジンなので＋１して３で割る。Ｙ方向は１つのグリッドの中に３つの場所がある。

		pos.x = iGridX;
		pos.y = iGridY;

		// 隣接する城のＩＤも入れておく。
		for (int iCastleID = 0; iCastleID < GAMEDATASTRUCT_CASTLE_NUM; iCastleID++) {
			int iFX = (nb6castlemax[iCastleID].x + 1) / 5; // ０オリジンなので＋１して５で割る。Ｘ方向は１つのグリッドの中に５つの場所がある。
			int iFY = (nb6castlemax[iCastleID].y + 1) / 3; // ０オリジンなので＋１して３で割る。Ｙ方向は１つのグリッドの中に３つの場所がある。

			if ( iFX - pos.x == -1 && iFY - pos.y == -1 ) { // 北西
				pos.nw = iCastleID + 1;
			} else if ( iFX - pos.x == 0 && iFY - pos.y == -1 ) { // 北
				pos.no = iCastleID + 1;
			} else if ( iFX - pos.x == 1 && iFY - pos.y == -1 ) { // 北東
				pos.ne = iCastleID + 1;
			} else if ( iFX - pos.x == -1 && iFY - pos.y == 0 ) { // 西
				pos.we = iCastleID + 1;
			} else if ( iFX - pos.x == 1 && iFY - pos.y == 0 ) { // 東
				pos.ea = iCastleID + 1;
			} else if ( iFX - pos.x == -1 && iFY - pos.y == 1 ) { // 南西
				pos.sw = iCastleID + 1;
			} else if ( iFX - pos.x == 0 && iFY - pos.y == 1 ) { // 南
				pos.so = iCastleID + 1;
			} else if ( iFX - pos.x == 1 && iFY - pos.y == 1 ) { // 南東
				pos.se = iCastleID + 1;
			}
		}

		return pos;
	}
	return pos;
}

// 指定の城に対して隣接している城のリストを得る
vector<int> getRinsetsuCastles(int iCastleID) {
	// まずは、指定城のグリッドの場所を得る
	GRID_POSITION CenterPos = getCastlePosInMainGrid(iCastleID);

	vector<int> RinsetsuCastles;

	// 隣接している城をベクトルに追加してゆく
	for ( int iTargetID = 0; iTargetID < GAMEDATASTRUCT_CASTLE_NUM; iTargetID++ ) {
		// 自分自身はパス
		if ( iCastleID == iTargetID ) {
			continue;
		}

		GRID_POSITION TargetPos = getCastlePosInMainGrid(iTargetID);
		// 隣接している。
		if ( abs( CenterPos.x - TargetPos.x ) <= 1 && abs( CenterPos.y - TargetPos.y) <= 1) {
			RinsetsuCastles.push_back( iTargetID );
		}
	}
	return RinsetsuCastles;
}


char* nb6kanitypelist[GAMEDATASTRUCT_KANITYPE_NUM] = {
	"正一位",
	"従一位",
	"正二位",
	"従二位",
	"正三位",
	"従三位",
	"正四位上",
	"正四位下",
	"従四位上",
	"従四位下",
	"正五位上",
	"正五位下",
	"従五位上",
	"従五位下",
	"正六位上",
	"正六位下",
	"従六位上",
	"従六位下",
	"正七位上",
	"正七位下",
	"従七位上",
	"従七位下",
	"正八位上",
};

char* getKaniTypeName(byte type) {
	return nb6kanitypelist[type];
}




// 役職の種類の文字列へのポインタのアドレス一覧 
int GAMEDATASTRUCT_POSITIONTYPELIST_POINTER[GAMEDATASTRUCT_POSITIONTYPE_NUM] = {};

// 役職タイプのIDから文字列を得る
char* getPositionTypeName(byte type) {
	char *pszPotisionType = NULL;
	if (type==0) {
		return NULL;
	}
	if ( type < GAMEDATASTRUCT_POSITIONTYPE_NUM ) {
		pszPotisionType = (char *)(GAMEDATASTRUCT_POSITIONTYPELIST_POINTER[type]);
		return pszPotisionType;
	}
	return  NULL;
}

int DeNormalizeDaimyoYakusyokuValue(int iYakusyokuValue) {
	switch (iYakusyokuValue) {
	case 1:
		return 0x01;
	case 2:
		return 0x02;
	case 3:
		return 0x04;
	case 4:
		return 0x08;
	case 5:
		return 0x10;
	case 6:
		return 0x20;
	case 7:
		return 0x40;
	default:
		return 0;
	}
}

// 大名IDから正規化された役職IDを得る
int getNormalizedDaimyoPosition(int iDaimyoID) {
	switch (nb6daimyo[iDaimyoID].position) {
		case 0x01:
			return 1;
		case 0x02:
			return 2;
		case 0x04:
			return 3;
		case 0x08 :
			return 4;
		case 0x10:
			return 5;
		case 0x20:
			return 6;
		case 0x40:
			return 7;
		default:
			return 0;
	}
}

// 軍団IDから正規化された役職IDを得る
int getNormalizedGundanPosition(int iGundanID) {
	return nb6gundan[iGundanID].position;
}

bool tryChangeDaimyoYakusyoku(int iDaimyoID, int iYakusyokuValue ) {
	if ( 0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM) {

		if ( 役職::無し <= iYakusyokuValue && iYakusyokuValue <= 役職::羽州探題 ) {

			int DenormalizeyakusyokuValue = DeNormalizeDaimyoYakusyokuValue(iYakusyokuValue);
			
			// 大名で目的のIDの役職を持っている人
			for (int iDID = 0; iDID < GAMEDATASTRUCT_DAIMYO_NUM; iDID++) {
				int iDaimyoYakusyokuID = getNormalizedDaimyoPosition(iDID);
				// その役職を持っていれば、はく奪
				if (iDaimyoYakusyokuID == iYakusyokuValue) {
					nb6daimyo[iDID].position = 0;
				}
			}

			// 軍団で目的のIDの役職を持っている人
			for (int iGID = 0; iGID < GAMEDATASTRUCT_GUNDAN_NUM; iGID++) {
				int iGundanYakusyokuID = getNormalizedGundanPosition(iGID);
				// その役職を持っていれば、はく奪
				if (iGundanYakusyokuID == iYakusyokuValue) {
					nb6gundan[iGID].position = 0;
				}
			}
		
			// 自分に対象の役職をつける。
			nb6daimyo[iDaimyoID].position = DenormalizeyakusyokuValue;

			return true;
		}
	}
		
	return false;
}


bool tryChangeGundanYakusyoku(int iGundanID, int iYakusyokuValue ) {

	if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM) {

		// 大名自身なら、別処理
		if (nb6gundan[iGundanID].number == 1) {
			int iDaimyoID = nb6gundan[iGundanID].attach-1;
			tryChangeDaimyoYakusyoku( iDaimyoID, iYakusyokuValue );
		}

		if ( 役職::無し <= iYakusyokuValue && iYakusyokuValue <= 役職::羽州探題 ) {

			// 大名で目的のIDの役職を持っている人
			for (int iDID = 0; iDID < GAMEDATASTRUCT_DAIMYO_NUM; iDID++) {
				int iDaimyoYakusyokuID = getNormalizedDaimyoPosition(iDID);
				// その役職を持っていれば、はく奪
				if (iDaimyoYakusyokuID == iYakusyokuValue) {
					nb6daimyo[iDID].position = 0;
				}
			}

			// 軍団で目的のIDの役職を持っている人
			for (int iGID = 0; iGID < GAMEDATASTRUCT_GUNDAN_NUM; iGID++) {
				int iGundanYakusyokuID = getNormalizedGundanPosition(iGID);
				// その役職を持っていれば、はく奪
				if (iGundanYakusyokuID == iYakusyokuValue) {
					nb6gundan[iGID].position = 0;
				}
			}

			// 自分に対象の役職をつける。
			nb6gundan[iGundanID].position = iYakusyokuValue;

			return true;
		}
	}

	return false;
}



/// 現在(その瞬間)、天翔記に存在する軍団の「軍団番号【配列用】」をリストで得る。
vector<int> getValidGundanList() {
	vector<int> list;

	// 該当の大名ＩＤに属する軍団をリストに加える
	for ( int iGundanID=0; iGundanID < GAMEDATASTRUCT_GUNDAN_NUM; iGundanID++ ) {
		if ( nb6gundan[iGundanID].leader !=  0xFFFF ) {
			list.push_back(iGundanID);
		}
	}
	
	return list;
}



// 軍団に所属する武将リストを得る
vector<int> getGundanBushouList(int iGundanID) {
	// それ以外は何も入れずに返す。
	vector<int> list;

	for (int i = 0; i<GAMEDATASTRUCT_BUSHOU_NUM; i++) {
		if (nb6bushouref[i].gundan - 1 == iGundanID) {
			if (nb6bushouname[i].State <= 2) { // 大名, 軍団長, 現役 でカウント
				list.push_back(i);
			}
		}
	}
	return list;
}

// とある大名勢力の統治軍団リストを得る。
vector<int> getGovernedGundanList(int iDaimyoID) {
	// それ以外は何も入れずに返す。
	vector<int> list;

	if ( 0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM ) {

		// 該当の大名ＩＤに属する軍団をリストに加える
		for ( int iGundanID=0; iGundanID<GAMEDATASTRUCT_GUNDAN_NUM; iGundanID++ ) {
			if ( nb6gundan[iGundanID].attach-1 ==  iDaimyoID) {
				list.push_back(iGundanID);
			}
		}
	}

	return list;
}

// 軍団に所属する兵数を得る
int getGundanHeisuCnt(int iGundanID) {
	int iSoldierCnt = 0;
	for ( int i=0; i<GAMEDATASTRUCT_BUSHOU_NUM; i++) {
		if ( nb6bushouref[i].gundan-1 == iGundanID ) {
			if ( nb6bushouname[i].State <= 2 ) { // 大名, 軍団長, 現役 でカウント
				iSoldierCnt += nb6bushouref[i].soldier;
			}
		}
	}
	return iSoldierCnt;
}

// 城に所属する兵数を得る
int getCastleHeisuCnt(int iCastleID) {
	int iSoldierCnt = 0;
	for ( int i=0; i<GAMEDATASTRUCT_BUSHOU_NUM; i++) {
		if ( nb6bushouref[i].castle-1 == iCastleID ) {
			if ( nb6bushouname[i].State <= 2 ) { // 大名, 軍団長, 現役 でカウント
				iSoldierCnt += nb6bushouref[i].soldier;
			}
		}
	}
	return iSoldierCnt;
}



// とある大名勢力の統治軍団割りあて番号リストを得る。
vector<int> getGovernedGundanWariateNumList(int iDaimyoID) {
	// それ以外は何も入れずに返す。
	vector<int> list;

	if ( 0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM ) {

		// 該当の大名ＩＤに属する軍団をリストに加える
		for ( int iGundanID=0; iGundanID<GAMEDATASTRUCT_GUNDAN_NUM; iGundanID++ ) {
			if ( nb6gundan[iGundanID].attach-1 ==  iDaimyoID) {
				list.push_back(nb6gundan[iGundanID].number);
			}
		}
	}
	
	std::sort(list.begin(),list.end());//昇順ソート

	return list;
}



// とある大名配下の軍団下にある軍団「未」割当番号のリストを得る。
vector<int> getNotGovernedGundanWariateNumList(int iDaimyoID) {

	// 割り当てられてるものに、１を入れる
	int wariate_array[8] = {0,0,0,0,0,0,0,0};
	
	if ( 0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM ) {

		// 該当の大名ＩＤに属する軍団をリストに加える
		for ( int iGundanID=0; iGundanID<GAMEDATASTRUCT_GUNDAN_NUM; iGundanID++ ) {
			if ( nb6gundan[iGundanID].attach-1 ==  iDaimyoID) {
				int g =  nb6gundan[iGundanID].number;
				if ( 1 <= g && g <= 8 ) {
					// 割り当てられてるものに、１を入れる
					wariate_array[g-1] = 1;
				}
			}
		}
	}

	vector<int> list;
	for ( int ix=0; ix<8; ix++ ) {
		if (wariate_array[ix] == 0) { // 軍団割当がなければ
			list.push_back(ix+1);
		}
	}

	std::sort(list.begin(),list.end());//昇順ソート

	return list;
}

// 軍団任命番号の変更(入替)を試みる。失敗すればfalse
bool tryChangeGundanWariateNum(int iGundanID, int iNewWariateNum) {

	// 第１軍団は大名自身。変更は無理。
	if (nb6gundan[iGundanID].number == 1) {
		return false;
	}
	
	if ( !( 1<= iNewWariateNum && iNewWariateNum <= 8) ) {
		return false;
	}

	// 自分の軍団の大名のID
	int iDaimyoID = nb6gundan[iGundanID].attach - 1;

	// 大名麾下で空きスロットとなっている任命番号一覧
	if (0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM) {
		vector<int> sameDaimyoGundanList = getGovernedGundanList(iDaimyoID);
		int iOldNinmeiNum = nb6gundan[iGundanID].number; // 今までの任命番号

		for( vector<int>::iterator it = sameDaimyoGundanList.begin(); it != sameDaimyoGundanList.end(); it++ ) {
			int iGID = *it;

			// すでに別軍団で利用されているならば
			if (nb6gundan[iGID].number == iNewWariateNum) {
				nb6gundan[iGID].number = iOldNinmeiNum;
				break;
			}
		}

		// 自分は指定の軍団番号となる。
		nb6gundan[iGundanID].number = iNewWariateNum;

		return true;
	}

	return false;

}


// 指定の軍団の所属する大名を勢力を越えて、変更する。
bool setGundanReAttachDaimyo(int iGundanID, int iNewDaimyoID) {

	// 新大名が正当ではない。
	if ( ! (0 <= iNewDaimyoID && iNewDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM) ) {
		return false;
	}

	// 軍団が全部埋まっているならば、
	vector<int> glist = getGovernedGundanWariateNumList(iNewDaimyoID);
	if ( glist.size() == 8 ) { // 軍団は全部埋まっている
		// 何もできない
		return false;
	}

	if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {

		// 指定の軍団が現在所属している大名番号を求める
		int iDaimyoID = nb6gundan[iGundanID].attach-1;
		// 新所属先の大名が、今所属している大名であれば、何もすることはない。
		if ( iDaimyoID == iNewDaimyoID) {
			return true;
		}

		// 軍団割り当て番号が１番は大名自身。これは固定。この場合、何もせず失敗とする。
		if ( nb6gundan[iGundanID].number == 1 ) {
			return false;
		}

		// 現在、新大名に所属していて、nextが0xFFFFになっている軍団が、ぶら下がるべきリンクリストの最後だ。
		// ぶら下がることが出来るか、事前にチェックしておく。
		int iLinkEndGundanID = -1;
		for ( int iCurGundanID=0; iCurGundanID < GAMEDATASTRUCT_GUNDAN_NUM; iCurGundanID++ ) {
			// 
			int iCurDaimyoID = nb6gundan[iCurGundanID].attach-1;

			// 指定の新大名に所属しているならば、
			if ( iCurDaimyoID == iNewDaimyoID) {
				// 次の軍団が無いのであれば、そいつが最終軍団番号
				if ( nb6gundan[iCurGundanID].next == 0xFFFF ) {
					iLinkEndGundanID = iCurGundanID+1;
				}
				
			}

		}

		// それが範囲に入っていないならば、新大名の軍団リンクリストは、改造か何かしらないが情報が壊れている。
		// 処理終了
		if ( ! (0 < iLinkEndGundanID && iLinkEndGundanID <= GAMEDATASTRUCT_GUNDAN_NUM ) ) {
			return false;
		}

		/* 　自軍団
		      ↓
		   ○-●-○-○

		   というのを

		   ○-○-○

		   とする。
		*/

		int iPrevID = nb6gundan[iGundanID].prev;
		int iNextID = nb6gundan[iGundanID].next;

		// 自分のprevは有効な軍団だ。
		if ( 0 < iPrevID && iPrevID <= GAMEDATASTRUCT_GUNDAN_NUM ) {

			// 自分が先頭ではない軍団で、
			// １つ前の軍団の次のリンク先が自軍団の場合
			if ( iPrevID != 0xFFFF && nb6gundan[(iPrevID-1)].next == iGundanID+1 ) {
				// 自軍団ではなく、次の軍団へと繋ぎかえる。
				nb6gundan[(iPrevID-1)].next = iNextID;
			}
		}

		// 自分のnextは有効な軍団だ。
		if ( 0 < iNextID && iNextID <= GAMEDATASTRUCT_GUNDAN_NUM ) {
			// １つ後の軍団の前のリンク先が自軍団の場合
			if ( iNextID != 0xFFFF && nb6gundan[(iNextID-1)].prev == iGundanID+1 ) {
				// 自軍団ではなく、前の軍団へと繋ぎかえる。
				nb6gundan[(iNextID-1)].prev = iPrevID;
			}
		}


		/* 　
		     
		   ○-○

		   というのを

		   ○-○-●

		   とする。
		*/



		nb6gundan[iLinkEndGundanID-1].next = iGundanID+1; // リンクリストの最後の軍団のnextを自軍団に

		nb6gundan[iGundanID].prev = iLinkEndGundanID; // 自軍団の前の軍団は、軍団のリンクリストの最後の軍団

		// 自軍団は、軍団リストの一番最後にくっつく。自軍団のnextは居ない
		nb6gundan[iGundanID].next = 0xFFFF; // 

		// メモリ書き換え
		{
			nb6gundan[iGundanID].number = 8; // 元の番号というわけにはいかないので、万が一のために、とりあえず、最も高い確率で空いてるセーフなものを取る

			vector<int> milist = getNotGovernedGundanWariateNumList(iNewDaimyoID);
			if ( milist.size() > 0 ) {
				nb6gundan[iGundanID].number = milist[0]; // 空番号の最初のものを放り込む
			}

			nb6gundan[iGundanID].attach = iNewDaimyoID+1;
		}

		for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {
			// 指定の軍団に所属している武将は、全て、新大名に所属を変更する。
			if ( nb6bushouref[iBushouID].gundan== iGundanID+1 ) {
				nb6bushouref[iBushouID].attach = iNewDaimyoID+1;
				nb6bushouref[iBushouID].work = 0; // 士官年数リセット
				nb6bushouref[iBushouID].rise = 0xFFFF; // 内応先リセット
				nb6bushouref[iBushouID].bRise = 0; // 内応状態なし
				nb6bushouref[iBushouID].grudge = 0; // 遺恨なし
			}
		}

		return true;

	}

	return false;
}






MAKEGUNDANKOUHOINFO Get_MakeGundanKouhoInfo(int m_iDaimyoID) {
	MAKEGUNDANKOUHOINFO ret;
	ret.m_iDaimyoID = m_iDaimyoID;
	ret.vGundanIDList = vector<int>();
	ret.vCastleIDList = vector<int>();
	ret.vBushouIDList = vector<int>();
	ret.CanMake = FALSE;

	if (!(0 <= m_iDaimyoID && m_iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM)) {
		OutputDebugStream("m_iDaimyoIDが範囲外");
		return ret;
	}

	bool is_gundan_has_empty_slot = false;
	for (int iGundanID = 0; iGundanID < GAMEDATASTRUCT_GUNDAN_NUM; iGundanID++) {
		if (nb6gundan[iGundanID].attach == 0xFFFF && // 所属大名番号
			nb6gundan[iGundanID].residence == 0xFFFF &&
			nb6gundan[iGundanID].next == 0xFFFF) {
			is_gundan_has_empty_slot = true;
			break;
		}
	}
	if (is_gundan_has_empty_slot == false) {
		OutputDebugStream("軍団スロットが全て埋まっている");
		return ret;
	}

	int iDaimyoBushouID = nb6daimyo[m_iDaimyoID].attach - 1; // 大名の武将番号
	// 死んで該当軍団にターンが回る間の危険なタイミング。このタイミングは承認者が生存していないので、新設できないものとする。
	if (nb6bushouname[iDaimyoBushouID].battleDeath || (nb6bushouname[iDaimyoBushouID].State == 7)) { // 死亡
		return ret;
	}

	// 割り当て候補は要するに、まだ割り当てられていない軍団番号のこと
	ret.vGundanIDList = getNotGovernedGundanWariateNumList(m_iDaimyoID);

	// 指定の大名が支配している全城のリスト
	auto daimyoHasCastleList = getDaimyoHasCastleList(m_iDaimyoID);

	// 候補にするのは、指定の大名家支配城のうち、「大名」も「軍団長」も居ない城。
	// 即ち、「非本拠」のものに限る
	for each(int iCastleID in daimyoHasCastleList) {
		if (nb6castle[iCastleID].honjo == 0) { //本城 居城 (0:非本拠, 大名の本城=1, 軍団長の本城=2);
			ret.vCastleIDList.push_back(iCastleID);
		}
	}

	for (int iBushouID = 0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {

		// まず対象の武将は、指定の大名家の人間である。(大名含む)
		if (nb6bushouref[iBushouID].attach - 1 == m_iDaimyoID) {

			// 現役である。(=大名や軍団長やヒメではなく、生存し、登場している一般武将)
			if (nb6bushouname[iBushouID].State == 2) { // 2:現役
				// 戦死中ではない
				if (!nb6bushouname[iBushouID].battleDeath) {
					ret.vBushouIDList.push_back(iBushouID);
				}
			}
		}
	}

	// 軍団がすでに８つある
	if (ret.vGundanIDList.size() == 0) {
		ret.CanMake = FALSE;
		return ret;
	}

	// 新軍団長を迎え入れるような余った城は存在しない
	if (ret.vCastleIDList.size() == 0) {
		ret.CanMake = FALSE;
		return ret;
	}

	// そもそも軍団長に任命すべき対象の一般武将が存在しない
	if (ret.vBushouIDList.size() == 0) {
		ret.CanMake = FALSE;
		return ret;
	}

	ret.CanMake = TRUE;

	return ret;
}


int Set_MakeGundan(int _m_iBushouID, int _m_iCastleID, MAKEGUNDANKOUHOINFO check) {
	if (check.CanMake == FALSE) {
		OutputDebugStream("MAKEGUNDANKOUHOINFO.CanMake が FALSE");
		return 0xFFFF;
	}

	// 名前長すぎ
	auto& bushous = check.vBushouIDList;
	auto& castles = check.vCastleIDList;
	auto& gwariates = check.vGundanIDList;
	if (bushous.size() == 0 || castles.size() == 0 || gwariates.size() == 0){
		OutputDebugStream("MAKEGUNDANKOUHOINFOの中身データが不正");
		return 0xFFFF;
	}

	auto bitr = std::find(bushous.begin(), bushous.end(), _m_iBushouID);
	// 武将候補以外を指定している。
	if (bitr == bushous.end()) {
		OutputDebugStream("_m_iBushouID が MAKEGUNDANKOUHOINFO.vBushouIDList にない");
		return 0xFFFF;
	}

	auto citr = std::find(castles.begin(), castles.end(), _m_iCastleID);
	// 城候補以外を指定している。
	if (citr == castles.end()) {
		OutputDebugStream("_m_iCastleID が MAKEGUNDANKOUHOINFO..vCastleIDList にない");
		return 0xFFFF;
	}

	// あきスロットを探す
	int iSelfGundanID = 0xFFFF;
	for (int iGundanID = 0; iGundanID < GAMEDATASTRUCT_GUNDAN_NUM; iGundanID++) {
		if (nb6gundan[iGundanID].attach == 0xFFFF && // 所属大名番号
			nb6gundan[iGundanID].residence == 0xFFFF &&  // 所属居城
			nb6gundan[iGundanID].next == 0xFFFF) {
			iSelfGundanID = iGundanID;
			break;
		}
	}

	// なぜか無い。安全チェック用軍団新設候補情の取得の後、Set_軍団新設を呼ぶタイミングがずれているのだろう。
	if (iSelfGundanID == 0xFFFF) {
		return 0xFFFF;
	}


	// まずは、軍団一覧の中から自分がぶら下がるべき軍団を探す。
	// それは「同じ大名番号」に所属していながら「next」が65535 すなわち、
	// リンクリストの末尾に所属している軍団である。
	int iPrevGundanID = 0xFFFF;
	for (int iGundanID = 0; iGundanID < GAMEDATASTRUCT_GUNDAN_NUM; iGundanID++) {
		// 大名が一致
		if (nb6gundan[iGundanID].attach == check.m_iDaimyoID + 1) { // 所属大名番号
			// 対象の大名内の軍団一覧のうち、末尾の軍団
			if (nb6gundan[iGundanID].next == 0xFFFF) {
				iPrevGundanID = iGundanID;
			}
		}
	}


	if (iPrevGundanID == 0xFFFF) {
		return 0xFFFF;
	}

	// 以上をクリアしていれば、まぁ大丈夫だろう。

	// -------------------------------------------------------------------
	// １つ前の軍団の次を自身とする
	nb6gundan[iPrevGundanID].next = iSelfGundanID + 1;


	// -------------------------------------------------------------------
	// 自分の前の軍団として、リンクリスト末尾の最後の軍団番号を入れる
	nb6gundan[iSelfGundanID].prev = iPrevGundanID + 1;
	// 自分自身がリンクリストの最後。
	nb6gundan[iSelfGundanID].next = 0xFFFF;

	// 空いているうちで、一番若い番号を入れる。
	nb6gundan[iSelfGundanID].number = gwariates[0];

	nb6gundan[iSelfGundanID].attach = check.m_iDaimyoID + 1;

	nb6gundan[iSelfGundanID].residence = _m_iCastleID + 1;

	nb6gundan[iSelfGundanID].leader = _m_iBushouID + 1; // 軍団長【武将番号】

	nb6gundan[iSelfGundanID].act = 50;

	tryChangeGundanYakusyoku(iSelfGundanID, 0);

	nb6gundan[iSelfGundanID].money = 1000;
	nb6gundan[iSelfGundanID].rise = 1000;

	nb6gundan[iSelfGundanID].gun = 100;
	nb6gundan[iSelfGundanID].horse = 100;



	// -------------------------------------------------------------------
	// 城の情報やリンクリストを変更する
	nb6castle[_m_iCastleID].attach = iSelfGundanID + 1;

	nb6castle[_m_iCastleID].honjo = 2; //本城 居城 (0:非本拠, 大名の本城=1, 軍団長の本城=2);

	// 自分自身(城)をリンクリストから外す。
	// 前の城の「次の城」を、自身ではなく、「自身の次の城」にする。
	if (nb6castle[_m_iCastleID].prev != 0 && nb6castle[_m_iCastleID].prev != 0xFFFF) {
		int _prev = nb6castle[_m_iCastleID].prev - 1;
		nb6castle[_prev].next = nb6castle[_m_iCastleID].next;
	}

	// 次の城の「前の城」を、自身ではなく、「自身の前の城」にする。
	if (nb6castle[_m_iCastleID].next != 0 && nb6castle[_m_iCastleID].next != 0xFFFF) {
		int _next = nb6castle[_m_iCastleID].next - 1;
		nb6castle[_next].prev = nb6castle[_m_iCastleID].prev;
	}


	// 新設なので自分が先頭は確定
	nb6castle[_m_iCastleID].prev = 0;
	// 新設なので自分が最後は確定
	nb6castle[_m_iCastleID].next = 0xFFFF;

	// -------------------------------------------------------------------

	// -------------------------------------------------------------------
	// 本人のパラメータ
	// 対象の武将の、該当城への移動
	setBushouTransport(_m_iBushouID, _m_iCastleID);

	setGenekiToJyosyu(_m_iBushouID);

	nb6bushouname[_m_iBushouID].State = 1; // 軍団長

	// 該当の城に居る人達の所属軍団の書き換え
	for (int iBushouID = 0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {
		if (nb6bushouref[iBushouID].attach == check.m_iDaimyoID + 1) { // 所属大名番号
			if (nb6bushouref[iBushouID].castle == _m_iCastleID + 1) {
				if (nb6bushouname[iBushouID].State == 1 || nb6bushouname[iBushouID].State == 2) { // 軍団長もしくは現役
					// 戦死中ではない
					if (!nb6bushouname[iBushouID].battleDeath) {
						nb6bushouref[iBushouID].gundan = iSelfGundanID + 1;
					}
				}
			}
		}
	}

	return iSelfGundanID;
}





// 家紋を決めているアルゴ(というか紐づけデータ)が不明なので、
// とりあえず、簡易なもので代用
static int _getKamonAtBorn(int iBushouID) {

	if (!(0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM)) {
		OutputDebugStream("武将番号が範囲外");
		return 0; // 返しようもないので、一旦信長のにしておく。
	}

	// 正規家紋全体の配列(但し、
	map<int, int> regular_kamons_remain_list;
	for (int k = 0; k < GAMEDATASTRUCT_KAMON_REGULAR_NUM; k++) {
		regular_kamons_remain_list.insert(map<int, int>::value_type(k, k));
	}

	// すでに大名が使っている家紋を、正規の余り家紋リストから除去することで、未使用のものだけにする
	for (int iDaimyoID = 0; iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM; iDaimyoID++) {

		int kamon = nb6daimyo[iDaimyoID].symbol;

		// 対象の家紋番号がリストにないなら、(正規家紋以外なら相手にしない)
		regular_kamons_remain_list.erase(kamon);
	}

	int bloodNormalized = nb6bushouref[iBushouID].blood;
	if (bloodNormalized >= GAMEDATASTRUCT_KAMON_REGULAR_NUM) { bloodNormalized = GAMEDATASTRUCT_KAMON_REGULAR_NUM - 1; }

	// 正規の余り家紋リストに、大名の武将の血筋番号と同じ家紋番号が未使用であまってるならそれにしておく(血筋番号と家紋番号は72ぐらいまでは一致しているため)
	if (regular_kamons_remain_list.count(bloodNormalized) > 0) {
		; // 何もしない。bloodNormalizedのそのままの数値を採用
	}
	// 正規リスト内に、大名の武将の血筋番号と同じ家紋番号が無い場合は、正規の余り家紋リスト(の余ってるの)
	else {
		// ん〜
		auto itr = regular_kamons_remain_list.begin();

		bloodNormalized = itr->first;
	}

	return bloodNormalized;
}




int Set_IndependentGundan(int _iGundanID) {
	if (!(0 <= _iGundanID && _iGundanID < GAMEDATASTRUCT_GUNDAN_NUM)) {
		return 0xFFFF;
	}

	int iSelfDaimyoID = 0xFFFF;
	for (int iDaimyoID = 0; iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM; iDaimyoID++) {
		// あいているという条件は以下の２つでわかるだろう。
		if (nb6daimyo[iDaimyoID].attach == 0xFFFF && // 大名の武将番号
			nb6daimyo[iDaimyoID].gundan == 0xFFFF) {
			iSelfDaimyoID = iDaimyoID;
			break;
		}
	}

	// 空きがなかったらダメ
	if (iSelfDaimyoID == 0xFFFF) {
		return 0xFFFF;
	}

	int iNewDaimyoBushouID = nb6gundan[_iGundanID].leader - 1; // 軍団長の武将番号
	int iNewDaimyoCastleID = nb6gundan[_iGundanID].residence - 1; // 所属城番号
	// 軍団長以外が指定されている。即ち、大名自身の(第１軍団相当の)軍団番号が指定されている。
	if (nb6bushouname[iNewDaimyoBushouID].State != 1) { // 軍団長
		return 0xFFFF;
	}

	// 戦場で死んでいて、メイン画面に戻る前。ダメ
	if (nb6bushouname[iNewDaimyoBushouID].battleDeath) { // 戦死
		return 0xFFFF;
	}

	// ----------------------------------------------------
	// ゴミ掃除
	nb6daimyo[iSelfDaimyoID].retire = 0xFFFF;
	nb6daimyo[iSelfDaimyoID].hostile = 0xFF;	// 敵対大名
	nb6daimyo[iSelfDaimyoID].friendship = 0xFF; // 友好大名
	nb6daimyo[iSelfDaimyoID].friendship = 50;   // 商人友好度
	nb6daimyo[iSelfDaimyoID].bPropagate = 0;  // 布教済
	nb6daimyo[iSelfDaimyoID].hyotei = 0;      // 評定
	nb6daimyo[iSelfDaimyoID].bMerchantAngry = 0;      // 商人が怒ってて商人不在

	if (nb6bushouref[iNewDaimyoBushouID].job == 0x60) { // 高家
		nb6daimyo[iSelfDaimyoID].tyotei = 30; // 朝廷
	}
	else {
		nb6daimyo[iSelfDaimyoID].tyotei = 0; // 朝廷
	}

	nb6daimyo[iSelfDaimyoID].symbol = _getKamonAtBorn(iNewDaimyoBushouID);

	tryChangeDaimyoYakusyoku(iSelfDaimyoID, 0);

	// ----------------------------------------------------
	// 対象の軍団長は、個人として大名となる
	nb6bushouname[iNewDaimyoBushouID].State = 0; // 0:大名
	nb6bushouname[iNewDaimyoBushouID].position = 1; // 1:大名

	// まずは新設した大名家の大名は、指定の軍団長を新設大名の武将に
	nb6daimyo[iSelfDaimyoID].attach = iNewDaimyoBushouID + 1; // 大名【武将番号】
	// 大名の本第１軍団となる
	nb6daimyo[iSelfDaimyoID].gundan = _iGundanID + 1;

	// ----------------------------------------------------
	// 現在の軍団の所属大名が、新設の大名IDに帰属
	nb6gundan[_iGundanID].attach = iSelfDaimyoID + 1; // 所属大名【大名番号】

	// 割当番号は１固定
	nb6gundan[_iGundanID].number = 1;

	// 軍団長の本拠だったという情報は、大名の本拠へ格上げ
	nb6daimyo[iSelfDaimyoID].castle = iNewDaimyoCastleID + 1; // 所属居城【城番号】
	nb6castle[iNewDaimyoCastleID].honjo = 1; //  本城::大名;

	// ----------------------------------------------------
	// リンクリストの変更。自分の軍団だけ、これまでの大名配下の軍団のリンクリストから削除する

	// 自分自身(軍団)をリンクリストから外す。
	// 前の軍団の「次の軍団」を、自身ではなく、「自身の次の軍団」にする。
	if (nb6gundan[_iGundanID].prev != 0 && nb6gundan[_iGundanID].prev != 0xFFFF) {
		int _prev = nb6gundan[_iGundanID].prev - 1;
		nb6gundan[_prev].next = nb6gundan[_iGundanID].next;
	}

	// 次の軍団の「前の軍団」を、自身ではなく、「自身の前の軍団」にする。
	if (nb6gundan[_iGundanID].next != 0 && nb6gundan[_iGundanID].next != 0xFFFF) {
		int _next = nb6gundan[_iGundanID].next - 1;
		nb6gundan[_next].prev = nb6gundan[_iGundanID].prev;
	}


	// 新設なので自分が先頭は確定
	nb6gundan[_iGundanID].prev = 0;
	// 新設なので自分が最後は確定
	nb6gundan[_iGundanID].next = 0xFFFF;

	// ----------------------------------------------------
	// 軍団配下武将の所属武将(自分を含む)の所属大名を、新設大名への変更する
	for (int iBushouID = 0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {
		if (nb6bushouref[iBushouID].gundan == _iGundanID + 1) {
			// 戦死中ではない
			if (!nb6bushouname[iBushouID].battleDeath) { // 戦死
				nb6bushouref[iBushouID].attach = iSelfDaimyoID + 1;
				nb6bushouref[iBushouID].work = 0; // 士官年数
			}
		}
	}

	// ----------------------------------------------------
	// 同盟系
	for (int iTargetDaimyoID = 0; iTargetDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM; iTargetDaimyoID++) {
		// 友好はまんなか
		setYuukouKankei(iSelfDaimyoID, iTargetDaimyoID, 50);
		// 同盟や婚姻はリセット
		setDoumeiKankei(iSelfDaimyoID, iTargetDaimyoID, FALSE);
		setKoninKankei(iSelfDaimyoID, iTargetDaimyoID, FALSE);
	}
	return iSelfDaimyoID;
}



// とある大名内でのＡ軍団の城をＢ軍団へとアタッチ変更
bool Set_ChangeCastleAttachGundan(int _l_CastleID, int _l_GundanID) {

	if (!(0 <= _l_CastleID && _l_CastleID < GAMEDATASTRUCT_CASTLE_NUM)) {
		OutputDebugStream("城番号が範囲外");
		return false;
	}

	if (nb6castle[_l_CastleID].honjo == 2 || nb6castle[_l_CastleID].honjo == 1) { // 2:軍団長の本拠 1:大名の本拠
		OutputDebugStream("大名や軍団長の本拠地は所持者を移動できない");
		return false;
	}

	if (!(0 <= _l_GundanID && _l_GundanID < GAMEDATASTRUCT_GUNDAN_NUM)) {
		OutputDebugStream("軍団番号が範囲外");
		return false;
	}

	int iGundanID = nb6castle[_l_CastleID].attach - 1; // 所属軍団軍団番号 -1

	if (!(0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM)) {
		OutputDebugStream("城番号がおそらく未統治城");
		return false;
	}

	int iOrgDaimyoID = nb6gundan[iGundanID].attach - 1; // 所属大名番号
	int iDstDaimyoID = nb6gundan[_l_GundanID].attach - 1;

	if (iOrgDaimyoID != iDstDaimyoID) {
		OutputDebugStream("城番号と新軍団番号が同じ大名のものではない");
		return false;
	}

	// -------------------------------------------------------------------
	// まず、元の軍団の城リンクリストから、自分の城を外す
	nb6castle[_l_CastleID].attach = _l_GundanID + 1;  // 所属軍団の軍団番号

	// 自分自身(城)をリンクリストから外す。
	// 前の城の「次の城」を、自身ではなく、「自身の次の城」にする。
	if (nb6castle[_l_CastleID].prev != 0 && nb6castle[_l_CastleID].prev != 0xFFFF) {
		int _prev = nb6castle[_l_CastleID].prev - 1;
		nb6castle[_prev].next = nb6castle[_l_CastleID].next;
	}

	// 次の城の「前の城」を、自身ではなく、「自身の前の城」にする。
	if (nb6castle[_l_CastleID].next != 0 && nb6castle[_l_CastleID].next != 0xFFFF) {
		int _next = nb6castle[_l_CastleID].next - 1;
		nb6castle[_next].prev = nb6castle[_l_CastleID].prev;
	}

	// -------------------------------------------------------------------
	// 次に、新軍団の城リンクリストの最後の城を探す
	int iTailCastleID = 0xFFFF;
	for (int iCastleID = 0; iCastleID < GAMEDATASTRUCT_CASTLE_NUM; iCastleID++) {
		// 対象の城の所属軍団が、指定の新軍団であり
		if (nb6castle[iCastleID].attach - 1 == _l_GundanID) {
			// その城がリンクリストの最後の城であるならば
			if (nb6castle[iCastleID].next == 0xFFFF) {
				iTailCastleID = iCastleID;
				break;
			}
		}
	}

	if (iTailCastleID == 0xFFFF) {
		OutputDebugStream("異常事態!! 城のリンクリストが壊れている!!");
		return false;
	}

	// 自分は新軍団ではリンクリストの末尾は確定なので次の城はない
	nb6castle[_l_CastleID].next = 0xFFFF;

	// 自分の１つ前は、これまで末尾だった城の番号
	nb6castle[_l_CastleID].prev = iTailCastleID + 1;

	// これまでリンクリストの末尾だった城の次の城が自分
	nb6castle[iTailCastleID].next = _l_CastleID + 1;

	// -------------------------------------------------------------------
	// その城にいる武将達の所属を変更する
	// 該当の城に居る人達の所属軍団の書き換え
	for (int iBushouID = 0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {
		if (nb6bushouref[iBushouID].attach == iOrgDaimyoID + 1) {  // 所属大名番号
			if (nb6bushouref[iBushouID].castle == _l_CastleID + 1) { // 所属居城
				if (nb6bushouname[iBushouID].State == 1 || nb6bushouname[iBushouID].State == 2) { // 1:軍団長 2:現役
					// 戦死中ではない
					if ( ! nb6bushouname[iBushouID].battleDeath) {
						nb6bushouref[iBushouID].gundan = _l_GundanID + 1;
					}
				}
			}
		}
	}

	return true;

}




// 全有効大名のリストを返す。ターン大名自身も入ってるので注意。
vector<int> getDaimyoList(BOOL isIncludeTurnDaimyo) {
	vector<int> list;
	for ( int iDaimyoID=0; iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM; iDaimyoID++ ) {
		// ターン大名は含まない
		if ( !isIncludeTurnDaimyo ) {
			// ターン大名ならパス
			if ( iDaimyoID == nb6turn.daimyo-1 ) {
				continue;
			}
		}
		// ちゃんと武将が納めている大名家のみ
		if ( nb6daimyo[iDaimyoID].attach != 0xFFFF ) {
			list.push_back(iDaimyoID);
		}
	}
	return list;
}



// Ａ大名(０始まりでの配列用指定)とＢ大名との同盟関係）
// 004CF213  05 55 55 55 95    とあるのは、004CF213 [0 5 5 5 5 5 5 5 9 5] というように1番目、2番目、3番目の相手…という感じとなる。  
// 一方同盟・婚姻情報はどこにあるのかというと、2人の組み合わせのうち、大きい方の大名番号用の同じ構造体に入っている。
// 例えば、大名番号21と25とすると、友好情報は21番の方の25番目に入っており、同盟や婚姻は、25番の方の21番目に入っているというわけだ。


// 下のgetDoumeiKankei内部で求まる値は、この配列のINDEXであるため、これを使って実際の値に直す。
int YuukouValueArray[] = {0, 10, 20, 30, 40, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 0xFFFF};


byte* getYuukouKannkeiPointer(int& iDaimyoA_ID, int& iDaimyoB_ID) {

	// 範囲チェック
	if ( 0 <= iDaimyoA_ID && iDaimyoA_ID < GAMEDATASTRUCT_DAIMYO_NUM &&
 		 0 <= iDaimyoB_ID && iDaimyoB_ID < GAMEDATASTRUCT_DAIMYO_NUM ) {
	} else {
		return NULL;
	}

	// 大名Ａと大名Ｂで、大名Ａの方が値が大きい場合、入れ替える。
	// なぜなら友好関係の値は、大名Ａが小さい方の組み合わせテーブルに入っていて、大名Ａが大きい方の組み合わせテーブルには婚姻や同盟の値が入っているから。
	if (iDaimyoA_ID > iDaimyoB_ID ) {
		// ２つ入れ替え
		iDaimyoA_ID ^= iDaimyoB_ID;
		iDaimyoB_ID ^= iDaimyoA_ID;
		iDaimyoA_ID ^= iDaimyoB_ID;
	}

	byte *p = (byte *)GAMEDATASTRUCT_DOUMEI_ADDRESS;
	
	// 以下2人の大名ＩＤより、同盟関係値が入っているメモリのアドレスを求める
	p += iDaimyoA_ID * ( GAMEDATASTRUCT_DAIMYO_NUM / 2 ); // もしも１つのＡ大名あたり、Ｂ大名への同盟が１バイトで表現されていれば、
															 // ７２大名いるので、７２バイトで表現される。
															 // ところが実際には、4ビットで表現されるので、
															 // １つの大名にぶら下がる構造体データとしては、半分の３６バイトである。

	p += iDaimyoB_ID / 2;									 // 上と同じ理由となる。１つの大名の(同盟関係構造体)の中でも
															 // １つの相手大名との同盟値に使われているのは半バイト。
															 // よってポインタの位置としては、/2 しておき、偶数か奇数かで、後で演算を振り分ける。

	return p;
}

int getYuukouKankei(int iDaimyoA_ID, int iDaimyoB_ID) {

	// 該当の友好関係情報が存在するアドレスを得る。ＡとＢは値が交換されるかもしれない。注意。
	byte *p = getYuukouKannkeiPointer( iDaimyoA_ID, iDaimyoB_ID );
	if (!p) return 0;

	// アドレスは求まったが、１つのアドレスの1バイト内には、２つの大名が入っている。
	// 大名番号が偶数なら上位桁、偶数なら下位桁である。
	int YuukouIX = 0;
	// 相手大名が偶数だ
	if ( iDaimyoB_ID % 2==0 ) {								 // 偶数であれば、上半分のビットとの＆結果[0 5] のうち[0]を返す
		YuukouIX = (*p) >> 4;

	// 相手大名が奇数だ										 // 奇数であれば、下半分のビットとの＆結果[0 5] のうち[5]を返す
	} else {
		YuukouIX = 0x0F & (*p);
	}

	return YuukouValueArray[YuukouIX];
}

// 友好関係値をセットする。
void setYuukouKankei(int iDaimyoA_ID, int iDaimyoB_ID, int iValue) {

	// 該当の友好関係情報が存在するアドレスを得る。ＡとＢは値が交換されるかもしれない。注意。
	byte *p = getYuukouKannkeiPointer( iDaimyoA_ID, iDaimyoB_ID );
	if (!p) return;


	// 負数はヤヴァイので友好関係値として０を指定したものとする。100オーバーも100にしとく。
	if ( iValue < 0 )	{ iValue = 0;	}
	if ( iValue > 100 ) { iValue = 100; }

	int YuukouIX = 0;
	// セットしたい友好関係値と、友好関係値の配列を比較して、もっとも近い切捨て値のindexを求める。
	// 例えば友好関係値として53というように指定されても、そのような値には出来ないので、50とするわけだ。
	// そして50は、YuukouValueArrayのindex==5に存在する。この5のような値を求めたい。
	for ( int i=0; i < sizeof(YuukouValueArray)/sizeof(YuukouValueArray[0]); i++) {
		if ( iValue < YuukouValueArray[i] ) {

			YuukouIX = i-1;	// 値が超えたので、１つ前のindexにしとく。

			if ( YuukouIX < 0 ) { YuukouIX = 0; }  // 理論上ありえないが、何足すかわからんので一応。
			break;
		}
	}

	// アドレスは求まったが、１つのアドレスの1バイト内には、２つの大名が入っている。
	// 大名番号が偶数なら上位桁、偶数なら下位桁である。
	// 相手大名が偶数だ
	if ( iDaimyoB_ID % 2==0 ) {								 // 偶数であれば、上半分のビットとの＆結果[0 5] のうち[0]を返す
		YuukouIX = YuukouIX << 4;
		// 上位バイトはYuukouIXをシフトしたものを、下位バイトは下位バイトの部分だけ抽出で
		*p = YuukouIX | (0x0F & (*p));

	// 相手大名が奇数だ										 // 奇数であれば、下半分のビットとの＆結果[0 5] のうち[5]を返す
	} else {
		// 上位バイトはそのまま採用で、下位バイトは、YuukouIXそのままで。
		*p = (0xF0 & (*p)) | YuukouIX;
	}
}

byte* getDoumeiKankeiPointer(int& iDaimyoA_ID, int& iDaimyoB_ID) {

	// 範囲チェック
	if ( 0 <= iDaimyoA_ID && iDaimyoA_ID < GAMEDATASTRUCT_DAIMYO_NUM &&
 		 0 <= iDaimyoB_ID && iDaimyoB_ID < GAMEDATASTRUCT_DAIMYO_NUM ) {
	} else {
		return NULL;
	}

	// 大名Ａと大名Ｂで、大名Ａの方が値が小さい場合、入れ替える。
	// なぜなら同盟関係の値は、大名Ａが大きい方の組み合わせテーブルに入っているから。
	if (iDaimyoA_ID < iDaimyoB_ID ) {
		// ２つ入れ替え
		iDaimyoA_ID ^= iDaimyoB_ID;
		iDaimyoB_ID ^= iDaimyoA_ID;
		iDaimyoA_ID ^= iDaimyoB_ID;
	}


	byte *p = (byte *)GAMEDATASTRUCT_DOUMEI_ADDRESS;
	
	// 以下2人の大名ＩＤより、同盟関係値が入っているメモリのアドレスを求める
	p += iDaimyoA_ID * ( GAMEDATASTRUCT_DAIMYO_NUM / 2 ); // もしも１つのＡ大名あたり、Ｂ大名への同盟が１バイトで表現されていれば、
															 // ７２大名いるので、７２バイトで表現される。
															 // ところが実際には、4ビットで表現されるので、
															 // １つの大名にぶら下がる構造体データとしては、半分の３６バイトである。

	p += iDaimyoB_ID / 2;									 // 上と同じ理由となる。１つの大名の(同盟関係構造体)の中でも
															 // １つの相手大名との同盟値に使われているのは半バイト。
															 // よってポインタの位置としては、/2 しておき、偶数か奇数かで、後で演算を振り分ける。
	return p;
}

int isDoumeiKankei(int iDaimyoA_ID, int iDaimyoB_ID) {

	// 該当の同盟関係情報が存在するアドレスを得る。ＡとＢは値が交換されるかもしれない。注意。
	byte *p = getDoumeiKankeiPointer( iDaimyoA_ID, iDaimyoB_ID );
	if (!p) return 0;

	// アドレスは求まったが、１つのアドレスの1バイト内には、２つの大名が入っている。
	// 大名番号が偶数なら上位桁、偶数なら下位桁である。
	// 相手大名が偶数だ
	if ( iDaimyoB_ID % 2==0 ) {								 // 偶数であれば、上半分のビット
		return 0x10 & (*p);
	// 相手大名が奇数だ										 // 奇数であれば、下半分のビット
	} else {
		return 0x01 & (*p);
	}
}
int isKoninKankei(int iDaimyoA_ID, int iDaimyoB_ID) {

	// 該当の婚姻関係情報が存在するアドレスを得る(同盟と同じ場所)。ＡとＢは値が交換されるかもしれない。注意。
	byte *p = getDoumeiKankeiPointer( iDaimyoA_ID, iDaimyoB_ID );
	if (!p) return 0;

	// アドレスは求まったが、１つのアドレスの1バイト内には、２つの大名が入っている。
	// 大名番号が偶数なら上位桁、偶数なら下位桁である。

	if ( iDaimyoB_ID % 2==0 ) {								 // 偶数であれば、上半分のビット
		return (0x20 & (*p)) > 0;							 // ２との比較では間違うかもしれないので、一応BOOLっぽくしておく。
	// 相手大名が奇数だ										 // 奇数であれば、下半分のビット
	} else {
		return (0x02 & (*p)) > 0;							 // ２との比較では間違うかもしれないので、一応BOOLっぽくしておく。
	}
}

void setDoumeiKankei(int iDaimyoA_ID, int iDaimyoB_ID, BOOL doDoumei) {

	// 該当の同盟関係情報が存在するアドレスを得る。ＡとＢは値が交換されるかもしれない。注意。
	byte *p = getDoumeiKankeiPointer( iDaimyoA_ID, iDaimyoB_ID );
	if (!p) return;
	
	// アドレスは求まったが、１つのアドレスの1バイト内には、２つの大名が入っている。
	// 大名番号が偶数なら上位桁、偶数なら下位桁である。
	// 相手大名が偶数だ
	if ( iDaimyoB_ID % 2==0 ) {								 // 偶数であれば、上半分のビット
		if (doDoumei) {
			*p = *p | 0x10;
		}
		else {
			*p = *p & 0xEF;
		}
		// 相手大名が奇数だ										 // 奇数であれば、下半分のビット
	} else {
		if (doDoumei) {
			*p = *p | 0x01;
		}
		else {
			*p = *p & 0x0E;
		}
	}
}


void setKoninKankei(int iDaimyoA_ID, int iDaimyoB_ID, BOOL doKonin) {

	// 該当の婚姻関係情報が存在するアドレスを得る(同盟と同じ場所)。ＡとＢは値が交換されるかもしれない。注意。
	byte *p = getDoumeiKankeiPointer( iDaimyoA_ID, iDaimyoB_ID );
	if (!p) return;
	
	// アドレスは求まったが、１つのアドレスの1バイト内には、２つの大名が入っている。
	// 大名番号が偶数なら上位桁、偶数なら下位桁である。
	// 相手大名が偶数だ
	if ( iDaimyoB_ID % 2==0 ) {								 // 偶数であれば、上半分のビット
		if (doKonin) {
			*p = *p | 0x20;
		}
		else {
			*p = *p & 0xEF;
		}
		// 相手大名が奇数だ										 // 奇数であれば、下半分のビット
	} else {
		if (doKonin) {
			*p = *p | 0x02;
		}
		else {
			*p = *p & 0x0E;
		}
	}
}




int DaimyoHasCastleArray[GAMEDATASTRUCT_DAIMYO_NUM] = {}; // 各々の大名が持つ城数

//最大勢力(最も持ち城数が多い)大名を得る
int GetMaximumInfluenceDaimyo() {
	// まず、一応呼ばれる度にリセット
	for (int iDaimyoID=0; iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM; iDaimyoID++ ) {
		DaimyoHasCastleArray[iDaimyoID] = 0;
	}

	for (int iCastleID=0; iCastleID < GAMEDATASTRUCT_CASTLE_NUM; iCastleID++ ) {
		// 所属大名番号に従って、カウントを増やしてゆく。
		int iGundanID = nb6castle[iCastleID].attach-1;
		if ( iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) { // 誰にも所属してない城はこの条件で弾かれる
			int iDaimyoID = nb6gundan[iGundanID].attach-1;
			DaimyoHasCastleArray[iDaimyoID]++;
		}
	}

	// 最大の城持ちの大名IDを探す
	int iMaximumInfluenceDaimyoID = 0;

	// 比較相手なので1スタートで良い
	for (int iDaimyoID=1; iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM; iDaimyoID++ ) {
		if ( DaimyoHasCastleArray[iDaimyoID] > DaimyoHasCastleArray[iMaximumInfluenceDaimyoID] ) {
			iMaximumInfluenceDaimyoID = iDaimyoID;
		}
	}

	return iMaximumInfluenceDaimyoID;
}

/*
004D1490  C0 FD 4C 00 B4 FD 4C 00 A8 FD 4C 00 9C FD 4C 00  ﾀ�L.ｴ�L.ｨ�L.・L.
004D14A0  8C FD 4C 00 80 FD 4C 00 70 FD 4C 00 60 FD 4C 00  ・L.��L.p�L.`�L.
004D14B0  54 FD 4C 00 48 FD 4C 00                          T�L.H�L.
*/
char *getGoyouSyouninMei(int iGoyousyouninID) {
	if ( 0 <= iGoyousyouninID && iGoyousyouninID <= GAMEDATASTRUCT_SYOUNIN_NUM ) {
		int *pArrayGoyouSyoninName = (int *)GAMEDATASTRUCT_GOYOUSYOUNINNAME_ARRAY;
		return (char *)(pArrayGoyouSyoninName[iGoyousyouninID]);
	} else {
		return "";
	}
}











// 家宝の種類の文字列へのポインタのアドレス一覧
int GAMEDATASTRUCT_KAHOUTYPELIST_POINTER[GAMEDATASTRUCT_KAHOUTYPE_NUM] = {NULL};




// 家宝タイプのIDから文字列を得る
char* getKahouTypeName(byte type) {
	char *pszKahouType = NULL;
	if ( type < GAMEDATASTRUCT_KAHOUTYPE_NUM ) {
		pszKahouType = (char *)(GAMEDATASTRUCT_KAHOUTYPELIST_POINTER[type]);
		return pszKahouType;
	}
	return NULL;
}


// セーブデータのファイル名
char *psznb6savedatafilename = (char *)GAMEDATASTRUCT_SAVEDATAFILENAME_ADDRESS;



// 天気情報を得る 0:晴れ 1:曇り 2:雨 3:雪
byte getWeather() {

	byte *weather = (byte *)GAMEDATASTRUCT_WEATHER_ADDRESSS;

	return *weather;
}



// 天気の予約
void reserveWeather(int eTenki, int iContinueTurn) {
	iReservedWeatherOnChangeWeather.clear();
	if ( getRemainTurnInBattle() > 0 ) {
		TReservedTenki tk = { getRemainTurnInBattle(), eTenki, iContinueTurn };
		iReservedWeatherOnChangeWeather.push_back(tk);
	}
}
// 天気の予約
void WINAPI Extern_reserveWeather(int eTenki, int iContinueTurn) {
	reserveWeather(eTenki, iContinueTurn);
}


// 戦争ですでに何ターン経過したか
byte getHowMuchTurnProceesInBattle() {

	byte *turn = (byte *)GAMEDATASTRUCT_HOWMUCHTURNPROCEED_INBATTLE;

	return *turn;
}

// 戦争で何ターン残っているか。メモリではなく、内部計算
extern int iCurIncrementTurnInBattle;
int getRemainTurnInBattle() {
	// 戦闘ターン中以外であれば、-1を返す。
	if (iCurIncrementTurnInBattle < 0) {
		return iCurIncrementTurnInBattle;
	}
	// 戦争中なら、30から経過ターンを引いたものを返す。
	return 31-iCurIncrementTurnInBattle;
}

// 外部参照用
int WINAPI Extern_getRemainTurnInBattle() {
	return getRemainTurnInBattle();
}


// 鉄砲伝来済みかどうか
bool isTeppouDenrai() {
	byte *bTeppouDenrai = (byte *)GAMEDATASTRUCT_EVENT_GROUP1_ADDRESSS;

	int isTeppouDenrai = *bTeppouDenrai & 0x01; // (= b0b00000001) 

	if (isTeppouDenrai) { return true; } else { return false; }
}

// 鉄砲伝来をやめる
void UnsetTeppouDenrai() {
	byte *bTeppouDenrai = (byte *)GAMEDATASTRUCT_EVENT_GROUP1_ADDRESSS;

	*bTeppouDenrai = *bTeppouDenrai & 0xFE; // ( & 11111101 ) することで、鉄砲伝来だけを消す。 
}


// キリスト教伝来済みかどうか
bool isChristDenrai() {
	byte *bChristDenrai = (byte *)GAMEDATASTRUCT_EVENT_GROUP1_ADDRESSS;

	int isChristDenrai = *bChristDenrai & 0x02; // (= b0b00000010) 

	if (isChristDenrai) { return true; } else { return false; }
}

// 安土城が建設済みかどうか
bool isAzuchiCastleComplete() {
	byte *bAzuchiCastleComplete = (byte *)GAMEDATASTRUCT_EVENT_GROUP1_ADDRESSS;

	int isAzuchiCastleComplete = *bAzuchiCastleComplete & 0x08; // (= b0b00001000) 

	if (isAzuchiCastleComplete) { return true; } else { return false; }
}

// 大坂城が建設済みかどうか
bool isOsakaCastleComplete() {
	byte *bOsakaCastleComplete = (byte *)GAMEDATASTRUCT_EVENT_GROUP2_ADDRESSS;

	int isOsakaCastleComplete = *bOsakaCastleComplete & 0x40; // (= b0b01000000) 

	if (isOsakaCastleComplete) { return true; } else { return false; }
}

// キリスト教伝来をやめる
void UnsetChristDenrai() {
	byte *bChristDenrai = (byte *)GAMEDATASTRUCT_EVENT_GROUP1_ADDRESSS;

	*bChristDenrai = *bChristDenrai & 0xFD; // ( & 11111101 ) することで、鉄砲伝来だけを消す。 
}



// SEは有効か？
bool isSEValid() {

	byte bSEValid = 0;

	bSEValid = nb6settings.se; // (= b0b00000001) 

	if (bSEValid) { return true; } else { return false; }

}

// BGMは有効か？
bool isBGMValid() {

	byte bBGMValid = 0;

	bBGMValid = nb6settings.bgm; // (= b0b00000010) 

	if (bBGMValid) { return true; } else { return false; }
}

// ムービーは有効か？
bool isMovieValid() {

	byte bMovieValid = 0;

	bool bMovieInValid = nb6settings.movie_is_off; // (= b0b00010000) 

	if (!bMovieInValid) { return true; } else { return false; }

}

// _INMMモードか？
bool is_InmmMode() {
	char *winmm = (char *)GAMEDATASTRUCT_WINMM_DLL_NAME_ADDRESS;
	if ( strcmp( winmm, "_INMM.dll") == 0 ) {
		return true;
	} else if ( TSModCommand::Environment::cmdBGMMode == 1 || TSModCommand::Environment::cmdBGMMode == 2 ) {
		return true;
	}
	return false;
}


// デモモードか？
bool is_DemoMode() {
	for ( int iPG=0; iPG<GAMEDATASTRUCT_PLAYERS8_NUM; iPG++) {
		// プレイヤーが担当している軍団のＩＤを確保
		int iGundanID = nb6players8[iPG].gundan;

		// １つでもプレイヤ担当軍団番号が0xFFFF以外の値があれば、デモモードではない。
		if ( iGundanID != 0xFFFF ) {
			return false;
		}
	}

	// 全部0xFFFFならデモモード
	return true;
}


// 相性の輪配列。(計算用)
int arrayAisho[32] = {8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7};

// A武将とB武将の相性の差分を得る
// 最小0(相性最高) 最大8(相性最悪)
int getAishoDiff(int iAishoA, int iAishoB) {

	// まず、A武将の相性位置を中央から取る
	int iAAishoIXInArray = 0;
	for (int iAishoID=8; iAishoID<8+GAMEDATASTRUCT_AISHOU_NUM; iAishoID++ ) {
		// Aの相性値を見つけたら、そこがAの位置
		if ( arrayAisho[iAishoID] == iAishoA ) {
			iAAishoIXInArray = iAishoID;
		}
	}

	// 不正な数字の場合は、相性差は最も悪いものとする。
	if ( iAAishoIXInArray == 0 ) { return 8; }

	// B武将の相性位置はA武将の-8からスタート
	int iBAishoIXInArray = 0;
	for (int iAishoID=iAAishoIXInArray-8; iAishoID<iAAishoIXInArray+8; iAishoID++ ) {
		// Aの相性値を見つけたら、そこがAの位置
		if ( arrayAisho[iAishoID] == iAishoB ) {
			iBAishoIXInArray = iAishoID;
		}
	}
	
	// 不正な数字の場合は、相性差は最も悪いものとする。
	if ( iBAishoIXInArray == 0 ) { return 8; }

	// ２つの相性の位置の差分が相性差
	return abs(iBAishoIXInArray-iAAishoIXInArray);
}


/*
 * 2人の武将IDから相性の違いを得る。
 * 最小0(相性最高) 最大8(相性最悪)
 */
int getBushouAishoDiff(int iBushouAID, int iBushouBID) {
	// 武将Aも武将Bも範囲内に収まっている
	if ( 0 <= iBushouAID && iBushouAID < GAMEDATASTRUCT_BUSHOU_NUM &&  0 <= iBushouBID && iBushouBID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		// 武将の相性の差を返す
		return getAishoDiff( nb6bushouref[iBushouAID].affinity, nb6bushouref[iBushouBID].affinity );
	}
	// 一番悪い値を返しておく
	else {
		return 8;
	}
}









