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



/*
【口調】:
0 無口			 217 真田幸村・長宗我部元親・島津義久・忍者全員 
1 礼儀正しい	 152 前田利家・明智光秀・立花宗茂・徳川光圀 
2 粗暴			 119 柴田勝家・龍造寺隆信・可児才蔵・佐々木小次郎 
3 高貴			 30 上杉憲政・今川義元・足利義晴・一条兼定 
4 生意気		 180 伊達政宗・三好長慶・鈴木重秀・由比正雪 
5 威厳			 143 織田信長・徳川家康・武田信玄・上杉謙信 
6 饒舌			 145 豊臣秀吉・斎藤道三・松永久秀・足利義昭 
7 一般			 314 本多忠勝・吉川元春・山中鹿之助・史実姫全員 
*/


string ％自名％		(int i1st_BushouID, int i2nd_BushouID			, int i3rd_BushouID = 0xFFFF);


// 00（相手に敬意を払う必要がない場合の自称） 
string ％わし％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	
	// 家族の場合を先に判定する。
	string szIsFamilyMsg = ％自名％( i1st_BushouID, i2nd_BushouID, i3rd_BushouID );

	if ( szIsFamilyMsg.length() > 0 ) {
		return szIsFamilyMsg;
	}

	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// わらわ 自女&自齢>35 
		if ( nb6bushouname[i1st_BushouID].sex == 1 && getYearsOld(i1st_BushouID) > 35 ) {
			return "わらわ";

		// わたくし 自女  
		} else if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
			return "わたくし";

		// まろ 自口=3 
		} else if ( nb6bushouref[i1st_BushouID].tone == 3 ) {
			return "まろ";

		// わし 自身=0or1 and 自齢>35  (0==隠居, 1==大名)
		} else if ( (nb6bushouname[i1st_BushouID].position==0 || nb6bushouname[i1st_BushouID].position==1) && getYearsOld(i1st_BushouID) > 35 ) {
			return "わし";

		// 俺 自口=2or4
		} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
			return "俺";

		// 私 自口=1 
		} else if ( nb6bushouref[i1st_BushouID].tone == 1 ) {
			return "私";

		// わし （他） 
		} else {
			return "わし";

		}
	}
	return "％わし％";
}



// 01（相手と親子・兄弟・夫婦にある場合の自称）
// 他のヘルパーなので、直接使わないこと!!
string ％自名％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {

	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		 // 娘 相自親子&自齢<相齢&自女 
		 if ( is自分は相手の娘(i1st_BushouID, i2nd_BushouID ) ) {
			  return "娘";

		 // 父 自相親子&自齢>相齢&自男 
		 } else if ( is自分は相手の父(i1st_BushouID, i2nd_BushouID ) ) {
			  return "父";

		 // 母 自相親子&自齢>相齢&自女 
		 } else if ( is自分は相手の母(i1st_BushouID, i2nd_BushouID ) ) {
			  return "母";

		 // 弟 自相兄弟&自齢<相齢&自男 
		 } else if ( is自分は相手の弟(i1st_BushouID, i2nd_BushouID ) ) {
			  return "弟";
		 
		 // 妹 自相兄弟&自齢<相齢&自女 
		 } else if ( is自分は相手の妹(i1st_BushouID, i2nd_BushouID ) ) {
			  return "妹";

		 // 兄 自相兄弟&自齢>相齢&自男
		 } else if ( is自分は相手の兄(i1st_BushouID, i2nd_BushouID ) ) {
			  return "兄";

		 // 姉 自相兄弟&自齢>相齢&自女
		 } else if ( is自分は相手の姉(i1st_BushouID, i2nd_BushouID ) ) {
			  return "姉";

		 // 夫 自相夫婦&自男
		 } else if ( is自分は相手の夫(i1st_BushouID, i2nd_BushouID ) ) {
			  return "夫";

		 // 妻 相自婦夫&自女
		 } else if ( is自分は相手の妻(i1st_BushouID, i2nd_BushouID ) ) {
			  return "妻";
		 }

	}
	return "";
}



