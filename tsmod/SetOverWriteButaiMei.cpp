#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXBushou_All.h"
#include "ScenarioMod.h"

/*
* •”‘à–¼‚ğ‚»‚ê‚¼‚ê•ªŠò‚³‚¹‚éB•”‘à–¼‚Ì‚S‚Â‚Ìchar *”z—ñ‚Ì‚¢‚¸‚ê‚©‚ğ•Ô‚·B
*/

extern int isOnEditingButaiMode;


// •«‚Ì•º‘Ô‚ª‚¢‚¸‚ê‚©‚Å‚ ‚é(0=‘«Œy,1=‹R”n,2=“S–C,3=“S–C‹R”n
BOOL IsBushouForm(int iBushouID, int f1, int f2=-1, int f3=-1, int f4=-3) {
	if ( nb6bushouref[iBushouID].form == f1 ) return true;
	if ( f2 == -1 ) { return false; }
	if ( nb6bushouref[iBushouID].form == f2 ) return true;
	if ( f3 == -1 ) { return false; }
	if ( nb6bushouref[iBushouID].form == f3 ) return true;
	if ( f4 == -1) { return false; }
	if ( nb6bushouref[iBushouID].form == f4 ) return true;

	return false;
}

char *IsButaiArrayLoslorien[4] = {
	"ˆÃ•‹Rm", // ©ƒƒXƒ[ƒŠƒAƒ“(‘«Œy)
	"ˆÃ•‹Rm", // ©ƒƒXƒ[ƒŠƒAƒ“(‹R”n)
	"ˆÃ•‹Rm", // ©ƒƒXƒ[ƒŠƒAƒ“(“S–C)
	"ˆÃ•‹Rm", // ©ƒƒXƒ[ƒŠƒAƒ“(‹R”n“S–C)
};

char *IsButaiArrayLoslorienWithDetail[4] = {
	" ˆÃ•‹Rm (‘«Œy) ", // ©ƒƒXƒ[ƒŠƒAƒ“(‘«Œy)
	" ˆÃ•‹Rm (‹R”n) ", // ©ƒƒXƒ[ƒŠƒAƒ“(‹R”n)
	" ˆÃ•‹Rm (“S–C) ", // ©ƒƒXƒ[ƒŠƒAƒ“(“S–C)
	" ˆÃ•‹Rm (‹R“S) ", // ©ƒƒXƒ[ƒŠƒAƒ“(‹R”n“S–C)
};


char *IsButaiArrayIkkousyu[4] = {
	"ˆêŒü“ã“",
	"‹R”n",
	"ˆêŒü“S–C",
	"‹R”n“S–C"
};
char *IsButaiArrayIkkousyuYumi[4] = {
	"‹|‘m",
	"‹|‹R”n",
	"ˆêŒü“S–C",
	"‹R”n“S–C"
};

char *IsButaiArrayUesughi[4] = {
	"ã™“ã“",
	"‹R”n",
	"“S–C",
	"‹R”n“S–C"
};

char *IsButaiArrayUesughiYumi[4] = {
	"ã™‹|‘m",
	"‹|‹R”n",
	"“S–C",
	"‹R”n“S–C"
};

char *IsButaiArrayShimazu[4] = {
	"“‡’Ã“˜",
	"‹R”n",
	"“S–C",
	"‹R”n“S–C"
};

char *IsButaiArrayDate[4] = {
	"ˆÉ’B“˜",
	"‹R”n",
	"“S–C",
	"ˆÉ’B‹R“S"
};

char *IsButaiArrayDateYumi[4] = {
	"‹|‘«Œy",
	"‹|‹R”n",
	"“S–C",
	"ˆÉ’B‹R“S"
};

char *IsButaiArrayHattori[4] = {
	"ˆÉ‰ê”EÒ",
	"‹R”n",
	"••”“S–C",
	"‹R”n“S–C"
};

char *IsButaiArrayHattoriYumi[4] = {
	"ˆÉ‰ê”EÒ",
	"‹|‹R”n",
	"••”“S–C",
	"‹R”n“S–C"
};
char *IsButaiArrayHuuma[4] = {
	"•—–‚”EÒ",
	"‹R”n",
	"“S–C",
	"‹R”n“S–C"
};

char *IsButaiArrayHuumaYumi[4] = {
	"•—–‚”EÒ",
	"‹|‹R”n",
	"“S–C",
	"‹R”n“S–C"
};

char *IsButaiArrayMomochi[4] = {
	"ˆÉ‰ê”EÒ",
	"‹R”n",
	"ˆÉ‰ê“S–C",
	"‹R”n“S–C"
};

char *IsButaiArrayMomochiYumi[4] = {
	"ˆÉ‰ê”EÒ",
	"‹|‹R”n",
	"ˆÉ‰ê“S–C",
	"‹R”n“S–C"
};

char *IsButaiArraySarutobi[4] = {
	"b‰ê”EÒ",
	"‹R”n",
	"“S–C",
	"‹R”n“S–C"
};

char *IsButaiArraySarutobiYumi[4] = {
	"b‰ê”EÒ",
	"‹|‹R”n",
	"“S–C",
	"‹R”n“S–C"
};

char *IsButaiArrayKirigakure[4] = {
	"ˆÉ‰ê”EÒ",
	"‹R”n",
	"ˆÉ‰ê“S–C",
	"‹R”n“S–C"
};

