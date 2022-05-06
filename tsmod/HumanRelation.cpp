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



bool isŽ©•ª‚Í‘ŠŽè‚Ì–º( int i1st_BushouID, int i2nd_BushouID ) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		if ( nb6bushouref[i1st_BushouID].parent == nb6bushouname[i2nd_BushouID].number && 
			 getYearsOld(i1st_BushouID) < getYearsOld(i2nd_BushouID) &&
			 nb6bushouname[i1st_BushouID].sex == 1 ) {
			
			 return true;
		}
	}
	return false;
}

bool isŽ©•ª‚Í‘ŠŽè‚Ì‘§Žq( int i1st_BushouID, int i2nd_BushouID ) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		if ( nb6bushouref[i1st_BushouID].parent == nb6bushouname[i2nd_BushouID].number && 
			 getYearsOld(i1st_BushouID) < getYearsOld(i2nd_BushouID) &&
			 nb6bushouname[i1st_BushouID].sex == 0 ) {
			
			 return true;
		}
	}
	return false;
}

bool isŽ©•ª‚Í‘ŠŽè‚ÌŽq( int i1st_BushouID, int i2nd_BushouID ) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		if ( nb6bushouref[i2nd_BushouID].parent == nb6bushouname[i1st_BushouID].number && 
			 getYearsOld(i1st_BushouID) < getYearsOld(i2nd_BushouID) ) {

			 return true;
		}
	}
	return false;
}

bool isŽ©•ª‚Í‘ŠŽè‚Ìe( int i1st_BushouID, int i2nd_BushouID ) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {
		if ( nb6bushouref[i2nd_BushouID].parent == nb6bushouname[i1st_BushouID].number && 
			 getYearsOld(i1st_BushouID) > getYearsOld(i2nd_BushouID) ) {

			 return true;
		}
	}
	return false;
}

bool isŽ©•ª‚Í‘ŠŽè‚Ì•ƒ( int i1st_BushouID, int i2nd_BushouID ) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		if ( nb6bushouref[i2nd_BushouID].parent == nb6bushouname[i1st_BushouID].number && 
			 getYearsOld(i1st_BushouID) > getYearsOld(i2nd_BushouID) &&
			 nb6bushouname[i1st_BushouID].sex == 0 ) {

			 return true;
		}
	}
	return false;
}

bool is‘ŠŽè‚ÍŽ©•ª‚Ì•ƒ( int i1st_BushouID, int i2nd_BushouID ) {
	return isŽ©•ª‚Í‘ŠŽè‚Ì•ƒ(i2nd_BushouID, i1st_BushouID);
}



bool isŽ©•ª‚Í‘ŠŽè‚Ì•ê( int i1st_BushouID, int i2nd_BushouID ) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		if ( nb6bushouref[i2nd_BushouID].parent == nb6bushouname[i1st_BushouID].number && 
			 getYearsOld(i1st_BushouID) > getYearsOld(i2nd_BushouID) &&
			 nb6bushouname[i1st_BushouID].sex == 1 ) {

			 return true;
		}
	}
	return false;
}

bool is‘ŠŽè‚ÍŽ©•ª‚Ì•ê( int i1st_BushouID, int i2nd_BushouID ) {
	return isŽ©•ª‚Í‘ŠŽè‚Ì•ê(i2nd_BushouID, i1st_BushouID);
}

bool isŽ©•ª‚Í‘ŠŽè‚Ì’í( int i1st_BushouID, int i2nd_BushouID ) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		if ( nb6bushouref[i1st_BushouID].parent < 5000 && 
			 nb6bushouref[i1st_BushouID].parent == nb6bushouref[i2nd_BushouID].parent && 
			 getYearsOld(i1st_BushouID) < getYearsOld(i2nd_BushouID) &&
			 nb6bushouname[i1st_BushouID].sex == 0 ) {

			 return true;
		}
	}
	return false;
}

bool is‘ŠŽè‚ÍŽ©•ª‚Ì’í( int i1st_BushouID, int i2nd_BushouID ) {
	return isŽ©•ª‚Í‘ŠŽè‚Ì’í( i2nd_BushouID, i1st_BushouID );
}

bool isŽ©•ª‚Í‘ŠŽè‚Ì–…( int i1st_BushouID, int i2nd_BushouID ) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		if ( nb6bushouref[i1st_BushouID].parent < 5000 && 
			 nb6bushouref[i1st_BushouID].parent == nb6bushouref[i2nd_BushouID].parent && 
			 getYearsOld(i1st_BushouID) < getYearsOld(i2nd_BushouID) &&
			 nb6bushouname[i1st_BushouID].sex == 1 ) {

			 return true;
		}
	}
	return false;
}

