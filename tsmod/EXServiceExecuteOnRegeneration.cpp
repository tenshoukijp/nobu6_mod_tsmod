#include "WinTarget.h"
#include <math.h>

#include "EXBushou.h"
#include "EXBushou_All.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"

extern int  getCustomSoldierMax( int iBushouID, int soldiermax );

// 現在のメイン画面のターンでの各々の武将が増加しても良い兵士の数 
int ServiceExecuteRegenerationAllIncrSoldierInMain[GAMEDATASTRUCT_BUSHOU_NUM] = {};

// 上記の各々の武将について、メイン画面の｢増加許可数｣を計算する。
void EXServiceExecuteRegeneration_All_SoldierCalcInMain() {

	// 他勢力の自動回復が0より大きい
	if ( TSModCommand::OthersGundan::cmdRegenerationSoldierAutomatically > 0) {

		// 自動兵力回復サービスを受ける武将IDを全員分セット
		for (int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {

			if (nb6bushouref[iBushouID].gundan == 0xFFFF) {
				continue;
			}

			// プレイヤーの配下軍団でなければ…
			if (! isPlayerGundan(nb6bushouref[iBushouID].gundan-1)) {

				//　現在の身分を得る
				int position = nb6bushouname[iBushouID].position;

				//　身分から最大兵数を得る
				int soldiermax = nb6soldiermax[position].soldiermax;

				// 最大兵士数は、忍者だったり官位・役職等によってフィルターがかかる
				soldiermax = getCustomSoldierMax( iBushouID, soldiermax );

				// 増加しても良い数＝(MAXの兵士数-現在の兵数 / 9-指定レベル(0～5) = ９～４)
				ServiceExecuteRegenerationAllIncrSoldierInMain[iBushouID] = 
					(int)ceil
					(
						( 
							(double)soldiermax - (double)nb6bushouref[iBushouID].soldier
						) / (9-TSModCommand::OthersGundan::cmdRegenerationSoldierAutomatically) 
					);

				// 増加数が１より少ない場合は１とする。
				if ( ServiceExecuteRegenerationAllIncrSoldierInMain[iBushouID] < 1 ) {
					ServiceExecuteRegenerationAllIncrSoldierInMain[iBushouID] = 1;
				}
			}
		}
	}

	// 自勢力の自動回復が0より大きい
	if ( TSModCommand::MyselfGundan::cmdRegenerationSoldierAutomatically > 0) {

		// 自動兵力回復サービスを受ける武将IDを全員分セット
		for (int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {

			if (nb6bushouref[iBushouID].gundan == 0xFFFF) {
				continue;
			}

			// プレイヤーの配下軍団であれば…
			if (isPlayerGundan(nb6bushouref[iBushouID].gundan-1)) {

				//　現在の身分を得る
				int position = nb6bushouname[iBushouID].position;

				//　身分から最大兵数を得る
				int soldiermax = nb6soldiermax[position].soldiermax;

				// 最大兵士数は、忍者だったり官位・役職等によってフィルターがかかる
				soldiermax = getCustomSoldierMax( iBushouID, soldiermax );

				// 増加しても良い数＝(MAXの兵士数-現在の兵数 / 9-指定レベル(0～5) = ９～４)
				ServiceExecuteRegenerationAllIncrSoldierInMain[iBushouID] = 
					(int)ceil
					(
						( 
							(double)soldiermax - (double)nb6bushouref[iBushouID].soldier
						) / (9-TSModCommand::MyselfGundan::cmdRegenerationSoldierAutomatically) 
					);

				// 増加数が１より少ない場合は１とする。
				if ( ServiceExecuteRegenerationAllIncrSoldierInMain[iBushouID] < 1 ) {
					ServiceExecuteRegenerationAllIncrSoldierInMain[iBushouID] = 1;
				}
			}
		}
	}
}




// 現在の戦争画面へ入る際の各々の武将の保存兵数(ただし戦争中に増加したら、戦争中内でのターン単位で増加する。 
int ServiceExecuteRegenerationAllSaveBeginSoldier[GAMEDATASTRUCT_BUSHOU_NUM] = {};

// これは必ず戦争開始時に呼ぶこと
void EXServiceExecuteRegeneration_All_SoldierResetInBattle() {

	for (int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {
		// 全ての武将の保存兵数をリセット
		ServiceExecuteRegenerationAllSaveBeginSoldier[iBushouID] = 0;
	}
}

void EXServiceExecuteRegeneration_All_SoldierSaveInBattle() {
	// 自動微回復のどちらかのオプションがＯＮ
	if ( TSModCommand::OthersGundan::cmdRegenerationSoldierAutomatically > 0 || TSModCommand::MyselfGundan::cmdRegenerationSoldierAutomatically > 0) {

		for (int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {

			// 現在の兵数が保存した兵数より多くなっていれば、それを新たな保存兵数とする。
			if ( ServiceExecuteRegenerationAllSaveBeginSoldier[iBushouID] < nb6bushouref[iBushouID].soldier ) {
				ServiceExecuteRegenerationAllSaveBeginSoldier[iBushouID] = nb6bushouref[iBushouID].soldier;
			}
		}
	}
}


int EXServiceExecuteDaimyoHasCastleCnt[GAMEDATASTRUCT_DAIMYO_NUM] = {}; // 各々の大名が持つ城数

//最大勢力(最も持ち城数が多い)大名を得る
void EXServiceExecuteSetDaimyoHasCastleCnt() {
	// まず、一応呼ばれる度にリセット
	for (int iDaimyoID=0; iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM; iDaimyoID++ ) {
		EXServiceExecuteDaimyoHasCastleCnt[iDaimyoID] = 0;
	}

	for (int iCastleID=0; iCastleID < GAMEDATASTRUCT_CASTLE_NUM; iCastleID++ ) {
		// 所属大名番号に従って、カウントを増やしてゆく。
		int iGundanID = nb6castle[iCastleID].attach-1;
		if ( iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) { // 誰にも所属してない城はこの条件で弾かれる
			int iDaimyoID = nb6gundan[iGundanID].attach-1;
			EXServiceExecuteDaimyoHasCastleCnt[iDaimyoID]++;
		}
	}
}



// 現在の戦争画面のターンでの各々の武将が増加しても良い兵士の数(各々の武将の兵数の1+Lv%[繰上げ] 実際には指定の6～Lv%通り[繰上げ]))
int ServiceExecuteRegenerationAllIncrSoldierInBattle[GAMEDATASTRUCT_BUSHOU_NUM] = {};

// 上記の各々の武将について、戦争画面の｢増加許可数｣を計算する。
void EXServiceExecuteRegeneration_All_SoldierCalcInBattle() {

	// 他勢力の自動回復が0より大きい
	if ( TSModCommand::OthersGundan::cmdRegenerationSoldierAutomatically > 0) {

		// プレイヤ担当大名の所持城の平均数を求める
		int iPlayerTantoDaimyoCnt = 0;
		int iPlayerTantoCastleCnt = 0;

		// プレイヤの進捗と、プレイヤ以外の全体の軍団の進捗を見つつ、プレイヤ以外の軍団は互いに資源バランスを取ろうとします。
		// よってここでは、プレイヤとの対比で、城が少ないほど、増加係数が高く、多いほど増加係数が少ない
		if ( TSModCommand::OthersGundan::cmdPlayerAndTheWholeInfluenceEachOtherData > 0 ) {

			// 全ての大名の城数をセッティングする。副作用タイプ。
			EXServiceExecuteSetDaimyoHasCastleCnt();

			for ( int iDaimyoID=0; iDaimyoID<GAMEDATASTRUCT_DAIMYO_NUM; iDaimyoID++ ) {

				// 該当はプレイヤ担当の大名だ
				if ( isPlayerDaimyo( iDaimyoID ) ) {
					iPlayerTantoDaimyoCnt++;
					iPlayerTantoCastleCnt += EXServiceExecuteDaimyoHasCastleCnt[iDaimyoID];
				}
			}
			// プレイヤが担当している大名の平均所持城数を求める
			if ( iPlayerTantoDaimyoCnt > 0 ) {
				iPlayerTantoCastleCnt = iPlayerTantoCastleCnt / iPlayerTantoDaimyoCnt;
			}
		}

		if ( iPlayerTantoCastleCnt <= 0 ) {
			// 計算しやすいように、最低でも１つは持ってることにしてしまう。
			iPlayerTantoCastleCnt = 1;
		}

		// 自動兵力回復サービスを受ける武将IDを全員分セット
		for (int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {

			if (nb6bushouref[iBushouID].gundan == 0xFFFF) {
				continue;
			}

			// プレイヤーの配下軍団でなければ…
			if (! isPlayerGundan(nb6bushouref[iBushouID].gundan-1)) {

				float fCoef = 1.0;
				if ( TSModCommand::OthersGundan::cmdPlayerAndTheWholeInfluenceEachOtherData > 0 ) {
					// 該当武将が所属する大名ID(大名番号)
					int iTargetDaimyoID = nb6bushouref[iBushouID].attach -1;
					// 該当の大名が持つ城数
					int iTargetCastleNum = EXServiceExecuteDaimyoHasCastleCnt[iTargetDaimyoID];

					// プレイヤーの持ってる城数 / 該当大名が持ってる城数を、増加係数の１つとする。
					// これによりプレイヤー勢力が大きいほど、１ターンあたりの兵力増加数が大きくなる。
					if ( iTargetCastleNum > 0 ) {
						fCoef = (float)iPlayerTantoCastleCnt/(float)iTargetCastleNum;
						fCoef = fCoef*TSModCommand::OthersGundan::cmdPlayerAndTheWholeInfluenceEachOtherData;
						if ( fCoef > 25 ) { fCoef = 25; }
						if ( fCoef < 1 ) { fCoef = 1; }
					}
				}

				// 増加しても良い数＝(戦争開始時～現在までの最大兵数の6%)
				ServiceExecuteRegenerationAllIncrSoldierInBattle[iBushouID] = 
					(int)ceil(
						(double)ServiceExecuteRegenerationAllSaveBeginSoldier[iBushouID] * (double)(TSModCommand::OthersGundan::cmdRegenerationSoldierAutomatically+fCoef) / 100
					);
			}
		}
	}

	// 自勢力の自動回復が0より大きい
	if ( TSModCommand::MyselfGundan::cmdRegenerationSoldierAutomatically > 0) {

		// 自動兵力回復サービスを受ける武将IDを全員分セット
		for (int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {

			if (nb6bushouref[iBushouID].gundan == 0xFFFF) {
				continue;
			}

			// プレイヤーの配下軍団であれば…
			if (isPlayerGundan(nb6bushouref[iBushouID].gundan-1)) {

				// 増加しても良い数＝(戦争開始時～現在までの最大兵数の6%)
				ServiceExecuteRegenerationAllIncrSoldierInBattle[iBushouID] = 
					(int)ceil(
						(double)ServiceExecuteRegenerationAllSaveBeginSoldier[iBushouID] * (double)(1+TSModCommand::MyselfGundan::cmdRegenerationSoldierAutomatically) / 100
					);
			}
		}
	}
}





// 自動回復サービスを受ける全武将
EXBushou EXB_RegenerationBushou[GAMEDATASTRUCT_BUSHOU_NUM];

// 1回だけ設定するので
bool bEXServiceExecuteRegeneration_All_SetBushouID = false;

void EXServiceExecuteRegeneration_All_SetBushouID() {
	// 未設定なら
	if ( !bEXServiceExecuteRegeneration_All_SetBushouID ) {
		// 設定済みとする
		bEXServiceExecuteRegeneration_All_SetBushouID = true;

		// 自動兵力回復サービスを受ける武将IDを全員分セット
		for (int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {
			EXB_RegenerationBushou[iBushouID].setNb6BushouIX(iBushouID);
		}
	}
}


void EXServiceExecuteRegeneration_All_CheckRegeneration(int isForce = 0) {


	// 他勢力の自動回復が0より大きい
	if ( TSModCommand::OthersGundan::cmdRegenerationSoldierAutomatically > 0) {

		// プレイヤ担当大名の所持城の平均数を求める
		int iPlayerTantoDaimyoCnt = 0;
		int iPlayerTantoCastleCnt = 0;

		// プレイヤの進捗と、プレイヤ以外の全体の軍団の進捗を見つつ、プレイヤ以外の軍団は互いに資源バランスを取ろうとします。
		// よってここでは、プレイヤとの対比で、城が少ないほど、増加係数が高く、多いほど増加係数が少ない
		if ( TSModCommand::OthersGundan::cmdPlayerAndTheWholeInfluenceEachOtherData > 0 ) {

			// 全ての大名の城数をセッティングする。副作用タイプ。
			EXServiceExecuteSetDaimyoHasCastleCnt();

			for ( int iDaimyoID=0; iDaimyoID<GAMEDATASTRUCT_DAIMYO_NUM; iDaimyoID++ ) {

				// 該当はプレイヤ担当の大名だ
				if ( isPlayerDaimyo( iDaimyoID ) ) {
					iPlayerTantoDaimyoCnt++;
					iPlayerTantoCastleCnt += EXServiceExecuteDaimyoHasCastleCnt[iDaimyoID];
				}
			}
			// プレイヤが担当している大名の平均所持城数を求める
			if ( iPlayerTantoDaimyoCnt > 0 ) {
				iPlayerTantoCastleCnt = iPlayerTantoCastleCnt / iPlayerTantoDaimyoCnt;
			} 
		}

		if ( iPlayerTantoCastleCnt <= 0 ) {
			// 計算しやすいように、最低でも１つは持ってることにしてしまう。
			iPlayerTantoCastleCnt = 1;
		}

		// 自動兵力回復サービスを受ける武将IDを全員分セット
		for (int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {

			if (nb6bushouref[iBushouID].gundan == 0xFFFF) {
				continue;
			}

			// プレイヤーの配下軍団でなければ…
			if (! isPlayerGundan(nb6bushouref[iBushouID].gundan-1)) {

				float fCoef = 1.0;
				byte lv = TSModCommand::OthersGundan::cmdRegenerationSoldierAutomatically;
				fCoef = float(lv * lv); // デフォルトならこの値

				if ( TSModCommand::OthersGundan::cmdPlayerAndTheWholeInfluenceEachOtherData > 0 ) {
					// 該当武将が所属する大名ID(大名番号)
					int iTargetDaimyoID = nb6bushouref[iBushouID].attach -1;
					// 該当の大名が持つ城数
					int iTargetCastleNum = EXServiceExecuteDaimyoHasCastleCnt[iTargetDaimyoID];

					// プレイヤーの持ってる城数 / 該当大名が持ってる城数を、増加係数の１つとする。
					// これによりプレイヤー勢力が大きいほど、１ターンあたりの兵力増加数が大きくなる。
					if ( iTargetCastleNum > 0 ) {
						// レベルが高いほど短い時間で回復する。レベルの２乗で短くなってゆく。
						fCoef = (float)iPlayerTantoCastleCnt/(float)iTargetCastleNum;
						if (fCoef > 20*TSModCommand::OthersGundan::cmdPlayerAndTheWholeInfluenceEachOtherData ) { 
							fCoef = float(20*TSModCommand::OthersGundan::cmdPlayerAndTheWholeInfluenceEachOtherData);
						}
						fCoef = (float)lv*(float)lv*fCoef;
						if ( fCoef > 100 ) { fCoef = 100; }
						if ( fCoef < 1 ) { fCoef = 1; }
					}
				}
				if ( fCoef > 0 ) {
					EXB_RegenerationBushou[iBushouID].checkRegenerationNB6BushouSoldier((int)((float)250000/fCoef), isForce); 
				}
			}
		}
	}

	// 自勢力の自動回復が0より大きい
	if ( TSModCommand::MyselfGundan::cmdRegenerationSoldierAutomatically > 0) {

		// 自動兵力回復サービスを受ける武将IDを全員分セット
		for (int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {

			if (nb6bushouref[iBushouID].gundan == 0xFFFF) {
				continue;
			}

			// プレイヤーの配下軍団であれば…
			if (isPlayerGundan(nb6bushouref[iBushouID].gundan-1)) {

				// レベルが高いほど短い時間で回復する。レベルの２乗で短くなってゆく。
				byte lv = TSModCommand::MyselfGundan::cmdRegenerationSoldierAutomatically;
				EXB_RegenerationBushou[iBushouID].checkRegenerationNB6BushouSoldier(250000/(lv*lv), isForce); 
			}

		}
	}

}


void EXSuperService_Regeneration(int isForce) {

	// 武将IDを一通り登録
	EXServiceExecuteRegeneration_All_SetBushouID();

	// リジェネチェック
	EXServiceExecuteRegeneration_All_CheckRegeneration(isForce);

}