char *IsButaiArrayKirigakureYumi[4] = {
	"ˆÉ‰ê”EÒ",
	"‹|‹R”n",
	"ˆÉ‰ê“S–C",
	"‹R”n“S–C"
};

char *IsButaiArrayNinja[4] = {
	"”EÒ",
	"‹R”n",
	"“S–C",
	"‹R”n“S–C"
};


char *IsButaiArrayOda[4] = {
	"D“c‘„",
	"‹R”n",
	"D“c“S–C",
	"‹R”n“S–C"
};

char *IsButaiArrayOdaYumi[4] = {
	"D“c‹|",
	"‹|‹R”n",
	"D“c“S–C",
	"‹R”n“S–C"
};

char *IsButaiArraySouryo[4] = {
	"“ã“‘m",
	"‹R”n",
	"‹R”n",
	"‹R”n“S–C"
};

char *IsButaiArraySouryoYumi[4] = {
	"‹|‘m",
	"‹|‹R”n",
	"“S–C",
	"‹R”n“S–C"
};

char *IsButaiArrayTakedaHigh[4] = {
	"˜Y“}˜",
	"•“cŠø–{",
	"“S–C",
	"‹R”n“S–C"
};

char *IsButaiArrayTakedaYari[4] = {
	"‘«Œy",
	"•“c‘„‹R",
	"“S–C",
	"‹R”n“S–C"
};

char *IsButaiArrayTakedaKatana[4] = {
	"‘«Œy",
	"•“c“‹R",
	"“S–C",
	"‹R”n“S–C"
};

char *IsButaiArrayTakedaYumi[4] = {
	"‹|‘«Œy",
	"•“c‹|‹R",
	"“S–C",
	"‹R”n“S–C"
};

char *IsButaiArrayTakedaWoman[4] = {
	"—•m",
	"—‹R”n",
	"“S–C",
	"‹R”n“S–C"
};

char *IsButaiArrayTakedaWomanYumi[4] = {
	"—•m",
	"—‹|‹R”n",
	"“S–C",
	"‹R”n“S–C"
};

char *IsButaiArrayHoujoTeppoGreat[4] = {
	"–kğ‘å“›",
	"–kğ‘å“›",
	"–kğ‘å“›",
	"–kğ‘å“›"
};

char *IsButaiArrayShiroKishi[4] = {
	"”’‹Rm",
	"”’‹R”n",
	"“S–C‹Rm",
	"‹R“S‹Rm",
};
char *IsButaiArrayMahou[4] = {
	"–‚–@",
	"–‚–@‹R”n",
	"–‚–@“S–C",
	"–‚–@‹R“S",
};
char *IsButaiArrayMajuu[4] = {
	"–‚b",
	"–‚b‹R”n",
	"–‚b“S–C",
	"–‚b‹R“S",
};


char *IsButaiArrayHighKatana[4] = {
	"“˜",
	"Šø–{‹R”n",
	"“S–C˜",
	"‹R”n“S–C"
};

char *IsButaiArrayHighYari[4] = {
	"‘„˜",
	"Šø–{‘„‹R",
	"“S–C˜",
	"‹R”n“S–C"
};

char *IsButaiArrayHighYumi[4] = {
	"‹|˜",
	"Šø–{‹|‹R",
	"“S–C˜",
	"‹R”n“S–C"
};

char *IsButaiArrayKatanaHigh[4] = {
	"“˜",
	"“˜",
	"“˜",
	"“˜"
};

char *IsButaiArrayKatanaMid[4] = {
	"–ì‘¾“˜",
	"–ì‘¾“˜",
	"–ì‘¾“˜",
	"–ì‘¾“˜"
};

char *IsButaiArrayNaginagaHigh[4] = {
	"“ã“˜",
	"“ã“˜",
	"“ã“˜",
	"“ã“˜"
};

char *IsButaiArrayYariHigh[4] = {
	"‘„˜",
	"‘„˜",
	"‘„˜",
	"‘„˜"
};

char *IsButaiArrayYari[4] = {
	"‘„‘«Œy",
	"‘„‘«Œy",
	"‘„‘«Œy",
	"‘„‘«Œy"
};

char *IsButaiArrayKibaYariHigh[4] = {
	"’·‘„‹R”n",
	"’·‘„‹R”n",
	"’·‘„‹R”n",
	"’·‘„‹R”n"
};

char *IsButaiArrayKibaYumi[4] = {
	"‹|‹R”n",
	"‹|‹R”n",
	"‹|‹R”n",
	"‹|‹R”n"
};

char *IsButaiArrayYumiHigh[4] = {
	"‹|˜",
	"‹|˜",
	"‹|˜",
	"‹|˜"
};

char *IsButaiArrayYumi[4] = {
	"‹|‘«Œy",
	"‹|‘«Œy",
	"‹|‘«Œy",
	"‹|‘«Œy"
};

char *IsButaiArrayTeppoHigh[4] = {
	"“S–C˜",
	"“S–C˜",
	"“S–C˜",
	"“S–C˜"
};

char *IsButaiArrayTeppoGreat[4] = {
	"‘å“›",
	"‘å“›",
	"‘å“›",
	"‘å“›"
};

