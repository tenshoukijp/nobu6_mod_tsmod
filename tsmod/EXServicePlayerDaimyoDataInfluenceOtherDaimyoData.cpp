#include "CommonGlobalHandle.h"
#include "EXServiceExecute.h"
#include "GameDataStruct.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "TSModCommand.h"
#include "TSMod.h"





// 毎シーズン、プレイヤのゲーム進捗に引き寄せられる処理
void EXSuperService_PlayerDaimyoDataInfluenceOtherDaimyoData() {

	// プレイヤの進度に引き寄せられる
	if ( TSModCommand::OthersGundan::cmdPlayerAndTheWholeInfluenceEachOtherData > 0 ) {

		int lvl = TSModCommand::OthersGundan::cmdPlayerAndTheWholeInfluenceEachOtherData;

		int iPlayerGundanCnt = 0;
		int iPlayerGundanGold = 0;
		int iPlayerGundanRise = 0;
		int iPlayerGundanTeppou = 0;
		int iPlayerGundanKiba = 0;
		int iPlayerGundanSoldier = 0;

		int iOtherGundanCnt = 0;
		int iOtherGundanGold = 0;
		int iOtherGundanRise = 0;
		int iOtherGundanTeppou = 0;
		int iOtherGundanKiba = 0;
		int iOtherGundanSoldier = 0;

		int iGundanTeppou[GAMEDATASTRUCT_GUNDAN_NUM] = {};
		int iGundanKiba[GAMEDATASTRUCT_GUNDAN_NUM] = {};

		// プレイヤ勢力の金と米を見る
		for (int iGundanNum=0; iGundanNum < GAMEDATASTRUCT_GUNDAN_NUM; iGundanNum++) {
			// 居城名ナンバーが0xffffだとその軍団は存在していない。次へ。
			if (nb6gundan[iGundanNum].residence == 0xFFFF ) {
				continue;
			}
			// 対象がプレイヤーの軍団であれば金・米・鉄砲・騎馬をカウントする。
			if ( isPlayerGundan(iGundanNum) ) {
				iPlayerGundanCnt++;
				iPlayerGundanGold += nb6gundan[iGundanNum].money;
				iPlayerGundanRise += nb6gundan[iGundanNum].rise;
				iPlayerGundanTeppou += nb6gundan[iGundanNum].gun;
				iPlayerGundanKiba += nb6gundan[iGundanNum].horse;
			// 対象がプレイヤー以外の軍団であれば金・米・鉄砲・騎馬をカウントする。
			} else {
				iOtherGundanCnt++;
				iOtherGundanGold += nb6gundan[iGundanNum].money;
				iOtherGundanRise += nb6gundan[iGundanNum].rise;
				iOtherGundanTeppou += nb6gundan[iGundanNum].gun;
				iOtherGundanKiba += nb6gundan[iGundanNum].horse;
			}
		}

		for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {
			int iGundanNum = nb6bushouref[iBushouID].gundan-1;

			// 対象武将がプレイヤーの軍団であれば武将にぶら下がってる兵士の分から、兵数・金・米・鉄砲・騎馬をカウントする。
			if ( isPlayerGundan(iGundanNum) ) {
				// 兵数をプラス
				iPlayerGundanSoldier += nb6bushouref[iBushouID].soldier;
				// もし騎馬隊ならば、騎馬をプラス
				if ( nb6bushouref[iBushouID].form == 1 ) {
					iPlayerGundanKiba += nb6bushouref[iBushouID].soldier;
				// もし鉄砲隊ならば、鉄砲をプラス
				} else if ( nb6bushouref[iBushouID].form == 2 ) {
					iPlayerGundanTeppou += nb6bushouref[iBushouID].soldier;
				// もし騎馬鉄砲隊ならば、鉄砲と騎馬をプラス
				} else if ( nb6bushouref[iBushouID].form == 3 ) {
					iPlayerGundanKiba += nb6bushouref[iBushouID].soldier;
					iPlayerGundanTeppou += nb6bushouref[iBushouID].soldier;
				}
			} else {
				// 兵数をプラス
				iOtherGundanSoldier += nb6bushouref[iBushouID].soldier;
				// もし騎馬隊ならば、騎馬をプラス
				if ( nb6bushouref[iBushouID].form == 1 ) {
					iOtherGundanKiba += nb6bushouref[iBushouID].soldier;
				// もし鉄砲隊ならば、鉄砲をプラス
				} else if ( nb6bushouref[iBushouID].form == 2 ) {
					iOtherGundanTeppou += nb6bushouref[iBushouID].soldier;
				// もし騎馬鉄砲隊ならば、鉄砲と騎馬をプラス
				} else if ( nb6bushouref[iBushouID].form == 3 ) {
					iOtherGundanKiba += nb6bushouref[iBushouID].soldier;
					iOtherGundanTeppou += nb6bushouref[iBushouID].soldier;

					iGundanKiba[iGundanNum] += nb6bushouref[iBushouID].soldier;
					iGundanTeppou[iGundanNum] += nb6bushouref[iBushouID].soldier;
				}
			}
		}	

		// プレイヤ勢力が存在している場合。
		if ( iPlayerGundanCnt > 0 ) {
			iPlayerGundanGold /= iPlayerGundanCnt;
			iPlayerGundanRise /= iPlayerGundanCnt;
			iPlayerGundanTeppou /= iPlayerGundanCnt;
			iPlayerGundanKiba /= iPlayerGundanCnt;
			iPlayerGundanSoldier /= iPlayerGundanCnt;
		}
		if ( iOtherGundanCnt > 0 ) {
			iOtherGundanGold /= iOtherGundanCnt;
			iOtherGundanRise /= iOtherGundanCnt;
			iOtherGundanTeppou /= iOtherGundanCnt;
			iOtherGundanKiba /= iOtherGundanCnt;
			iOtherGundanSoldier /= iOtherGundanCnt;
		}

		int iAvgGundanGold = iPlayerGundanGold + iOtherGundanGold;
		int iAvgGundanRise = iPlayerGundanRise + iOtherGundanRise;
		int iAvgGundanTeppou = iPlayerGundanTeppou + iOtherGundanTeppou;
		int iAvgGundanKiba = iPlayerGundanKiba + iOtherGundanKiba;
		int iAvgGundanSoldier = iPlayerGundanSoldier + iOtherGundanSoldier;

		// 各軍団に対して、お金と米を増やす。
		for (int iGundanNum=0; iGundanNum < GAMEDATASTRUCT_GUNDAN_NUM; iGundanNum++) {

			// 居城名ナンバーが0xffffだとその軍団は存在していない。次へ。
			if (nb6gundan[iGundanNum].residence == 0xFFFF ) {
				continue;
			}
			// 対象がプレイヤーの軍団であれば パス
			if ( isPlayerGundan(iGundanNum) ) {
				continue;
			}

			// 軍団ナンバー(1番は影響大、2番以降は影響半分)
			int iGNumber = nb6gundan[iGundanNum].number;

			// お金関連
			// オーバーフロー防止のためINT型へコピー
			int money = nb6gundan[iGundanNum].money;
			// 平均と元の間を取る
			if ( iGNumber == 1 ) {
				money = (5*money + lvl*iAvgGundanGold)/(5+lvl); 
			} else {
				money = (15*money + lvl*iAvgGundanGold)/(15+lvl); 
			}
			// オーバーフローを抑えて
			if ( money < 0 ) { money = 0; }
			if ( money > GAMEDATASTRUCT_MONEY_MAX) { money = GAMEDATASTRUCT_MONEY_MAX; }
			// 格納
			nb6gundan[iGundanNum].money = money;


			// お米関連
			// オーバーフロー防止のためINT型へコピー
			int rise = nb6gundan[iGundanNum].rise;
			// 平均と元の間を取る
			if ( iGNumber == 1 ) {
				rise = (5*rise + lvl*iAvgGundanRise)/(5+lvl); 
			} else {
				rise = (15*rise + lvl*iAvgGundanRise)/(15+lvl); 
			}
			// オーバーフローを抑えて
			if ( rise < 0 ) { rise = 0; }
			if ( rise > GAMEDATASTRUCT_RISE_MAX) { rise = GAMEDATASTRUCT_RISE_MAX; }
			// 格納
			nb6gundan[iGundanNum].rise = rise;

			// 鉄砲関連
			// オーバーフロー防止のためINT型へコピー
			int teppou = iGundanTeppou[iGundanNum];  // 該当軍団の兵士が持つ鉄砲＋余ってる鉄砲の合計

			// 該当軍団の合計鉄砲数 - 平均鉄砲数
			int subTeppou = teppou - iAvgGundanTeppou; 
				
			// これを該当の軍団に足し引きする

			int gun = nb6gundan[iGundanNum].gun;
			// 平均と元の間を取る
			if ( iGNumber == 1 ) {
				gun = gun - subTeppou/(2*(6-lvl)); 
			} else {
				gun = gun - subTeppou/(4*(6-lvl));
			}
			// オーバーフローを抑えて
			if ( gun < 0 ) { gun = 0; }
			if ( gun > GAMEDATASTRUCT_GUN_MAX) { gun = GAMEDATASTRUCT_GUN_MAX; }
			// 格納
			nb6gundan[iGundanNum].gun = gun;


			// 鉄砲関連
			// オーバーフロー防止のためINT型へコピー
			int kiba = iGundanKiba[iGundanNum];  // 該当軍団の兵士が持つ鉄砲＋余ってる鉄砲の合計

			// 該当軍団の合計鉄砲数 - 平均鉄砲数
			int subKiba = kiba - iAvgGundanKiba; 
				
			// これを該当の軍団に足し引きする

			int horse = nb6gundan[iGundanNum].horse;
			// 平均と元の間を取る
			if ( iGNumber == 1 ) {
				horse = horse - subKiba/(2*(6-lvl)); 
			} else {
				horse = horse - subKiba/(4*(6-lvl));
			}
			// オーバーフローを抑えて
			if ( horse < 0 ) { horse = 0; }
			if ( horse > GAMEDATASTRUCT_KIBA_MAX) { horse = GAMEDATASTRUCT_KIBA_MAX; }
			// 格納
			nb6gundan[iGundanNum].horse = horse;


		}
	}
}

