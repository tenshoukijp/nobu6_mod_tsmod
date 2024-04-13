// ScenarioMod 用ヘッダ
//
#pragma once;

#include "ScenarioDataStruct.h"
#include "ScenarioMessageN6PPatch.h"
#include "GameDataStruct.h"

// ScenarioMod.dll、シナリオデータ設定用メソッド

// snConstructor
typedef void (WINAPI *PFNSNONCONSTRUCTOR)(void);
extern PFNSNONCONSTRUCTOR p_snOnConstructor;

// snDestructor
typedef void (WINAPI *PFNSNONDESTRUCTOR)(void);
extern PFNSNONDESTRUCTOR p_snOnDestructor;

// snDllMainBgn
typedef void (WINAPI *PFNSNONDLLMAINBGN)(void);
extern PFNSNONDLLMAINBGN p_snOnDllMainBgn;

// snTaikiBushouDataTransfer
typedef void (WINAPI *PFNSNONTAIKIBUSHOUDATATRANSFER)(byte *p0, int n0, byte *p1, int n1, byte *p2, int n2, byte *p3, int n3, byte *p4, int n4, byte *p5, int n5);
extern PFNSNONTAIKIBUSHOUDATATRANSFER p_snOnTaikiBushouDataTransfer;

// snDllMainEnd
typedef void (WINAPI *PFNSNONDLLMAINEND)(void);
extern PFNSNONDLLMAINEND p_snOnDllMainEnd;


// snOnViewingTopMenu
typedef void (WINAPI *PFNSNONVIEWINGTOPMENU)(void);
extern PFNSNONVIEWINGTOPMENU p_snOnViewingTopMenu;

// snOnAfterChangeWindowSize
typedef void (WINAPI *PFNSNONAFTERCHANGEWINDOWSIZE)(void);
extern PFNSNONAFTERCHANGEWINDOWSIZE p_snOnAfterChangeWindowSize;

// snOnSelectingScenarioAfter
typedef bool (WINAPI *PFNSNONSELECTINGSCENARIOAFTER)(void *);
extern PFNSNONSELECTINGSCENARIOAFTER p_snOnSelectingScenarioAfter;


// snOnSelectingScenarioDaimyo
typedef bool (WINAPI *PFNSNONSELECTINGSCENARIODAIMYO)(void *);
extern PFNSNONSELECTINGSCENARIODAIMYO p_snOnSelectingScenarioDaimyo;

// snOnRewriteFukidashiMessage
typedef bool (WINAPI *PFNSNONREWRITEFUKIDASHIMESSAGE)(char *szMsgBuf, char *szBushouName, char *szOrgMsg, NB6FUKIDASHIRELATEPERSONS *);
extern PFNSNONREWRITEFUKIDASHIMESSAGE p_snOnRewriteFukidashiMessage;

// snOnCustomCondition
typedef int (WINAPI *PFNSNONCUSTOMCONDITION)(char *szConditionName, int iprm1, int iprm2, int iprm3, char *sz1, char *sz2, float f1, float f2);
extern PFNSNONCUSTOMCONDITION p_snOnCustomCondition;

// snOnPlayerTurnInMainScreen
typedef void (WINAPI *PFNSNPLAYERTURNINMAINSCREEN)();
extern PFNSNPLAYERTURNINMAINSCREEN p_snOnPlayerTurnInMainScreen;

// snOnGundanTurnInMainScreen
typedef void (WINAPI *PFNSNGUNDANTURNINMAINSCREEN)(int iGundanID);
extern PFNSNGUNDANTURNINMAINSCREEN p_snOnGundanTurnInMainScreen;

// snOnWritingScenarioDetailメソッド用の型宣言
typedef char ** (WINAPI *PFNSNWRITINGSCENARIODETAIL)(int iScenarioNo);
extern PFNSNWRITINGSCENARIODETAIL p_snOnWritingScenarioDetail;

// snOnPlayerTurnInMainScreen
typedef void (WINAPI *PFNSNVIEWINGSCENARIOTITLELIST)();
extern PFNSNVIEWINGSCENARIOTITLELIST p_snOnViewingScenarioTitleList;

