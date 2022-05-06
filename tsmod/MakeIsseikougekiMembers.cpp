#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


vector<int> vListIsseiKougekiMembers;
void MakeIsseikougekiMembers(int iBushouID, int iTargetID) {
	// メンバのクリア
	vListIsseiKougekiMembers.clear();

	if ( !(0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) ) {
		return;
	}
	if ( !(0 <= iTargetID && iTargetID < GAMEDATASTRUCT_BUSHOU_NUM) ) {
		return;
	}

	// 野戦中
	if ( Is_FieldWar() ) {
		// 現在のマップに出陣している武将リスト全員
		vector<int> blist = GetCurMapSyutujinBushouArray();

		// 相手の武将のPOSITIONを得る
		HEX_POSITION posTarget = GetFieldHexUnitPos(iTargetID);

		// 該当の武将の戦場における立場を返す(不明=0, 直接攻める側=1, 直接守る側=2, 攻める側の援軍=3, 守る側の援軍=4, 中立軍=5)
		int iBushouIDTachiba = GetWarStandPointTheBushou(iBushouID);


		// リストで… 一斉攻撃してくれる人を探す。
		for ( vector<int>::iterator it = blist.begin(); it != blist.end(); it++ ) {

			int iSearchID = *it;

			// 自分自身はパス
			if ( iBushouID == iSearchID ) { continue; }

			// サーチ対象の武将の位置
			HEX_POSITION posSearch = GetFieldHexUnitPos(iSearchID);

			// サーチ対象の武将の立場
			int iSearchIDTachiba = GetWarStandPointTheBushou(iSearchID);

			// ヘックス間隔が１の武将。即ち隣接している武将
			if ( getHexDistance( posTarget, posSearch ) == 1 ) {

				// 攻める側もしくは、攻める側の援軍である(これが一斉攻撃で巻き添え可能な人)
				if ( (iBushouIDTachiba == 1 || iBushouIDTachiba == 3) && (iSearchIDTachiba == 1 || iSearchIDTachiba== 3) ) {
					vListIsseiKougekiMembers.push_back(iSearchID);
				}
				// 守る側もしくは、守る側の援軍である(これが一斉攻撃で巻き添え可能な人)
				else if ( (iBushouIDTachiba == 2 || iBushouIDTachiba == 4) && (iSearchIDTachiba == 2 || iSearchIDTachiba== 4) ) {
					vListIsseiKougekiMembers.push_back(iSearchID);
				}
			}
		}
	}

	// 攻城戦ちゅう
	if ( Is_CastleWar() ) {
		// 現在のマップに出陣している武将リスト全員
		vector<int> blist = GetCurMapSyutujinBushouArray();

		// 相手の武将のPOSITIONを得る
		HEX_POSITION posTarget = GetCastleHexUnitPos(iTargetID);

		// 該当の武将の戦場における立場を返す(不明=0, 直接攻める側=1, 直接守る側=2, 攻める側の援軍=3, 守る側の援軍=4, 中立軍=5)
		int iBushouIDTachiba = GetWarStandPointTheBushou(iBushouID);


		// リストで… 一斉攻撃してくれる人を探す。
		for ( vector<int>::iterator it = blist.begin(); it != blist.end(); it++ ) {

			int iSearchID = *it;

			// 自分自身はパス
			if ( iBushouID == iSearchID ) { continue; }

			// サーチ対象の武将の位置
			HEX_POSITION posSearch = GetCastleHexUnitPos(iSearchID);

			// サーチ対象の武将の立場
			int iSearchIDTachiba = GetWarStandPointTheBushou(iSearchID);

			// ヘックス間隔が１の武将。即ち隣接している武将
			if ( getHexDistance( posTarget, posSearch ) == 1 ) {

				// 攻める側もしくは、攻める側の援軍である(これが一斉攻撃で巻き添え可能な人)
				if ( (iBushouIDTachiba == 1 || iBushouIDTachiba == 3) && (iSearchIDTachiba == 1 || iSearchIDTachiba== 3) ) {
					vListIsseiKougekiMembers.push_back(iSearchID);
				}
				// 守る側もしくは、守る側の援軍である(これが一斉攻撃で巻き添え可能な人)
				else if ( (iBushouIDTachiba == 2 || iBushouIDTachiba == 4) && (iSearchIDTachiba == 2 || iSearchIDTachiba== 4) ) {
					vListIsseiKougekiMembers.push_back(iSearchID);
				}
			}
		}
	}


}