// ScenarioMod �p�w�b�_
//
#pragma once;

#include "ScenarioDataStruct.h"
#include "ScenarioMessageN6PPatch.h"
#include "GameDataStruct.h"

// ScenarioMod.dll�A�V�i���I�f�[�^�ݒ�p���\�b�h

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

// snOnWritingScenarioDetail���\�b�h�p�̌^�錾
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


// �R�}���h���x�����X�g
struct TCommandLabelList {
	char CommandName[20][20];
};

// snOnViewingBattleMenuInBattle 
typedef void (WINAPI *PFNSNONVIEWINGBATTLEMENUINBATTLE)(int iBushouID,
										   char *p�ʏ�U��,
										   char *p��čU��,
										   char *p�ˌ�,
										   char *p�S�C�U��,
										   char *p��C�U��
										   );
extern PFNSNONVIEWINGBATTLEMENUINBATTLE p_snOnViewingBattleMenuInBattle;

// snOnViewingCommandMenuInBattle
typedef void (WINAPI *PFNSNONVIEWINGCOMMANDMENUINBATTLE)(int iBushouID,
											char *p�ە�,
											char *p��O,
											char *p�ދp,
											char *p����,
											char *p�Z�\,
											char *p����,
											char *p����,
											char *p����,
											char *p�΍U��,
											char *p�ÎE,
											char *p����,
											char *p�ꊅ
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

// p_snOnRatificationDialog // �ǔF�n�_�C�A���O�m�F��
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


// ScenarioMod��snOnBattleStart�p�̃p�����^
struct �푈�J�n�p�����^ {
	int ��ԍ�;						// �W�I�̏�ԍ�
	int ������R�c_�R�c�ԍ�;		// ������̌R�c�ԍ�
	int ������R�c��_�����ԍ�;	// ������̌R�c���̕����ԍ�
	int �U�����R�c_�R�c�ԍ�;		// �U�߂����̌R�c�ԍ�
	int �U�����R�c��_�����ԍ�;	// �U�߂����̌R�c���̕����ԍ�
};

// ScenarioMod��snOnBattleStart�p�̃p�����^
struct �U�����@���莞�p�����^ {
	int �U�����@�ԍ�;
	int �ړ��O;						// �W�I�̏�ԍ�
	int �\��[8];
};

struct ���l���̓_�C�A���O�p�����^�^ {
	int �ŏ��l;
	int �ő�l;
	int ���͒l;						// 0xFFFF�Ȃ�L�����Z������
};


// �V�i���I���b�hDLL�̃��[�h
BOOL LoadLibraryOfScenarioMod();
// �V�i���I���b�hDLL�̉��
BOOL FreeLibraryOfScenarioMod();