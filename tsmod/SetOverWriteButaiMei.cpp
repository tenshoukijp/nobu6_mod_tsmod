#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXBushou_All.h"
#include "ScenarioMod.h"

/*
* 部隊名をそれぞれ分岐させる。部隊名の４つのchar *配列のいずれかを返す。
*/

extern int isOnEditingButaiMode;


// 武将の兵態がいずれかである(0=足軽,1=騎馬,2=鉄砲,3=鉄砲騎馬
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
	"暗黒騎士", // ←ロスローリアン(足軽)
	"暗黒騎士", // ←ロスローリアン(騎馬)
	"暗黒騎士", // ←ロスローリアン(鉄砲)
	"暗黒騎士", // ←ロスローリアン(騎馬鉄砲)
};

char *IsButaiArrayLoslorienWithDetail[4] = {
	" 暗黒騎士 (足軽) ", // ←ロスローリアン(足軽)
	" 暗黒騎士 (騎馬) ", // ←ロスローリアン(騎馬)
	" 暗黒騎士 (鉄砲) ", // ←ロスローリアン(鉄砲)
	" 暗黒騎士 (騎鉄) ", // ←ロスローリアン(騎馬鉄砲)
};


char *IsButaiArrayIkkousyu[4] = {
	"一向薙刀",
	"騎馬",
	"一向鉄砲",
	"騎馬鉄砲"
};
char *IsButaiArrayIkkousyuYumi[4] = {
	"弓僧",
	"弓騎馬",
	"一向鉄砲",
	"騎馬鉄砲"
};

char *IsButaiArrayUesughi[4] = {
	"上杉薙刀",
	"騎馬",
	"鉄砲",
	"騎馬鉄砲"
};

char *IsButaiArrayUesughiYumi[4] = {
	"上杉弓僧",
	"弓騎馬",
	"鉄砲",
	"騎馬鉄砲"
};

char *IsButaiArrayShimazu[4] = {
	"島津刀侍",
	"騎馬",
	"鉄砲",
	"騎馬鉄砲"
};

char *IsButaiArrayDate[4] = {
	"伊達刀侍",
	"騎馬",
	"鉄砲",
	"伊達騎鉄"
};

char *IsButaiArrayDateYumi[4] = {
	"弓足軽",
	"弓騎馬",
	"鉄砲",
	"伊達騎鉄"
};

char *IsButaiArrayHattori[4] = {
	"伊賀忍者",
	"騎馬",
	"服部鉄砲",
	"騎馬鉄砲"
};

char *IsButaiArrayHattoriYumi[4] = {
	"伊賀忍者",
	"弓騎馬",
	"服部鉄砲",
	"騎馬鉄砲"
};
char *IsButaiArrayHuuma[4] = {
	"風魔忍者",
	"騎馬",
	"鉄砲",
	"騎馬鉄砲"
};

char *IsButaiArrayHuumaYumi[4] = {
	"風魔忍者",
	"弓騎馬",
	"鉄砲",
	"騎馬鉄砲"
};

char *IsButaiArrayMomochi[4] = {
	"伊賀忍者",
	"騎馬",
	"伊賀鉄砲",
	"騎馬鉄砲"
};

char *IsButaiArrayMomochiYumi[4] = {
	"伊賀忍者",
	"弓騎馬",
	"伊賀鉄砲",
	"騎馬鉄砲"
};

char *IsButaiArraySarutobi[4] = {
	"甲賀忍者",
	"騎馬",
	"鉄砲",
	"騎馬鉄砲"
};

char *IsButaiArraySarutobiYumi[4] = {
	"甲賀忍者",
	"弓騎馬",
	"鉄砲",
	"騎馬鉄砲"
};

char *IsButaiArrayKirigakure[4] = {
	"伊賀忍者",
	"騎馬",
	"伊賀鉄砲",
	"騎馬鉄砲"
};

char *IsButaiArrayKirigakureYumi[4] = {
	"伊賀忍者",
	"弓騎馬",
	"伊賀鉄砲",
	"騎馬鉄砲"
};

char *IsButaiArrayNinja[4] = {
	"忍者",
	"騎馬",
	"鉄砲",
	"騎馬鉄砲"
};


