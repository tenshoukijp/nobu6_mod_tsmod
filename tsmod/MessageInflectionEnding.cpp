#include "WinTarget.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>


#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "MessageInflection.h"
#include "HumanRelation.h"


BOOL “—ª‹LğŒ‚P“(int i1st_BushouID, int i2nd_BushouID) {

	// ©•ª‚ª‰ºA‚Æ‚¢‚¤‚±‚ÆB
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

			// ©g>‘Šg (©•ª‚Ìg•ª‚Ì•û‚ª‰º‚Æ‚¢‚¤‚±‚Æ)
			if ( nb6bushouname[i1st_BushouID].position > nb6bushouname[i2nd_BushouID].position ){
				return true;
			}
	}
	return false;
}

BOOL “—ª‹LğŒ‚Q“(int i1st_BushouID, int i2nd_BushouID) {

	// ŒŒ‰ŠÖŒW‚âA“¯‚¶Š˜‚Ì”Ñ‚¾‚ªA©•ª‚ªãA‚Æ‚¢‚¤‚±‚ÆB
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

			// ©‘ŠŒŒ‰&©—î†‘Š—î 
			if ( is©•ª‚Í‘Šè‚ÌŒŒ‰( i1st_BushouID, i2nd_BushouID ) && getYearsOld(i1st_BushouID) >= getYearsOld(i2nd_BushouID) ) {
				return true;
			}
			// ‘Šè‚Í©•ª‚Ì‰Æb
			if ( is‘Šè‚Í©•ª‚Ì‰Æb( i1st_BushouID, i2nd_BushouID ) ) {
				return true;
			}
			// “¯‚¶‰Æ‚ÉŠ‘®‚µ‚Ä‚¢‚é & ©—î†‘Š—î & ©g<‘Šg(©•ª‚Ì•û‚ªg•ª‚ª‚‚¢)
			if ( is©•ª‚Æ‘Šè‚Í“¯‚¶‘å–¼‰ÆŠ‘®( i1st_BushouID, i2nd_BushouID ) &&  getYearsOld(i1st_BushouID) >= getYearsOld(i2nd_BushouID) && 
				nb6bushouname[i1st_BushouID].position < nb6bushouname[i2nd_BushouID].position ) {
					return true;
			}
	}
	return false;
}

int “—ª‹LğŒ‚s“(int i1st_BushouID, int i2nd_BushouID) {
	// ‘Šè‚Ì•«‚ª‚¢‚È‚¢‚Ì‚Å‚ ‚ê‚ÎA1‚Æ‚È‚éB
	if ( i2nd_BushouID == 0xFFFF ) {
		return 1;
	}

	// —ª‹LğŒ‚Q‚ğ–‚½‚·B(“¯‚¶Š˜‚Ì”Ñ‚Å‚©‚ÂA©•ª‚ªã)
	else if ( “—ª‹LğŒ‚Q“( i1st_BushouID, i2nd_BushouID ) ) {
		return 1;
	}
	// —ª‹LğŒ‚P‚ğ–‚½‚·B(©•ª‚ª‰º)
	else if ( “—ª‹LğŒ‚P“( i1st_BushouID, i2nd_BushouID ) ) {
		return 2;
	}
	// —ª‹LğŒ‚P‚à—ª‹LğŒ‚Q‚à–‚½‚³‚È‚¢(©•ª‚Ì•û‚ªg•ª‚Íã)
	else {
		// ‚»‚êˆÈŠO
		return 3;
	}
}



// `‚Å‚· `‚¾ `‚¶‚á
string “‚Å‚·“(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {

	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// —ª‹LğŒ‚s‚Ì‚Ç‚ê‚ª‚ ‚Ä‚Í‚Ü‚é‚©H ‚Ç‚ê‚É‚à‚ ‚Ä‚Í‚Ü‚ç‚È‚©‚Á‚½‚ç—ª‹LğŒ‚x‚Å‚Ç‚¿‚ç‚©‚É“–‚Ä‚Í‚Ü‚éB
		int Group = “—ª‹LğŒ‚s“(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ‚Å‚· ©— 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‚Å‚·";

				// ‚¾ ©Œû=2 || ©Œû=4
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
				return "‚¾";

				// ‚¶‚á i‘¼j 
			} else {
				return "‚¶‚á";
			}

			// Group B
		} else {
			// ‚Å‚· i‘¼j 
			return "‚Å‚·";
		}
	}

	return "“‚Å‚·“";
}



// `‚Å‚·‚© `‚Å‚²‚´‚é‚© `‚©
string “‚Å‚·‚©“(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {

	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// —ª‹LğŒ‚s‚Ì‚Ç‚ê‚ª‚ ‚Ä‚Í‚Ü‚é‚©H ‚Ç‚ê‚É‚à‚ ‚Ä‚Í‚Ü‚ç‚È‚©‚Á‚½‚ç—ª‹LğŒ‚x‚Å‚Ç‚¿‚ç‚©‚É“–‚Ä‚Í‚Ü‚éB
		int Group = “—ª‹LğŒ‚s“(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ‚Å‚·‚© ©— 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‚Å‚·‚©";

			// ‚© i‘¼j 
			} else {
				return "‚©";
			}

		// Group B
		} else {
			// ‚Å‚·‚© ©— 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‚Å‚·‚©";

			// ‚Å‚²‚´‚é‚© ©Œû=5 || ©E=1or3(”EÒorŒ•‹)
			} else if ( nb6bushouref[i1st_BushouID].tone == 5 || nb6bushouref[i1st_BushouID].job == 0x10 || nb6bushouref[i1st_BushouID].job == 0x30 ) {
				return "‚Å‚²‚´‚é‚©";

			// ‚Å‚·‚© i‘¼j 
			} else {
				return "‚Å‚·‚©";
			}
		}
	}

	return "“‚Å‚·‚©“";
}