bool is‘ŠŽè‚ÍŽ©•ª‚Ì–…( int i1st_BushouID, int i2nd_BushouID ) {
	return isŽ©•ª‚Í‘ŠŽè‚Ì–…( i2nd_BushouID, i1st_BushouID );
}

bool isŽ©•ª‚Í‘ŠŽè‚ÌŒZ( int i1st_BushouID, int i2nd_BushouID ) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		if ( nb6bushouref[i1st_BushouID].parent < 5000 && 
			 nb6bushouref[i1st_BushouID].parent == nb6bushouref[i2nd_BushouID].parent && 
			 getYearsOld(i1st_BushouID) > getYearsOld(i2nd_BushouID) &&
			 nb6bushouname[i1st_BushouID].sex == 0 ) {

			 return true;
		}
	}
	return false;
}

bool is‘ŠŽè‚ÍŽ©•ª‚ÌŒZ( int i1st_BushouID, int i2nd_BushouID ) {
	return isŽ©•ª‚Í‘ŠŽè‚ÌŒZ( i2nd_BushouID, i1st_BushouID);
}


bool isŽ©•ª‚Í‘ŠŽè‚ÌŽo( int i1st_BushouID, int i2nd_BushouID ) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		if ( nb6bushouref[i1st_BushouID].parent < 5000 && 
			 nb6bushouref[i1st_BushouID].parent == nb6bushouref[i2nd_BushouID].parent && 
			 getYearsOld(i1st_BushouID) > getYearsOld(i2nd_BushouID) &&
			 nb6bushouname[i1st_BushouID].sex == 1 ) {

			 return true;
		}
	}
	return false;
}

bool is‘ŠŽè‚ÍŽ©•ª‚ÌŽo( int i1st_BushouID, int i2nd_BushouID ) {
	return isŽ©•ª‚Í‘ŠŽè‚ÌŽo( i2nd_BushouID, i1st_BushouID);
}

bool isŽ©•ª‚Æ‘ŠŽè‚ÍŒZ’í( int i1st_BushouID, int i2nd_BushouID ) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		if ( nb6bushouref[i1st_BushouID].parent < 5000 && 
			 nb6bushouref[i1st_BushouID].parent == nb6bushouref[i2nd_BushouID].parent &&
			 getYearsOld(i1st_BushouID) > getYearsOld(i2nd_BushouID) &&
			 nb6bushouname[i1st_BushouID].sex == 0 && nb6bushouname[i2nd_BushouID].sex == 0) {

			 return true;
		}
	}
	return false;
}

bool isŽ©•ª‚Æ‘ŠŽè‚ÍŽo–…( int i1st_BushouID, int i2nd_BushouID ) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		if ( nb6bushouref[i1st_BushouID].parent < 5000 && 
			 nb6bushouref[i1st_BushouID].parent == nb6bushouref[i2nd_BushouID].parent && 
			 getYearsOld(i1st_BushouID) > getYearsOld(i2nd_BushouID) &&
			 nb6bushouname[i1st_BushouID].sex == 1 && nb6bushouname[i2nd_BushouID].sex == 1) {

			 return true;
		}
	}
	return false;
}


bool isŽ©•ª‚Æ‘ŠŽè‚ÍŽo’í( int i1st_BushouID, int i2nd_BushouID ) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		if ( nb6bushouref[i1st_BushouID].parent < 5000 && 
			 nb6bushouref[i1st_BushouID].parent == nb6bushouref[i2nd_BushouID].parent &&
			 getYearsOld(i1st_BushouID) > getYearsOld(i2nd_BushouID) &&
			 nb6bushouname[i1st_BushouID].sex == 1 && nb6bushouname[i2nd_BushouID].sex == 0) {

			 return true;
		}
	}
	return false;
}



bool isŽ©•ª‚Æ‘ŠŽè‚ÍŒZ–…( int i1st_BushouID, int i2nd_BushouID ) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		if ( nb6bushouref[i1st_BushouID].parent < 5000 && 
			 nb6bushouref[i1st_BushouID].parent == nb6bushouref[i2nd_BushouID].parent &&
			 getYearsOld(i1st_BushouID) > getYearsOld(i2nd_BushouID) &&
			 nb6bushouname[i1st_BushouID].sex == 0 && nb6bushouname[i2nd_BushouID].sex == 1) {

			 return true;
		}
	}
	return false;
}




