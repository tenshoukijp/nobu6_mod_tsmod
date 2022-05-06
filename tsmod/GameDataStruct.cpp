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


NB6BUSHOU_HEX_UNIT_INFO *const nb6bushou_hex_unit_info = (NB6BUSHOU_HEX_UNIT_INFO *)GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_ADDRESS; // íê‚É‚¨‚¯‚éŠe•«‚Ìî•ñ
NB6GUNDAN_HEX_INFO *const _nb6gundan_hex_info = (NB6GUNDAN_HEX_INFO *)GAMEDATASTRUCT_WAR_HEX_TURN_ADDRESS; 

NB6WAR_JOINT_DAIMYO_HEX_GROUP_INFO *const nb6war_joint_daimyo_hex_group_info = (NB6WAR_JOINT_DAIMYO_HEX_GROUP_INFO *)GAMEDATASTRUCT_WAR_JOINT_DAIMYO_HEX_GROUP_INFO_ADDRESS; // íê‚ÉQ‰Á‚µ‚½‘å–¼‚â‘‘å«‚Ìî•ñ

NB6FUKIDASHI_DIALOG_LAYOUT *const _nb6fukidashi_dialog_layout = (NB6FUKIDASHI_DIALOG_LAYOUT *)GAMEDATASTRUCT_FUKIDASHI_DIALOG_LAYOUT_ADDRESS; // 

// Œ»İƒvƒŒƒC’†‚ÌƒVƒiƒŠƒIƒƒP[ƒVƒ‡ƒ“‚ğ“¾‚é(0`5, •s–¾‚Èê‡-1)

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
004D3728  [E4 36 4D 00] [C4 36 4D 00] [A4 36 4D 00] [84 36 4D 00]  EM.Ä6M.¤6M.EM.
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


// Œ»İ‚ÌƒQ[ƒ€is’†‚Ì”N”‚ğ“¾‚éB
int getYearAD() {
	return nb6year.year + getStartYearAD();
}

// ƒQ[ƒ€‚ÌŠJn”NBƒfƒtƒHƒ‹ƒg‚Í1454”N‚Å‚ ‚é‚ªA‰ü‘¢‚µ‚½ê‡‚É”õ‚¦‚ÄƒAƒhƒŒƒX‚©‚ç“¾‚é‚æ‚¤‚É‚·‚éB
int getStartYearAD() {
	return *((WORD *)GAMEDATASTRUCT_START_YEAR_ADDRESS);
}

// Œ»İ‚ÌƒQ[ƒ€is’†‚Ì‹Gß‚ğ“¾‚é
char szSeasonName[5][3] = {"t", "‰Ä", "H", "“~", "–³"};
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

// •«‚Ì”N—î‚ğ“¾‚é
int getYearsOld( int iBushouID ) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		return nb6year.year - nb6bushouref[iBushouID].birth + 1;
	}
	return 0xFF;
}

/*
* •«‚Ì”N—î‚ğİ’è‚·‚éB
*/
bool setYearsOld(int iBushouID, int iYearsOld) {

	// ”ÍˆÍƒ`ƒFƒbƒN
	if (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		int check = nb6year.year + 1 - iYearsOld;
		if (check < 0) {
			OutputDebugStream("w’è”N—î‚Í”ÍˆÍŠOB’a¶”N‚ªŒ‹‰Ê‚Æ‚µ‚ÄA“VãÄ‹LŠî€”N(1454)‚æ‚è‘‚¢¶‚Ü‚ê‚Æ‚È‚Á‚Ä‚µ‚Ü‚Á‚Ä‚¢‚éB");
			check = 0;
		}
		if (check > 255) {
			OutputDebugStream("w’è”N—î‚Í”ÍˆÍŠOB’a¶”N‚ªŒ‹‰Ê‚Æ‚µ‚ÄA“VãÄ‹LÅI”N(1708)‚æ‚è’x‚¢¶‚Ü‚ê‚Æ‚È‚Á‚Ä‚µ‚Ü‚Á‚Ä‚¢‚éB");
			check = 255;
		}
		nb6bushouref[iBushouID].birth = check;
		return true;
	}
	else {
		OutputDebugStream("iBushouID‚Í”ÍˆÍŠO");
		return false;
	}
}




// ‘ÎÛ‚ÌŒR’cID‚ª‚¢‚¸‚ê‚©‚ÌƒvƒŒƒCƒ„[¨—Í‰º‚ÌŒR’c‚Å‚ ‚é(Å‘å‚Wl)
// iTargetGundanID‚É‚Í”z—ñã‚Ì”Ô†(attach-1‚Ì”)‚ğ“n‚·‚±‚Æ
bool isPlayerGundan( WORD iTargetGundanID ) {

	if (iTargetGundanID >= GAMEDATASTRUCT_GUNDAN_NUM) { 
		return false;
	}

	WORD iGundanID = 0;
	for ( int iPG=0; iPG<GAMEDATASTRUCT_PLAYERS8_NUM; iPG++) {
		// ƒvƒŒƒCƒ„[‚ª’S“–‚µ‚Ä‚¢‚éŒR’c‚Ì‚h‚c‚ğŠm•Û
		iGundanID = nb6players8[iPG].gundan;

		// 0xFFFF‚È‚ç‚ÎƒvƒŒƒCƒ„[‚Å‚Í‚È‚¢
		if ( iGundanID == 0xFFFF ) {
			continue;
		}

		// ƒvƒŒƒCƒ„[’S“–‚ÌŒR’c‚Ì‘å–¼‚ªAƒ^[ƒQƒbƒg‚Æ‚µ‚Ä‚¢‚é‘ÎÛ‚ÌŒR’c‚Ì‘å–¼‚Æˆê’v‚·‚é‚È‚ç‚ÎA
		// ‚»‚ê‚ÍƒvƒŒƒCƒ„[‚Ì¨—Í‰º‚ÌŒR’c‚Å‚ ‚éB
		// ‚Ç‚¤‚ànb6player8‚Ìgundan‚Ì’l‚ÍA“à•”ŠÇ—‚Ìgundan‚Ì’l‚æ‚è‚à+1‚³‚ê‚Ä‚¢‚é‚æ‚¤‚Å‚ ‚éB
		// ‚æ‚Á‚Ä‚±‚±‚Å‚Í-1‚·‚éB
		if (iGundanID >= 1 && nb6gundan[iGundanID-1].attach == nb6gundan[iTargetGundanID].attach ) {
			return true;
		}
	}
	
	return false;
}

// ‘ÎÛ‚Ì•«ID‚ª‚¢‚¸‚ê‚©‚ÌƒvƒŒƒCƒ„[¨—Í‰º‚Ì•«‚Å‚ ‚é
bool isPlayerBushou( WORD iTargetBushouID ) {
	// •«ID‚Ì”ÍˆÍ‚Éû‚Ü‚Á‚Ä‚¢‚é‚±‚Æ
	if ( 0 <= iTargetBushouID && iTargetBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		// ƒ^[ƒQƒbƒg‚Ì•«‚ÌŒR’cID‚ªƒvƒŒƒCƒ„[‚ÌŒR’c‚Å‚ ‚é‚©‚ğ•Ô‚·
		return isPlayerGundan(nb6bushouref[iTargetBushouID].gundan-1);
	} else {
		return false;
	}
}

// ‘ÎÛ‚Ì‘å–¼ID‚ª‚¢‚¸‚ê‚©‚ÌƒvƒŒƒCƒ„[’S“–‚Ì‘å–¼‚Å‚ ‚é
bool isPlayerDaimyo( WORD iTargetDaimyoID ) {
	// •«ID‚Ì”ÍˆÍ‚Éû‚Ü‚Á‚Ä‚¢‚é‚±‚Æ
	if ( 0 <= iTargetDaimyoID && iTargetDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM ) {
		// ƒ^[ƒQƒbƒg‚Ì•«‚ÌŒR’cID‚ªƒvƒŒƒCƒ„[‚ÌŒR’c‚Å‚ ‚é‚©‚ğ•Ô‚·
		return isPlayerGundan(nb6daimyo[iTargetDaimyoID].gundan-1);
	} else {
		return false;
	}
}


