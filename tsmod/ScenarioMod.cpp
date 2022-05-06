#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"


#include "TSMod.h"
#include "GameDataStruct.h"
#include "menu.h"


// ScenarioMod.dll�n���h��
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
	// ScenarioMod.dll��ǂ݂ɂ�����B��{�I�ɂ͖������낤�B
	h_scenario = LoadLibrary( "ScenarioMod.dll" );

	if ( h_scenario != NULL ) {

		AddModuleItem( h_scenario, "scenariomod.dll" ); // �S�ď�������
		
		// �R���X�g���N�^
		p_snOnConstructor = (PFNSNONCONSTRUCTOR)GetProcAddress( h_scenario, "snOnConstructor" );

		// �f�X�g���N�^
		p_snOnDestructor = (PFNSNONDESTRUCTOR)GetProcAddress( h_scenario, "snOnDestructor" );

		// �ҋ@�����̃f�[�^�̂܂�܂�̓]��
		p_snOnTaikiBushouDataTransfer = (PFNSNONTAIKIBUSHOUDATATRANSFER)GetProcAddress(h_scenario, "snOnTaikiBushouDataTransfer");

		// DLLMain�J�n����
		p_snOnDllMainBgn = (PFNSNONCONSTRUCTOR)GetProcAddress( h_scenario, "snOnDllMainBgn" );

		// DLLMain�I�����O
		p_snOnDllMainEnd = (PFNSNONDESTRUCTOR)GetProcAddress( h_scenario, "snOnDllMainEnd" );

		// snOnViewingTopMenu
		p_snOnAfterChangeWindowSize = (PFNSNONAFTERCHANGEWINDOWSIZE)GetProcAddress( h_scenario, "snOnAfterChangeWindowSize" );

		// snOnViewingTopMenu
		p_snOnViewingTopMenu = (PFNSNONVIEWINGTOPMENU)GetProcAddress( h_scenario, "snOnViewingTopMenu" );

		// �U�̃V�i���I�^�C�g����\�����钼�O
		p_snOnViewingScenarioTitleList = (PFNSNVIEWINGSCENARIOTITLELIST)GetProcAddress( h_scenario, "snOnViewingScenarioTitleList" );
		if ( ! p_snOnViewingScenarioTitleList ) {
			p_snOnViewingScenarioTitleList = (PFNSNVIEWINGSCENARIOTITLELIST)GetProcAddress( h_scenario, "snViewingScenarioTitleList" ); // or �ɂȂ��Ă���̂͌Â��~�X�B���̐��N�o�߂��������
		}

		// �V�i���I�ڍׂ�\�����鎞
		p_snOnWritingScenarioDetail = (PFNSNWRITINGSCENARIODETAIL)GetProcAddress( h_scenario, "snOnWritingScenarioDetail" );

		// �V�i���I�ڍׂŁu�m�F�v�{�^������������A�V�i���I�f�[�^�z�u��
		p_snOnSelectingScenarioAfter = (PFNSNONSELECTINGSCENARIOAFTER)GetProcAddress(h_scenario, "snOnSelectingScenarioAfter");

		// �V�i���I�f�[�^�̈����o���p���\�b�h�̃|�C���^�m��
		p_snOnSelectingScenarioDaimyo = (PFNSNONSELECTINGSCENARIODAIMYO)GetProcAddress( h_scenario, "snOnSelectingScenarioDaimyo" );

		// ���C���Q�[���X�^�[�g���ɁA�Ăяo�����\�b�h�B
		p_snOnMainGameStart = (PFNSNONMAINGAMESTART)GetProcAddress( h_scenario, "snOnMainGameStart" );

		// �v���C���̓���̃R�}���h���s�̐N����
		p_snOnEnterMainPlayerCommand = (PFNSNONENTERPLAYERCOMMAND)GetProcAddress( h_scenario, "snOnEnterMainPlayerCommand" );

		// �v���C���̓���̃R�}���h���s�̒E�o��
		p_snOnLeaveMainPlayerCommand = (PFNSNONLEAVEPLAYERCOMMAND)GetProcAddress( h_scenario, "snOnLeaveMainPlayerCommand" );

		// ���o�����b�Z�[�W������������p�̃��\�b�h�̃|�C���^�m��
		p_snOnRewriteFukidashiMessage = (PFNSNONREWRITEFUKIDASHIMESSAGE)GetProcAddress( h_scenario, "snOnRewriteFukidashiMessage" );

		// ���l���̓_�C�A���O�Ő��l�����肵�����Ɏ��s
		p_snOnDecisionInputNumberDialog = (PFNSNONDECISIONINPUTNUMBERDIALOG)GetProcAddress( h_scenario, "snOnDecisionInputNumberDialog" );

		// �ǔF�n�_�C�A���O�̊m�F��
		p_snOnRatificationDialog = (PFNSNONRATIFICATIONDIALOG)GetProcAddress( h_scenario, "snOnRatificationDialog" );

		// �v���C���^�[�����̃��C����ʂł̃��\�b�h�̃|�C���^�m��
		p_snOnPlayerTurnInMainScreen = (PFNSNPLAYERTURNINMAINSCREEN)GetProcAddress( h_scenario, "snOnPlayerTurnInMainScreen" );

		// �R�c�^�[�����̃��C����ʂł̃��\�b�h�̃|�C���^�m��
		p_snOnGundanTurnInMainScreen = (PFNSNGUNDANTURNINMAINSCREEN)GetProcAddress( h_scenario, "snOnGundanTurnInMainScreen" );

		// �J�X�^���R���f�B�V�����p�B��{��TRUE/FALSE�����A�ꉞINT�^�Œl��Ԃ���悤�ɂ��Ă���B
		p_snOnCustomCondition = (PFNSNONCUSTOMCONDITION)GetProcAddress( h_scenario, "snOnCustomCondition" );

		// �퓬�R�}���h��\�����钼�O
		p_snOnViewingBattleMenuInBattle = (PFNSNONVIEWINGBATTLEMENUINBATTLE)GetProcAddress( h_scenario, "snOnViewingBattleMenuInBattle5" );

		// ��{�R�}���h��\�����钼�O
		p_snOnViewingCommandMenuInBattle = (PFNSNONVIEWINGCOMMANDMENUINBATTLE)GetProcAddress( h_scenario, "snOnViewingCommandMenuInBattle12" );

		// �퓬�s�����莞
		p_snOnSelectingBattleAction = (PFNSNONSELECTINGBATTLEACTION)GetProcAddress( h_scenario, "snOnSelectingBattleAction" );

		// �_���[�W����
		p_snOnBattleDamaged = (PFNSNONBATTLEDAMAGED)GetProcAddress( h_scenario, "snOnBattleDamaged" );

		// �ړ���
		p_snOnChangeUnitMovePowerInBattle = (PFNSNONCHANTEUNITMOVEPOWERININBATTLE)GetProcAddress( h_scenario, "snOnChangeUnitMovePowerInBattle" );

		// ���̒���A��킩��킩�𔻒f���邽�߂ɁA��ԍ���n���B0xFFFF�Ȃ���
		p_snSetCurrentCastleIDInBattle = (PFNSNSETCURRENTCASTLEIDINBATTLE)GetProcAddress( h_scenario, "snSetCurrentCastleIDInBattle" );

		// �g���b�N�̍Đ����ɌĂуo�X�B�Đ����O�ɍĐ��ԍ���ύX���邽�߁B-1���Ԃ��Ă����炻�̂܂܂̔ԍ����Đ��B
		p_snOnN6PAudioPlayTrack = (PFNSNONN6PAUDIOPLAYTRACK)GetProcAddress( h_scenario, "snOnN6PAudioPlayTrack" );

		// �퓬���n�܂�����
		p_snOnBattleStart = (PFNSNONBATTLESTART)GetProcAddress( h_scenario, "snOnBattleStart" );

		// �퓬���̌o�߃^�[�����ω��������ɁA�Ăяo�����\�b�h�B
		p_snOnChangeTurnInBattle = (PFNSNONCHANGETURNINBATTLE)GetProcAddress( h_scenario, "snOnChangeTurnInBattle" );

		// �V�C���莞
		p_snOnCalcWeatherInBattle = (PFNSNONCALCWEATHERINBATTLE)GetProcAddress( h_scenario, "snOnCalcWeatherInBattle" );

		// �U���̌o�߃^�[�����ω��������ɁA�Ăяo�����\�b�h�B
		p_snOnChangeCastleTurnInBattle = (PFNSNONCHANGECASTLETURNINBATTLE)GetProcAddress( h_scenario, "snOnChangeCastleTurnInBattle" );

		// �퓬��ʂ��I�������(PC/NPC�����BNPC�̏ꍇ�́A��ʂɓ����ĂȂ��Ă�����)
		p_snOnBattleScreenEnd = (PFNSNONBATTLESCREENEND)GetProcAddress( h_scenario, "snOnBattleScreenEnd" );

		// �퓬���I�������
		p_snOnBattleEnd = (PFNSNONBATTLEEND)GetProcAddress( h_scenario, "snOnBattleEnd" );

		// ���ꂪ�ω����钼�O�ɁA�Ăяo�����\�b�h�B
		p_snOnChangeSoba = (PFNSNONCHANGESOBA)GetProcAddress( h_scenario, "snOnChangeSoba" );

		// �ҋ@�������o�ꂷ�钼�O
		p_snOnSkippedRekishiBushou = (PFNSNONSKIPPEDGENERALBUSHOU)GetProcAddress(h_scenario, "snOnSkippedRekishiBushou");

		// �ҋ@�̕����o��
		p_snOnBornRekishiBushou = (PFNSNONBORNREKISHIBUSHOU)GetProcAddress(h_scenario, "snOnBornRekishiBushou");

		// �ėp�̕����a��
		p_snOnBornGeneralBushou = (PFNSNONBORNGENERALBUSHOU)GetProcAddress(h_scenario, "snOnBornGeneralBushou");

		// �E�Ɩ���\�����钼�O�ɌĂяo�����\�b�h�B
		p_snOnViewingJobName = (PFNSNONVIEWINGJOBNAME)GetProcAddress( h_scenario, "snOnViewingJobName" );

		// ���ꕺ�Ԗ���\�����钼�O�ɌĂяo�����\�b�h�B(�R�n�S�C�̂Ƃ���)
		p_snOnViewingSpecialFormName = (PFNSNONVIEWINGSPECIALFORMNAME)GetProcAddress( h_scenario, "snOnViewingSpecialFormName" );

		// �ƕ��ޖ���\�����钼�O�ɌĂяo�����\�b�h�B
		p_snOnViewingKahouTypeName = (PFNSNONVIEWINGKAHOUTYPENAME)GetProcAddress( h_scenario, "snOnViewingKahouTypeName" );

		// ���Ԗ���\�����钼�O�ɌĂяo�����\�b�h�B
		p_snOnViewingButaiName = (PFNSNONVIEWINGBUTAINAME)GetProcAddress( h_scenario, "snOnViewingButaiName" );

		// ���Ԗ���\�����钼�O�ɌĂяo�����\�b�h�B
		p_snOnGettingButaimeiGobiSprintf = (PFNSNONGETTINGBUTAIMEIGOBISPRINTF)GetProcAddress( h_scenario, "snOnGettingButaimeiGobiSprintf" );

		// �g������\�����钼�O�ɌĂяo�����\�b�h�B
		p_snOnViewingPositionName = (PFNSNONVIEWINGPOSITIONNAME)GetProcAddress( h_scenario, "snOnViewingPositionName" );

		// �����̏�Ԗ���\�����钼�O�ɌĂяo�����\�b�h�B
		p_snOnViewingBushouStateName = (PFNSNONVIEWINGBUSHOUSTATENAME)GetProcAddress( h_scenario, "snOnViewingBushouStateName" );

		// �u�Ɓv���\������钼�O�ɌĂяo����郁�\�b�h
		p_snOnViewingIe = (PFNSNONVIEWINGIE)GetProcAddress(h_scenario, "snOnViewingIe");

		// ������`�̃^�C�g���̕\����
		p_snOnViewingRetsuden = (PFNSNONVIEWINGRETSUDEN)GetProcAddress( h_scenario, "snOnViewingRetsuden" );

		// �ƕ󎖓T�̃^�C�g���̕\����
		p_snOnViewingMeibutsuJiten = (PFNSNONVIEWINGMEIBUTSUJITEN)GetProcAddress( h_scenario, "snOnViewingMeibutsuJiten" );

		// ���`�̃^�C�g���̕\����
		p_snOnViewingMeijoMonogatari = (PFNSNONVIEWINGMEIJOMONOGATARI)GetProcAddress( h_scenario, "snOnViewingMeijoMonogatari" );

		// �Ώە����̕����̍ő吔��v�����ꂽ��
		p_snOnRequestSoldierMax = (PFNSNONREQUESTSOLDIERMAX)GetProcAddress( h_scenario, "snOnRequestSoldierMax" );

		// �Z�[�u�������ɌĂяo�����\�b�h�B
		p_snOnSaveSaveData = (PFNSNSAVESAVEDATA)GetProcAddress( h_scenario, "snOnSaveSaveData" );

		// ���[�h�������ɌĂяo�����\�b�h�B
		p_snOnLoadSaveData = (PFNSNLOADSAVEDATA)GetProcAddress( h_scenario, "snOnLoadSaveData" );

		// �A�v���P�[�V�����̃��j���[��I���������ɌĂяo�����\�b�h�B
		p_snOnApplicationSelectMenuItem = (PFNSNONAPPLICATIONSELECTMENUITEM)GetProcAddress( h_scenario, "snOnApplicationSelectMenuItem" );

		// �펞�B�P�b��5��ڈ��B
		p_snOnTick = (PFNSNONTICK)GetProcAddress( h_scenario, "snOnTick" );

		// �G���f�B���O�̏����`�F�b�N���B���x
		p_snOnEndingCondition = (PFNSNONENDINGCONDITION)GetProcAddress( h_scenario, "snOnEndingCondition" );
	} 

	return TRUE;
}


BOOL FreeLibraryOfScenarioMod() {
	// �V�i���I�̕��J��
	FreeLibrary( h_scenario );

	return TRUE;
}