// ‚Å‚µ‚å‚¤ `‚¶‚á‚ë‚¤ `‚Å‚ ‚ë‚¤ `‚¾‚ë‚¤
string “‚Å‚µ‚å‚¤“(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {

	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// —ª‹LğŒ‚s‚Ì‚Ç‚ê‚ª‚ ‚Ä‚Í‚Ü‚é‚©H ‚Ç‚ê‚É‚à‚ ‚Ä‚Í‚Ü‚ç‚È‚©‚Á‚½‚ç—ª‹LğŒ‚x‚Å‚Ç‚¿‚ç‚©‚É“–‚Ä‚Í‚Ü‚éB
		int Group = “—ª‹LğŒ‚s“(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ‚Å‚µ‚å‚¤ ©— 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‚Å‚µ‚å‚¤";

			// ‚Å‚ ‚ë‚¤ ©Œû=3 || ©Œû=1 || ©Œû=5
			} else if ( nb6bushouref[i1st_BushouID].tone == 3 || nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 5 ) {
				return "‚Å‚ ‚ë‚¤";

			// ‚¾‚ë‚¤ ©Œû=2 || ©Œû=4
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
				return "‚¾‚ë‚¤";

			// ‚¶‚á‚ë‚¤ (‘¼)
			} else {
				return "‚¶‚á‚ë‚¤";
			}

		// Group B
		} else {
			// ‚Å‚µ‚å‚¤
			return "‚Å‚µ‚å‚¤";
		}
	}

	return "“‚Å‚µ‚å‚¤“";
}



// `‚Å‚µ‚½ `‚¶‚á‚Á‚½ `‚Å‚ ‚Á‚½ `‚¾‚Á‚½
string “‚Å‚µ‚½“(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// —ª‹LğŒ‚s‚Ì‚Ç‚ê‚ª‚ ‚Ä‚Í‚Ü‚é‚©H ‚Ç‚ê‚É‚à‚ ‚Ä‚Í‚Ü‚ç‚È‚©‚Á‚½‚ç—ª‹LğŒ‚x‚Å‚Ç‚¿‚ç‚©‚É“–‚Ä‚Í‚Ü‚éB
		int Group = “—ª‹LğŒ‚s“(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ‚Å‚µ‚½ ©— 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‚Å‚µ‚½";

			// ‚Å‚ ‚Á‚½ ©Œû=3 || ©Œû=1 || ©Œû=5
			} else if ( nb6bushouref[i1st_BushouID].tone == 3 || nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 5 ) {
				return "‚Å‚ ‚Á‚½";

			// ‚¾‚Á‚½ ©Œû=2 || ©Œû=4
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
				return "‚¾‚Á‚½";

			// ‚¶‚á‚Á‚½ (‘¼)
			} else {
				return "‚¶‚á‚Á‚½";
			}

		// Group B
		} else {
			// ‚Å‚µ‚½
			return "‚Å‚µ‚½";
		}
	}

	return "“‚Å‚µ‚½“";
}



// `‚ ‚è‚Ü‚· `‚²‚´‚¢‚Ü‚· `‚²‚´‚é `‚ ‚é `‚¨‚¶‚á‚é
string “‚ ‚è‚Ü‚·“(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// —ª‹LğŒ‚s‚Ì‚Ç‚ê‚ª‚ ‚Ä‚Í‚Ü‚é‚©H ‚Ç‚ê‚É‚à‚ ‚Ä‚Í‚Ü‚ç‚È‚©‚Á‚½‚ç—ª‹LğŒ‚x‚Å‚Ç‚¿‚ç‚©‚É“–‚Ä‚Í‚Ü‚éB
		int Group = “—ª‹LğŒ‚s“(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ‚ ‚è‚Ü‚· ©— 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‚ ‚è‚Ü‚·";

			// ‚¨‚¶‚á‚é ©Œû=3
			} else if ( nb6bushouref[i1st_BushouID].tone == 3 ) {
				return "‚¨‚¶‚á‚é";

			// ‚ ‚é (‘¼)
			} else {
				return "‚ ‚é";
			}

		// Group B
		} else {
			// ‚ ‚è‚Ü‚· ©— 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‚ ‚è‚Ü‚·";

			// ‚²‚´‚é ©Œû=5 || ©E=1or3(”EÒorŒ•‹)
			} else if ( nb6bushouref[i1st_BushouID].tone == 5 || nb6bushouref[i1st_BushouID].job == 0x10 || nb6bushouref[i1st_BushouID].job == 0x30 ) {
				return "‚²‚´‚é";

			// ‚ ‚è‚Ü‚· ©‘Š=“¯‚¶‘å–¼‰ÆŠ‘® 
			} else if ( is©•ª‚Æ‘Šè‚Í“¯‚¶‘å–¼‰ÆŠ‘®(i1st_BushouID,i2nd_BushouID) ) {
				return "‚ ‚è‚Ü‚·";

			// ‚²‚´‚¢‚Ü‚· i‘¼j 
			} else {
				return "‚²‚´‚¢‚Ü‚·";

			}
		}
	}

	return "“‚ ‚è‚Ü‚·“";
}


// `‚ ‚è‚Ü‚¹‚ñ `‚²‚´‚¢‚Ü‚¹‚ñ `‚²‚´‚ç‚Ê `‚È‚¢ `‚¨‚¶‚á‚ç‚Ê `‚ ‚è‚Ü‚¹‚Ê
string “‚ ‚è‚Ü‚¹‚ñ“(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// —ª‹LğŒ‚s‚Ì‚Ç‚ê‚ª‚ ‚Ä‚Í‚Ü‚é‚©H ‚Ç‚ê‚É‚à‚ ‚Ä‚Í‚Ü‚ç‚È‚©‚Á‚½‚ç—ª‹LğŒ‚x‚Å‚Ç‚¿‚ç‚©‚É“–‚Ä‚Í‚Ü‚éB
		int Group = “—ª‹LğŒ‚s“(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ‚ ‚è‚Ü‚¹‚Ê ©— 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‚ ‚è‚Ü‚¹‚Ê";

			// ‚¨‚¶‚á‚ç‚Ê ©Œû=3
			} else if ( nb6bushouref[i1st_BushouID].tone == 3 ) {
				return "‚¨‚¶‚á‚ç‚Ê";

			// ‚È‚¢ (‘¼)
			} else {
				return "‚È‚¢";
			}

		// Group B
		} else {
			// ‚ ‚è‚Ü‚¹‚Ê ©— 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‚ ‚è‚Ü‚¹‚Ê";

			// ‚²‚´‚ç‚Ê ©Œû=5 || ©E=1or3(”EÒorŒ•‹)
			} else if ( nb6bushouref[i1st_BushouID].tone == 5 || nb6bushouref[i1st_BushouID].job == 0x10 || nb6bushouref[i1st_BushouID].job == 0x30 ) {
				return "‚²‚´‚ç‚Ê";

			// ‚ ‚è‚Ü‚¹‚ñ ©‘Š=“¯‚¶‘å–¼‰ÆŠ‘® 
			} else if ( is©•ª‚Æ‘Šè‚Í“¯‚¶‘å–¼‰ÆŠ‘®(i1st_BushouID,i2nd_BushouID) ) {
				return "‚ ‚è‚Ü‚·";

			// ‚²‚´‚¢‚Ü‚¹‚ñ i‘¼j 
			} else {
				return "‚²‚´‚¢‚Ü‚·";

			}
		}
	}

	return "“‚ ‚è‚Ü‚¹‚ñ“";
}




