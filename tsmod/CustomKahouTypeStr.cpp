#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include <windows.h>

#include "GameDataStruct.h"
#include "TSModCommand.h"

#include "KahouType.h"


CUSTOM_KAHOU_TYPE_STRING nb6kahou_custom_type_string[GAMEDATASTRUCT_KAHOU_NUM] = {""};


// ‰Ζ•σν—ή‚Μƒ^ƒCƒv‚ΜƒJƒXƒ^ƒ€•¶—ρ‚Φ‚Μƒ|ƒCƒ“ƒ^[‚π“Ύ‚ι
char *GetOverWriteKahouTypeMeiPointer(int iKahouID) {

	// .jp“Ακ‰Ζ•σ‚Μ‘¶έ‚ª‚n‚m
	if ( !TSModCommand::World::cmdCustomKahouExists ) {
		return NULL;
	}

	char *p = nb6kahou[iKahouID].name;
	if ( strstr( p, "’cΝ" ) ) {
		return "’cΝ";
	} else if ( strcmp( p, "‰©‹ΰ‚qψ" ) == 0 ) {
		return "ψ";
	} else if ( strcmp( p, "ƒuη‡“ΫΪƒg" ) == 0 ) { // ƒuƒƒ…ƒ“ƒqƒ‹ƒg
		return "‰F’Ν";
	} else if ( strcmp( p, "ƒoƒ‹ƒoƒƒbƒT" ) == 0 ) {
		return "‰F’Ν";
	} else if ( strcmp( p, "ƒxƒCƒIƒEƒ‹ƒt" ) == 0 ) {
		return "‰F’Ν";
	} else if ( strcmp( p, "ƒgƒƒXƒ^ƒ“" ) == 0 ) {
		return "‰F’Ν";
	} else if ( strstr( p, "ρό‚θ" ) ) {
		return "ρό‚θ";
	} else if ( strcmp( p, "£" ) == 0 ) {
		return "’ƒΫ";
	} else if ( strcmp( p, "π“ª" ) == 0 ) {
		return "…w";
	} else if ( strcmp( p, "‰Ξ‰®" ) == 0 ) {
		return "F";
	} else if ( strcmp( p, "η’Ή" ) == 0 ) {
		return "F";
	} else if ( strcmp( p, "–‚pt’c”t" ) == 0 ) {
		return "”t";
	} else if ( strcmp( p, "Ή”t" ) == 0 ) {
		return "”t";
	} else if ( strcmp( p, "ƒTƒ“ƒVƒIƒ“" ) == 0 ) {
		return "’Ζ";
	} else if ( strcmp( p, "„|„}„~„„„" ) == 0 ) { // ƒ‰ƒvƒ`ƒƒ[ƒ[ƒY
		return "•Ϊ";
	} else if ( strcmp( p, "—‹_‚Μƒ€ƒ`" ) == 0 ) {
		return "•Ϊ";
	} else if ( strstr( p, "‚Μρ" ) ) {
		return "ρ";
	} else if ( strstr( p, "‚Μe" ) ) {
		return "“S–C";
	} else if ( strcmp( p, "ƒTƒUƒ“ƒNƒƒX" ) == 0 ) {
		return "‹ο‘«";
	} else if ( strcmp( p, "‰¤‚Μ“Ή" ) == 0 ) {
		return "ί‘•";
	} else if ( strcmp( p, "[•£‚Μƒ[ƒu" ) == 0 ) {
		return "ί‘•";
	} else if ( strcmp( p, "‹`α" ) == 0 ) {
		return "‹`α";
	} else if ( strcmp( p, "g‘γ‚ν‚θl`" ) == 0 ) {
		return "l`";
	} else if ( strstr( p, "‚ΜΖ" ) ) {
		return "Ζ";
	} else if ( strcmp( p, "ll‚Μg“k" ) == 0) {
		return "G‰ζ";
	} else if ( strcmp( p, "ƒSƒ‹ƒSƒ^‚Μ‹u" ) == 0 ) {
		return "G‰ζ";
	} else if ( strcmp( p, "‰Τ—Φ‚ΜΉ•κ" ) == 0 ) {
		return "G‰ζ";
	} else if ( strcmp( p, "—_‚Μ’a¶" ) == 0 ) {
		return "G‰ζ";
	} else if ( strcmp( p, "Εγ‚ΜR”»" ) == 0 ) {
		return "G‰ζ";
	} else if ( strcmp( p, "‘Σ‚―Ò‚Μ“V‘" ) == 0 ) {
		return "G‰ζ";
	} else if ( strcmp( p, "l‘Μ}" ) == 0 ) {
		return "‘f•`";
	} else if ( strcmp( p, "ƒ‚ƒiƒƒU" ) == 0 ) {
		return "G‰ζ";
	} else if ( strcmp( p, "–ςάt‚ΜΡ‘" ) == 0 ) {
		return "G‰ζ";
	} else if ( strcmp( p, "ƒ}ƒM‚Μ—η”q" ) == 0 ) {
		return "G‰ζ";
	} else if ( strcmp( p, "ƒLƒƒXƒgτ—η" ) == 0 ) {
		return "G‰ζ";
	} else if ( strcmp( p, "σ‘Ω’m" ) == 0 ) {
		return "G‰ζ";
	} else if ( strcmp( p, "ƒgƒƒh‚Μ•—i" ) == 0 ) {
		return "G‰ζ";
	} else if ( strcmp( p, "ƒoƒxƒ‹‚Μ“ƒ" ) == 0 ) {
		return "G‰ζ";
	} else if ( strcmp( p, "ƒoƒxƒ‹‚Μ“ƒ" ) == 0 ) {
		return "G‰ζ";
	} else if ( strcmp( p, "j‹L" ) == 0) {
		return "j‘";
	} else if ( strcmp( p, "ΰq" ) == 0) {
		return "•Ί–@‘";
	} else if ( strcmp( p, "‘·q" ) == 0 ) {
		return "•Ί–@‘";
	} else if ( strcmp( p, "Zθι" ) == 0 ) {
		return "•Ί–@‘";
	} else if ( strcmp( p, "O—ª" ) == 0 ) {
		return "•Ί–@‘";
	} else if ( strcmp( p, "b—zRΣ" ) == 0 ) {
		return "Rw‘";
	} else if ( strcmp( p, "’†π—¬•½–@‘" ) == 0 ) {
		return "•Ί–@‘";
	} else if ( strcmp( p, "ζ_“Ή—¬‘" ) == 0 ) {
		return "•Ί–@‘";
	} else if ( strstr( p, "•p‘" ) ) {
		return "•Ί–@‘";
	} else if ( strstr( p, "‹|p‘" ) ) {
		return "•Ί–@‘";
	} else if ( strcmp( p, "–μWC" ) == 0) {
		return "”Ep‘";
	} else if ( strcmp( p, "‹•‹σ‘ ‚Μ‘" ) == 0 ) {
		return "‹L^‘";
	} else if ( strcmp( p, "–‚b•S‰Θ«“T" ) == 0 ) {
		return "«“T";
	} else if ( strcmp( p, "€—μ”ι–@" ) == 0 ) {
		return "–‚“±‘";
	} else if ( strcmp( p, "ƒOƒƒ‚ƒ[ƒ‹" ) == 0 ) {
		return "–‚“±‘";
	} else if ( strcmp( p, "—³Ύκ–‚–@" ) == 0 ) {
		return "–‚“±‘";
	} else if ( strcmp( p, "‰Ξ‹ς‚Ά’Ή‚Μ‰H" ) == 0 ) {
		return "‰Hª";
	} else if ( strcmp( p, "—³‹ΚΞ" ) == 0 ) {
		return "ΰ•σ";
	} else if ( strcmp( p, "«Ò‚Μ…" ) == 0 ) {
		return "_…";
	} else if ( strcmp( p, "ƒKƒ‰ƒX‚Μ“μ‰Z" ) == 0 ) {
		return "ΰ•σ";
	} else if ( strcmp( p, "”’‹ΰ‚Μƒƒ_ƒ‹" ) == 0 ) {
		return "ΰ•σ";
	} else if ( strcmp( p, "«ϋ”Υ" ) == 0 ) {
		return "«ϋ”Υ";
	} else if ( strcmp( p, "ƒyƒ“ƒ_ƒ“ƒg" ) == 0 ) {
		return "‘•g‹ο";
	} else if ( strcmp( p, "ƒTƒƒGƒ‰" ) == 0 ) {
		return "ΰ•σ";
	} else if ( strcmp( p, "–ΌΊ‚Μ—_" ) == 0 ) {
		return "Ξ‘";
	} else if ( strcmp( p, "ƒƒ‹ƒNƒƒEƒX" ) == 0 ) {
		return "Ξ‘";
	} else if ( strcmp( p, "Z”Υ" ) == 0 ) {
		return "Z”Υ";
	} else if ( strstr( p, "‘ζ" ) && strstr( p, "‚ΜΨ" ) ) {
		return "”ι•σ";
	} else if ( strcmp( p, "Ή•κq‘" ) == 0 ) {
		return "Ή•κ‘";
	} else if ( strcmp( p, "‹ΰ‚Μƒ‰ƒ“ƒv" ) == 0 ) {
		return "—m“”";
	} else if ( strcmp( p, "•’“Έπ" ) == 0 ) {
		return "π";
	}

	return NULL;
}