// snOnSaveSaveData
typedef void (WINAPI *PFNSNSAVESAVEDATA)(int iSelectID, char *szFileName);
extern PFNSNSAVESAVEDATA p_snOnSaveSaveData;

// snOnLoadSaveData
typedef void (WINAPI *PFNSNLOADSAVEDATA)(int iSelectID, char *szFileName);
extern PFNSNLOADSAVEDATA p_snOnLoadSaveData;


// コマンドラベルリスト
struct TCommandLabelList {
	char CommandName[20][20];
};

// snOnViewingBattleMenuInBattle 
typedef void (WINAPI *PFNSNONVIEWINGBATTLEMENUINBATTLE)(int iBushouID,
										   char *p通常攻撃,
										   char *p一斉攻撃,
										   char *p突撃,
										   char *p鉄砲攻撃,
										   char *p大砲攻撃
										   );
extern PFNSNONVIEWINGBATTLEMENUINBATTLE p_snOnViewingBattleMenuInBattle;

// snOnViewingCommandMenuInBattle
typedef void (WINAPI *PFNSNONVIEWINGCOMMANDMENUINBATTLE)(int iBushouID,
											char *p鼓舞,
											char *p城外,
											char *p退却,
											char *p内応,
											char *p技能,
											char *p挑発,
											char *p煽動,
											char *p混乱,
											char *p火攻め,
											char *p暗殺,
											char *p徴兵,
											char *p一喝
											);
extern PFNSNONVIEWINGCOMMANDMENUINBATTLE p_snOnViewingCommandMenuInBattle;

// snSetCurrentCastleIDInBattle
typedef void (WINAPI *PFNSNSETCURRENTCASTLEIDINBATTLE)(int iCastleID);
extern PFNSNSETCURRENTCASTLEIDINBATTLE p_snSetCurrentCastleIDInBattle;

// snOnN6PAudioPlayTrack
typedef int (WINAPI *PFNSNONN6PAUDIOPLAYTRACK)(int iTrackID);
extern PFNSNONN6PAUDIOPLAYTRACK p_snOnN6PAudioPlayTrack;

// p_snOnChangeTurnInBattle
typedef void (WINAPI *PFNSNONCHANGETURNINBATTLE)(int iCurTurn);
extern PFNSNONCHANGETURNINBATTLE p_snOnChangeTurnInBattle;

typedef int (WINAPI *PFNSNONCALCWEATHERINBATTLE)(int iWeather, int iCurTurn);
extern PFNSNONCALCWEATHERINBATTLE p_snOnCalcWeatherInBattle;

// p_snOnChangeCastleTurnInBattle
typedef void (WINAPI *PFNSNONCHANGECASTLETURNINBATTLE)(int iCurTurn);
extern PFNSNONCHANGECASTLETURNINBATTLE p_snOnChangeCastleTurnInBattle;

// p_snOnChangeSoba
typedef void (WINAPI *PFNSNONCHANGESOBA)(NB6SOBA *pnewsoba);
extern PFNSNONCHANGESOBA p_snOnChangeSoba;

// p_snOnMainGameStart
typedef void (WINAPI *PFNSNONMAINGAMESTART)();
extern PFNSNONMAINGAMESTART p_snOnMainGameStart;

// p_p_snOnEnterMainPlayerCommand
typedef void (WINAPI *PFNSNONENTERPLAYERCOMMAND)(int iMainCommandID, int iSubCommandID);
extern PFNSNONENTERPLAYERCOMMAND p_snOnEnterMainPlayerCommand;

// p_p_snOnLeaveMainPlayerCommand
typedef void (WINAPI *PFNSNONLEAVEPLAYERCOMMAND)();
extern PFNSNONLEAVEPLAYERCOMMAND p_snOnLeaveMainPlayerCommand;

// p_snOnBattleStart
typedef void (WINAPI *PFNSNONBATTLESTART)(void *param);
extern PFNSNONBATTLESTART p_snOnBattleStart;

// p_snOnBattleEnd
typedef void (WINAPI *PFNSNONBATTLEEND)();
extern PFNSNONBATTLEEND p_snOnBattleEnd;