// `‚µ‚Ü‚· `‚¢‚½‚· `‚·‚é
string “‚µ‚Ü‚·“(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// —ª‹LğŒ‚s‚Ì‚Ç‚ê‚ª‚ ‚Ä‚Í‚Ü‚é‚©H ‚Ç‚ê‚É‚à‚ ‚Ä‚Í‚Ü‚ç‚È‚©‚Á‚½‚ç—ª‹LğŒ‚x‚Å‚Ç‚¿‚ç‚©‚É“–‚Ä‚Í‚Ü‚éB
		int Group = “—ª‹LğŒ‚s“(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ‚µ‚Ü‚· ©— 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‚µ‚Ü‚·";

			// ‚·‚é ©Œû=2 || ©Œû=4
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
				return "‚·‚é";

			// ‚¢‚½‚· (‘¼)
			} else {
				return "‚¢‚½‚·";
			}

		// Group B
		} else {
			// ‚¢‚½‚· ©Œû=5 || ©’j
			if ( nb6bushouref[i1st_BushouID].tone == 5 || nb6bushouname[i1st_BushouID].sex == 0 ) {
				return "‚¢‚½‚·";

			// ‚µ‚Ü‚· (‘¼)
			} else {
				return "‚µ‚Ü‚·";
			}
		}
	}

	return "“‚µ‚Ü‚·“";
}

// `‚µ‚Ü‚¹‚ñ `‚¢‚½‚³‚Ê `‚¹‚Ê `‚¹‚ñ `‚µ‚Ü‚¹‚Ê
string “‚µ‚Ü‚¹‚ñ“(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// —ª‹LğŒ‚s‚Ì‚Ç‚ê‚ª‚ ‚Ä‚Í‚Ü‚é‚©H ‚Ç‚ê‚É‚à‚ ‚Ä‚Í‚Ü‚ç‚È‚©‚Á‚½‚ç—ª‹LğŒ‚x‚Å‚Ç‚¿‚ç‚©‚É“–‚Ä‚Í‚Ü‚éB
		int Group = “—ª‹LğŒ‚s“(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ‚µ‚Ü‚¹‚Ê ©— 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‚µ‚Ü‚¹‚Ê";

			// ‚¹‚ñ ©Œû=2 || ©Œû=4
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
				return "‚¹‚ñ";

			// ‚¹‚Ê (‘¼)
			} else {
				return "‚¹‚Ê";
			}

		// Group B
		} else {
			// ‚µ‚Ü‚¹‚Ê ©Œû=1 || ©Œû=3 || ©—
			if ( nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 3 || nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‚µ‚Ü‚¹‚Ê";

			// ‚¢‚½‚³‚Ê ©Œû=5
			} else if ( nb6bushouref[i1st_BushouID].tone == 5 ) {
				return "‚¢‚½‚³‚Ê";

			// ‚µ‚Ü‚¹‚ñ (‘¼)
			} else {
				return "‚µ‚Ü‚¹‚ñ";
			}
		}
	}

	return "“‚µ‚Ü‚¹‚ñ“";
}



// `‚µ‚Ü‚µ‚å‚¤ `‚¢‚½‚»‚¤ `‚µ‚æ‚¤
string “‚µ‚Ü‚µ‚å‚¤“(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// —ª‹LğŒ‚s‚Ì‚Ç‚ê‚ª‚ ‚Ä‚Í‚Ü‚é‚©H ‚Ç‚ê‚É‚à‚ ‚Ä‚Í‚Ü‚ç‚È‚©‚Á‚½‚ç—ª‹LğŒ‚x‚Å‚Ç‚¿‚ç‚©‚É“–‚Ä‚Í‚Ü‚éB
		int Group = “—ª‹LğŒ‚s“(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ‚µ‚Ü‚µ‚å‚¤ ©— 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‚µ‚Ü‚µ‚å‚¤";

			// ‚µ‚æ‚¤ ©Œû=2 || ©Œû=4
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
				return "‚µ‚æ‚¤";

			// ‚¢‚½‚»‚¤ (‘¼)
			} else {
				return "‚¢‚½‚»‚¤";
			}

		// Group B
		} else {
			// ‚¢‚½‚»‚¤ ©Œû=5 || ©’j
			if ( nb6bushouref[i1st_BushouID].tone == 5 || nb6bushouname[i1st_BushouID].sex == 0 ) {
				return "‚¢‚½‚»‚¤";

			// ‚µ‚Ü‚µ‚å‚¤ (‘¼)
			} else {
				return "‚µ‚Ü‚µ‚å‚¤";
			}
		}
	}

	return "“‚µ‚Ü‚µ‚å‚¤“";
}