char *IsButaiArrayOda[4] = {
	"織田槍",
	"騎馬",
	"織田鉄砲",
	"騎馬鉄砲"
};

char *IsButaiArrayOdaYumi[4] = {
	"織田弓",
	"弓騎馬",
	"織田鉄砲",
	"騎馬鉄砲"
};

char *IsButaiArraySouryo[4] = {
	"薙刀僧",
	"騎馬",
	"騎馬",
	"騎馬鉄砲"
};

char *IsButaiArraySouryoYumi[4] = {
	"弓僧",
	"弓騎馬",
	"鉄砲",
	"騎馬鉄砲"
};

char *IsButaiArrayTakedaHigh[4] = {
	"郎党侍",
	"武田旗本",
	"鉄砲",
	"騎馬鉄砲"
};

char *IsButaiArrayTakedaYari[4] = {
	"足軽",
	"武田槍騎",
	"鉄砲",
	"騎馬鉄砲"
};

char *IsButaiArrayTakedaKatana[4] = {
	"足軽",
	"武田刀騎",
	"鉄砲",
	"騎馬鉄砲"
};

char *IsButaiArrayTakedaYumi[4] = {
	"弓足軽",
	"武田弓騎",
	"鉄砲",
	"騎馬鉄砲"
};

char *IsButaiArrayTakedaWoman[4] = {
	"女武士",
	"女騎馬",
	"鉄砲",
	"騎馬鉄砲"
};

char *IsButaiArrayTakedaWomanYumi[4] = {
	"女武士",
	"女弓騎馬",
	"鉄砲",
	"騎馬鉄砲"
};

char *IsButaiArrayHoujoTeppoGreat[4] = {
	"北条大筒",
	"北条大筒",
	"北条大筒",
	"北条大筒"
};

char *IsButaiArrayShiroKishi[4] = {
	"白騎士",
	"白騎馬",
	"鉄砲騎士",
	"騎鉄騎士",
};
char *IsButaiArrayMahou[4] = {
	"魔法",
	"魔法騎馬",
	"魔法鉄砲",
	"魔法騎鉄",
};
char *IsButaiArrayMajuu[4] = {
	"魔獣",
	"魔獣騎馬",
	"魔獣鉄砲",
	"魔獣騎鉄",
};


char *IsButaiArrayHighKatana[4] = {
	"刀侍",
	"旗本騎馬",
	"鉄砲侍",
	"騎馬鉄砲"
};

char *IsButaiArrayHighYari[4] = {
	"槍侍",
	"旗本槍騎",
	"鉄砲侍",
	"騎馬鉄砲"
};

char *IsButaiArrayHighYumi[4] = {
	"弓侍",
	"旗本弓騎",
	"鉄砲侍",
	"騎馬鉄砲"
};

char *IsButaiArrayKatanaHigh[4] = {
	"刀侍",
	"刀侍",
	"刀侍",
	"刀侍"
};

char *IsButaiArrayKatanaMid[4] = {
	"野太刀侍",
	"野太刀侍",
	"野太刀侍",
	"野太刀侍"
};

char *IsButaiArrayNaginagaHigh[4] = {
	"薙刀侍",
	"薙刀侍",
	"薙刀侍",
	"薙刀侍"
};

char *IsButaiArrayYariHigh[4] = {
	"槍侍",
	"槍侍",
	"槍侍",
	"槍侍"
};

char *IsButaiArrayYari[4] = {
	"槍足軽",
	"槍足軽",
	"槍足軽",
	"槍足軽"
};

char *IsButaiArrayKibaYariHigh[4] = {
	"長槍騎馬",
	"長槍騎馬",
	"長槍騎馬",
	"長槍騎馬"
};

char *IsButaiArrayKibaYumi[4] = {
	"弓騎馬",
	"弓騎馬",
	"弓騎馬",
	"弓騎馬"
};

char *IsButaiArrayYumiHigh[4] = {
	"弓侍",
	"弓侍",
	"弓侍",
	"弓侍"
};

char *IsButaiArrayYumi[4] = {
	"弓足軽",
	"弓足軽",
	"弓足軽",
	"弓足軽"
};

char *IsButaiArrayTeppoHigh[4] = {
	"鉄砲侍",
	"鉄砲侍",
	"鉄砲侍",
	"鉄砲侍"
};