// p_snOnBattleScreenEnd
typedef void (WINAPI *PFNSNONBATTLESCREENEND)();
extern PFNSNONBATTLESCREENEND p_snOnBattleScreenEnd;

// p_snOnBornGeneralBushou
typedef bool (WINAPI *PFNSNONBORNGENERALBUSHOU)(int iBushouID);
extern PFNSNONBORNGENERALBUSHOU p_snOnBornGeneralBushou;

// p_snOnBornRekishiBushou
typedef bool (WINAPI *PFNSNONBORNREKISHIBUSHOU)(int iBushouID);
extern PFNSNONBORNGENERALBUSHOU p_snOnBornRekishiBushou;

// p_snOnSkippedRekishiBushou
typedef int (WINAPI *PFNSNONSKIPPEDGENERALBUSHOU)(int iBushouID, int TouzyouTaikiCnt, int AllTaikiDataCnt);
extern PFNSNONSKIPPEDGENERALBUSHOU p_snOnSkippedRekishiBushou;


// p_snOnViewingJobName
typedef bool (WINAPI *PFNSNONVIEWINGJOBNAME)(char *szBuffer, int iBushouID);
extern PFNSNONVIEWINGJOBNAME p_snOnViewingJobName;

// p_snOnViewingSpecialFormName
typedef bool (WINAPI *PFNSNONVIEWINGSPECIALFORMNAME)(char *szBuffer, int iBushouID);
extern PFNSNONVIEWINGSPECIALFORMNAME p_snOnViewingSpecialFormName;

// p_snOnViewingKahouTypeName
typedef bool (WINAPI *PFNSNONVIEWINGKAHOUTYPENAME)(char *szBuffer, int iKahouID);
extern PFNSNONVIEWINGKAHOUTYPENAME p_snOnViewingKahouTypeName;

// p_snOnViewingButaiName
typedef int (WINAPI *PFNSNONVIEWINGBUTAINAME)(int iBushouID);
extern PFNSNONVIEWINGBUTAINAME p_snOnViewingButaiName;

// p_snOnGettingButaimeiGobiSprintf
typedef int (WINAPI *PFNSNONGETTINGBUTAIMEIGOBISPRINTF)(void *pSendParam, char *pBuf);
extern PFNSNONGETTINGBUTAIMEIGOBISPRINTF p_snOnGettingButaimeiGobiSprintf;

// p_snOnViewingPositionName
typedef bool (WINAPI *PFNSNONVIEWINGPOSITIONNAME)(int* iPointerAddress, int iBushouID, int iPosition);
extern PFNSNONVIEWINGPOSITIONNAME p_snOnViewingPositionName;

// p_snOnViewingBushouStateName
typedef bool (WINAPI *PFNSNONVIEWINGBUSHOUSTATENAME)(int* iPointerAddress, int iBushouID, int iPosition);
extern PFNSNONVIEWINGBUSHOUSTATENAME p_snOnViewingBushouStateName;

// p_snOnViewingIe
typedef bool (WINAPI *PFNSNONVIEWINGIE)(int iBushouID, char *szBuferPointer);
extern PFNSNONVIEWINGIE p_snOnViewingIe;

// p_snOnViewingRetsuden
typedef bool (WINAPI *PFNSNONVIEWINGRETSUDEN)(int iBushouID, int iFaceID, char *szRetsudenTitle, char *szRetsudenDetail);
extern PFNSNONVIEWINGRETSUDEN p_snOnViewingRetsuden;

// p_snOnViewingMeibutsuJiten
typedef bool (WINAPI *PFNSNONVIEWINGMEIBUTSUJITEN)(int iKahouID, char *szRetsudenTitle, char *szRetsudenDetail);
extern PFNSNONVIEWINGMEIBUTSUJITEN p_snOnViewingMeibutsuJiten;

// p_snOnViewingMeibutsuJiten
typedef bool (WINAPI *PFNSNONVIEWINGMEIJOMONOGATARI)(int iCastleID, char *szRetsudenTitle, char *szRetsudenDetail);
extern PFNSNONVIEWINGMEIJOMONOGATARI p_snOnViewingMeijoMonogatari;