// `‚¢‚Ü‚µ‚å‚¤ `‚¨‚¤  (Œ¾‚¢‚Ü‚µ‚å‚¤‚È‚Ç)
string “‚¢‚Ü‚µ‚å‚¤“(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// —ª‹LğŒ‚s‚Ì‚Ç‚ê‚ª‚ ‚Ä‚Í‚Ü‚é‚©H ‚Ç‚ê‚É‚à‚ ‚Ä‚Í‚Ü‚ç‚È‚©‚Á‚½‚ç—ª‹LğŒ‚x‚Å‚Ç‚¿‚ç‚©‚É“–‚Ä‚Í‚Ü‚éB
		int Group = “—ª‹LğŒ‚s“(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ‚¢‚Ü‚µ‚å‚¤ ©— 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‚¢‚Ü‚µ‚å‚¤";

			// ‚¨‚¤ (‘¼)
			} else {
				return "‚¨‚¤";
			}

		// Group B
		} else {
			// ‚¢‚Ü‚µ‚å‚¤ (‘¼)
			return "‚¢‚Ü‚µ‚å‚¤";
		}
	}

	return "“‚¢‚Ü‚µ‚å‚¤“";
}



// `‚«‚Ü‚µ‚å‚¤ `‚±‚¤  (s‚«‚Ü‚µ‚å‚¤‚È‚Ç)
string “‚«‚Ü‚µ‚å‚¤“(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// —ª‹LğŒ‚s‚Ì‚Ç‚ê‚ª‚ ‚Ä‚Í‚Ü‚é‚©H ‚Ç‚ê‚É‚à‚ ‚Ä‚Í‚Ü‚ç‚È‚©‚Á‚½‚ç—ª‹LğŒ‚x‚Å‚Ç‚¿‚ç‚©‚É“–‚Ä‚Í‚Ü‚éB
		int Group = “—ª‹LğŒ‚s“(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ‚«‚Ü‚µ‚å‚¤ ©— 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‚«‚Ü‚µ‚å‚¤";

				// ‚±‚¤ (‘¼)
			} else {
				return "‚±‚¤";
			}

			// Group B
		} else {
			// ‚«‚Ü‚µ‚å‚¤ (‘¼)
			return "‚«‚Ü‚µ‚å‚¤";
		}
	}

	return "“‚«‚Ü‚µ‚å‚¤“";
}


// `‚µ‚Ü‚µ‚å‚¤ `‚»‚¤  (Hc “‚µ‚Ü‚µ‚å‚¤“‚Ì•û‚ª—Ç‚¢‚©)
string “‚µ‚Ü‚µ‚å‚¤‚Q“(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// —ª‹LğŒ‚s‚Ì‚Ç‚ê‚ª‚ ‚Ä‚Í‚Ü‚é‚©H ‚Ç‚ê‚É‚à‚ ‚Ä‚Í‚Ü‚ç‚È‚©‚Á‚½‚ç—ª‹LğŒ‚x‚Å‚Ç‚¿‚ç‚©‚É“–‚Ä‚Í‚Ü‚éB
		int Group = “—ª‹LğŒ‚s“(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ‚»‚¤ (‘¼)
			return "‚»‚¤";

			// Group B
		} else {
			// ‚µ‚Ü‚µ‚å‚¤ (‘¼)
			return "‚µ‚Ü‚µ‚å‚¤";
		}
	}

	return "“‚µ‚Ü‚µ‚å‚¤‚Q“";
}



// `‚¿‚Ü‚µ‚å‚¤ `‚Æ‚¤  (‘Ò‚¿‚Ü‚µ‚å‚¤‚È‚Ç)
string “‚¿‚Ü‚µ‚å‚¤“(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// —ª‹LğŒ‚s‚Ì‚Ç‚ê‚ª‚ ‚Ä‚Í‚Ü‚é‚©H ‚Ç‚ê‚É‚à‚ ‚Ä‚Í‚Ü‚ç‚È‚©‚Á‚½‚ç—ª‹LğŒ‚x‚Å‚Ç‚¿‚ç‚©‚É“–‚Ä‚Í‚Ü‚éB
		int Group = “—ª‹LğŒ‚s“(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ‚Æ‚¤ (‘¼)
			return "‚Æ‚¤";

			// Group B
		} else {
			// ‚¿‚Ü‚µ‚å‚¤ (‘¼)
			return "‚¿‚Ü‚µ‚å‚¤";
		}
	}

	return "“‚¿‚Ü‚µ‚å‚¤“";
}


// 29 `‚İ‚Ü‚µ‚å‚¤ `‚à‚¤  (U‚ß‚İ‚Ü‚µ‚å‚¤‚È‚Ç)
string “‚İ‚Ü‚µ‚å‚¤“(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// —ª‹LğŒ‚s‚Ì‚Ç‚ê‚ª‚ ‚Ä‚Í‚Ü‚é‚©H ‚Ç‚ê‚É‚à‚ ‚Ä‚Í‚Ü‚ç‚È‚©‚Á‚½‚ç—ª‹LğŒ‚x‚Å‚Ç‚¿‚ç‚©‚É“–‚Ä‚Í‚Ü‚éB
		int Group = “—ª‹LğŒ‚s“(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ‚à‚¤ (‘¼)
			return "‚à‚¤";

			// Group B
		} else {
			// ‚İ‚Ü‚µ‚å‚¤ (‘¼)
			return "‚İ‚Ü‚µ‚å‚¤";
		}
	}

	return "“‚¿‚Ü‚µ‚å‚¤“";
}


// `‚Ü‚µ‚å‚¤ `‚æ‚¤  (‚»‚Ì‘¼‚Ìˆê”Êƒpƒ^[ƒ“B—§‚Ä‚Ü‚µ‚å‚¤‚È‚Ç)
string “‚Ü‚µ‚å‚¤“(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// —ª‹LğŒ‚s‚Ì‚Ç‚ê‚ª‚ ‚Ä‚Í‚Ü‚é‚©H ‚Ç‚ê‚É‚à‚ ‚Ä‚Í‚Ü‚ç‚È‚©‚Á‚½‚ç—ª‹LğŒ‚x‚Å‚Ç‚¿‚ç‚©‚É“–‚Ä‚Í‚Ü‚éB
		int Group = “—ª‹LğŒ‚s“(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ‚Ü‚µ‚å‚¤ ©— 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‚Ü‚µ‚å‚¤";

				// ‚æ‚¤ (‘¼)
			} else {
				return "‚æ‚¤";
			}

			// Group B
		} else {
			// ‚Ü‚µ‚å‚¤ (‘¼)
			return "‚Ü‚µ‚å‚¤";
		}
	}

	return "“‚Ü‚µ‚å‚¤“";
}



