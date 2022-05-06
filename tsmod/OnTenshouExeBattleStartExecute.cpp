#include "GameDataStruct.h"
#include "ScenarioMod.h"


í‘ˆŠJnƒpƒ‰ƒƒ^ war_start_param;


// í‘ˆ‚ª”­¶‚µ‚½‚ÉAdŠ|‚¯‚ç‚ê‚½é”Ô†‚ÆAdŠ|‚¯‚½ŒR’cID‚ª“n‚Á‚Ä‚­‚éB
// @iRecieveCastleID : U‚ß‚ç‚ê‚½é‚ÌéID
// @iRecieveGundanID : U‚ß‚ç‚ê‚½‘¤‚ÌŒR’c‚ÌID
// @iRecieveBushouID : U‚ß‚ç‚ê‚½‘¤‚ÌŒR’c’·‚Ì•«ID
// @iAttackGundanID  : U‚ß‚½‘¤‚ÌŒR’cID
// @iAttackBushouID  : U‚ß‚½‘¤‚ÌŒR’c’·‚Ì•«ID
void OnTenshouExeBattleStartExecute(int iRecieveCastleID ) {

	// ˆÈ‰º‚æ‚­g‚¤”Ô†‚Ì€”õ

	// U‚ß‚ç‚ê‚½é‚ª”ÍˆÍŠO‚È‚çƒ_ƒ
	if ( ! (0 <= iRecieveCastleID && iRecieveCastleID < GAMEDATASTRUCT_CASTLE_NUM) ) {
		return;
	}

	// U‚ß‚ç‚ê‚½‘¤‚ÌŒR’c‚h‚c‚ğ“¾‚é
	int iRecieveGundanID = nb6castle[iRecieveCastleID].attach-1;

	// U‚ß‚ç‚ê‚½‘¤‚ÌŒR’c’·‚Ì•«‚h‚c‚ğ“¾‚é
	int iRecieveBushouID = nb6gundan[iRecieveGundanID].leader-1;

	int iAttackGundanID = nb6turn.gundan-1;
	// ƒ^[ƒ“ŒR’c‚ª–³‚¢‚Æ‚©‚ ‚è‚¦‚È‚¢‚Í‚¸‚¾‚ªˆê‰c
	if ( ! (0 <= iAttackGundanID && iAttackGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) ) {
		return;
	}

	// U‚ß‚½‘¤‚ÌŒR’c’·‚Ì•«‚h‚c‚ğ“¾‚é
	int iAttackBushouID = nb6gundan[iAttackGundanID].leader-1;

	
	// ScenarioMod‚Ìí‘ˆŠJn‚É“n‚·‚½‚ßB
	war_start_param.é”Ô† = iRecieveCastleID+1;				// •W“I‚Ìé”Ô†
	war_start_param.ç”õ‘¤ŒR’c_ŒR’c”Ô† = iRecieveGundanID + 1;		// ç”õ‘¤‚ÌŒR’c”Ô†
	war_start_param.ç”õ‘¤ŒR’c’·_•«”Ô† = iRecieveBushouID + 1;
	war_start_param.UŒ‚‘¤ŒR’c_ŒR’c”Ô†  = iAttackGundanID + 1;		// ç”õ‘¤‚ÌŒR’c”Ô†
	war_start_param.UŒ‚‘¤ŒR’c’·_•«”Ô†  = iAttackBushouID + 1;

	// «—ˆ‚±‚±‚Å‰½‚©B
	// OutputDebugStream("%s, %s, %s", nb6castle[iRecieveCastleID].name, nb6bushouname[iRecieveBushouID].fastname, nb6bushouname[iAttackBushouID].fastname );

	// ScenarioMod‚Ìí‘ˆŠJn‚Ìƒƒ\ƒbƒhŒÄo‚µB
	if ( p_snOnBattleStart ) {
		((PFNSNONBATTLESTART) p_snOnBattleStart)(&war_start_param);
	}
}
