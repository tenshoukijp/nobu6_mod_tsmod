

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
#include "MessageInflectionSurfix.h"
#include "HumanRelation.h"



// ～か ～ね
string ％か（１）％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ね 自女 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "ね";

		// か （他） 
		} else {
			return "か";
		}
	}

	return "％か（１）％";
}




// ～か ～なの
string ％か（２）％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// なの 自女 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "なの";

		// か （他） 
		} else {
			return "か";
		}
	}

	return "％か（２）％";
}





// ～か ～わ
string ％か（３）％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// わ 自女 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "わ";

		// か （他） 
		} else {
			return "か";
		}
	}

	return "％か（３）％";
}



// ～か ～かしら
string ％か（４）％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// かしら 自女 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "かしら";

		// か （他） 
		} else {
			return "か";
		}
	}

	return "％か（４）％";
}





// ～ぞ ～ぜ ～ぞよ ～わ
string ％ぞ（１）％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// わ 自女 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "わ";

		// ぞよ 自口=3  
		} else if ( nb6bushouref[i1st_BushouID].tone == 3) {
			return "ぞよ";

		// ぜ 自口=2&自身=1
		} else if ( nb6bushouref[i1st_BushouID].tone == 2 && nb6bushouname[i1st_BushouID].position==1 ) {
			return "ぜ";

		// ぞ （他） 
		} else {
			return "ぞ";
		}
	}

	return "％ぞ（１）％";
}




// ～ぞ ～ぜ ～ぞよ ～わよ
string ％ぞ（２）％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// わよ 自女 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "わよ";

		// ぞよ 自口=3  
		} else if ( nb6bushouref[i1st_BushouID].tone == 3) {
			return "ぞよ";

		// ぜ 自口=2&自身=1
		} else if ( nb6bushouref[i1st_BushouID].tone == 2 && nb6bushouname[i1st_BushouID].position==1 ) {
			return "ぜ";

		// ぞ （他） 
		} else {
			return "ぞ";
		}
	}

	return "％ぞ（２）％";
}







// ～な ～わ
string ％な（１）％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// わ 自女 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "わ";

		// な （他） 
		} else {
			return "な";
		}
	}

	return "％な（１）％";
}





// ～な ～ね
string ％な（２）％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ね 自女 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "ね";

		// な （他） 
		} else {
			return "な";
		}
	}

	return "％な（２）％";
}




// ～(無し) ～わ
string ％わ（１）％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// わ 自女 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "わ";

		// (無し) （他） 
		} else {
			return "";
		}
	}

	return "％わ（１）％";
}




// ～わ ～わい
string ％わ（２）％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// わ 自女 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "わ";

		// わい 自口=2&自齢≧50 
		} else if ( nb6bushouref[i1st_BushouID].tone == 2 && getYearsOld(i1st_BushouID) >= 50 ) {
			return "わい";

		// わ （他） 
		} else {
			return "わ";

		}
	}

	return "％わ（２）";
}






// ～なあ ～のお ～ねえ 
string ％なあ％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ねえ 自女 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "ねえ";

		// のお 自口=2&自齢≧50 
		} else if ( nb6bushouref[i1st_BushouID].tone == 2 && getYearsOld(i1st_BushouID) >= 50 ) {
			return "のお";

		// なあ （他） 
		} else {
			return "なあ";

		}
	}

	return "％なあ％";
}






// ～お ～(無し)
string ％お（１）％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// (無し) 自相血縁&自齢≧相齢 
		if ( is自分は相手の血縁( i1st_BushouID, i2nd_BushouID ) && getYearsOld(i1st_BushouID) >= getYearsOld(i2nd_BushouID) ) {
			return "";

		// お （他） 
		} else {
			return "お";

		}
	}

	return "％お（１）％";
}




// ～お ～(無し)
string ％お（２）％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// (無し) 自相血縁&自齢≧相齢 
		if ( is自分は相手の血縁( i1st_BushouID, i2nd_BushouID ) && getYearsOld(i1st_BushouID) >= getYearsOld(i2nd_BushouID) ) {
			return "";

		// お 自口=1or3or自女 
		} else if ( nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 3 || nb6bushouname[i1st_BushouID].sex == 1 ) {
			return "お";

		} else {
			return "";
		}
	}

	return "％お（２）％";
}







// ははは ふふふ くくく あはは わはは ふっ うふふ おほほ 
string ％ははは％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// おほほ 自女&自齢≧25 
		if ( nb6bushouname[i1st_BushouID].sex == 1 && getYearsOld(i1st_BushouID) >= 25 ) {
			return "おほほ";

		// うふふ 自女
		} else if ( nb6bushouname[i1st_BushouID].sex == 1  ) {
			return "うふふ";

		// わはは 自口=2
		} else if ( nb6bushouref[i1st_BushouID].tone == 2  ) {
			return "わはは";

		// ふっ 自口=0
		} else if ( nb6bushouref[i1st_BushouID].tone == 0  ) {
			return "ふっ";

		// あはは 自齢≦20 
		} else if ( getYearsOld(i1st_BushouID) <= 20 ) {
			return "あはは";

		// くくく 戦才B以下&自戦≦50
		} else if ( nb6bushouref[i1st_BushouID].aptitudebat <= 1 && getBushouBat(i1st_BushouID) <= 50 ) {
			return "くくく";

		// ふふふ 戦才B以下
		} else if ( nb6bushouref[i1st_BushouID].aptitudebat <= 1 ) {
			return "ふふふ";

		// ははは （他） 
		} else {
			return "ははは";
		}
	}

	return "％ははは％";
}



