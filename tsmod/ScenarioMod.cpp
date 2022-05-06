#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"


#include "TSMod.h"
#include "GameDataStruct.h"
#include "menu.h"


// ScenarioMod.dllハンドル
HINSTANCE h_scenario = NULL;

// snConstructor
PFNSNONCONSTRUCTOR p_snOnConstructor;
// snDestructor
PFNSNONDESTRUCTOR p_snOnDestructor;
// snDllMainBgn
PFNSNONDLLMAINBGN p_snOnDllMainBgn;
// snDllMainEnd
PFNSNONDLLMAINEND p_snOnDllMainEnd;

PFNSNONTAIKIBUSHOUDATATRANSFER p_snOnTaikiBushouDataTransfer;

// snOnAfterChangeWindowSize
PFNSNONAFTERCHANGEWINDOWSIZE p_snOnAfterChangeWindowSize;
// snOnViewingTopMenu
PFNSNONVIEWINGTOPMENU p_snOnViewingTopMenu;

// snOnWritingScenarioDetail
PFNSNWRITINGSCENARIODETAIL p_snOnWritingScenarioDetail;

// snOnSelectingScenarioAfter
PFNSNONSELECTINGSCENARIOAFTER p_snOnSelectingScenarioAfter;
// snOnSelectingScenarioDaimyo
PFNSNONSELECTINGSCENARIODAIMYO p_snOnSelectingScenarioDaimyo;
// snOnRewriteFukidashiMessage
PFNSNONREWRITEFUKIDASHIMESSAGE p_snOnRewriteFukidashiMessage;
// p_snOnDecisionInputNumberDialog
PFNSNONDECISIONINPUTNUMBERDIALOG p_snOnDecisionInputNumberDialog;
// p_snOnRatificationDialog
PFNSNONRATIFICATIONDIALOG p_snOnRatificationDialog;
// snOnCustomCondition
PFNSNONCUSTOMCONDITION p_snOnCustomCondition;
// snOnPlayerTurnInMainScreen
PFNSNPLAYERTURNINMAINSCREEN p_snOnPlayerTurnInMainScreen;
// snOnPlayerTurnInMainScreen
PFNSNGUNDANTURNINMAINSCREEN p_snOnGundanTurnInMainScreen;
// snOnViewingBattleMenuInBattle
PFNSNONVIEWINGBATTLEMENUINBATTLE p_snOnViewingBattleMenuInBattle;
// snOnViewingCommandMenuInBattle
PFNSNONVIEWINGCOMMANDMENUINBATTLE p_snOnViewingCommandMenuInBattle;
// snSetCurrentCastleIDInBattle
PFNSNSETCURRENTCASTLEIDINBATTLE p_snSetCurrentCastleIDInBattle;
// snOnN6PAudioPlayTrack
PFNSNONN6PAUDIOPLAYTRACK p_snOnN6PAudioPlayTrack;
// snOnChangeTurnInBattle
PFNSNONCHANGETURNINBATTLE p_snOnChangeTurnInBattle;
// p_snOnCalcWeatherInBattle
PFNSNONCALCWEATHERINBATTLE p_snOnCalcWeatherInBattle;
// snOnChangeCastleTurnInBattle
PFNSNONCHANGECASTLETURNINBATTLE p_snOnChangeCastleTurnInBattle;
// snOnBattleStart
PFNSNONBATTLESTART p_snOnBattleStart;
// snOnBattleEnd
PFNSNONBATTLEEND p_snOnBattleEnd;
// snOnBattleScreenEnd
PFNSNONBATTLESCREENEND p_snOnBattleScreenEnd;
// snOnChangeSoba
PFNSNONCHANGESOBA p_snOnChangeSoba;
// snOnMainGameStart
PFNSNONMAINGAMESTART p_snOnMainGameStart;

// p_snOnEnterMainPlayerCommand
PFNSNONENTERPLAYERCOMMAND p_snOnEnterMainPlayerCommand;
// p_snOnLeaveMainPlayerCommand
PFNSNONLEAVEPLAYERCOMMAND p_snOnLeaveMainPlayerCommand;

