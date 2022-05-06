#include "WinTarget.h"
#include <iostream>

#include "CommonGlobalHandle.h"
#include "EXServiceExecute.h"
#include "GameDataStruct.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "TSModCommand.h"
#include "TSMod.h"
#include "PluginMod.h"
#include "FuncImageDialog.h"
#include "EXBushou_All.h"
#include "DijkstraCastle.h"

extern void setJobEXOfCurAllBushou();
extern void EXBushou_All_CheckLifeTimes();
extern bool CheckChangeYearDisplay(); // 元号表示をやめて強制的に西暦表示に切り替える
extern void EXServieExecute_TenShougiCheck(); // 天将棋をプレイしていなければ、確率で天将棋の処理


// プレイヤーのターンが切り替わった(複数大名担当)
void EXSuperService_OnPlayerTurnChange() {

	// ゲームデータをプレイヤーターンが変わった段階で読み込み更新しておく
	// この処理はこの関数の最初に必ず行うこと!!
	copyAllBushouSoldiersOriginToCopy();

	// シナリオModのプレイヤターン関数を呼び出す
	if ( p_snOnPlayerTurnInMainScreen ) {
		((PFNSNPLAYERTURNINMAINSCREEN) p_snOnPlayerTurnInMainScreen)();
	}
}



// プレイヤーのターンが切り替わりかつ、季節も変わった
// プレイヤーのターンが切り替わった処理(EXSuperService_OnPlayerTurnChange)は通った後にここに来る。
void EXSuperService_OnSeasonChangeAndPlayerTurnChange() {

	// 鍛冶村を鉄砲得意な人が作る処理
	EXServiceExecute_TeppouSpMakeSmith();

	// 国際港を海賊が作る処理
	EXServiceExecute_PirateMakePort();

	// 剣豪武将に刀剣を意図的に持たせる処理
	EXServieExecute_NPCKengouBushouHasAKahouSword();

	// 天将棋の処理
	EXServieExecute_TenShougiCheck();
}



// メインゲーム画面が始まった時
void EXSuperService_OnMainGameStart() {

	// 元号表示を西暦表示にする必要があるかチェック(1454年を変更している場合)
	CheckChangeYearDisplay();

	// 武将の拡張職業を設定する
	setJobEXOfCurAllBushou();

	// 鉄砲関連のイベントのコントロール
	CheckTeppouEventControll();

	// 軍団番号がバグっていたら、直す。(※シナリオエディタが軍団の役職番号を15と設定しているバグを直す。
	patchBugOfGundanYakusyokuInSenarioEditor();

	// 武将の熟練度がＭＡＸ固定であった場合の処理
	EXSuperService_IfFixExperienceMaxOnChangeTiming();

	// 武将の勇猛を設定する
	EXSuperService_IfBravesIsTheValue();

	// 武将の寿命を設定する
	EXSuperService_IfLifesIsTheValue();

	// 武将の義理を最低に、独立を最大にする。
	EXSuperService_IfNoDutiesAndMaxIndependences();

	// 兵士数を増やすまえの事前計算(EXSuperService_Regenerationより前にあること!!)
	EXServiceExecuteRegeneration_All_SoldierCalcInMain();

	// ゲームスタート時に、兵士自動回復処理をチェック。兵士が０→１にするためisForceとして１を渡す。
	EXSuperService_Regeneration(1);

	// PluginMod系の０個以上（複数可能）なＤＬＬのメインゲーム開始時のメソッドを呼び出す。
	// 実行対象となっているDLLを順に捜査して…
	for ( vector<HMODULE>::iterator it = listOfTargetDLL.begin(); it != listOfTargetDLL.end(); it++ ) {

		// 対象のDLLにMainGameStart関数が存在するかをチェック。
		PFNTARGETMODMAINGAMESTART p_MainGameStart = (PFNTARGETMODMAINGAMESTART)GetProcAddress(*it, "OnMainGameStart");

		if ( !p_MainGameStart ) {
			continue;
		}

		((PFNTARGETMODMAINGAMESTART) p_MainGameStart)();
	}

	// ScenarioMod内にあるメインゲーム開始時メソッドを呼び出す。
	if ( p_snOnMainGameStart ) {
		( (PFNSNONMAINGAMESTART ) p_snOnMainGameStart)();
	}


	// 城と経路の情報構築はここ。 
	InitCastleNodeAndEdge();
}


extern int iEnterOfClickMainScreenMainCommand;