// ～い ～き
string ％い％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// き 自女 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "き";

		// い （他） 
		} else {
			return "い";
		}
	}

	return "％い％";
}




// うむむ まあ 
string ％うむむ％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// まあ 自女 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "まあ";

		// うむむ （他） 
		} else {
			return "うむむ";
		}
	}

	return "％うむむ％";
}



// おお まあ
string ％おお％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// まあ 自女 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "まあ";

		// おお （他） 
		} else {
			return "おお";
		}
	}

	return "％おお％";
}




// つまらぬ… おもしろくありません 
string ％つまらぬ％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// おもしろくありません 自女 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "おもしろくありません";

		// つまらぬ… （他） 
		} else {
			return "つまらぬ…";
		}
	}

	return "％つまらぬ…％";
}




// やっ し 
string ％やっ％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// し 自女 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "し";

		// やっ （他） 
		} else {
			return "やっ";
		}
	}

	return "％やっ％";
}




// ち てし
string ％ち％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// てし 自女 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "てし";

		// ち （他） 
		} else {
			return "ち";
		}
	}

	return "％ち％";
}





// れ られよ 
string ％れ％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// られよ 自齢<相齢  
		if ( getYearsOld(i1st_BushouID) < getYearsOld(i2nd_BushouID) ) {
			return "られよ";

		// れ （他） 
		} else {
			return "れ";
		}
	}

	return "％れ％";
}






// しんぜよう くれよう しんぜましょう 
string ％しんぜよう％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// しんぜましょう 自相血縁&自齢<相齢 
		if ( is自分は相手の血縁( i1st_BushouID, i2nd_BushouID ) && getYearsOld(i1st_BushouID) < getYearsOld(i2nd_BushouID) ) {
			return "しんぜましょう";

		// しんぜましょう 自女  
		} else if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
			return "しんぜましょう";

		// くれよう
		} else if ( nb6bushouref[i1st_BushouID].tone == 2 ) {
			return "くれよう";

		// しんぜよう （他） 
		} else {
			return "しんぜよう";
		}
	}

	return "％しんぜよう％";
}


// 一族 親子 姉妹 姉弟 兄妹 夫婦 兄弟 姉妹 姉弟 兄妹 
string ％一族％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		if ( is自分は相手の親(i1st_BushouID, i2nd_BushouID ) ) {
			return "親子";

		// 兄弟
		} else if ( is自分と相手は兄弟(i1st_BushouID, i2nd_BushouID) ) {
			return "兄弟";

		// 姉妹
		} else if ( is自分と相手は姉妹(i1st_BushouID, i2nd_BushouID) ) {
			return "姉妹";

		// 姉弟
		} else if ( is自分と相手は姉弟(i1st_BushouID, i2nd_BushouID) ) {
			return "姉弟";

		// 兄妹 
		} else if ( is自分と相手は兄妹(i1st_BushouID, i2nd_BushouID) ) {
			return "兄妹";

		} else if ( is自分は相手の夫(i1st_BushouID, i2nd_BushouID) ) {
			return "夫婦";

		} else if ( is自分は相手の妻(i1st_BushouID, i2nd_BushouID) ) {
			return "夫婦";

		} else {
			return "一族";

		}
	}

	return "％一族％";
}




// でござる だぞ、ははは でおじゃるぞえ ですわ 
string ％でござる％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ですわ 自女 
		if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
			return "しんぜましょう";

		// だぞ、ははは 自口=2
		} else if ( nb6bushouref[i1st_BushouID].tone == 2 ) {
			return "だぞ、" + ％ははは％(i1st_BushouID, i2nd_BushouID,i3rd_BushouID);

		// でおじゃるぞえ 自口=3 
		} else if ( nb6bushouref[i1st_BushouID].tone == 3 ) {
			return "でおじゃるぞえ";

		// でござる （他） 
		} else {
			return "でござる";
		}
	}

	return "％でござる％";
}




// すな しますな 
string ％すな％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// しますな 自女 
		if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
			return "しますな";


		// すな （他） 
		} else {
			return "すな";
		}
	}

	return "％すな％";
}





// てみよ なさい
string ％てみよ％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// なさい 自女 
		if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
			return "なさい";

		// てみよ （他） 
		} else {
			return "てみよ";
		}
	}

	return "％てみよ％";
}



// よ ませ
string ％よ％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ませ 自女 
		if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
			return "ませ";

		// よ （他） 
		} else {
			return "よ";
		}
	}

	return "％よ％";
}