// p_snOnRequestFullcolorFaceFileName
typedef bool (WINAPI *PFNSNONREQUESTFACEFULLCOLORFACEFILENAME)(char* szOrgFileNameBuffer, char *szDstFileNameBuffer);
extern PFNSNONREQUESTFACEFULLCOLORFACEFILENAME p_snOnRequestFullcolorFaceFileName;

// p_snOnSelectingBattleAction
typedef int (WINAPI *PFNSNONSELECTINGBATTLEACTION)(int iBushouID, int iTargetID, int iSelectingActionID, void *pOtherInfo);
extern PFNSNONSELECTINGBATTLEACTION p_snOnSelectingBattleAction;

// p_snOnBattleDamaged
typedef int (WINAPI *PFNSNONBATTLEDAMAGED)(int iDefBushouID, int iDamagedValueu, void *pOtherInfo);
extern PFNSNONBATTLEDAMAGED p_snOnBattleDamaged;

// p_snOnChangeUnitMovePowerInBattle
typedef int (WINAPI *PFNSNONCHANTEUNITMOVEPOWERININBATTLE)(int iBushouID, int iPrev, int iNext);
extern PFNSNONCHANTEUNITMOVEPOWERININBATTLE p_snOnChangeUnitMovePowerInBattle;

// p_snOnRequestSoldierMax
typedef int (WINAPI *PFNSNONREQUESTSOLDIERMAX)(int iBushouID, int iOriginalSoldierNum, int iModifiedSoldierNum);
extern PFNSNONREQUESTSOLDIERMAX p_snOnRequestSoldierMax;

// p_snOnDecisionInputNumberDialog
typedef int (WINAPI *PFNSNONDECISIONINPUTNUMBERDIALOG)(char *pszMessage, void *pOtherInfo);
extern PFNSNONDECISIONINPUTNUMBERDIALOG p_snOnDecisionInputNumberDialog;

// p_snOnRatificationDialog // 追認系ダイアログ確認時
typedef int (WINAPI *PFNSNONRATIFICATIONDIALOG)(char *pszMessage, char *szTitle);
extern PFNSNONRATIFICATIONDIALOG p_snOnRatificationDialog;

// p_snOnApplicationSelectMenuItem
typedef void (WINAPI *PFNSNONAPPLICATIONSELECTMENUITEM)(int iSelectMenuItem);
extern PFNSNONAPPLICATIONSELECTMENUITEM p_snOnApplicationSelectMenuItem;

// p_snOnTick
typedef void (WINAPI *PFNSNONTICK)(int iSystemTime, int iProceedTime);
extern PFNSNONTICK p_snOnTick;

// p_snOnEndingInfo
typedef int (WINAPI *PFNSNONENDINGCONDITION)(void *pBuf, int iDaimyoID, int iIsIxist);
extern PFNSNONENDINGCONDITION p_snOnEndingCondition;


#define CUSTOM_CONDITION_NOMODIFY -1


// ScenarioModのsnOnBattleStart用のパラメタ
struct 戦争開始パラメタ {
	int 城番号;						// 標的の城番号
	int 守備側軍団_軍団番号;		// 守備側の軍団番号
	int 守備側軍団長_武将番号;	// 守備側の軍団長の武将番号
	int 攻撃側軍団_軍団番号;		// 攻めた側の軍団番号
	int 攻撃側軍団長_武将番号;	// 攻めた側の軍団長の武将番号
};

// ScenarioModのsnOnBattleStart用のパラメタ
struct 攻撃方法決定時パラメタ {
	int 攻撃方法番号;
	int 移動前;						// 標的の城番号
	int 予備[8];
};

struct 数値入力ダイアログパラメタ型 {
	int 最小値;
	int 最大値;
	int 入力値;						// 0xFFFFならキャンセルした
};


// シナリオモッドDLLのロード
BOOL LoadLibraryOfScenarioMod();
// シナリオモッドDLLの解放
BOOL FreeLibraryOfScenarioMod();