#include "WinTarget.h"
#include <fstream>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"




/*
 * TSMod.txtで指定すると、戦闘中にプレイヤ勢力の武将以外は全技能が利用可能である。
 */

struct OriginalTechPreviousBattle {
	byte tech_ikkatsu:1;	//技能・一喝
	byte tech_ansatsu:1;	//技能・暗殺
	byte tech_ryusyutsu:1;	//技能・流出
	byte tech_sendou:1;		//技能・煽動
	byte tech_ryugen:1;		//技能・流言
	byte tech_benzetsu:1;	//技能・弁舌
	byte tech_yakiuchi:1;	//技能・焼討
	byte tech_chohatsu:1;	//技能・挑発
} OriginalTachOf_IfCanAllAlibityInBattleStart[GAMEDATASTRUCT_BUSHOU_NUM];

// 出陣前にＮＰＣ武将であったかどうか。
bool isNPCBushouOfPreviousBattle[GAMEDATASTRUCT_BUSHOU_NUM];

// 戦闘に入る前に、元の技能をバックアップする。
void EXSuperService_IfCanAllAlibityInBattleStart() {

	// 戦闘中にプレイヤ勢力の武将以外は全技能が利用可能である。がＯＦＦかどうか。
	if ( ! TSModCommand::OtherBushou::cmdCanUseAllAbilityInBattle ) {
		return;
	}

	for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

		if ( !isPlayerBushou( iBushouID ) ) { 
			isNPCBushouOfPreviousBattle[iBushouID] = true;

			OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_ikkatsu  = nb6bushouref[iBushouID].tech_ikkatsu;
			OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_ansatsu  = nb6bushouref[iBushouID].tech_ansatsu;
			OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_ryusyutsu= nb6bushouref[iBushouID].tech_ryusyutsu;
			OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_sendou   = nb6bushouref[iBushouID].tech_sendou;
			OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_ryugen   = nb6bushouref[iBushouID].tech_ryugen;
			OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_benzetsu = nb6bushouref[iBushouID].tech_benzetsu;
			OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_yakiuchi = nb6bushouref[iBushouID].tech_yakiuchi;
			OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_chohatsu = nb6bushouref[iBushouID].tech_chohatsu;

			nb6bushouref[iBushouID].tech_ikkatsu = 1; 
			nb6bushouref[iBushouID].tech_ansatsu = 1;  
			nb6bushouref[iBushouID].tech_ryusyutsu = 1;
			nb6bushouref[iBushouID].tech_sendou = 1;	
			nb6bushouref[iBushouID].tech_ryugen = 1;	
			nb6bushouref[iBushouID].tech_benzetsu = 1; 
			nb6bushouref[iBushouID].tech_yakiuchi = 1; 
			nb6bushouref[iBushouID].tech_chohatsu = 1; 
		} else {
			isNPCBushouOfPreviousBattle[iBushouID] = false;
		}
	}
}


// 戦闘が終了したので元へと戻す
void EXSuperService_IfCanAllAlibityInBattleEnd() {

	// 戦闘中にプレイヤ勢力の武将以外は全技能が利用可能である。がＯＦＦかどうか。
	if ( ! TSModCommand::OtherBushou::cmdCanUseAllAbilityInBattle ) {
		return;
	}

	for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

		if ( isNPCBushouOfPreviousBattle[iBushouID] ) { 

			nb6bushouref[iBushouID].tech_ikkatsu   = OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_ikkatsu;
			nb6bushouref[iBushouID].tech_ansatsu   = OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_ansatsu;
			nb6bushouref[iBushouID].tech_ryusyutsu = OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_ryusyutsu;
			nb6bushouref[iBushouID].tech_sendou    = OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_sendou;
			nb6bushouref[iBushouID].tech_ryugen    = OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_ryugen;
			nb6bushouref[iBushouID].tech_benzetsu  = OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_benzetsu;
			nb6bushouref[iBushouID].tech_yakiuchi  = OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_yakiuchi; 
			nb6bushouref[iBushouID].tech_chohatsu  = OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_chohatsu; 

		}
	}
}


// 敵勢力下から、プレイヤ勢力下になったので、能力を元へと戻す。
// iIamBushou 噴出しの主、　iUreBushou 相手武将
void EXSuperService_IfCanAllAlibityInBattleSeiryokuChange(int iIamBushou, int iUreBushou) { //

	// 戦闘中にプレイヤ勢力の武将以外は全技能が利用可能である。がＯＦＦかどうか。
	if ( TSModCommand::OtherBushou::cmdCanUseAllAbilityInBattle == 0) {
		return;
	}

	// 自分か相手かどちらかが武将の範囲チェックを満たさなければ何もしない。
	if ( 0 <= iIamBushou && iIamBushou < GAMEDATASTRUCT_BUSHOU_NUM ) {
	} else {
		return;
	}
	if ( 0 <= iUreBushou && iUreBushou < GAMEDATASTRUCT_BUSHOU_NUM ) {
	} else {
		return;
	}

	// 裏切りの相手先武将がプレイヤ勢力かどうか
	if ( isPlayerBushou( iUreBushou ) ) {
		int iBushouID = iIamBushou;

		nb6bushouref[iBushouID].tech_ikkatsu   = OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_ikkatsu;
		nb6bushouref[iBushouID].tech_ansatsu   = OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_ansatsu;
		nb6bushouref[iBushouID].tech_ryusyutsu = OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_ryusyutsu;
		nb6bushouref[iBushouID].tech_sendou    = OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_sendou;
		nb6bushouref[iBushouID].tech_ryugen    = OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_ryugen;
		nb6bushouref[iBushouID].tech_benzetsu  = OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_benzetsu;
		nb6bushouref[iBushouID].tech_yakiuchi  = OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_yakiuchi; 
		nb6bushouref[iBushouID].tech_chohatsu  = OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_chohatsu; 
	}
}