bool isŽ©•ª‚Í‘ŠŽè‚Ì•v( int i1st_BushouID, int i2nd_BushouID ) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		if ( nb6bushouref[i1st_BushouID].spouse != 0xFF &&
			 nb6bushouref[i1st_BushouID].spouse == nb6bushouref[i2nd_BushouID].spouse && 
			 nb6bushouname[i1st_BushouID].sex == 0 ) {

			 return true;
		}
	}
	return false;
}

bool isŽ©•ª‚Í‘ŠŽè‚ÌÈ( int i1st_BushouID, int i2nd_BushouID ) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		if ( nb6bushouref[i1st_BushouID].spouse != 0xFF &&
			 nb6bushouref[i1st_BushouID].spouse == nb6bushouref[i2nd_BushouID].spouse && 
			 nb6bushouname[i1st_BushouID].sex == 1 ) {

			 return true;
		}
	}
	return false;
}


bool isŽ©•ª‚Í‘ŠŽè‚ÌŒŒ‰( int i1st_BushouID, int i2nd_BushouID ) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		if ( nb6bushouref[i1st_BushouID].blood != 0xFF &&
			nb6bushouref[i1st_BushouID].blood == nb6bushouref[i2nd_BushouID].blood ) {

			 return true;
		}
	}
	return false;
}

bool is‘ŠŽè‚ÍŽ©•ª‚Ì‰Æb( int i1st_BushouID, int i2nd_BushouID ) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {
		 // ‘æ‚PlÌ‚Í‘å–¼
		if ( nb6bushouname[i1st_BushouID].State == 0 ) {
			// ‘æ‚QlÌ‚Í‘å–¼‚É‚¿‚á‚ñ‚ÆŠ‘®‚µ‚Ä‚¢‚éB
			if ( 0 < nb6bushouref[i2nd_BushouID].attach && nb6bushouref[i2nd_BushouID].attach <= GAMEDATASTRUCT_DAIMYO_NUM ) {
				int iAttachDaimyoID = nb6bushouref[i2nd_BushouID].attach-1;
				if ( i1st_BushouID == nb6daimyo[iAttachDaimyoID].attach-1 ) {
					return true;
				}
			}
		}
	}
	return false;
}

bool is‘ŠŽè‚ÍŽ©•ª‚Ì‘å–¼( int i1st_BushouID, int i2nd_BushouID ) {
	return is‘ŠŽè‚ÍŽ©•ª‚Ì‰Æb( i2nd_BushouID, i1st_BushouID );
}

// ‰Æb‚Í‚à‚¿‚ë‚ñA‘å–¼Ž©g‚àŽ©•ª‚Ì‘å–¼‰Æ‚ÉŠ‘®‚µ‚Ä‚¢‚é‚Æ‚Ý‚È‚³‚ê‚éB
bool isŽ©•ª‚Æ‘ŠŽè‚Í“¯‚¶‘å–¼‰ÆŠ‘®( int i1st_BushouID, int i2nd_BushouID ) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		int i1stAttachDaimyoID = -1;
		int i2ndAttachDaimyoID = -1;

		// ‘æ‚PlÌ‚Í‘å–¼‚É‚¿‚á‚ñ‚ÆŠ‘®‚µ‚Ä‚¢‚éB
		if ( 0 < nb6bushouref[i1st_BushouID].attach && nb6bushouref[i1st_BushouID].attach <= GAMEDATASTRUCT_DAIMYO_NUM ) {
			i1stAttachDaimyoID = nb6bushouref[i1st_BushouID].attach-1;
		}

		// ‘æ‚QlÌ‚Í‘å–¼‚É‚¿‚á‚ñ‚ÆŠ‘®‚µ‚Ä‚¢‚éB
		if ( 0 < nb6bushouref[i2nd_BushouID].attach && nb6bushouref[i2nd_BushouID].attach <= GAMEDATASTRUCT_DAIMYO_NUM ) {
			i2ndAttachDaimyoID = nb6bushouref[i2nd_BushouID].attach-1;
		}

		if ( i1stAttachDaimyoID != -1 && i2ndAttachDaimyoID != -1 ) {
			if ( i1stAttachDaimyoID == i2ndAttachDaimyoID ) { 
				return true;
			}
		}
	}
	return false;
}