// 02（その他の場合の自称） 
string ％私％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	
	// 家族の場合を先に判定する。
	string szIsFamilyMsg = ％自名％( i1st_BushouID, i2nd_BushouID, i3rd_BushouID );

	if ( szIsFamilyMsg.length() > 0 ) {
		return szIsFamilyMsg;
	}

	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// わたくし 自女  
		if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
			return "わたくし";

		// まろ 自相血縁&自口=3 
		} else if ( is自分は相手の血縁(i1st_BushouID, i2nd_BushouID) && nb6bushouref[i1st_BushouID].tone == 3 ) {
			return "まろ";

		// わし 自相血縁&自齢>相齢&自齢>35
		} else if ( is自分は相手の血縁(i1st_BushouID, i2nd_BushouID) && getYearsOld(i1st_BushouID) > getYearsOld(i2nd_BushouID) && getYearsOld(i1st_BushouID) > 35 ) {
			return "わし";

		// わし 自身=(0or1)&自齢>35  (0==隠居, 1==大名)
		} else if ( (nb6bushouname[i1st_BushouID].position==0 || nb6bushouname[i1st_BushouID].position==1) && getYearsOld(i1st_BushouID) > 35 ) {
			return "わし";

		// 拙者 自職=1or3 (忍者か剣豪)
		} else if ( nb6bushouref[i1st_BushouID].job==0x10 || nb6bushouref[i1st_BushouID].job==0x30 ) {
			return "拙者";

		// 拙僧 自職=4 (僧侶)
		} else if ( nb6bushouref[i1st_BushouID].job==0x40 ) {
			return "拙僧";

		// 俺 自口=2
		} else if ( nb6bushouref[i1st_BushouID].tone == 2 ) {
			return "俺";

		// それがし 自口=5
		} else if ( nb6bushouref[i1st_BushouID].tone == 5 ) {
			return "それがし";

		// 私 （他） 
		} else  {
			return "私";

		}
	}
	return "％私％";
}


// 03（所有形容詞）  
string ％我が％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		// 私の 自女 
		if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
			return "私の";
		} else {
			return "我が";
		}
	}
	return "％我が％";
}


