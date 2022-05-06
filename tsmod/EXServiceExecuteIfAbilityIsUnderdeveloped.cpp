#include "WinTarget.h"
#include <fstream>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"



/*
 * TSMod.txtで指定すると、デフォでは全ての武将が技能を持たない。その後取得してゆくことは出来る。
 */

// 武将の技術が未開発であった場合の処理。１回だけ今いる武将の技能を剥奪
void EXSuperService_IfAbilityIsUnderdevelopedOnSelectingScenarioDaimyo() {

	// 技能が未開発をＯＮにするコマンドを有効としていなければ何もしない。
	if ( ! TSModCommand::AllBushou::cmdAbilityIsUnderdeveloped ) {
		return;
	}

	for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

		// もし、技能を１つでも修得しているならば…
		if ( nb6bushouref[iBushouID].tech_ikkatsu    ||
			 nb6bushouref[iBushouID].tech_ansatsu    ||
			 nb6bushouref[iBushouID].tech_ryusyutsu  ||
			 nb6bushouref[iBushouID].tech_sendou	 ||
			 nb6bushouref[iBushouID].tech_ryugen	 ||
			 nb6bushouref[iBushouID].tech_benzetsu   ||
			 nb6bushouref[iBushouID].tech_yakiuchi   ||
			 nb6bushouref[iBushouID].tech_chohatsu ) {

			nb6bushouref[iBushouID].tech_ikkatsu = 0; 
			nb6bushouref[iBushouID].tech_ansatsu = 0;  
			nb6bushouref[iBushouID].tech_ryusyutsu = 0;
			nb6bushouref[iBushouID].tech_sendou = 0;	
			nb6bushouref[iBushouID].tech_ryugen = 0;	
			nb6bushouref[iBushouID].tech_benzetsu = 0; 
			nb6bushouref[iBushouID].tech_yakiuchi = 0; 
			nb6bushouref[iBushouID].tech_chohatsu = 0; 
		}
	}
}





// 武将の技術が未開発であった場合の処理。
// ゲームスタート以後は常に処理する。
// 列伝IDが変化していたら、中の人が変化したということ(新たな武将が登場したということ)で、技能を除去する。
void EXSuperService_IfAbilityIsUnderdevelopedOnAlwaysMainGame(int iBushouID) {

	// 技能が未開発をＯＮにするコマンドを有効としていなければ何もしない。
	if ( ! TSModCommand::AllBushou::cmdAbilityIsUnderdeveloped ) {
		return;
	}

	// まだメインゲームまわってなければ、実行しない
	if ( !CommonGlobalHandle::isMainGameStart ) {
		return;
	}


	// 新しい人が…
	// もし、技能を１つでも修得しているならば…

	// 全部技能を消す
	nb6bushouref[iBushouID].tech_ikkatsu = 0; 
	nb6bushouref[iBushouID].tech_ansatsu = 0;  
	nb6bushouref[iBushouID].tech_ryusyutsu = 0;
	nb6bushouref[iBushouID].tech_sendou = 0;	
	nb6bushouref[iBushouID].tech_ryugen = 0;	
	nb6bushouref[iBushouID].tech_benzetsu = 0; 
	nb6bushouref[iBushouID].tech_yakiuchi = 0; 
	nb6bushouref[iBushouID].tech_chohatsu = 0; 
}