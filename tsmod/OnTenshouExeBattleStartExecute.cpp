#include "GameDataStruct.h"
#include "ScenarioMod.h"


戦争開始パラメタ war_start_param;


// 戦争が発生した時に、仕掛けられた城番号と、仕掛けた軍団IDが渡ってくる。
// @iRecieveCastleID : 攻められた城の城ID
// @iRecieveGundanID : 攻められた側の軍団のID
// @iRecieveBushouID : 攻められた側の軍団長の武将ID
// @iAttackGundanID  : 攻めた側の軍団ID
// @iAttackBushouID  : 攻めた側の軍団長の武将ID
void OnTenshouExeBattleStartExecute(int iRecieveCastleID ) {

	// 以下よく使う番号の準備

	// 攻められた城が範囲外ならダメ
	if ( ! (0 <= iRecieveCastleID && iRecieveCastleID < GAMEDATASTRUCT_CASTLE_NUM) ) {
		return;
	}

	// 攻められた側の軍団ＩＤを得る
	int iRecieveGundanID = nb6castle[iRecieveCastleID].attach-1;

	// 攻められた側の軍団長の武将ＩＤを得る
	int iRecieveBushouID = nb6gundan[iRecieveGundanID].leader-1;

	int iAttackGundanID = nb6turn.gundan-1;
	// ターン軍団が無いとかありえないはずだが一応…
	if ( ! (0 <= iAttackGundanID && iAttackGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) ) {
		return;
	}

	// 攻めた側の軍団長の武将ＩＤを得る
	int iAttackBushouID = nb6gundan[iAttackGundanID].leader-1;

	
	// ScenarioModの戦争開始時に渡すため。
	war_start_param.城番号 = iRecieveCastleID+1;				// 標的の城番号
	war_start_param.守備側軍団_軍団番号 = iRecieveGundanID + 1;		// 守備側の軍団番号
	war_start_param.守備側軍団長_武将番号 = iRecieveBushouID + 1;
	war_start_param.攻撃側軍団_軍団番号  = iAttackGundanID + 1;		// 守備側の軍団番号
	war_start_param.攻撃側軍団長_武将番号  = iAttackBushouID + 1;

	// 将来ここで何か。
	// OutputDebugStream("%s, %s, %s", nb6castle[iRecieveCastleID].name, nb6bushouname[iRecieveBushouID].fastname, nb6bushouname[iAttackBushouID].fastname );

	// ScenarioModの戦争開始時のメソッド呼出し。
	if ( p_snOnBattleStart ) {
		((PFNSNONBATTLESTART) p_snOnBattleStart)(&war_start_param);
	}
}