// 04～07 相手をさげすむ場合「以外」の二人称
string ％相手％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		/*
		 * 04（相手と血縁関係にある場合の二人称）   
		 */ 
		// 自相親子&自齢<相齢&相男
		if ( is相手は自分の父(i1st_BushouID, i2nd_BushouID) ) {
			return "父上";

		// 自相親子&自齢<相齢&相女
		} else if ( is相手は自分の母(i1st_BushouID, i2nd_BushouID) ) {
			return "母上";

		// 兄上 自相兄弟&自齢<相齢&相男 
		} else if ( is相手は自分の兄(i1st_BushouID, i2nd_BushouID) ) {
			return "兄上";

		// 姉上 自相兄弟&自齢<相齢&相女 
		} else if ( is相手は自分の姉(i1st_BushouID, i2nd_BushouID) ) {
			return "姉上";

		// あなた 自相夫婦&相男 
		} else if ( is自分は相手の妻(i1st_BushouID, i2nd_BushouID) ) {
			return "あなた";

		// (相名) 自相親子&自齢>相齢 (自分は相手の親である)
		} else if ( is自分は相手の親(i1st_BushouID, i2nd_BushouID) ) {
			return nb6bushouname[i2nd_BushouID].fastname;

		// (相名)殿 自相血縁&相=大名
		} else if ( is自分は相手の血縁(i1st_BushouID, i2nd_BushouID) && nb6bushouname[i2nd_BushouID].State == 0) {
			return nb6bushouname[i2nd_BushouID].fastname + string("殿");

		// (相名) 自相兄弟&自齢>相齢&相≠大名
		} else if ( is自分は相手の兄(i1st_BushouID, i2nd_BushouID) && nb6bushouname[i2nd_BushouID].State != 0) {
			return nb6bushouname[i2nd_BushouID].fastname;

		// (相名) 自相夫婦&相女 
		} else if ( is自分は相手の夫(i1st_BushouID, i2nd_BushouID) ) {
			return nb6bushouname[i2nd_BushouID].fastname;

		// (相名)殿 自相血縁
		} else if ( is自分は相手の血縁(i1st_BushouID, i2nd_BushouID) ) {
			return nb6bushouname[i2nd_BushouID].fastname + string("殿");

		/*
		 *	05（自分が大名で相手がその家臣の場合の二人称）
		 */
		// ご隠居 自=大名&相身=0 
		} else if ( is相手は自分の家臣(i1st_BushouID, i2nd_BushouID) && nb6bushouname[i2nd_BushouID].position==0 ) {
			return "ご隠居";

		// じい 自齢<相仕&自齢≦30&相齢≧50&相男
		} else if ( is相手は自分の家臣(i1st_BushouID, i2nd_BushouID) && 
			        (getYearsOld(i1st_BushouID) < nb6bushouref[i2nd_BushouID].work) && getYearsOld(i1st_BushouID) <=30 && 
					getYearsOld(i2nd_BushouID) >= 50 && nb6bushouname[i2nd_BushouID].sex==0 ) {
			return "じい";
		// 猿 自=織田信長(=顔0)&相=豊臣秀吉(=顔5) 
		} else if ( is相手は自分の家臣(i1st_BushouID, i2nd_BushouID) && nb6bushouname[i1st_BushouID].number == 0 && nb6bushouname[i2nd_BushouID].number == 5 ) {
			return "猿";

		// (相名) （他） 
		} else if ( is相手は自分の家臣(i1st_BushouID, i2nd_BushouID) ) {
			return nb6bushouname[i2nd_BushouID].fastname;

		/*
		 *	06（相手が自分の大名の場合の二人称）
		 */
		// 若 相齢<自仕&相齢≦30&自齢≧50&相男 
		} else if ( is相手は自分の大名(i1st_BushouID, i2nd_BushouID) && 
					(getYearsOld(i2nd_BushouID) < nb6bushouref[i1st_BushouID].work) && getYearsOld(i2nd_BushouID) <=30 && 
					getYearsOld(i1st_BushouID) >= 50 && nb6bushouname[i2nd_BushouID].sex==0 ) {
			return "若";

		// お館様 自口≠3&33%
		} else if ( is相手は自分の大名(i1st_BushouID, i2nd_BushouID) && nb6bushouref[i1st_BushouID].tone != 3 && rand() % 3 == 0 ) {
			return "お館様";

		// (相名)様 （他） 
		} else if ( is相手は自分の大名(i1st_BushouID, i2nd_BushouID) ){
			return nb6bushouname[i2nd_BushouID].fastname + string("様");

		/*
		 *	07（その他の場合の二人称） 
		 */
		// (相名)殿 相身=1&自身=1 (1==大名)
		} else if ( nb6bushouname[i2nd_BushouID].position==1 && nb6bushouname[i1st_BushouID].position==1 ) {
			return nb6bushouname[i2nd_BushouID].fastname + string("殿");

		// (相名)様 相身=1&自身≠1 
		} else if ( nb6bushouname[i2nd_BushouID].position==1 && nb6bushouname[i1st_BushouID].position!=1 ) {
			return nb6bushouname[i2nd_BushouID].fastname + string("様");

		// ご隠居 相身=0 
		} else if ( nb6bushouname[i2nd_BushouID].position==0 ) {
			return "ご隠居";

		// (相名) 自身≧相身(内部番号的には自身≦相身)&自齢≧相齢&自属=相属  (身分=0:隠居 1:大名 2:宿老 3:家老 …なので、番号が小さい方が強い)
		} else if ( nb6bushouname[i1st_BushouID].position <= nb6bushouname[i2nd_BushouID].position && 
					getYearsOld(i1st_BushouID) >= getYearsOld(i2nd_BushouID) && is自分と相手は同じ大名家所属(i1st_BushouID, i2nd_BushouID) ) {
			return nb6bushouname[i2nd_BushouID].fastname;

		// (相姓)様 自身>相身+1(3階級自分が下の身分ということ)&自口≠4 
		} else if ( nb6bushouname[i1st_BushouID].position > nb6bushouname[i2nd_BushouID].position+1 && 
			nb6bushouref[i1st_BushouID].tone != 4 ) {
			return nb6bushouname[i2nd_BushouID].familyname + string("様");

		// (相名)様 相女 
		} else if ( nb6bushouname[i2nd_BushouID].sex == 1 ) {
			return nb6bushouname[i2nd_BushouID].fastname + string("様");

		// (相名)殿 （他） 
		} else {
			return nb6bushouname[i2nd_BushouID].fastname + string("殿");
		}

	}
	return "％相手％";
}


