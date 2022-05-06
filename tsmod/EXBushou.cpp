#define _CRT_SECURE_NO_WARNINGS

#pragma pack(1)

#include "WinTarget.h"

#include "EXBushou.h"
#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"



extern int getCustomSoldierMax(int, int);


EXBushou::EXBushou() {
	// 名前は空でスタート
	m_name[0] = NULL;

	// 武将データ配列上でのIX(順番)は-1でセット
	m_nb6bushou_ix = -1;

	m_curTimeGetTime = ::timeGetTime();
	m_preTimeGetTime = m_curTimeGetTime;

	// 歳の取りにくさ
	m_lifetimes = 1;
}

void EXBushou::setName(char *name) {
	strcpy(m_name, name);
}


void EXBushou::setNb6BushouIX(int iBushouID) {
	m_nb6bushou_ix = iBushouID;
}

int EXBushou::getNb6BushouIX() {


	// すでに武将データ配列上でのIndexが分かっているなら、それを返す
	if (m_nb6bushou_ix >= 0) {
		// 死亡しているならば名前を消して-1を返す
		if ( 0 <= m_nb6bushou_ix && m_nb6bushou_ix < GAMEDATASTRUCT_BUSHOU_NUM ) {
			if ( nb6bushouname[m_nb6bushou_ix].State == 7 ) {
				strcpy(m_name, "");
				return -1;
			}
		}
 
		return m_nb6bushou_ix;

	// 名前が設定されていれば、ixが判明する。
	} else if (strlen(m_name) > 2) {

		for (int b_ix=0; b_ix<GAMEDATASTRUCT_BUSHOU_NUM; b_ix++) {
			// 苗字+名前が一致したら、それがそいつの番号だ!!

			// 苗字+名前の枠を用意
			char fullname[100];
			// 苗字+名前をセット
			wsprintf(fullname, "%s%s", nb6bushouname[b_ix].familyname, nb6bushouname[b_ix].fastname);

			fullname[12] = NULL;

			// 比較して一致したら、配列番号を返す
			if (strcmp( m_name, fullname) == 0) {
				// 以後検索しなくても良いようにセット
				m_nb6bushou_ix = b_ix;
				
				// 値を返しておく
				return b_ix;
			}
		}
		return m_nb6bushou_ix;

	// 
	} else {
		return m_nb6bushou_ix;
	}
}

// 各々の武将が今季節メイン画面で増加させても良い兵士数
extern int ServiceExecuteRegenerationAllIncrSoldierInMain[GAMEDATASTRUCT_BUSHOU_NUM];
// 各々の武将が今回の戦闘画面で増加させても良い兵士数
extern int ServiceExecuteRegenerationAllIncrSoldierInBattle[GAMEDATASTRUCT_BUSHOU_NUM];
// 各々の武将が戦争開始前に保存した兵指数(戦争中に徴兵するなりして最大すうが増えると一応戦争内ターン切り替え時に増える);
extern int ServiceExecuteRegenerationAllSaveBeginSoldier[GAMEDATASTRUCT_BUSHOU_NUM];



