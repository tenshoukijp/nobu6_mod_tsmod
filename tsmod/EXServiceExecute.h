#pragma once

#include "EXBushou.h"
#include "EXBushou_LancelotTartare.h"
#include "EXBushou_ShuiHidechiyo.h"


// プレイヤーのターンが切り替わった(複数大名担当)
void EXSuperService_OnPlayerTurnChange();

// プレイヤーのターンが切り替わりかつ、季節も変わった
void EXSuperService_OnSeasonChangeAndPlayerTurnChange();

// IntersectRectMessage() という比較的短いタイミングで更新チェックを行う特殊サービス
void EXSuperService_OnIntersectRectMessage();

// メインゲーム画面が始まった時
void EXSuperService_OnMainGameStart();


// 毎季節、特別に金や米が加算される。
void EXSuperService_CheckOtherDaimyosGetMoneyAndRiseOnSeasonChange();

// 毎季節、プレイヤ勢力の進度や全体の進度に合わせて、バランスが取られる。
void EXSuperService_PlayerDaimyoDataInfluenceOtherDaimyoData();

// 特産品に関連した項目が自動的に増える(鉄砲・馬・国際港)
void EXSuperService_CheckIncreaseSpecialProductByTheThing();

// 全ての武将の兵士が自動回復する。ただしプレイヤーが操作する軍団配下は除外
void EXSuperService_Regeneration(int isForce = 0);

// 勲功に関する特殊なフィルターサービス
void EXSuperService_FilterMerits();

// 季節が変わる度に４等級以下の剣家宝について、剣を持ち合わせていない剣豪武将に、ごく低い確率で渡るようにする。
void EXServieExecute_NPCKengouBushouHasAKahouSword();

// 鉄砲が得意な人が鍛冶村を作る処理
void EXServiceExecute_TeppouSpMakeSmith();

// 海賊が国際港を作る処理
void EXServiceExecute_PirateMakePort();

// 武将の勇猛を強制的に一定の値にする処理
void EXSuperService_IfBravesIsTheValue();

// 武将の寿命を強制的に一定の値にする処理
void EXSuperService_IfLifesIsTheValue();

// 武将が全員弓が使えなかったら
void EXSuperService_IfYumiNoExists();

// 武将が歳を取るのが遅かったら
void EXSuperService_IfLifesIsTheTimes();

// 武将の義理を最低値に、独立を最大値にする
void EXSuperService_IfNoDutiesAndMaxIndependences();

// 相場変更変更時に行うサービス
void EXSuperService_OnSobaChange();

// 季節の変わり目で暗殺カウントがリセットされる。
void EXSuperService_AnsatsuCntReset();

// セーブデータに持たせている、季節の変わり目にフィルターのための累積の熟練度が0になる。
void EXSuperService_ExperienceLimitReset();

// 武将の技術が未開発であった場合の処理
void EXSuperService_IfAbilityIsUnderdevelopedOnSelectingScenarioDaimyo();
void EXSuperService_IfAbilityIsUnderdevelopedOnAlwaysMainGame();

// 武将の熟練度をＭＡＸ固定であった場合の処理
void EXSuperService_IfFixExperienceMaxOnChangeTiming();
void EXSuperService_IfFixExperienceMaxOnAlwaysMainGame();

// 相場変更時に、熟練度自動鍛錬する。(技能も覚える)
void EXSuperService_AutoUpExperienceAndTechOnSobaChange();

// 軍団長との相性によって毎季節忠誠度を上下させる
void EXSuperService_AishoCalc();

// シナリオ説明文の後で、シナリオ大名選択時の前。ブラックアウト中
void EXSuperService_OnSelectingScenarioAfter();

// シナリオ大名選択時の処理
void EXSuperService_OnSelectingScenarioDaimyo();
// EXServiceExecuteRegeneration_All_Regeneration()をする前にする必要がある事前計算。
void EXServiceExecuteRegeneration_All_SoldierCalcInMain();

// 敵武将なら、戦闘中全ての技能が使える。
void EXSuperService_IfCanAllAlibityInBattleStart();
void EXSuperService_IfCanAllAlibityInBattleEnd();
void EXSuperService_IfCanAllAlibityInBattleSeiryokuChange(int iIamBushouID, int iUreBushouID );


// 戦争を見ていない場合に、熟練値が飛躍的に上昇するため、比較用に戦闘前に保存
void EXServiceExecuteBushouValueControllerOfStartBattle();

// 戦争を見ていない場合に、熟練値が飛躍的に上昇してる場合、抑える
void EXServiceExecuteBushouValueControllerOfEndBattle();


// シナリオエディタのバグ対応。軍団役職に15を代入しているというバグ
void patchBugOfGundanYakusyokuInSenarioEditor();



// 鉄砲イベントのコントローラー
void CheckTeppouEventControll();


// 天翔記以降の武将のパラメタを反映する。
void SetBushouAvgParam(int iFlagFalue);

// 浪人の勲功が毎季節にダウンする
void EXSuperService_RoninKunkouDecrement();