// p_snOnBornGeneralBushou
PFNSNONBORNGENERALBUSHOU p_snOnBornGeneralBushou;
// p_snOnBornRekishiBushou
PFNSNONBORNGENERALBUSHOU p_snOnBornRekishiBushou;
// p_snOnSkippedRekishiBushou
PFNSNONSKIPPEDGENERALBUSHOU p_snOnSkippedRekishiBushou;

// snOnViewingJobName
PFNSNONVIEWINGJOBNAME p_snOnViewingJobName;
// snOnViewingSpecialFormName
PFNSNONVIEWINGSPECIALFORMNAME p_snOnViewingSpecialFormName;
// snOnViewingKahouTypeName
PFNSNONVIEWINGKAHOUTYPENAME p_snOnViewingKahouTypeName;
// p_snOnViewingKahouTypeName
PFNSNONVIEWINGBUTAINAME p_snOnViewingButaiName;
// p_snOnGettingButaimeiGobiSprintf
PFNSNONGETTINGBUTAIMEIGOBISPRINTF p_snOnGettingButaimeiGobiSprintf;
// p_snOnViewingPositionName
PFNSNONVIEWINGPOSITIONNAME p_snOnViewingPositionName;
// p_snOnViewingBushouStateName
PFNSNONVIEWINGBUSHOUSTATENAME p_snOnViewingBushouStateName;
// p_snOnViewingIe
PFNSNONVIEWINGIE p_snOnViewingIe;
// p_snOnViewingRetsuden
PFNSNONVIEWINGRETSUDEN p_snOnViewingRetsuden;
// p_snOnViewingMeibutsuJiten
PFNSNONVIEWINGMEIBUTSUJITEN p_snOnViewingMeibutsuJiten;
// p_snOnViewingMeijoMonogatari
PFNSNONVIEWINGMEIJOMONOGATARI p_snOnViewingMeijoMonogatari;


// p_snOnSelectingBattleAction
PFNSNONSELECTINGBATTLEACTION p_snOnSelectingBattleAction;
// p_snOnBattleDamaged
PFNSNONBATTLEDAMAGED p_snOnBattleDamaged;

// p_snOnChangeUnitMovePowerInBattle
PFNSNONCHANTEUNITMOVEPOWERININBATTLE p_snOnChangeUnitMovePowerInBattle;

// p_snOnRequestSoldierMax
PFNSNONREQUESTSOLDIERMAX p_snOnRequestSoldierMax;

// snOnViewingScenarioTitleList
PFNSNVIEWINGSCENARIOTITLELIST p_snOnViewingScenarioTitleList;
// snOnTick
PFNSNONTICK p_snOnTick;

PFNSNONENDINGCONDITION p_snOnEndingCondition;

// snOnSaveSaveData
PFNSNSAVESAVEDATA p_snOnSaveSaveData;
// snOnLoadSaveData
PFNSNLOADSAVEDATA p_snOnLoadSaveData;

// p_snOnApplicationSelectMenuItem
PFNSNONAPPLICATIONSELECTMENUITEM p_snOnApplicationSelectMenuItem;