// 08 相手をさげすむ場合の二人称
string ％罵倒相手％		(int i1st_BushouID, int i2nd_BushouID , int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {
		// 小わっぱ 相男&相齢<18&自齢-相齢>5 
		if ( nb6bushouname[i2nd_BushouID].sex == 0 && getYearsOld(i2nd_BushouID) < 18 && getYearsOld(i1st_BushouID)-getYearsOld(i2nd_BushouID) > 5 ) {
			return "小わっぱ";

		// 小娘 相女&相齢<18&自齢-相齢>5 
		} else if ( nb6bushouname[i2nd_BushouID].sex == 1 && getYearsOld(i2nd_BushouID) < 18 && getYearsOld(i1st_BushouID)-getYearsOld(i2nd_BushouID) > 5 ) {
			return "小娘";

		// 青二才 相齢<30&自齢-相齢>20 
		} else if ( getYearsOld(i2nd_BushouID) < 30 && getYearsOld(i1st_BushouID)-getYearsOld(i2nd_BushouID) > 20 ) {
			return "青二才";

		// 老いぼれ 相齢>60&相齢-自齢>20 
		} else if ( getYearsOld(i2nd_BushouID) > 60 && getYearsOld(i2nd_BushouID)-getYearsOld(i1st_BushouID) > 20 ) {
			return "老いぼれ";

		// くそ坊主
		} else if ( nb6bushouref[i2nd_BushouID].job == 0x40 ) {
			return "くそ坊主";

		// えせ茶人
		} else if ( nb6bushouref[i2nd_BushouID].job == 0x20 ) {
			return "えせ茶人";

		// 伴天連
		} else if ( nb6bushouref[i2nd_BushouID].job != 0x90 && nb6bushouref[i2nd_BushouID].job == 0x90 ) {
			return "伴天連";

		} else {
			return nb6bushouname[i2nd_BushouID].fastname;
		}
	}
	return "％罵倒相手％";
}


