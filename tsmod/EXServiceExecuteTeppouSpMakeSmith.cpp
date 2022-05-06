#include "WinTarget.h"
#include <fstream>
#include <time.h>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "FuncMessageDialog.h"
#include "FuncPlayResourceWav.h"
#include "FuncMoveCamera.h"


using namespace std;

char szTeppouSpMakeSmithEventMsg[100] = "";
void EXServiceExecute_TeppouSpMakeSmith() {

	// ロード直後は発動しない
	if ( nb6progressseason <= 1 ) { return; }

	// 鉄砲伝来していなければ、何もしない
	if ( !isTeppouDenrai() ) { return; }

	// ある特定の城に武将が何人いるか？
	int iBushouNumInCastle[GAMEDATASTRUCT_CASTLE_NUM] = {0L};
	// 全ての武将をなめて、その武将が、鉄砲が得意であれば、どの城にいるかをカウントする。
	for ( int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

		// 浪人ではなく、対象の武将が鉄砲Ａ以上であれば…
		if (nb6bushouname[iBushouID].State <=2 && getBushouAptitude(iBushouID, APTITUDE_TEPPOU) >= 4 && nb6bushouref[iBushouID].castle <= GAMEDATASTRUCT_CASTLE_NUM) {
			// 対象の城にいる海賊人数カウントを１つ増やす
			iBushouNumInCastle[(nb6bushouref[iBushouID].castle-1)]++;
		}
	}

	// 城をなめていって、対象の城に鍛冶村が無い場合、家事村を作る
	for ( int iCastleID=0; iCastleID<GAMEDATASTRUCT_CASTLE_NUM; iCastleID++ ) {

		// 家事村がない、そして商業値が120以上に達成している。
		if ( !nb6castle[iCastleID].bSmith &&
			 nb6castle[iCastleID].town >= 120 ) {

		    // 鉄砲得意な人がその城に2人以上いた場合  確率2/20, 5人なら確率5/20というように･･･
			if ( (iBushouNumInCastle[iCastleID] >= 2) && (rand() % 20 < iBushouNumInCastle[iCastleID]) ) {

				nb6castle[iCastleID].bSmith = 1; // 鍛冶村を1とする。

				// 対象の城のなかで、最も勲功が高い人を見つける
				int iBushouIDOfHightTeppou = -1;
				// 全ての武将をなめて、
				for ( int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

					// 浪人ではなく、対象の武将が鉄砲が得意、現在ターゲットとなっている城にいる。
					if (nb6bushouname[iBushouID].State <= 2 && getBushouAptitude(iBushouID, APTITUDE_TEPPOU) >= 4  && nb6bushouref[iBushouID].castle-1==iCastleID) {

						// 未代入ならそのまま入れる
						if ( iBushouIDOfHightTeppou == -1 ) {
							iBushouIDOfHightTeppou = iBushouID;
						
						// より勲功が高い人が出てくればその人に変更する(身分上げてないかもしれないけれど、まぁいいかな)
						} else if (nb6bushouref[iBushouIDOfHightTeppou].merits < nb6bushouref[iBushouID].merits ) {
							iBushouIDOfHightTeppou = iBushouID;
						}
					}
				}

				// カメラをその城に移動
				FuncMoveCamera( iCastleID );

				// 音を鳴らす
				FuncPlaySoundWav(602);

				// ゲーム内の汎用情報ダイアログで情報を出す				
				wsprintf(szTeppouSpMakeSmithEventMsg, "%s%sらの働きにより、\x0A%s%sの近郊に、鍛冶村が出来ました。", nb6bushouname[iBushouIDOfHightTeppou].familyname, nb6bushouname[iBushouIDOfHightTeppou].fastname,  nb6castle[iCastleID].name, getCastleTypeName(nb6castle[iCastleID].title) );

				FuncConfirmDialog( szTeppouSpMakeSmithEventMsg );
			}
		}
	}
}