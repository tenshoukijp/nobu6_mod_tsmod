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



bool is自分は相手の娘( int i1st_BushouID, int i2nd_BushouID ) {
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

bool is自分は相手の息子( int i1st_BushouID, int i2nd_BushouID ) {
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

bool is自分は相手の子( int i1st_BushouID, int i2nd_BushouID ) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		if ( nb6bushouref[i2nd_BushouID].parent == nb6bushouname[i1st_BushouID].number && 
			 getYearsOld(i1st_BushouID) < getYearsOld(i2nd_BushouID) ) {

			 return true;
		}
	}
	return false;
}

bool is自分は相手の親( int i1st_BushouID, int i2nd_BushouID ) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {
		if ( nb6bushouref[i2nd_BushouID].parent == nb6bushouname[i1st_BushouID].number && 
			 getYearsOld(i1st_BushouID) > getYearsOld(i2nd_BushouID) ) {

			 return true;
		}
	}
	return false;
}

bool is自分は相手の父( int i1st_BushouID, int i2nd_BushouID ) {
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

bool is相手は自分の父( int i1st_BushouID, int i2nd_BushouID ) {
	return is自分は相手の父(i2nd_BushouID, i1st_BushouID);
}



bool is自分は相手の母( int i1st_BushouID, int i2nd_BushouID ) {
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

bool is相手は自分の母( int i1st_BushouID, int i2nd_BushouID ) {
	return is自分は相手の母(i2nd_BushouID, i1st_BushouID);
}

bool is自分は相手の弟( int i1st_BushouID, int i2nd_BushouID ) {
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

bool is相手は自分の弟( int i1st_BushouID, int i2nd_BushouID ) {
	return is自分は相手の弟( i2nd_BushouID, i1st_BushouID );
}

bool is自分は相手の妹( int i1st_BushouID, int i2nd_BushouID ) {
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

bool is相手は自分の妹( int i1st_BushouID, int i2nd_BushouID ) {
	return is自分は相手の妹( i2nd_BushouID, i1st_BushouID );
}

bool is自分は相手の兄( int i1st_BushouID, int i2nd_BushouID ) {
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

bool is相手は自分の兄( int i1st_BushouID, int i2nd_BushouID ) {
	return is自分は相手の兄( i2nd_BushouID, i1st_BushouID);
}


bool is自分は相手の姉( int i1st_BushouID, int i2nd_BushouID ) {
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

bool is相手は自分の姉( int i1st_BushouID, int i2nd_BushouID ) {
	return is自分は相手の姉( i2nd_BushouID, i1st_BushouID);
}

bool is自分と相手は兄弟( int i1st_BushouID, int i2nd_BushouID ) {
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

bool is自分と相手は姉妹( int i1st_BushouID, int i2nd_BushouID ) {
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


bool is自分と相手は姉弟( int i1st_BushouID, int i2nd_BushouID ) {
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



bool is自分と相手は兄妹( int i1st_BushouID, int i2nd_BushouID ) {
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




bool is自分は相手の夫( int i1st_BushouID, int i2nd_BushouID ) {
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

bool is自分は相手の妻( int i1st_BushouID, int i2nd_BushouID ) {
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


bool is自分は相手の血縁( int i1st_BushouID, int i2nd_BushouID ) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		if ( nb6bushouref[i1st_BushouID].blood != 0xFF &&
			nb6bushouref[i1st_BushouID].blood == nb6bushouref[i2nd_BushouID].blood ) {

			 return true;
		}
	}
	return false;
}

bool is相手は自分の家臣( int i1st_BushouID, int i2nd_BushouID ) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {
		 // 第１人称は大名
		if ( nb6bushouname[i1st_BushouID].State == 0 ) {
			// 第２人称は大名にちゃんと所属している。
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

bool is相手は自分の大名( int i1st_BushouID, int i2nd_BushouID ) {
	return is相手は自分の家臣( i2nd_BushouID, i1st_BushouID );
}

// 家臣はもちろん、大名自身も自分の大名家に所属しているとみなされる。
bool is自分と相手は同じ大名家所属( int i1st_BushouID, int i2nd_BushouID ) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		int i1stAttachDaimyoID = -1;
		int i2ndAttachDaimyoID = -1;

		// 第１人称は大名にちゃんと所属している。
		if ( 0 < nb6bushouref[i1st_BushouID].attach && nb6bushouref[i1st_BushouID].attach <= GAMEDATASTRUCT_DAIMYO_NUM ) {
			i1stAttachDaimyoID = nb6bushouref[i1st_BushouID].attach-1;
		}

		// 第２人称は大名にちゃんと所属している。
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