// `‚è‚Ü‚µ‚å‚¤ `‚ë‚¤  (Q‚è‚Ü‚µ‚å‚¤‚È‚Ç)
string “‚è‚Ü‚µ‚å‚¤“(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// —ª‹LğŒ‚s‚Ì‚Ç‚ê‚ª‚ ‚Ä‚Í‚Ü‚é‚©H ‚Ç‚ê‚É‚à‚ ‚Ä‚Í‚Ü‚ç‚È‚©‚Á‚½‚ç—ª‹LğŒ‚x‚Å‚Ç‚¿‚ç‚©‚É“–‚Ä‚Í‚Ü‚éB
		int Group = “—ª‹LğŒ‚s“(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ‚è‚Ü‚µ‚å‚¤ ©— 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‚è‚Ü‚µ‚å‚¤";

				// ‚ë‚¤ (‘¼)
			} else {
				return "‚ë‚¤";
			}

			// Group B
		} else {
			// ‚è‚Ü‚µ‚å‚¤ (‘¼)
			return "‚è‚Ü‚µ‚å‚¤";
		}
	}

	return "“‚è‚Ü‚µ‚å‚¤“";
}


// `‚¬‚Ü‚µ‚å‚¤ `‚²‚¤  (‹}‚¬‚Ü‚µ‚å‚¤‚È‚Ç)
string “‚¬‚Ü‚µ‚å‚¤“(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// —ª‹LğŒ‚s‚Ì‚Ç‚ê‚ª‚ ‚Ä‚Í‚Ü‚é‚©H ‚Ç‚ê‚É‚à‚ ‚Ä‚Í‚Ü‚ç‚È‚©‚Á‚½‚ç—ª‹LğŒ‚x‚Å‚Ç‚¿‚ç‚©‚É“–‚Ä‚Í‚Ü‚éB
		int Group = “—ª‹LğŒ‚s“(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ‚²‚¤ (‘¼)
			return "‚²‚¤";

		// Group B
		} else {
			// ‚¬‚Ü‚µ‚å‚¤ (‘¼)
			return "‚¬‚Ü‚µ‚å‚¤";
		}
	}

	return "“‚¬‚Ü‚µ‚å‚¤“";
}


// `‚Ñ‚Ü‚µ‚å‚¤ `‚Ú‚¤@(—V‚Ñ‚Ü‚µ‚å‚¤‚È‚Ç)
string “‚Ñ‚Ü‚µ‚å‚¤“(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// —ª‹LğŒ‚s‚Ì‚Ç‚ê‚ª‚ ‚Ä‚Í‚Ü‚é‚©H ‚Ç‚ê‚É‚à‚ ‚Ä‚Í‚Ü‚ç‚È‚©‚Á‚½‚ç—ª‹LğŒ‚x‚Å‚Ç‚¿‚ç‚©‚É“–‚Ä‚Í‚Ü‚éB
		int Group = “—ª‹LğŒ‚s“(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ‚Ú‚¤ (‘¼)
			return "‚Ú‚¤";

		// Group B
		} else {
			// ‚Ñ‚Ü‚µ‚å‚¤ (‘¼)
			return "‚Ñ‚Ü‚µ‚å‚¤";
		}
	}

	return "“‚Ñ‚Ü‚µ‚å‚¤“";
}



// 35 `‚¢‚Ü‚· `‚²‚´‚é `‚¨‚é `‚¢‚é `‚¨‚¶‚á‚é `‚¨‚è‚Ü‚·
string “‚¢‚Ü‚·“(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// —ª‹LğŒ‚s‚Ì‚Ç‚ê‚ª‚ ‚Ä‚Í‚Ü‚é‚©H ‚Ç‚ê‚É‚à‚ ‚Ä‚Í‚Ü‚ç‚È‚©‚Á‚½‚ç—ª‹LğŒ‚x‚Å‚Ç‚¿‚ç‚©‚É“–‚Ä‚Í‚Ü‚éB
		int Group = “—ª‹LğŒ‚s“(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ‚¨‚è‚Ü‚· ©— 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‚¨‚è‚Ü‚·";

			// ‚¨‚¶‚á‚ç‚Ê ©Œû=3
			} else if ( nb6bushouref[i1st_BushouID].tone == 3 ) {
				return "‚¨‚¶‚á‚é";

			// ‚¢‚é ©Œû=2 || ©Œû=4
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
				return "‚¢‚é";

			// ‚¨‚é (‘¼)
			} else {
				return "‚¨‚é";
			}

		// Group B
		} else {
			// ‚¨‚è‚Ü‚· ©Œû=1 or ©Œû=3 or ©— 
			if ( nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 3 || nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‚¨‚è‚Ü‚·";

			// ‚²‚´‚é ©Œû=5 || ©E=1or3(”EÒorŒ•‹)
			} else if ( nb6bushouref[i1st_BushouID].tone == 5 || nb6bushouref[i1st_BushouID].job == 0x10 || nb6bushouref[i1st_BushouID].job == 0x30 ) {
				return "‚²‚´‚é";

			// ‚¢‚Ü‚· (‘¼j 
			} else {
				return "‚¢‚Ü‚·";

			}
		}
	}

	return "“‚¢‚Ü‚·“";
}



