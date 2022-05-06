#include "WinTarget.h"
#include <fstream>
#include <time.h>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "FuncMessageDialog.h"
#include "FuncPlayResourceWav.h"
#include "FuncMoveCamera.h"



using namespace std;

char szPirateMakePortEventMsg[100] = "";
void EXServiceExecute_PirateMakePort() {

	// ロード直後は発動しない
	if ( nb6progressseason <= 1 ) { return; }

	// ある特定の城に武将が何人いるか？
	int iBushouNumInCastle[GAMEDATASTRUCT_CASTLE_NUM] = {0L};
	// 全ての武将をなめて、その武将が、海賊であれば、どの城にいるかをカウントする。
	for ( int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {
		// 対象の武将が海賊であれば… かつ、どこかの城にちゃんと所属していれば…
		if (nb6bushouname[iBushouID].State <=2 && nb6bushouref[iBushouID].job == 0x50 && nb6bushouref[iBushouID].castle <= GAMEDATASTRUCT_CASTLE_NUM) {
			// 対象の城にいる海賊人数カウントを１つ増やす
			iBushouNumInCastle[(nb6bushouref[iBushouID].castle-1)]++;
		}
	}

	// 城をなめていって、対象の城が既に港があって、国際港がない場合、国際港を作る
	for ( int iCastleID=0; iCastleID<GAMEDATASTRUCT_CASTLE_NUM; iCastleID++ ) {

		// 港はある、しかし、国際港がない、そして商業値が150以上に達成している。
		if ( nb6castle[iCastleID].bPort  &&
			!nb6castle[iCastleID].blPort &&
			 nb6castle[iCastleID].town >= 150 ) {

		    // 海賊がその城に2人以上いた場合  確率2/20, 5人なら確率5/20というように･･･
			if ( (iBushouNumInCastle[iCastleID] >= 2) && (rand() % 20 < iBushouNumInCastle[iCastleID]) ) {

				// 国際港を１とする。
				nb6castle[iCastleID].blPort = 1;

				// 対象城の最大商業値を増やす。最大商業値を改造している人もいるかもしれないので慎重な増加
				byte iTownMax = nb6castlemax[iCastleID].maxtown;
				// 200より小さいならば増やす
				if (iTownMax < 200) {
					iTownMax += 30;
				}
				// 200を超えたならば、200とする。
				if (iTownMax > nb6parammax_main.town && nb6parammax_main.town <= 255 ) {
					iTownMax = (byte)nb6parammax_main.town;
				}	
				// 最大商業値のアドレス
				nb6castlemax[iCastleID].maxtown = iTownMax;


				// 対象の城のなかで、最も勲功が高い人を見つける
				int iBushouIDOfHightSuigun = -1;
				// 全ての武将をなめて、
				for ( int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {
					// 対象の武将が海賊であり、現在ターゲットとなっている城にいる。
					if (nb6bushouname[iBushouID].State <= 2 && nb6bushouref[iBushouID].job == 0x50 && nb6bushouref[iBushouID].castle-1==iCastleID) {
						// 未代入ならそのまま入れる
						if ( iBushouIDOfHightSuigun == -1 ) {
							iBushouIDOfHightSuigun = iBushouID;
						
						// より勲功が高い人が出てくればその人に変更する(身分上げてないかもしれないけれど、まぁいいかな)
						} else if (nb6bushouref[iBushouIDOfHightSuigun].merits < nb6bushouref[iBushouID].merits ) {
							iBushouIDOfHightSuigun = iBushouID;
						}
					}
				}

				// カメラをその城に移動
				FuncMoveCamera( iCastleID );

				// 音を鳴らす
				FuncPlaySoundWav(602);

				// ゲーム内の汎用情報ダイアログで情報を出す				
				wsprintf(szPirateMakePortEventMsg, "%s%sらが率いる水軍衆の働きにより、\x0A%s%s近郊の港が、国際港に発展しました。", nb6bushouname[iBushouIDOfHightSuigun].familyname, nb6bushouname[iBushouIDOfHightSuigun].fastname,  nb6castle[iCastleID].name, getCastleTypeName(nb6castle[iCastleID].title) );
				FuncConfirmDialog( szPirateMakePortEventMsg );
			}
		}
	}
}