void EXBushou::checkRegenerationNB6BushouSoldier(int RefleshTime, int isForce ) {

	// 武将配列データ内に、自身が確立された状態であれば
	int iBushouID = getNb6BushouIX();

	// 武将配列データ内に、自身が確立された状態であれば
	if ( !(0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) ) {
		return;
	}

	m_curTimeGetTime = timeGetTime();

	// RefleshTimeミリ秒に１ずつ兵が回復、また、強制フラグに１が立ってる時
	if ( int(m_curTimeGetTime - m_preTimeGetTime) > int(RefleshTime) || isForce ) { // RefleshTimeミリ秒に1ずつ兵が回復してゆく

		m_preTimeGetTime = m_curTimeGetTime;



		int iSoldierNum    = nb6bushouref[iBushouID].soldier;  // 兵数
		int iSoldierTrain  = nb6bushouref[iBushouID].training; // 訓練
		int iSoldierLoyal  = nb6bushouref[iBushouID].solloyal; // 兵忠


		//　現在の身分を得る
		int position = nb6bushouname[iBushouID].position;
		//　身分から最大兵数を得る
		int soldiermax = nb6soldiermax[position].soldiermax;

		// 最大兵士数は、忍者だったり官位・役職等によってフィルターがかかる
		soldiermax = getCustomSoldierMax( iBushouID, soldiermax );

		// 兵士が０の時強制的に増やす(主に相場変更時やゲームスタート時)
		if ( iSoldierNum == 0 && isForce ) {

			// 兵力を１増やす。
			iSoldierNum++;

			nb6bushouref[iBushouID].soldier = iSoldierNum;

			// 兵力０から増やすので訓練値の設定。訓練値を35+戦才(100換算)/3 とする。
			iSoldierTrain = 35 + (nb6bushouref[iBushouID].maxbat / 3);

			nb6bushouref[iBushouID].training = iSoldierTrain;

			// 兵力０から増やすので士気値の設定。士気値を35+魅力(100換算)/3 とする。
			iSoldierLoyal = 35 + (nb6bushouref[iBushouID].charm / 3);

			nb6bushouref[iBushouID].solloyal = iSoldierLoyal;


		// 最大兵数数より多くなってしまっている場合もランディングさせる
		} else if (iSoldierNum > soldiermax) {

			// 兵力を１減らす。
			if ( iSoldierNum > 0 ) { iSoldierNum--; } else { iSoldierNum = 0; }

			nb6bushouref[iBushouID].soldier = iSoldierNum;


		// 1以上の兵数がいて、該当の最大兵数より少ない場合のみ処理を行う (ただし相場切り替え時のみ、0以上なら増加処理をする)
		} else if (1 <= iSoldierNum && iSoldierNum < soldiermax ) {

			// ピンチの時は…
			if (iSoldierNum < soldiermax*0.3 ) { // 兵数が3割未満ですごくヤバイ
				m_preTimeGetTime -= DWORD(RefleshTime*0.8); // 5000の場合、4000引くことで、次回の更新を1秒間隔とする。(時間が４秒経過したことにしてしまう)
			} else if ( iSoldierNum < soldiermax*0.6 ) { // 兵数が6未満 でちょっと減ってきた
				m_preTimeGetTime -= DWORD(RefleshTime*0.4); // 5000の場合、2000引くことで、次回の更新を3秒間隔とする。(時間が２秒経過したことにしてしまう)
			}

			if (  m_isUltraBushou ) {
				// 兵力を１増やす。
				iSoldierNum++;

				nb6bushouref[iBushouID].soldier = iSoldierNum;

			// 現在戦争が起きておらず、メイン画面の残存加算数が残っているならば…
			} else if ( !CommonGlobalHandle::isBattleMode && ServiceExecuteRegenerationAllIncrSoldierInMain[iBushouID] > 0 ) { // メイン画面用の増加しても良い兵数が残っている

				// 兵力を１増やす。
				iSoldierNum++;

				// 増やした分を減らす。
				ServiceExecuteRegenerationAllIncrSoldierInMain[iBushouID]--;

				nb6bushouref[iBushouID].soldier = iSoldierNum;

			// 現在戦争中で、戦争用の残存加算数が残っているならば…
			} else if ( CommonGlobalHandle::isBattleMode && ServiceExecuteRegenerationAllIncrSoldierInBattle[iBushouID] > 0 ) { // 戦闘画面用の増加しても良い兵数が残っている

				// 現在の兵士数が、戦争前より少ないならば…
				if ( iSoldierNum < ServiceExecuteRegenerationAllSaveBeginSoldier[iBushouID] ) {
					// 兵力を１増やす。
					iSoldierNum++;

					// 増やした分を減らす。
					ServiceExecuteRegenerationAllIncrSoldierInBattle[iBushouID]--;

					nb6bushouref[iBushouID].soldier = iSoldierNum;
				}
			}


		}

		
	}

}


// 大砲の修復
void EXBushou::checkRestoreNB6BushouTaihou() {
	
	// 武将配列データ内に、自身が確立された状態であれば
	int iBushouID = getNb6BushouIX();

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		// 大砲を修正
		nb6bushouref[iBushouID].biggun = 1;

	}
}


/*
 * 寿命はデフォルトで数倍である。
 */
void EXBushou::checkTimesNB6BushouLife() {

	// まだメインゲームまわってなければ、実行しない
	if ( !CommonGlobalHandle::isMainGameStart ) {
		return;
	}

	// 歳を取るのが遅くなるを倍数化させるコマンドが有効の時だけ発動
	if ( TSModCommand::AllBushou::cmdLifeIsTheTimes >= 2 ) {
		return;
	}

	// 冬から春にかわる時以外以外は無効
	if ( nb6year.season != 3 ) {
		return;
	}

	int iBushouID = getNb6BushouIX();

	// 武将配列データ内に、自身が確立された状態であれば
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ３倍としてした数でyearを割った時、0でなければ、歳を取らない。
		// 歳を取らないためには、｢誕生年｣を１つ増やさなければならない。
		if ( (nb6year.year % m_lifetimes) != 0 ) {

			// 誕生年を１つ。255だと増やさない(0になってまう)
			if ( nb6bushouref[iBushouID].birth <= 254 ) {

				int old = getYearsOld(iBushouID);
				if (old >= 13) {
					int birth = nb6bushouref[iBushouID].birth;
					birth++;
					nb6bushouref[iBushouID].birth = birth;
				}
			}
		}
	}
}