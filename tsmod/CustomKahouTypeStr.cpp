#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include <windows.h>

#include "GameDataStruct.h"
#include "TSModCommand.h"

#include "KahouType.h"


CUSTOM_KAHOU_TYPE_STRING nb6kahou_custom_type_string[GAMEDATASTRUCT_KAHOU_NUM] = {""};


// ĆóíŢĚ^CvĚJX^śńÖĚ|C^[đžé
char *GetOverWriteKahouTypeMeiPointer(int iKahouID) {

	// .jpÁęĆóĚśÝŞnm
	if ( !TSModCommand::World::cmdCustomKahouExists ) {
		return NULL;
	}

	char *p = nb6kahou[iKahouID].name;
	if ( strstr( p, "cÍ" ) ) {
		return "cÍ";
	} else if ( strcmp( p, "Šŕqř" ) == 0 ) {
		return "ř";
	} else if ( strcmp( p, "uçŰÚg" ) == 0 ) { // uqg
		return "FÍ";
	} else if ( strcmp( p, "oobT" ) == 0 ) {
		return "FÍ";
	} else if ( strcmp( p, "xCIEt" ) == 0 ) {
		return "FÍ";
	} else if ( strcmp( p, "gX^" ) == 0 ) {
		return "FÍ";
	} else if ( strstr( p, "ńüč" ) ) {
		return "ńüč";
	} else if ( strcmp( p, "Ł" ) == 0 ) {
		return "Ű";
	} else if ( strcmp( p, "đŞ" ) == 0 ) {
		return "w";
	} else if ( strcmp( p, "ÎŽ" ) == 0 ) {
		return "F";
	} else if ( strcmp( p, "çš" ) == 0 ) {
		return "F";
	} else if ( strcmp( p, "ptct" ) == 0 ) {
		return "t";
	} else if ( strcmp( p, "št" ) == 0 ) {
		return "t";
	} else if ( strcmp( p, "TVI" ) == 0 ) {
		return "Ć";
	} else if ( strcmp( p, "|}~" ) == 0 ) { // v`[[Y
		return "Ú";
	} else if ( strcmp( p, "_Ě`" ) == 0 ) {
		return "Ú";
	} else if ( strstr( p, "Ěń" ) ) {
		return "ń";
	} else if ( strstr( p, "Ěe" ) ) {
		return "SC";
	} else if ( strcmp( p, "TUNX" ) == 0 ) {
		return "ďŤ";
	} else if ( strcmp( p, "¤Ěš" ) == 0 ) {
		return "ß";
	} else if ( strcmp( p, "[ŁĚ[u" ) == 0 ) {
		return "ß";
	} else if ( strcmp( p, "`á" ) == 0 ) {
		return "`á";
	} else if ( strcmp( p, "găíčl`" ) == 0 ) {
		return "l`";
	} else if ( strstr( p, "ĚĆ" ) ) {
		return "Ć";
	} else if ( strcmp( p, "llĚgk" ) == 0) {
		return "Gć";
	} else if ( strcmp( p, "SS^Ěu" ) == 0 ) {
		return "Gć";
	} else if ( strcmp( p, "ÔÖĚšę" ) == 0 ) {
		return "Gć";
	} else if ( strcmp( p, "_Ěaś" ) == 0 ) {
		return "Gć";
	} else if ( strcmp( p, "ĹăĚRť" ) == 0 ) {
		return "Gć";
	} else if ( strcmp( p, "ÓŻŇĚV" ) == 0 ) {
		return "Gć";
	} else if ( strcmp( p, "lĚ}" ) == 0 ) {
		return "f`";
	} else if ( strcmp( p, "iU" ) == 0 ) {
		return "Gć";
	} else if ( strcmp( p, "ňÜtĚŃ" ) == 0 ) {
		return "Gć";
	} else if ( strcmp( p, "}MĚçq" ) == 0 ) {
		return "Gć";
	} else if ( strcmp( p, "LXgôç" ) == 0 ) {
		return "Gć";
	} else if ( strcmp( p, "óŮm" ) == 0 ) {
		return "Gć";
	} else if ( strcmp( p, "ghĚi" ) == 0 ) {
		return "Gć";
	} else if ( strcmp( p, "oxĚ" ) == 0 ) {
		return "Gć";
	} else if ( strcmp( p, "oxĚ" ) == 0 ) {
		return "Gć";
	} else if ( strcmp( p, "jL" ) == 0) {
		return "j";
	} else if ( strcmp( p, "ŕq" ) == 0) {
		return "ş@";
	} else if ( strcmp( p, "ˇq" ) == 0 ) {
		return "ş@";
	} else if ( strcmp( p, "Zčé" ) == 0 ) {
		return "ş@";
	} else if ( strcmp( p, "OŞ" ) == 0 ) {
		return "ş@";
	} else if ( strcmp( p, "bzRÓ" ) == 0 ) {
		return "Rw";
	} else if ( strcmp( p, "đŹ˝@" ) == 0 ) {
		return "ş@";
	} else if ( strcmp( p, "ć_šŹ" ) == 0 ) {
		return "ş@";
	} else if ( strstr( p, "p" ) ) {
		return "ş@";
	} else if ( strstr( p, "|p" ) ) {
		return "ş@";
	} else if ( strcmp( p, "ěWC" ) == 0) {
		return "Ep";
	} else if ( strcmp( p, "ó Ě" ) == 0 ) {
		return "L^";
	} else if ( strcmp( p, "bSČŤT" ) == 0 ) {
		return "ŤT";
	} else if ( strcmp( p, "ěé@" ) == 0 ) {
		return "ą";
	} else if ( strcmp( p, "O[" ) == 0 ) {
		return "ą";
	} else if ( strcmp( p, "łžę@" ) == 0 ) {
		return "ą";
	} else if ( strcmp( p, "Îň˘šĚH" ) == 0 ) {
		return "HŞ";
	} else if ( strcmp( p, "łĘÎ" ) == 0 ) {
		return "ŕó";
	} else if ( strcmp( p, "ŤŇĚ" ) == 0 ) {
		return "_";
	} else if ( strcmp( p, "KXĚěZ" ) == 0 ) {
		return "ŕó";
	} else if ( strcmp( p, "ŕĚ_" ) == 0 ) {
		return "ŕó";
	} else if ( strcmp( p, "ŤűŐ" ) == 0 ) {
		return "ŤűŐ";
	} else if ( strcmp( p, "y_g" ) == 0 ) {
		return "gď";
	} else if ( strcmp( p, "TG" ) == 0 ) {
		return "ŕó";
	} else if ( strcmp( p, "źşĚ_" ) == 0 ) {
		return "Î";
	} else if ( strcmp( p, "NEX" ) == 0 ) {
		return "Î";
	} else if ( strcmp( p, "ZŐ" ) == 0 ) {
		return "ZŐ";
	} else if ( strstr( p, "ć" ) && strstr( p, "ĚŘ" ) ) {
		return "éó";
	} else if ( strcmp( p, "šęq" ) == 0 ) {
		return "šę";
	} else if ( strcmp( p, "ŕĚv" ) == 0 ) {
		return "m";
	} else if ( strcmp( p, "¸đ" ) == 0 ) {
		return "đ";
	}

	return NULL;
}