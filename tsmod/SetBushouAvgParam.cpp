#include "WinTarget.h"

#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "VariableNameRegistAndJudge.h"

#include "SetBushouAvgParam.h"
#include "EXBushou_All.h"

void CommonSetBushouAvgParamWithRekidaiFlagOn(int iBushouID, int iFaceID);
void CommonSetBushouAvgParam(int iBushouID, int iFaceID);

// 職業についてはフラグに関係なく設定
extern void setJobEXOfCurABushou(int iBushouID);

// シナリオ開始/ロード時用のパラメタ反映処理
void SetBushouAvgParam(int iFlagValue) {

	// 計算軽量化のために、最初に一度登録しておく。
	bool is_original_bushou_flag[GAMEDATASTRUCT_BUSHOU_NUM] = {false};

	for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

		// 元々天翔記に居る武将であれば、能力を反映可能
		if ( IsOriginalRegistedBushou(iBushouID) ) {
			// その列伝が1300番未満になっている。(名前だけ一致していて、顔や列伝違うなどということはない)
			if ( nb6bushouname[iBushouID].number < GAMEDATASTRUCT_REGISTED_BUSHOU_NUM ) {
				is_original_bushou_flag[iBushouID] = true;
			}
		}
	}

	// 家宝による追加値
	byte kahou_add_gov[GAMEDATASTRUCT_BUSHOU_NUM] = {};
	byte kahou_add_bat[GAMEDATASTRUCT_BUSHOU_NUM] = {};
	byte kahou_add_int[GAMEDATASTRUCT_BUSHOU_NUM] = {};
	byte kahou_add_amb[GAMEDATASTRUCT_BUSHOU_NUM] = {};

	// 家宝はかならず、天翔記に現在登場している奴(532メンバー)が持つこととなる。
	for ( int iKahouID=0; iKahouID < GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {

		// 所持者が武将かどうか
		int iHasID = nb6kahou[iKahouID].attach - 1;
		if ( 0 <= iHasID && iHasID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			// 所持者が武将の場合
			// 登場しているということ

			int ktype = nb6kahou[iKahouID].type;
			int krank = nb6kahou[iKahouID].param;

			// 政治増加タイプ：0 茶碗  1 茶壺  2 茶入  3 茶釜  4 花入  
			if ( 0x0 <= ktype && ktype <= 0x4 ) {
				// 今回の家宝の方が上昇値が大きいならば…
				if ( krank > kahou_add_gov[iHasID] ) {
					kahou_add_gov[iHasID] = krank;
				}
			}
			// 戦闘増加タイプ：5 刀剣  6 槍  7 具足  8 馬  9 鞍  A 陣羽織   
			else if ( 0x5 <= ktype && ktype <= 0xA ) {
				// 今回の家宝の方が上昇値が大きいならば…
				if ( krank > kahou_add_bat[iHasID] ) {
					kahou_add_bat[iHasID] = krank;
				}
			}
			// 智謀増加タイプ：B 巻子本  C 明朝綴本
			else if ( 0xB <= ktype && ktype <= 0xC ) {
				// 今回の家宝の方が上昇値が大きいならば…
				if ( krank > kahou_add_int[iHasID] ) {
					kahou_add_int[iHasID] = krank;
				}
			}
			// 野望増加タイプ：D 香木  E 地球儀  F 南蛮時計  10 水墨画  11 十字架  12 聖書  13 玉手箱  14 ビードロ杯  15 遠眼鏡  16 ルソン壺  17 煙草
			else if ( 0xD <= ktype && ktype <= 0x17 ) {
				// 今回の家宝の方が上昇値が大きいならば…
				if ( krank > kahou_add_amb[iHasID] ) {
					kahou_add_amb[iHasID] = krank;
				}
			}
		} else {
			continue;
		}
	}

	// 官位による魅力値の追加
	byte kani_add_chm[GAMEDATASTRUCT_BUSHOU_NUM] = {};

	// 官位はかならず、天翔記に現在登場している奴(532メンバー)が持つこととなる。
	for ( int iKaniID=0; iKaniID < GAMEDATASTRUCT_KANI_NUM; iKaniID++) {
		// 所持者が武将かどうか
		int iHasID = nb6kani[iKaniID].attach - 1;
		if ( 0 <= iHasID && iHasID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			// 所持者が武将の場合
			// 登場しているということ

			int krank = nb6kani[iKaniID].param;
			// 今回の官位の方が上昇値が大きいならば…
			if ( krank > kani_add_chm[iHasID] ) {
				kani_add_chm[iHasID] = krank;
			}
		} else {
			continue;
		}
	}


	// 天翔記に居る武将の能力を歴代シリーズに沿う形にするならば(1==新規シナリオ時に反映する)
	for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

		// 天翔記に元々存在する武将ならば…
		if ( is_original_bushou_flag[iBushouID] ) {
			int iFaceID = nb6bushouname[iBushouID].number;

			// 歴代シリーズフラグがＯＮであり、シナリオ開始時までか、セーブデータもするかの範囲も考慮
			if ( 0 < TSModCommand::AllBushou::cmdParamConsiderNobunagaSeries &&
				     iFlagValue <= TSModCommand::AllBushou::cmdParamConsiderNobunagaSeries ) {

				// 政治・戦闘・智謀
				nb6bushouref[iBushouID].maxgov = nb6AvgParam_Gov[iFaceID] + kahou_add_gov[iBushouID];
				nb6bushouref[iBushouID].maxbat = nb6AvgParam_Bat[iFaceID] + kahou_add_bat[iBushouID];
				nb6bushouref[iBushouID].maxint = nb6AvgParam_Int[iFaceID] + kahou_add_int[iBushouID];
				// 野望・魅力
				nb6bushouref[iBushouID].ambition = nb6AvgParam_Amb[iFaceID] + kahou_add_amb[iBushouID];
				nb6bushouref[iBushouID].charm = nb6AvgParam_Chm[iFaceID] + kani_add_chm[iBushouID];

				// 共通のパラメタ処理
				CommonSetBushouAvgParamWithRekidaiFlagOn(iBushouID, iFaceID);
			}

			CommonSetBushouAvgParam(iBushouID, iFaceID);
		}

	}
}