char *IsButaiArrayTeppoGreat[4] = {
	"大筒",
	"大筒",
	"大筒",
	"大筒"
};

char *IsButaiArrayUmiueKurohune[4] = {
	"黒船",
	"黒船",
	"黒船",
	"黒船"
};
char *IsButaiArrayUmiueNihonmaru[4] = {
	"日本丸",
	"日本丸",
	"日本丸",
	"日本丸"
};
char *IsButaiArrayUmiueDaiAtake[4] = {
	"大安宅船",
	"大安宅船",
	"大安宅船",
	"大安宅船"
};
char *IsButaiArrayUmiueDaiAtakeWithDetail[4] = {
	" 大安宅船 (足軽) ",
	" 大安宅船 (騎馬) ",
	" 大安宅船 (鉄砲) ",
	" 大安宅船 (騎鉄) "
};

char *IsButaiArrayUmiueAtake[4] = {
	"安宅船",
	"安宅船",
	"安宅船",
	"安宅船"
};
char *IsButaiArrayUmiueAtakeWithDetail[4] = {
	" 安宅船 (足軽) ",
	" 安宅船 (騎馬) ",
	" 安宅船 (鉄砲) ",
	" 安宅船 (騎鉄) "
};
char *IsButaiArrayUmiueSenseki[4] = {
	"千石船",
	"千石船",
	"千石船",
	"千石船"
};
char *IsButaiArrayUmiueSensekiWithDetail[4] = {
	" 千石船 (足軽) ",
	" 千石船 (騎馬) ",
	" 千石船 (鉄砲) ",
	" 千石船 (騎鉄) "
};
char *IsButaiArrayUmiueSekibune[4] = {
	"関船",
	"関船",
	"関船",
	"関船"
};
char *IsButaiArrayUmiueSekibuneWithDetail[4] = {
	" 関船 (足軽) ",
	" 関船 (騎馬) ",
	" 関船 (鉄砲) ",
	" 関船 (騎鉄) "
};
char *IsButaiArrayUmiueIrou[4] = {
	"井楼船",
	"井楼船",
	"鉄砲小早",
	"鉄砲小早"
};
char *IsButaiArrayUmiueIrouWithDetail[4] = {
	" 井楼船 (足軽) ",
	" 井楼船 (騎馬) ",
	" 鉄砲小早 (鉄砲) ",
	" 鉄砲小早 (騎鉄) "
};
char *IsButaiArrayUmiueMini[4] = {
	"小早",
	"小早",
	"鉄砲小早",
	"鉄砲小早"
};
char *IsButaiArrayUmiueMiniWithDetail[4] = {
	" 小早 (足軽) ",
	" 小早 (騎馬) ",
	" 鉄砲小早 (鉄砲) ",
	" 鉄砲小早 (騎鉄) "
};
char *IsButaiArrayUmiueMiniYumi[4] = {
	"弓小早",
	"弓小早",
	"鉄砲小早",
	"鉄砲小早"
};
char *IsButaiArrayUmiueMiniYumiWithDetail[4] = {
	" 弓小早 (足軽) ",
	" 弓小早 (騎馬) ",
	" 鉄砲小早 (鉄砲) ",
	" 鉄砲小早 (騎鉄) "
};

// "\xec\x8c\xec\x8d\xec\x8e";//長宗我部
// "\xec\x8f\xec\x8d\xec\x8e";//香宗我部
char *IsButaiArrayChosokabeYumi[4] = {
	"\xec\x8c\xec\x8d\xec\x8e弓",
	"騎馬",
	"鉄砲",
	"騎馬鉄砲"
};


