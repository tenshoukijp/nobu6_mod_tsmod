#pragma pack(1)

#include "2DDirectString.h"

// ƒxƒNƒgƒ‹‚ğ•¶š—ñ‰»‚·‚é‚ªA“Œ¼“ì–kŒni‚W•ûˆÊj‚Å•¶š—ñ‰»B
string get‚W•ûˆÊ•¶š—ñ(D2D_VECTOR v) {

	if ( v.length() == 0 ) {
		return "–³•ûˆÊ";
	}

	double inner = -1; // ^‹t‚©‚ç‚ÌƒXƒ^[ƒg
	string dir = "";

	//@‚W‚Â‚Ì•ûŒü‚Ìu’PˆÊƒxƒNƒgƒ‹v‚ğ—pˆÓ‚µ‚ÄAuÅŠñ‚èé‚©‚ç‚ÌƒxƒNƒgƒ‹v‚Æ‚Ì“àÏ‚ªÅ‘å‚Ì‚à‚Ì‚ğ‚¢Ì—p‚·‚éB
	// ’PˆÊƒxƒNƒgƒ‹“¯u‚Ì“àÏ‚ª‚P‚É‹ß‚¢‚Ù‚Çi‚·‚È‚í‚¿‘å‚«‚¢‚Ù‚Çj‚Æ‚¢‚¤‚±‚Æ‚ÍA‚Q‚Â‚ÌƒxƒNƒgƒ‹‚Ì•ûŒü‚Í“¯‚¶•ûŒü‚Å‚ ‚éB
	// ‹t‚ÉŒ¾‚¦‚Î^‹t‚Å‚ ‚é‚Ù‚ÇÅ­i‚·‚È‚í‚¿-1)‚É‹ß‚Ã‚­B
	D2D_VECTOR i(0,-1); // –k‚Æ”äŠr
	if ( i * v > inner) {
		inner = i * v;
		dir = "–k";
	}

	i = D2D_VECTOR(0,1); // “ì‚Æ”äŠr
	if ( i * v > inner) {
		inner = i * v;
		dir = "“ì";
	}

	i = D2D_VECTOR(1,0); // “Œ‚Æ”äŠr
	if ( i * v > inner) {
		inner = i * v;
		dir = "“Œ";
	}

	i = D2D_VECTOR(-1,0); // ¼‚Æ”äŠr
	if ( i * v > inner) {
		inner = i * v;
		dir = "¼";
	}

	i = D2D_VECTOR(1,-1); // “Œ–k‚Æ”äŠr
	i.normalize(); // ’PˆÊƒxƒNƒgƒ‹‰»
	if ( i * v > inner) {
		inner = i * v;
		dir = "–k“Œ";
	}

	i = D2D_VECTOR(-1,-1); // ¼–k‚Æ”äŠr
	i.normalize(); // ’PˆÊƒxƒNƒgƒ‹‰»
	if ( i * v > inner) {
		inner = i * v;
		dir = "–k¼";
	}

	i = D2D_VECTOR(1,1); // “Œ“ì‚Æ”äŠr
	i.normalize(); // ’PˆÊƒxƒNƒgƒ‹‰»
	if ( i * v > inner) {
		inner = i * v;
		dir = "“ì“Œ";
	}

	i = D2D_VECTOR(-1,1); // ¼“ì‚Æ”äŠr
	i.normalize(); // ’PˆÊƒxƒNƒgƒ‹‰»
	if ( i * v > inner) {
		inner = i * v;
		dir = "“ì¼";
	}

	return dir;
}



// ƒxƒNƒgƒ‹‚ğ•¶š—ñ‰»‚·‚é‚ªA“Œ¼“ì–kŒni‚S•ûˆÊj‚Å•¶š—ñ‰»B
string get‚S•ûˆÊ•¶š—ñ(D2D_VECTOR v) {

	if ( v.length() == 0 ) {
		return "–³•ûˆÊ";
	}

	double inner = -1; // “àÏBˆê”Ô‘å‚«‚¢‚à‚Ì‚ğ’T‚·B^‹t‚©‚ç‚ÌƒXƒ^[ƒg
	string dir = "";

	//@‚S‚Â‚Ì•ûŒü‚Ìu’PˆÊƒxƒNƒgƒ‹v‚ğ—pˆÓ‚µ‚ÄAuÅŠñ‚èé‚©‚ç‚ÌƒxƒNƒgƒ‹v‚Æ‚Ì“àÏ‚ªÅ‘å‚Ì‚à‚Ì‚ğ‚¢Ì—p‚·‚éB
	// ’PˆÊƒxƒNƒgƒ‹“¯u‚Ì“àÏ‚ª‚P‚É‹ß‚¢‚Ù‚Çi‚·‚È‚í‚¿‘å‚«‚¢‚Ù‚Çj‚Æ‚¢‚¤‚±‚Æ‚ÍA‚Q‚Â‚ÌƒxƒNƒgƒ‹‚Ì•ûŒü‚Í“¯‚¶•ûŒü‚Å‚ ‚éB
	// ‹t‚ÉŒ¾‚¦‚Î^‹t‚Å‚ ‚é‚Ù‚ÇÅ­i‚·‚È‚í‚¿-1)‚É‹ß‚Ã‚­B
	D2D_VECTOR i(0,-1); // –k‚Æ”äŠr
	if ( i * v > inner) {
		inner = i * v;
		dir = "–k";
	}

	i = D2D_VECTOR(0,1); // “ì‚Æ”äŠr
	if ( i * v > inner) {
		inner = i * v;
		dir = "“ì";
	}

	i = D2D_VECTOR(1,0); // “Œ‚Æ”äŠr
	if ( i * v > inner) {
		inner = i * v;
		dir = "“Œ";
	}

	i = D2D_VECTOR(-1,0); // ¼‚Æ”äŠr
	if ( i * v > inner) {
		inner = i * v;
		dir = "¼";
	}

	return dir;
}