char *IsButaiArrayUmiueKurohune[4] = {
	"•‘D",
	"•‘D",
	"•‘D",
	"•‘D"
};
char *IsButaiArrayUmiueNihonmaru[4] = {
	"“ú–{ŠÛ",
	"“ú–{ŠÛ",
	"“ú–{ŠÛ",
	"“ú–{ŠÛ"
};
char *IsButaiArrayUmiueDaiAtake[4] = {
	"‘åˆÀ‘î‘D",
	"‘åˆÀ‘î‘D",
	"‘åˆÀ‘î‘D",
	"‘åˆÀ‘î‘D"
};
char *IsButaiArrayUmiueDaiAtakeWithDetail[4] = {
	" ‘åˆÀ‘î‘D (‘«Œy) ",
	" ‘åˆÀ‘î‘D (‹R”n) ",
	" ‘åˆÀ‘î‘D (“S–C) ",
	" ‘åˆÀ‘î‘D (‹R“S) "
};

char *IsButaiArrayUmiueAtake[4] = {
	"ˆÀ‘î‘D",
	"ˆÀ‘î‘D",
	"ˆÀ‘î‘D",
	"ˆÀ‘î‘D"
};
char *IsButaiArrayUmiueAtakeWithDetail[4] = {
	" ˆÀ‘î‘D (‘«Œy) ",
	" ˆÀ‘î‘D (‹R”n) ",
	" ˆÀ‘î‘D (“S–C) ",
	" ˆÀ‘î‘D (‹R“S) "
};
char *IsButaiArrayUmiueSenseki[4] = {
	"çÎ‘D",
	"çÎ‘D",
	"çÎ‘D",
	"çÎ‘D"
};
char *IsButaiArrayUmiueSensekiWithDetail[4] = {
	" çÎ‘D (‘«Œy) ",
	" çÎ‘D (‹R”n) ",
	" çÎ‘D (“S–C) ",
	" çÎ‘D (‹R“S) "
};
char *IsButaiArrayUmiueSekibune[4] = {
	"ŠÖ‘D",
	"ŠÖ‘D",
	"ŠÖ‘D",
	"ŠÖ‘D"
};
char *IsButaiArrayUmiueSekibuneWithDetail[4] = {
	" ŠÖ‘D (‘«Œy) ",
	" ŠÖ‘D (‹R”n) ",
	" ŠÖ‘D (“S–C) ",
	" ŠÖ‘D (‹R“S) "
};
char *IsButaiArrayUmiueIrou[4] = {
	"ˆä˜O‘D",
	"ˆä˜O‘D",
	"“S–C¬‘",
	"“S–C¬‘"
};
char *IsButaiArrayUmiueIrouWithDetail[4] = {
	" ˆä˜O‘D (‘«Œy) ",
	" ˆä˜O‘D (‹R”n) ",
	" “S–C¬‘ (“S–C) ",
	" “S–C¬‘ (‹R“S) "
};
char *IsButaiArrayUmiueMini[4] = {
	"¬‘",
	"¬‘",
	"“S–C¬‘",
	"“S–C¬‘"
};
char *IsButaiArrayUmiueMiniWithDetail[4] = {
	" ¬‘ (‘«Œy) ",
	" ¬‘ (‹R”n) ",
	" “S–C¬‘ (“S–C) ",
	" “S–C¬‘ (‹R“S) "
};
char *IsButaiArrayUmiueMiniYumi[4] = {
	"‹|¬‘",
	"‹|¬‘",
	"“S–C¬‘",
	"“S–C¬‘"
};
char *IsButaiArrayUmiueMiniYumiWithDetail[4] = {
	" ‹|¬‘ (‘«Œy) ",
	" ‹|¬‘ (‹R”n) ",
	" “S–C¬‘ (“S–C) ",
	" “S–C¬‘ (‹R“S) "
};

// "\xec\x8c\xec\x8d\xec\x8e";//’·@‰ä•”
// "\xec\x8f\xec\x8d\xec\x8e";//@‰ä•”
char *IsButaiArrayChosokabeYumi[4] = {
	"\xec\x8c\xec\x8d\xec\x8e‹|",
	"‹R”n",
	"“S–C",
	"‹R”n“S–C"
};