// 09～0D 三名をさげすむ場合「以外」の三人称
string ％三名％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i3rd_BushouID && i3rd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		/*
		 * 0A09（大名が血縁関係にある武将を指す場合の三人称） 
		 */ 
		// 父上 自三親子&自齢<三齢&三男 (1==大名)
		if ( nb6bushouname[i1st_BushouID].position == 1 && is相手は自分の父(i1st_BushouID, i3rd_BushouID) ) {
			return "父上";

		// 母上 自三親子&自齢<三齢&三女
		} else if ( nb6bushouname[i1st_BushouID].position == 1 && is相手は自分の母(i1st_BushouID, i3rd_BushouID) ) {
			return "母上";

		// 兄上 自三兄弟&自齢<三齢&三男
		} else if ( nb6bushouname[i1st_BushouID].position == 1 && is相手は自分の兄(i1st_BushouID, i3rd_BushouID) ) {
			return "兄上";

		// 姉上 自三兄弟&自齢<三齢&三女
		} else if ( nb6bushouname[i1st_BushouID].position == 1 && is相手は自分の姉(i1st_BushouID, i3rd_BushouID) ) {
			return "姉上";

		// 姉上 自三兄弟&自齢<三齢&三女
		} else if ( nb6bushouname[i1st_BushouID].position == 1 && is自分は相手の血縁(i1st_BushouID, i3rd_BushouID) && nb6bushouname[i3rd_BushouID].sex == 1 ) {
			return nb6bushouname[i3rd_BushouID].fastname;

		// (三名殿) 血縁関係
		} else if ( nb6bushouname[i1st_BushouID].position == 1 && is自分は相手の血縁(i1st_BushouID, i3rd_BushouID) ) {
			return nb6bushouname[i3rd_BushouID].fastname + string("殿");

		// ご隠居 三身=0 
		} else if ( nb6bushouname[i1st_BushouID].position == 1 && nb6bushouname[i3rd_BushouID].position==0 ) {
			return "ご隠居";

		/*
		 * 0A（血縁・夫婦関係にある大名を指す場合の三人称） 
		 */ 
		// 父 自三親子&自齢<三齢&三男
		} else if ( nb6bushouname[i3rd_BushouID].position == 1 && is相手は自分の父(i1st_BushouID, i3rd_BushouID) ) {
			return "父";

		// 母 自三親子&自齢<三齢&三女
		} else if ( nb6bushouname[i3rd_BushouID].position == 1 && is相手は自分の母(i1st_BushouID, i3rd_BushouID) ) {
			return "母";

		// 兄 自三兄弟&自齢<三齢&三男
		} else if ( nb6bushouname[i3rd_BushouID].position == 1 && is相手は自分の兄(i1st_BushouID, i3rd_BushouID) ) {
			return "兄";

		// 姉 自三兄弟&自齢<三齢&三女
		} else if ( nb6bushouname[i3rd_BushouID].position == 1 && is相手は自分の姉(i1st_BushouID, i3rd_BushouID) ) {
			return "姉";

		// 弟 自三兄弟&自齢>三齢&三男
		} else if ( nb6bushouname[i3rd_BushouID].position == 1 && is相手は自分の弟(i1st_BushouID, i3rd_BushouID) ) {
			return "弟";

		// 妹 自三兄弟&自齢>三齢&三女
		} else if ( nb6bushouname[i3rd_BushouID].position == 1 && is相手は自分の妹(i1st_BushouID, i3rd_BushouID) ) {
			return "妹";

		// 家内 自三夫婦&相女
		} else if ( nb6bushouname[i3rd_BushouID].position == 1 && is自分は相手の夫(i1st_BushouID, i3rd_BushouID) ) {
			return "家内";

		// 夫 自三夫婦&相男
		} else if ( nb6bushouname[i3rd_BushouID].position == 1 && is自分は相手の妻(i1st_BushouID, i3rd_BushouID) ) {
			return "夫";

		// (三名) 自三血縁&自齢>相齢&三女 
		} else if ( nb6bushouname[i3rd_BushouID].position == 1 && is自分は相手の血縁(i1st_BushouID, i3rd_BushouID) && getYearsOld( i1st_BushouID ) > getYearsOld( i3rd_BushouID ) && nb6bushouname[i3rd_BushouID].sex == 1 ) {
			return nb6bushouname[i3rd_BushouID].fastname;

		// (三名殿) 血縁関係
		} else if ( nb6bushouname[i3rd_BushouID].position == 1 && is自分は相手の血縁(i1st_BushouID, i3rd_BushouID) ) {
			return nb6bushouname[i3rd_BushouID].fastname + string("殿");

		/*
		 * 0B（自分が大名であり、血縁関係ではない場合の三人称） 
		 */ 
		// じい 自齢<三仕&自齢≦30&三齢≧50&？&三男 
		} else if ( nb6bushouname[i1st_BushouID].position == 1 && 
			        (getYearsOld(i1st_BushouID) < nb6bushouref[i3rd_BushouID].work) && getYearsOld(i1st_BushouID) <=30 && 
					getYearsOld(i3rd_BushouID) >= 50 && nb6bushouname[i3rd_BushouID].sex==0 ) {
			return "じい";

		// (三名) 自三血縁&自齢>相齢&三女 
		} else if ( nb6bushouname[i1st_BushouID].position == 1 && nb6bushouname[i3rd_BushouID].position == 1 && getYearsOld( i1st_BushouID ) > getYearsOld( i3rd_BushouID ) && nb6bushouname[i3rd_BushouID].sex == 1 ) {
			return nb6bushouname[i3rd_BushouID].fastname;

		// (三名殿) 血縁関係
		} else if (nb6bushouname[i1st_BushouID].position == 1 &&  nb6bushouname[i3rd_BushouID].position == 1 ) {
			return nb6bushouname[i3rd_BushouID].fastname + string("殿");

		/*
		 *	0C（相手が自分の大名の場合の三人称）
		 */
		// 若 相齢<自仕&相齢≦30&自齢≧50&相男 
		} else if ( is相手は自分の大名(i1st_BushouID, i3rd_BushouID) && 
					(getYearsOld(i3rd_BushouID) < nb6bushouref[i1st_BushouID].work) && getYearsOld(i3rd_BushouID) <=30 && 
					getYearsOld(i1st_BushouID) >= 50 && nb6bushouname[i3rd_BushouID].sex==0 ) {
			return "若";

		// お館様 33%
		} else if ( is相手は自分の大名(i1st_BushouID, i3rd_BushouID) && rand() % 3 == 0 ) {
			return "お館様";

		// (相名)様 （他） 
		} else if ( is相手は自分の大名(i1st_BushouID, i3rd_BushouID) ){
			return nb6bushouname[i3rd_BushouID].fastname + string("様");
		
		/*
		 * 0D（その他の場合の三人称） 
		 */ 
		// 父 自三親子&自齢<三齢&三男
		} else if ( is相手は自分の父(i1st_BushouID, i3rd_BushouID) ) {
			return "父";

		// 母 自三親子&自齢<三齢&三女
		} else if ( is相手は自分の母(i1st_BushouID, i3rd_BushouID) ) {
			return "母";

		// 兄 自三兄弟&自齢<三齢&三男
		} else if ( is相手は自分の兄(i1st_BushouID, i3rd_BushouID) ) {
			return "兄";

		// 姉 自三兄弟&自齢<三齢&三女
		} else if ( is相手は自分の姉(i1st_BushouID, i3rd_BushouID) ) {
			return "姉";

		// 弟 自三兄弟&自齢>三齢&三男
		} else if ( is相手は自分の弟(i1st_BushouID, i3rd_BushouID) ) {
			return "弟";

		// 妹 自三兄弟&自齢>三齢&三女
		} else if ( is相手は自分の妹(i1st_BushouID, i3rd_BushouID) ) {
			return "妹";

		// 家内 自三夫婦&相女
		} else if ( is自分は相手の夫(i1st_BushouID, i3rd_BushouID) ) {
			return "家内";

		// 夫 自三夫婦&相男
		} else if ( is自分は相手の妻(i1st_BushouID, i3rd_BushouID) ) {
			return "夫";

		// (三名) 自三血縁&自齢>相齢&三女 
		} else if ( is自分は相手の血縁(i1st_BushouID, i3rd_BushouID) && getYearsOld( i1st_BushouID ) > getYearsOld( i3rd_BushouID ) && nb6bushouname[i3rd_BushouID].sex == 1 ) {
			return nb6bushouname[i3rd_BushouID].fastname;

		// (三名殿) 血縁関係
		} else if ( is自分は相手の血縁(i1st_BushouID, i3rd_BushouID) ) {
			return nb6bushouname[i3rd_BushouID].fastname + string("殿");

		// (三名) 自身≧三身(内部番号的には自身≦三身)&自齢≧三齢&自属=三属  (身分=0:隠居 1:大名 2:宿老 3:家老 …なので、番号が小さい方が強い)
		} else if ( nb6bushouname[i1st_BushouID].position <= nb6bushouname[i3rd_BushouID].position && 
					getYearsOld(i3rd_BushouID) >= getYearsOld(i3rd_BushouID) && is自分と相手は同じ大名家所属(i1st_BushouID, i3rd_BushouID) ) {
			return nb6bushouname[i3rd_BushouID].fastname;

		// (三姓)様 自身>三身+1(3階級自分が下の身分ということ)&自口≠4 
		} else if ( nb6bushouname[i1st_BushouID].position > nb6bushouname[i3rd_BushouID].position+1 && 
			nb6bushouref[i1st_BushouID].tone != 4 ) {
			return nb6bushouname[i3rd_BushouID].familyname + string("様");

		// (三名)様 三女 
		} else if ( nb6bushouname[i3rd_BushouID].sex == 1 ) {
			return nb6bushouname[i3rd_BushouID].fastname + string("様");

		// (相名)殿 （他） 
		} else {
			return nb6bushouname[i3rd_BushouID].fastname + string("殿");
		}

	}
	return "％三名％";
}




