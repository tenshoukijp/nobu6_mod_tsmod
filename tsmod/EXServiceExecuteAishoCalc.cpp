#include "WinTarget.h"

#include "EXBushou.h"
#include "EXBushou_All.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"

#include <fstream>

using namespace std;

// 相性によって忠誠度を上下させる。
void EXSuperService_AishoCalc() {

	// 「上長との相性の良し悪しが、毎季節、忠誠に影響する」を有効にしていなければ、何もしない。
	if ( TSModCommand::AllBushou::cmdAffinityWithSuperiorInfluenceLoyalty == 0 ) { return; }

	// 全ての武将をなめて、
	for ( int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

		// ｢現役(一般武将)｣でも｢軍団長｣でなければパス
		if ( nb6bushouname[iBushouID].State != 2 && nb6bushouname[iBushouID].State != 1 ) { continue; }

		// 自分のリーダーとは。
		int iLeaderID = 0;

		// ｢軍団長｣の場合
		if ( nb6bushouname[iBushouID].State == 1 ) { 


			// 所属の大名番号を求める
			int iDaimyoID = nb6bushouref[iBushouID].attach-1;

			// 大名番号から大名IDを求める
			iLeaderID = nb6daimyo[iDaimyoID].attach-1;

			// 有効な武将IDでなければ次
			if ( !(0 <= iLeaderID && iLeaderID < GAMEDATASTRUCT_BUSHOU_NUM) ) {
				continue;
			}
		}

		// ｢現役(=一般武将)｣の場合
		if ( nb6bushouname[iBushouID].State == 2 ) {  

			// 該当の武将の軍団IDを得る
			int iGundanID = nb6bushouref[iBushouID].gundan-1;

			// 有効な軍団IDでなければ次
			if ( !(0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM) ) {
				continue;
			}

			// 軍団長の武将IDを求める
			iLeaderID = nb6gundan[iGundanID].leader-1;

			// 有効な武将IDでなければ次
			if ( !(0 <= iLeaderID && iLeaderID < GAMEDATASTRUCT_BUSHOU_NUM) ) {
				continue;
			}
		}

		// 2人の相性の差分を求める
		int iDiffer = getBushouAishoDiff( iBushouID, iLeaderID );

		// 2人の相性が大きいほど、マイナスする
		iDiffer = -iDiffer;

		// 最高で+4, 最悪で-4となるように調整する。
		iDiffer += 4;

		// 加算が0ならば何もしなくても良い
		if ( iDiffer == 0 ) { continue; }

		// それ以外なら計算する。
		int cLoyal = nb6bushouref[iBushouID].loyal + iDiffer;

		// 100を超えていたら100とする。
		if ( cLoyal > 100 ) { cLoyal = 100; }
		// 0未満なら0とする。
		if ( cLoyal <  0 )  { cLoyal =   0; }

		// それを武将にアップデート
		nb6bushouref[iBushouID].loyal = (char)cLoyal;
	}
}