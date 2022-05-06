#include "WinTarget.h"
#include <fstream>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"



/*
 * TSMod.txtで指定すると、すべての武将の寿命を特定の値に出来る。登場武将も同様。
 */
void EXSuperService_IfLifesIsTheTimes() {

	// まだメインゲームまわってなければ、実行しない
	if ( !CommonGlobalHandle::isMainGameStart ) {
		return;
	}


	// 歳を取るのが遅くなるを倍数化させるコマンドが２倍以上で有効。１以下では無効。
	if ( TSModCommand::AllBushou::cmdLifeIsTheTimes <= 1 ) {
		return;
	}

	// 春以外は無効(くすのがはやすぎて、まだ季節情報が１つ前の冬になっている
	if ( nb6year.season != 3 ) {
		return;
	}

	for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

		// 数倍としてした数でyearを割った時、0でなければ、歳を取らない。即ち数倍というのが多いほど、歳を取らない
		// 歳を取らないためには、｢誕生年｣を１つ増やさなければならない。
		if ( (nb6year.year % TSModCommand::AllBushou::cmdLifeIsTheTimes) != 0 ) {

			// 誕生年を１つ。255だと増やさない(0になってまう)
			if ( nb6bushouref[iBushouID].birth <= 254 ) {

				int old = getYearsOld(iBushouID);

				// 姫ならば12才の時は処理しない【姫が登場して姫になるか武将になるかの判定イベントがある、これが毎年繰り返さないようにする】
				if (nb6bushouname[iBushouID].State == 5 && old == 12) {
					;

				// それ意外は生まれた年をずらしていくことで、歳を結果として取らなくする。
				} else {
					int birth = nb6bushouref[iBushouID].birth;
					birth++;
					nb6bushouref[iBushouID].birth = birth;
				}
			}
		}
	}
}
