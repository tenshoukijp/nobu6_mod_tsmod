#include "CommonGlobalHandle.h"
#include "EXServiceExecute.h"
#include "GameDataStruct.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "TSModCommand.h"
#include "TSMod.h"


// 毎シーズン、余計に、金を米を入手する処理
void EXSuperService_CheckOtherDaimyosGetMoneyAndRiseOnSeasonChange() {

	// 自勢力への増加関連のコマンドが有効ならば
	if ( TSModCommand::MyselfGundan::cmdGetMoneyAndRiseOnSeasonChangeLv > 0 ) {

		// 各軍団に対して、鉄砲や馬を増やす
		for (int iGundanNum=0; iGundanNum < GAMEDATASTRUCT_GUNDAN_NUM; iGundanNum++) {

			// 居城名ナンバーが0xffffだとその軍団は存在していない。次へ。
			if (nb6gundan[iGundanNum].residence == 0xFFFF ) {
				continue;
			}
			// 対象が他勢力の軍団であれば パス
			if ( !isPlayerGundan(iGundanNum) ) {
				continue;
			}

			// オーバーフロー防止のためINT型へコピー
			int money = nb6gundan[iGundanNum].money;
			// コマンドレベルに合わせて増加
			money += TSModCommand::MyselfGundan::cmdGetMoneyAndRiseOnSeasonChangeLv * 50 * nb6progressseason; // 経過季節
			// オーバーフローを抑えて
			if ( money > GAMEDATASTRUCT_MONEY_MAX) { money = GAMEDATASTRUCT_MONEY_MAX; }
			// 格納
			nb6gundan[iGundanNum].money = money;


			// オーバーフロー防止のためINT型へコピー
			int rise = nb6gundan[iGundanNum].rise;
			// コマンドレベルに合わせて増加
			rise += TSModCommand::MyselfGundan::cmdGetMoneyAndRiseOnSeasonChangeLv * 50 * nb6progressseason; // 経過季節
			// オーバーフローを抑えて
			if ( rise > GAMEDATASTRUCT_RISE_MAX) { rise = GAMEDATASTRUCT_RISE_MAX; }
			// 格納
			nb6gundan[iGundanNum].rise = rise;
		}
	}

	// 他勢力への増加関連のコマンドが有効ならば
	if ( TSModCommand::OthersGundan::cmdGetMoneyAndRiseOnSeasonChangeLv > 0 ) {

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

			// オーバーフロー防止のためINT型へコピー
			int money = nb6gundan[iGundanNum].money;
			// コマンドレベルに合わせて増加
			money += TSModCommand::OthersGundan::cmdGetMoneyAndRiseOnSeasonChangeLv * 50 * nb6progressseason; // 経過季節
			// オーバーフローを抑えて
			if ( money > GAMEDATASTRUCT_MONEY_MAX) { money = GAMEDATASTRUCT_MONEY_MAX; }
			// 格納
			nb6gundan[iGundanNum].money = money;


			// オーバーフロー防止のためINT型へコピー
			int rise = nb6gundan[iGundanNum].rise;
			// コマンドレベルに合わせて増加
			rise += TSModCommand::OthersGundan::cmdGetMoneyAndRiseOnSeasonChangeLv * 50 * nb6progressseason; // 経過季節
			// オーバーフローを抑えて
			if ( rise > GAMEDATASTRUCT_RISE_MAX) { rise = GAMEDATASTRUCT_RISE_MAX; }
			// 格納
			nb6gundan[iGundanNum].rise = rise;
		}
	}
}