BOOL LoadLibraryOfScenarioMod() {
	// ScenarioMod.dllを読みにかかる。基本的には無いだろう。
	h_scenario = LoadLibrary( "ScenarioMod.dll" );

	if ( h_scenario != NULL ) {

		AddModuleItem( h_scenario, "scenariomod.dll" ); // 全て小文字で
		
		// コンストラクタ
		p_snOnConstructor = (PFNSNONCONSTRUCTOR)GetProcAddress( h_scenario, "snOnConstructor" );

		// デストラクタ
		p_snOnDestructor = (PFNSNONDESTRUCTOR)GetProcAddress( h_scenario, "snOnDestructor" );

		// 待機武将のデータのまるまるの転送
		p_snOnTaikiBushouDataTransfer = (PFNSNONTAIKIBUSHOUDATATRANSFER)GetProcAddress(h_scenario, "snOnTaikiBushouDataTransfer");

		// DLLMain開始直後
		p_snOnDllMainBgn = (PFNSNONCONSTRUCTOR)GetProcAddress( h_scenario, "snOnDllMainBgn" );

		// DLLMain終了直前
		p_snOnDllMainEnd = (PFNSNONDESTRUCTOR)GetProcAddress( h_scenario, "snOnDllMainEnd" );

		// snOnViewingTopMenu
		p_snOnAfterChangeWindowSize = (PFNSNONAFTERCHANGEWINDOWSIZE)GetProcAddress( h_scenario, "snOnAfterChangeWindowSize" );

		// snOnViewingTopMenu
		p_snOnViewingTopMenu = (PFNSNONVIEWINGTOPMENU)GetProcAddress( h_scenario, "snOnViewingTopMenu" );

		// ６つのシナリオタイトルを表示する直前
		p_snOnViewingScenarioTitleList = (PFNSNVIEWINGSCENARIOTITLELIST)GetProcAddress( h_scenario, "snOnViewingScenarioTitleList" );
		if ( ! p_snOnViewingScenarioTitleList ) {
			p_snOnViewingScenarioTitleList = (PFNSNVIEWINGSCENARIOTITLELIST)GetProcAddress( h_scenario, "snViewingScenarioTitleList" ); // or になっているのは古いミス。その数年経過したら消す
		}

		// シナリオ詳細を表示する時
		p_snOnWritingScenarioDetail = (PFNSNWRITINGSCENARIODETAIL)GetProcAddress( h_scenario, "snOnWritingScenarioDetail" );

		// シナリオ詳細で「確認」ボタンを押した後、シナリオデータ配置後
		p_snOnSelectingScenarioAfter = (PFNSNONSELECTINGSCENARIOAFTER)GetProcAddress(h_scenario, "snOnSelectingScenarioAfter");

		// シナリオデータの引き出し用メソッドのポインタ確保
		p_snOnSelectingScenarioDaimyo = (PFNSNONSELECTINGSCENARIODAIMYO)GetProcAddress( h_scenario, "snOnSelectingScenarioDaimyo" );

		// メインゲームスタート時に、呼び出すメソッド。
		p_snOnMainGameStart = (PFNSNONMAINGAMESTART)GetProcAddress( h_scenario, "snOnMainGameStart" );

		// プレイヤの特定のコマンド実行の侵入時
		p_snOnEnterMainPlayerCommand = (PFNSNONENTERPLAYERCOMMAND)GetProcAddress( h_scenario, "snOnEnterMainPlayerCommand" );

		// プレイヤの特定のコマンド実行の脱出時
		p_snOnLeaveMainPlayerCommand = (PFNSNONLEAVEPLAYERCOMMAND)GetProcAddress( h_scenario, "snOnLeaveMainPlayerCommand" );

		// 噴出しメッセージを書き換える用のメソッドのポインタ確保
		p_snOnRewriteFukidashiMessage = (PFNSNONREWRITEFUKIDASHIMESSAGE)GetProcAddress( h_scenario, "snOnRewriteFukidashiMessage" );

		// 数値入力ダイアログで数値を決定した時に実行
		p_snOnDecisionInputNumberDialog = (PFNSNONDECISIONINPUTNUMBERDIALOG)GetProcAddress( h_scenario, "snOnDecisionInputNumberDialog" );

		// 追認系ダイアログの確認時
		p_snOnRatificationDialog = (PFNSNONRATIFICATIONDIALOG)GetProcAddress( h_scenario, "snOnRatificationDialog" );

		// プレイヤターン時のメイン画面でのメソッドのポインタ確保
		p_snOnPlayerTurnInMainScreen = (PFNSNPLAYERTURNINMAINSCREEN)GetProcAddress( h_scenario, "snOnPlayerTurnInMainScreen" );

		// 軍団ターン時のメイン画面でのメソッドのポインタ確保
		p_snOnGundanTurnInMainScreen = (PFNSNGUNDANTURNINMAINSCREEN)GetProcAddress( h_scenario, "snOnGundanTurnInMainScreen" );

		// カスタムコンディション用。基本はTRUE/FALSEだが、一応INT型で値を返せるようにしている。
		p_snOnCustomCondition = (PFNSNONCUSTOMCONDITION)GetProcAddress( h_scenario, "snOnCustomCondition" );

		// 戦闘コマンドを表示する直前
		p_snOnViewingBattleMenuInBattle = (PFNSNONVIEWINGBATTLEMENUINBATTLE)GetProcAddress( h_scenario, "snOnViewingBattleMenuInBattle5" );

		// 基本コマンドを表示する直前
		p_snOnViewingCommandMenuInBattle = (PFNSNONVIEWINGCOMMANDMENUINBATTLE)GetProcAddress( h_scenario, "snOnViewingCommandMenuInBattle12" );

		// 戦闘行動決定時
		p_snOnSelectingBattleAction = (PFNSNONSELECTINGBATTLEACTION)GetProcAddress( h_scenario, "snOnSelectingBattleAction" );

		// ダメージ直後
		p_snOnBattleDamaged = (PFNSNONBATTLEDAMAGED)GetProcAddress( h_scenario, "snOnBattleDamaged" );

		// 移動力
		p_snOnChangeUnitMovePowerInBattle = (PFNSNONCHANTEUNITMOVEPOWERININBATTLE)GetProcAddress( h_scenario, "snOnChangeUnitMovePowerInBattle" );

		// この直後、野戦か城戦かを判断するために、城番号を渡す。0xFFFFなら野戦
		p_snSetCurrentCastleIDInBattle = (PFNSNSETCURRENTCASTLEIDINBATTLE)GetProcAddress( h_scenario, "snSetCurrentCastleIDInBattle" );

		// トラックの再生時に呼びバス。再生直前に再生番号を変更するため。-1が返ってきたらそのままの番号を再生。
		p_snOnN6PAudioPlayTrack = (PFNSNONN6PAUDIOPLAYTRACK)GetProcAddress( h_scenario, "snOnN6PAudioPlayTrack" );

		// 戦闘が始まった時
		p_snOnBattleStart = (PFNSNONBATTLESTART)GetProcAddress( h_scenario, "snOnBattleStart" );

		// 戦闘中の経過ターンが変化した時に、呼び出すメソッド。
		p_snOnChangeTurnInBattle = (PFNSNONCHANGETURNINBATTLE)GetProcAddress( h_scenario, "snOnChangeTurnInBattle" );

		// 天気決定時
		p_snOnCalcWeatherInBattle = (PFNSNONCALCWEATHERINBATTLE)GetProcAddress( h_scenario, "snOnCalcWeatherInBattle" );

		// 攻城戦の経過ターンが変化した時に、呼び出すメソッド。
		p_snOnChangeCastleTurnInBattle = (PFNSNONCHANGECASTLETURNINBATTLE)GetProcAddress( h_scenario, "snOnChangeCastleTurnInBattle" );

		// 戦闘画面が終わった時(PC/NPC両方。NPCの場合は、画面に入ってなくても発動)
		p_snOnBattleScreenEnd = (PFNSNONBATTLESCREENEND)GetProcAddress( h_scenario, "snOnBattleScreenEnd" );

		// 戦闘が終わった時
		p_snOnBattleEnd = (PFNSNONBATTLEEND)GetProcAddress( h_scenario, "snOnBattleEnd" );

		// 相場が変化する直前に、呼び出すメソッド。
		p_snOnChangeSoba = (PFNSNONCHANGESOBA)GetProcAddress( h_scenario, "snOnChangeSoba" );

		// 待機武将が登場する直前
		p_snOnSkippedRekishiBushou = (PFNSNONSKIPPEDGENERALBUSHOU)GetProcAddress(h_scenario, "snOnSkippedRekishiBushou");

		// 待機の武将登場
		p_snOnBornRekishiBushou = (PFNSNONBORNREKISHIBUSHOU)GetProcAddress(h_scenario, "snOnBornRekishiBushou");

		// 汎用の武将誕生
		p_snOnBornGeneralBushou = (PFNSNONBORNGENERALBUSHOU)GetProcAddress(h_scenario, "snOnBornGeneralBushou");

		// 職業名を表示する直前に呼び出すメソッド。
		p_snOnViewingJobName = (PFNSNONVIEWINGJOBNAME)GetProcAddress( h_scenario, "snOnViewingJobName" );

		// 特殊兵態名を表示する直前に呼び出すメソッド。(騎馬鉄砲のところ)
		p_snOnViewingSpecialFormName = (PFNSNONVIEWINGSPECIALFORMNAME)GetProcAddress( h_scenario, "snOnViewingSpecialFormName" );

		// 家宝種類名を表示する直前に呼び出すメソッド。
		p_snOnViewingKahouTypeName = (PFNSNONVIEWINGKAHOUTYPENAME)GetProcAddress( h_scenario, "snOnViewingKahouTypeName" );

		// 兵態名を表示する直前に呼び出すメソッド。
		p_snOnViewingButaiName = (PFNSNONVIEWINGBUTAINAME)GetProcAddress( h_scenario, "snOnViewingButaiName" );

		// 兵態名を表示する直前に呼び出すメソッド。
		p_snOnGettingButaimeiGobiSprintf = (PFNSNONGETTINGBUTAIMEIGOBISPRINTF)GetProcAddress( h_scenario, "snOnGettingButaimeiGobiSprintf" );

		// 身分名を表示する直前に呼び出すメソッド。
		p_snOnViewingPositionName = (PFNSNONVIEWINGPOSITIONNAME)GetProcAddress( h_scenario, "snOnViewingPositionName" );

		// 武将の状態名を表示する直前に呼び出すメソッド。
		p_snOnViewingBushouStateName = (PFNSNONVIEWINGBUSHOUSTATENAME)GetProcAddress( h_scenario, "snOnViewingBushouStateName" );

		// 「家」が表示される直前に呼び出されるメソッド
		p_snOnViewingIe = (PFNSNONVIEWINGIE)GetProcAddress(h_scenario, "snOnViewingIe");

		// 武将列伝のタイトルの表示時
		p_snOnViewingRetsuden = (PFNSNONVIEWINGRETSUDEN)GetProcAddress( h_scenario, "snOnViewingRetsuden" );

		// 家宝事典のタイトルの表示時
		p_snOnViewingMeibutsuJiten = (PFNSNONVIEWINGMEIBUTSUJITEN)GetProcAddress( h_scenario, "snOnViewingMeibutsuJiten" );

		// 城列伝のタイトルの表示時
		p_snOnViewingMeijoMonogatari = (PFNSNONVIEWINGMEIJOMONOGATARI)GetProcAddress( h_scenario, "snOnViewingMeijoMonogatari" );

		// 対象武将の兵数の最大数を要求された時
		p_snOnRequestSoldierMax = (PFNSNONREQUESTSOLDIERMAX)GetProcAddress( h_scenario, "snOnRequestSoldierMax" );

		// セーブした時に呼び出すメソッド。
		p_snOnSaveSaveData = (PFNSNSAVESAVEDATA)GetProcAddress( h_scenario, "snOnSaveSaveData" );

		// ロードした時に呼び出すメソッド。
		p_snOnLoadSaveData = (PFNSNLOADSAVEDATA)GetProcAddress( h_scenario, "snOnLoadSaveData" );

		// アプリケーションのメニューを選択した時に呼び出すメソッド。
		p_snOnApplicationSelectMenuItem = (PFNSNONAPPLICATIONSELECTMENUITEM)GetProcAddress( h_scenario, "snOnApplicationSelectMenuItem" );

		// 常時。１秒に5回目安。
		p_snOnTick = (PFNSNONTICK)GetProcAddress( h_scenario, "snOnTick" );

		// エンディングの条件チェック時。毎度
		p_snOnEndingCondition = (PFNSNONENDINGCONDITION)GetProcAddress( h_scenario, "snOnEndingCondition" );
	} 

	return TRUE;
}


BOOL FreeLibraryOfScenarioMod() {
	// シナリオの方開放
	FreeLibrary( h_scenario );

	return TRUE;
}