// 武将登場時用のパラメタ反映処理
void SetBushouAvgParamAppearanceBushou(int iBushouID) {

	SetExBushouAvgParam(iBushouID); // 特殊武将用のパラメタ設定

	// 元々天翔記に居る武将であれば、能力を反映可能
	if ( IsOriginalRegistedBushou(iBushouID) ) {

		// その列伝が1300番未満になっている。(名前だけ一致していて、顔や列伝違うなどということはない)
		if ( nb6bushouname[iBushouID].number < GAMEDATASTRUCT_REGISTED_BUSHOU_NUM ) {

			int iFaceID = nb6bushouname[iBushouID].number;

			if ( TSModCommand::AllBushou::cmdParamConsiderNobunagaSeries ) { // 1でも2でも適応する。
				// 政治・戦闘・智謀
				nb6bushouref[iBushouID].maxgov = nb6AvgParam_Gov[iFaceID];
				nb6bushouref[iBushouID].maxbat = nb6AvgParam_Bat[iFaceID];
				nb6bushouref[iBushouID].maxint = nb6AvgParam_Int[iFaceID];
				// 野望・魅力
				nb6bushouref[iBushouID].ambition = nb6AvgParam_Amb[iFaceID];
				nb6bushouref[iBushouID].charm = nb6AvgParam_Chm[iFaceID];

				// 共通のパラメタ処理
				CommonSetBushouAvgParamWithRekidaiFlagOn(iBushouID, iFaceID);
			}

			// 職業についてはフラグに関係なく設定
			setJobEXOfCurABushou(iBushouID);

			CommonSetBushouAvgParam(iBushouID, iFaceID);

		}

	}
}

// 歴代シリーズがＯＮの時だけやる。平均値反映の共通処理(シナリオ開始時と、武将登場時の共通)
void CommonSetBushouAvgParamWithRekidaiFlagOn(int iBushouID, int iFaceID) {

	// 義理
	nb6bushouref[iBushouID].duty = nb6AvgParam_Gir[iFaceID];

	// 足軽
	setBushouAptitude( iBushouID, APTITUDE_ASIGARU, nb6AvgParam_Bat_Ashigaru[iFaceID] );
	// 騎馬
	setBushouAptitude( iBushouID, APTITUDE_KIBA, nb6AvgParam_Bat_Kiba[iFaceID] );
	// 鉄砲
	setBushouAptitude( iBushouID, APTITUDE_TEPPOU, nb6AvgParam_Bat_Teppou[iFaceID] );
	// 水軍
	setBushouAptitude( iBushouID, APTITUDE_SUIGUN, nb6AvgParam_Bat_Suigun[iFaceID] );
}

// 高家・国人 などといった置き換えようの文字列1300人分のリスト
extern char *pOriginalBushouJobExList[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM];

// 歴代シリーズがＯＮかどうか関係なく。平均値反映の共通処理
void CommonSetBushouAvgParam(int iBushouID, int iFaceID) {


	// 弓はTSMod.txtの設定には関係なく設定する。
	// 弓の適性をコピー
	// 弓が無いというフラグがONなのであれば、0で上書きする。
	if ( TSModCommand::World::cmdTheYumiNoExists ) {
		nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = 0;
	} else {
		nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = nb6AvgParam_Bat_Yumi[iFaceID];
		if (0 <= iFaceID && iFaceID < GAMEDATASTRUCT_REGISTED_BUSHOU_NUM) {
			if (strstr(pOriginalBushouJobExList[iFaceID], "弓")) {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = max(3, nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi); // B以上にはして弓は使えるように
			}
		}
	}
	// 槍はTSMod.txtの設定には関係なく設定する。
	// 槍の適性をコピー
	nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari = nb6AvgParam_Bat_Yari[iFaceID];

	// 特殊属性は、一般武将は持ち合わせないので、全てクリア
	nb6savedata_ex.bushou_ex[iBushouID].aptitudeono = 0;
	nb6savedata_ex.bushou_ex[iBushouID].aptitudemuchi = 0;
	nb6savedata_ex.bushou_ex[iBushouID].aptitudetuchi = 0;
	nb6savedata_ex.bushou_ex[iBushouID].aptitudetue = 0;
	nb6savedata_ex.bushou_ex[iBushouID].aptitudeken = 0;

	// 教養・統率
	nb6savedata_ex.bushou_ex[iBushouID].education = nb6AvgParam_Edu[iFaceID];
	nb6savedata_ex.bushou_ex[iBushouID].command = nb6AvgParam_Cmd[iFaceID];
	// 農業
	nb6savedata_ex.bushou_ex[iBushouID].aptitudenougyou = nb6AvgParam_Nougyou[iFaceID];
	// 商業
	nb6savedata_ex.bushou_ex[iBushouID].aptitudesyougyou = nb6AvgParam_Syougyou[iFaceID];
	// 築城
	nb6savedata_ex.bushou_ex[iBushouID].aptitudechikujo = nb6AvgParam_Chikujo[iFaceID];
	// 外交
	nb6savedata_ex.bushou_ex[iBushouID].aptitudegaikou = nb6AvgParam_Gaikou[iFaceID];
	// 登用
	nb6savedata_ex.bushou_ex[iBushouID].aptitudetouyou = nb6AvgParam_Touyou[iFaceID];
}