// 37 `‚¢‚Ü‚¹‚ñ `‚²‚´‚ç‚Ê `‚¨‚ç‚Ê `‚¨‚ç‚ñ `‚¨‚¶‚á‚ç‚Ê `‚¨‚è‚Ü‚¹‚Ê
string “‚¢‚Ü‚¹‚ñ“(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// —ª‹LğŒ‚s‚Ì‚Ç‚ê‚ª‚ ‚Ä‚Í‚Ü‚é‚©H ‚Ç‚ê‚É‚à‚ ‚Ä‚Í‚Ü‚ç‚È‚©‚Á‚½‚ç—ª‹LğŒ‚x‚Å‚Ç‚¿‚ç‚©‚É“–‚Ä‚Í‚Ü‚éB
		int Group = “—ª‹LğŒ‚s“(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ‚¨‚è‚Ü‚¹‚Ê ©— 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‚¨‚è‚Ü‚¹‚Ê";

			// ‚¨‚¶‚á‚ç‚Ê ©Œû=3
			} else if ( nb6bushouref[i1st_BushouID].tone == 3 ) {
				return "‚¨‚¶‚á‚ç‚Ê";

			// ‚¨‚ç‚ñ ©Œû=2 || ©Œû=4
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
				return "‚¨‚ç‚ñ";

			// ‚¨‚ç‚Ê (‘¼)
			} else {
				return "‚¨‚ç‚Ê";
			}

		// Group B
		} else {
			// ‚¨‚è‚Ü‚¹‚Ê ©Œû=1 or ©Œû=3 or ©— 
			if ( nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 3 || nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‚¨‚è‚Ü‚¹‚Ê";

			// ‚²‚´‚ç‚Ê ©Œû=5 || ©E=1or3(”EÒorŒ•‹)
			} else if ( nb6bushouref[i1st_BushouID].tone == 5 || nb6bushouref[i1st_BushouID].job == 0x10 || nb6bushouref[i1st_BushouID].job == 0x30 ) {
				return "‚²‚´‚ç‚Ê";

			// ‚¢‚Ü‚¹‚ñ (‘¼j 
			} else {
				return "‚¢‚Ü‚¹‚ñ";

			}
		}
	}

	return "“‚¢‚Ü‚¹‚ñ“";
}



// 39 `‚¢‚Ü‚µ‚½ `‚²‚´‚Á‚½ `‚¨‚Á‚½ `‚¢‚½ `‚¨‚¶‚á‚Á‚½ `‚¨‚è‚Ü‚µ‚½
string “‚¢‚Ü‚µ‚½“(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// —ª‹LğŒ‚s‚Ì‚Ç‚ê‚ª‚ ‚Ä‚Í‚Ü‚é‚©H ‚Ç‚ê‚É‚à‚ ‚Ä‚Í‚Ü‚ç‚È‚©‚Á‚½‚ç—ª‹LğŒ‚x‚Å‚Ç‚¿‚ç‚©‚É“–‚Ä‚Í‚Ü‚éB
		int Group = “—ª‹LğŒ‚s“(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ‚¨‚è‚Ü‚µ‚½ ©— 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‚¨‚è‚Ü‚µ‚½";

			// ‚¨‚¶‚á‚Á‚½ ©Œû=3
			} else if ( nb6bushouref[i1st_BushouID].tone == 3 ) {
				return "‚¨‚¶‚á‚Á‚½";

			// ‚¢‚½ ©Œû=2 || ©Œû=4
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
				return "‚¢‚½";

			// ‚¨‚Á‚½ (‘¼)
			} else {
				return "‚¨‚Á‚½";
			}

		// Group B
		} else {
			// ‚¨‚è‚Ü‚µ‚½ ©Œû=1 or ©Œû=3 or ©— 
			if ( nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 3 || nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‚¨‚è‚Ü‚µ‚½";

			// ‚²‚´‚Á‚½ ©Œû=5 || ©E=1or3(”EÒorŒ•‹)
			} else if ( nb6bushouref[i1st_BushouID].tone == 5 || nb6bushouref[i1st_BushouID].job == 0x10 || nb6bushouref[i1st_BushouID].job == 0x30 ) {
				return "‚²‚´‚Á‚½";

			// ‚¢‚Ü‚µ‚½ (‘¼j 
			} else {
				return "‚¢‚Ü‚µ‚½";

			}
		}
	}

	return "“‚¢‚Ü‚µ‚½“";
}



// `‚Ü‚· `‚é `‚è‚Ü‚·
string “‚Ü‚·“(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// —ª‹LğŒ‚s‚Ì‚Ç‚ê‚ª‚ ‚Ä‚Í‚Ü‚é‚©H ‚Ç‚ê‚É‚à‚ ‚Ä‚Í‚Ü‚ç‚È‚©‚Á‚½‚ç—ª‹LğŒ‚x‚Å‚Ç‚¿‚ç‚©‚É“–‚Ä‚Í‚Ü‚éB
		int Group = “—ª‹LğŒ‚s“(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ‚Ü‚· ©— 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‚Ü‚·";

			// ‚é (‘¼)
			} else {
				return "‚é";
			}

		// Group B
		} else {

			// ‚Ü‚· ©‘Š=“¯‚¶‘å–¼‰ÆŠ‘® 
			if ( is©•ª‚Æ‘Šè‚Í“¯‚¶‘å–¼‰ÆŠ‘®(i1st_BushouID,i2nd_BushouID) ) {
				return "‚Ü‚·";

			// ‚è‚Ü‚·i‘¼j 
			} else {
				return "‚è‚Ü‚·";

			}
		}
	}

	return "“‚Ü‚·“";
}



// `‚è‚Ü‚¹‚ñ `‚ç‚Ê
string “‚è‚Ü‚¹‚ñ“(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// —ª‹LğŒ‚s‚Ì‚Ç‚ê‚ª‚ ‚Ä‚Í‚Ü‚é‚©H ‚Ç‚ê‚É‚à‚ ‚Ä‚Í‚Ü‚ç‚È‚©‚Á‚½‚ç—ª‹LğŒ‚x‚Å‚Ç‚¿‚ç‚©‚É“–‚Ä‚Í‚Ü‚éB
		int Group = “—ª‹LğŒ‚s“(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ‚è‚Ü‚¹‚ñ ©— 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‚è‚Ü‚¹‚ñ";

			// ‚ç‚Ê (‘¼)
			} else {
				return "‚ç‚Ê";
			}

		// Group B
		} else {

			// ‚è‚Ü‚¹‚ñi‘¼j 
			return "‚è‚Ü‚¹‚ñ";
		}
	}

	return "“‚è‚Ü‚¹‚ñ“";
}