// NULL‚ğ•Ô‚¹‚ÎA•ÏX‚µ‚È‚¢‚Æ‚¢‚¤‚±‚ÆB
int GetOverWriteButaiMeiArrayPointer(int iBushouID) {

	if ( !(0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) ) {
		return NULL;
	}

	// ScenarioMod‚ğ“±“ü‚µ‚Ä‚¢‚éB
	if ( p_snOnViewingButaiName ) {
		int iSceanrioModValidButaiName = p_snOnViewingButaiName( iBushouID + 1 );

		// —LŒø‚È•ÏX‚ª‚³‚ê‚Ä‚¢‚é‚Ì‚Å‚ ‚ê‚ÎA‚»‚ê‚ğ•Ô‚·B
		if ( iSceanrioModValidButaiName ) {
			return iSceanrioModValidButaiName;
		}
	}


	// ¡í“¬‚µ‚Ä‚¢‚ÄAŠC‚Ìã‚É‚¢‚é
	HEX_POSITION p = GetFieldHexUnitPos( iBushouID );
	// •«‚ÌˆÊ’u‚Ì–ğŠ„‚ğ“¾‚é
	int iHexRole = GetFieldHexRole( p );

	//------------------------------------------ˆÈ‰ºA‘å–¼‰Æ‚æ‚è‚à—Dæ‚·‚é
	// ƒ^ƒ‹ƒ^ƒƒX‚È‚ç‚Î
	if ( isLancelotTartare( iBushouID ) ) {
		// ŠC‚Ìã
		if ( Is_FieldWar() && (iHexRole == FIELD_HEX_ROLE::ŠC || iHexRole == FIELD_HEX_ROLE::ŒÎ) ) {
			// “Sb‘D‚ğ‚Á‚Ä‚¢‚é
			if (nb6bushouref[iBushouID].steelship) {
				return int(IsButaiArrayUmiueKurohune);
			}
		}
		// ŠC‚ÌãˆÈŠO
		// •”‘à•Ò¬ƒ‚[ƒh
		if ( isOnEditingButaiMode ) {
			return int(IsButaiArrayLoslorienWithDetail);
		} else {
			return int(IsButaiArrayLoslorien);
		}
	}

	// ŠC‚Ìã
	if ( Is_FieldWar() && (iHexRole == FIELD_HEX_ROLE::ŠC || iHexRole == FIELD_HEX_ROLE::ŒÎ) ) {
		// Å‘å¨—Í‚Ì‘å–¼
		int iDaimyoID = nb6bushouref[iBushouID].attach -1;
		int iMaximumInfluenceDaimyo = GetMaximumInfluenceDaimyo();
		// Å‘å¨—Í‚Ì‘å–¼©g‚Å‚ ‚èA–ì–]‚ª‚‚¢
		if ( nb6bushouname[iBushouID].position == 1 && (iDaimyoID == iMaximumInfluenceDaimyo) && nb6bushouref[iBushouID].ambition > 80 && nb6bushouref[iBushouID].steelship ) {
			return int(IsButaiArrayUmiueNihonmaru);
		}

		// “Sb‘D‚ğ‚Á‚Ä‚¢‚é
		if (nb6bushouref[iBushouID].steelship) {
			return int(IsButaiArrayUmiueDaiAtake);
		}

		// ‰B‹=0, ‘å–¼=1, h˜V=2, ‰Æ˜V=3, •”«=4, ˜‘å«=5, ‘«Œy“ª=6
		if (nb6bushouname[iBushouID].position == 1 ) {
			return int(IsButaiArrayUmiueDaiAtake);
		}

		// ‰B‹=0, ‘å–¼=1, h˜V=2, ‰Æ˜V=3, •”«=4, ˜‘å«=5, ‘«Œy“ª=6
		if ( nb6bushouname[iBushouID].position == 2 ) {
			return int(IsButaiArrayUmiueAtake);
		}

		// ‰B‹=0, ‘å–¼=1, h˜V=2, ‰Æ˜V=3, •”«=4, ˜‘å«=5, ‘«Œy“ª=6
		if (nb6bushouname[iBushouID].position == 3 ) {
			return int(IsButaiArrayUmiueSenseki);
		}

		// ‰B‹=0, ‘å–¼=1, h˜V=2, ‰Æ˜V=3, •”«=4, ˜‘å«=5, ‘«Œy“ª=6
		if (nb6bushouname[iBushouID].position == 4 ) {
			return int(IsButaiArrayUmiueSekibune);
		}

		// ‰B‹=0, ‘å–¼=1, h˜V=2, ‰Æ˜V=3, •”«=4, ˜‘å«=5, ‘«Œy“ª=6
		if (nb6bushouname[iBushouID].position == 5 ) {
			return int(IsButaiArrayUmiueIrou);
		}

		// ‰B‹=0, ‘å–¼=1, h˜V=2, ‰Æ˜V=3, •”«=4, ˜‘å«=5, ‘«Œy“ª=6
		if (nb6bushouname[iBushouID].position == 6 ) {
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 ) {
				return int(IsButaiArrayUmiueMiniYumi);
			} else {
				return int(IsButaiArrayUmiueMini);
			}
		}
	}

	// ƒƒXƒ[ƒŠƒAƒ“‚È‚ç‚Îc
	if ( IsLoslorien( iBushouID ) ) {
		// •”‘à•Ò¬ƒ‚[ƒh
		if ( isOnEditingButaiMode ) {
			return int(IsButaiArrayLoslorienWithDetail);
		} else {
			return int(IsButaiArrayLoslorien);
		}
	}

	// ƒEƒH[ƒŒƒ“‚È‚ç‚Îc–‚pt‚Å‚Í‚È‚­è¯pt‚Æ‚·‚é
	if ( strcmp(nb6bushouname[iBushouID].familyname, "ğqğr")==0 && strcmp(nb6bushouname[iBushouID].fastname, "ğnğoğp")==0 ) {
		return int(IsButaiArrayMahou);
	}
	// ƒJƒm[ƒvƒX‚È‚ç‚Îc–‚l‚Å‚Í‚È‚­—L—ƒl‚Æ‚·‚é
	if ( strcmp(nb6bushouname[iBushouID].familyname, "ğOğu")==0 && strcmp(nb6bushouname[iBushouID].fastname, "ğ[ğ\ğ]")==0 ) {
		return int(IsButaiArrayMajuu);
	}
	// ƒMƒ‹ƒoƒ‹ƒh‚Å‚ ‚ê‚Îc‹Rm‚Å‚Í‚È‚­bg‚¢‚Æ‚·‚é
	if ( strcmp(nb6bushouname[iBushouID].familyname, "ğgğhği")==0 && strcmp(nb6bushouname[iBushouID].fastname, "ğœğğ")==0 ) {
		return int(IsButaiArrayMajuu);
	}
	// ƒ~ƒ‹ƒfƒBƒ“‚Ü‚ç‚ÎA”’‹Rm‚Æ‚·‚é
	if ( strcmp(nb6bushouname[iBushouID].familyname, "ğOğPğQ")==0 && strcmp(nb6bushouname[iBushouID].fastname, "ğRğSğT")==0 ) {
		return int(IsButaiArrayShiroKishi);
	}
	// ƒMƒ‹ƒ_ƒX‚È‚ç‚Îc”’‹Rm‚Æ‚·‚é
	if	(strcmp(nb6bushouname[iBushouID].familyname, "ğGğHğF")==0 && strcmp(nb6bushouname[iBushouID].fastname, "„Š„‹„Œ")==0) {
		return int(IsButaiArrayShiroKishi);
	}		
	// ƒfƒlƒu‚È‚ç‚Îc–‚pt‚Å‚Í‚È‚­–‚—‚Æ‚·‚é
	if ( strcmp(nb6bushouname[iBushouID].familyname, "ğjğk")==0 && strcmp(nb6bushouname[iBushouID].fastname, "ğlğm")==0 ) {
		return int(IsButaiArrayMahou);
	}
	// ƒ‰ƒhƒ‰ƒ€‚È‚ç‚Îc–‚pt‚Å‚Í‚È‚­–‚“±m‚Æ‚·‚é
	if ( strcmp(nb6bushouname[iBushouID].familyname, "ğyğzğ{")==0 && strcmp(nb6bushouname[iBushouID].fastname, "ğ|ğ}ğ~")==0 ) {
		return int(IsButaiArrayMahou);
	}



	// ••””¼‘ ‚Ìê‡A•”‘à–¼”z—ñ‚ğ•ÏX‚·‚éB
	if ( strcmp(nb6bushouname[iBushouID].familyname, "••”") == 0 && nb6bushouname[iBushouID].number == 108 ) {
		// ”EÒ‚Ìê‡
		if ( nb6bushouref[iBushouID].job == 0x10 ) {
			// ‹|‚ªg‚¦‚é•«
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 0,2)) {
				return int(IsButaiArrayHattoriYumi);
			} 
			// ’Êí•«
			if ( IsBushouForm(iBushouID, 0,2) ) {
				return int(IsButaiArrayHattori);
			}
		}
	}
	// •—–‚¬‘¾˜Y‚Ìê‡A•”‘à–¼”z—ñ‚ğ•ÏX‚·‚éB
	if ( strcmp(nb6bushouname[iBushouID].familyname, "•—–‚") == 0 && nb6bushouname[iBushouID].number == 119 ) {
		// ”EÒ‚Ìê‡
		if ( nb6bushouref[iBushouID].job == 0x10 ) {
			// ‹|‚ªg‚¦‚é•«
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 0)) {
				return int(IsButaiArrayHuumaYumi);
			}
			// ’Êí•«
			if ( IsBushouForm(iBushouID, 0) ) {
				return int(IsButaiArrayHuuma);
			}
		}
	}
	// •S’n’O”g‚Ìê‡A•”‘à–¼”z—ñ‚ğ•ÏX‚·‚éB
	if ( strcmp(nb6bushouname[iBushouID].familyname, "•S’n") == 0 && nb6bushouname[iBushouID].number == 328 ) {
		// ”EÒ‚Ìê‡
		if ( nb6bushouref[iBushouID].job == 0x10 ) {
			// ‹|‚ªg‚¦‚é•«
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 0,2) ) {
				return int(IsButaiArrayMomochiYumi);
			}
			// ’Êí•«
			if ( IsBushouForm(iBushouID, 0,2) ) {
				return int(IsButaiArrayMomochi);
			}
		}
	}

	// ‰”ò²•‚Ìê‡A•”‘à–¼”z—ñ‚ğ•ÏX‚·‚éB
	if ( strcmp(nb6bushouname[iBushouID].familyname, "‰”ò") == 0 && nb6bushouname[iBushouID].number == 361 ) {
		// ”EÒ‚Ìê‡
		if ( nb6bushouref[iBushouID].job == 0x10 ) {
			// ‹|‚ªg‚¦‚é•«
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 0) ) {
				return int(IsButaiArraySarutobiYumi);
			}
			// ’Êí•«
			if ( IsBushouForm(iBushouID, 0) ) {
				return int(IsButaiArraySarutobi);
			}
		}
	}

	// –¶‰BË‘ ‚Ìê‡A•”‘à–¼”z—ñ‚ğ•ÏX‚·‚éB
	if ( strcmp(nb6bushouname[iBushouID].familyname, "–¶‰B") == 0 && nb6bushouname[iBushouID].number == 362 ) {
		// ”EÒ‚Ìê‡
		if ( nb6bushouref[iBushouID].job == 0x10 ) {
			// ‹|‚ªg‚¦‚é•«
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 0) ) {
				return int(IsButaiArrayKirigakureYumi);
			}
			// ’Êí•«
			if ( IsBushouForm(iBushouID, 0) ) {
				return int(IsButaiArrayKirigakure);
			}
		}
	}

	// ”EÒ‚Ìê‡
	if ( nb6bushouref[iBushouID].job == 0x10 ) {
		if ( IsBushouForm(iBushouID, 0) ) {
			return int(IsButaiArrayNinja);
		}
	}

	// ‘å–¼‚ÌŒŒ‹Ø‚Å•ª‚¯‚Ä‚¢‚­
	int iDaimyoID = nb6bushouref[iBushouID].attach -1;
	int iGundanID = nb6bushouref[iBushouID].gundan -1;

	if ( (0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM) && (0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM) ) {
		int iDaimyoBushouID = nb6daimyo[iDaimyoID].attach - 1;
		int iGundanBushouID = nb6gundan[iGundanID].leader - 1;

		// –{Šè›‰Æ‚Ìê‡A•”‘à–¼”z—ñ‚ğ•ÏX‚·‚éB
		if ( nb6bushouref[iDaimyoBushouID].blood == 41 ) {
			// ‹|‚ªg‚¦‚é•«
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 2) ) {
				return int(IsButaiArrayIkkousyuYumi);
			}
			// ’Êí•«
			if ( IsBushouForm(iBushouID, 0, 2) ) {
				return int(IsButaiArrayIkkousyu);
			}

		} 

		// •cš‚ªã™‚ÅAã™ŒªM‚à‚µ‚­‚ÍAã™ŒiŸ‚Ìê‡A•”‘à–¼”z—ñ‚ğ•ÏX‚·‚éB
		if ( strcmp(nb6bushouname[iDaimyoBushouID].familyname, "ã™")==0 && (nb6bushouname[iDaimyoBushouID].number == 26 || nb6bushouname[iDaimyoBushouID].number == 27) ) {

			// ‹|‚ªg‚¦‚é•«
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 0) ) {
				return int(IsButaiArrayUesughiYumi);
			}
			// ’Êí•«
			if ( IsBushouForm(iBushouID, 0) ) {
				return int(IsButaiArrayUesughi);
			}
		}

		// “‡’Ã‰Æ‚Ìê‡A•”‘à–¼”z—ñ‚ğ•ÏX‚·‚éB
		if ( nb6bushouref[iDaimyoBushouID].blood == 6 ) {
			// ’Êí•«
			if ( IsBushouForm(iBushouID, 0) && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi < 3 ) {
				return int(IsButaiArrayShimazu);
			}
		}

		// ’·@‰ä•”‚Ìê‡A•”‘à–¼”z—ñ‚ğ•ÏX‚·‚éB
		if ( nb6bushouref[iDaimyoBushouID].blood == 11 ) {
			// ‹|‚ªg‚¦‚é•«
			if ( IsBushouForm(iBushouID, 0) && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 ) {
				return int(IsButaiArrayChosokabeYumi);
			}
		}

		// ˆÉ’B‰Æ‚Ìê‡A•”‘à–¼”z—ñ‚ğ•ÏX‚·‚éB
		if ( nb6bushouref[iDaimyoBushouID].blood == 7 ) {
			// ‹|‚ªg‚¦‚é•«
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 3) ) {
				return int(IsButaiArrayDateYumi);
			}
			// “•«
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari <= 2 && IsBushouForm(iBushouID, 0, 3) ) {
				return int(IsButaiArrayDate);
			}

		}

		// ‘å–¼‚ªD“cM’·‚Ìê‡A•”‘à–¼”z—ñ‚ğ•ÏX‚·‚éB
		if ( strcmp(nb6bushouname[iDaimyoBushouID].familyname, "D“c")==0 && nb6bushouname[iDaimyoBushouID].number == 0 ) {
			// ‹|‚ªg‚¦‚é•«
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 0, 2) ) {
				return int(IsButaiArrayOdaYumi);
			}
			// ‘„Œn
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari >= 3 && IsBushouForm(iBushouID, 0, 2) ) {
				return int(IsButaiArrayOda);
			}
			// ’Êí
			if ( IsBushouForm(iBushouID, 2) ) {
				return int(IsButaiArrayOda);
			}
		}

		// ‘å–¼‚ª•“c‰Æ‚Ìê‡A•”‘à–¼”z—ñ‚ğ•ÏX‚·‚éB
		if ( nb6bushouref[iDaimyoBushouID].blood == 1 ) {
			// í“¬‚ª150ˆÈã‚É‚È‚Á‚½‚çA—DG‚È•«‚Å‚ ‚ê‚ÎA‘æˆêŒR’c’¼‰º
			if ( getBushouBat(iBushouID) > 75 && (iDaimyoBushouID==iGundanBushouID) && nb6bushouname[iBushouID].State != 0 && (nb6bushouref[iBushouID].maxbat + nb6bushouref[iBushouID].maxgov + nb6bushouref[iBushouID].maxint) >= 200 ) {
				if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi <= 2 && IsBushouForm(iBushouID, 0) ) {
					return int(IsButaiArrayTakedaHigh);
				}
				if ( IsBushouForm(iBushouID, 1) ) {
					return int(IsButaiArrayTakedaHigh);
				}
			}
			// ‹|‚ªg‚¦‚é•«
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 ) {
				if ( nb6bushouname[iBushouID].sex == 0 && IsBushouForm(iBushouID, 1) ) {
					return int(IsButaiArrayTakedaYumi);
				}
				if ( nb6bushouname[iBushouID].sex == 1 && IsBushouForm(iBushouID, 0, 1) ) {
					return int(IsButaiArrayTakedaWomanYumi);
				}
				// ’Êí•«
			} else {
				if ( nb6bushouname[iBushouID].sex == 0 && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari >= 3 && IsBushouForm(iBushouID, 1) ) {
					return int(IsButaiArrayTakedaYari);
				}
				if ( nb6bushouname[iBushouID].sex == 0 && IsBushouForm(iBushouID, 1) ) {
					return int(IsButaiArrayTakedaKatana);
				}
				if ( nb6bushouname[iBushouID].sex == 1 && IsBushouForm(iBushouID, 0, 1) ) {
					return int(IsButaiArrayTakedaWoman);
				}
			}
		}

		// –kğ‰Æ‚Ìê‡A•”‘à–¼”z—ñ‚ğ•ÏX‚·‚éB
		if ( nb6bushouref[iDaimyoBushouID].blood == 8 ) {
			// “S–C‚ªg‚¦‚é•«
			if ( IsBushouForm(iBushouID, 2) ) {
				// í“¬‚ª170ˆÈã‚É‚È‚Á‚½‚çA‹R”n“K³‚ª‚`ˆÈã
				if ( getBushouBat(iBushouID) > 85 && getBushouAptitude(iBushouID, APTITUDE_TEPPOU) >= 4) {
					return int(IsButaiArrayHoujoTeppoGreat);
				}
			}
		}


		//------------------------------------------ˆÈ‰ºA‘å–¼‰Æ‚æ‚è‚à—Dæ“x‚Í—ò‚é•’Ê‚Ì‚à‚Ì
		// ‘m—µ‚Ìê‡A•”‘à–¼”z—ñ‚ğ•ÏX‚·‚éB
		// ‘m—µ‚Ìê‡
		if ( nb6bushouref[iBushouID].job == 0x40 ) {
			// ‹|‚ªg‚¦‚é•«
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 0) ) {
				return int(IsButaiArraySouryoYumi);
			}
			if ( IsBushouForm(iBushouID, 0) ) {
				return int(IsButaiArraySouryo);
			}

		}

		// ŠCŒn‚Å‚ ‚ê‚Îc
		if ( nb6bushouref[iBushouID].job==0x50 ) { // ŠC‘¯‚©…ŒR“K³‚aˆÈã

			// “Sb‘D‚ğ‚Á‚Ä‚¢‚é
			if (nb6bushouref[iBushouID].steelship) {
				// •”‘à•ÒWƒ‚[ƒh
				if ( isOnEditingButaiMode ) {
					return int(IsButaiArrayUmiueDaiAtakeWithDetail);
				} else {
					return int(IsButaiArrayUmiueDaiAtake);
				}
			}

			// ‰B‹=0, ‘å–¼=1, h˜V=2, ‰Æ˜V=3, •”«=4, ˜‘å«=5, ‘«Œy“ª=6
			if (nb6bushouname[iBushouID].position == 1 ) {
				// •”‘à•ÒWƒ‚[ƒh
				if ( isOnEditingButaiMode ) {
					return int(IsButaiArrayUmiueDaiAtakeWithDetail);
				} else {
					return int(IsButaiArrayUmiueDaiAtake);
				}
			}

			// ‰B‹=0, ‘å–¼=1, h˜V=2, ‰Æ˜V=3, •”«=4, ˜‘å«=5, ‘«Œy“ª=6
			if ( nb6bushouname[iBushouID].position == 2 ) {
				// •”‘à•ÒWƒ‚[ƒh
				if ( isOnEditingButaiMode ) {
					return int(IsButaiArrayUmiueAtakeWithDetail);
				} else {
					return int(IsButaiArrayUmiueAtake);
				}
			}

			// ‰B‹=0, ‘å–¼=1, h˜V=2, ‰Æ˜V=3, •”«=4, ˜‘å«=5, ‘«Œy“ª=6
			if (nb6bushouname[iBushouID].position == 3 ) {
				// •”‘à•ÒWƒ‚[ƒh
				if ( isOnEditingButaiMode ) {
					return int(IsButaiArrayUmiueSensekiWithDetail);
				} else {
					return int(IsButaiArrayUmiueSenseki);
				}
			}

			// ‰B‹=0, ‘å–¼=1, h˜V=2, ‰Æ˜V=3, •”«=4, ˜‘å«=5, ‘«Œy“ª=6
			if (nb6bushouname[iBushouID].position == 4 ) {
				// •”‘à•ÒWƒ‚[ƒh
				if ( isOnEditingButaiMode ) {
					return int(IsButaiArrayUmiueSekibuneWithDetail);
				} else {
					return int(IsButaiArrayUmiueSekibune);
				}
			}

			// ‰B‹=0, ‘å–¼=1, h˜V=2, ‰Æ˜V=3, •”«=4, ˜‘å«=5, ‘«Œy“ª=6
			if (nb6bushouname[iBushouID].position == 5 ) {
				// •”‘à•ÒWƒ‚[ƒh
				if ( isOnEditingButaiMode ) {
					return int(IsButaiArrayUmiueIrouWithDetail);
				} else {
					return int(IsButaiArrayUmiueIrou);
				}
			}

			// ‰B‹=0, ‘å–¼=1, h˜V=2, ‰Æ˜V=3, •”«=4, ˜‘å«=5, ‘«Œy“ª=6
			if (nb6bushouname[iBushouID].position == 6 ) {
				if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 ) {
					// •”‘à•ÒWƒ‚[ƒh
					if ( isOnEditingButaiMode ) {
						return int(IsButaiArrayUmiueMiniYumiWithDetail);
					} else {
						return int(IsButaiArrayUmiueMiniYumi);
					}
				} else {
					// •”‘à•ÒWƒ‚[ƒh
					if ( isOnEditingButaiMode ) {
						return int(IsButaiArrayUmiueMiniWithDetail);
					} else {
						return int(IsButaiArrayUmiueMini);
					}
				}
			}
		}

		// í“¬‚ª150ˆÈã‚É‚È‚Á‚½‚çA—DG‚È•«‚Å‚ ‚ê‚ÎA‘æˆêŒR’c’¼‰º
		if ( getBushouBat(iBushouID) > 75 && (iDaimyoBushouID==iGundanBushouID) && nb6bushouname[iBushouID].State != 0 && (nb6bushouref[iBushouID].maxbat + nb6bushouref[iBushouID].maxgov + nb6bushouref[iBushouID].maxint) >= 200 ) {
			// ‹|‚ªg‚¦‚é•«
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 1) ) {
				return int(IsButaiArrayHighYumi);
			}
			// ‘„‚ªg‚¦‚é•«
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari >= 3 && IsBushouForm(iBushouID, 1) ) {
				return int(IsButaiArrayHighYari);
			}
			if ( IsBushouForm(iBushouID, 1) ) {
				return int(IsButaiArrayHighKatana);
			}
		}

		// “S–C‚Ì•«
		if ( IsBushouForm(iBushouID, 2) ) {

			// í“¬‚ª170ˆÈã‚É‚È‚Á‚½‚çA‹R”n“K³‚ª‚`ˆÈã
			if ( getBushouBat(iBushouID) > 85 && getBushouAptitude(iBushouID, APTITUDE_TEPPOU) >= 4) {
				return int(IsButaiArrayTeppoGreat);
			}
			// í“¬‚ª150ˆÈã‚É‚È‚Á‚½‚çA‹R”n“K³‚ª‚aˆÈã
			if ( getBushouBat(iBushouID) > 75 && getBushouAptitude(iBushouID, APTITUDE_TEPPOU) >= 3) {
				return int(IsButaiArrayTeppoHigh);
			}
		}

		// ‹|‚Ì•«
		if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 0) ) {
			// í“¬‚ª150ˆÈã‚É‚È‚Á‚½‚çA‘«Œy“K³‚ª‚aˆÈã
			if ( getBushouBat(iBushouID) > 75 && getBushouAptitude(iBushouID, APTITUDE_ASIGARU) >= 3) {
				return int(IsButaiArrayYumiHigh);
			}
			return int(IsButaiArrayYumi);
		}

		// ‹|‚Ì‹R”n•«
		if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 1) ) {
			return int(IsButaiArrayKibaYumi);
		}

		// ‘„‚Ì‹R”n•«
		if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari > 3 && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi <= 2 && IsBushouForm(iBushouID, 1) ) {

			// í“¬‚ª150ˆÈã‚É‚È‚Á‚½‚çA‹R”n“K³‚ª‚aˆÈã
			if ( getBushouBat(iBushouID) > 75 && getBushouAptitude(iBushouID, APTITUDE_KIBA) >= 3) {
				return int(IsButaiArrayKibaYariHigh);
			}
		}

		// ‘„‚Ì‘«Œy•«
		if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari >= 3 && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi <= 2 && IsBushouForm(iBushouID, 0) ) {

			// í“¬‚ª150ˆÈã‚É‚È‚Á‚½‚çA‘«Œy“K³‚ª‚aˆÈã
			if ( getBushouBat(iBushouID) > 75 && getBushouAptitude(iBushouID, APTITUDE_ASIGARU) >= 3) {
				return int(IsButaiArrayYariHigh);
			}
			// ‘«Œy“K³‚ª‚bˆÈã
			if ( getBushouAptitude(iBushouID, APTITUDE_ASIGARU) >= 2) {
				return int(IsButaiArrayYari);
			}
		}

		// “‚Ì‘«Œy•«(ˆê”Ô“Á’¥‚ª‚È‚¢)
		if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari <= 2 && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi <= 2 && IsBushouForm(iBushouID, 0) ) {

			// í“¬‚ª150ˆÈã‚É‚È‚Á‚½‚çA‘«Œy“K³‚ª‚aˆÈã
			if ( getBushouBat(iBushouID) > 75 && getBushouAptitude(iBushouID, APTITUDE_ASIGARU) >= 3) {
				return int(IsButaiArrayKatanaHigh);
			}
			// í“¬‚ª120ˆÈã‚É‚È‚Á‚½‚çA‘«Œy“K³‚ª‚bˆÈã
			if ( getBushouBat(iBushouID) > 60 && getBushouAptitude(iBushouID, APTITUDE_ASIGARU ) >= 2) {
				return int(IsButaiArrayKatanaMid);
			}

		}


	}


	return NULL;
}