// 0E（第三者をさげすむ場合の三人称） 
string ％罵倒三名％		(int i1st_BushouID, int i2nd_BushouID , int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i3rd_BushouID && i3rd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {
		// 小わっぱ 三男&三齢<18&自齢-三齢>5 
		if ( nb6bushouname[i3rd_BushouID].sex == 0 && getYearsOld(i3rd_BushouID) < 18 && getYearsOld(i1st_BushouID)-getYearsOld(i3rd_BushouID) > 5 ) {
			return "小わっぱ";

		// 小娘 三女&三齢<18&自齢-三齢>5 
		} else if ( nb6bushouname[i3rd_BushouID].sex == 1 && getYearsOld(i3rd_BushouID) < 18 && getYearsOld(i1st_BushouID)-getYearsOld(i3rd_BushouID) > 5 ) {
			return "小娘";

		// 青二才 三齢<30&自齢-三齢>20 
		} else if ( getYearsOld(i3rd_BushouID) < 30 && getYearsOld(i1st_BushouID)-getYearsOld(i3rd_BushouID) > 20 ) {
			return "青二才";

		// 老いぼれ 三齢>60&三齢-自齢>20 
		} else if ( getYearsOld(i3rd_BushouID) > 60 && getYearsOld(i3rd_BushouID)-getYearsOld(i1st_BushouID) > 20 ) {
			return "老いぼれ";

		// (三名) （他） 
		} else {
			return nb6bushouname[i3rd_BushouID].fastname;
		}
	}
	return "％罵倒三名％";
}