// `‚Ü‚¹‚ñ `‚Ê `‚Ë‚¦ `‚Ü‚¹‚Ê
string “‚Ü‚¹‚ñ“(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// —ª‹LğŒ‚s‚Ì‚Ç‚ê‚ª‚ ‚Ä‚Í‚Ü‚é‚©H ‚Ç‚ê‚É‚à‚ ‚Ä‚Í‚Ü‚ç‚È‚©‚Á‚½‚ç—ª‹LğŒ‚x‚Å‚Ç‚¿‚ç‚©‚É“–‚Ä‚Í‚Ü‚éB
		int Group = “—ª‹LğŒ‚s“(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ‚Ü‚¹‚Ê ©— 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‚Ü‚¹‚Ê";

			// ‚Ë‚¦ ©Œû=2 && ©g<=3
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 && nb6bushouname[i2nd_BushouID].position <= 3 ) {
				return "‚Ë‚¦";

			// ‚Ê (‘¼)
			} else {
				return "‚Ê";
			}

		// Group B
		} else {
			// ‚Ü‚¹‚Ê ©Œû=1 || ©Œû=5
			if ( nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 5 ) {
				return "‚Ü‚¹‚Ê";

			// ‚Ü‚¹‚ñ (‘¼j 
			} else {
				return "‚Ü‚¹‚ñ";

			}
		}
	}

	return "“‚Ü‚¹‚ñ“";
}



// `‚Ü‚µ‚½ `‚½
string “‚Ü‚µ‚½“(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// —ª‹LğŒ‚s‚Ì‚Ç‚ê‚ª‚ ‚Ä‚Í‚Ü‚é‚©H ‚Ç‚ê‚É‚à‚ ‚Ä‚Í‚Ü‚ç‚È‚©‚Á‚½‚ç—ª‹LğŒ‚x‚Å‚Ç‚¿‚ç‚©‚É“–‚Ä‚Í‚Ü‚éB
		int Group = “—ª‹LğŒ‚s“(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ‚Ü‚µ‚½ ©— 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‚Ü‚µ‚½";

			// ‚½ (‘¼)
			} else {
				return "‚½";
			}

		// Group B
		} else {

			// ‚Ü‚µ‚½ (‘¼j 
			return "‚Ü‚µ‚½";

		}
	}

	return "“‚Ü‚µ‚½“";
}



// `‚è‚Ü‚µ‚½ `‚Á‚½
string “‚è‚Ü‚µ‚½“(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// —ª‹LğŒ‚s‚Ì‚Ç‚ê‚ª‚ ‚Ä‚Í‚Ü‚é‚©H ‚Ç‚ê‚É‚à‚ ‚Ä‚Í‚Ü‚ç‚È‚©‚Á‚½‚ç—ª‹LğŒ‚x‚Å‚Ç‚¿‚ç‚©‚É“–‚Ä‚Í‚Ü‚éB
		int Group = “—ª‹LğŒ‚s“(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ‚è‚Ü‚µ‚½ ©— 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‚è‚Ü‚µ‚½";

			// ‚Á‚½ (‘¼)
			} else {
				return "‚Á‚½";
			}

		// Group B
		} else {

			// ‚è‚Ü‚µ‚½ (‘¼j 
			return "‚è‚Ü‚µ‚½";

		}
	}

	return "“‚è‚Ü‚µ‚½“";
}



// `‚Ü‚¹‚ñ‚Å‚µ‚½ `‚È‚©‚Á‚½
string “‚Ü‚¹‚ñ‚Å‚µ‚½“(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// —ª‹LğŒ‚s‚Ì‚Ç‚ê‚ª‚ ‚Ä‚Í‚Ü‚é‚©H ‚Ç‚ê‚É‚à‚ ‚Ä‚Í‚Ü‚ç‚È‚©‚Á‚½‚ç—ª‹LğŒ‚x‚Å‚Ç‚¿‚ç‚©‚É“–‚Ä‚Í‚Ü‚éB
		int Group = “—ª‹LğŒ‚s“(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ‚Ü‚¹‚ñ‚Å‚µ‚½ ©— 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‚Ü‚¹‚ñ‚Å‚µ‚½";

			// ‚È‚©‚Á‚½ (‘¼)
			} else {
				return "‚È‚©‚Á‚½";
			}

		// Group B
		} else {

			// ‚Ü‚¹‚ñ‚Å‚µ‚½ (‘¼j 
			return "‚Ü‚¹‚ñ‚Å‚µ‚½";

		}
	}

	return "“‚Ü‚¹‚ñ‚Å‚µ‚½“";
}



// `v‚¢‚Ü‚· `‘¶‚¶‚Ü‚· `v‚¤
string “v‚¢‚Ü‚·“(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// —ª‹LğŒ‚s‚Ì‚Ç‚ê‚ª‚ ‚Ä‚Í‚Ü‚é‚©H ‚Ç‚ê‚É‚à‚ ‚Ä‚Í‚Ü‚ç‚È‚©‚Á‚½‚ç—ª‹LğŒ‚x‚Å‚Ç‚¿‚ç‚©‚É“–‚Ä‚Í‚Ü‚éB
		int Group = “—ª‹LğŒ‚s“(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// v‚¢‚Ü‚· ©— 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "v‚¢‚Ü‚·";

			// v‚¤ (‘¼)
			} else {
				return "v‚¤";
			}

		// Group B
		} else {

			// ‘¶‚¶‚Ü‚· ©Œû=1 or ©Œû=3 or ©—
			if ( nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 3 || nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‘¶‚¶‚Ü‚·";

			// v‚¢‚Ü‚· (‘¼)
			} else {
				return "v‚¢‚Ü‚·";
			}

		}
	}

	return "“v‚¢‚Ü‚·“";
}


// `Q‚è‚Ü‚µ‚½ `Qã‚µ‚Ü‚µ‚½ `Q‚Á‚½
string “Q‚è‚Ü‚µ‚½“(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// —ª‹LğŒ‚s‚Ì‚Ç‚ê‚ª‚ ‚Ä‚Í‚Ü‚é‚©H ‚Ç‚ê‚É‚à‚ ‚Ä‚Í‚Ü‚ç‚È‚©‚Á‚½‚ç—ª‹LğŒ‚x‚Å‚Ç‚¿‚ç‚©‚É“–‚Ä‚Í‚Ü‚éB
		int Group = “—ª‹LğŒ‚s“(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// Q‚è‚Ü‚µ‚½ ©— 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "Q‚è‚Ü‚µ‚½";

			// Q‚Á‚½ (‘¼)
			} else {
				return "Q‚Á‚½";
			}

		// Group B
		} else {

			// Q‚è‚Ü‚µ‚½ ©—
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "Q‚è‚Ü‚µ‚½";

			// Qã‚µ‚Ü‚µ‚½ ©Œû=1 or ©Œû=5
			} else if ( nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 5 ) {
				return "Qã‚µ‚Ü‚µ‚½";

			// Q‚è‚Ü‚µ‚½ (‘¼)
			} else {
				return "Q‚è‚Ü‚µ‚½";
			}

		}
	}

	return "“Q‚è‚Ü‚µ‚½“";
}