// ‘«ŒyE‹R”nE“S–CE…ŒR‚Ì“K³‚ğ”’l‚Å“¾‚éB(0=E,1=D,2=C,3=B,4=A,5=S)
int getBushouAptitude( int iBushouID, APTITUDE eAptitude ) {
	if (eAptitude==APTITUDE_ASIGARU) {
		return nb6bushouref[iBushouID].aptitudeasi;
	} else if (eAptitude==APTITUDE_KIBA) {
		return nb6bushouref[iBushouID].aptitudekib;
	} else if (eAptitude==APTITUDE_TEPPOU) {
		// “S–C‚Í‚Q‚Â‚ÌƒtƒBƒbƒgƒtƒB[ƒ‹ƒh‚É•ª‚©‚ê‚Ä‚µ‚Ü‚Á‚Ä‚¢‚é‚Ì‚ÅAŒvZ
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

// ‘«ŒyE‹R”nE“S–CE…ŒR‚Ì“K³‚ğ”’l‚Åİ’è‚·‚éB(0=E,1=D,2=C,3=B,4=A,5=S)
void setBushouAptitude( int iBushouID, APTITUDE eAptitude, int iTekisei ) {
	if (eAptitude==APTITUDE_ASIGARU) {
		nb6bushouref[iBushouID].aptitudeasi = iTekisei;
	} else if (eAptitude==APTITUDE_KIBA) {
		nb6bushouref[iBushouID].aptitudekib = iTekisei;
	} else if (eAptitude==APTITUDE_TEPPOU) {
		// “S–C‚Í‚Q‚Â‚ÌƒtƒBƒbƒgƒtƒB[ƒ‹ƒh‚É•ª‚©‚ê‚Ä‚µ‚Ü‚Á‚Ä‚¢‚é‚Ì‚ÅAŒvZ
		nb6bushouref[iBushouID]._aptitudegun_hi = iTekisei & 0x0003;  // 0b000000HH
		nb6bushouref[iBushouID]._aptitudegun_lo = iTekisei >> 2;      // 0b00000L00
	} else if (eAptitude==APTITUDE_SUIGUN) {
		nb6bushouref[iBushouID].aptitudesui = iTekisei;

	}
	else {
		Extern_setBushouExAptitude(iBushouID, eAptitude, iTekisei);
	}
}

// ‹|‚Ì“K«’l‚ğİ’è‚·‚é(ScenarioMod“™‚©‚ç‚Ìİ’è) (0=E,1=D,2=C,3=B,4=A,5=S) ’l‚Ìæ“¾‚àŒ“‚Ë‚éB
int WINAPI Extern_setBushouExAptitude(int iBushouID, APTITUDE eAptitude, int iTekisei ) {
	if ( eAptitude==APTITUDE_YUMI) {
		// -1‚È‚ç‚Îİ’è‚Å‚Í‚È‚­A’l‚Ìæ“¾
		if ( iTekisei == -1 ) {
			return nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi;
		}
		// —LŒø‚È’l‚È‚ç‘ã“ü
		else {
			// ‚à‚µ‹|‚ª‚È‚¢‚Ì‚Å‚ ‚ê‚ÎA‚O‚Åã‘‚«
			if ( TSModCommand::World::cmdTheYumiNoExists ) {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = 0;
			} else {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = iTekisei;
			}
			return iTekisei;
		}

	} else if (eAptitude==APTITUDE_YARI) {
		// -1‚È‚ç‚Îİ’è‚Å‚Í‚È‚­A’l‚Ìæ“¾
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




// •«‚Ì“Æ—§’l‚ğ“¾‚é
int getBushouIndependence( int iBushouID ) {
	// “S–C‚Í‚Q‚Â‚ÌƒtƒBƒbƒgƒtƒB[ƒ‹ƒh‚É•ª‚©‚ê‚Ä‚µ‚Ü‚Á‚Ä‚¢‚é‚Ì‚ÅAŒvZ
	int ret = nb6bushouref[iBushouID]._independence_lo << 2;
	ret += nb6bushouref[iBushouID]._independence_hi;
	return ret;
}

// •«‚Ì“Æ—§’l‚ğİ’è‚·‚é
void setBushouIndependence( int iBushouID, int iIndependence ) {
	nb6bushouref[iBushouID]._independence_hi = iIndependence & 0x0003;  // 0b000000HH
	nb6bushouref[iBushouID]._independence_lo = iIndependence >> 2;      // 0b00000L00
}

// •«‚Ìí“¬’l
int getBushouBat(int iBushouID) {
	// íË~ím/2000‚ª
	return int(nb6bushouref[iBushouID].maxbat * (float(nb6bushouref[iBushouID].expbat) / float(2000)));
}
// •«‚Ì’q–d’l
int getBushouInt(int iBushouID) {
	// íË~ím/2000‚ª
	return int(nb6bushouref[iBushouID].maxint * (float(nb6bushouref[iBushouID].expint) / float(2000)));
}
// •«‚Ì­¡’l
int getBushouGov(int iBushouID) {
	// íË~ím/2000‚ª
	return int(nb6bushouref[iBushouID].maxgov * (float(nb6bushouref[iBushouID].expgov) / float(2000)));
}

// ‰Æ•ó‚È‚Ç‚ğŠÜ‚ß‚È‚¢‘f‚Ì­¡Ë”\
int getBushouPureGovMax(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int upparam = 0; // ã¸’l
		for (int iKahouID =0; iKahouID < GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {
			// ­¡‘‰Áƒ^ƒCƒvF0 ’ƒ˜q  1 ’ƒšâ  2 ’ƒ“ü  3 ’ƒŠ˜  4 ‰Ô“ü  
			if ( 0 <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 4 ) {
				// ‚¿å‚Æw’è•«‚ªˆê’v‚µ‚½
				if (nb6kahou[iKahouID].attach == iBushouID + 1) {
					// ¡‚Ü‚Å‚ÅÅ‘å‚Ìã¸’l
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

// ‰Æ•ó‚È‚Ç‚ğŠÜ‚ß‚È‚¢‘f‚Ìí“¬Ë”\
int getBushouPureBatMax(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int upparam = 0; // ã¸’l
		for (int iKahouID =0; iKahouID < GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {
			// í“¬‘‰Áƒ^ƒCƒvF5 “Œ•  6 ‘„  7 ‹ï‘«  8 ”n  9 ˆÆ  A w‰HD   
			if ( 5 <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 0xA ) {
				// ‚¿å‚Æw’è•«‚ªˆê’v‚µ‚½
				if (nb6kahou[iKahouID].attach == iBushouID + 1) {
					// ¡‚Ü‚Å‚ÅÅ‘å‚Ìã¸’n
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

// ‰Æ•ó‚È‚Ç‚ğŠÜ‚ß‚È‚¢‘f‚Ì’q–dË”\
int getBushouPureIntMax(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int upparam = 0; // ã¸’l
		for (int iKahouID =0; iKahouID < GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {
			// ’q–d‘‰Áƒ^ƒCƒvFB Šªq–{  C –¾’©’Ô–{
			if ( 0xB <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 0xC ) {
				// ‚¿å‚Æw’è•«‚ªˆê’v‚µ‚½
				if (nb6kahou[iKahouID].attach == iBushouID + 1) {
					// ¡‚Ü‚Å‚ÅÅ‘å‚Ìã¸’n
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

// ‰Æ•ó‚È‚Ç‚ğŠÜ‚ß‚È‚¢‘f‚Ì–ì–]
int getBushouPureAmbition(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int upparam = 0; // ã¸’l
		for (int iKahouID =0; iKahouID < GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {
			// –ì–]‘‰Áƒ^ƒCƒvFD –Ø  E ’n‹…‹V  F “ì”ØŒv  10 …–n‰æ  11 \š‰Ë  12 ¹‘  13 ‹Êè”   14 ƒr[ƒhƒ”t  15 ‰“Šá‹¾  16 ƒ‹ƒ\ƒ“šâ  17 ‰Œ‘
			if ( 0xD <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 0x17 ) {
				// ‚¿å‚Æw’è•«‚ªˆê’v‚µ‚½
				if (nb6kahou[iKahouID].attach == iBushouID + 1) {
					// ¡‚Ü‚Å‚ÅÅ‘å‚Ìã¸’n
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

// Š¯ˆÊ‚È‚Ç‚ğŠÜ‚ß‚È‚¢‘f‚Ì–£—Í
int getBushouPureCharm(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int upparam = 0; // ã¸’l
		for (int iKaniID =0; iKaniID < GAMEDATASTRUCT_KANI_NUM; iKaniID++) {
			// ‚¿å‚Æw’è•«‚ªˆê’v‚µ‚½
			if (nb6kani[iKaniID].attach == iBushouID + 1) {
				// ¡‚Ü‚Å‚ÅÅ‘å‚Ìã¸’n
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



// ‰Æ•ó‚É‚æ‚é­¡‚Ìã‚°•
int getBushouGovMaxUpByKahou(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int upparam = 0; // ã¸’l
		for (int iKahouID =0; iKahouID < GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {
			// ­¡‘‰Áƒ^ƒCƒvF0 ’ƒ˜q  1 ’ƒšâ  2 ’ƒ“ü  3 ’ƒŠ˜  4 ‰Ô“ü  
			if ( 0 <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 4 ) {
				// ‚¿å‚Æw’è•«‚ªˆê’v‚µ‚½
				if (nb6kahou[iKahouID].attach == iBushouID + 1) {
					// ¡‚Ü‚Å‚ÅÅ‘å‚Ìã¸’l
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

// ‰Æ•ó‚É‚æ‚éí“¬‚Ìã‚°•
int getBushouBatMaxUpByKahou(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int upparam = 0; // ã¸’l
		for (int iKahouID =0; iKahouID < GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {
			// í“¬‘‰Áƒ^ƒCƒvF5 “Œ•  6 ‘„  7 ‹ï‘«  8 ”n  9 ˆÆ  A w‰HD   
			if ( 5 <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 0xA ) {
				// ‚¿å‚Æw’è•«‚ªˆê’v‚µ‚½
				if (nb6kahou[iKahouID].attach == iBushouID + 1) {
					// ¡‚Ü‚Å‚ÅÅ‘å‚Ìã¸’n
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

// ‰Æ•ó‚É‚æ‚é’q–d‚Ìã‚°•
int getBushouIntMaxUpByKahou(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int upparam = 0; // ã¸’l
		for (int iKahouID =0; iKahouID < GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {
			// ’q–d‘‰Áƒ^ƒCƒvFB Šªq–{  C –¾’©’Ô–{
			if ( 0xB <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 0xC ) {
				// ‚¿å‚Æw’è•«‚ªˆê’v‚µ‚½
				if (nb6kahou[iKahouID].attach == iBushouID + 1) {
					// ¡‚Ü‚Å‚ÅÅ‘å‚Ìã¸’n
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

// ‰Æ•ó‚É‚æ‚é–ì–]‚Ìã‚°•
int getBushouAmbitionUpByKahou(int iBushouID) {
		if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int upparam = 0; // ã¸’l
		for (int iKahouID =0; iKahouID < GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {
			// –ì–]‘‰Áƒ^ƒCƒvFD –Ø  E ’n‹…‹V  F “ì”ØŒv  10 …–n‰æ  11 \š‰Ë  12 ¹‘  13 ‹Êè”   14 ƒr[ƒhƒ”t  15 ‰“Šá‹¾  16 ƒ‹ƒ\ƒ“šâ  17 ‰Œ‘
			if ( 0xD <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 0x17 ) {
				// ‚¿å‚Æw’è•«‚ªˆê’v‚µ‚½
				if (nb6kahou[iKahouID].attach == iBushouID + 1) {
					// ¡‚Ü‚Å‚ÅÅ‘å‚Ìã¸’n
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


// Š¯ˆÊ‚É‚æ‚é–£—Í‚Ìã‚°•
int getBushouCharmUpByKani(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int upparam = 0; // ã¸’l
		for (int iKaniID =0; iKaniID < GAMEDATASTRUCT_KANI_NUM; iKaniID++) {
			// ‚¿å‚Æw’è•«‚ªˆê’v‚µ‚½
			if (nb6kani[iKaniID].attach == iBushouID + 1) {
				// ¡‚Ü‚Å‚ÅÅ‘å‚Ìã¸’n
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


// Š¯ˆÊ‚ğ•«‚ÉƒAƒ^ƒbƒ`‚·‚éBŒ³X•Ê•«‚ª‚Á‚Ä‚¢‚½ê‡‚ÍAŒ³‚Ì•«‚©‚çw’è‚µ‚½•«‚Ö‚ÆƒAƒ^ƒbƒ`‚ª•Ï‚í‚éB
// ”\—Í’l‚Ì•ÏX‚ğ”º‚¤B’©’ì‚É–ß‚·ê‡‚ÍA0xFFFF‚ğw’è‚·‚é‚±‚ÆB
bool ReAttachKani(int iKaniID, int iNewBushouID) {
	if ( !(0 <= iKaniID && iKaniID < GAMEDATASTRUCT_KANI_NUM) ) {
		return false;
	}

	int iOldBushouID = nb6kani[iKaniID].attach - 1;
	// –{l‚¾B‰½‚à‚·‚é•K—v‚ª‚È‚¢B
	if ( iOldBushouID == iNewBushouID ) {
		return true;
	}
	// Œ³X’©’ì‚ª‚Á‚Ä‚¢‚ÄAV‚½‚Èw’è‚à’©’ì‚¾B‰½‚à‚·‚é•K—v‚ª‚È‚¢B
	if ( iOldBushouID >= 0xFFFE && iNewBushouID >= 0xFFFE )  {
		return true;
	}


	// Œ³‚Ì•«‚Ì­¡”\—Í‚Ì‘f’l
	int iOldBushouPureCharm = 0;
	// V‚µ‚¢w’è‚Ì•«‚Í³‹K‚Ì•«‚Å‚ ‚èA’©’ì‚Å‚Í‚È‚¢B
	if ( 0 <= iOldBushouID && iOldBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		iOldBushouPureCharm = getBushouPureCharm(iOldBushouID);
	// •s³‚È”Ô†‚Í’©’ì‚Æ‚¢‚¤‚±‚Æ‚É‚·‚éB
	} else {
		iOldBushouID = 0xFFFF;
	}

	int iNewBushouPureCharm = 0;
	// V‚µ‚¢w’è‚Ì•«‚Í³‹K‚Ì•«‚Å‚ ‚èA’©’ì‚Å‚Í‚È‚¢B
	if ( 0 <= iNewBushouID && iNewBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		iNewBushouPureCharm = getBushouPureCharm(iNewBushouID);

	// •s³‚È”Ô†‚Í’©’ì‚Æ‚¢‚¤‚±‚Æ‚É‚·‚éB
	} else {
		iNewBushouID = 0xFFFF;
	}

	// ‘ÎÛ‚Ì‰Æ•ó‚ÌŠÒ‚ğ•ÏX‚·‚éB
	nb6kani[iKaniID].attach = (iNewBushouID == 0xFFFF) ? 0xFFFF : (iNewBushouID + 1);

	// Œ³‚Ì•«‚Í³‹K‚Ì•«‚Å‚ ‚èA’©’ì‚Å‚Í‚È‚¢B
	if ( 0 <= iOldBushouID && iOldBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int iOldBushouCharmUpByKani = getBushouCharmUpByKani(iOldBushouID);
		nb6bushouref[iOldBushouID].charm = iOldBushouPureCharm + iOldBushouCharmUpByKani;
	}

	// V‚µ‚¢w’è‚Ì•«‚Í³‹K‚Ì•«‚Å‚ ‚èA’©’ì‚Å‚Í‚È‚¢B
	if ( 0 <= iNewBushouID && iNewBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int iNewBushouCharmUpByKani = getBushouCharmUpByKani(iNewBushouID);
		nb6bushouref[iNewBushouID].charm = iNewBushouPureCharm + iNewBushouCharmUpByKani;
	}

	return true;
}


// ‰Æ•ó‚ğ•«‚ÉƒAƒ^ƒbƒ`‚·‚éBŒ³X•Ê•«‚ª‚Á‚Ä‚¢‚½ê‡‚ÍAŒ³‚Ì•«‚©‚çw’è‚µ‚½•«‚Ö‚ÆƒAƒ^ƒbƒ`‚ª•Ï‚í‚éB
// ”\—Í’l‚Ì•ÏX‚ğ”º‚¤B¤l‚É–ß‚·ê‡‚ÍA0xFFFF‚ğw’è‚·‚é‚±‚ÆB
bool ReAttachKahou(int iKahouID, int iNewBushouID) {
	if ( !(0 <= iKahouID && iKahouID < GAMEDATASTRUCT_KAHOU_NUM) ) {
		return false;
	}

	// Á¸‚Í~‚ë‚·
	nb6kahou[iKahouID].bLost = false;
	// –¢“oê‚Í~‚ë‚·B
	nb6kahou[iKahouID].bMitojo = false;

	int iOldBushouID = nb6kahou[iKahouID].attach - 1;
	// –{l‚¾B‰½‚à‚·‚é•K—v‚ª‚È‚¢B
	if ( iOldBushouID == iNewBushouID ) {
		return true;
	}
	// Œ³X¤l‚ª‚Á‚Ä‚¢‚ÄAV‚½‚Èw’è‚à¤l‚¾B‰½‚à‚·‚é•K—v‚ª‚È‚¢B
	if ( iOldBushouID >= 0xFFFE && iNewBushouID >= 0xFFFE )  {
		return true;
	}

	// ­¡‘‰Áƒ^ƒCƒvF0 ’ƒ˜q  1 ’ƒšâ  2 ’ƒ“ü  3 ’ƒŠ˜  4 ‰Ô“ü  
	if ( 0 <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 4 ) {

		// Œ³‚Ì•«‚Ì­¡”\—Í‚Ì‘f’l
		int iOldBushouPureGovMax = 0;
		// V‚µ‚¢w’è‚Ì•«‚Í³‹K‚Ì•«‚Å‚ ‚èA¤l‚Å‚Í‚È‚¢B
		if ( 0 <= iOldBushouID && iOldBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			iOldBushouPureGovMax = getBushouPureGovMax(iOldBushouID);
		// •s³‚È”Ô†‚Í¤l‚Æ‚¢‚¤‚±‚Æ‚É‚·‚éB
		} else {
			iOldBushouID = 0xFFFF;
		}

		int iNewBushouPureGovMax = 0;
		// V‚µ‚¢w’è‚Ì•«‚Í³‹K‚Ì•«‚Å‚ ‚èA¤l‚Å‚Í‚È‚¢B
		if ( 0 <= iNewBushouID && iNewBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			iNewBushouPureGovMax = getBushouPureGovMax(iNewBushouID);

		// •s³‚È”Ô†‚Í¤l‚Æ‚¢‚¤‚±‚Æ‚É‚·‚éB
		} else {
			iNewBushouID = 0xFFFF;
		}

		// ‘ÎÛ‚Ì‰Æ•ó‚ÌŠÒ‚ğ•ÏX‚·‚éB
		nb6kahou[iKahouID].attach = (iNewBushouID == 0xFFFF) ? 0xFFFF : (iNewBushouID + 1);

		// V‚µ‚¢w’è‚Ì•«‚Í³‹K‚Ì•«‚Å‚ ‚èA¤l‚Å‚Í‚È‚¢B
		if ( 0 <= iOldBushouID && iOldBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			int iOldBushouGovMaxUpByKahou = getBushouGovMaxUpByKahou(iOldBushouID);
			nb6bushouref[iOldBushouID].maxgov = iOldBushouPureGovMax + iOldBushouGovMaxUpByKahou;
		}

		// V‚µ‚¢w’è‚Ì•«‚Í³‹K‚Ì•«‚Å‚ ‚èA¤l‚Å‚Í‚È‚¢B
		if ( 0 <= iNewBushouID && iNewBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			int iNewBushouGovMaxUpByKahou = getBushouGovMaxUpByKahou(iNewBushouID);
			nb6bushouref[iNewBushouID].maxgov = iNewBushouPureGovMax + iNewBushouGovMaxUpByKahou;
		}

	// í“¬‘‰Áƒ^ƒCƒvF5 “Œ•  6 ‘„  7 ‹ï‘«  8 ”n  9 ˆÆ  A w‰HD   
	} else if ( 5 <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 0xA ) {

		// Œ³‚Ì•«‚Ì­¡”\—Í‚Ì‘f’l
		int iOldBushouPureBatMax = 0;
		// V‚µ‚¢w’è‚Ì•«‚Í³‹K‚Ì•«‚Å‚ ‚èA¤l‚Å‚Í‚È‚¢B
		if ( 0 <= iOldBushouID && iOldBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			iOldBushouPureBatMax = getBushouPureBatMax(iOldBushouID);
		// •s³‚È”Ô†‚Í¤l‚Æ‚¢‚¤‚±‚Æ‚É‚·‚éB
		} else {
			iOldBushouID = 0xFFFF;
		}

		int iNewBushouPureBatMax = 0;
		// V‚µ‚¢w’è‚Ì•«‚Í³‹K‚Ì•«‚Å‚ ‚èA¤l‚Å‚Í‚È‚¢B
		if ( 0 <= iNewBushouID && iNewBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			iNewBushouPureBatMax = getBushouPureBatMax(iNewBushouID);

		// •s³‚È”Ô†‚Í¤l‚Æ‚¢‚¤‚±‚Æ‚É‚·‚éB
		} else {
			iNewBushouID = 0xFFFF;
		}

		// ‘ÎÛ‚Ì‰Æ•ó‚ÌŠÒ‚ğ•ÏX‚·‚éB
		nb6kahou[iKahouID].attach = (iNewBushouID == 0xFFFF) ? 0xFFFF : (iNewBushouID + 1);

		// V‚µ‚¢w’è‚Ì•«‚Í³‹K‚Ì•«‚Å‚ ‚èA¤l‚Å‚Í‚È‚¢B
		if ( 0 <= iOldBushouID && iOldBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			int iOldBushouBatMaxUpByKahou = getBushouBatMaxUpByKahou(iOldBushouID);
			nb6bushouref[iOldBushouID].maxbat = iOldBushouPureBatMax + iOldBushouBatMaxUpByKahou;
		}

		// V‚µ‚¢w’è‚Ì•«‚Í³‹K‚Ì•«‚Å‚ ‚èA¤l‚Å‚Í‚È‚¢B
		if ( 0 <= iNewBushouID && iNewBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			int iNewBushouBatMaxUpByKahou = getBushouBatMaxUpByKahou(iNewBushouID);
			nb6bushouref[iNewBushouID].maxbat = iNewBushouPureBatMax + iNewBushouBatMaxUpByKahou;
		}


	// ’q–d‘‰Áƒ^ƒCƒvFB Šªq–{  C –¾’©’Ô–{
	} else if ( 0xB <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 0xC ) {

		// Œ³‚Ì•«‚Ì­¡”\—Í‚Ì‘f’l
		int iOldBushouPureIntMax = 0;
		// V‚µ‚¢w’è‚Ì•«‚Í³‹K‚Ì•«‚Å‚ ‚èA¤l‚Å‚Í‚È‚¢B
		if ( 0 <= iOldBushouID && iOldBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			iOldBushouPureIntMax = getBushouPureIntMax(iOldBushouID);
		// •s³‚È”Ô†‚Í¤l‚Æ‚¢‚¤‚±‚Æ‚É‚·‚éB
		} else {
			iOldBushouID = 0xFFFF;
		}

		int iNewBushouPureIntMax = 0;
		// V‚µ‚¢w’è‚Ì•«‚Í³‹K‚Ì•«‚Å‚ ‚èA¤l‚Å‚Í‚È‚¢B
		if ( 0 <= iNewBushouID && iNewBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			iNewBushouPureIntMax = getBushouPureIntMax(iNewBushouID);

		// •s³‚È”Ô†‚Í¤l‚Æ‚¢‚¤‚±‚Æ‚É‚·‚éB
		} else {
			iNewBushouID = 0xFFFF;
		}

		// ‘ÎÛ‚Ì‰Æ•ó‚ÌŠÒ‚ğ•ÏX‚·‚éB
		nb6kahou[iKahouID].attach = (iNewBushouID == 0xFFFF) ? 0xFFFF : (iNewBushouID + 1);

		// V‚µ‚¢w’è‚Ì•«‚Í³‹K‚Ì•«‚Å‚ ‚èA¤l‚Å‚Í‚È‚¢B
		if ( 0 <= iOldBushouID && iOldBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			int iOldBushouIntMaxUpByKahou = getBushouIntMaxUpByKahou(iOldBushouID);
			nb6bushouref[iOldBushouID].maxint = iOldBushouPureIntMax + iOldBushouIntMaxUpByKahou;
		}

		// V‚µ‚¢w’è‚Ì•«‚Í³‹K‚Ì•«‚Å‚ ‚èA¤l‚Å‚Í‚È‚¢B
		if ( 0 <= iNewBushouID && iNewBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			int iNewBushouIntMaxUpByKahou = getBushouIntMaxUpByKahou(iNewBushouID);
			nb6bushouref[iNewBushouID].maxint = iNewBushouPureIntMax + iNewBushouIntMaxUpByKahou;
		}

	// –ì–]‘‰Áƒ^ƒCƒvFD –Ø  E ’n‹…‹V  F “ì”ØŒv  10 …–n‰æ  11 \š‰Ë  12 ¹‘  13 ‹Êè”   14 ƒr[ƒhƒ”t  15 ‰“Šá‹¾  16 ƒ‹ƒ\ƒ“šâ  17 ‰Œ‘
	} else if  ( 0xD <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 0x17 ) {

		// Œ³‚Ì•«‚Ì­¡”\—Í‚Ì‘f’l
		int iOldBushouPureAmbition = 0;
		// V‚µ‚¢w’è‚Ì•«‚Í³‹K‚Ì•«‚Å‚ ‚èA¤l‚Å‚Í‚È‚¢B
		if ( 0 <= iOldBushouID && iOldBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			iOldBushouPureAmbition = getBushouPureAmbition(iOldBushouID);
		// •s³‚È”Ô†‚Í¤l‚Æ‚¢‚¤‚±‚Æ‚É‚·‚éB
		} else {
			iOldBushouID = 0xFFFF;
		}

		int iNewBushouPureAmbition = 0;
		// V‚µ‚¢w’è‚Ì•«‚Í³‹K‚Ì•«‚Å‚ ‚èA¤l‚Å‚Í‚È‚¢B
		if ( 0 <= iNewBushouID && iNewBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			iNewBushouPureAmbition = getBushouPureAmbition(iNewBushouID);

		// •s³‚È”Ô†‚Í¤l‚Æ‚¢‚¤‚±‚Æ‚É‚·‚éB
		} else {
			iNewBushouID = 0xFFFF;
		}

		// ‘ÎÛ‚Ì‰Æ•ó‚ÌŠÒ‚ğ•ÏX‚·‚éB
		nb6kahou[iKahouID].attach = (iNewBushouID == 0xFFFF) ? 0xFFFF : (iNewBushouID + 1);

		// V‚µ‚¢w’è‚Ì•«‚Í³‹K‚Ì•«‚Å‚ ‚èA¤l‚Å‚Í‚È‚¢B
		if ( 0 <= iOldBushouID && iOldBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			int iOldBushouAmbitionUpByKahou = getBushouAmbitionUpByKahou(iOldBushouID);
			nb6bushouref[iOldBushouID].ambition = iOldBushouPureAmbition + iOldBushouAmbitionUpByKahou;
		}

		// V‚µ‚¢w’è‚Ì•«‚Í³‹K‚Ì•«‚Å‚ ‚èA¤l‚Å‚Í‚È‚¢B
		if ( 0 <= iNewBushouID && iNewBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			int iNewBushouAmbitionUpByKahou = getBushouAmbitionUpByKahou(iNewBushouID);
			nb6bushouref[iNewBushouID].ambition = iNewBushouPureAmbition + iNewBushouAmbitionUpByKahou;
		}

	}

	return true;
}




// ŠçID‚©‚ç•«ID‚ğ‹‚ß‚éB¸”s‚Í0xFFFF
int getBushouIDFromFaceID(int iFaceID) {
	for ( int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {
		if ( nb6bushouname[iBushouID].number == iFaceID ) {
			return iBushouID;
		}
	}
	return 0xFFFF;
}


// w’è•«‚Ì”º—µ‚ÌiBushouID‚ğ“¾‚éB”º—µ‚ª‹‚È‚¢ê‡‚É‚ÍA0xFFFF‚ª•Ô‚Á‚Ä‚­‚éB
int getHanryoBushouID(int iBushouID) {

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// w’è‚Ì•«‚Ì”º—µî•ñ
		int iBushouIDSpouse = nb6bushouref[iBushouID].spouse; //

		// Œ‹¥‚µ‚Ä‚¢‚È‚¢
		if ( iBushouIDSpouse == 0xFF ) {
			return 0xFFFF;
		}

		int iTsumaBushouID = iBushouIDSpouse+500;

		// ”º—µ‚Ì”Ô†‚ª©•ª©g‚¾‚Á‚½‚çA©•ª‚ÍÈ‘¤‚Ì—§ê
		if ( iTsumaBushouID == iBushouID ) {
			for ( int i=0; i<GAMEDATASTRUCT_BUSHOU_NUM; i++ ) {
				// ©•ª©gˆÈŠO‚ÅA“¯‚¶”º—µ”Ô†‚ğ‚Á‚Ä‚é‚Ì‚ªA”º—µ‚Ì•«
				if ( i != iBushouID &&  nb6bushouref[i].spouse == iBushouIDSpouse ) {
					return i;
				}
			}
			return 0xFFFF;

		// ©•ª‚Í•v‘¤BÈ‚É‚à“¯‚¶”Ô†‚ªİ’è‚³‚ê‚Ä‚¢‚é‚©Šm”FB
		} else {
			if ( nb6bushouref[iTsumaBushouID].spouse == iBushouIDSpouse ) {
				return iTsumaBushouID;
			// ‚È‚º‚©İ’è‚³‚ê‚Ä‚¢‚È‚¢cB‰ü‘¢“™‚Åƒf[ƒ^‰ó‚ê‚Ä‚¢‚éB
			} else {
				return 0xFFFF;
			}
		}
	} else {
		return 0xFFFF;
	}
}

// •P(‘¼‚Ì•«‚ÌÈŠÜ‚Ş)‚ğAw’è‚Ì•«‚ÆŒ‹¥‚³‚¹‚éA“¯–¿ŠÖŒW“™‚ÍˆêØ‘€ì‚³‚ê‚È‚¢B
bool setHimeMarriage( int iHimeID, int iOttoID, BOOL withKodokuDeath ) {

	// “¯ˆêl•¨BƒAƒEƒgB
	if ( iHimeID == iOttoID ) { return false; }

	// •v‚Æ•P‚ª‹K’è‚Ì”ÍˆÍ‚É“ü‚Á‚Ä‚¢‚È‚¯‚ê‚ÎƒAƒEƒg
	if (   0 <= iOttoID && iOttoID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 500 <= iHimeID && iHimeID < GAMEDATASTRUCT_BUSHOU_NUM) {

		// ‚à‚µ•P‘¤‚Ì‚h‚c‚Ì•«‚ªA•P‚Å‚È‚¢‚È‚ç‚ÎAƒAƒEƒg
		 if ( nb6bushouname[iHimeID].State != 5 ) { // 
			return false;
		}

		int _new_spouse = iHimeID-500; // È‚É“ü‚é‚×‚«’l‚ÍA•P•«”Ô†y”z—ñ—pz-500

		// ‚·‚Å‚É—¼Ò‚Í•v•w‚Å‚ ‚éA‰½‚à‚·‚é•K—v‚ª–³‚¢B
		if ( nb6bushouref[iOttoID].spouse == _new_spouse &&  nb6bushouref[iHimeID].spouse == _new_spouse ) {
			return true;
		}

		// •v‘¤‚ª‚·‚Å‚ÉŒ‹¥‚µ‚Ä‚¢‚é‚©AŒ‹¥‚µ‚Ä‚¢‚½‚ç—£¥
		int _otto_old_spouse = nb6bushouref[iOttoID].spouse; //
		// —£¥
		setHimeDivorce( _otto_old_spouse+500, withKodokuDeath );

		nb6bushouname[iHimeID].State = 5; // “VŠWŒÇ“Æˆ—‚Å€–S‚µ‚½‚©‚à‚µ‚ê‚È‚¢‚Ì‚Å•P‚É–ß‚·B

		int _tuma_old_spouse = nb6bushouref[iHimeID].spouse; //
		setHimeDivorce( _tuma_old_spouse+500, withKodokuDeath );

		nb6bushouname[iHimeID].State = 5; // “VŠWŒÇ“Æˆ—‚Å€–S‚µ‚½‚©‚à‚µ‚ê‚È‚¢‚Ì‚Å•P‚É–ß‚·B

		// g‚ªãY—í‚É‚È‚Á‚½‚Ì‚Å‰ü‚ß‚ÄŒ‹¥
		nb6bushouref[iOttoID].spouse = _new_spouse;
		nb6bushouref[iHimeID].spouse = _new_spouse;

		// ‹Œ‘å–¼‰Æ‚ğŠo‚¦‚Ä‚¨‚¢‚Ä
		int OldDaimyoID = nb6bushouref[iHimeID].attach;

		// •v‘¤‚Ì‘å–¼‚Ì•û‚Ég‚ğŠñ‚¹‚é
		nb6bushouref[iHimeID].attach = nb6bushouref[iOttoID].attach;

		// –¼š‚ğ•v‚Ì‚à‚Ì‚Åã‘‚«‚·‚éB
		strcpy( nb6bushouname[iHimeID].familyname, nb6bushouname[iOttoID].familyname );

		// Œ‹‰ÊA‘å–¼‰Æ‚ª•Ï‰»‚µ‚½‚Ì‚Å‚ ‚ê‚ÎAmŠ¯”N”‚àƒŠƒZƒbƒg
		if ( OldDaimyoID != nb6bushouref[iHimeID].attach) {
			nb6bushouref[iHimeID].work = 0;
		}

		return true;
	}

	return false;
}


// w’è‚Ì•P‚ğ—£¥‚³‚¹‚éB
bool setHimeDivorce( int iHimeID, BOOL withKodokuDeath ) {

	// •v‚Æ•P‚ª‹K’è‚Ì”ÍˆÍ‚É“ü‚Á‚Ä‚¢‚È‚¯‚ê‚ÎƒAƒEƒg
	if ( 500 <= iHimeID && iHimeID < GAMEDATASTRUCT_BUSHOU_NUM ) {


		// ‚à‚µ•P‘¤‚Ì‚h‚c‚Ì•«‚ªA•P‚Å‚È‚¢‚È‚ç‚ÎAƒAƒEƒg
		if ( nb6bushouname[iHimeID].State != 5 ) {
			return false;
		}

		int _tuma_spouse = nb6bushouref[iHimeID].spouse; //

		// Œ³XŒ‹¥‚µ‚Ä‚¢‚È‚¢B
		if ( _tuma_spouse == 0xFF ) {
			return false;
		}

		// —¼Ò‚ÌŒ‹¥î•ñ‚ğƒNƒŠƒA
		for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {
			// •v‚à‚µ‚­‚ÍÈ‚Å‚ ‚é
			if ( nb6bushouref[iBushouID].spouse == _tuma_spouse ) {
				// —£¥
				nb6bushouref[iBushouID].spouse = 0xFF;
				// •v‚ÆÈ2l—¼•û‚Æ‚à’l‚ğ0xFF‚É‚·‚é•K—v‚ª‚ ‚é‚Ì‚ÅAbreak‚µ‚È‚¢‚±‚ÆB
				// ‚Â‚¢‚Å‚Éƒf[ƒ^‚ÉƒSƒ~‚ª“ü‚Á‚Ä‚¢‚éê‡‚Å‚àƒNƒŠƒA
			}
		}


		// ‹Œ‘å–¼‰Æ‚ğŠo‚¦‚Ä‚¨‚¢‚Ä
		int OldDaimyoID = nb6bushouref[iHimeID].attach;

		// ˆÈ‰º‚ÌgŒ³ˆøólŒn‚Ìˆ—‚ÍAƒvƒƒOƒ‰ƒ€‘¤‚Åo—ˆ‚é‚¾‚¯‚â‚Á‚Ä‚¨‚±‚¤‚Æ‚¢‚¤ï|B
		// ‰¼‚É‚±‚±‚Å“KØ‚È‘å–¼‚É‹A‘®“]ˆÚ‚Å‚«‚È‚©‚Á‚½‚Æ‚µ‚Ä‚àA“VãÄ‹L‚ÌƒvƒƒOƒ‰ƒ€–{‘Ì‚ªŸ‹Gß‚É‚ÍA“KØ‚È‘å–¼‚É‹A‘®‚³‚¹‚éˆ—‚ğs‚¤B

		// —£¥‚µ‚½‚Ì‚ÅŠ‘®‘å–¼‚ğ•ÏX‚·‚é•K—v«‚ª‚ ‚éB‚Ü‚¸Ae‚ª‘å–¼‚ğ‚µ‚Ä‚¢‚é‚©‚Ç‚¤‚©B
		for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

			// ‘å–¼‚Ì‚¤‚¿c
			if ( nb6bushouname[iBushouID].State == 0 ) { // ‘å–¼

				// ©•ª‚Ìe‚ª‘å–¼‚â‚Á‚Ä‚éB
				if ( nb6bushouref[iHimeID].parent < 5000 ) {
					// ‚»‚Ì‘å–¼‚Í•P‚Ìe
					if ( nb6bushouname[iBushouID].number == nb6bushouref[iHimeID].parent ) {

						// ‚»‚±‚Ég‚ğŠñ‚¹‚é
						nb6bushouref[iHimeID].attach = nb6bushouref[iBushouID].attach;

						strcpy( nb6bushouname[iHimeID].familyname, nb6bushouname[iBushouID].familyname );
						break;
					}
				}
			}
		}


		// e‚ª‘å–¼‚ğ‚â‚Á‚Ä‚¢‚È‚­‚Æ‚àAe‚ª“¯‚¶l(‘¦‚¿ŒZ’ío–…)‚ª‘å–¼‚ğ‚â‚Á‚Ä‚¢‚é‚©‚Ç‚¤‚©B
		for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

			// ‘å–¼‚Ì‚¤‚¿c
			if ( nb6bushouname[iBushouID].State == 0 ) { // ‘å–¼

				// e‚ª“¯‚¶‚¾B
				if ( nb6bushouref[iHimeID].parent < 5000 ) {
					if ( nb6bushouref[iBushouID].parent == nb6bushouref[iHimeID].parent ) {

						// ‚»‚±‚Ég‚ğŠñ‚¹‚é
						nb6bushouref[iHimeID].attach = nb6bushouref[iBushouID].attach;

						strcpy( nb6bushouname[iHimeID].familyname, nb6bushouname[iBushouID].familyname );
						break;
					}
				}
			}
		}

		// eŒZ’í‚ª‘å–¼‚â‚Á‚Ä‚¢‚È‚­‚Ä‚àAˆê–å‚ª‘å–¼‚ğ‚â‚Á‚Ä‚¢‚é‚Ì‚Å‚Í‚È‚¢‚©B
		for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

			// ‘å–¼‚Ì‚¤‚¿c
			if ( nb6bushouname[iBushouID].State == 0 ) { // ‘å–¼

				// e‚ª“¯‚¶‚¾B
				if ( nb6bushouref[iHimeID].blood != 0xFF ) {
					if ( nb6bushouref[iBushouID].blood == nb6bushouref[iHimeID].blood ) {

						// ‚»‚±‚Ég‚ğŠñ‚¹‚é
						nb6bushouref[iHimeID].attach = nb6bushouref[iBushouID].attach;

						strcpy( nb6bushouname[iHimeID].familyname, nb6bushouname[iBushouID].familyname );
						break;
					}
				}
			}
		}

		bool isTengaiKodoku = true;
		// ‘å–¼‚Å‚È‚­‚Æ‚àAe‚ª‚Ç‚±‚©‚É‚¢‚é‚Ì‚Å‚Í‚È‚¢‚©H
		for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

			// ©•ª‚Ìe
			if ( nb6bushouref[iHimeID].parent < 5000 ) {
				if ( nb6bushouname[iBushouID].number == nb6bushouref[iHimeID].parent ) {

					// ‚»‚±‚Ég‚ğŠñ‚¹‚é
					nb6bushouref[iHimeID].attach = nb6bushouref[iBushouID].attach;

					strcpy( nb6bushouname[iHimeID].familyname, nb6bushouname[iBushouID].familyname );

					// g‚æ‚è‚ª‹‚½‚Ì‚Å“VŠWŒÇ“Æ‚Å‚Í‚È‚¢
					isTengaiKodoku = false;
					break;
				}
			}
		}

		// ‘å–¼‚Å‚È‚­‚Æ‚àAe‚ª“¯‚¶l(‘¦‚¿ŒZ’ío–…)‚ª‚Ç‚±‚©‚É‚¢‚é‚Ì‚Å‚Í‚È‚¢‚©H
		for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

			// e‚ª“¯‚¶‚¾B
			if ( nb6bushouref[iHimeID].parent < 5000 ) {
				if ( nb6bushouref[iBushouID].parent == nb6bushouref[iHimeID].parent ) {

					// ‚»‚±‚Ég‚ğŠñ‚¹‚é
					nb6bushouref[iHimeID].attach = nb6bushouref[iBushouID].attach;

					strcpy( nb6bushouname[iHimeID].familyname, nb6bushouname[iBushouID].familyname );

					// g‚æ‚è‚ª‹‚½‚Ì‚Å“VŠWŒÇ“Æ‚Å‚Í‚È‚¢
					isTengaiKodoku = false;
					break;
				}
			}
		}


		// Œ‹‰ÊA‘å–¼‰Æ‚ª•Ï‰»‚µ‚½‚Ì‚Å‚ ‚ê‚ÎAmŠ¯”N”‚àƒŠƒZƒbƒg
		if ( OldDaimyoID != nb6bushouref[iHimeID].attach) {
			nb6bushouref[iHimeID].work = 0;
		}

		if ( withKodokuDeath ) {
			if ( isTengaiKodoku ) {
				nb6bushouname[iHimeID].State = 7; // “VŠWŒÇ“Æ‚É‚È‚Á‚½‚ç€–S‚Æ‚·‚éB
			}
		}

		return true;
	}

	return false;
}




// State Œ»–ğ ¨ State ˜Ql‚Ö
bool setGenekiToRounin(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		if ( nb6bushouname[iBushouID].State == 2 ) { // Œ»–ğ•«‚Ìê‡‚¾‚¯ˆ—‚ğ‚·‚éB

			// ŠY“–•«‚Ì‹é‚ª‚¿‚á‚ñ‚Æ‚µ‚½î•ñ‚ªŠi”[‚³‚ê‚Ä‚¢‚éB(ƒ†[ƒU[‰ü‘¢“™‚É‚æ‚Á‚Ä‰ó‚ê‚Ä‚¢‚È‚¢)
			int iCastleID = nb6bushouref[iBushouID].castle-1;
			if (0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM ) {
				if ( nb6castle[iCastleID].master == 0xFFFF ) { return false; } // ƒŠƒ“ƒNƒŠƒXƒg‚ª‰ó‚ê‚Ä‚¢‚é‚Ì‚©é‚É’N‚àŠ‘®‚µ‚Ä‚¢‚È‚¢B‰½‚à‚µ‚È‚¢Breturn;

				// ƒƒ‚ƒŠã‚Ì’l•ÏX
				{
					nb6bushouname[iBushouID].State = 4; // ó‘Ô‚ğ˜Ql‚Æ‚·‚éB
						
					nb6bushouname[iBushouID].position = 6; // g•ª‚ğ‘«Œy‚Æ‚·‚éB

					nb6bushouref[iBushouID].attach = 0xFFFF; // ‘å–¼‚ÉŠ‘®‚µ‚È‚¢B
					nb6bushouref[iBushouID].gundan = 0xFFFF; // ŒR’c‚ÉŠ‘®‚µ‚È‚¢B
					nb6bushouref[iBushouID].loyal = 50; // ’‰½‚ğ50‚Æ‚·‚éB
					nb6bushouref[iBushouID].work = 0; // dŠ¯”N”‚ğ0‚Æ‚·‚éB

					nb6bushouref[iBushouID].rise = 0xFFFF; // “à‰æ‚ğ‚È‚µ‚Æ‚·‚éB
					nb6bushouref[iBushouID].bRise = 0; // “à‰‚È‚µB

					nb6bushouref[iBushouID].soldier = 0; // •º”‚ğ0‚Æ‚·‚éB
					nb6bushouref[iBushouID].training = 0; // ŒP—û‚ğ0‚Æ‚·‚éB
					nb6bushouref[iBushouID].solloyal = 0; // •º’‰‚ğ0‚Æ‚·‚éB
					nb6bushouref[iBushouID].form = 0; // ‘«Œy‘à‚Æ‚·‚éB

					// ŒMŒ÷ƒŠƒZƒbƒg‚µ‚È‚¢
					if ( TSModCommand::AllBushou::cmdNonMeritoriousReset ) {
						int merits = nb6bushouref[iBushouID].merits; // ŒMŒ÷’l‚ğ•œŒ³B0.7”{
						nb6bushouref[iBushouID].merits = (WORD)(0.7 * merits);
					} else {
						nb6bushouref[iBushouID].merits = 0; // ŒMŒ÷‚ğ0‚Æ‚·‚éB
					}
				}

				/* @©•ª
				      «
				   ›-œ-›-›

				   ‚Æ‚¢‚¤‚Ì‚ğ

				   ›-›-›

				   ‚Æ‚·‚éB
				*/


				// ˜Ql‚É‚µ‚½‚¢•«‚ÍA‚¢‚Üéå‚¾
				if ( nb6castle[iCastleID].master-1 == iBushouID ) {
					nb6castle[iCastleID].master =  nb6bushouref[iBushouID].next; // ©•ª‚Ìnext‚ğéå‚É‚·‚éB
				}

				// ©•ª‚Ìprev‚Í—LŒø‚È•«‚¾B
				if ( 0 < nb6bushouref[iBushouID].prev && nb6bushouref[iBushouID].prev <= GAMEDATASTRUCT_BUSHOU_NUM ) {

					// ©•ª‚ªéå‚Å‚Í‚È‚¢•«‚ÅA
					// ‚P‚Â‘O‚Ìl‚ÌŸ‚ÌƒŠƒ“ƒNæ‚ª©•ª©g‚Ìê‡
					if ( nb6bushouref[iBushouID].prev != 0xFFFF && nb6bushouref[(nb6bushouref[iBushouID].prev-1)].next == iBushouID+1 ) {
						// ©•ª‚Å‚Í‚È‚­AŸ‚Ìl‚Ö‚ÆŒq‚¬‚©‚¦‚éB
						nb6bushouref[(nb6bushouref[iBushouID].prev-1)].next = nb6bushouref[iBushouID].next;
					}
				}

				// ©•ª‚Ìnext‚Í—LŒø‚È•«‚¾B
				if ( 0 < nb6bushouref[iBushouID].next && nb6bushouref[iBushouID].next <= GAMEDATASTRUCT_BUSHOU_NUM ) {
					// ‚P‚ÂŒã‚Ìl‚Ì‘O‚ÌƒŠƒ“ƒNæ‚ª©•ª©g‚Ìê‡
					if ( nb6bushouref[iBushouID].next != 0xFFFF && nb6bushouref[(nb6bushouref[iBushouID].next-1)].prev == iBushouID+1 ) {
						// ©•ª‚Å‚Í‚È‚­A‘O‚Ìl‚Ö‚ÆŒq‚¬‚©‚¦‚éB
						nb6bushouref[(nb6bushouref[iBushouID].next-1)].prev = nb6bushouref[iBushouID].prev;
					}
				}


				/* @
				     
				   ›-›

				   ‚Æ‚¢‚¤‚Ì‚ğ

				   œ-›-›

				   ‚Æ‚·‚éB
				*/

				// ˜Ql‚Æ‚µ‚Ä‚İ‚È‚µ‚½A©•ª‚Ìnext‚ÍAŒ»İ˜QlƒŠƒXƒg‚Ìæ“ª‚Ìl
				nb6bushouref[iBushouID].next = nb6castle[iCastleID].ronin; // Œ»İ‚Ìæ“ª‚Ìl‚ğ©•ª‚ÌŸ‚Ìl‚É

				// next‚ª—LŒø‚È•«‚È‚ç‚Îc
				if ( 0 < nb6castle[iCastleID].ronin && nb6castle[iCastleID].ronin <= GAMEDATASTRUCT_BUSHOU_NUM ) {
					// Ÿ‚Ìl‚Ìprev‚ğ©•ª‚É
					nb6bushouref[ nb6castle[iCastleID].ronin-1].prev = iBushouID+1;

				}
				nb6bushouref[iBushouID].prev = 0xFFFF; // prev‚Í‹‚È‚¢

				nb6castle[iCastleID].ronin = iBushouID+1; // é‚Ì˜Ql‚Ìæ“ª‚ğ©•ª‚É‚·‚éB

				return true;

			}
		}
	}

	return false;
}


// State ˜Ql ¨ State Œ»–ğ‚Ö
bool setRouninToGeneki(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		if ( nb6bushouname[iBushouID].State == 4 ) { // ˜Ql•«‚Ìê‡‚¾‚¯ˆ—‚ğ‚·‚éB
			// ŠY“–•«‚Ì‹é‚ª‚¿‚á‚ñ‚Æ‚µ‚½î•ñ‚ªŠi”[‚³‚ê‚Ä‚¢‚éB(ƒ†[ƒU[‰ü‘¢“™‚É‚æ‚Á‚Ä‰ó‚ê‚Ä‚¢‚È‚¢)
			int iCastleID = nb6bushouref[iBushouID].castle-1;
			if (0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM ) {

				if ( nb6castle[iCastleID].ronin == 0xFFFF ) { return false; } // ƒŠƒ“ƒNƒŠƒXƒg‚ª‰ó‚ê‚Ä‚¢‚é‚Ì‚©˜Ql‚ª’N‚à‹‚È‚¢‚±‚Æ‚É‚È‚Á‚¿‚á‚Á‚Ä‚éB‰½‚à‚µ‚È‚¢Breturn;
				if ( nb6castle[iCastleID].attach == 0xFFFF ) { return false; } // ŠY“–‚Ìé‚ª‚Ç‚±‚ÌŒR’c‚É‚àŠ‘®‚µ‚Ä‚¢‚È‚¢ê‡A˜Ql‚ğŒ»–ğ‚Æ‚Ío—ˆ‚È‚¢B
																		       // ‚È‚º‚È‚çA‚»‚ê‚Í‘å–¼‰»‚·‚é‚±‚Æ‚ğˆÓ–¡‚·‚é‚©‚çB
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

				// Œ»İ‚Ìé‚Ìéå‚©‚çAƒŠƒ“ƒNƒŠƒXƒg‚ğ’H‚Á‚Ä‚¢‚­
				int iLinkEndBusyouID = nb6castle[iCastleID].master;
				int iSafetyCnt = 0;
				while ( true ) {
					// ƒZ[ƒtƒeƒBƒJƒEƒ“ƒg‚ª–\‘–‚µ‚Ä‚¢‚éB
					if ( iSafetyCnt > GAMEDATASTRUCT_BUSHOU_NUM ) { 
						return false;
					}

					// éå‚ª‹‚È‚¢‚È‚ç‚ÎAI‚í‚è
					if ( iLinkEndBusyouID == 0xFFFF ) {
						break;
					}

					// Œ»İ‚Ì•«‚ÌAnext‚ª—LŒø‚Å‚Í‚È‚¯‚ê‚ÎAŒ»İ‚Ì•«‚ªÅŒã‚ÌlBI‚í‚èB
					if ( nb6bushouref[iLinkEndBusyouID-1].next == 0xFFFF ) {
						break;

					// Œ»İ‚Ìl‚ª—LŒø‚Å‚ ‚éB
					} else {
						// ƒŠƒ“ƒNƒŠƒXƒg‚ğŸ‚Ìl‚Æ‚·‚éB
						if ( 0 < iLinkEndBusyouID && iLinkEndBusyouID <= GAMEDATASTRUCT_BUSHOU_NUM ) {
							iLinkEndBusyouID = nb6bushouref[iLinkEndBusyouID-1].next;
						}
					}
				}

				// ƒƒ‚ƒŠã‚Ì’l•ÏX
				{
					nb6bushouname[iBushouID].State = 2; // ó‘Ô‚ğŒ»–ğ‚Æ‚·‚éB
						
					nb6bushouref[iBushouID].attach = iAttachDaimyoID+1; // ‘å–¼‚ÉŠ‘®‚·‚éB
					nb6bushouref[iBushouID].gundan = iAttachGundanID+1; // ŒR’c‚ÉŠ‘®‚·‚éB
					nb6bushouref[iBushouID].loyal = 50; // ’‰½‚ğ50‚Æ‚·‚éB

					nb6bushouref[iBushouID].rise = 0xFFFF; // “à‰æ‚ğ‚È‚µ‚Æ‚·‚éB
					nb6bushouref[iBushouID].bRise = 0; // “à‰‚È‚µB

					nb6bushouref[iBushouID].soldier = 0; // •º”‚ğ0‚Æ‚·‚éB
					nb6bushouref[iBushouID].training = 0; // ŒP—û‚ğ0‚Æ‚·‚éB
					nb6bushouref[iBushouID].solloyal = 0; // •º’‰‚ğ0‚Æ‚·‚éB
					nb6bushouref[iBushouID].form = 0; // ‘«Œy‘à‚Æ‚·‚éB

					//------------------------------------------------------
					// ŒMŒ÷ƒŠƒZƒbƒg‚µ‚È‚¢ƒ‚[ƒh‚¾‚ÆAc‚èŒMŒ÷‚É‡‚í‚¹‚Ä¸i
					if ( TSModCommand::AllBushou::cmdNonMeritoriousReset ) {

						if ( nb6bushouref[iBushouID].merits >= nb6requiredmerits[0].merits ) { // h˜V‹‰ŒMŒ÷‚ğ‚ÂB
							nb6bushouname[iBushouID].position = 2; // h˜V‚Ö
						} else if ( nb6bushouref[iBushouID].merits >= nb6requiredmerits[1].merits ) { // ‰Æ˜V‹‰ŒMŒ÷‚ğ‚ÂB
							nb6bushouname[iBushouID].position = 3; // ‰Æ˜V‚Ö
						} else if ( nb6bushouref[iBushouID].merits >= nb6requiredmerits[2].merits ) { // •”«‹‰ŒMŒ÷‚ğ‚ÂB
							nb6bushouname[iBushouID].position = 4; // •”«‚Ö
						} else if ( nb6bushouref[iBushouID].merits >= nb6requiredmerits[3].merits ) { // ˜‘å«‹‰ŒMŒ÷‚ğ‚ÂB
							nb6bushouname[iBushouID].position = 5; // ˜‘å«‚Ö
						}
					// ƒfƒtƒHƒ‹ƒg‚¾‚ÆŒMŒ÷‚ÍƒŠƒZƒbƒg
					} else {
						nb6bushouname[iBushouID].position = 6; // g•ª‚ğ‘«Œy‚Æ‚·‚éB
					}
					//------------------------------------------------------
				}

				/* @©•ª
				      «
				   ›-œ-›-›

				   ‚Æ‚¢‚¤‚Ì‚ğ

				   ›-›-›

				   ‚Æ‚·‚éB
				*/


				// Œ»–ğ‚É‚µ‚½‚¢•«‚ÍA‚¢‚Üæ“ª‚¾
				if ( nb6castle[iCastleID].ronin-1 == iBushouID ) {
					nb6castle[iCastleID].ronin =  nb6bushouref[iBushouID].next; // ©•ª‚Ìnext‚ğéå‚É‚·‚éB
				}

				// ©•ª‚Ìprev‚Í—LŒø‚È•«‚¾B
				if ( 0 < nb6bushouref[iBushouID].prev && nb6bushouref[iBushouID].prev <= GAMEDATASTRUCT_BUSHOU_NUM ) {

					// ©•ª‚ªæ“ª‚Å‚Í‚È‚¢•«‚ÅA
					// ‚P‚Â‘O‚Ìl‚ÌŸ‚ÌƒŠƒ“ƒNæ‚ª©•ª©g‚Ìê‡
					if ( nb6bushouref[iBushouID].prev != 0xFFFF && nb6bushouref[(nb6bushouref[iBushouID].prev-1)].next == iBushouID+1 ) {
						// ©•ª‚Å‚Í‚È‚­AŸ‚Ìl‚Ö‚ÆŒq‚¬‚©‚¦‚éB
						nb6bushouref[(nb6bushouref[iBushouID].prev-1)].next = nb6bushouref[iBushouID].next;
					}
				}

				// ©•ª‚Ìnext‚Í—LŒø‚È•«‚¾B
				if ( 0 < nb6bushouref[iBushouID].next && nb6bushouref[iBushouID].next <= GAMEDATASTRUCT_BUSHOU_NUM ) {
					// ‚P‚ÂŒã‚Ìl‚Ì‘O‚ÌƒŠƒ“ƒNæ‚ª©•ª©g‚Ìê‡
					if ( nb6bushouref[iBushouID].next != 0xFFFF && nb6bushouref[(nb6bushouref[iBushouID].next-1)].prev == iBushouID+1 ) {
						// ©•ª‚Å‚Í‚È‚­A‘O‚Ìl‚Ö‚ÆŒq‚¬‚©‚¦‚éB
						nb6bushouref[(nb6bushouref[iBushouID].next-1)].prev = nb6bushouref[iBushouID].prev;
					}
				}


				/* @
				     
				   ›-›

				   ‚Æ‚¢‚¤‚Ì‚ğ

				   ›-›-œ

				   ‚Æ‚·‚éB
				*/


				// ©•ª‚ÍŒ»–ğ‚Æ‚µ‚Äˆê”ÔÅŒã‚É‚­‚Á‚Â‚­B©•ª‚Ìnext‚Í‹‚È‚¢
				nb6bushouref[iBushouID].next = 0xFFFF;

				// ‚à‚µ©•ª‚ÍAŒ»–ğ‚É‚È‚é‚Æ‚Æ‚à‚ÉAéå‚É‚à‚È‚é‚×‚«‚È‚ç‚Îc
				if ( iLinkEndBusyouID == 0xFFFF ) {
					nb6bushouref[iBushouID].prev = 0xFFFF; // ©•ª‚Ì‘O‚Ìl‚Í‹‚È‚¢

					nb6castle[iCastleID].master = iBushouID+1; // éå‚Í©•ª

				// •«ƒŠƒ“ƒNƒŠƒXƒg‚Ìˆê”ÔÅŒã‚É‚Ô‚ç‰º‚ª‚é‚×‚«B
				} else {

					nb6bushouref[iLinkEndBusyouID-1].next = iBushouID+1; // ƒŠƒ“ƒNƒŠƒXƒg‚ÌÅŒã‚Ìnext‚ğ©•ª‚É

					nb6bushouref[iBushouID].prev = iLinkEndBusyouID; // ©•ª‚Ì‘O‚Ìl‚ÍAé‚ÌƒŠƒ“ƒNƒŠƒXƒg‚ÌÅŒã‚Ìl•¨
				}
				return true;

			}
		}
	}

	return false;
}


bool _setBushouTransportRonin(int iBushouID, int iNewCastleID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		if ( nb6bushouname[iBushouID].State == 4 ) { // ˜Ql•«‚Ìê‡‚¾‚¯ˆ—‚ğ‚·‚éB
			// ŠY“–•«‚Ì‹é‚ª‚¿‚á‚ñ‚Æ‚µ‚½î•ñ‚ªŠi”[‚³‚ê‚Ä‚¢‚éB(ƒ†[ƒU[‰ü‘¢“™‚É‚æ‚Á‚Ä‰ó‚ê‚Ä‚¢‚È‚¢)
			int iCastleID = nb6bushouref[iBushouID].castle-1;

			if ( iCastleID == iNewCastleID ) { return true; } // ˆø‚Á‰z‚µæ‚ª“¯‚¶‚¾‚Á‚½‚ç‰½‚à‚·‚é•K—v‚Í‚È‚­¬Œ÷‚µ‚½‚Æ‚İ‚È‚·B

			if (0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM ) {

				if ( nb6castle[iCastleID].ronin == 0xFFFF ) { return false; } // ƒŠƒ“ƒNƒŠƒXƒg‚ª‰ó‚ê‚Ä‚¢‚é‚Ì‚©˜Ql‚ª’N‚à‹‚È‚¢‚±‚Æ‚É‚È‚Á‚¿‚á‚Á‚Ä‚éB‰½‚à‚µ‚È‚¢Breturn;

				// ƒƒ‚ƒŠã‚Ì’l•ÏX
				{
					// Š‘®‹é‚ğ“]‘—æ‚É•ÏX‚·‚éB
					nb6bushouref[iBushouID].castle = iNewCastleID + 1;				
				}

				/* @©•ª
				      «
				   ›-œ-›-›

				   ‚Æ‚¢‚¤‚Ì‚ğ

				   ›-›-›

				   ‚Æ‚·‚éB
				*/


				// “]‘—‚µ‚½‚¢•«‚ÍA‚¢‚Üæ“ª‚¾
				if ( nb6castle[iCastleID].ronin-1 == iBushouID ) {
					nb6castle[iCastleID].ronin =  nb6bushouref[iBushouID].next; // ©•ª‚Ìnext‚ğéå‚É‚·‚éB
				}

				// ©•ª‚Ìprev‚Í—LŒø‚È•«‚¾B
				if ( 0 < nb6bushouref[iBushouID].prev && nb6bushouref[iBushouID].prev <= GAMEDATASTRUCT_BUSHOU_NUM ) {

					// ©•ª‚ªæ“ª‚Å‚Í‚È‚¢•«‚ÅA
					// ‚P‚Â‘O‚Ìl‚ÌŸ‚ÌƒŠƒ“ƒNæ‚ª©•ª©g‚Ìê‡
					if ( nb6bushouref[iBushouID].prev != 0xFFFF && nb6bushouref[(nb6bushouref[iBushouID].prev-1)].next == iBushouID+1 ) {
						// ©•ª‚Å‚Í‚È‚­AŸ‚Ìl‚Ö‚ÆŒq‚¬‚©‚¦‚éB
						nb6bushouref[(nb6bushouref[iBushouID].prev-1)].next = nb6bushouref[iBushouID].next;
					}
				}

				// ©•ª‚Ìnext‚Í—LŒø‚È•«‚¾B
				if ( 0 < nb6bushouref[iBushouID].next && nb6bushouref[iBushouID].next <= GAMEDATASTRUCT_BUSHOU_NUM ) {
					// ‚P‚ÂŒã‚Ìl‚Ì‘O‚ÌƒŠƒ“ƒNæ‚ª©•ª©g‚Ìê‡
					if ( nb6bushouref[iBushouID].next != 0xFFFF && nb6bushouref[(nb6bushouref[iBushouID].next-1)].prev == iBushouID+1 ) {
						// ©•ª‚Å‚Í‚È‚­A‘O‚Ìl‚Ö‚ÆŒq‚¬‚©‚¦‚éB
						nb6bushouref[(nb6bushouref[iBushouID].next-1)].prev = nb6bushouref[iBushouID].prev;
					}
				}

				/* @
				     
				   ›-›

				   ‚Æ‚¢‚¤‚Ì‚ğ

				   œ-›-›

				   ‚Æ‚·‚éB
				*/

				// ˜Ql‚Æ‚µ‚Ä‚İ‚È‚µ‚½A©•ª‚Ìnext‚ÍAŒ»İ˜QlƒŠƒXƒg‚Ìæ“ª‚Ìl
				nb6bushouref[iBushouID].next = nb6castle[iNewCastleID].ronin; // “]‘—æ‚ÌŒ»İ‚Ìæ“ª‚Ìl‚ğ©•ª‚ÌŸ‚Ìl‚É

				// next‚ª—LŒø‚È•«‚È‚ç‚Îc
				if ( 0 < nb6castle[iNewCastleID].ronin && nb6castle[iNewCastleID].ronin <= GAMEDATASTRUCT_BUSHOU_NUM ) {
					// Ÿ‚Ìl‚Ìprev‚ğ©•ª‚É
					nb6bushouref[ nb6castle[iNewCastleID].ronin-1].prev = iBushouID+1;
				}
				nb6bushouref[iBushouID].prev = 0xFFFF; // prev‚Í‹‚È‚¢

				nb6castle[iNewCastleID].ronin = iBushouID+1; // é‚Ì˜Ql‚Ìæ“ª‚ğ©•ª‚É‚·‚éB

				return true;
			}
		}
	}

	return false;
}


bool setBushouTransport(int iBushouID, int iNewCastleID) {

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ¡‚Ì‹é‚ÆV‚µ‚¢‹é
		int iCastleID = nb6bushouref[iBushouID].castle-1;
		if ( iCastleID == iNewCastleID ) { return true; } // ¡‹‚éêŠ‚ÆA“]‹æ‚ª“¯‚¶‚È‚ç‰½‚à‚·‚é•K—v‚Í‚È‚¢B

		// ˜Ql‚Ìê‡‚ÍˆÚ“]‚·‚é‚¾‚¯
		if ( nb6bushouname[iBushouID].State == 4 ) { // ˜Ql•«‚Ìê‡‚¾‚¯ˆ—‚ğ‚·‚éB
			return _setBushouTransportRonin(iBushouID, iNewCastleID);

		// Œ»–ğ‚Ìê‡‚ÍA˜Ql‰»‚µ‚Ä‚©‚çAˆÚ“]‚µ‚ÄA‚Ü‚½(‰Â”\‚Å‚ ‚ê‚Î)Œ»–ğ‚É–ß‚·B
		// –¢“¡é‚¾‚Æ˜Ql‚Ì‚Ü‚ÜˆÚ“]‚·‚é‚±‚Æ‚Æ‚È‚éB
		} else if ( nb6bushouname[iBushouID].State == 2 ) { // Œ»–ğ•«‚Å‚ ‚é
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

			// ˆê’[A˜Ql‚É‚µ‚Ä
			bool isRouninka = setGenekiToRounin(iBushouID);
			if (! isRouninka) { return false; }

			// ˆÊ’uˆÚ“®
			bool isTranska = _setBushouTransportRonin(iBushouID, iNewCastleID);
			if (! isTranska) { return false; }

			bool isGenekika = setRouninToGeneki(iBushouID);
			// Œ»–ğ‚É‚Å‚«‚È‚©‚Á‚½‚ç‚»‚Ì‚Ü‚Ü˜Ql‚Æ‚µ‚ÄI‚í‚éB(–¢x”zé‚Æ‚©‚¾‚ÆA‚»‚Ì‚Ü‚Ü˜Ql‚Ì‚Ü‚Ü‚Æ‚È‚éB)
			if (! isGenekika) { return false; }

			// Œ³XŒ»–ğ‚È‚Ì‚ÅA˜Ql‰»‚·‚é‘O‚Ì’l‚Å•œŒ³‚·‚é‚à‚Ì‚ğ–ß‚·B
			nb6bushouref[iBushouID].merits = _merits; // ŒMŒ÷
			nb6bushouname[iBushouID].position = _position; // g•ª
			nb6bushouref[iBushouID].loyal = _loyal; // ’‰½
			nb6bushouref[iBushouID].soldier = _soldier; // •º”
			nb6bushouref[iBushouID].training = _training; // ŒP—û
			nb6bushouref[iBushouID].solloyal = _solloyal; // m‹C
			nb6bushouref[iBushouID].form = _form; // •º‘Ô

			// “]‹‘O‚Æ“]‹Œã‚Å‘å–¼‚ª•Ï‰»‚µ‚Ä‚¢‚È‚¢ê‡‚Ì‚İAmŠ¯”N”‚â“à‰Œn‚àŒ³‚Ì’l‚ğ•œŒ³
			if (_attach == nb6bushouref[iBushouID].attach) {
				nb6bushouref[iBushouID].work = _work; // dŠ¯”N”
				nb6bushouref[iBushouID].rise = _rise; // “à‰æ
				nb6bushouref[iBushouID].bRise = _bRise; // “à‰ó‘Ô
			} else {
				nb6bushouref[iBushouID].grudge = 0; // ‘å–¼‚ª•Ï‚í‚Á‚½ê‡A¦‚İ‚ÍÁ‚·
			}

		}
	}
	return false;
}


// Œ»–ğ‚ğ‚»‚Ìé‚Ìéå‚Æ‚·‚éB
// “Á’è‚ÌuŒ»–ğv‚Ì•«‚ğA‚»‚Ìé‚Ìéå‚É‚·‚éB
// Œ³‚Ìéå‚ªuŒR’c’·v‚âu‘å–¼v‚¾‚ÆA‚»‚Ìl•¨‚Ì—§ê‚Æ‚È‚éB
bool setGenekiToJyosyu(int iBushouID) {

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		//0‘å–¼,1ŒR’·,2Œ»–ğ
		if ( nb6bushouname[iBushouID].State == 2 ) { // Œ»–ğ•«‚Ìê‡‚¾‚¯ˆ—‚ğ‚·‚éB

			// ŠY“–•«‚Ì‹é‚ª‚¿‚á‚ñ‚Æ‚µ‚½î•ñ‚ªŠi”[‚³‚ê‚Ä‚¢‚éB(ƒ†[ƒU[‰ü‘¢“™‚É‚æ‚Á‚Ä‰ó‚ê‚Ä‚¢‚È‚¢)
			int iCastleID = nb6bushouref[iBushouID].castle-1;

			if (0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM ) {

				if ( nb6castle[iCastleID].master == 0xFFFF ) { return false; } // ƒŠƒ“ƒNƒŠƒXƒg‚ª‰ó‚ê‚Ä‚¢‚é‚Ì‚©é‚É’N‚àŠ‘®‚µ‚Ä‚¢‚È‚¢B‰½‚à‚µ‚È‚¢Breturn;

				// ‚±‚±‚Ü‚Å—ˆ‚½‚çéå‚ª‹‚é‚Ì‚Åc
				int iJyosyuBushouID = nb6castle[iCastleID].master-1;

				// w’è•«‚ª‚·‚Å‚Ééå‚È‚ç‰½‚à‚µ‚È‚­‚Ä‚àéå‚Å‚ ‚éB
				if ( iBushouID== iJyosyuBushouID ) { return true; }

				// éå‚Í‹‚é‚ª(‰ü‘¢“™‚Å)‚¨‚©‚µ‚È”j‚½‚ñƒf[ƒ^‚É‚È‚Á‚Ä‚¢‚½‚çA‰½‚à‚µ‚È‚¢B
				if ( ! (0 <= iJyosyuBushouID && iJyosyuBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) ) {
					return false;
				}

				//0‘å–¼,1ŒR’·,2Œ»–ğ

				// éå‚ªŒ»–ğ‚Å‚àŒR’c’·‚Å‚à‚È‚¢‚È‚ç‚Î(‘å–¼)A‰½‚à‚µ‚È‚¢B“ü‚ê‘Ö‚¦‚é‚Æ•Ï‚È‚±‚Æ‚É‚È‚éB
				// Œ»–ğ‚È‚ç‚Î
				if ( nb6bushouname[iJyosyuBushouID].State == 2 ) {	

					; // “Á‚É‰½‚à‚µ‚È‚¢
				// ŒR’c’·‚È‚ç‚Î
				} else if ( nb6bushouname[iJyosyuBushouID].State == 1 ) {
					// ¡‚ÌŒR’c’·‚ğŒ»–ğ‚Æ‚·‚é
					nb6bushouname[iJyosyuBushouID].State = 2; // Œ»–ğ‚Æ‚·‚é
					
					// ‹t‚Éw’è•«‚ğŒR’c’·‰»‚·‚é
					nb6bushouname[iBushouID].State = 1; // ŒR’c’·
					int iGundanID = nb6bushouref[iJyosyuBushouID].gundan-1;
					// Œˆ‚µ‚Ä—v‚ç‚Ê”»’è‚Ì‚Í‚¸‚Å‚ ‚é‚ªˆê‰
					if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
						// ŒR’cî•ñ‚ÌŒR’c’·‚Ì”Ô†‚ğAw’è‚Ì•«‚É‚·‚éB
						nb6gundan[iGundanID].leader = iBushouID + 1;
					}
				// ‘å–¼‚È‚ç‚Îc
				} else if ( nb6bushouname[iJyosyuBushouID].State == 0 ) {
					// ¡‚Ì‘å–¼‚ğŒ»–ğ‚Æ‚·‚é
					nb6bushouname[iJyosyuBushouID].State = 2;
					// ‘å–¼‚¾‚Á‚½l‚Ìg•ª‚ğh˜V‚Æ‚·‚éB
					nb6bushouname[iJyosyuBushouID].position = 2; //(‰B‹=0, ‘å–¼=1, h˜V=2, ‰Æ˜V=3, •”«=4, ˜‘å«=5, ‘«Œy“ª=6)
					// ‹t‚Éw’è•«‚ğ‘å–¼‚Æ‚·‚é
					nb6bushouname[iBushouID].State = 0;
					nb6bushouname[iBushouID].position = 1;

					int iGundanID = nb6bushouref[iJyosyuBushouID].gundan-1;
					int iDaimyoID = nb6bushouref[iJyosyuBushouID].attach-1;
					// Œˆ‚µ‚Ä—v‚ç‚Ê”»’è‚Ì‚Í‚¸‚Å‚ ‚é‚ªˆê‰
					if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
						// ŒR’cî•ñ‚ÌŒR’c’·‚Ì”Ô†‚ğAw’è‚Ì•«‚É‚·‚éB
						nb6gundan[iGundanID].leader = iBushouID + 1;
					}
					if ( 0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM ) {
						// nb6daimyo‚Ì‘å–¼‚Ì”Ô†‚ğAw’è‚Ì•«‚É‚·‚éB
						nb6daimyo[iDaimyoID].attach = iBushouID + 1;
					}

				} else {
					return false;
				}




				/*éî•ñ‚Ì•«ƒŠƒXƒg

				@ @©•ª
				   «
				   ›-œ-›-›

				   ‚Æ‚¢‚¤‚Ì‚ğ

				   ›-›-›

				   ‚Æ‚·‚éB
				   */


				// ƒp[ƒW‚µ‚½‚¢•«‚ÍA‚¢‚Üéå‚¾
				if ( nb6castle[iCastleID].master-1 == iBushouID ) {
					nb6castle[iCastleID].master =  nb6bushouref[iBushouID].next; // ©•ª‚Ìnext‚ğéå‚É‚·‚éB
				}

				// ©•ª‚Ìprev‚Í—LŒø‚È•«‚¾B
				if ( 0 < nb6bushouref[iBushouID].prev && nb6bushouref[iBushouID].prev <= GAMEDATASTRUCT_BUSHOU_NUM ) {

					// ©•ª‚ªéå‚Å‚Í‚È‚¢•«‚ÅA
					// ‚P‚Â‘O‚Ìl‚ÌŸ‚ÌƒŠƒ“ƒNæ‚ª©•ª©g‚Ìê‡
					if ( nb6bushouref[iBushouID].prev != 0xFFFF && nb6bushouref[(nb6bushouref[iBushouID].prev-1)].next == iBushouID+1 ) {
						// ©•ª‚Å‚Í‚È‚­AŸ‚Ìl‚Ö‚ÆŒq‚¬‚©‚¦‚éB
						nb6bushouref[(nb6bushouref[iBushouID].prev-1)].next = nb6bushouref[iBushouID].next;
					}
				}

				// ©•ª‚Ìnext‚Í—LŒø‚È•«‚¾B
				if ( 0 < nb6bushouref[iBushouID].next && nb6bushouref[iBushouID].next <= GAMEDATASTRUCT_BUSHOU_NUM ) {
					// ‚P‚ÂŒã‚Ìl‚Ì‘O‚ÌƒŠƒ“ƒNæ‚ª©•ª©g‚Ìê‡
					if ( nb6bushouref[iBushouID].next != 0xFFFF && nb6bushouref[(nb6bushouref[iBushouID].next-1)].prev == iBushouID+1 ) {
						// ©•ª‚Å‚Í‚È‚­A‘O‚Ìl‚Ö‚ÆŒq‚¬‚©‚¦‚éB
						nb6bushouref[(nb6bushouref[iBushouID].next-1)].prev = nb6bushouref[iBushouID].prev;
					}
				}



				/* éî•ñ‚ÌéåƒŠƒXƒg@

				›-›

				‚Æ‚¢‚¤‚Ì‚ğ

				œ-›-›

				‚Æ‚·‚éB
				*/

				// ©•ª‚ªéå‚Æ‚È‚é‚Ì‚ÅA©•ª‚ÌŸ‚Ìl‚ğA¡‚Ìéå‚Ìl‚Æ‚·‚éB
				int iJosyu = nb6castle[iCastleID].master;
				nb6bushouref[iBushouID].next = iJosyu; // Œ»İ‚Ìæ“ª‚Ìl‚ğ©•ª‚ÌŸ‚Ìl‚É

				// next‚ª—LŒø‚È•«‚È‚ç‚Îc
				if ( 0 < iJosyu && iJosyu <= GAMEDATASTRUCT_BUSHOU_NUM ) {
					// Ÿ‚Ìl‚Ìprev‚ğ©•ª‚É
					nb6bushouref[iJosyu-1].prev = iBushouID+1;
				}
				
				nb6bushouref[iBushouID].prev = 0xFFFF; // –{l‚ªæ“ª‚É‚È‚é‚Ì‚ÅAprev‚Í‹‚È‚¢

				// ‚»‚µ‚ÄÅŒã‚É©•ª‚ğéå‚É
				nb6castle[iCastleID].master = iBushouID+1; // é‚Ìéå‚ğ©•ª‚É‚·‚éB

				return true;
			}
		}
	}

	return false;
}



// bfile.nb6‚É‘¶İ‚·‚é—ñ“`‚Ì”
int iExistRetudenNumOfBFileN6P = -1;



// ‘ÎÛ‚Ì•«ID‚ªŒ»İ“Sb‘Dó‘Ô‚©‚Ç‚¤‚©(“Sb‘D‚ğ‚Á‚Ä‚¢‚é‚©‚Ç‚¤‚©‚Å‚Í‚È‚­Aíê‚É‚¨‚¢‚ÄA¡“Sb‘D‚ÌŒ`ó‚©‚Ç‚¤‚©)
int iCurTekkousenForm[GAMEDATASTRUCT_BUSHOU_NUM] = {-1};


int getTouzyouBushouNum() {
	WORD *pTouzyouBushouNum = (WORD *)GAMEDATASTRUCT_TOUZYOU_BUSHOU_NUM_ADDRESS;
	return *pTouzyouBushouNum;
}

int getTaikiBushouDataNum() {
	WORD *pMitouzyouBushouNum = (WORD *)GAMEDATASTRUCT_MITOUZYOU_BUSHOU_NUM_ADDRESS;
	return *pMitouzyouBushouNum;
}



// ƒ[ƒh‚µ‚È‚¢‚Å‚â‚Á‚Ä‚éŒo‰ß‹GßB–ˆ‹Gß‚Ì•Ï‚í‚è–Ú‚Å‰ÁZ‚³‚ê‚Ä‚ä‚­B
int nb6progressseason = 1L;


void setCastleName(int iCastleID, string name) {
	if (0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM) {

		strncpy_s(nb6castle[iCastleID].name, name.c_str(), 8);
		nb6castle[iCastleID].name[8] = NULL;
		nb6castle[iCastleID].nameEdited = true;
	}
}


// Ÿè‚É•¶š—ñ‚ğŒˆ‚ß‚¤‚¿‚¹‚¸‚ÉATENSHOU.EXE“à‚Ìƒ|ƒCƒ“ƒ^”z—ñ‚©‚çˆø‚Á’£‚é‚æ‚¤‚É‚µ‚½•û‚ªA‘¼Ò‚Ì‰ü‘¢‚Ö‚Ì‘Ï‹v—Í‚ª‘‚·‚©‚àB
char nb6CastleTypeName[GAMEDATASTRUCT_CASTLETYPE_NUM][5] = {
	"é",
	"Œä–V",
	"›",
	"ŒäŠ",
	"ŠÙ",
	"",
	"é"
};

// é‚Ìƒ^ƒCƒvID‚©‚ç‹ï‘Ì“I‚È•¶š—ñ‚ğ“¾‚é
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


// w’è‚ÌŒR’c‚ªŠ‚µ‚Ä‚¢‚éé‚Ì”B
int getGundanHasCastleNum(int iGundanID) {
	int iHasCastleCnt = 0;
	if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
		for ( int iCastleID=0; iCastleID < GAMEDATASTRUCT_CASTLE_NUM; iCastleID++ ) {
			// ŒR’c”Ô†‚ªˆê’v‚·‚é‚à‚Ì‚ğƒJƒEƒ“ƒg
			if ( iGundanID == nb6castle[iCastleID].attach-1 ) {
				iHasCastleCnt++;
			}
		}
	}
	return iHasCastleCnt;
}

// w’è‚Ì‘å–¼‚ªŠ‚µ‚Ä‚¢‚éé‚Ì”B
int getDaimyoHasCastleNum(int iDaimyoID) {
	int iHasCastleCnt = 0;
	
	if ( 0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM ) {
		for ( int iCastleID=0; iCastleID < GAMEDATASTRUCT_CASTLE_NUM; iCastleID++ ) {
			int iGundanID = nb6castle[iCastleID].attach-1;

			if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
				// ‘å–¼”Ô†‚ªˆê’v‚·‚é‚à‚Ì‚ğƒJƒEƒ“ƒg
				if ( iDaimyoID == nb6gundan[iGundanID].attach-1 ) {
					iHasCastleCnt++;
				}
			}
		}
	}
	return iHasCastleCnt;
}

// w’è‚Ì‘å–¼‚ªŠ‚µ‚Ä‚¢‚éé‚Ì”Ô†y”z—ñ—pz‚ÌƒŠƒXƒg
vector<int> getDaimyoHasCastleList(int iDaimyoID) {
	vector<int> castlelist;

	if (0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM) {
		for (int iCastleID = 0; iCastleID < GAMEDATASTRUCT_CASTLE_NUM; iCastleID++) {
			int iGundanID = nb6castle[iCastleID].attach - 1;

			if (0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM) {
				// ‘å–¼”Ô†‚ªˆê’v‚·‚é‚à‚Ì‚ğƒJƒEƒ“ƒg
				if (iDaimyoID == nb6gundan[iGundanID].attach - 1) {
					castlelist.push_back(iCastleID);
				}
			}
		}
	}
	return castlelist;
}



// –ìí‚ÌƒwƒbƒNƒXŒn‚ÌÀ•W‚ğA”z—ñ‚Æ‚µ‚Ä‚ÌÀ’l‚Ìindex‚Ö‚Æ•ÏX‚·‚éB
void ConvertFieldHexPosToFieldIndexPos(double x, double y, int *out_x, int *out_y) {

	// À•W’l‚Í‘Ã“–‚È”ÍˆÍ‚©H
	if ( 0<=x && x< GAMEDATASTRUCT_FIELD_HEX_COLS && 0<=y && y< GAMEDATASTRUCT_FIELD_HEX_ROWS) {
		// y‚ğÀ•WÀ’l‚É‚µ‚ÄA®”‰»
		int _y = round_®”‰»(y);
		// –ìí‚Ì‹ô”s–Ú‚©Šï”s–Ú‚©‚ÅA”¼g‚¸‚ç‚·B‹ô”s‚È‚çA"”¼HEX"‘½‚¢’l‚ªx‚É“ü‚Á‚Ä‚­‚é‚½‚ßB
		if (_y % 2==1) {
			x = x - 0.5;
		}
		// x‚ğÅ‚à‹ß‚¢’l‚Ì®”‰»
		int _x = round_®”‰»(x);
		// ‚O‚æ‚è¬‚³‚­‚È‚é‚±‚Æ‚à‚ ‚é‚Ì‚Å‚O‚É
		if ( _x < 0 ) { _x = 0; }
		if ( _x >= GAMEDATASTRUCT_FIELD_HEX_COLS ) { _x = GAMEDATASTRUCT_FIELD_HEX_COLS-1; }
		if ( _y >= GAMEDATASTRUCT_FIELD_HEX_ROWS ) { _y = GAMEDATASTRUCT_FIELD_HEX_ROWS-1; }

		// ”z—ñƒAƒNƒZƒX’l‚Æ‚µ‚Ä‚Ìx‚Æy‚ªo‚½
		*out_x = _x;
		*out_y = _y;
	} else {
		*out_x = 0xFFFF;
		*out_y = 0xFFFF;
	}
}


// Uéí‚ÌƒwƒbƒNƒXŒn‚ÌÀ•W‚ğA”z—ñ‚Æ‚µ‚Ä‚ÌÀ’l‚Ìindex‚Ö‚Æ•ÏX‚·‚éB
void ConvertCastleHexPosToCastleIndexPos(double x, double y, int *out_x, int *out_y) {

	// À•W’l‚Í‘Ã“–‚È”ÍˆÍ‚©H
	if ( 0<=x && x< GAMEDATASTRUCT_CASTLE_HEX_COLS && 0<=y && y< GAMEDATASTRUCT_CASTLE_HEX_ROWS) {
		// y‚ğÀ•WÀ’l‚É‚µ‚ÄA®”‰»
		int _y = round_®”‰»(y);
		// –ìí‚Ì‹ô”s–Ú‚©Šï”s–Ú‚©‚ÅA”¼g‚¸‚ç‚·B‹ô”s‚È‚çA"”¼HEX"‘½‚¢’l‚ªx‚É“ü‚Á‚Ä‚­‚é‚½‚ßB
		if (_y % 2==1) {
			x = x - 0.5;
		}
		// x‚ğÅ‚à‹ß‚¢’l‚Ì®”‰»
		int _x = round_®”‰»(x);
		// ‚O‚æ‚è¬‚³‚­‚È‚é‚±‚Æ‚à‚ ‚é‚Ì‚Å‚O‚É
		if ( _x < 0 ) { _x = 0; }
		if ( _x >= GAMEDATASTRUCT_CASTLE_HEX_COLS ) { _x = GAMEDATASTRUCT_CASTLE_HEX_COLS-1; }
		if ( _y >= GAMEDATASTRUCT_CASTLE_HEX_ROWS ) { _y = GAMEDATASTRUCT_CASTLE_HEX_ROWS-1; }

		// ”z—ñƒAƒNƒZƒX’l‚Æ‚µ‚Ä‚Ìx‚Æy‚ªo‚½
		*out_x = _x;
		*out_y = _y;
	} else {
		*out_x = 0xFFFF;
		*out_y = 0xFFFF;
	}
}

// ‚ ‚éƒwƒbƒNƒXêŠ‚Ìƒ`ƒbƒv‚Ì–ğŠ„(FIELD_HEX_ROLE)‚ğ“¾‚é
int GetFieldHexRole(double x, double y) {
	// À•W’l‚Í‘Ã“–‚È”ÍˆÍ‚©H
	if ( 0<=x && x<= GAMEDATASTRUCT_FIELD_HEX_COLS && 0<=y && y<=GAMEDATASTRUCT_FIELD_HEX_ROWS) {
		// y‚ğÀ•WÀ’l‚É‚µ‚ÄA®”‰»
		int _y = round_®”‰»(y);
		// –ìí‚Ì‹ô”s–Ú‚©Šï”s–Ú‚©‚ÅA”¼g‚¸‚ç‚·B‹ô”s‚È‚çA"”¼HEX"‘½‚¢’l‚ªx‚É“ü‚Á‚Ä‚­‚é‚½‚ßB
		if (_y % 2==1) {
			x = x - 0.5;
		}
		// x‚ğÅ‚à‹ß‚¢’l‚Ì®”‰»
		int _x = round_®”‰»(x);
		// ‚O‚æ‚è¬‚³‚­‚È‚é‚±‚Æ‚à‚ ‚é‚Ì‚Å‚O‚É
		if ( _x < 0 ) { _x = 0; }
		if ( _x >= GAMEDATASTRUCT_FIELD_HEX_COLS ) { _x = GAMEDATASTRUCT_FIELD_HEX_COLS-1; }
		if ( _y >= GAMEDATASTRUCT_FIELD_HEX_ROWS ) { _y = GAMEDATASTRUCT_FIELD_HEX_ROWS-1; }
		// ŠY“–‚ÌêŠ‚É‚ ‚éƒ`ƒbƒv‚Ì–ğŠ„”Ô†(FIELD_HEX_ROLE)‚ğ•Ô‚·
		return nb6field_hex_role.pos_y[_y].pos_x[_x];
	} else {
		return -1;
	}
}

int GetFieldHexRole(HEX_POSITION p) {
	return GetFieldHexRole(p.x, p.y);
}

// w’è‚Ì–ìí‚Ì–ğŠ„‚Æ‡’v‚·‚éAˆÊ’uƒŠƒXƒg‚ğ“¾‚é
// role ‚É‚Í FIELD_HEX_ROLE::* ‚Ì’l‚ğw’è‚·‚é
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



// ‚ ‚éƒwƒbƒNƒXêŠ‚Ìƒ`ƒbƒv‚Ì–ğŠ„(CASTLE_HEX_ROLE)‚ğ“¾‚é
int GetCastleHexRole(double x, double y) {
	// À•W’l‚Í‘Ã“–‚È”ÍˆÍ‚©H
	if ( 0<=x && x< GAMEDATASTRUCT_CASTLE_HEX_COLS && 0<=y && y< GAMEDATASTRUCT_CASTLE_HEX_ROWS) {
		// y‚ğÀ•WÀ’l‚É‚µ‚ÄA®”‰»
		int _y = round_®”‰»(y);
		// –ìí‚Ì‹ô”s–Ú‚©Šï”s–Ú‚©‚ÅA”¼g‚¸‚ç‚·B‹ô”s‚È‚çA"”¼HEX"‘½‚¢’l‚ªx‚É“ü‚Á‚Ä‚­‚é‚½‚ßB
		if (_y % 2==1) {
			x = x - 0.5;
		}
		// x‚ğÅ‚à‹ß‚¢’l‚Ì®”‰»
		int _x = round_®”‰»(x);
		// ‚O‚æ‚è¬‚³‚­‚È‚é‚±‚Æ‚à‚ ‚é‚Ì‚Å‚O‚É
		if ( _x < 0 ) { _x = 0; }
		if ( _x >= GAMEDATASTRUCT_CASTLE_HEX_COLS ) { _x = GAMEDATASTRUCT_CASTLE_HEX_COLS-1; }
		if ( _y >= GAMEDATASTRUCT_CASTLE_HEX_ROWS ) { _y = GAMEDATASTRUCT_CASTLE_HEX_ROWS-1; }
		// ŠY“–‚ÌêŠ‚É‚ ‚éƒ`ƒbƒv‚Ì–ğŠ„”Ô†(CASTLE_HEX_ROLE)‚ğ•Ô‚·
		return nb6castle_hex_role.pos_y[_y].pos_x[_x];
	} else {
		return -1;
	}
}


int GetCastleHexRole(HEX_POSITION p) {
	return GetCastleHexRole(p.x, p.y);
}


// w’è‚ÌUéí‚Ì–ğŠ„‚Æ‡’v‚·‚éAˆÊ’uƒŠƒXƒg‚ğ“¾‚é
// role ‚É‚Í CASTLE_HEX_ROLE::* ‚Ì’l‚ğw’è‚·‚é
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



/* ‰º‚Å‰E‚PƒoƒCƒg‚ª‚P‚ÌŠ‚Í‰Š‚ª•t‚¢‚Ä‚¢‚é
004D4DAA                          10 10 10 10 10 10 10 10          
004D4DBA  10 00 00 00 00 00 00 00 00 10 10 10 11 98 90 10  ........E
004D4DCA  10 10 10 00 00 00 00 00 00 00 00 10 10 10 10 90  ........E
004D4DDA  90 90 10 10 10 10 00 00 00 00 00 00 00 10 10 10  .......
004D4DEA  10 91 90 10 10 10 10 00 00 00 00 00 00 00 00 00  ‘.........
004D4DFA  10 10 10 11 10 10 10 10 10 00 00 00 00 00 00 00  .......
004D4E0A  00 00 10 10 11 11 11 10 10 10 00 00 00 00 00 00  ........
004D4E1A  00 00 00 00 00 10 10 10 11 10 10 10 00 00 00 00  .........
004D4E2A  00 00 00 00 00 00 00 10 10 10 10 10 10 00 00 00  ..........
*/
// ‚ ‚éƒwƒbƒNƒXêŠ‚Ìƒ`ƒbƒv‚Ì‰Š‚Ì–ğŠ„(CASTLE_HEX_ROLE)‚ğ“¾‚é
// ‰Š‚ª•t‚¢‚Ä‚¢‚éŠ‚ÍA‰E‚Ì‚PƒoƒCƒg‚ª1‚É‚È‚éB
int GetFireHexRole(double x, double y) {
	// À•W’l‚Í‘Ã“–‚È”ÍˆÍ‚©H
	if ( 0<=x && x< GAMEDATASTRUCT_CASTLE_HEX_COLS && 0<=y && y< GAMEDATASTRUCT_CASTLE_HEX_ROWS) {
		// y‚ğÀ•WÀ’l‚É‚µ‚ÄA®”‰»
		int _y = round_®”‰»(y);
		// –ìí‚Ì‹ô”s–Ú‚©Šï”s–Ú‚©‚ÅA”¼g‚¸‚ç‚·B‹ô”s‚È‚çA"”¼HEX"‘½‚¢’l‚ªx‚É“ü‚Á‚Ä‚­‚é‚½‚ßB
		if (_y % 2==1) {
			x = x - 0.5;
		}
		// x‚ğÅ‚à‹ß‚¢’l‚Ì®”‰»
		int _x = round_®”‰»(x);
		// ‚O‚æ‚è¬‚³‚­‚È‚é‚±‚Æ‚à‚ ‚é‚Ì‚Å‚O‚É
		if ( _x < 0 ) { _x = 0; }
		if ( _x >= GAMEDATASTRUCT_CASTLE_HEX_COLS ) { _x = GAMEDATASTRUCT_CASTLE_HEX_COLS-1; }
		if ( _y >= GAMEDATASTRUCT_CASTLE_HEX_ROWS ) { _y = GAMEDATASTRUCT_CASTLE_HEX_ROWS-1; }
		// ŠY“–‚ÌêŠ‚É‚ ‚éƒ`ƒbƒv‚Ì–ğŠ„”Ô†(CASTLE_HEX_ROLE)‚ğ•Ô‚·
		return (nb6fire_hex_role.pos_y[_y].pos_x[_x] & 0xF) == 1;
	} else {
		return -1;
	}
}


int GetFireHexRole(HEX_POSITION p) {
	return GetFireHexRole(p.x, p.y);
}


// w’è‚ÌUéí‚Ì‰Šãó‘Ô‚Æ‡’v‚·‚éAˆÊ’uƒŠƒXƒg‚ğ“¾‚é
// role ‚É‚Í CASTLE_HEX_ROLE::* ‚Ì’l‚ğw’è‚·‚éBw’è‚µ‚È‚¯‚ê‚Î‰Šãó‘Ô‚Ì‚à‚Ì‚ğW‚ß‚é
vector<HEX_POSITION> GetFireeHexPosOfHexRole(int role = FIRE_HEX_ROLE::‰Šã) {
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



// ‚ ‚éƒwƒbƒNƒXêŠ‚Ìƒ`ƒbƒv‚Ì‚‚³‚ğ“¾‚é
int GetFieldHexHigh(double x, double y) {
	int role = GetFieldHexRole(x, y);
	if ( role == FIELD_HEX_ROLE::ì || FIELD_HEX_ROLE::ŒÎ || FIELD_HEX_ROLE::ŠC ) {
		return 0;
	} else if ( role == FIELD_HEX_ROLE::’áR ) {
		return 2;
	} else if ( role == FIELD_HEX_ROLE::’†R ) {
		return 3;
	} else if ( role == FIELD_HEX_ROLE::‚R ) {
		return 4;
	} else {
		return 1;
	}
}

int GetFieldHexHigh(HEX_POSITION p) {
	return GetFieldHexHigh(p.x, p.y);
}

// ‚ ‚éƒwƒbƒNƒXêŠ‚Ìƒ`ƒbƒv‚Ì‚‚³‚ğ“¾‚é
int GetCastleHexHigh(double x, double y) {
	// À•W’l‚Í‘Ã“–‚È”ÍˆÍ‚©H
	if ( 0<=x && x< GAMEDATASTRUCT_CASTLE_HEX_COLS && 0<=y && y< GAMEDATASTRUCT_CASTLE_HEX_ROWS) {
		// y‚ğÀ•WÀ’l‚É‚µ‚ÄA®”‰»
		int _y = round_®”‰»(y);
		// –ìí‚Ì‹ô”s–Ú‚©Šï”s–Ú‚©‚ÅA”¼g‚¸‚ç‚·B‹ô”s‚È‚çA"”¼HEX"‘½‚¢’l‚ªx‚É“ü‚Á‚Ä‚­‚é‚½‚ßB
		if (_y % 2==1) {
			x = x - 0.5;
		}
		// x‚ğÅ‚à‹ß‚¢’l‚Ì®”‰»
		int _x = round_®”‰»(x);
		// ‚O‚æ‚è¬‚³‚­‚È‚é‚±‚Æ‚à‚ ‚é‚Ì‚Å‚O‚É
		if ( _x < 0 ) { _x = 0; }
		if ( _x >= GAMEDATASTRUCT_CASTLE_HEX_COLS ) { _x = GAMEDATASTRUCT_CASTLE_HEX_COLS-1; }
		if ( _y >= GAMEDATASTRUCT_CASTLE_HEX_ROWS ) { _y = GAMEDATASTRUCT_CASTLE_HEX_ROWS-1; }
		// ŠY“–‚ÌêŠ‚É‚ ‚éƒ`ƒbƒv‚Ì–ğŠ„”Ô†(CASTLE_HEX_ROLE)‚ğ•Ô‚·
		return nb6castle_hex_high.pos_y[_y].pos_x[_x];
	} else {
		return -1;
	}
}

int GetCastleHexHigh(HEX_POSITION p) {
	return GetCastleHexHigh(p.x, p.y);
}


// ‹t‚Éİ’è‚·‚éB(‚Æ‚¢‚Á‚Ä‚àAİ’è‚µ‚½‚Æ‚µ‚Ä‚àAŠG‚ª•Ï‚í‚ç‚È‚¢‚Ì‚Åãè‚­‚¢‚©‚È‚¢‚ª)
void SetFieldHexRole(double x, double y, int eHexRole) {
	// À•W’l‚Í‘Ã“–‚È”ÍˆÍ‚©H
	if ( 0<=x && x< GAMEDATASTRUCT_FIELD_HEX_COLS && 0<=y && y< GAMEDATASTRUCT_FIELD_HEX_ROWS) {
		// y‚ğÀ•WÀ’l‚É‚µ‚ÄA®”‰»
		int _y = round_®”‰»(y);
		// –ìí‚Ì‹ô”s–Ú‚©Šï”s–Ú‚©‚ÅA”¼g‚¸‚ç‚·B‹ô”s‚È‚çA"”¼HEX"‘½‚¢’l‚ªx‚É“ü‚Á‚Ä‚­‚é‚½‚ßB
		if (_y % 2==1) {
			x = x - 0.5;
		}
		// x‚ğÅ‚à‹ß‚¢’l‚Ì®”‰»
		int _x = round_®”‰»(x);
		// ‚O‚æ‚è¬‚³‚­‚È‚é‚±‚Æ‚à‚ ‚é‚Ì‚Å‚O‚É
		if ( _x < 0 ) { _x = 0; }
		if ( _x >= GAMEDATASTRUCT_FIELD_HEX_COLS ) { _x = GAMEDATASTRUCT_FIELD_HEX_COLS-1; }
		if ( _y >= GAMEDATASTRUCT_FIELD_HEX_ROWS ) { _y = GAMEDATASTRUCT_FIELD_HEX_ROWS-1; }
		// ŠY“–‚ÌêŠ‚Ìƒ`ƒbƒv‚Ì–ğŠ„”Ô†(FIELD_HEX_ROLE)‚ğİ’è‚·‚éB
		nb6field_hex_role.pos_y[_y].pos_x[_x] = eHexRole;
	}
}

void SetFieldHexRole(HEX_POSITION p, int eHexRole) {
	SetFieldHexRole(p.x, p.y, eHexRole);
}

// ƒtƒB[ƒ‹ƒhƒwƒbƒNƒX‚ÌÀ•W¨•’Ê‚Ì‚RŸŒ³À•W(¶èƒfƒJƒ‹ƒgE‰¹‚Æ“¯‚¶‚Ö)
D3D_POSITION FieldHexPos_To_WorldPos(HEX_POSITION p) {
	D3D_POSITION p_real;
	p_real.x = p.x - GAMEDATASTRUCT_FIELD_HEX_CENTER_X;
	p_real.z = (p.y -GAMEDATASTRUCT_FIELD_HEX_CENTER_Y) * ( sqrt(3.0f) / 2.0f ) * -1; // sin60‹
	if ( p_real.z == 0 ) { p_real.z = 0; } // ƒAƒz‚İ‚½‚¢‚¾‚ªA"-0"‚Æ‚È‚ç‚È‚¢‚æ‚¤‚É‚·‚éB

	// y‚Ì‚‚³‚ğŒˆ‚ß‚é‚É‚ ‚½‚Á‚ÄA’áR‚È‚ç1, ’†R‚È‚ç2, ‚R‚È‚ç3‚Æ‚·‚éB
	int iHexRole = GetFieldHexRole( p.x, p.y );

	if ( iHexRole == FIELD_HEX_ROLE::‚R ) {
		p_real.y = 3;
	} else if ( iHexRole == FIELD_HEX_ROLE::’†R ) {
		p_real.y = 2;
	} else if ( iHexRole == FIELD_HEX_ROLE::’áR ) {
		p_real.y = 1;
	} else {
		p_real.y = 0;
	}
	return p_real;
}

// •’Ê‚Ì‚RŸŒ³À•W(¶èƒfƒJƒ‹ƒg)¨ƒtƒB[ƒ‹ƒhƒwƒbƒNƒX‚ÌÀ•W‚Ö
HEX_POSITION WorldPos_To_FieldHexPos(D3D_POSITION p) {
	HEX_POSITION p_hex;
	p_hex.x = round_®”‰»(p.x + GAMEDATASTRUCT_FIELD_HEX_CENTER_X);
	p_hex.y = round_“ñ•ª‚Ìˆê‹z’…‰»((2.0f / sqrt(3.0f) ) * (p.z) * -1 + GAMEDATASTRUCT_FIELD_HEX_CENTER_Y);
	return p_hex;
}

// éƒwƒbƒNƒX‚ÌÀ•W¨•’Ê‚Ì‚RŸŒ³À•W(¶èƒfƒJƒ‹ƒgE‰¹‚Æ“¯‚¶‚Ö)
D3D_POSITION CastleHexPos_To_WorldPos(HEX_POSITION p) {
	D3D_POSITION p_real;
	p_real.x = p.x - GAMEDATASTRUCT_CASTLE_HEX_CENTER_X;
	p_real.z = (p.y -GAMEDATASTRUCT_CASTLE_HEX_CENTER_Y) * ( sqrt(3.0f) / 2.0f ) * -1; // sin60‹
	if ( p_real.z == 0 ) { p_real.z = 0; } // ƒAƒz‚İ‚½‚¢‚¾‚ªA"-0"‚Æ‚È‚ç‚È‚¢‚æ‚¤‚É‚·‚éB

	// y‚Ì‚‚³‚ğŒˆ‚ß‚é‚É‚ ‚½‚Á‚Äé‚Ì‚‚³î•ñ‚ğ“–‚Ä‚Í‚ß‚éB
	int iHexHigh = GetCastleHexHigh( p.x, p.y );
	p_real.y = iHexHigh;
	return p_real;
}

// •’Ê‚Ì‚RŸŒ³À•W(¶èƒfƒJƒ‹ƒg)¨éƒwƒbƒNƒX‚ÌÀ•W‚Ö
HEX_POSITION WorldPos_To_CastleHexPos(D3D_POSITION p) {
	HEX_POSITION p_hex;
	p_hex.x = round_®”‰»(p.x + GAMEDATASTRUCT_CASTLE_HEX_CENTER_X);
	p_hex.y = round_“ñ•ª‚Ìˆê‹z’…‰»((2.0f / sqrt(3.0f) ) * (p.z) * -1 + GAMEDATASTRUCT_CASTLE_HEX_CENTER_Y);
	return p_hex;
}


// ƒwƒbƒNƒXŠÔŠu‚Ìæ“¾Bp1¨p2‚Ü‚Å‰½eˆÚ“®‚·‚é•K—v‚ª‚ ‚é‚Ì‚©(‘S‚Ä‚Ìe‚Ís‚¯‚é‚à‚Ì‚Æ‰¼’è‚·‚é)
int getHexDistance( HEX_POSITION p1, HEX_POSITION p2 ) {
	double ax = double(abs(p1.x-p2.x));
	double ay = double(abs(p1.y-p2.y));
	if ( p1.y == p2.y ) { // “¯‚¶s‚É‹‚é‚È‚ç‚Îc
		return round_®”‰»(ax);
	} else if ( ax*2 <= ay ) { // 2“_ŠÔ‚ÌXÀ•W‚Ì·‚ª2“_ŠÔ‚ÌYÀ•W‚Ì·‚Ì”¼•ªˆÈ‰º‚Ìê‡AYÀ•W‚ÌˆÚ“®‚¾‚¯‚ÅŠY“–ƒ|ƒCƒ“ƒg‚Ü‚Å“’B‚Å‚«‚éB
		return round_®”‰»(ay);
	} else {
		// X‚Ì· + Y‚Ì· - Y‚Ì·‚Ì”¼•ª
		return round_®”‰»( ax + ay - 0.5*ay );
	}
}

// Œ»İow‚µ‚Ä‚¢‚éiBushouID‚Ìˆê——‚ğ“¾‚éB
vector<int> GetSyutujinBushouArray() {
	vector<int> bArray;

	// íê‚É‚¢‚é•«‚©‚çğŒ‚ğ–‚½‚·‚à‚Ì‚ÌˆÚ“®c—Ê‚ğ‘‚â‚·B
	for ( int i=0; i<GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_NUM; i++ ) {
		if ( nb6bushou_hex_unit_info[i].bushou == 0xFFFF ) { continue; } // 0xFFFF‚¾‚Æ‚»‚ÌƒXƒƒbƒg‚É•«‚Í‹‚È‚¢BŸ‚Ìl
		if ( nb6bushou_hex_unit_info[i].bushou > GAMEDATASTRUCT_BUSHOU_NUM ) { continue; } // ‚±‚±‚Å”ÍˆÍ•Ûá‚µ‚Ä‚¨‚¢‚Ä‚ ‚°‚éBŸ‚Ìl
		
		int iBushouID = nb6bushou_hex_unit_info[i].bushou - 1;

		if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			// í€‚â‘Ş‹p‚µ‚Ä‚¢‚È‚¢
			if ( nb6bushouname[iBushouID].battleDeath == 0 && nb6bushouname[iBushouID].battleRetreat == 0 ) {
				bArray.push_back( iBushouID );
			}
		}
	}

	return bArray;
}

// Œ»İ•\¦‚³‚ê‚Ä‚¢‚éƒ}ƒbƒvŒÀ’è‚ÅAow‚µ‚Ä‚¢‚éiBushouID‚Ìˆê——‚ğ“¾‚éB
vector<int> GetCurMapSyutujinBushouArray() {
	vector<int> bArray;

	if ( Is_CastleWar() ) {
		for ( int i=0; i<GAMEDATASTRUCT_CASTLE_HEX_COLS * GAMEDATASTRUCT_CASTLE_HEX_ROWS; i++ ) {
			int iBango = nb6castle_hex_unit_pos.index[i];
			if ( 1 <= iBango && iBango <= GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_NUM ) {
				if ( nb6bushou_hex_unit_info[iBango-1].bushou != 0xFFFF ) {
					int iBushouID = nb6bushou_hex_unit_info[iBango-1].bushou-1;

					if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
						// í€‚â‘Ş‹p‚µ‚Ä‚¢‚È‚¢
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
						// í€‚â‘Ş‹p‚µ‚Ä‚¢‚È‚¢
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


// w’è‚Ì•«‚h‚c‚Ì•«‚ª‚Ç‚±‚É‚¢‚é‚Ì‚©H ‹‚È‚¢ê‡Ap(x=0xFFFF, y=0xFFFF) ‚ª•Ô‚Á‚Ä‚­‚éB
HEX_POSITION GetFieldHexUnitPos(int iBushouID) {

	HEX_POSITION pos( 0xFFFF, 0xFFFF );

	if ( !(0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) ) {
		return pos;
	}
	if ( !Is_FieldWar() ) {
		return pos;
	}

	// íê‚É‚¢‚é•«‚©‚çğŒ‚ğ–‚½‚·‚à‚Ì‚ÌˆÚ“®c—Ê‚ğ‘‚â‚·B
	for ( int i=0; i<GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_NUM; i++ ) {
		if ( nb6bushou_hex_unit_info[i].bushou == 0xFFFF ) { continue; } // 0xFFFF‚¾‚Æ‚»‚ÌƒXƒƒbƒg‚É•«‚Í‹‚È‚¢BŸ‚Ìl
		if ( nb6bushou_hex_unit_info[i].bushou > GAMEDATASTRUCT_BUSHOU_NUM ) { continue; } //

		// ’T‚µ‚Ä‚é•«‚ªA•\¦’†‚Ì–ìí‚É‹‚½!!
		if ( iBushouID == nb6bushou_hex_unit_info[i].bushou - 1 ) {
			int x = nb6bushou_hex_unit_info[i].pos_x;
			int y = nb6bushou_hex_unit_info[i].pos_y;

			if ( x==0xFFFF ) {
				return pos;
			}

			// y‚ğÀ•WÀ’l‚É‚µ‚ÄA®”‰»
			pos.y = y;

			// –ìí‚Ì‹ô”s–Ú‚©Šï”s–Ú‚©‚ÅA”¼g‚¸‚ç‚·B‹ô”s‚È‚çA"À•W‚ğ0.5•ª‰E‚É‚¸‚ç‚·B
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




// w’è‚Ì•«‚h‚c‚Ì•«‚ª‚Ç‚±‚É‚¢‚é‚Ì‚©H ‹‚È‚¢ê‡Ap(x=0xFFFF, y=0xFFFF) ‚ª•Ô‚Á‚Ä‚­‚éB
HEX_POSITION GetCastleHexUnitPos(int iBushouID) {


	HEX_POSITION pos( 0xFFFF, 0xFFFF );

	if ( !(0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) ) {
		return pos;
	}
	if ( !Is_CastleWar() ) {
		return pos;
	}

	// íê‚É‚¢‚é•«‚©‚çğŒ‚ğ–‚½‚·‚à‚Ì‚ÌˆÚ“®c—Ê‚ğ‘‚â‚·B
	for ( int i=0; i<GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_NUM; i++ ) {
		if ( nb6bushou_hex_unit_info[i].bushou == 0xFFFF ) { continue; } // 0xFFFF‚¾‚Æ‚»‚ÌƒXƒƒbƒg‚É•«‚Í‹‚È‚¢BŸ‚Ìl
		if ( nb6bushou_hex_unit_info[i].bushou > GAMEDATASTRUCT_BUSHOU_NUM ) { continue; } //

		// ’T‚µ‚Ä‚é•«‚ªA•\¦’†‚ÌUéí“¬‚É‹‚½!!
		if ( iBushouID == nb6bushou_hex_unit_info[i].bushou - 1 ) {
			int x = nb6bushou_hex_unit_info[i].pos_x;
			int y = nb6bushou_hex_unit_info[i].pos_y;

			if ( x==0xFFFF ) {
				return pos;
			}

			// y‚ğÀ•WÀ’l‚É‚µ‚ÄA®”‰»
			pos.y = y;

			// –ìí‚Ì‹ô”s–Ú‚©Šï”s–Ú‚©‚ÅA”¼g‚¸‚ç‚·B‹ô”s‚È‚çA"À•W‚ğ0.5•ª‰E‚É‚¸‚ç‚·B
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


// Œ»İis’†‚Ìí‘ˆ‚ÉŠÖ‚í‚Á‚Ä‚¢‚éiDaimyoID‚ÌƒŠƒXƒgB(ow‚µ‚Ä‚¢‚é‚©‚Ç‚¤‚©‚ÍŠÖŒW‚È‚¢)
vector<int> GetCurWarJointDaimyoIDList() {
	vector<int> list;
	if ( Is_FieldWar() || Is_CastleWar() ) {
		// Q‰Á‘å–¼”Ô†‚ğŒŸõ‚µ‚ÄA
		for ( int ix=0; ix < GAMEDATASTRUCT_WAR_JOINT_DAIMYO_HEX_GROUP_INFO_NUM; ix++ ) {

			int iDaimyoID = (int)nb6war_joint_daimyo_hex_group_info[ix].daimyo - 1;
			// ”ÍˆÍ“à‚Å‚ ‚ê‚ÎAƒŠƒXƒg‚É’Ç‰Á
			if ( 0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM ) {
				// ŠY“–‚Ì’l‚ª‚·‚Å‚Élist“à‚É‚ ‚é‚©
				vector<int>::iterator iter = std::find( list.begin(), list.end() , iDaimyoID );
				// ‘¶İ‚µ‚È‚¯‚ê‚Îc
				if ( iter == list.end() ) {
					// ’Ç‰Á
					list.push_back( iDaimyoID );
				}
			}
		}
	}

	
	return list;
}

// Œ»İis’†‚Ìí‘ˆ‚ÉŠÖ‚í‚Á‚Ä‚¢‚éiGundanID‚ÌƒŠƒXƒgB(ow‚µ‚Ä‚¢‚é‚©‚Ç‚¤‚©‚ÍŠÖŒW‚È‚¢)
vector<int> GetCurWarJointGundanIDList() {
	vector<int> list;
	if ( Is_FieldWar() || Is_CastleWar() ) {
		// Q‰Á‘å–¼”Ô†‚ğŒŸõ‚µ‚ÄA
		for ( int ix=0; ix < GAMEDATASTRUCT_WAR_JOINT_DAIMYO_HEX_GROUP_INFO_NUM; ix++ ) {

			int iGundanID = (int)nb6war_joint_daimyo_hex_group_info[ix].gundan - 1;
			// ”ÍˆÍ“à‚Å‚ ‚ê‚ÎAƒŠƒXƒg‚É’Ç‰Á
			if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
				list.push_back( iGundanID );
			}
		}
	}

	return list;
}

extern í‘ˆŠJnƒpƒ‰ƒƒ^ war_start_param;
// ŠY“–‚ÌŒR’c‚Ìíê‚É‚¨‚¯‚é—§ê‚ğ•Ô‚·(’¼Úç‚é‘¤=0, ’¼ÚU‚ß‚é‘¤=1, U‚ß‚é‘¤‚Ì‰‡ŒR=3, ç‚é‘¤‚Ì‰‡ŒR=4, ’†—§ŒR=5)
int GetWarStandPointTheGundan(int iGundanID) {

	if ( !(0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) ) {
		return WarStandPoint::unknown;
	}

	// enum { unknown = -1,  UŒ‚eŒR = 1,  UŒ‚qŒR = 5,  ç”õeŒR=0, ç”õqŒR = 4, ’†—§eŒR=2, ’†—§qŒR=6, UŒ‚‰‡ŒReŒR=1, UŒ‚‰‡ŒRqŒR=5, ç”õ‰‡ŒReŒR=0, ç”õ‰‡ŒRqŒR=4 };

	if ( Is_FieldWar() || Is_CastleWar() ) {

		//--------------------------------- ’¼ÚUŒ‚‘¤A‚à‚µ‚­‚ÍA’¼Ú–hŒä‘¤‚Å‚ ‚ê‚ÎAí‘ˆŠJnƒpƒ‰ƒƒ^‚©‚ç‚Í‚¶‚«o‚¹‚é
		// w’è‚ÌŒR’c‚Ì‘å–¼ID
		int iDaimyoID = nb6gundan[iGundanID].attach-1;

		// UŒ‚‘¤‚ÌŒR’c‚Ì‘å–¼ID
		int iAttackDaimyoID = -1; 
		if ( 0 <= war_start_param.UŒ‚‘¤ŒR’c_ŒR’c”Ô†-1 && war_start_param.UŒ‚‘¤ŒR’c_ŒR’c”Ô†-1 < GAMEDATASTRUCT_GUNDAN_NUM ) {
			iAttackDaimyoID = nb6gundan[war_start_param.UŒ‚‘¤ŒR’c_ŒR’c”Ô†-1].attach-1;
		}

		// ç”õ‘¤‚ÌŒR’c‚Ì‘å–¼ID
		int iDefendDaimyoID = -1; 
		if ( 0 <= war_start_param.ç”õ‘¤ŒR’c_ŒR’c”Ô†-1 && war_start_param.ç”õ‘¤ŒR’c_ŒR’c”Ô†-1 < GAMEDATASTRUCT_GUNDAN_NUM ) {
			iDefendDaimyoID = nb6gundan[war_start_param.ç”õ‘¤ŒR’c_ŒR’c”Ô†-1].attach-1;
		}

		// w’è‚ÌŒR’c‚Ì‘å–¼‚Í’¼ÚUŒ‚‚Ì‘å–¼
		if ( iDaimyoID == iAttackDaimyoID ) {
			return WarStandPoint::attack; // ’¼ÚUŒ‚‘¤
		}
		// w’è‚ÌŒR’c‚Ì‘å–¼‚Í’¼Úç”õ‚Ì‘å–¼
		else if ( iDaimyoID == iDefendDaimyoID ) {
			return WarStandPoint::defend; // ’¼Ú–hŒä‘¤
		}

		//--------------------------------- ‚»‚êˆÈŠO‚Ìê‡‚ÍAWAR_JOINT_DAIMYO_HEX_GROUP_INFO Œn‚Ìî•ñ‚©‚ç’H‚Á‚Ä‚¢‚­

		// Q‰Á‘å–¼”Ô†‚ğŒŸõ‚µ‚ÄAUŒ‚‘¤‚ÌƒOƒ‹[ƒvIDAç”õ‘¤‚ÌƒOƒ‹[ƒvID‚ğo‚·B
		for ( int ix=0; ix < GAMEDATASTRUCT_WAR_JOINT_DAIMYO_HEX_GROUP_INFO_NUM; ix++ ) {
			// ‰‡ŒR‚â’†—§ŒR‚©ƒ`ƒFƒbƒN
			int JoinGundanID = (int)nb6war_joint_daimyo_hex_group_info[ix].gundan - 1; // Q‰ÁŒR’cID
			int JoinDaimyoID = (int)nb6war_joint_daimyo_hex_group_info[ix].daimyo - 1; // Q‰Á‘å–¼ID
			// w’è‚ÌŒR’c‚Æ“¯‚¶ŒR’cID‚Å‚ ‚éê‡‚Ì‚İ’l‚ğ•Ô‚·B
			if ( iGundanID == JoinGundanID ) {

				// w’è‚ÌŒR’c‚Ì‘å–¼‚Í’¼ÚUŒ‚‚Ì‘å–¼
				if ( JoinDaimyoID == iAttackDaimyoID ) {
					return WarStandPoint::attack; // ’¼ÚUŒ‚‘¤
				}
				// w’è‚ÌŒR’c‚Ì‘å–¼‚Í’¼Úç”õ‚Ì‘å–¼
				else if ( JoinDaimyoID == iDefendDaimyoID ) {
					return WarStandPoint::defend; // ’¼Ú–hŒä‘¤
				}

				// 2‚©6‚È‚ç’†—§ŒRB2‚ª’†—§‚ÌeŒR‚ÅA6‚ª’†—§‚ÌqŒR
				else if ( nb6war_joint_daimyo_hex_group_info[ix].group == 2 || nb6war_joint_daimyo_hex_group_info[ix].group == 6 ) {
					// ’†—§ŒR
					return WarStandPoint::chuuritsu;
				}
				// 1‚©5‚È‚ç‰‡ŒR‚ÌUŒ‚ŒRB1‚ª‰‡ŒR‚ÌeŒR‚ÅA5‚ª‰‡ŒR‚ÌqŒR
				else if ( nb6war_joint_daimyo_hex_group_info[ix].group == 1 || nb6war_joint_daimyo_hex_group_info[ix].group == 5 ) {
					// ‰‡ŒR‚ÌUŒ‚ŒR
					return WarStandPoint::attack_enngun;
				}
				// 0‚©4‚È‚ç‰‡ŒR‚Ìç”õŒRB0‚ª‰‡ŒR‚ÌeŒR‚ÅA4‚ª‰‡ŒR‚ÌqŒR
				else if ( nb6war_joint_daimyo_hex_group_info[ix].group == 0 || nb6war_joint_daimyo_hex_group_info[ix].group == 4 ) {
					// ‰‡ŒR‚Ìç”õŒR
					return WarStandPoint::defend_enngun;
				}
			}
		}
	}

	return WarStandPoint::unknown; // •s–¾
}

// ŠY“–‚Ì•«‚Ìíê‚É‚¨‚¯‚é—§ê‚ğ•Ô‚·(•s–¾=0, ’¼ÚU‚ß‚é‘¤=1, ’¼Úç‚é‘¤=2, U‚ß‚é‘¤‚Ì‰‡ŒR=3, ç‚é‘¤‚Ì‰‡ŒR=4, ’†—§ŒR=5)
int GetWarStandPointTheBushou(int iBushouID) {

	// ”ÍˆÍƒ`ƒFƒbƒN
	if ( !(0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) ) {
		return WarStandPoint::unknown;
	}

	// ŠY“–•«‚ÌŒR’cID“üè
	int iGundanID = nb6bushouref[iBushouID].gundan-1;
	if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
		return GetWarStandPointTheGundan(iGundanID);
	}

	return WarStandPoint::unknown;
}

// Œ»İ‚Ìí‚É‚¨‚¢‚ÄAw’èŒR’c‚Ì‘‘å«(ŒR’c’·‚â‘å–¼‚Ì‚±‚Æ‚Å‚Í‚È‚­Aíê‚É‚¨‚¢‚Ä‚ÌŠY“–ŒR’c‚Ì‘‘å«B
int GetWarSoudaishoTheGundan(int iGundanID) {
	if ( !(0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) ) {
		return 0xFFFF;
	}

	if ( Is_FieldWar() || Is_CastleWar() ) {
		// Q‰Á‘å–¼”Ô†‚ğŒŸõ‚µ‚ÄAUŒ‚‘¤‚ÌƒOƒ‹[ƒvIDAç”õ‘¤‚ÌƒOƒ‹[ƒvID‚ğo‚·B
		for ( int ix=0; ix < GAMEDATASTRUCT_WAR_JOINT_DAIMYO_HEX_GROUP_INFO_NUM; ix++ ) {

			int iJointGundanID = (int)nb6war_joint_daimyo_hex_group_info[ix].gundan-1;

			// w’è‚ÌŒR’c‚ğŒ©‚Â‚¯‚½B
			if ( iGundanID == iJointGundanID ) {
				// •«”Ô†(+1‚Ì•û)‚Ì”ÍˆÍ‚Éû‚Ü‚Á‚Ä‚¢‚éB
				if ( 0 < nb6war_joint_daimyo_hex_group_info[ix].soudaisho && nb6war_joint_daimyo_hex_group_info[ix].soudaisho <= GAMEDATASTRUCT_BUSHOU_NUM ) {
					return nb6war_joint_daimyo_hex_group_info[ix].soudaisho-1;
				// •«”Ô†‚Ì”ÍˆÍ‚Éû‚Ü‚Á‚Ä‚¢‚È‚¢‚Æ‚¢‚¤‚±‚Æ‚ÍA‘å–¼‚âŒR’c‚ÍŒˆ‚Ü‚Á‚Ä‚¢‚é‚ªA‘‘å«‚ªŒˆ‚Ü‚Á‚Ä‚¢‚È‚¢Bƒ†ƒjƒbƒg”z’u‘O‚¾‚Æ‘‘å«‚ªŒˆ‚Ü‚ç‚È‚¢‚Ì‚ÅA‚±‚Ì‚æ‚¤‚È’l‚Æ‚È‚éB
				} else {
					return 0xFFFF;
				}
			}
		}
	}

	return 0xFFFF;
}

// –ìí‚È‚Ì‚©Uéí“¬‚È‚Ì‚©H
// Œ»İ‰æ–Ê‚Éo‚Ä‚éíê‚ÍA–ìí
extern int iNextTurnCastleID;
// Œ»İ‚Ìíê‚É‚¨‚¯‚éƒ^[ƒ“Œo‰ß”BíêˆÈŠO‚Å‚Í-1‚Æ‚È‚Á‚Ä‚¢‚éB
extern int iCurIncrementTurnInBattle;
bool Is_FieldWar() {
	return (iCurIncrementTurnInBattle >= 0) && iNextTurnCastleID == 0xFFFF && CommonGlobalHandle::isBattleMode;
}

// Œ»İ‰æ–Ê‚Éo‚Ä‚éíê‚ÍAUéí
bool Is_CastleWar() {
	return (iCurIncrementTurnInBattle >= 0) && ( 0 <= iNextTurnCastleID-1 && iNextTurnCastleID-1 < GAMEDATASTRUCT_CASTLE_NUM ) && CommonGlobalHandle::isBattleMode; 
}


// •\¦’†‚ÌUéí‚Å‰½ƒ^[ƒ“–Ú‚©B‚P`‚RDUéí‚Å‚È‚¢ê‡‚Í-1D
int getCastleTurnInBattle() {

	if ( Is_CastleWar() ) {

		byte *turn = (byte *)GAMEDATASTRUCT_HOWMUCHTURNPROCEED_INCASTLEBATTLE;
		return *turn + 1;

	} else {
		return -1;
	}
}

// ŠO•”QÆ—p
int WINAPI Extern_getCastleTurnInBattle() {
	return getCastleTurnInBattle();
}

//--------------

// w’è•«‚ªíê‚É‚¨‚¢‚ÄAs“®Ï‚İ‚©‚Ç‚¤‚©
BOOL IsActionEndInBattle(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		for ( int i=0; i < GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_NUM; i++ ) {
			if ( nb6bushou_hex_unit_info[i].bushou == iBushouID+1 ) {
				if ( nb6bushou_hex_unit_info[i].bEnd == 0x2 ) { // s“®Ï‚Å‚ ‚éB
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


// ‘ÎÛ‚Ì•«‚ª¬—‚µ‚Ä‚¢‚é‚©B
int GetKonranStatus(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		for ( int i=0; i < GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_NUM; i++ ) {
			if ( nb6bushou_hex_unit_info[i].bushou == iBushouID+1 ) {
				return nb6bushou_hex_unit_info[i].konran; 
			}
		}
	}
	return KONRAN_STATUS::–³‚µ;
}

// ‘ÎÛ‚Ì•«‚ğ¬—‚³‚¹‚éB(•`‰æ‚ª”º‚Á‚Ä‚¢‚È‚¢)
BOOL SetKonranStatus(int iBushouID, int iKonranStatus ) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		for ( int i=0; i < GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_NUM; i++ ) {
			if ( nb6bushou_hex_unit_info[i].bushou == iBushouID+1 ) {
				nb6bushou_hex_unit_info[i].konran = iKonranStatus;
				// ¬—E‘å¬—E’§”­ ‚Ìó‘Ô‚É‚È‚Á‚½‚Ì‚È‚çc
				if ( iKonranStatus > 0 ) {
					nb6bushou_hex_unit_info[i].bEnd   = 0x2; // s“®Ï
				}
				FuncBattleUnitStatusRepaint(iBushouID); // •`‰æXV
				return TRUE;
			}
		}
	}
	return FALSE;
}

// w’è•«‚ğs“®Ï‚İ‚Æ‚·‚é
extern int arrRemainMoveCountAllBushou[GAMEDATASTRUCT_BUSHOU_NUM];
void setIsActionEndInBattle(int iBushouID, int isEnd=TRUE) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		for ( int i=0; i < GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_NUM; i++ ) {
			if ( nb6bushou_hex_unit_info[i].bushou == iBushouID+1 ) {
				if ( isEnd ) {
					nb6bushou_hex_unit_info[i].bEnd = 0x2;  // s“®Ï‚Æ‚·‚éB
					arrRemainMoveCountAllBushou[iBushouID] = 0;
				} else {
					nb6bushou_hex_unit_info[i].bEnd = 0;  // –¢s“®‚Æ‚·‚éB
				}
				break;
			}
		}
	}
}


//--------------MAIN‚ÌƒOƒŠƒbƒhŒn

// ƒOƒŠƒbƒhƒ|ƒWƒVƒ‡ƒ“ƒNƒ‰ƒX
GRID_POSITION::GRID_POSITION(int _x,int _y) {
	x=_x,
	y=_y;

	// Še—×Úé‚Í–³‚¢ó‘Ô‚Å‰Šú‰»
	nw= 0xFFFF;
	no= 0xFFFF;
	ne= 0xFFFF;
	we= 0xFFFF;
	ea= 0xFFFF;
	sw= 0xFFFF;
	so= 0xFFFF;
	se= 0xFFFF;

}

// ƒOƒŠƒbƒhÀ•W“¯u‚ÌŒ¸ZBÀ•W‚`¨À•W‚a‚ÌƒxƒNƒgƒ‹‚ğ‹‚ß‚é‚Ì‚ÉA’ÊíA‚a|‚`‚ğ‚·‚é‚¾‚ë‚¤‚©‚çB
GRID_VECTOR operator-(GRID_POSITION &p1, GRID_POSITION &p2) {
	GRID_VECTOR p;
	p.x = p1.x - p2.x;
	p.y = p1.y - p2.y;
	return p;
}

// é‚h‚c‚©‚çAƒƒCƒ“ƒOƒŠƒbƒhã‚Ìƒ|ƒWƒVƒ‡ƒ“‚ğ“¾‚éB¶ãƒOƒŠƒbƒh‚ª0,0Bmapdataobgk‚ÌƒOƒŠƒbƒh‚ğŒ©‚ê‚Î‚í‚©‚è‚â‚·‚¢B
// ‚P‚Â‚ÌƒOƒŠƒbƒh‚É‚ÍA‚P‚Â‚Ìé‚µ‚©‚È‚¢B
GRID_POSITION getCastlePosInMainGrid(int iCastleID) {
	GRID_POSITION pos(0, 0);
	if ( 0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM ) {
		int iGridX = (nb6castlemax[iCastleID].x + 1) / 5; // ‚OƒIƒŠƒWƒ“‚È‚Ì‚Å{‚P‚µ‚Ä‚T‚ÅŠ„‚éB‚w•ûŒü‚Í‚P‚Â‚ÌƒOƒŠƒbƒh‚Ì’†‚É‚T‚Â‚ÌêŠ‚ª‚ ‚éB
		int iGridY = (nb6castlemax[iCastleID].y + 1) / 3; // ‚OƒIƒŠƒWƒ“‚È‚Ì‚Å{‚P‚µ‚Ä‚R‚ÅŠ„‚éB‚x•ûŒü‚Í‚P‚Â‚ÌƒOƒŠƒbƒh‚Ì’†‚É‚R‚Â‚ÌêŠ‚ª‚ ‚éB

		pos.x = iGridX;
		pos.y = iGridY;

		// —×Ú‚·‚éé‚Ì‚h‚c‚à“ü‚ê‚Ä‚¨‚­B
		for (int iCastleID = 0; iCastleID < GAMEDATASTRUCT_CASTLE_NUM; iCastleID++) {
			int iFX = (nb6castlemax[iCastleID].x + 1) / 5; // ‚OƒIƒŠƒWƒ“‚È‚Ì‚Å{‚P‚µ‚Ä‚T‚ÅŠ„‚éB‚w•ûŒü‚Í‚P‚Â‚ÌƒOƒŠƒbƒh‚Ì’†‚É‚T‚Â‚ÌêŠ‚ª‚ ‚éB
			int iFY = (nb6castlemax[iCastleID].y + 1) / 3; // ‚OƒIƒŠƒWƒ“‚È‚Ì‚Å{‚P‚µ‚Ä‚R‚ÅŠ„‚éB‚x•ûŒü‚Í‚P‚Â‚ÌƒOƒŠƒbƒh‚Ì’†‚É‚R‚Â‚ÌêŠ‚ª‚ ‚éB

			if ( iFX - pos.x == -1 && iFY - pos.y == -1 ) { // –k¼
				pos.nw = iCastleID + 1;
			} else if ( iFX - pos.x == 0 && iFY - pos.y == -1 ) { // –k
				pos.no = iCastleID + 1;
			} else if ( iFX - pos.x == 1 && iFY - pos.y == -1 ) { // –k“Œ
				pos.ne = iCastleID + 1;
			} else if ( iFX - pos.x == -1 && iFY - pos.y == 0 ) { // ¼
				pos.we = iCastleID + 1;
			} else if ( iFX - pos.x == 1 && iFY - pos.y == 0 ) { // “Œ
				pos.ea = iCastleID + 1;
			} else if ( iFX - pos.x == -1 && iFY - pos.y == 1 ) { // “ì¼
				pos.sw = iCastleID + 1;
			} else if ( iFX - pos.x == 0 && iFY - pos.y == 1 ) { // “ì
				pos.so = iCastleID + 1;
			} else if ( iFX - pos.x == 1 && iFY - pos.y == 1 ) { // “ì“Œ
				pos.se = iCastleID + 1;
			}
		}

		return pos;
	}
	return pos;
}

// w’è‚Ìé‚É‘Î‚µ‚Ä—×Ú‚µ‚Ä‚¢‚éé‚ÌƒŠƒXƒg‚ğ“¾‚é
vector<int> getRinsetsuCastles(int iCastleID) {
	// ‚Ü‚¸‚ÍAw’èé‚ÌƒOƒŠƒbƒh‚ÌêŠ‚ğ“¾‚é
	GRID_POSITION CenterPos = getCastlePosInMainGrid(iCastleID);

	vector<int> RinsetsuCastles;

	// —×Ú‚µ‚Ä‚¢‚éé‚ğƒxƒNƒgƒ‹‚É’Ç‰Á‚µ‚Ä‚ä‚­
	for ( int iTargetID = 0; iTargetID < GAMEDATASTRUCT_CASTLE_NUM; iTargetID++ ) {
		// ©•ª©g‚ÍƒpƒX
		if ( iCastleID == iTargetID ) {
			continue;
		}

		GRID_POSITION TargetPos = getCastlePosInMainGrid(iTargetID);
		// —×Ú‚µ‚Ä‚¢‚éB
		if ( abs( CenterPos.x - TargetPos.x ) <= 1 && abs( CenterPos.y - TargetPos.y) <= 1) {
			RinsetsuCastles.push_back( iTargetID );
		}
	}
	return RinsetsuCastles;
}


char* nb6kanitypelist[GAMEDATASTRUCT_KANITYPE_NUM] = {
	"³ˆêˆÊ",
	"]ˆêˆÊ",
	"³“ñˆÊ",
	"]“ñˆÊ",
	"³OˆÊ",
	"]OˆÊ",
	"³lˆÊã",
	"³lˆÊ‰º",
	"]lˆÊã",
	"]lˆÊ‰º",
	"³ŒÜˆÊã",
	"³ŒÜˆÊ‰º",
	"]ŒÜˆÊã",
	"]ŒÜˆÊ‰º",
	"³˜ZˆÊã",
	"³˜ZˆÊ‰º",
	"]˜ZˆÊã",
	"]˜ZˆÊ‰º",
	"³µˆÊã",
	"³µˆÊ‰º",
	"]µˆÊã",
	"]µˆÊ‰º",
	"³”ªˆÊã",
};

char* getKaniTypeName(byte type) {
	return nb6kanitypelist[type];
}




// –ğE‚Ìí—Ş‚Ì•¶š—ñ‚Ö‚Ìƒ|ƒCƒ“ƒ^‚ÌƒAƒhƒŒƒXˆê—— 
int GAMEDATASTRUCT_POSITIONTYPELIST_POINTER[GAMEDATASTRUCT_POSITIONTYPE_NUM] = {};

// –ğEƒ^ƒCƒv‚ÌID‚©‚ç•¶š—ñ‚ğ“¾‚é
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

// ‘å–¼ID‚©‚ç³‹K‰»‚³‚ê‚½–ğEID‚ğ“¾‚é
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

// ŒR’cID‚©‚ç³‹K‰»‚³‚ê‚½–ğEID‚ğ“¾‚é
int getNormalizedGundanPosition(int iGundanID) {
	return nb6gundan[iGundanID].position;
}

bool tryChangeDaimyoYakusyoku(int iDaimyoID, int iYakusyokuValue ) {
	if ( 0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM) {

		if ( –ğE::–³‚µ <= iYakusyokuValue && iYakusyokuValue <= –ğE::‰HB’T‘è ) {

			int DenormalizeyakusyokuValue = DeNormalizeDaimyoYakusyokuValue(iYakusyokuValue);
			
			// ‘å–¼‚Å–Ú“I‚ÌID‚Ì–ğE‚ğ‚Á‚Ä‚¢‚él
			for (int iDID = 0; iDID < GAMEDATASTRUCT_DAIMYO_NUM; iDID++) {
				int iDaimyoYakusyokuID = getNormalizedDaimyoPosition(iDID);
				// ‚»‚Ì–ğE‚ğ‚Á‚Ä‚¢‚ê‚ÎA‚Í‚­’D
				if (iDaimyoYakusyokuID == iYakusyokuValue) {
					nb6daimyo[iDID].position = 0;
				}
			}

			// ŒR’c‚Å–Ú“I‚ÌID‚Ì–ğE‚ğ‚Á‚Ä‚¢‚él
			for (int iGID = 0; iGID < GAMEDATASTRUCT_GUNDAN_NUM; iGID++) {
				int iGundanYakusyokuID = getNormalizedGundanPosition(iGID);
				// ‚»‚Ì–ğE‚ğ‚Á‚Ä‚¢‚ê‚ÎA‚Í‚­’D
				if (iGundanYakusyokuID == iYakusyokuValue) {
					nb6gundan[iGID].position = 0;
				}
			}
		
			// ©•ª‚É‘ÎÛ‚Ì–ğE‚ğ‚Â‚¯‚éB
			nb6daimyo[iDaimyoID].position = DenormalizeyakusyokuValue;

			return true;
		}
	}
		
	return false;
}


bool tryChangeGundanYakusyoku(int iGundanID, int iYakusyokuValue ) {

	if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM) {

		// ‘å–¼©g‚È‚çA•Êˆ—
		if (nb6gundan[iGundanID].number == 1) {
			int iDaimyoID = nb6gundan[iGundanID].attach-1;
			tryChangeDaimyoYakusyoku( iDaimyoID, iYakusyokuValue );
		}

		if ( –ğE::–³‚µ <= iYakusyokuValue && iYakusyokuValue <= –ğE::‰HB’T‘è ) {

			// ‘å–¼‚Å–Ú“I‚ÌID‚Ì–ğE‚ğ‚Á‚Ä‚¢‚él
			for (int iDID = 0; iDID < GAMEDATASTRUCT_DAIMYO_NUM; iDID++) {
				int iDaimyoYakusyokuID = getNormalizedDaimyoPosition(iDID);
				// ‚»‚Ì–ğE‚ğ‚Á‚Ä‚¢‚ê‚ÎA‚Í‚­’D
				if (iDaimyoYakusyokuID == iYakusyokuValue) {
					nb6daimyo[iDID].position = 0;
				}
			}

			// ŒR’c‚Å–Ú“I‚ÌID‚Ì–ğE‚ğ‚Á‚Ä‚¢‚él
			for (int iGID = 0; iGID < GAMEDATASTRUCT_GUNDAN_NUM; iGID++) {
				int iGundanYakusyokuID = getNormalizedGundanPosition(iGID);
				// ‚»‚Ì–ğE‚ğ‚Á‚Ä‚¢‚ê‚ÎA‚Í‚­’D
				if (iGundanYakusyokuID == iYakusyokuValue) {
					nb6gundan[iGID].position = 0;
				}
			}

			// ©•ª‚É‘ÎÛ‚Ì–ğE‚ğ‚Â‚¯‚éB
			nb6gundan[iGundanID].position = iYakusyokuValue;

			return true;
		}
	}

	return false;
}



/// Œ»İ(‚»‚ÌuŠÔ)A“VãÄ‹L‚É‘¶İ‚·‚éŒR’c‚ÌuŒR’c”Ô†y”z—ñ—pzv‚ğƒŠƒXƒg‚Å“¾‚éB
vector<int> getValidGundanList() {
	vector<int> list;

	// ŠY“–‚Ì‘å–¼‚h‚c‚É‘®‚·‚éŒR’c‚ğƒŠƒXƒg‚É‰Á‚¦‚é
	for ( int iGundanID=0; iGundanID < GAMEDATASTRUCT_GUNDAN_NUM; iGundanID++ ) {
		if ( nb6gundan[iGundanID].leader !=  0xFFFF ) {
			list.push_back(iGundanID);
		}
	}
	
	return list;
}



// ŒR’c‚ÉŠ‘®‚·‚é•«ƒŠƒXƒg‚ğ“¾‚é
vector<int> getGundanBushouList(int iGundanID) {
	// ‚»‚êˆÈŠO‚Í‰½‚à“ü‚ê‚¸‚É•Ô‚·B
	vector<int> list;

	for (int i = 0; i<GAMEDATASTRUCT_BUSHOU_NUM; i++) {
		if (nb6bushouref[i].gundan - 1 == iGundanID) {
			if (nb6bushouname[i].State <= 2) { // ‘å–¼, ŒR’c’·, Œ»–ğ ‚ÅƒJƒEƒ“ƒg
				list.push_back(i);
			}
		}
	}
	return list;
}

// ‚Æ‚ ‚é‘å–¼¨—Í‚Ì“¡ŒR’cƒŠƒXƒg‚ğ“¾‚éB
vector<int> getGovernedGundanList(int iDaimyoID) {
	// ‚»‚êˆÈŠO‚Í‰½‚à“ü‚ê‚¸‚É•Ô‚·B
	vector<int> list;

	if ( 0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM ) {

		// ŠY“–‚Ì‘å–¼‚h‚c‚É‘®‚·‚éŒR’c‚ğƒŠƒXƒg‚É‰Á‚¦‚é
		for ( int iGundanID=0; iGundanID<GAMEDATASTRUCT_GUNDAN_NUM; iGundanID++ ) {
			if ( nb6gundan[iGundanID].attach-1 ==  iDaimyoID) {
				list.push_back(iGundanID);
			}
		}
	}

	return list;
}

// ŒR’c‚ÉŠ‘®‚·‚é•º”‚ğ“¾‚é
int getGundanHeisuCnt(int iGundanID) {
	int iSoldierCnt = 0;
	for ( int i=0; i<GAMEDATASTRUCT_BUSHOU_NUM; i++) {
		if ( nb6bushouref[i].gundan-1 == iGundanID ) {
			if ( nb6bushouname[i].State <= 2 ) { // ‘å–¼, ŒR’c’·, Œ»–ğ ‚ÅƒJƒEƒ“ƒg
				iSoldierCnt += nb6bushouref[i].soldier;
			}
		}
	}
	return iSoldierCnt;
}

// é‚ÉŠ‘®‚·‚é•º”‚ğ“¾‚é
int getCastleHeisuCnt(int iCastleID) {
	int iSoldierCnt = 0;
	for ( int i=0; i<GAMEDATASTRUCT_BUSHOU_NUM; i++) {
		if ( nb6bushouref[i].castle-1 == iCastleID ) {
			if ( nb6bushouname[i].State <= 2 ) { // ‘å–¼, ŒR’c’·, Œ»–ğ ‚ÅƒJƒEƒ“ƒg
				iSoldierCnt += nb6bushouref[i].soldier;
			}
		}
	}
	return iSoldierCnt;
}



// ‚Æ‚ ‚é‘å–¼¨—Í‚Ì“¡ŒR’cŠ„‚è‚ ‚Ä”Ô†ƒŠƒXƒg‚ğ“¾‚éB
vector<int> getGovernedGundanWariateNumList(int iDaimyoID) {
	// ‚»‚êˆÈŠO‚Í‰½‚à“ü‚ê‚¸‚É•Ô‚·B
	vector<int> list;

	if ( 0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM ) {

		// ŠY“–‚Ì‘å–¼‚h‚c‚É‘®‚·‚éŒR’c‚ğƒŠƒXƒg‚É‰Á‚¦‚é
		for ( int iGundanID=0; iGundanID<GAMEDATASTRUCT_GUNDAN_NUM; iGundanID++ ) {
			if ( nb6gundan[iGundanID].attach-1 ==  iDaimyoID) {
				list.push_back(nb6gundan[iGundanID].number);
			}
		}
	}
	
	std::sort(list.begin(),list.end());//¸‡ƒ\[ƒg

	return list;
}



// ‚Æ‚ ‚é‘å–¼”z‰º‚ÌŒR’c‰º‚É‚ ‚éŒR’cu–¢vŠ„“–”Ô†‚ÌƒŠƒXƒg‚ğ“¾‚éB
vector<int> getNotGovernedGundanWariateNumList(int iDaimyoID) {

	// Š„‚è“–‚Ä‚ç‚ê‚Ä‚é‚à‚Ì‚ÉA‚P‚ğ“ü‚ê‚é
	int wariate_array[8] = {0,0,0,0,0,0,0,0};
	
	if ( 0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM ) {

		// ŠY“–‚Ì‘å–¼‚h‚c‚É‘®‚·‚éŒR’c‚ğƒŠƒXƒg‚É‰Á‚¦‚é
		for ( int iGundanID=0; iGundanID<GAMEDATASTRUCT_GUNDAN_NUM; iGundanID++ ) {
			if ( nb6gundan[iGundanID].attach-1 ==  iDaimyoID) {
				int g =  nb6gundan[iGundanID].number;
				if ( 1 <= g && g <= 8 ) {
					// Š„‚è“–‚Ä‚ç‚ê‚Ä‚é‚à‚Ì‚ÉA‚P‚ğ“ü‚ê‚é
					wariate_array[g-1] = 1;
				}
			}
		}
	}

	vector<int> list;
	for ( int ix=0; ix<8; ix++ ) {
		if (wariate_array[ix] == 0) { // ŒR’cŠ„“–‚ª‚È‚¯‚ê‚Î
			list.push_back(ix+1);
		}
	}

	std::sort(list.begin(),list.end());//¸‡ƒ\[ƒg

	return list;
}

// ŒR’c”C–½”Ô†‚Ì•ÏX(“ü‘Ö)‚ğ‚İ‚éB¸”s‚·‚ê‚Îfalse
bool tryChangeGundanWariateNum(int iGundanID, int iNewWariateNum) {

	// ‘æ‚PŒR’c‚Í‘å–¼©gB•ÏX‚Í–³—B
	if (nb6gundan[iGundanID].number == 1) {
		return false;
	}
	
	if ( !( 1<= iNewWariateNum && iNewWariateNum <= 8) ) {
		return false;
	}

	// ©•ª‚ÌŒR’c‚Ì‘å–¼‚ÌID
	int iDaimyoID = nb6gundan[iGundanID].attach - 1;

	// ‘å–¼Ÿ€‰º‚Å‹ó‚«ƒXƒƒbƒg‚Æ‚È‚Á‚Ä‚¢‚é”C–½”Ô†ˆê——
	if (0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM) {
		vector<int> sameDaimyoGundanList = getGovernedGundanList(iDaimyoID);
		int iOldNinmeiNum = nb6gundan[iGundanID].number; // ¡‚Ü‚Å‚Ì”C–½”Ô†

		for( vector<int>::iterator it = sameDaimyoGundanList.begin(); it != sameDaimyoGundanList.end(); it++ ) {
			int iGID = *it;

			// ‚·‚Å‚É•ÊŒR’c‚Å—˜—p‚³‚ê‚Ä‚¢‚é‚È‚ç‚Î
			if (nb6gundan[iGID].number == iNewWariateNum) {
				nb6gundan[iGID].number = iOldNinmeiNum;
				break;
			}
		}

		// ©•ª‚Íw’è‚ÌŒR’c”Ô†‚Æ‚È‚éB
		nb6gundan[iGundanID].number = iNewWariateNum;

		return true;
	}

	return false;

}


// w’è‚ÌŒR’c‚ÌŠ‘®‚·‚é‘å–¼‚ğ¨—Í‚ğ‰z‚¦‚ÄA•ÏX‚·‚éB
bool setGundanReAttachDaimyo(int iGundanID, int iNewDaimyoID) {

	// V‘å–¼‚ª³“–‚Å‚Í‚È‚¢B
	if ( ! (0 <= iNewDaimyoID && iNewDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM) ) {
		return false;
	}

	// ŒR’c‚ª‘S•”–„‚Ü‚Á‚Ä‚¢‚é‚È‚ç‚ÎA
	vector<int> glist = getGovernedGundanWariateNumList(iNewDaimyoID);
	if ( glist.size() == 8 ) { // ŒR’c‚Í‘S•”–„‚Ü‚Á‚Ä‚¢‚é
		// ‰½‚à‚Å‚«‚È‚¢
		return false;
	}

	if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {

		// w’è‚ÌŒR’c‚ªŒ»İŠ‘®‚µ‚Ä‚¢‚é‘å–¼”Ô†‚ğ‹‚ß‚é
		int iDaimyoID = nb6gundan[iGundanID].attach-1;
		// VŠ‘®æ‚Ì‘å–¼‚ªA¡Š‘®‚µ‚Ä‚¢‚é‘å–¼‚Å‚ ‚ê‚ÎA‰½‚à‚·‚é‚±‚Æ‚Í‚È‚¢B
		if ( iDaimyoID == iNewDaimyoID) {
			return true;
		}

		// ŒR’cŠ„‚è“–‚Ä”Ô†‚ª‚P”Ô‚Í‘å–¼©gB‚±‚ê‚ÍŒÅ’èB‚±‚Ìê‡A‰½‚à‚¹‚¸¸”s‚Æ‚·‚éB
		if ( nb6gundan[iGundanID].number == 1 ) {
			return false;
		}

		// Œ»İAV‘å–¼‚ÉŠ‘®‚µ‚Ä‚¢‚ÄAnext‚ª0xFFFF‚É‚È‚Á‚Ä‚¢‚éŒR’c‚ªA‚Ô‚ç‰º‚ª‚é‚×‚«ƒŠƒ“ƒNƒŠƒXƒg‚ÌÅŒã‚¾B
		// ‚Ô‚ç‰º‚ª‚é‚±‚Æ‚ªo—ˆ‚é‚©A–‘O‚Éƒ`ƒFƒbƒN‚µ‚Ä‚¨‚­B
		int iLinkEndGundanID = -1;
		for ( int iCurGundanID=0; iCurGundanID < GAMEDATASTRUCT_GUNDAN_NUM; iCurGundanID++ ) {
			// 
			int iCurDaimyoID = nb6gundan[iCurGundanID].attach-1;

			// w’è‚ÌV‘å–¼‚ÉŠ‘®‚µ‚Ä‚¢‚é‚È‚ç‚ÎA
			if ( iCurDaimyoID == iNewDaimyoID) {
				// Ÿ‚ÌŒR’c‚ª–³‚¢‚Ì‚Å‚ ‚ê‚ÎA‚»‚¢‚Â‚ªÅIŒR’c”Ô†
				if ( nb6gundan[iCurGundanID].next == 0xFFFF ) {
					iLinkEndGundanID = iCurGundanID+1;
				}
				
			}

		}

		// ‚»‚ê‚ª”ÍˆÍ‚É“ü‚Á‚Ä‚¢‚È‚¢‚È‚ç‚ÎAV‘å–¼‚ÌŒR’cƒŠƒ“ƒNƒŠƒXƒg‚ÍA‰ü‘¢‚©‰½‚©‚µ‚ç‚È‚¢‚ªî•ñ‚ª‰ó‚ê‚Ä‚¢‚éB
		// ˆ—I—¹
		if ( ! (0 < iLinkEndGundanID && iLinkEndGundanID <= GAMEDATASTRUCT_GUNDAN_NUM ) ) {
			return false;
		}

		/* @©ŒR’c
		      «
		   ›-œ-›-›

		   ‚Æ‚¢‚¤‚Ì‚ğ

		   ›-›-›

		   ‚Æ‚·‚éB
		*/

		int iPrevID = nb6gundan[iGundanID].prev;
		int iNextID = nb6gundan[iGundanID].next;

		// ©•ª‚Ìprev‚Í—LŒø‚ÈŒR’c‚¾B
		if ( 0 < iPrevID && iPrevID <= GAMEDATASTRUCT_GUNDAN_NUM ) {

			// ©•ª‚ªæ“ª‚Å‚Í‚È‚¢ŒR’c‚ÅA
			// ‚P‚Â‘O‚ÌŒR’c‚ÌŸ‚ÌƒŠƒ“ƒNæ‚ª©ŒR’c‚Ìê‡
			if ( iPrevID != 0xFFFF && nb6gundan[(iPrevID-1)].next == iGundanID+1 ) {
				// ©ŒR’c‚Å‚Í‚È‚­AŸ‚ÌŒR’c‚Ö‚ÆŒq‚¬‚©‚¦‚éB
				nb6gundan[(iPrevID-1)].next = iNextID;
			}
		}

		// ©•ª‚Ìnext‚Í—LŒø‚ÈŒR’c‚¾B
		if ( 0 < iNextID && iNextID <= GAMEDATASTRUCT_GUNDAN_NUM ) {
			// ‚P‚ÂŒã‚ÌŒR’c‚Ì‘O‚ÌƒŠƒ“ƒNæ‚ª©ŒR’c‚Ìê‡
			if ( iNextID != 0xFFFF && nb6gundan[(iNextID-1)].prev == iGundanID+1 ) {
				// ©ŒR’c‚Å‚Í‚È‚­A‘O‚ÌŒR’c‚Ö‚ÆŒq‚¬‚©‚¦‚éB
				nb6gundan[(iNextID-1)].prev = iPrevID;
			}
		}


		/* @
		     
		   ›-›

		   ‚Æ‚¢‚¤‚Ì‚ğ

		   ›-›-œ

		   ‚Æ‚·‚éB
		*/



		nb6gundan[iLinkEndGundanID-1].next = iGundanID+1; // ƒŠƒ“ƒNƒŠƒXƒg‚ÌÅŒã‚ÌŒR’c‚Ìnext‚ğ©ŒR’c‚É

		nb6gundan[iGundanID].prev = iLinkEndGundanID; // ©ŒR’c‚Ì‘O‚ÌŒR’c‚ÍAŒR’c‚ÌƒŠƒ“ƒNƒŠƒXƒg‚ÌÅŒã‚ÌŒR’c

		// ©ŒR’c‚ÍAŒR’cƒŠƒXƒg‚Ìˆê”ÔÅŒã‚É‚­‚Á‚Â‚­B©ŒR’c‚Ìnext‚Í‹‚È‚¢
		nb6gundan[iGundanID].next = 0xFFFF; // 

		// ƒƒ‚ƒŠ‘‚«Š·‚¦
		{
			nb6gundan[iGundanID].number = 8; // Œ³‚Ì”Ô†‚Æ‚¢‚¤‚í‚¯‚É‚Í‚¢‚©‚È‚¢‚Ì‚ÅA–œ‚ªˆê‚Ì‚½‚ß‚ÉA‚Æ‚è‚ ‚¦‚¸AÅ‚à‚‚¢Šm—¦‚Å‹ó‚¢‚Ä‚éƒZ[ƒt‚È‚à‚Ì‚ğæ‚é

			vector<int> milist = getNotGovernedGundanWariateNumList(iNewDaimyoID);
			if ( milist.size() > 0 ) {
				nb6gundan[iGundanID].number = milist[0]; // ‹ó”Ô†‚ÌÅ‰‚Ì‚à‚Ì‚ğ•ú‚è‚Ş
			}

			nb6gundan[iGundanID].attach = iNewDaimyoID+1;
		}

		for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {
			// w’è‚ÌŒR’c‚ÉŠ‘®‚µ‚Ä‚¢‚é•«‚ÍA‘S‚ÄAV‘å–¼‚ÉŠ‘®‚ğ•ÏX‚·‚éB
			if ( nb6bushouref[iBushouID].gundan== iGundanID+1 ) {
				nb6bushouref[iBushouID].attach = iNewDaimyoID+1;
				nb6bushouref[iBushouID].work = 0; // mŠ¯”N”ƒŠƒZƒbƒg
				nb6bushouref[iBushouID].rise = 0xFFFF; // “à‰æƒŠƒZƒbƒg
				nb6bushouref[iBushouID].bRise = 0; // “à‰ó‘Ô‚È‚µ
				nb6bushouref[iBushouID].grudge = 0; // ˆâ¦‚È‚µ
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
		OutputDebugStream("m_iDaimyoID‚ª”ÍˆÍŠO");
		return ret;
	}

	bool is_gundan_has_empty_slot = false;
	for (int iGundanID = 0; iGundanID < GAMEDATASTRUCT_GUNDAN_NUM; iGundanID++) {
		if (nb6gundan[iGundanID].attach == 0xFFFF && // Š‘®‘å–¼”Ô†
			nb6gundan[iGundanID].residence == 0xFFFF &&
			nb6gundan[iGundanID].next == 0xFFFF) {
			is_gundan_has_empty_slot = true;
			break;
		}
	}
	if (is_gundan_has_empty_slot == false) {
		OutputDebugStream("ŒR’cƒXƒƒbƒg‚ª‘S‚Ä–„‚Ü‚Á‚Ä‚¢‚é");
		return ret;
	}

	int iDaimyoBushouID = nb6daimyo[m_iDaimyoID].attach - 1; // ‘å–¼‚Ì•«”Ô†
	// €‚ñ‚ÅŠY“–ŒR’c‚Éƒ^[ƒ“‚ª‰ñ‚éŠÔ‚ÌŠëŒ¯‚Èƒ^ƒCƒ~ƒ“ƒOB‚±‚Ìƒ^ƒCƒ~ƒ“ƒO‚Í³”FÒ‚ª¶‘¶‚µ‚Ä‚¢‚È‚¢‚Ì‚ÅAVİ‚Å‚«‚È‚¢‚à‚Ì‚Æ‚·‚éB
	if (nb6bushouname[iDaimyoBushouID].battleDeath || (nb6bushouname[iDaimyoBushouID].State == 7)) { // €–S
		return ret;
	}

	// Š„‚è“–‚ÄŒó•â‚Í—v‚·‚é‚ÉA‚Ü‚¾Š„‚è“–‚Ä‚ç‚ê‚Ä‚¢‚È‚¢ŒR’c”Ô†‚Ì‚±‚Æ
	ret.vGundanIDList = getNotGovernedGundanWariateNumList(m_iDaimyoID);

	// w’è‚Ì‘å–¼‚ªx”z‚µ‚Ä‚¢‚é‘Sé‚ÌƒŠƒXƒg
	auto daimyoHasCastleList = getDaimyoHasCastleList(m_iDaimyoID);

	// Œó•â‚É‚·‚é‚Ì‚ÍAw’è‚Ì‘å–¼‰Æx”zé‚Ì‚¤‚¿Au‘å–¼v‚àuŒR’c’·v‚à‹‚È‚¢éB
	// ‘¦‚¿Au”ñ–{‹’v‚Ì‚à‚Ì‚ÉŒÀ‚é
	for each(int iCastleID in daimyoHasCastleList) {
		if (nb6castle[iCastleID].honjo == 0) { //–{é ‹é (0:”ñ–{‹’, ‘å–¼‚Ì–{é=1, ŒR’c’·‚Ì–{é=2);
			ret.vCastleIDList.push_back(iCastleID);
		}
	}

	for (int iBushouID = 0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {

		// ‚Ü‚¸‘ÎÛ‚Ì•«‚ÍAw’è‚Ì‘å–¼‰Æ‚ÌlŠÔ‚Å‚ ‚éB(‘å–¼ŠÜ‚Ş)
		if (nb6bushouref[iBushouID].attach - 1 == m_iDaimyoID) {

			// Œ»–ğ‚Å‚ ‚éB(=‘å–¼‚âŒR’c’·‚âƒqƒ‚Å‚Í‚È‚­A¶‘¶‚µA“oê‚µ‚Ä‚¢‚éˆê”Ê•«)
			if (nb6bushouname[iBushouID].State == 2) { // 2:Œ»–ğ
				// í€’†‚Å‚Í‚È‚¢
				if (!nb6bushouname[iBushouID].battleDeath) {
					ret.vBushouIDList.push_back(iBushouID);
				}
			}
		}
	}

	// ŒR’c‚ª‚·‚Å‚É‚W‚Â‚ ‚é
	if (ret.vGundanIDList.size() == 0) {
		ret.CanMake = FALSE;
		return ret;
	}

	// VŒR’c’·‚ğŒ}‚¦“ü‚ê‚é‚æ‚¤‚È—]‚Á‚½é‚Í‘¶İ‚µ‚È‚¢
	if (ret.vCastleIDList.size() == 0) {
		ret.CanMake = FALSE;
		return ret;
	}

	// ‚»‚à‚»‚àŒR’c’·‚É”C–½‚·‚×‚«‘ÎÛ‚Ìˆê”Ê•«‚ª‘¶İ‚µ‚È‚¢
	if (ret.vBushouIDList.size() == 0) {
		ret.CanMake = FALSE;
		return ret;
	}

	ret.CanMake = TRUE;

	return ret;
}


int Set_MakeGundan(int _m_iBushouID, int _m_iCastleID, MAKEGUNDANKOUHOINFO check) {
	if (check.CanMake == FALSE) {
		OutputDebugStream("MAKEGUNDANKOUHOINFO.CanMake ‚ª FALSE");
		return 0xFFFF;
	}

	// –¼‘O’·‚·‚¬
	auto& bushous = check.vBushouIDList;
	auto& castles = check.vCastleIDList;
	auto& gwariates = check.vGundanIDList;
	if (bushous.size() == 0 || castles.size() == 0 || gwariates.size() == 0){
		OutputDebugStream("MAKEGUNDANKOUHOINFO‚Ì’†gƒf[ƒ^‚ª•s³");
		return 0xFFFF;
	}

	auto bitr = std::find(bushous.begin(), bushous.end(), _m_iBushouID);
	// •«Œó•âˆÈŠO‚ğw’è‚µ‚Ä‚¢‚éB
	if (bitr == bushous.end()) {
		OutputDebugStream("_m_iBushouID ‚ª MAKEGUNDANKOUHOINFO.vBushouIDList ‚É‚È‚¢");
		return 0xFFFF;
	}

	auto citr = std::find(castles.begin(), castles.end(), _m_iCastleID);
	// éŒó•âˆÈŠO‚ğw’è‚µ‚Ä‚¢‚éB
	if (citr == castles.end()) {
		OutputDebugStream("_m_iCastleID ‚ª MAKEGUNDANKOUHOINFO..vCastleIDList ‚É‚È‚¢");
		return 0xFFFF;
	}

	// ‚ ‚«ƒXƒƒbƒg‚ğ’T‚·
	int iSelfGundanID = 0xFFFF;
	for (int iGundanID = 0; iGundanID < GAMEDATASTRUCT_GUNDAN_NUM; iGundanID++) {
		if (nb6gundan[iGundanID].attach == 0xFFFF && // Š‘®‘å–¼”Ô†
			nb6gundan[iGundanID].residence == 0xFFFF &&  // Š‘®‹é
			nb6gundan[iGundanID].next == 0xFFFF) {
			iSelfGundanID = iGundanID;
			break;
		}
	}

	// ‚È‚º‚©–³‚¢BˆÀ‘Sƒ`ƒFƒbƒN—pŒR’cVİŒó•âî‚Ìæ“¾‚ÌŒãASet_ŒR’cVİ‚ğŒÄ‚Ôƒ^ƒCƒ~ƒ“ƒO‚ª‚¸‚ê‚Ä‚¢‚é‚Ì‚¾‚ë‚¤B
	if (iSelfGundanID == 0xFFFF) {
		return 0xFFFF;
	}


	// ‚Ü‚¸‚ÍAŒR’cˆê——‚Ì’†‚©‚ç©•ª‚ª‚Ô‚ç‰º‚ª‚é‚×‚«ŒR’c‚ğ’T‚·B
	// ‚»‚ê‚Íu“¯‚¶‘å–¼”Ô†v‚ÉŠ‘®‚µ‚Ä‚¢‚È‚ª‚çunextv‚ª65535 ‚·‚È‚í‚¿A
	// ƒŠƒ“ƒNƒŠƒXƒg‚Ì––”ö‚ÉŠ‘®‚µ‚Ä‚¢‚éŒR’c‚Å‚ ‚éB
	int iPrevGundanID = 0xFFFF;
	for (int iGundanID = 0; iGundanID < GAMEDATASTRUCT_GUNDAN_NUM; iGundanID++) {
		// ‘å–¼‚ªˆê’v
		if (nb6gundan[iGundanID].attach == check.m_iDaimyoID + 1) { // Š‘®‘å–¼”Ô†
			// ‘ÎÛ‚Ì‘å–¼“à‚ÌŒR’cˆê——‚Ì‚¤‚¿A––”ö‚ÌŒR’c
			if (nb6gundan[iGundanID].next == 0xFFFF) {
				iPrevGundanID = iGundanID;
			}
		}
	}


	if (iPrevGundanID == 0xFFFF) {
		return 0xFFFF;
	}

	// ˆÈã‚ğƒNƒŠƒA‚µ‚Ä‚¢‚ê‚ÎA‚Ü‚Ÿ‘åä•v‚¾‚ë‚¤B

	// -------------------------------------------------------------------
	// ‚P‚Â‘O‚ÌŒR’c‚ÌŸ‚ğ©g‚Æ‚·‚é
	nb6gundan[iPrevGundanID].next = iSelfGundanID + 1;


	// -------------------------------------------------------------------
	// ©•ª‚Ì‘O‚ÌŒR’c‚Æ‚µ‚ÄAƒŠƒ“ƒNƒŠƒXƒg––”ö‚ÌÅŒã‚ÌŒR’c”Ô†‚ğ“ü‚ê‚é
	nb6gundan[iSelfGundanID].prev = iPrevGundanID + 1;
	// ©•ª©g‚ªƒŠƒ“ƒNƒŠƒXƒg‚ÌÅŒãB
	nb6gundan[iSelfGundanID].next = 0xFFFF;

	// ‹ó‚¢‚Ä‚¢‚é‚¤‚¿‚ÅAˆê”Ôá‚¢”Ô†‚ğ“ü‚ê‚éB
	nb6gundan[iSelfGundanID].number = gwariates[0];

	nb6gundan[iSelfGundanID].attach = check.m_iDaimyoID + 1;

	nb6gundan[iSelfGundanID].residence = _m_iCastleID + 1;

	nb6gundan[iSelfGundanID].leader = _m_iBushouID + 1; // ŒR’c’·y•«”Ô†z

	nb6gundan[iSelfGundanID].act = 50;

	tryChangeGundanYakusyoku(iSelfGundanID, 0);

	nb6gundan[iSelfGundanID].money = 1000;
	nb6gundan[iSelfGundanID].rise = 1000;

	nb6gundan[iSelfGundanID].gun = 100;
	nb6gundan[iSelfGundanID].horse = 100;



	// -------------------------------------------------------------------
	// é‚Ìî•ñ‚âƒŠƒ“ƒNƒŠƒXƒg‚ğ•ÏX‚·‚é
	nb6castle[_m_iCastleID].attach = iSelfGundanID + 1;

	nb6castle[_m_iCastleID].honjo = 2; //–{é ‹é (0:”ñ–{‹’, ‘å–¼‚Ì–{é=1, ŒR’c’·‚Ì–{é=2);

	// ©•ª©g(é)‚ğƒŠƒ“ƒNƒŠƒXƒg‚©‚çŠO‚·B
	// ‘O‚Ìé‚ÌuŸ‚Ìév‚ğA©g‚Å‚Í‚È‚­Au©g‚ÌŸ‚Ìév‚É‚·‚éB
	if (nb6castle[_m_iCastleID].prev != 0 && nb6castle[_m_iCastleID].prev != 0xFFFF) {
		int _prev = nb6castle[_m_iCastleID].prev - 1;
		nb6castle[_prev].next = nb6castle[_m_iCastleID].next;
	}

	// Ÿ‚Ìé‚Ìu‘O‚Ìév‚ğA©g‚Å‚Í‚È‚­Au©g‚Ì‘O‚Ìév‚É‚·‚éB
	if (nb6castle[_m_iCastleID].next != 0 && nb6castle[_m_iCastleID].next != 0xFFFF) {
		int _next = nb6castle[_m_iCastleID].next - 1;
		nb6castle[_next].prev = nb6castle[_m_iCastleID].prev;
	}


	// Vİ‚È‚Ì‚Å©•ª‚ªæ“ª‚ÍŠm’è
	nb6castle[_m_iCastleID].prev = 0;
	// Vİ‚È‚Ì‚Å©•ª‚ªÅŒã‚ÍŠm’è
	nb6castle[_m_iCastleID].next = 0xFFFF;

	// -------------------------------------------------------------------

	// -------------------------------------------------------------------
	// –{l‚Ìƒpƒ‰ƒ[ƒ^
	// ‘ÎÛ‚Ì•«‚ÌAŠY“–é‚Ö‚ÌˆÚ“®
	setBushouTransport(_m_iBushouID, _m_iCastleID);

	setGenekiToJyosyu(_m_iBushouID);

	nb6bushouname[_m_iBushouID].State = 1; // ŒR’c’·

	// ŠY“–‚Ìé‚É‹‚él’B‚ÌŠ‘®ŒR’c‚Ì‘‚«Š·‚¦
	for (int iBushouID = 0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {
		if (nb6bushouref[iBushouID].attach == check.m_iDaimyoID + 1) { // Š‘®‘å–¼”Ô†
			if (nb6bushouref[iBushouID].castle == _m_iCastleID + 1) {
				if (nb6bushouname[iBushouID].State == 1 || nb6bushouname[iBushouID].State == 2) { // ŒR’c’·‚à‚µ‚­‚ÍŒ»–ğ
					// í€’†‚Å‚Í‚È‚¢
					if (!nb6bushouname[iBushouID].battleDeath) {
						nb6bushouref[iBushouID].gundan = iSelfGundanID + 1;
					}
				}
			}
		}
	}

	return iSelfGundanID;
}





// ‰Æ–ä‚ğŒˆ‚ß‚Ä‚¢‚éƒAƒ‹ƒS(‚Æ‚¢‚¤‚©•R‚Ã‚¯ƒf[ƒ^)‚ª•s–¾‚È‚Ì‚ÅA
// ‚Æ‚è‚ ‚¦‚¸AŠÈˆÕ‚È‚à‚Ì‚Å‘ã—p
static int _getKamonAtBorn(int iBushouID) {

	if (!(0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM)) {
		OutputDebugStream("•«”Ô†‚ª”ÍˆÍŠO");
		return 0; // •Ô‚µ‚æ‚¤‚à‚È‚¢‚Ì‚ÅAˆê’UM’·‚Ì‚É‚µ‚Ä‚¨‚­B
	}

	// ³‹K‰Æ–ä‘S‘Ì‚Ì”z—ñ(’A‚µA
	map<int, int> regular_kamons_remain_list;
	for (int k = 0; k < GAMEDATASTRUCT_KAMON_REGULAR_NUM; k++) {
		regular_kamons_remain_list.insert(map<int, int>::value_type(k, k));
	}

	// ‚·‚Å‚É‘å–¼‚ªg‚Á‚Ä‚¢‚é‰Æ–ä‚ğA³‹K‚Ì—]‚è‰Æ–äƒŠƒXƒg‚©‚çœ‹‚·‚é‚±‚Æ‚ÅA–¢g—p‚Ì‚à‚Ì‚¾‚¯‚É‚·‚é
	for (int iDaimyoID = 0; iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM; iDaimyoID++) {

		int kamon = nb6daimyo[iDaimyoID].symbol;

		// ‘ÎÛ‚Ì‰Æ–ä”Ô†‚ªƒŠƒXƒg‚É‚È‚¢‚È‚çA(³‹K‰Æ–äˆÈŠO‚È‚ç‘Šè‚É‚µ‚È‚¢)
		regular_kamons_remain_list.erase(kamon);
	}

	int bloodNormalized = nb6bushouref[iBushouID].blood;
	if (bloodNormalized >= GAMEDATASTRUCT_KAMON_REGULAR_NUM) { bloodNormalized = GAMEDATASTRUCT_KAMON_REGULAR_NUM - 1; }

	// ³‹K‚Ì—]‚è‰Æ–äƒŠƒXƒg‚ÉA‘å–¼‚Ì•«‚ÌŒŒ‹Ø”Ô†‚Æ“¯‚¶‰Æ–ä”Ô†‚ª–¢g—p‚Å‚ ‚Ü‚Á‚Ä‚é‚È‚ç‚»‚ê‚É‚µ‚Ä‚¨‚­(ŒŒ‹Ø”Ô†‚Æ‰Æ–ä”Ô†‚Í72‚®‚ç‚¢‚Ü‚Å‚Íˆê’v‚µ‚Ä‚¢‚é‚½‚ß)
	if (regular_kamons_remain_list.count(bloodNormalized) > 0) {
		; // ‰½‚à‚µ‚È‚¢BbloodNormalized‚Ì‚»‚Ì‚Ü‚Ü‚Ì”’l‚ğÌ—p
	}
	// ³‹KƒŠƒXƒg“à‚ÉA‘å–¼‚Ì•«‚ÌŒŒ‹Ø”Ô†‚Æ“¯‚¶‰Æ–ä”Ô†‚ª–³‚¢ê‡‚ÍA³‹K‚Ì—]‚è‰Æ–äƒŠƒXƒg(‚Ì—]‚Á‚Ä‚é‚Ì)
	else {
		// ‚ñ`
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
		// ‚ ‚¢‚Ä‚¢‚é‚Æ‚¢‚¤ğŒ‚ÍˆÈ‰º‚Ì‚Q‚Â‚Å‚í‚©‚é‚¾‚ë‚¤B
		if (nb6daimyo[iDaimyoID].attach == 0xFFFF && // ‘å–¼‚Ì•«”Ô†
			nb6daimyo[iDaimyoID].gundan == 0xFFFF) {
			iSelfDaimyoID = iDaimyoID;
			break;
		}
	}

	// ‹ó‚«‚ª‚È‚©‚Á‚½‚çƒ_ƒ
	if (iSelfDaimyoID == 0xFFFF) {
		return 0xFFFF;
	}

	int iNewDaimyoBushouID = nb6gundan[_iGundanID].leader - 1; // ŒR’c’·‚Ì•«”Ô†
	int iNewDaimyoCastleID = nb6gundan[_iGundanID].residence - 1; // Š‘®é”Ô†
	// ŒR’c’·ˆÈŠO‚ªw’è‚³‚ê‚Ä‚¢‚éB‘¦‚¿A‘å–¼©g‚Ì(‘æ‚PŒR’c‘Š“–‚Ì)ŒR’c”Ô†‚ªw’è‚³‚ê‚Ä‚¢‚éB
	if (nb6bushouname[iNewDaimyoBushouID].State != 1) { // ŒR’c’·
		return 0xFFFF;
	}

	// íê‚Å€‚ñ‚Å‚¢‚ÄAƒƒCƒ“‰æ–Ê‚É–ß‚é‘OBƒ_ƒ
	if (nb6bushouname[iNewDaimyoBushouID].battleDeath) { // í€
		return 0xFFFF;
	}

	// ----------------------------------------------------
	// ƒSƒ~‘|œ
	nb6daimyo[iSelfDaimyoID].retire = 0xFFFF;
	nb6daimyo[iSelfDaimyoID].hostile = 0xFF;	// “G‘Î‘å–¼
	nb6daimyo[iSelfDaimyoID].friendship = 0xFF; // —FD‘å–¼
	nb6daimyo[iSelfDaimyoID].friendship = 50;   // ¤l—FD“x
	nb6daimyo[iSelfDaimyoID].bPropagate = 0;  // •z‹³Ï
	nb6daimyo[iSelfDaimyoID].hyotei = 0;      // •]’è
	nb6daimyo[iSelfDaimyoID].bMerchantAngry = 0;      // ¤l‚ª“{‚Á‚Ä‚Ä¤l•sİ

	if (nb6bushouref[iNewDaimyoBushouID].job == 0x60) { // ‚‰Æ
		nb6daimyo[iSelfDaimyoID].tyotei = 30; // ’©’ì
	}
	else {
		nb6daimyo[iSelfDaimyoID].tyotei = 0; // ’©’ì
	}

	nb6daimyo[iSelfDaimyoID].symbol = _getKamonAtBorn(iNewDaimyoBushouID);

	tryChangeDaimyoYakusyoku(iSelfDaimyoID, 0);

	// ----------------------------------------------------
	// ‘ÎÛ‚ÌŒR’c’·‚ÍAŒÂl‚Æ‚µ‚Ä‘å–¼‚Æ‚È‚é
	nb6bushouname[iNewDaimyoBushouID].State = 0; // 0:‘å–¼
	nb6bushouname[iNewDaimyoBushouID].position = 1; // 1:‘å–¼

	// ‚Ü‚¸‚ÍVİ‚µ‚½‘å–¼‰Æ‚Ì‘å–¼‚ÍAw’è‚ÌŒR’c’·‚ğVİ‘å–¼‚Ì•«‚É
	nb6daimyo[iSelfDaimyoID].attach = iNewDaimyoBushouID + 1; // ‘å–¼y•«”Ô†z
	// ‘å–¼‚Ì–{‘æ‚PŒR’c‚Æ‚È‚é
	nb6daimyo[iSelfDaimyoID].gundan = _iGundanID + 1;

	// ----------------------------------------------------
	// Œ»İ‚ÌŒR’c‚ÌŠ‘®‘å–¼‚ªAVİ‚Ì‘å–¼ID‚É‹A‘®
	nb6gundan[_iGundanID].attach = iSelfDaimyoID + 1; // Š‘®‘å–¼y‘å–¼”Ô†z

	// Š„“–”Ô†‚Í‚PŒÅ’è
	nb6gundan[_iGundanID].number = 1;

	// ŒR’c’·‚Ì–{‹’‚¾‚Á‚½‚Æ‚¢‚¤î•ñ‚ÍA‘å–¼‚Ì–{‹’‚ÖŠiã‚°
	nb6daimyo[iSelfDaimyoID].castle = iNewDaimyoCastleID + 1; // Š‘®‹éyé”Ô†z
	nb6castle[iNewDaimyoCastleID].honjo = 1; //  –{é::‘å–¼;

	// ----------------------------------------------------
	// ƒŠƒ“ƒNƒŠƒXƒg‚Ì•ÏXB©•ª‚ÌŒR’c‚¾‚¯A‚±‚ê‚Ü‚Å‚Ì‘å–¼”z‰º‚ÌŒR’c‚ÌƒŠƒ“ƒNƒŠƒXƒg‚©‚çíœ‚·‚é

	// ©•ª©g(ŒR’c)‚ğƒŠƒ“ƒNƒŠƒXƒg‚©‚çŠO‚·B
	// ‘O‚ÌŒR’c‚ÌuŸ‚ÌŒR’cv‚ğA©g‚Å‚Í‚È‚­Au©g‚ÌŸ‚ÌŒR’cv‚É‚·‚éB
	if (nb6gundan[_iGundanID].prev != 0 && nb6gundan[_iGundanID].prev != 0xFFFF) {
		int _prev = nb6gundan[_iGundanID].prev - 1;
		nb6gundan[_prev].next = nb6gundan[_iGundanID].next;
	}

	// Ÿ‚ÌŒR’c‚Ìu‘O‚ÌŒR’cv‚ğA©g‚Å‚Í‚È‚­Au©g‚Ì‘O‚ÌŒR’cv‚É‚·‚éB
	if (nb6gundan[_iGundanID].next != 0 && nb6gundan[_iGundanID].next != 0xFFFF) {
		int _next = nb6gundan[_iGundanID].next - 1;
		nb6gundan[_next].prev = nb6gundan[_iGundanID].prev;
	}


	// Vİ‚È‚Ì‚Å©•ª‚ªæ“ª‚ÍŠm’è
	nb6gundan[_iGundanID].prev = 0;
	// Vİ‚È‚Ì‚Å©•ª‚ªÅŒã‚ÍŠm’è
	nb6gundan[_iGundanID].next = 0xFFFF;

	// ----------------------------------------------------
	// ŒR’c”z‰º•«‚ÌŠ‘®•«(©•ª‚ğŠÜ‚Ş)‚ÌŠ‘®‘å–¼‚ğAVİ‘å–¼‚Ö‚Ì•ÏX‚·‚é
	for (int iBushouID = 0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {
		if (nb6bushouref[iBushouID].gundan == _iGundanID + 1) {
			// í€’†‚Å‚Í‚È‚¢
			if (!nb6bushouname[iBushouID].battleDeath) { // í€
				nb6bushouref[iBushouID].attach = iSelfDaimyoID + 1;
				nb6bushouref[iBushouID].work = 0; // mŠ¯”N”
			}
		}
	}

	// ----------------------------------------------------
	// “¯–¿Œn
	for (int iTargetDaimyoID = 0; iTargetDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM; iTargetDaimyoID++) {
		// —FD‚Í‚Ü‚ñ‚È‚©
		setYuukouKankei(iSelfDaimyoID, iTargetDaimyoID, 50);
		// “¯–¿‚â¥ˆ÷‚ÍƒŠƒZƒbƒg
		setDoumeiKankei(iSelfDaimyoID, iTargetDaimyoID, FALSE);
		setKoninKankei(iSelfDaimyoID, iTargetDaimyoID, FALSE);
	}
	return iSelfDaimyoID;
}



// ‚Æ‚ ‚é‘å–¼“à‚Å‚Ì‚`ŒR’c‚Ìé‚ğ‚aŒR’c‚Ö‚ÆƒAƒ^ƒbƒ`•ÏX
bool Set_ChangeCastleAttachGundan(int _l_CastleID, int _l_GundanID) {

	if (!(0 <= _l_CastleID && _l_CastleID < GAMEDATASTRUCT_CASTLE_NUM)) {
		OutputDebugStream("é”Ô†‚ª”ÍˆÍŠO");
		return false;
	}

	if (nb6castle[_l_CastleID].honjo == 2 || nb6castle[_l_CastleID].honjo == 1) { // 2:ŒR’c’·‚Ì–{‹’ 1:‘å–¼‚Ì–{‹’
		OutputDebugStream("‘å–¼‚âŒR’c’·‚Ì–{‹’’n‚ÍŠÒ‚ğˆÚ“®‚Å‚«‚È‚¢");
		return false;
	}

	if (!(0 <= _l_GundanID && _l_GundanID < GAMEDATASTRUCT_GUNDAN_NUM)) {
		OutputDebugStream("ŒR’c”Ô†‚ª”ÍˆÍŠO");
		return false;
	}

	int iGundanID = nb6castle[_l_CastleID].attach - 1; // Š‘®ŒR’cŒR’c”Ô† -1

	if (!(0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM)) {
		OutputDebugStream("é”Ô†‚ª‚¨‚»‚ç‚­–¢“¡é");
		return false;
	}

	int iOrgDaimyoID = nb6gundan[iGundanID].attach - 1; // Š‘®‘å–¼”Ô†
	int iDstDaimyoID = nb6gundan[_l_GundanID].attach - 1;

	if (iOrgDaimyoID != iDstDaimyoID) {
		OutputDebugStream("é”Ô†‚ÆVŒR’c”Ô†‚ª“¯‚¶‘å–¼‚Ì‚à‚Ì‚Å‚Í‚È‚¢");
		return false;
	}

	// -------------------------------------------------------------------
	// ‚Ü‚¸AŒ³‚ÌŒR’c‚ÌéƒŠƒ“ƒNƒŠƒXƒg‚©‚çA©•ª‚Ìé‚ğŠO‚·
	nb6castle[_l_CastleID].attach = _l_GundanID + 1;  // Š‘®ŒR’c‚ÌŒR’c”Ô†

	// ©•ª©g(é)‚ğƒŠƒ“ƒNƒŠƒXƒg‚©‚çŠO‚·B
	// ‘O‚Ìé‚ÌuŸ‚Ìév‚ğA©g‚Å‚Í‚È‚­Au©g‚ÌŸ‚Ìév‚É‚·‚éB
	if (nb6castle[_l_CastleID].prev != 0 && nb6castle[_l_CastleID].prev != 0xFFFF) {
		int _prev = nb6castle[_l_CastleID].prev - 1;
		nb6castle[_prev].next = nb6castle[_l_CastleID].next;
	}

	// Ÿ‚Ìé‚Ìu‘O‚Ìév‚ğA©g‚Å‚Í‚È‚­Au©g‚Ì‘O‚Ìév‚É‚·‚éB
	if (nb6castle[_l_CastleID].next != 0 && nb6castle[_l_CastleID].next != 0xFFFF) {
		int _next = nb6castle[_l_CastleID].next - 1;
		nb6castle[_next].prev = nb6castle[_l_CastleID].prev;
	}

	// -------------------------------------------------------------------
	// Ÿ‚ÉAVŒR’c‚ÌéƒŠƒ“ƒNƒŠƒXƒg‚ÌÅŒã‚Ìé‚ğ’T‚·
	int iTailCastleID = 0xFFFF;
	for (int iCastleID = 0; iCastleID < GAMEDATASTRUCT_CASTLE_NUM; iCastleID++) {
		// ‘ÎÛ‚Ìé‚ÌŠ‘®ŒR’c‚ªAw’è‚ÌVŒR’c‚Å‚ ‚è
		if (nb6castle[iCastleID].attach - 1 == _l_GundanID) {
			// ‚»‚Ìé‚ªƒŠƒ“ƒNƒŠƒXƒg‚ÌÅŒã‚Ìé‚Å‚ ‚é‚È‚ç‚Î
			if (nb6castle[iCastleID].next == 0xFFFF) {
				iTailCastleID = iCastleID;
				break;
			}
		}
	}

	if (iTailCastleID == 0xFFFF) {
		OutputDebugStream("ˆÙí–‘Ô!! é‚ÌƒŠƒ“ƒNƒŠƒXƒg‚ª‰ó‚ê‚Ä‚¢‚é!!");
		return false;
	}

	// ©•ª‚ÍVŒR’c‚Å‚ÍƒŠƒ“ƒNƒŠƒXƒg‚Ì––”ö‚ÍŠm’è‚È‚Ì‚ÅŸ‚Ìé‚Í‚È‚¢
	nb6castle[_l_CastleID].next = 0xFFFF;

	// ©•ª‚Ì‚P‚Â‘O‚ÍA‚±‚ê‚Ü‚Å––”ö‚¾‚Á‚½é‚Ì”Ô†
	nb6castle[_l_CastleID].prev = iTailCastleID + 1;

	// ‚±‚ê‚Ü‚ÅƒŠƒ“ƒNƒŠƒXƒg‚Ì––”ö‚¾‚Á‚½é‚ÌŸ‚Ìé‚ª©•ª
	nb6castle[iTailCastleID].next = _l_CastleID + 1;

	// -------------------------------------------------------------------
	// ‚»‚Ìé‚É‚¢‚é•«’B‚ÌŠ‘®‚ğ•ÏX‚·‚é
	// ŠY“–‚Ìé‚É‹‚él’B‚ÌŠ‘®ŒR’c‚Ì‘‚«Š·‚¦
	for (int iBushouID = 0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {
		if (nb6bushouref[iBushouID].attach == iOrgDaimyoID + 1) {  // Š‘®‘å–¼”Ô†
			if (nb6bushouref[iBushouID].castle == _l_CastleID + 1) { // Š‘®‹é
				if (nb6bushouname[iBushouID].State == 1 || nb6bushouname[iBushouID].State == 2) { // 1:ŒR’c’· 2:Œ»–ğ
					// í€’†‚Å‚Í‚È‚¢
					if ( ! nb6bushouname[iBushouID].battleDeath) {
						nb6bushouref[iBushouID].gundan = _l_GundanID + 1;
					}
				}
			}
		}
	}

	return true;

}




// ‘S—LŒø‘å–¼‚ÌƒŠƒXƒg‚ğ•Ô‚·Bƒ^[ƒ“‘å–¼©g‚à“ü‚Á‚Ä‚é‚Ì‚Å’ˆÓB
vector<int> getDaimyoList(BOOL isIncludeTurnDaimyo) {
	vector<int> list;
	for ( int iDaimyoID=0; iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM; iDaimyoID++ ) {
		// ƒ^[ƒ“‘å–¼‚ÍŠÜ‚Ü‚È‚¢
		if ( !isIncludeTurnDaimyo ) {
			// ƒ^[ƒ“‘å–¼‚È‚çƒpƒX
			if ( iDaimyoID == nb6turn.daimyo-1 ) {
				continue;
			}
		}
		// ‚¿‚á‚ñ‚Æ•«‚ª”[‚ß‚Ä‚¢‚é‘å–¼‰Æ‚Ì‚İ
		if ( nb6daimyo[iDaimyoID].attach != 0xFFFF ) {
			list.push_back(iDaimyoID);
		}
	}
	return list;
}



// ‚`‘å–¼(‚On‚Ü‚è‚Å‚Ì”z—ñ—pw’è)‚Æ‚a‘å–¼‚Æ‚Ì“¯–¿ŠÖŒWj
// 004CF213  05 55 55 55 95    ‚Æ‚ ‚é‚Ì‚ÍA004CF213 [0 5 5 5 5 5 5 5 9 5] ‚Æ‚¢‚¤‚æ‚¤‚É1”Ô–ÚA2”Ô–ÚA3”Ô–Ú‚Ì‘Šèc‚Æ‚¢‚¤Š´‚¶‚Æ‚È‚éB  
// ˆê•û“¯–¿E¥ˆ÷î•ñ‚Í‚Ç‚±‚É‚ ‚é‚Ì‚©‚Æ‚¢‚¤‚ÆA2l‚Ì‘g‚İ‡‚í‚¹‚Ì‚¤‚¿A‘å‚«‚¢•û‚Ì‘å–¼”Ô†—p‚Ì“¯‚¶\‘¢‘Ì‚É“ü‚Á‚Ä‚¢‚éB
// —á‚¦‚ÎA‘å–¼”Ô†21‚Æ25‚Æ‚·‚é‚ÆA—FDî•ñ‚Í21”Ô‚Ì•û‚Ì25”Ô–Ú‚É“ü‚Á‚Ä‚¨‚èA“¯–¿‚â¥ˆ÷‚ÍA25”Ô‚Ì•û‚Ì21”Ô–Ú‚É“ü‚Á‚Ä‚¢‚é‚Æ‚¢‚¤‚í‚¯‚¾B


// ‰º‚ÌgetDoumeiKankei“à•”‚Å‹‚Ü‚é’l‚ÍA‚±‚Ì”z—ñ‚ÌINDEX‚Å‚ ‚é‚½‚ßA‚±‚ê‚ğg‚Á‚ÄÀÛ‚Ì’l‚É’¼‚·B
int YuukouValueArray[] = {0, 10, 20, 30, 40, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 0xFFFF};


byte* getYuukouKannkeiPointer(int& iDaimyoA_ID, int& iDaimyoB_ID) {

	// ”ÍˆÍƒ`ƒFƒbƒN
	if ( 0 <= iDaimyoA_ID && iDaimyoA_ID < GAMEDATASTRUCT_DAIMYO_NUM &&
 		 0 <= iDaimyoB_ID && iDaimyoB_ID < GAMEDATASTRUCT_DAIMYO_NUM ) {
	} else {
		return NULL;
	}

	// ‘å–¼‚`‚Æ‘å–¼‚a‚ÅA‘å–¼‚`‚Ì•û‚ª’l‚ª‘å‚«‚¢ê‡A“ü‚ê‘Ö‚¦‚éB
	// ‚È‚º‚È‚ç—FDŠÖŒW‚Ì’l‚ÍA‘å–¼‚`‚ª¬‚³‚¢•û‚Ì‘g‚İ‡‚í‚¹ƒe[ƒuƒ‹‚É“ü‚Á‚Ä‚¢‚ÄA‘å–¼‚`‚ª‘å‚«‚¢•û‚Ì‘g‚İ‡‚í‚¹ƒe[ƒuƒ‹‚É‚Í¥ˆ÷‚â“¯–¿‚Ì’l‚ª“ü‚Á‚Ä‚¢‚é‚©‚çB
	if (iDaimyoA_ID > iDaimyoB_ID ) {
		// ‚Q‚Â“ü‚ê‘Ö‚¦
		iDaimyoA_ID ^= iDaimyoB_ID;
		iDaimyoB_ID ^= iDaimyoA_ID;
		iDaimyoA_ID ^= iDaimyoB_ID;
	}

	byte *p = (byte *)GAMEDATASTRUCT_DOUMEI_ADDRESS;
	
	// ˆÈ‰º2l‚Ì‘å–¼‚h‚c‚æ‚èA“¯–¿ŠÖŒW’l‚ª“ü‚Á‚Ä‚¢‚éƒƒ‚ƒŠ‚ÌƒAƒhƒŒƒX‚ğ‹‚ß‚é
	p += iDaimyoA_ID * ( GAMEDATASTRUCT_DAIMYO_NUM / 2 ); // ‚à‚µ‚à‚P‚Â‚Ì‚`‘å–¼‚ ‚½‚èA‚a‘å–¼‚Ö‚Ì“¯–¿‚ª‚PƒoƒCƒg‚Å•\Œ»‚³‚ê‚Ä‚¢‚ê‚ÎA
															 // ‚V‚Q‘å–¼‚¢‚é‚Ì‚ÅA‚V‚QƒoƒCƒg‚Å•\Œ»‚³‚ê‚éB
															 // ‚Æ‚±‚ë‚ªÀÛ‚É‚ÍA4ƒrƒbƒg‚Å•\Œ»‚³‚ê‚é‚Ì‚ÅA
															 // ‚P‚Â‚Ì‘å–¼‚É‚Ô‚ç‰º‚ª‚é\‘¢‘Ìƒf[ƒ^‚Æ‚µ‚Ä‚ÍA”¼•ª‚Ì‚R‚UƒoƒCƒg‚Å‚ ‚éB

	p += iDaimyoB_ID / 2;									 // ã‚Æ“¯‚¶——R‚Æ‚È‚éB‚P‚Â‚Ì‘å–¼‚Ì(“¯–¿ŠÖŒW\‘¢‘Ì)‚Ì’†‚Å‚à
															 // ‚P‚Â‚Ì‘Šè‘å–¼‚Æ‚Ì“¯–¿’l‚Ég‚í‚ê‚Ä‚¢‚é‚Ì‚Í”¼ƒoƒCƒgB
															 // ‚æ‚Á‚Äƒ|ƒCƒ“ƒ^‚ÌˆÊ’u‚Æ‚µ‚Ä‚ÍA/2 ‚µ‚Ä‚¨‚«A‹ô”‚©Šï”‚©‚ÅAŒã‚Å‰‰Z‚ğU‚è•ª‚¯‚éB

	return p;
}

int getYuukouKankei(int iDaimyoA_ID, int iDaimyoB_ID) {

	// ŠY“–‚Ì—FDŠÖŒWî•ñ‚ª‘¶İ‚·‚éƒAƒhƒŒƒX‚ğ“¾‚éB‚`‚Æ‚a‚Í’l‚ªŒğŠ·‚³‚ê‚é‚©‚à‚µ‚ê‚È‚¢B’ˆÓB
	byte *p = getYuukouKannkeiPointer( iDaimyoA_ID, iDaimyoB_ID );
	if (!p) return 0;

	// ƒAƒhƒŒƒX‚Í‹‚Ü‚Á‚½‚ªA‚P‚Â‚ÌƒAƒhƒŒƒX‚Ì1ƒoƒCƒg“à‚É‚ÍA‚Q‚Â‚Ì‘å–¼‚ª“ü‚Á‚Ä‚¢‚éB
	// ‘å–¼”Ô†‚ª‹ô”‚È‚çãˆÊŒ…A‹ô”‚È‚ç‰ºˆÊŒ…‚Å‚ ‚éB
	int YuukouIX = 0;
	// ‘Šè‘å–¼‚ª‹ô”‚¾
	if ( iDaimyoB_ID % 2==0 ) {								 // ‹ô”‚Å‚ ‚ê‚ÎAã”¼•ª‚Ìƒrƒbƒg‚Æ‚Ì•Œ‹‰Ê[0 5] ‚Ì‚¤‚¿[0]‚ğ•Ô‚·
		YuukouIX = (*p) >> 4;

	// ‘Šè‘å–¼‚ªŠï”‚¾										 // Šï”‚Å‚ ‚ê‚ÎA‰º”¼•ª‚Ìƒrƒbƒg‚Æ‚Ì•Œ‹‰Ê[0 5] ‚Ì‚¤‚¿[5]‚ğ•Ô‚·
	} else {
		YuukouIX = 0x0F & (*p);
	}

	return YuukouValueArray[YuukouIX];
}

// —FDŠÖŒW’l‚ğƒZƒbƒg‚·‚éB
void setYuukouKankei(int iDaimyoA_ID, int iDaimyoB_ID, int iValue) {

	// ŠY“–‚Ì—FDŠÖŒWî•ñ‚ª‘¶İ‚·‚éƒAƒhƒŒƒX‚ğ“¾‚éB‚`‚Æ‚a‚Í’l‚ªŒğŠ·‚³‚ê‚é‚©‚à‚µ‚ê‚È‚¢B’ˆÓB
	byte *p = getYuukouKannkeiPointer( iDaimyoA_ID, iDaimyoB_ID );
	if (!p) return;


	// •‰”‚Íƒ„ƒ”ƒ@ƒC‚Ì‚Å—FDŠÖŒW’l‚Æ‚µ‚Ä‚O‚ğw’è‚µ‚½‚à‚Ì‚Æ‚·‚éB100ƒI[ƒo[‚à100‚É‚µ‚Æ‚­B
	if ( iValue < 0 )	{ iValue = 0;	}
	if ( iValue > 100 ) { iValue = 100; }

	int YuukouIX = 0;
	// ƒZƒbƒg‚µ‚½‚¢—FDŠÖŒW’l‚ÆA—FDŠÖŒW’l‚Ì”z—ñ‚ğ”äŠr‚µ‚ÄA‚à‚Á‚Æ‚à‹ß‚¢ØÌ‚Ä’l‚Ìindex‚ğ‹‚ß‚éB
	// —á‚¦‚Î—FDŠÖŒW’l‚Æ‚µ‚Ä53‚Æ‚¢‚¤‚æ‚¤‚Éw’è‚³‚ê‚Ä‚àA‚»‚Ì‚æ‚¤‚È’l‚É‚Ío—ˆ‚È‚¢‚Ì‚ÅA50‚Æ‚·‚é‚í‚¯‚¾B
	// ‚»‚µ‚Ä50‚ÍAYuukouValueArray‚Ìindex==5‚É‘¶İ‚·‚éB‚±‚Ì5‚Ì‚æ‚¤‚È’l‚ğ‹‚ß‚½‚¢B
	for ( int i=0; i < sizeof(YuukouValueArray)/sizeof(YuukouValueArray[0]); i++) {
		if ( iValue < YuukouValueArray[i] ) {

			YuukouIX = i-1;	// ’l‚ª’´‚¦‚½‚Ì‚ÅA‚P‚Â‘O‚Ìindex‚É‚µ‚Æ‚­B

			if ( YuukouIX < 0 ) { YuukouIX = 0; }  // —˜_ã‚ ‚è‚¦‚È‚¢‚ªA‰½‘«‚·‚©‚í‚©‚ç‚ñ‚Ì‚Åˆê‰B
			break;
		}
	}

	// ƒAƒhƒŒƒX‚Í‹‚Ü‚Á‚½‚ªA‚P‚Â‚ÌƒAƒhƒŒƒX‚Ì1ƒoƒCƒg“à‚É‚ÍA‚Q‚Â‚Ì‘å–¼‚ª“ü‚Á‚Ä‚¢‚éB
	// ‘å–¼”Ô†‚ª‹ô”‚È‚çãˆÊŒ…A‹ô”‚È‚ç‰ºˆÊŒ…‚Å‚ ‚éB
	// ‘Šè‘å–¼‚ª‹ô”‚¾
	if ( iDaimyoB_ID % 2==0 ) {								 // ‹ô”‚Å‚ ‚ê‚ÎAã”¼•ª‚Ìƒrƒbƒg‚Æ‚Ì•Œ‹‰Ê[0 5] ‚Ì‚¤‚¿[0]‚ğ•Ô‚·
		YuukouIX = YuukouIX << 4;
		// ãˆÊƒoƒCƒg‚ÍYuukouIX‚ğƒVƒtƒg‚µ‚½‚à‚Ì‚ğA‰ºˆÊƒoƒCƒg‚Í‰ºˆÊƒoƒCƒg‚Ì•”•ª‚¾‚¯’Šo‚Å
		*p = YuukouIX | (0x0F & (*p));

	// ‘Šè‘å–¼‚ªŠï”‚¾										 // Šï”‚Å‚ ‚ê‚ÎA‰º”¼•ª‚Ìƒrƒbƒg‚Æ‚Ì•Œ‹‰Ê[0 5] ‚Ì‚¤‚¿[5]‚ğ•Ô‚·
	} else {
		// ãˆÊƒoƒCƒg‚Í‚»‚Ì‚Ü‚ÜÌ—p‚ÅA‰ºˆÊƒoƒCƒg‚ÍAYuukouIX‚»‚Ì‚Ü‚Ü‚ÅB
		*p = (0xF0 & (*p)) | YuukouIX;
	}
}

byte* getDoumeiKankeiPointer(int& iDaimyoA_ID, int& iDaimyoB_ID) {

	// ”ÍˆÍƒ`ƒFƒbƒN
	if ( 0 <= iDaimyoA_ID && iDaimyoA_ID < GAMEDATASTRUCT_DAIMYO_NUM &&
 		 0 <= iDaimyoB_ID && iDaimyoB_ID < GAMEDATASTRUCT_DAIMYO_NUM ) {
	} else {
		return NULL;
	}

	// ‘å–¼‚`‚Æ‘å–¼‚a‚ÅA‘å–¼‚`‚Ì•û‚ª’l‚ª¬‚³‚¢ê‡A“ü‚ê‘Ö‚¦‚éB
	// ‚È‚º‚È‚ç“¯–¿ŠÖŒW‚Ì’l‚ÍA‘å–¼‚`‚ª‘å‚«‚¢•û‚Ì‘g‚İ‡‚í‚¹ƒe[ƒuƒ‹‚É“ü‚Á‚Ä‚¢‚é‚©‚çB
	if (iDaimyoA_ID < iDaimyoB_ID ) {
		// ‚Q‚Â“ü‚ê‘Ö‚¦
		iDaimyoA_ID ^= iDaimyoB_ID;
		iDaimyoB_ID ^= iDaimyoA_ID;
		iDaimyoA_ID ^= iDaimyoB_ID;
	}


	byte *p = (byte *)GAMEDATASTRUCT_DOUMEI_ADDRESS;
	
	// ˆÈ‰º2l‚Ì‘å–¼‚h‚c‚æ‚èA“¯–¿ŠÖŒW’l‚ª“ü‚Á‚Ä‚¢‚éƒƒ‚ƒŠ‚ÌƒAƒhƒŒƒX‚ğ‹‚ß‚é
	p += iDaimyoA_ID * ( GAMEDATASTRUCT_DAIMYO_NUM / 2 ); // ‚à‚µ‚à‚P‚Â‚Ì‚`‘å–¼‚ ‚½‚èA‚a‘å–¼‚Ö‚Ì“¯–¿‚ª‚PƒoƒCƒg‚Å•\Œ»‚³‚ê‚Ä‚¢‚ê‚ÎA
															 // ‚V‚Q‘å–¼‚¢‚é‚Ì‚ÅA‚V‚QƒoƒCƒg‚Å•\Œ»‚³‚ê‚éB
															 // ‚Æ‚±‚ë‚ªÀÛ‚É‚ÍA4ƒrƒbƒg‚Å•\Œ»‚³‚ê‚é‚Ì‚ÅA
															 // ‚P‚Â‚Ì‘å–¼‚É‚Ô‚ç‰º‚ª‚é\‘¢‘Ìƒf[ƒ^‚Æ‚µ‚Ä‚ÍA”¼•ª‚Ì‚R‚UƒoƒCƒg‚Å‚ ‚éB

	p += iDaimyoB_ID / 2;									 // ã‚Æ“¯‚¶——R‚Æ‚È‚éB‚P‚Â‚Ì‘å–¼‚Ì(“¯–¿ŠÖŒW\‘¢‘Ì)‚Ì’†‚Å‚à
															 // ‚P‚Â‚Ì‘Šè‘å–¼‚Æ‚Ì“¯–¿’l‚Ég‚í‚ê‚Ä‚¢‚é‚Ì‚Í”¼ƒoƒCƒgB
															 // ‚æ‚Á‚Äƒ|ƒCƒ“ƒ^‚ÌˆÊ’u‚Æ‚µ‚Ä‚ÍA/2 ‚µ‚Ä‚¨‚«A‹ô”‚©Šï”‚©‚ÅAŒã‚Å‰‰Z‚ğU‚è•ª‚¯‚éB
	return p;
}

int isDoumeiKankei(int iDaimyoA_ID, int iDaimyoB_ID) {

	// ŠY“–‚Ì“¯–¿ŠÖŒWî•ñ‚ª‘¶İ‚·‚éƒAƒhƒŒƒX‚ğ“¾‚éB‚`‚Æ‚a‚Í’l‚ªŒğŠ·‚³‚ê‚é‚©‚à‚µ‚ê‚È‚¢B’ˆÓB
	byte *p = getDoumeiKankeiPointer( iDaimyoA_ID, iDaimyoB_ID );
	if (!p) return 0;

	// ƒAƒhƒŒƒX‚Í‹‚Ü‚Á‚½‚ªA‚P‚Â‚ÌƒAƒhƒŒƒX‚Ì1ƒoƒCƒg“à‚É‚ÍA‚Q‚Â‚Ì‘å–¼‚ª“ü‚Á‚Ä‚¢‚éB
	// ‘å–¼”Ô†‚ª‹ô”‚È‚çãˆÊŒ…A‹ô”‚È‚ç‰ºˆÊŒ…‚Å‚ ‚éB
	// ‘Šè‘å–¼‚ª‹ô”‚¾
	if ( iDaimyoB_ID % 2==0 ) {								 // ‹ô”‚Å‚ ‚ê‚ÎAã”¼•ª‚Ìƒrƒbƒg
		return 0x10 & (*p);
	// ‘Šè‘å–¼‚ªŠï”‚¾										 // Šï”‚Å‚ ‚ê‚ÎA‰º”¼•ª‚Ìƒrƒbƒg
	} else {
		return 0x01 & (*p);
	}
}
int isKoninKankei(int iDaimyoA_ID, int iDaimyoB_ID) {

	// ŠY“–‚Ì¥ˆ÷ŠÖŒWî•ñ‚ª‘¶İ‚·‚éƒAƒhƒŒƒX‚ğ“¾‚é(“¯–¿‚Æ“¯‚¶êŠ)B‚`‚Æ‚a‚Í’l‚ªŒğŠ·‚³‚ê‚é‚©‚à‚µ‚ê‚È‚¢B’ˆÓB
	byte *p = getDoumeiKankeiPointer( iDaimyoA_ID, iDaimyoB_ID );
	if (!p) return 0;

	// ƒAƒhƒŒƒX‚Í‹‚Ü‚Á‚½‚ªA‚P‚Â‚ÌƒAƒhƒŒƒX‚Ì1ƒoƒCƒg“à‚É‚ÍA‚Q‚Â‚Ì‘å–¼‚ª“ü‚Á‚Ä‚¢‚éB
	// ‘å–¼”Ô†‚ª‹ô”‚È‚çãˆÊŒ…A‹ô”‚È‚ç‰ºˆÊŒ…‚Å‚ ‚éB

	if ( iDaimyoB_ID % 2==0 ) {								 // ‹ô”‚Å‚ ‚ê‚ÎAã”¼•ª‚Ìƒrƒbƒg
		return (0x20 & (*p)) > 0;							 // ‚Q‚Æ‚Ì”äŠr‚Å‚ÍŠÔˆá‚¤‚©‚à‚µ‚ê‚È‚¢‚Ì‚ÅAˆê‰BOOL‚Á‚Û‚­‚µ‚Ä‚¨‚­B
	// ‘Šè‘å–¼‚ªŠï”‚¾										 // Šï”‚Å‚ ‚ê‚ÎA‰º”¼•ª‚Ìƒrƒbƒg
	} else {
		return (0x02 & (*p)) > 0;							 // ‚Q‚Æ‚Ì”äŠr‚Å‚ÍŠÔˆá‚¤‚©‚à‚µ‚ê‚È‚¢‚Ì‚ÅAˆê‰BOOL‚Á‚Û‚­‚µ‚Ä‚¨‚­B
	}
}

void setDoumeiKankei(int iDaimyoA_ID, int iDaimyoB_ID, BOOL doDoumei) {

	// ŠY“–‚Ì“¯–¿ŠÖŒWî•ñ‚ª‘¶İ‚·‚éƒAƒhƒŒƒX‚ğ“¾‚éB‚`‚Æ‚a‚Í’l‚ªŒğŠ·‚³‚ê‚é‚©‚à‚µ‚ê‚È‚¢B’ˆÓB
	byte *p = getDoumeiKankeiPointer( iDaimyoA_ID, iDaimyoB_ID );
	if (!p) return;
	
	// ƒAƒhƒŒƒX‚Í‹‚Ü‚Á‚½‚ªA‚P‚Â‚ÌƒAƒhƒŒƒX‚Ì1ƒoƒCƒg“à‚É‚ÍA‚Q‚Â‚Ì‘å–¼‚ª“ü‚Á‚Ä‚¢‚éB
	// ‘å–¼”Ô†‚ª‹ô”‚È‚çãˆÊŒ…A‹ô”‚È‚ç‰ºˆÊŒ…‚Å‚ ‚éB
	// ‘Šè‘å–¼‚ª‹ô”‚¾
	if ( iDaimyoB_ID % 2==0 ) {								 // ‹ô”‚Å‚ ‚ê‚ÎAã”¼•ª‚Ìƒrƒbƒg
		if (doDoumei) {
			*p = *p | 0x10;
		}
		else {
			*p = *p & 0xEF;
		}
		// ‘Šè‘å–¼‚ªŠï”‚¾										 // Šï”‚Å‚ ‚ê‚ÎA‰º”¼•ª‚Ìƒrƒbƒg
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

	// ŠY“–‚Ì¥ˆ÷ŠÖŒWî•ñ‚ª‘¶İ‚·‚éƒAƒhƒŒƒX‚ğ“¾‚é(“¯–¿‚Æ“¯‚¶êŠ)B‚`‚Æ‚a‚Í’l‚ªŒğŠ·‚³‚ê‚é‚©‚à‚µ‚ê‚È‚¢B’ˆÓB
	byte *p = getDoumeiKankeiPointer( iDaimyoA_ID, iDaimyoB_ID );
	if (!p) return;
	
	// ƒAƒhƒŒƒX‚Í‹‚Ü‚Á‚½‚ªA‚P‚Â‚ÌƒAƒhƒŒƒX‚Ì1ƒoƒCƒg“à‚É‚ÍA‚Q‚Â‚Ì‘å–¼‚ª“ü‚Á‚Ä‚¢‚éB
	// ‘å–¼”Ô†‚ª‹ô”‚È‚çãˆÊŒ…A‹ô”‚È‚ç‰ºˆÊŒ…‚Å‚ ‚éB
	// ‘Šè‘å–¼‚ª‹ô”‚¾
	if ( iDaimyoB_ID % 2==0 ) {								 // ‹ô”‚Å‚ ‚ê‚ÎAã”¼•ª‚Ìƒrƒbƒg
		if (doKonin) {
			*p = *p | 0x20;
		}
		else {
			*p = *p & 0xEF;
		}
		// ‘Šè‘å–¼‚ªŠï”‚¾										 // Šï”‚Å‚ ‚ê‚ÎA‰º”¼•ª‚Ìƒrƒbƒg
	} else {
		if (doKonin) {
			*p = *p | 0x02;
		}
		else {
			*p = *p & 0x0E;
		}
	}
}




int DaimyoHasCastleArray[GAMEDATASTRUCT_DAIMYO_NUM] = {}; // ŠeX‚Ì‘å–¼‚ª‚Âé”

//Å‘å¨—Í(Å‚à‚¿é”‚ª‘½‚¢)‘å–¼‚ğ“¾‚é
int GetMaximumInfluenceDaimyo() {
	// ‚Ü‚¸Aˆê‰ŒÄ‚Î‚ê‚é“x‚ÉƒŠƒZƒbƒg
	for (int iDaimyoID=0; iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM; iDaimyoID++ ) {
		DaimyoHasCastleArray[iDaimyoID] = 0;
	}

	for (int iCastleID=0; iCastleID < GAMEDATASTRUCT_CASTLE_NUM; iCastleID++ ) {
		// Š‘®‘å–¼”Ô†‚É]‚Á‚ÄAƒJƒEƒ“ƒg‚ğ‘‚â‚µ‚Ä‚ä‚­B
		int iGundanID = nb6castle[iCastleID].attach-1;
		if ( iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) { // ’N‚É‚àŠ‘®‚µ‚Ä‚È‚¢é‚Í‚±‚ÌğŒ‚Å’e‚©‚ê‚é
			int iDaimyoID = nb6gundan[iGundanID].attach-1;
			DaimyoHasCastleArray[iDaimyoID]++;
		}
	}

	// Å‘å‚Ìé‚¿‚Ì‘å–¼ID‚ğ’T‚·
	int iMaximumInfluenceDaimyoID = 0;

	// ”äŠr‘Šè‚È‚Ì‚Å1ƒXƒ^[ƒg‚Å—Ç‚¢
	for (int iDaimyoID=1; iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM; iDaimyoID++ ) {
		if ( DaimyoHasCastleArray[iDaimyoID] > DaimyoHasCastleArray[iMaximumInfluenceDaimyoID] ) {
			iMaximumInfluenceDaimyoID = iDaimyoID;
		}
	}

	return iMaximumInfluenceDaimyoID;
}

/*
004D1490  C0 FD 4C 00 B4 FD 4C 00 A8 FD 4C 00 9C FD 4C 00  ÀıL.´ıL.¨ıL.EL.
004D14A0  8C FD 4C 00 80 FD 4C 00 70 FD 4C 00 60 FD 4C 00  EL.€ıL.pıL.`ıL.
004D14B0  54 FD 4C 00 48 FD 4C 00                          TıL.HıL.
*/
char *getGoyouSyouninMei(int iGoyousyouninID) {
	if ( 0 <= iGoyousyouninID && iGoyousyouninID <= GAMEDATASTRUCT_SYOUNIN_NUM ) {
		int *pArrayGoyouSyoninName = (int *)GAMEDATASTRUCT_GOYOUSYOUNINNAME_ARRAY;
		return (char *)(pArrayGoyouSyoninName[iGoyousyouninID]);
	} else {
		return "";
	}
}











// ‰Æ•ó‚Ìí—Ş‚Ì•¶š—ñ‚Ö‚Ìƒ|ƒCƒ“ƒ^‚ÌƒAƒhƒŒƒXˆê——
int GAMEDATASTRUCT_KAHOUTYPELIST_POINTER[GAMEDATASTRUCT_KAHOUTYPE_NUM] = {NULL};




// ‰Æ•óƒ^ƒCƒv‚ÌID‚©‚ç•¶š—ñ‚ğ“¾‚é
char* getKahouTypeName(byte type) {
	char *pszKahouType = NULL;
	if ( type < GAMEDATASTRUCT_KAHOUTYPE_NUM ) {
		pszKahouType = (char *)(GAMEDATASTRUCT_KAHOUTYPELIST_POINTER[type]);
		return pszKahouType;
	}
	return NULL;
}


// ƒZ[ƒuƒf[ƒ^‚Ìƒtƒ@ƒCƒ‹–¼
char *psznb6savedatafilename = (char *)GAMEDATASTRUCT_SAVEDATAFILENAME_ADDRESS;



// “V‹Cî•ñ‚ğ“¾‚é 0:°‚ê 1:“Ü‚è 2:‰J 3:á
byte getWeather() {

	byte *weather = (byte *)GAMEDATASTRUCT_WEATHER_ADDRESSS;

	return *weather;
}



// “V‹C‚Ì—\–ñ
void reserveWeather(int eTenki, int iContinueTurn) {
	iReservedWeatherOnChangeWeather.clear();
	if ( getRemainTurnInBattle() > 0 ) {
		TReservedTenki tk = { getRemainTurnInBattle(), eTenki, iContinueTurn };
		iReservedWeatherOnChangeWeather.push_back(tk);
	}
}
// “V‹C‚Ì—\–ñ
void WINAPI Extern_reserveWeather(int eTenki, int iContinueTurn) {
	reserveWeather(eTenki, iContinueTurn);
}


// í‘ˆ‚Å‚·‚Å‚É‰½ƒ^[ƒ“Œo‰ß‚µ‚½‚©
byte getHowMuchTurnProceesInBattle() {

	byte *turn = (byte *)GAMEDATASTRUCT_HOWMUCHTURNPROCEED_INBATTLE;

	return *turn;
}

// í‘ˆ‚Å‰½ƒ^[ƒ“c‚Á‚Ä‚¢‚é‚©Bƒƒ‚ƒŠ‚Å‚Í‚È‚­A“à•”ŒvZ
extern int iCurIncrementTurnInBattle;
int getRemainTurnInBattle() {
	// í“¬ƒ^[ƒ“’†ˆÈŠO‚Å‚ ‚ê‚ÎA-1‚ğ•Ô‚·B
	if (iCurIncrementTurnInBattle < 0) {
		return iCurIncrementTurnInBattle;
	}
	// í‘ˆ’†‚È‚çA30‚©‚çŒo‰ßƒ^[ƒ“‚ğˆø‚¢‚½‚à‚Ì‚ğ•Ô‚·B
	return 31-iCurIncrementTurnInBattle;
}

// ŠO•”QÆ—p
int WINAPI Extern_getRemainTurnInBattle() {
	return getRemainTurnInBattle();
}


// “S–C“`—ˆÏ‚İ‚©‚Ç‚¤‚©
bool isTeppouDenrai() {
	byte *bTeppouDenrai = (byte *)GAMEDATASTRUCT_EVENT_GROUP1_ADDRESSS;

	int isTeppouDenrai = *bTeppouDenrai & 0x01; // (= b0b00000001) 

	if (isTeppouDenrai) { return true; } else { return false; }
}

// “S–C“`—ˆ‚ğ‚â‚ß‚é
void UnsetTeppouDenrai() {
	byte *bTeppouDenrai = (byte *)GAMEDATASTRUCT_EVENT_GROUP1_ADDRESSS;

	*bTeppouDenrai = *bTeppouDenrai & 0xFE; // ( & 11111101 ) ‚·‚é‚±‚Æ‚ÅA“S–C“`—ˆ‚¾‚¯‚ğÁ‚·B 
}


// ƒLƒŠƒXƒg‹³“`—ˆÏ‚İ‚©‚Ç‚¤‚©
bool isChristDenrai() {
	byte *bChristDenrai = (byte *)GAMEDATASTRUCT_EVENT_GROUP1_ADDRESSS;

	int isChristDenrai = *bChristDenrai & 0x02; // (= b0b00000010) 

	if (isChristDenrai) { return true; } else { return false; }
}

// ˆÀ“yé‚ªŒšİÏ‚İ‚©‚Ç‚¤‚©
bool isAzuchiCastleComplete() {
	byte *bAzuchiCastleComplete = (byte *)GAMEDATASTRUCT_EVENT_GROUP1_ADDRESSS;

	int isAzuchiCastleComplete = *bAzuchiCastleComplete & 0x08; // (= b0b00001000) 

	if (isAzuchiCastleComplete) { return true; } else { return false; }
}

// ‘åâé‚ªŒšİÏ‚İ‚©‚Ç‚¤‚©
bool isOsakaCastleComplete() {
	byte *bOsakaCastleComplete = (byte *)GAMEDATASTRUCT_EVENT_GROUP2_ADDRESSS;

	int isOsakaCastleComplete = *bOsakaCastleComplete & 0x40; // (= b0b01000000) 

	if (isOsakaCastleComplete) { return true; } else { return false; }
}

// ƒLƒŠƒXƒg‹³“`—ˆ‚ğ‚â‚ß‚é
void UnsetChristDenrai() {
	byte *bChristDenrai = (byte *)GAMEDATASTRUCT_EVENT_GROUP1_ADDRESSS;

	*bChristDenrai = *bChristDenrai & 0xFD; // ( & 11111101 ) ‚·‚é‚±‚Æ‚ÅA“S–C“`—ˆ‚¾‚¯‚ğÁ‚·B 
}



// SE‚Í—LŒø‚©H
bool isSEValid() {

	byte bSEValid = 0;

	bSEValid = nb6settings.se; // (= b0b00000001) 

	if (bSEValid) { return true; } else { return false; }

}

// BGM‚Í—LŒø‚©H
bool isBGMValid() {

	byte bBGMValid = 0;

	bBGMValid = nb6settings.bgm; // (= b0b00000010) 

	if (bBGMValid) { return true; } else { return false; }
}

// ƒ€[ƒr[‚Í—LŒø‚©H
bool isMovieValid() {

	byte bMovieValid = 0;

	bool bMovieInValid = nb6settings.movie_is_off; // (= b0b00010000) 

	if (!bMovieInValid) { return true; } else { return false; }

}

// _INMMƒ‚[ƒh‚©H
bool is_InmmMode() {
	char *winmm = (char *)GAMEDATASTRUCT_WINMM_DLL_NAME_ADDRESS;
	if ( strcmp( winmm, "_INMM.dll") == 0 ) {
		return true;
	} else if ( TSModCommand::Environment::cmdBGMMode == 1 || TSModCommand::Environment::cmdBGMMode == 2 ) {
		return true;
	}
	return false;
}


// ƒfƒ‚ƒ‚[ƒh‚©H
bool is_DemoMode() {
	for ( int iPG=0; iPG<GAMEDATASTRUCT_PLAYERS8_NUM; iPG++) {
		// ƒvƒŒƒCƒ„[‚ª’S“–‚µ‚Ä‚¢‚éŒR’c‚Ì‚h‚c‚ğŠm•Û
		int iGundanID = nb6players8[iPG].gundan;

		// ‚P‚Â‚Å‚àƒvƒŒƒCƒ„’S“–ŒR’c”Ô†‚ª0xFFFFˆÈŠO‚Ì’l‚ª‚ ‚ê‚ÎAƒfƒ‚ƒ‚[ƒh‚Å‚Í‚È‚¢B
		if ( iGundanID != 0xFFFF ) {
			return false;
		}
	}

	// ‘S•”0xFFFF‚È‚çƒfƒ‚ƒ‚[ƒh
	return true;
}


// ‘Š«‚Ì—Ö”z—ñB(ŒvZ—p)
int arrayAisho[32] = {8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7};

// A•«‚ÆB•«‚Ì‘Š«‚Ì·•ª‚ğ“¾‚é
// Å¬0(‘Š«Å‚) Å‘å8(‘Š«Åˆ«)
int getAishoDiff(int iAishoA, int iAishoB) {

	// ‚Ü‚¸AA•«‚Ì‘Š«ˆÊ’u‚ğ’†‰›‚©‚çæ‚é
	int iAAishoIXInArray = 0;
	for (int iAishoID=8; iAishoID<8+GAMEDATASTRUCT_AISHOU_NUM; iAishoID++ ) {
		// A‚Ì‘Š«’l‚ğŒ©‚Â‚¯‚½‚çA‚»‚±‚ªA‚ÌˆÊ’u
		if ( arrayAisho[iAishoID] == iAishoA ) {
			iAAishoIXInArray = iAishoID;
		}
	}

	// •s³‚È”š‚Ìê‡‚ÍA‘Š«·‚ÍÅ‚àˆ«‚¢‚à‚Ì‚Æ‚·‚éB
	if ( iAAishoIXInArray == 0 ) { return 8; }

	// B•«‚Ì‘Š«ˆÊ’u‚ÍA•«‚Ì-8‚©‚çƒXƒ^[ƒg
	int iBAishoIXInArray = 0;
	for (int iAishoID=iAAishoIXInArray-8; iAishoID<iAAishoIXInArray+8; iAishoID++ ) {
		// A‚Ì‘Š«’l‚ğŒ©‚Â‚¯‚½‚çA‚»‚±‚ªA‚ÌˆÊ’u
		if ( arrayAisho[iAishoID] == iAishoB ) {
			iBAishoIXInArray = iAishoID;
		}
	}
	
	// •s³‚È”š‚Ìê‡‚ÍA‘Š«·‚ÍÅ‚àˆ«‚¢‚à‚Ì‚Æ‚·‚éB
	if ( iBAishoIXInArray == 0 ) { return 8; }

	// ‚Q‚Â‚Ì‘Š«‚ÌˆÊ’u‚Ì·•ª‚ª‘Š«·
	return abs(iBAishoIXInArray-iAAishoIXInArray);
}


/*
 * 2l‚Ì•«ID‚©‚ç‘Š«‚Ìˆá‚¢‚ğ“¾‚éB
 * Å¬0(‘Š«Å‚) Å‘å8(‘Š«Åˆ«)
 */
int getBushouAishoDiff(int iBushouAID, int iBushouBID) {
	// •«A‚à•«B‚à”ÍˆÍ“à‚Éû‚Ü‚Á‚Ä‚¢‚é
	if ( 0 <= iBushouAID && iBushouAID < GAMEDATASTRUCT_BUSHOU_NUM &&  0 <= iBushouBID && iBushouBID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		// •«‚Ì‘Š«‚Ì·‚ğ•Ô‚·
		return getAishoDiff( nb6bushouref[iBushouAID].affinity, nb6bushouref[iBushouBID].affinity );
	}
	// ˆê”Ôˆ«‚¢’l‚ğ•Ô‚µ‚Ä‚¨‚­
	else {
		return 8;
	}
}