// NULLを返せば、変更しないということ。
int GetOverWriteButaiMeiArrayPointer(int iBushouID) {

	if ( !(0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) ) {
		return NULL;
	}

	// ScenarioModを導入している。
	if ( p_snOnViewingButaiName ) {
		int iSceanrioModValidButaiName = p_snOnViewingButaiName( iBushouID + 1 );

		// 有効な変更がされているのであれば、それを返す。
		if ( iSceanrioModValidButaiName ) {
			return iSceanrioModValidButaiName;
		}
	}


	// 今戦闘していて、海の上にいる
	HEX_POSITION p = GetFieldHexUnitPos( iBushouID );
	// 武将の位置の役割を得る
	int iHexRole = GetFieldHexRole( p );

	//------------------------------------------以下、大名家よりも優先する
	// タルタロスならば
	if ( isLancelotTartare( iBushouID ) ) {
		// 海の上
		if ( Is_FieldWar() && (iHexRole == FIELD_HEX_ROLE::海 || iHexRole == FIELD_HEX_ROLE::湖) ) {
			// 鉄甲船を持っている
			if (nb6bushouref[iBushouID].steelship) {
				return int(IsButaiArrayUmiueKurohune);
			}
		}
		// 海の上以外
		// 部隊編成モード
		if ( isOnEditingButaiMode ) {
			return int(IsButaiArrayLoslorienWithDetail);
		} else {
			return int(IsButaiArrayLoslorien);
		}
	}

	// 海の上
	if ( Is_FieldWar() && (iHexRole == FIELD_HEX_ROLE::海 || iHexRole == FIELD_HEX_ROLE::湖) ) {
		// 最大勢力の大名
		int iDaimyoID = nb6bushouref[iBushouID].attach -1;
		int iMaximumInfluenceDaimyo = GetMaximumInfluenceDaimyo();
		// 最大勢力の大名自身であり、野望が高い
		if ( nb6bushouname[iBushouID].position == 1 && (iDaimyoID == iMaximumInfluenceDaimyo) && nb6bushouref[iBushouID].ambition > 80 && nb6bushouref[iBushouID].steelship ) {
			return int(IsButaiArrayUmiueNihonmaru);
		}

		// 鉄甲船を持っている
		if (nb6bushouref[iBushouID].steelship) {
			return int(IsButaiArrayUmiueDaiAtake);
		}

		// 隠居=0, 大名=1, 宿老=2, 家老=3, 部将=4, 侍大将=5, 足軽頭=6
		if (nb6bushouname[iBushouID].position == 1 ) {
			return int(IsButaiArrayUmiueDaiAtake);
		}

		// 隠居=0, 大名=1, 宿老=2, 家老=3, 部将=4, 侍大将=5, 足軽頭=6
		if ( nb6bushouname[iBushouID].position == 2 ) {
			return int(IsButaiArrayUmiueAtake);
		}

		// 隠居=0, 大名=1, 宿老=2, 家老=3, 部将=4, 侍大将=5, 足軽頭=6
		if (nb6bushouname[iBushouID].position == 3 ) {
			return int(IsButaiArrayUmiueSenseki);
		}

		// 隠居=0, 大名=1, 宿老=2, 家老=3, 部将=4, 侍大将=5, 足軽頭=6
		if (nb6bushouname[iBushouID].position == 4 ) {
			return int(IsButaiArrayUmiueSekibune);
		}

		// 隠居=0, 大名=1, 宿老=2, 家老=3, 部将=4, 侍大将=5, 足軽頭=6
		if (nb6bushouname[iBushouID].position == 5 ) {
			return int(IsButaiArrayUmiueIrou);
		}

		// 隠居=0, 大名=1, 宿老=2, 家老=3, 部将=4, 侍大将=5, 足軽頭=6
		if (nb6bushouname[iBushouID].position == 6 ) {
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 ) {
				return int(IsButaiArrayUmiueMiniYumi);
			} else {
				return int(IsButaiArrayUmiueMini);
			}
		}
	}

	// ロスローリアンならば…
	if ( IsLoslorien( iBushouID ) ) {
		// 部隊編成モード
		if ( isOnEditingButaiMode ) {
			return int(IsButaiArrayLoslorienWithDetail);
		} else {
			return int(IsButaiArrayLoslorien);
		}
	}

	// ウォーレンならば…魔術師ではなく占星術師とする
	if ( strcmp(nb6bushouname[iBushouID].familyname, "�q�r")==0 && strcmp(nb6bushouname[iBushouID].fastname, "�n�o�p")==0 ) {
		return int(IsButaiArrayMahou);
	}
	// カノープスならば…魔人ではなく有翼人とする
	if ( strcmp(nb6bushouname[iBushouID].familyname, "�O�u")==0 && strcmp(nb6bushouname[iBushouID].fastname, "�[�\�]")==0 ) {
		return int(IsButaiArrayMajuu);
	}
	// ギルバルドであれば…騎士ではなく獣使いとする
	if ( strcmp(nb6bushouname[iBushouID].familyname, "�g�h�i")==0 && strcmp(nb6bushouname[iBushouID].fastname, "�懺晰�")==0 ) {
		return int(IsButaiArrayMajuu);
	}
	// ミルディンまらば、白騎士とする
	if ( strcmp(nb6bushouname[iBushouID].familyname, "�O�P�Q")==0 && strcmp(nb6bushouname[iBushouID].fastname, "�R�S�T")==0 ) {
		return int(IsButaiArrayShiroKishi);
	}
	// ギルダスならば…白騎士とする
	if	(strcmp(nb6bushouname[iBushouID].familyname, "�G�H�F")==0 && strcmp(nb6bushouname[iBushouID].fastname, "шщъ")==0) {
		return int(IsButaiArrayShiroKishi);
	}		
	// デネブならば…魔術師ではなく魔女とする
	if ( strcmp(nb6bushouname[iBushouID].familyname, "�j�k")==0 && strcmp(nb6bushouname[iBushouID].fastname, "�l�m")==0 ) {
		return int(IsButaiArrayMahou);
	}
	// ラドラムならば…魔術師ではなく魔導士とする
	if ( strcmp(nb6bushouname[iBushouID].familyname, "�y�z�{")==0 && strcmp(nb6bushouname[iBushouID].fastname, "�|�}�~")==0 ) {
		return int(IsButaiArrayMahou);
	}



	// 服部半蔵の場合、部隊名配列を変更する。
	if ( strcmp(nb6bushouname[iBushouID].familyname, "服部") == 0 && nb6bushouname[iBushouID].number == 108 ) {
		// 忍者の場合
		if ( nb6bushouref[iBushouID].job == 0x10 ) {
			// 弓が使える武将
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 0,2)) {
				return int(IsButaiArrayHattoriYumi);
			} 
			// 通常武将
			if ( IsBushouForm(iBushouID, 0,2) ) {
				return int(IsButaiArrayHattori);
			}
		}
	}
	// 風魔小太郎の場合、部隊名配列を変更する。
	if ( strcmp(nb6bushouname[iBushouID].familyname, "風魔") == 0 && nb6bushouname[iBushouID].number == 119 ) {
		// 忍者の場合
		if ( nb6bushouref[iBushouID].job == 0x10 ) {
			// 弓が使える武将
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 0)) {
				return int(IsButaiArrayHuumaYumi);
			}
			// 通常武将
			if ( IsBushouForm(iBushouID, 0) ) {
				return int(IsButaiArrayHuuma);
			}
		}
	}
	// 百地丹波の場合、部隊名配列を変更する。
	if ( strcmp(nb6bushouname[iBushouID].familyname, "百地") == 0 && nb6bushouname[iBushouID].number == 328 ) {
		// 忍者の場合
		if ( nb6bushouref[iBushouID].job == 0x10 ) {
			// 弓が使える武将
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 0,2) ) {
				return int(IsButaiArrayMomochiYumi);
			}
			// 通常武将
			if ( IsBushouForm(iBushouID, 0,2) ) {
				return int(IsButaiArrayMomochi);
			}
		}
	}

	// 猿飛佐助の場合、部隊名配列を変更する。
	if ( strcmp(nb6bushouname[iBushouID].familyname, "猿飛") == 0 && nb6bushouname[iBushouID].number == 361 ) {
		// 忍者の場合
		if ( nb6bushouref[iBushouID].job == 0x10 ) {
			// 弓が使える武将
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 0) ) {
				return int(IsButaiArraySarutobiYumi);
			}
			// 通常武将
			if ( IsBushouForm(iBushouID, 0) ) {
				return int(IsButaiArraySarutobi);
			}
		}
	}

	// 霧隠才蔵の場合、部隊名配列を変更する。
	if ( strcmp(nb6bushouname[iBushouID].familyname, "霧隠") == 0 && nb6bushouname[iBushouID].number == 362 ) {
		// 忍者の場合
		if ( nb6bushouref[iBushouID].job == 0x10 ) {
			// 弓が使える武将
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 0) ) {
				return int(IsButaiArrayKirigakureYumi);
			}
			// 通常武将
			if ( IsBushouForm(iBushouID, 0) ) {
				return int(IsButaiArrayKirigakure);
			}
		}
	}

	// 忍者の場合
	if ( nb6bushouref[iBushouID].job == 0x10 ) {
		if ( IsBushouForm(iBushouID, 0) ) {
			return int(IsButaiArrayNinja);
		}
	}

	// 大名の血筋で分けていく
	int iDaimyoID = nb6bushouref[iBushouID].attach -1;
	int iGundanID = nb6bushouref[iBushouID].gundan -1;

	if ( (0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM) && (0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM) ) {
		int iDaimyoBushouID = nb6daimyo[iDaimyoID].attach - 1;
		int iGundanBushouID = nb6gundan[iGundanID].leader - 1;

		// 本願寺家の場合、部隊名配列を変更する。
		if ( nb6bushouref[iDaimyoBushouID].blood == 41 ) {
			// 弓が使える武将
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 2) ) {
				return int(IsButaiArrayIkkousyuYumi);
			}
			// 通常武将
			if ( IsBushouForm(iBushouID, 0, 2) ) {
				return int(IsButaiArrayIkkousyu);
			}

		} 

		// 苗字が上杉で、上杉謙信もしくは、上杉景勝の場合、部隊名配列を変更する。
		if ( strcmp(nb6bushouname[iDaimyoBushouID].familyname, "上杉")==0 && (nb6bushouname[iDaimyoBushouID].number == 26 || nb6bushouname[iDaimyoBushouID].number == 27) ) {

			// 弓が使える武将
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 0) ) {
				return int(IsButaiArrayUesughiYumi);
			}
			// 通常武将
			if ( IsBushouForm(iBushouID, 0) ) {
				return int(IsButaiArrayUesughi);
			}
		}

		// 島津家の場合、部隊名配列を変更する。
		if ( nb6bushouref[iDaimyoBushouID].blood == 6 ) {
			// 通常武将
			if ( IsBushouForm(iBushouID, 0) && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi < 3 ) {
				return int(IsButaiArrayShimazu);
			}
		}

		// 長宗我部の場合、部隊名配列を変更する。
		if ( nb6bushouref[iDaimyoBushouID].blood == 11 ) {
			// 弓が使える武将
			if ( IsBushouForm(iBushouID, 0) && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 ) {
				return int(IsButaiArrayChosokabeYumi);
			}
		}

		// 伊達家の場合、部隊名配列を変更する。
		if ( nb6bushouref[iDaimyoBushouID].blood == 7 ) {
			// 弓が使える武将
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 3) ) {
				return int(IsButaiArrayDateYumi);
			}
			// 刀武将
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari <= 2 && IsBushouForm(iBushouID, 0, 3) ) {
				return int(IsButaiArrayDate);
			}

		}

		// 大名が織田信長の場合、部隊名配列を変更する。
		if ( strcmp(nb6bushouname[iDaimyoBushouID].familyname, "織田")==0 && nb6bushouname[iDaimyoBushouID].number == 0 ) {
			// 弓が使える武将
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 0, 2) ) {
				return int(IsButaiArrayOdaYumi);
			}
			// 槍系
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari >= 3 && IsBushouForm(iBushouID, 0, 2) ) {
				return int(IsButaiArrayOda);
			}
			// 通常
			if ( IsBushouForm(iBushouID, 2) ) {
				return int(IsButaiArrayOda);
			}
		}

		// 大名が武田家の場合、部隊名配列を変更する。
		if ( nb6bushouref[iDaimyoBushouID].blood == 1 ) {
			// 戦闘が150以上になったら、優秀な武将であれば、第一軍団直下
			if ( getBushouBat(iBushouID) > 75 && (iDaimyoBushouID==iGundanBushouID) && nb6bushouname[iBushouID].State != 0 && (nb6bushouref[iBushouID].maxbat + nb6bushouref[iBushouID].maxgov + nb6bushouref[iBushouID].maxint) >= 200 ) {
				if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi <= 2 && IsBushouForm(iBushouID, 0) ) {
					return int(IsButaiArrayTakedaHigh);
				}
				if ( IsBushouForm(iBushouID, 1) ) {
					return int(IsButaiArrayTakedaHigh);
				}
			}
			// 弓が使える武将
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 ) {
				if ( nb6bushouname[iBushouID].sex == 0 && IsBushouForm(iBushouID, 1) ) {
					return int(IsButaiArrayTakedaYumi);
				}
				if ( nb6bushouname[iBushouID].sex == 1 && IsBushouForm(iBushouID, 0, 1) ) {
					return int(IsButaiArrayTakedaWomanYumi);
				}
				// 通常武将
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

		// 北条家の場合、部隊名配列を変更する。
		if ( nb6bushouref[iDaimyoBushouID].blood == 8 ) {
			// 鉄砲が使える武将
			if ( IsBushouForm(iBushouID, 2) ) {
				// 戦闘が170以上になったら、騎馬適正がＡ以上
				if ( getBushouBat(iBushouID) > 85 && getBushouAptitude(iBushouID, APTITUDE_TEPPOU) >= 4) {
					return int(IsButaiArrayHoujoTeppoGreat);
				}
			}
		}


		//------------------------------------------以下、大名家よりも優先度は劣る普通のもの
		// 僧侶の場合、部隊名配列を変更する。
		// 僧侶の場合
		if ( nb6bushouref[iBushouID].job == 0x40 ) {
			// 弓が使える武将
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 0) ) {
				return int(IsButaiArraySouryoYumi);
			}
			if ( IsBushouForm(iBushouID, 0) ) {
				return int(IsButaiArraySouryo);
			}

		}

		// 海系であれば…
		if ( nb6bushouref[iBushouID].job==0x50 ) { // 海賊か水軍適正Ｂ以上

			// 鉄甲船を持っている
			if (nb6bushouref[iBushouID].steelship) {
				// 部隊編集モード
				if ( isOnEditingButaiMode ) {
					return int(IsButaiArrayUmiueDaiAtakeWithDetail);
				} else {
					return int(IsButaiArrayUmiueDaiAtake);
				}
			}

			// 隠居=0, 大名=1, 宿老=2, 家老=3, 部将=4, 侍大将=5, 足軽頭=6
			if (nb6bushouname[iBushouID].position == 1 ) {
				// 部隊編集モード
				if ( isOnEditingButaiMode ) {
					return int(IsButaiArrayUmiueDaiAtakeWithDetail);
				} else {
					return int(IsButaiArrayUmiueDaiAtake);
				}
			}

			// 隠居=0, 大名=1, 宿老=2, 家老=3, 部将=4, 侍大将=5, 足軽頭=6
			if ( nb6bushouname[iBushouID].position == 2 ) {
				// 部隊編集モード
				if ( isOnEditingButaiMode ) {
					return int(IsButaiArrayUmiueAtakeWithDetail);
				} else {
					return int(IsButaiArrayUmiueAtake);
				}
			}

			// 隠居=0, 大名=1, 宿老=2, 家老=3, 部将=4, 侍大将=5, 足軽頭=6
			if (nb6bushouname[iBushouID].position == 3 ) {
				// 部隊編集モード
				if ( isOnEditingButaiMode ) {
					return int(IsButaiArrayUmiueSensekiWithDetail);
				} else {
					return int(IsButaiArrayUmiueSenseki);
				}
			}

			// 隠居=0, 大名=1, 宿老=2, 家老=3, 部将=4, 侍大将=5, 足軽頭=6
			if (nb6bushouname[iBushouID].position == 4 ) {
				// 部隊編集モード
				if ( isOnEditingButaiMode ) {
					return int(IsButaiArrayUmiueSekibuneWithDetail);
				} else {
					return int(IsButaiArrayUmiueSekibune);
				}
			}

			// 隠居=0, 大名=1, 宿老=2, 家老=3, 部将=4, 侍大将=5, 足軽頭=6
			if (nb6bushouname[iBushouID].position == 5 ) {
				// 部隊編集モード
				if ( isOnEditingButaiMode ) {
					return int(IsButaiArrayUmiueIrouWithDetail);
				} else {
					return int(IsButaiArrayUmiueIrou);
				}
			}

			// 隠居=0, 大名=1, 宿老=2, 家老=3, 部将=4, 侍大将=5, 足軽頭=6
			if (nb6bushouname[iBushouID].position == 6 ) {
				if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 ) {
					// 部隊編集モード
					if ( isOnEditingButaiMode ) {
						return int(IsButaiArrayUmiueMiniYumiWithDetail);
					} else {
						return int(IsButaiArrayUmiueMiniYumi);
					}
				} else {
					// 部隊編集モード
					if ( isOnEditingButaiMode ) {
						return int(IsButaiArrayUmiueMiniWithDetail);
					} else {
						return int(IsButaiArrayUmiueMini);
					}
				}
			}
		}

		// 戦闘が150以上になったら、優秀な武将であれば、第一軍団直下
		if ( getBushouBat(iBushouID) > 75 && (iDaimyoBushouID==iGundanBushouID) && nb6bushouname[iBushouID].State != 0 && (nb6bushouref[iBushouID].maxbat + nb6bushouref[iBushouID].maxgov + nb6bushouref[iBushouID].maxint) >= 200 ) {
			// 弓が使える武将
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 1) ) {
				return int(IsButaiArrayHighYumi);
			}
			// 槍が使える武将
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari >= 3 && IsBushouForm(iBushouID, 1) ) {
				return int(IsButaiArrayHighYari);
			}
			if ( IsBushouForm(iBushouID, 1) ) {
				return int(IsButaiArrayHighKatana);
			}
		}

		// 鉄砲の武将
		if ( IsBushouForm(iBushouID, 2) ) {

			// 戦闘が170以上になったら、騎馬適正がＡ以上
			if ( getBushouBat(iBushouID) > 85 && getBushouAptitude(iBushouID, APTITUDE_TEPPOU) >= 4) {
				return int(IsButaiArrayTeppoGreat);
			}
			// 戦闘が150以上になったら、騎馬適正がＢ以上
			if ( getBushouBat(iBushouID) > 75 && getBushouAptitude(iBushouID, APTITUDE_TEPPOU) >= 3) {
				return int(IsButaiArrayTeppoHigh);
			}
		}

		// 弓の武将
		if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 0) ) {
			// 戦闘が150以上になったら、足軽適正がＢ以上
			if ( getBushouBat(iBushouID) > 75 && getBushouAptitude(iBushouID, APTITUDE_ASIGARU) >= 3) {
				return int(IsButaiArrayYumiHigh);
			}
			return int(IsButaiArrayYumi);
		}

		// 弓の騎馬武将
		if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 1) ) {
			return int(IsButaiArrayKibaYumi);
		}

		// 槍の騎馬武将
		if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari > 3 && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi <= 2 && IsBushouForm(iBushouID, 1) ) {

			// 戦闘が150以上になったら、騎馬適正がＢ以上
			if ( getBushouBat(iBushouID) > 75 && getBushouAptitude(iBushouID, APTITUDE_KIBA) >= 3) {
				return int(IsButaiArrayKibaYariHigh);
			}
		}

		// 槍の足軽武将
		if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari >= 3 && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi <= 2 && IsBushouForm(iBushouID, 0) ) {

			// 戦闘が150以上になったら、足軽適正がＢ以上
			if ( getBushouBat(iBushouID) > 75 && getBushouAptitude(iBushouID, APTITUDE_ASIGARU) >= 3) {
				return int(IsButaiArrayYariHigh);
			}
			// 足軽適正がＣ以上
			if ( getBushouAptitude(iBushouID, APTITUDE_ASIGARU) >= 2) {
				return int(IsButaiArrayYari);
			}
		}

		// 刀の足軽武将(一番特徴がない)
		if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari <= 2 && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi <= 2 && IsBushouForm(iBushouID, 0) ) {

			// 戦闘が150以上になったら、足軽適正がＢ以上
			if ( getBushouBat(iBushouID) > 75 && getBushouAptitude(iBushouID, APTITUDE_ASIGARU) >= 3) {
				return int(IsButaiArrayKatanaHigh);
			}
			// 戦闘が120以上になったら、足軽適正がＣ以上
			if ( getBushouBat(iBushouID) > 60 && getBushouAptitude(iBushouID, APTITUDE_ASIGARU ) >= 2) {
				return int(IsButaiArrayKatanaMid);
			}

		}


	}


	return NULL;
}