// `‚­‚¾‚³‚ê `‰º‚³‚¢ `‚­‚ê
string “‚­‚¾‚³‚ê“(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// —ª‹LğŒ‚s‚Ì‚Ç‚ê‚ª‚ ‚Ä‚Í‚Ü‚é‚©H ‚Ç‚ê‚É‚à‚ ‚Ä‚Í‚Ü‚ç‚È‚©‚Á‚½‚ç—ª‹LğŒ‚x‚Å‚Ç‚¿‚ç‚©‚É“–‚Ä‚Í‚Ü‚éB
		int Group = “—ª‹LğŒ‚s“(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ‰º‚³‚¢ ©— 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‰º‚³‚¢";

			// ‚­‚ê (‘¼)
			} else {
				return "‚­‚ê";
			}

		// Group B
		} else {

			// ‰º‚³‚¢ ©— 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‰º‚³‚¢";

			// ‚­‚¾‚³‚ê (‘¼)
			} else {
				return "‚­‚¾‚³‚ê";
			}

		}
	}

	return "“‚­‚¾‚³‚ê“";
}



// `‚·‚İ‚Ü‚¹‚ñ `\‚µ–ó‚È‚¢ `‚·‚Ü‚Ê `‚·‚Ü‚ñ `‚²‚ß‚ñ‚È‚³‚¢
string “‚·‚İ‚Ü‚¹‚ñ“(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// —ª‹LğŒ‚s‚Ì‚Ç‚ê‚ª‚ ‚Ä‚Í‚Ü‚é‚©H ‚Ç‚ê‚É‚à‚ ‚Ä‚Í‚Ü‚ç‚È‚©‚Á‚½‚ç—ª‹LğŒ‚x‚Å‚Ç‚¿‚ç‚©‚É“–‚Ä‚Í‚Ü‚éB
		int Group = “—ª‹LğŒ‚s“(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ‚²‚ß‚ñ‚È‚³‚¢ ©— 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‚²‚ß‚ñ‚È‚³‚¢";

			// ‚·‚Ü‚ñ ©Œû=2 || ©Œû=4
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
				return "‚·‚Ü‚ñ";

			// ‚·‚Ü‚Ê (‘¼)
			} else {
				return "‚·‚Ü‚Ê";
			}

		// Group B
		} else {
			// \‚µ–ó‚È‚¢ ©Œû=5 or ©’j 
			if ( nb6bushouref[i1st_BushouID].tone == 5 || nb6bushouname[i1st_BushouID].sex == 0 ) {
				return "\‚µ–ó‚È‚¢";

			// ‚·‚İ‚Ü‚¹‚ñ (‘¼j 
			} else {
				return "‚·‚İ‚Ü‚¹‚ñ";

			}
		}
	}

	return "“‚·‚İ‚Ü‚¹‚ñ“";
}

// `Šè‚¢‚Ü‚· `‚¨Šè‚¢‚¢‚½‚· `Šè‚¨‚¤ `Šè‚¤ `‚¨Šè‚¢‚µ‚Ü‚·
string “Šè‚¢‚Ü‚·“(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// —ª‹LğŒ‚s‚Ì‚Ç‚ê‚ª‚ ‚Ä‚Í‚Ü‚é‚©H ‚Ç‚ê‚É‚à‚ ‚Ä‚Í‚Ü‚ç‚È‚©‚Á‚½‚ç—ª‹LğŒ‚x‚Å‚Ç‚¿‚ç‚©‚É“–‚Ä‚Í‚Ü‚éB
		int Group = “—ª‹LğŒ‚s“(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// Šè‚¢‚Ü‚· ©— 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "Šè‚¢‚Ü‚·";

			// Šè‚¤ ©Œû=2 || ©Œû=4
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
				return "Šè‚¤";

			// Šè‚¨‚¤ (‘¼)
			} else {
				return "Šè‚¨‚¤";
			}

		// Group B
		} else {
			// ‚¨Šè‚µ‚Ü‚· ©Œû=1 or ©Œû=3 or ©— 
			if ( nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 3 || nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‚¨Šè‚¢‚µ‚Ü‚·";

			// ‚¨Šè‚¢‚¢‚½‚· ©Œû=5
			} else if ( nb6bushouref[i1st_BushouID].tone == 5 ) {
				return "‚²‚´‚Á‚½";

			// Šè‚¢‚Ü‚· (‘¼j 
			} else {
				return "Šè‚¢‚Ü‚·";

			}
		}
	}

	return "“Šè‚¢‚Ü‚·“";
}



// `‚æ‚¢ `‚æ‚ë‚µ‚¢
string “‚æ‚¢“(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// —ª‹LğŒ‚s‚Ì‚Ç‚ê‚ª‚ ‚Ä‚Í‚Ü‚é‚©H ‚Ç‚ê‚É‚à‚ ‚Ä‚Í‚Ü‚ç‚È‚©‚Á‚½‚ç—ª‹LğŒ‚x‚Å‚Ç‚¿‚ç‚©‚É“–‚Ä‚Í‚Ü‚éB
		int Group = “—ª‹LğŒ‚s“(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ‚æ‚ë‚µ‚¢ ©— 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‚æ‚ë‚µ‚¢";

			// ‚æ‚¢ (‘¼)
			} else {
				return "‚æ‚¢";
			}

		// Group B
		} else {

			// ‚æ‚ë‚µ‚¢ ©Œû=1 or ©Œû=3 or ©—
			if ( nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 3 || nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "‚æ‚ë‚µ‚¢";

			// ‚æ‚¢ (‘¼)
			} else {
				return "‚æ‚¢";
			}

		}
	}

	return "“‚æ‚¢“";
}