// 相場が変更となった時
void EXSuperService_OnSobaChange() {

	iEnterOfClickMainScreenMainCommand = 0;

	// 相場が変わった段階でもやっておく必要がある。
	// プレイヤーターンが回ってくる間でも状況が変わるので、２回やる必要がある。
	copyAllBushouSoldiersOriginToCopy();

	// 季節の変わり目で暗殺カウントがリセットされる。
	EXSuperService_AnsatsuCntReset();

	// 季節の変わり目で熟練値の累積がリセットされる。
	EXSuperService_ExperienceLimitReset();

	// 軍団長と相性が良い武将は忠誠度が上がり、悪い武将は忠誠度が下がる
	EXSuperService_AishoCalc();

	// 相場変更時に、熟練度自動鍛錬する。(技能も覚える)
	EXSuperService_AutoUpExperienceAndTechOnSobaChange();

	// 毎季節浪人の勲功が落ちてゆく
	EXSuperService_RoninKunkouDecrement();

	// 武将の熟練度がＭＡＸ固定であった場合の処理
	EXSuperService_IfFixExperienceMaxOnChangeTiming();

	// すべての武将の勇猛を特定の値に出来る。登場武将も同様。
	EXSuperService_IfBravesIsTheValue();

	// すべての武将の寿命を特定の値に出来る。登場武将も同様。
	EXSuperService_IfLifesIsTheValue();

	// すべての武将の義理を最小に、独立を最大にする。登場武将も同様。
	EXSuperService_IfNoDutiesAndMaxIndependences();

	// もしも弓が存在しなかったらということ
	EXSuperService_IfYumiNoExists();

	// 毎季節、金と米を余分に調整
	EXSuperService_CheckOtherDaimyosGetMoneyAndRiseOnSeasonChange();

	// 鍛冶・馬産・国際港などの情報に従って、それぞれの項目を増やす
	EXSuperService_CheckIncreaseSpecialProductByTheThing();

	// プレイヤや全体のバランスに従って各勢力の資金・兵力に調整が入る
	EXSuperService_PlayerDaimyoDataInfluenceOtherDaimyoData();
	
	// 兵士数を増やすまえの事前計算(EXSuperService_Regenerationより前にあること!!)
	EXServiceExecuteRegeneration_All_SoldierCalcInMain();

	// 相場変更時に、兵士自動回復処理をチェック。兵士が０→１にするためisForceとして１を渡す。
	EXSuperService_Regeneration(1);
}

// シナリオ大名選択時の特殊処理を行う。
void EXSuperService_OnSelectingScenarioAfter() {
	// 武将の技術が未開発であった場合の処理。１回だけ今いる武将の技能を剥奪
	EXSuperService_IfAbilityIsUnderdevelopedOnSelectingScenarioDaimyo();

	// 武将の熟練度がＭＡＸ固定であった場合の処理
	EXSuperService_IfFixExperienceMaxOnChangeTiming();

	// シナリオエディターが持っている、｢軍団番号｣が15番となっているバグを直す。
	patchBugOfGundanYakusyokuInSenarioEditor();

	// 鉄砲伝来イベントコントローラー
	CheckTeppouEventControll();

	// 天翔記以降の武将のパラメタを反映する。
	SetBushouAvgParam(1); // 1はシナリオ選択時
	SetExBushouAvgParam(-1); // -1は全員に対して行えということ。特殊武将用

	// もしもScenarioMod.dllで、snOnSelectingScenarioDaimyo(void *)メソッドを持っているならば…
	if (p_snOnSelectingScenarioAfter) {
		// とりあえず一旦NULLいれておく。
		((PFNSNONSELECTINGSCENARIOAFTER)p_snOnSelectingScenarioAfter)(NULL);
	}


}


// シナリオ大名選択時の処理
void EXSuperService_OnSelectingScenarioDaimyo() {

	// もしもPluginMod.dllで、OnTenshouExeSelectingScenarioDaimyoExecute()メソッドを持っているならば…
	// 実行対象となっているDLLを順に捜査して…
	for ( vector<HMODULE>::iterator it = listOfTargetDLL.begin(); it != listOfTargetDLL.end(); it++ ) {

		// 対象のDLLにOnSelectingScenarioDaimyoStart関数が存在するかをチェック。
		PFNTARGETMODSELECTINGSCENARIODAIMYOSTART p_SelectingScenarioDaimyoStart = (PFNTARGETMODSELECTINGSCENARIODAIMYOSTART)GetProcAddress(*it, "OnSelectingScenarioDaimyoStart");

		if ( !p_SelectingScenarioDaimyoStart ) {
			continue;
		}

		((PFNTARGETMODSELECTINGSCENARIODAIMYOSTART) p_SelectingScenarioDaimyoStart)();
	}

	// もしもScenarioMod.dllで、snOnSelectingScenarioDaimyo(void *)メソッドを持っているならば…
	if ( p_snOnSelectingScenarioDaimyo ) {
		// とりあえず一旦NULLいれておく。
		((PFNSNONSELECTINGSCENARIODAIMYO) p_snOnSelectingScenarioDaimyo)( NULL );
	}


}