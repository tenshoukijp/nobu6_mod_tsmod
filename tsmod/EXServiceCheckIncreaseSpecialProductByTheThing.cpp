#include "CommonGlobalHandle.h"
#include "EXServiceExecute.h"
#include "GameDataStruct.h"
#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "TSModCommand.h"
#include "TSMod.h"

// いずれかの軍団が鉄砲を所持しているのか
bool isAnyGudanHasTeppou() {
	int iSumOfGun = 0;
	for (int iGundanID = 0; iGundanID < GAMEDATASTRUCT_GUNDAN_NUM; iGundanID++) {
		iSumOfGun += nb6gundan[iGundanID].gun;
	}

	if (iSumOfGun > 0) {
		return true;
	}
	else {
		return false;
	}
}

// 特産品に関連した項目が自動的に増える(鉄砲・馬・国際港)
void EXSuperService_CheckIncreaseSpecialProductByTheThing() {

	// 自勢力の特産品に関連した項目が自動的に増える(鉄砲・馬・国際港)
	if ( TSModCommand::MyselfGundan::cmdIncreaseSpecialProductByTheThing > 0 ) {
		for ( int iCastleID=0; iCastleID < GAMEDATASTRUCT_CASTLE_NUM; iCastleID++) {

			// どこの軍団に所属しているか？
			if ( nb6castle[iCastleID].attach == 0xFFFF) {
				continue; // 所属してなければ次へ
			}

			int iGundanID = nb6castle[iCastleID].attach-1;

			// 値が不正
			if ( !(0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) ) {
				continue;
			}

			// 対象が他勢力の軍団であれば パス
			if ( !isPlayerGundan(iGundanID) ) {
				continue;
			}
			
			// 鉄砲が存在しない世の中、にフラグが立っていない
			if ( TSModCommand::World::cmdTheTeppouNoExists == 0 ) {

				if (isTeppouDenrai() || isAnyGudanHasTeppou() ) {
					// 該当の城に鍛冶の属性が付いていたら、TSModCommand::MyselfGundan::cmdIncreaseSpecialProductByTheThing * 3 の値だけ増える
					nb6gundan[iGundanID].gun += nb6castle[iCastleID].bSmith * TSModCommand::MyselfGundan::cmdIncreaseSpecialProductByTheThing * 3;
					if (nb6gundan[iGundanID].gun > GAMEDATASTRUCT_GUN_MAX) {
						nb6gundan[iGundanID].gun = GAMEDATASTRUCT_GUN_MAX;
					}
					// 該当の城に国際港の属性が付いていたら、TSModCommand::MyselfGundan::cmdIncreaseSpecialProductByTheThing * 3 の値だけ増える
					nb6gundan[iGundanID].gun += nb6castle[iCastleID].blPort * TSModCommand::MyselfGundan::cmdIncreaseSpecialProductByTheThing * 3;
					if (nb6gundan[iGundanID].gun > GAMEDATASTRUCT_GUN_MAX) {
						nb6gundan[iGundanID].gun = GAMEDATASTRUCT_GUN_MAX;
					}
				}
			}

			// 該当の城に馬産の属性が付いていたら、TSModCommand::MyselfGundan::cmdIncreaseSpecialProductByTheThing の値だけ増える
			nb6gundan[iGundanID].horse +=  nb6castle[iCastleID].bHorse * TSModCommand::MyselfGundan::cmdIncreaseSpecialProductByTheThing;
			if ( nb6gundan[iGundanID].horse > GAMEDATASTRUCT_KIBA_MAX ) {
				nb6gundan[iGundanID].horse = GAMEDATASTRUCT_KIBA_MAX;
			}

		}
	}


	// 他勢力の特産品に関連した項目が自動的に増える(鉄砲・馬・国際港)
	if ( TSModCommand::OthersGundan::cmdIncreaseSpecialProductByTheThing > 0 ) {
		for ( int iCastleID=0; iCastleID < GAMEDATASTRUCT_CASTLE_NUM; iCastleID++) {

			// どこの軍団に所属しているか？
			if ( nb6castle[iCastleID].attach == 0xFFFF) {
				continue; // 所属してなければ次へ
			}

			int iGundanID = nb6castle[iCastleID].attach-1;

			// 値が不正
			if ( !(0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) ) {
				continue;
			}

			// 対象が自勢力の軍団であれば パス
			if ( isPlayerGundan(iGundanID) ) {
				continue;
			}
			
			// 鉄砲が存在しない世の中、にフラグが立っていない
			if (TSModCommand::World::cmdTheTeppouNoExists == 0) {

				if (isTeppouDenrai() || isAnyGudanHasTeppou() ) {

					// 該当の城に鍛冶の属性が付いていたら、TSModCommand::OthersGundan::cmdIncreaseSpecialProductByTheThing * 3 の値だけ増える
					nb6gundan[iGundanID].gun += nb6castle[iCastleID].bSmith * TSModCommand::OthersGundan::cmdIncreaseSpecialProductByTheThing * 3;

					if (nb6gundan[iGundanID].gun > GAMEDATASTRUCT_GUN_MAX) {
						nb6gundan[iGundanID].gun = GAMEDATASTRUCT_GUN_MAX;
					}

					// 該当の城に国際港の属性が付いていたら、TSModCommand::OthersGundan::cmdIncreaseSpecialProductByTheThing * 3 の値だけ増える
					nb6gundan[iGundanID].gun += nb6castle[iCastleID].blPort * TSModCommand::OthersGundan::cmdIncreaseSpecialProductByTheThing * 3;
					if (nb6gundan[iGundanID].gun > GAMEDATASTRUCT_GUN_MAX) {
						nb6gundan[iGundanID].gun = GAMEDATASTRUCT_GUN_MAX;
					}
				}
			}

			// 該当の城に馬産の属性が付いていたら、TSModCommand::OthersGundan::cmdIncreaseSpecialProductByTheThing の値だけ増える
			nb6gundan[iGundanID].horse +=  nb6castle[iCastleID].bHorse * TSModCommand::OthersGundan::cmdIncreaseSpecialProductByTheThing;
			if ( nb6gundan[iGundanID].horse > GAMEDATASTRUCT_KIBA_MAX ) {
				nb6gundan[iGundanID].horse = GAMEDATASTRUCT_KIBA_MAX;
			}

		}
	}

}
