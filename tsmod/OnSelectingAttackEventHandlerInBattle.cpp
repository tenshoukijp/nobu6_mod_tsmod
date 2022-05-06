#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"

// 攻撃が実行される直前に行動が選択された時
// 
namespace SelectingBattleAction {
	// Tenshou.exe や TSMod内での判定
	enum {
		攻撃 = 0,
		突撃 = 1, 騎馬突撃 = 1,
		鉄砲 = 2, 弓 = 2,
		大砲 = 3,
		一斉攻撃 = 4,
	};
}


// 重要なイベントハンドラ!! 操作キャラや委任キャラ(NPC含む)が、行動を実行した時に選択される。
// 混乱など「自分の意思でない」時には、実行されない。
void OnSelectingAttackEventHandlerInBattle(int iBushouID, int iTargetID, int iSelectActionID, BOOL IsMoveBefore) {

	if ( p_snOnSelectingBattleAction ) {

		攻撃方法決定時パラメタ param;
		param.移動前 = IsMoveBefore;
		param.攻撃方法番号 = iSelectActionID;

		// TSMODでの弓番号→SCENARIOMODでの弓番号。
		p_snOnSelectingBattleAction(iBushouID, iTargetID, iSelectActionID, &param);
	}
}