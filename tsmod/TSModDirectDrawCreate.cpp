

#include "WinTarget.h"
#include "TSMod.h"
#include "PluginMod.h"
#include "CommonGlobalHandle.h"
#include "GetProcessCount.h"
#include "TSModPatchProcessMemory.h"
#include "HookFunctions.h"
#include "KaoSwapExtend.h"
#include "AllocConsole.h"
#include "Font.h"
#include "Inmm.h"
#include "GodPatch.h"
#include "TimeStamp.h"
#include "SetExtensionMovie.h"
#include "ResolutionMobile.h"
#include "ResolutionWide.h"
#include "WriteSoundMap.h"
#include "GetProcessCount.h"
#include "NoCompactDisk.h"
#include "SetNeedKoudouryoku.h"
#include "SetMyDamageOfDoor.h"
#include "SetHelpFileName.h"
#include "BFileN6P.h"
#include "KahouN6P.h"
#include "HexmapNB6.h"
#include "TenShougiMod.h"
#include "DijkstraCastle.h"

/*----------------------------------------------------------------*
 �A�Z���u�����������n�� Extern
 *----------------------------------------------------------------*/
// ��W�����p�[
extern void InitFixExperienceDefault();
extern void InitAutoUpExperienceDefault();

extern void WriteAsmJumperOnTenshouExeCallBackWindowProc();
extern void WriteAsmOnTenshouExeRetsuDenExistNumOfBFileN6P();
extern void WriteAsmJumperOnTenshouExeTeppouDenraiEvent(); // ����̓W�����p�[�ł͂Ȃ��A���^�[��
extern void WriteAsmJumperOnNonFaceBushouIndependences(); // 
extern int LoadSnDataN6PToVector();
extern int LoadSnTaikiBushouN6PToVector();
extern void setMenuEnableOrDisable();
extern void WriteAsmJumperOnTenshouExeJudgeKahouDialogBigMove();
extern void OnTenshouExeCallSoundModServer();
extern void SetDeathFrequencyNotInfluenceLife();
extern void SetCanInstructDirectOtherGundan();
extern void SetIncreaseRatioCanDraftSoldier();
extern void SetOtherDaimyoNeverThreat();
extern void SetWorkToDieEverySeason();
extern void SetCastleDefenceCoefOfNPC();
extern void SetDegreeLoyaltyOnCaptiveLibra();
extern void SetAppearanceHimeBecomeBushou();
extern void SetNonFaceBushouIndependences();
extern void SetHideRangePatch(); // �P�Ɣ��肷��̂�500�l�ڈȍ~�ł͂Ȃ��A0�l�ڈȍ~�Ƃ���B
extern void InitModifyBushouListTitle();
extern void InitKunkouIsNotResetOnRonin();
extern void SetChangeBGMTrackNum();
extern void patchDisableDDrawResizeFunction();
extern void patchDisableDDrawHimeWrongAppearBug();
extern void WriteDDrawDrawKaoFullColorFileName();
extern void SetUnviewingEndingLogo();
extern int setHexMapNB6ToHexMapArray();
extern void setNoCaptionAndMenuBarBefore();


// �W�����p�[
extern void WriteAsmJumperOnTenshouExeMessageLoad();
extern void WriteAsmJumperOnTenshouExeMessageCopy();
extern void WriteAsmJumperOnTenshouExeWmMessage();
extern void WriteAsmJumperOnTenshouExePlayerTurnChange();
extern void WriteAsmJumperOnTenshouExeBushouSoldierMax();
extern void WriteAsmJumperOnTenshouExeBushouIDSoldier();
extern void WriteAsmJumperOnTenshouExeViewingScreenMenu();
extern void WriteAsmJumperOnTenshouExeViewingTopMenu();
extern void WriteAsmJumperOnTenshouExeLoadSaveDataInGame();
extern void WriteAsmJumperOnTenshouExeSaveSaveDataInGame();
extern void WriteAsmJumperOnTenshouExeSoldierDraftFilterInBattle();
extern void WriteAsmJumperOnTenshouExeViewingCommandMenuInBattle();
extern void WriteAsmJumperOnTenshouExeViewingBattleMenuInBattle1();
extern void WriteAsmJumperOnTenshouExeViewingBattleMenuInBattle2();
extern void WriteAsmJumperOnTenshouExeViewingJobInBushouDetail();
extern void WriteAsmJumperOnTenshouExeViewingJobNoInBushouDetail();
extern void WriteAsmJumperOnTenshouExeRainRangeJudge();
extern void WriteAsmJumperOnTenshouExeEmperorManeuvers();
extern void WriteAsmJumperOnTenshouExeDamagedOfDoor();
extern void WriteAsmJumperOnTenshouExeCommerceBenzetsuJudge();
extern void WriteAsmJumperOnTenshouExeCommerceSuccessJudge();
extern void WriteAsmJumperOnTenshouExeCommerceMessageJudge();
extern void	WriteAsmJumperOnTenshouExeMainGameStart();
extern void WriteAsmJumperOnTenshouExeSelectingScenarioDaimyo();
extern void WriteAsmJumperOnTenshouExeSobaChange();
extern void WriteAsmJumperOnTenshouExeSobaGunChange();
extern void WriteAsmJumperOnTenshouExeSobaSoldierChange();
extern void WriteAsmJumperOnTenshouExeSobaHorseChange();
extern void WriteAsmJumperOnTenshouExeGlobalFreeMemoryInGameEnd();
extern void WriteAsmJumperOnTenshouExeMovieWsprintfPush();
extern void WriteAsmJumperOnTenshouExeCastleFallDown();
extern void WriteAsmJumperOnTenshouExeAnsatsuAttackInBattle();
extern void WriteAsmJumperOnTenshouExeSelectingAbilityMenuInBattle();
extern void WriteAsmJumperOnTenshouExeSoldierDraftConditionInBattle();
extern void WriteAsmJumperOnTenshouExeSendouOneOnceJudgement();
extern void WriteAsmJumperOnTenshouExeSendouSelectingWhich();
extern void WriteAsmJumperOnTenshouExeSendouKonranIsOneOrAll();
extern void WriteAsmJumperOnTenshouExeSendouKonranMultiKonran();
extern void WriteAsmJumperOnTenshouExeSendouKonranAllMessage();
extern void WriteAsmJumperOnTenshouExeDecrementTurnInBattle();
extern void WriteAsmJumperOnTenshouExePlayerGundanTurnEndInBattle();
extern void WriteAsmJumperOnTenshouExeBattleStartOfPlayer();
extern void WriteAsmJumperOnTenshouExeBattleStartOfNPC();
extern void WriteAsmJumperOnTenshouExeTaihouAttackTimesRewrite();
extern void WriteAsmJumperOnTenshouExeTaihouAndTekkousenAttackTimesRewrite();
extern void WriteAsmJumperOnTenshouExeHirouSelectingBushouList();
extern void WriteAsmJumperOnTenshouExeAptitudeKiba();
extern void WriteAsmJumperOnTenshouExeAptitudeTeppou();
extern void WriteAsmJumperOnTenshouExeCurTekkousenFormCheck();
extern void WriteAsmJumperOnTenshouExeCompareDeclineBat();
extern void WriteAsmJumperOnTenshouExeCompareDeclineInt();
extern void WriteAsmJumperOnTenshouExeCompareDeclineGov();
extern void WriteAsmJumperOnTenshouExeDamagedOfAttack();
extern void WriteAsmJumperOnTenshouExeSeiiTaishoConditionCastleNum();
extern void WriteAsmJumperOnTenshouExeMoveEndAnimationInBattle();
extern void WriteAsmJumperOnTenshouExeMoveEndJudgeInBattle();
extern void	WriteAsmJumperOnTenshouExeExecuteBattleCommandInBattle();
extern void WriteAsmJumperOnTenshouExeExecuteBattleAttackInBattle();
extern void WriteAsmJumperOnTenshouExeMoveUnitAnimationChangeInBattle();
extern void WriteAsmJumperOnTenshouExeWsprintfOppositeIDAndStr();
extern void WriteAsmJumperOnTenshouExeFukidashiTypeB_Left();
extern void WriteAsmJumperOnTenshouExeFukidashiTypeB_Right();
extern void WriteAsmJumperOnTenshouExeFukidashiTypeBFocusChange();
extern void WriteAsmJumperOnTenshouExeFukidashiTypeBMessageChange();
extern void WriteAsmJumperOnTenshouExeFukidashiTypeBMessageStart();
extern void WriteAsmJumperOnTenshouExeMessageCopyMem();
extern void WriteAsmJumperOnTenshouExeEndingConditionTest();
extern void WriteAsmJumperOnTenshouExeEndingTitlePush();
extern void WriteAsmJumperOnTenshouExeTotsugekiResultTest();
extern void WriteAsmJumperOnTenshouExeNextTurnIsFieldOrCastle();
extern void WriteAsmJumperOnTenshouExeGetNextTurnCastleID();
extern void WriteAsmJumperOnTenshouExeN6PAudioPlayTracking();
extern void WriteAsmJumperOnTenshouExeSubstituteMaxShutujinNum();
extern void WriteAsmJumperOnTenshouExeSubstituteMaxShutujinMsg();
extern void	WriteAsmJumperOnTenshouExeSubstituteMaxShutujinRemain1();
extern void	WriteAsmJumperOnTenshouExeSubstituteMaxShutujinRemain2();
extern void WriteAsmJumperOnTenshouExeSubstituteMaxShutujinCaseAll();
extern void WriteAsmJumperOnTenshouExeSelectingScenario();
extern void WriteAsmJumperOnTenshouExeTaikyakuTry();
extern void WriteAsmJumperOnTenshouExeViewingKahouDetail();
extern void WriteAsmJumperOnTenshouExeShowingKahouDetail();
extern void WriteAsmJumperOnTenshouExeBattleAfterOfNPC();
extern void WriteAsmJumperOnTenshouExeBattleStartOfNPCPrevious();
extern void WriteAsmJumperOnTenshouExeEnteringFromScenarioDaimyo();
extern void WriteAsmJumperOnTenshouExeEnteringAfterScenarioDaimyo();
extern void WriteAsmJumperOnTenshouExeMovieMoveWindow();
extern void WriteAsmJumperOnTenshouExeMovieBlackBack1();
extern void WriteAsmJumperOnTenshouExeMovieBlackBack2();
extern void WriteAsmJumperOnTenshouExeMovieBGMNonStop();
extern void WriteAsmJumperOnTenshouExeHirouFukidashi();
extern void WriteAsmJumperOnTenshouExeGungiFukidashi();
extern void WriteAsmJumperOnTenshouExeGungiGudanNo();
extern void WriteAsmJumperOnTenshouExeGundiDoing();
extern void WriteAsmJumperOnTenshouExeJudgeTeppouDeathInBattle();
extern void WriteAsmJumperOnTenshouExeTeppouDeathInBattle();
extern void WriteAsmJumperOnTenshouExeAnsatsuBushouInMain();
extern void WriteAsmJumperOnTenshouExeMoveRemainPowerInBattle();
extern void WriteAsmJumperOnTenshouExeBushouDetailKibaTeppouJudge();
extern void WriteAsmJumperOnTenshouExeBushouDetailKibaTeppou();
extern void WriteAsmJumperOnTenshouExeTaikiAppearance();
extern void WriteAsmJumperOnTenshouExeJudgeTeppouOrTeppouKiba();
extern void WriteAsmJumperOnTenshouExeJudgeTeppouOrTeppouTai();
extern void WriteAsmJumperOnTenshouExeCanAnyAttackDistance();
extern void WriteAsmJumperOnTenshouExeSetCanTaihouAndTekkousenDistance();
extern void WriteAsmJumperOnTenshouExeFireTeppou();
extern void WriteAsmJumperOnTenshouExeFireTeppouSE();
extern void WriteAsmJumperOnTenshouExeJudgeHasTaihou1();
extern void WriteAsmJumperOnTenshouExeJudgeHasTaihou2();
extern void WriteAsmJumperOnTenshouExeJudgeHasTaihou3();
extern void WriteAsmJumperOnTenshouExeChangeWeather();
extern void WriteAsmJumperOnTenshouExeSelectingAttackMenuInBattle();
extern void WriteAsmJumperOnTenshouExeNPCSelectingAttackPrevMoveInBattle();
extern void WriteAsmJumperOnTenshouExeNPCSelectingAttackPostMoveInBattle();
extern void WriteAsmJumperOnTenshouExeNCSelectingAttackPostMoveInBattle();
extern void WriteAsmJumperOnTenshouExePCSelectingAttackPostMoveInBattle();
extern void WriteAsmJumperOnTenshouExeNPCSelectingAttackPostMoveAfterInBattle();
extern void WriteAsmJumperOnTenshouExeViewingBushouDetailButaimei();
extern void WriteAsmJumperOnTenshouExeViewingMainBushouListButaimei();
extern void	WriteAsmJumperOnTenshouExeSelectingMainButaiHensyuUnit();
extern void WriteAsmJumperOnTenshouExeDetailKahouLayout();
extern void WriteAsmJumperOnTenshouExeDetailKahouGraphicPush();
extern void WriteAsmJumperOnTenshouExeFireKonranTeppouSE();
extern void WriteAsmJumperOnTenshouExeIketottariInBattle();
extern void WriteAsmJumperOnTenshouExeUchitottariInBattle();
extern void WriteAsmJumperOnTenshouExeJudgeWarCommissionCancel();
extern void WriteAsmJumperOnTenshouExeJudgeMainCommissionCancel();
extern void WriteAsmJumperOnTenshouExeUnitMoveEnd();
extern void WriteAsmJumperOnTenshouExeCheckRojoBushouNum();
extern void WriteAsmJumperOnTenshouExeEditingButai();
extern void WriteAsmJumperOnTenshouExeOnActionEndInBattle();
extern void WriteAsmJumperOnTenshouExeIncrementCastleTurnInBattle();
extern void WriteAsmJumperOnTenshouExeChangeGundanTurnInBattle();
extern void WriteAsmJumperOnTenshouExeJudgeGungiDone();
extern void WriteAsmJumperOnTenshouExeHirouSelectingBushouList();
extern void WriteAsmJumperOnTenshouExeJudgeHimeHanteiPrev();
extern void WriteAsmJumperOnTenshouExeJudgeHimeHanteiPost();
extern void WriteAsmJumperOnTenshouExeViewingHimeDetail();
extern void WriteAsmJumperOnTenshouExeViewingHimeDetailMusume();
extern void WriteAsmJumperOnTenshouExeViewingHimeDetailTsuma();
extern void WriteAsmJumperOnTenshouExeViewingHimeDetailShimai();
extern void WriteAsmJumperOnTenshouExeGetPointerKaikenHeapMemory();
extern void WriteAsmJumperOnTenshouExeGundanTurnChange();
extern void WriteAsmJumperOnTenshouExeCastleTurnChange();
extern void WriteAsmJumperOnTenshouExeViewingKahouDetailKahouType();
extern void WriteAsmJumperOnTenshouExeViewingKahouList();
extern void WriteAsmJumperOnTenshouExePushSaveFileNameOfUnknown();
extern void WriteAsmJumperOnTenshouExePushSaveFileName();
extern void WriteAsmJumperOnTenshouExeViewingBushouListKibaTeppou();
extern void WriteAsmJumperOnTenshouExeUpdateToRoninKunkouParam();
extern void WriteAsmJumperOnTenshouExeUpdateToRoninOfWarKunkouParam();
extern void WriteAsmJumperOnTenshouExeUpdateToGennekiKunkouParam();
extern void WriteAsmJumperOnTenshouExeUpdateToHataageKunkouParam();
extern void WriteAsmJumperOnTenshouExeDecisionScenarioIDOnSaveOrLoad();
extern void WriteAsmJumperOnTenshouExeWritingScenarioDetail1st();
extern void WriteAsmJumperOnTenshouExeWritingScenarioDetail2nd();
extern void WriteAsmJumperOnTenshouExeWritingScenarioDetail3rd();
extern void WriteAsmJumperOnTenshouExeViewingScenarioTitleList();
extern void WriteAsmJumperOnTenshouExeEndingAfterJumper();
extern void WriteAsmJumperOnTenshouExeEnteringDoEnding();
extern void WriteAsmJumperOnTenshouExeEndLogoWaiting();
extern void WriteAsmJumperOnTenshouExeGetGunshiBushouIDPrev();
extern void WriteAsmJumperOnTenshouExeGetGunshiBushouIDAfter();
extern void WriteAsmJumperOnTenshouExeLoadSaveDataInOpening();
extern void WriteAsmJumperOnTenshouExeDecideOfGeneralKaoFace();
extern void WriteAsmJumperOnTenshouExeUpdateExperienceGov();
extern void WriteAsmJumperOnTenshouExeUpdateExperienceBat();
extern void WriteAsmJumperOnTenshouExeUpdateExperienceInt();
extern void WriteAsmJumperOnTenshouExeGetDaimyoKoukeishaBushouID();
extern void WriteAsmJumperOnTenshouExeResultOfNaiouSuccessInBattle();
extern void WriteAsmJumperOnTenshouExeResultOfNaiouFailInBattle();
extern void WriteAsmJumperOnTenshouExePostBattleTreatment();
extern void WriteAsmJumperOnTenshouExeSuccessOrFailOfPostBattleTreatment();
extern void WriteAsmJumperOnTenshouExeDaimyoBushouIDOfPostBattleTreatment();
extern void WriteAsmJumperOnTenshouExeInputNumberDialog();
extern void WriteAsmJumperOnTenshouExeConfirmRatificationDialog();
extern void WriteAsmJumperOnTenshouExePrevMainPlayerCommandWait();
extern void WriteAsmJumperOnTenshouExeClickMainScreenMainCommand();
extern void WriteAsmJumperOnTenshouExeClickMainScreenSubCommand();
extern void WriteAsmJumperOnTenshouExeClickMainScreenSeikanCommand();
extern void WriteAsmJumperOnTenshouExeLeavePlayerGame();
extern void WriteAsmJumperOnTenshouExeAfterChangeWindowSize();
extern void WriteAsmJumperOnTenshouExeWarBGMCheck();
extern void WriteAsmJumperOnTenshouExeUIPaintOfMainScreenBackGround();
extern void WriteAsmJumperOnTenshouExeFunctionPointerTableJump();
extern void WriteAsmJumperOnTenshouExeFukidashiLeftUpdateMsgPointer();
extern void WriteAsmJumperOnTenshouExeFukidashiRightUpdateMsgPointer();
extern void WriteAsmJumperOnTenshouExeDDrawDllWindowProcA(); // DDraw.dll��WinProc�⑫
extern void WriteAsmJumperOnTenshouExeSelectWindowSize();
extern void WriteAsmJumperOnTenshouExeCheckDiskSpace();
extern void WriteAsmJumperOnTenshouExeSelectSaveOrLoadNo();
extern void WriteAsmJumperOnTenshouExeShowingMibun1();
extern void WriteAsmJumperOnTenshouExeShowingMibun2();
extern void WriteAsmJumperOnTenshouExeShowingMibun3();
extern void WriteAsmJumperOnTenshouExeShowingMibun4();
extern void WriteAsmJumperOnTenshouExeShowingMibun5();
extern void WriteAsmJumperOnTenshouExeShowingMibun6_Before();
extern void WriteAsmJumperOnTenshouExeShowingMibun6();
extern void WriteAsmJumperOnTenshouExeShowingMibun7();
extern void WriteAsmJumperOnTenshouExeShowingMibun8();
extern void WriteAsmJumperOnTenshouExeShowingMibun9();
extern void WriteAsmJumperOnTenshouExeShowingMibun10();
extern void WriteAsmJumperOnTenshouExeShowingMibun11();
extern void WriteAsmJumperOnTenshouExeShowingJotai1();
extern void WriteAsmJumperOnTenshouExeShowingJotai2();
extern void WriteAsmJumperOnTenshouExeShowingJotai3();
extern void WriteAsmJumperOnTenshouExeJudgeResultOfPCNaiouInMain();
extern void WriteAsmJumperOnTenshouExeJudgeResultOfNPCNaiouInMain_Before();
extern void WriteAsmJumperOnTenshouExeJudgeResultOfNPCNaiouInMain();
extern void WriteAsmJumperOnTenshouExeBushouRetsudenTitlePrev();
extern void WriteAsmJumperOnTenshouExeBushouRetsudenTitle();
extern void WriteAsmJumperOnTenshouExeBushouRetsudenDetail();
extern void WriteAsmJumperOnTenshouExeKahouMeibutsuJitenTitle();
extern void WriteAsmJumperOnTenshouExeKahouMeibutsuJitenDetail();
extern void WriteAsmJumperOnTenshouExeCastleMeijoMonogatariTitle();
extern void WriteAsmJumperOnTenshouExeCastleMeijoMonogatariDetail();
extern void WriteAsmJumperOnTenshouExeResultOfMouseDriverInstalledInPC();
extern void WriteAsmJumperOnTenshouExeGundanTurnChangeAfter();
extern void WriteAsmJumperOnTenshouExeGunshiFukidashiMessage();
extern void WriteAsmJumperOnTenshouExeSetTurnInBattle();
extern void WriteAsmJumperOnTenshouExeSettingMovePowerInBattle();
extern void WriteAsmJumperOnTenshouExeViewingCastleRetsudenFunc();
extern void WriteAsmJumperOnTenshouExeMainWindowCastleButton();
extern void WriteAsmJumperOnTenshouExeTaikyakuCastleInstruction();
extern void WriteAsmJumperOnTenshouExeDaimyoAnsatsuMovieInMain();
extern void WriteAsmJumperOnTenshouExeViewingNaiseiExecutantsEtc5NingumiDialog();
extern void WriteAsmJumperOnTenshouExeViewingNaiseiExecutantsEtc5WakuNum();
extern void WriteAsmJumperOnTenshouExeShowingIe1();
extern void WriteAsmJumperOnTenshouExeShowingIe2();
extern void WriteAsmJumperOnTenshouExeDefenceBushouHeisuZeroTujou();
extern void WriteAsmJumperOnTenshouExeDefenceBushouHeisuZeroInIssei();
extern void WriteAsmJumperOnTenshouExeDefenceBushouHeisuZeroInIsseiLoop();
extern void WriteAsmJumperOnTenshouExeDefenceBushouHeisuZeroInTeppou();
extern void WriteAsmJumperOnTenshouExeDefenceBushouHeisuZeroInTaihou();
extern void WriteAsmJumperOnTenshouExeDecideMakeNewBushou();
extern void WriteAsmJumperOnTenshouExeSelectingScenarioAfter();
extern void WriteAsmJumperOnTenshouExeIncrementTaikiBushouCount();
extern void WriteAsmJumperOnTenshouExeIncrementTaikiBushouCountAfter();
// extern void WriteAsmJumperOnTenshouExeCommerceAngryJudge();

/*----------------------------------------------------------------*
 DDRAW�n����
 *----------------------------------------------------------------*/

bool isExeDirectDrawCreate = false;


void d_DirectDrawCreateExecute() {
	// �E�B���h�E�n���h���𓾂Ă���
	if ( !CommonGlobalHandle::tenshouWND ) {
		CommonGlobalHandle::curProcess = ::GetCurrentProcess();
		CommonGlobalHandle::tenshouWND = ::GetCurrentWindowHandle();
		CommonGlobalHandle::hWndTaskBar = ::FindWindow("Shell_TrayWnd", NULL); // �^�X�N�o�[�̃E�B���h�E
		if ( !CommonGlobalHandle::tenshouWND ) { CommonGlobalHandle::tenshouWND = FindWindow("Tenshouki95", NULL); }
		CommonGlobalHandle::tenshouThreadId = GetWindowThreadProcessId(CommonGlobalHandle::tenshouWND, NULL);

		// tenshou.exe�̃t���l�[��
		GetModuleFileName(NULL, CommonGlobalHandle::szTenshouExeFullPath, sizeof(CommonGlobalHandle::szTenshouExeFullPath) );
		// tenshou.exe�̕��̃��W���[���n���h��
		CommonGlobalHandle::hTenshouExe = GetModuleHandle(CommonGlobalHandle::szTenshouExeFullPath);

		// ���j���[�n���h���̃Z�b�g
		CommonGlobalHandle::hMenu =  GetMenu(CommonGlobalHandle::tenshouWND);

		// ���̃p�b�`�͓��ʂȂ̂ł��������⑫����B
		WriteAsmJumperOnTenshouExeDDrawDllWindowProcA();

		// �N�����̈ʒu��������B
		GetWindowRect( CommonGlobalHandle::tenshouWND, &CommonGlobalHandle::latestCenterWindowRect );

		// �}�E�X�����݂��Ȃ��Ă��A�N������悤�ɂ���B
		WriteAsmJumperOnTenshouExeResultOfMouseDriverInstalledInPC();

		// �V�X�e�����j���[�Ɂu�����z�u�v�Ƃ������ڂ�����
		addCenterItemInApplicationSystemMenu();

		// �������g�̃^�C���X�^���v���`�F�b�N����B
		CheckTimeStampMyself();

		// Bfile.nb6�ɑ��݂����`��������
		// bfile.n6p �� ���� NB6BushouExoption�Ɋi�[����B
		iExistRetudenNumOfBFileN6P = setBfileN6PToNB6BUSHOUEXOPTION();

		// message.n6p �� ���̂����A�ƕ󖼂�ƕ��`��NB6KahouExoption�Ɋi�[����B
		setBKahouN6PToNB6KAHOUEXOPTION();

		// hexmap.nb6�̃f�[�^���i�[
		// ��������f�t�H���g��hexmap.nb6���ƁA�������W�J���傫������tenshou.exe�����s�ł��Ȃ��Ȃ�̂ŁA�R�����g�A�E�g
		// setHexMapNB6ToHexMapArray();


		// �n���xMAX�ɂ���A���_�p�b�`�Őݒ肳��Ă���Atenshou.exe�̃o�C�i�����ύX����Ă���̂��Ƃ肠�������ւƖ߂��B
		InitFixExperienceDefault();

		// �����ŏn���x�������Ă����A���_�p�b�`�Őݒ肳��Ă�ƁAtenshou.exe�̃o�C�i�����ύX����Ă���̂łƂ肠�������ւƖ߂��B
		InitAutoUpExperienceDefault();

		// �Q�l�ɂȂ����Ƃ��A�M�����O�ɂȂ�����A�o�p�������ɁA�M�����O�ɂȂ����肷��̂�}���鏈���B
		InitKunkouIsNotResetOnRonin();

		// �Ō�̃��S�������B
		SetUnviewingEndingLogo();

		// �鋭�x�̐ݒ�
		// setHexMapNB6ToHexMapArray();

		// �V�t�g�L�[��������Ȃ���N�������̂ł���΁c
		if ( GetKeyState(VK_SHIFT) ) {
			CommonGlobalHandle::isShiftKey = true;
			CommonGlobalHandle::isNoDiskMode = true;

			// �L���ȓV�ċL�n�h���C�u��T���B
			// CheckValidLogicalDrive(); 20130407 ������Əd���̂ňꎞ�I�ɃR�����g�A�E�g

			// �m�[�f�B�X�N��
			NoCompactDisk();
		}

		// �R�c�����֘A�d�w�d�����s�B
		OnTenshouExeCallSoundModServer();
		CreateSoundFMap();

		// n6pwav�n�ǂݍ��񂾂̂ŁA�����łR�c���n������
		// ConstructX3DAudioThread();
	}

	// �ȉ��A�A�Z���u�����x���ł́ATENSHOU.EXE�ւ̏������������B
	// ���L�̊e�X�́ATENSHOU.EXE�̊e�X�̏ꏊ����ATSMod.dll���̑Ή�����󂯎M�֐�(__declspec naked WINAPI�n)�ւƒ��ڃW�����v����悤�ɁA�j�[���j�b�N�����������Ă��܂��B
	if ( !isExeDirectDrawCreate ) {

		isExeDirectDrawCreate = true;

		//----------------------------------------------
		// TSMod.txt�ŏ������p�b�`�̓K��
		TDModPatchProcessMemory(TSModReadMode::MAIN);
		//----------------------------------------------

		// TSMod.txt�Ŏw�肪�������ꍇ�ɃR���\�[���E�B���h�E�̍쐬�B
		// CreateAllocConsole();

		// �^�C�g���o�[�Ȃ��A���j���[�o�[�Ȃ��B
		setNoCaptionAndMenuBarBefore();

		// ���o�C���p�̉𑜓x�̐ݒ�`�F�b�N
		setMobileResolution();

		// ���C�h�𑜓x���[�h�̃`�F�b�N
		setWideResolution();


		// �������X�g�̃^�C�g�����C���B
		InitModifyBushouListTitle();

		// �E�B���h�E�T�C�Y��I��������
		WriteAsmJumperOnTenshouExeSelectWindowSize();

		// �E�B���h�E�T�C�Y�����ۂɐ؂�ւ�낤�Ƃ��Ă��鎞�A���t���X�N���[�����[�h
		WriteAsmJumperOnTenshouExeAfterChangeWindowSize();

		// LRESULT CALLBACK WindowProc�֐�������ʉ߂�����
		WriteAsmJumperOnTenshouExeCallBackWindowProc();

		// �f�B�X�N�X�y�[�X�̃`�F�b�N�̎��ɒʉ߁B�I�[�o�[�t���[�΍�
		WriteAsmJumperOnTenshouExeCheckDiskSpace();

	    // ��`�̐l���̂Ƃ���̃j�[���j�b�N��bfile.n6p�����ɏ��������B�����Jumper�ł͂Ȃ��B
		WriteAsmOnTenshouExeRetsuDenExistNumOfBFileN6P();

		// ��Ȃ����������g������p�b�`
		WriteAsmJumperOnNonFaceBushouIndependences();

		// sndata.n6p�̃f�[�^��Vector�Ɋi�[���Ă����B�����Jumper�ł͂Ȃ��B
		LoadSnDataN6PToVector();

		// taiki.n6p�̃f�[�^��Vector�Ɋi�[���Ă����B�����Jumper�ł͂Ȃ��B
		LoadSnTaikiBushouN6PToVector();

		// TENSHOU.EXE���X���b�Z�[�W�����[�h���I�������AOnTenshouExeMessageLoad��ʉ߂���悤��
		WriteAsmJumperOnTenshouExeMessageLoad();

		// TENSHOU.EXE���X���b�Z�[�W���������R�s�[���I�������AOnTenshouExeMessageCopy��ʉ߂���悤��
		WriteAsmJumperOnTenshouExeMessageCopy();

		// �R�t�̃��b�Z�[�W�ȂǁA���̏ꍇ�ƃ��b�Z�[�W�̃������̏������ވʒu���قȂ鎞	
		WriteAsmJumperOnTenshouExeGunshiFukidashiMessage();


		// �֐��|�C���^�̃W�����p�[��ʉ߂�����
		WriteAsmJumperOnTenshouExeFunctionPointerTableJump();

		// ---------------------------------------------------------------
		// �ȉ��͂��΂炭����V���[�Y
		// ---------------------------------------------------------------
		// TENSHOU.EXE ���b�Z�[�W���������ɃR�s�[������
		// WriteAsmJumperOnTenshouExeMessageCopyMem();

		// ���b�Z�[�W���ɑ���̖��O���K�v�ƂȂ������A����ID
		// �� WriteAsmJumperOnTenshouExeWsprintfOppositeIDAndStr();

		// �^�C�vB�^�̕��o����\�����钼�O�ɒʉ߂���ꏊ�B
		// WriteAsmJumperOnTenshouExeFukidashiTypeB_Left();
		//WriteAsmJumperOnTenshouExeFukidashiTypeB_Right();

		// �^�C�vB�^�̕��o��(�o�������ȂȂ���)�t�H�[�J�X���؂�ւ�����B
		// �� WriteAsmJumperOnTenshouExeFukidashiTypeBFocusChange();
		//
		// ---------------------------------------------------------------

		// ���o���ŁA�V�K�ł͂Ȃ��A�u�Z���t�v�����X�V����ۂɒʉ߂���֐��́u����v
		WriteAsmJumperOnTenshouExeFukidashiLeftUpdateMsgPointer();
		// ���o���ŁA�V�K�ł͂Ȃ��A�u�Z���t�v�����X�V����ۂɒʉ߂���֐��́u�E���v
		WriteAsmJumperOnTenshouExeFukidashiRightUpdateMsgPointer();

		// ��I�^�C�v�̐����o���̏��������Ή��B
		WriteAsmJumperOnTenshouExeHirouFukidashi();

		// ���l���̓_�C�A���O�Ō��肵����
		WriteAsmJumperOnTenshouExeInputNumberDialog();

		// �ǔF�_�C�A���O�n�Ŋm�F�{�^��������
		WriteAsmJumperOnTenshouExeConfirmRatificationDialog();

		// �������łɌR�c���s�������ǂ����̔���
		WriteAsmJumperOnTenshouExeJudgeGungiDone();

		// �R�c�����ǂ����̔���p�B
		WriteAsmJumperOnTenshouExeGundiDoing();

		// �R�c���ɁA�}�E�X���񂽂������R�c�ԍ����擾���邽�߁B
		WriteAsmJumperOnTenshouExeGungiGudanNo();
		// �R�c�^�C�v�̐����o���̏��������Ή��B
		WriteAsmJumperOnTenshouExeGungiFukidashi();

		// ���ꂩ�畬�o���̃t�H�[�J�X���ς�낤�Ƃ��Ă���A�܂��������}�b�v��̃��b�Z�[�W�͎��̃��b�Z�[�W�ւƐ؂�ւ���Ă��Ȃ��B
		WriteAsmJumperOnTenshouExeFukidashiTypeBMessageChange();

		// ���ꂩ�畬�o���̃t�H�[�J�X���J�n����悤�Ƃ��Ă���A�܂��������}�b�v��̃��b�Z�[�W�͎��̃��b�Z�[�W�ւƐ؂�ւ���Ă��Ȃ��B
		WriteAsmJumperOnTenshouExeFukidashiTypeBMessageStart();


		// TENSHOU.EXE��WM_MESSAGE�������������AOnTenshouExeWmMessage��ʉ߂���悤��
		WriteAsmJumperOnTenshouExeWmMessage();

		// ���[�r�[�̈������v�b�V�����ꂽ���AOnTenshouExeMovieWsprintfPush��ʉ߂���悤��
		WriteAsmJumperOnTenshouExeMovieWsprintfPush();
		// ���[�r�[���Đ�����Ă��鎞�A�T�C�Y��ʒu�𒲐�����B
		WriteAsmJumperOnTenshouExeMovieMoveWindow();

		// ���[�r�[�Đ����ɍ��o�b�N�ɂȂ邱�Ƃ̐���
		WriteAsmJumperOnTenshouExeMovieBlackBack1();
		WriteAsmJumperOnTenshouExeMovieBlackBack2();

		// ���[�r�[�Đ����ɂb�c�a�f�l���X�g�b�v���邱�Ƃ̐���
		WriteAsmJumperOnTenshouExeMovieBGMNonStop();

		// �U�̃V�i���I�^�C�g�����X�g���\������鎞
		WriteAsmJumperOnTenshouExeViewingScenarioTitleList();

		// �푈���ɐ��炷�a�f�l���A����̔ԍ��ȏォ�ȉ����Ńt�B���^�����O���Ă���A���S���Y��
		WriteAsmJumperOnTenshouExeWarBGMCheck();

		// �V�i���I�ڍו����\������鎞�B�㕔�E�����E����
		WriteAsmJumperOnTenshouExeWritingScenarioDetail1st();
		WriteAsmJumperOnTenshouExeWritingScenarioDetail2nd();
		WriteAsmJumperOnTenshouExeWritingScenarioDetail3rd();

		// �V�i���I�ڍא������\���̌�ŁA�V�i���I�喼�I���̑O�̃u���b�N�A�E�g��
		WriteAsmJumperOnTenshouExeSelectingScenarioAfter();

		// �V�i���I�I����̑喼�I����ʂ��\�������O�ɁAOnTenshouExeSelectingScenarioDaimyo��ʉ߂���悤��
		WriteAsmJumperOnTenshouExeSelectingScenarioDaimyo();

		// �V�i���I�I����̑喼�I����ʂ����肳��A���悢��V�i���I���Q�[���X�^�[�g�Ƃ������̊ԁB
		WriteAsmJumperOnTenshouExeEnteringFromScenarioDaimyo();
		WriteAsmJumperOnTenshouExeEnteringAfterScenarioDaimyo();

		// ���C���̃Q�[���J�n���AOnTenshouExMainGameStarte��ʉ߂���悤��
		WriteAsmJumperOnTenshouExeMainGameStart();

		// ���C����ʂŃv���C�����R�}���h����͂���T��
		WriteAsmJumperOnTenshouExePrevMainPlayerCommandWait();

		// �v���C�������C���R�}���h���͂ɐN��
		WriteAsmJumperOnTenshouExeClickMainScreenMainCommand();

		// �v���C�����T�u�R�}���h���͂ɐN��
		WriteAsmJumperOnTenshouExeClickMainScreenSubCommand();

		// �v���C�����ÊσR�}���h�ɐN��
		WriteAsmJumperOnTenshouExeClickMainScreenSeikanCommand();

		// �v���C���^�[������̒E�p(�Êς�����A���[�h������A�Q�[����������)
		WriteAsmJumperOnTenshouExeLeavePlayerGame();

		// ���C����ʒ��̌R�c�^�[�����؂�ւ�����ꍇ�B
		WriteAsmJumperOnTenshouExeGundanTurnChange();

		// ���C����ʒ��̌R�c�^�[����0xFFFF�ւƐ؂�ւ�������B
		WriteAsmJumperOnTenshouExeGundanTurnChangeAfter();

		// ���C����ʒ��̏�^�[�����؂�ς�����ꍇ�B
		WriteAsmJumperOnTenshouExeCastleTurnChange();

		// TENSHOU.EXE���v���C���[�Ƀ^�[�����܂���Ă������AOnTenshouExePlayerTurnChange��ʉ߂���悤��
		WriteAsmJumperOnTenshouExePlayerTurnChange();

		//  TENSHOU.EXE�����ꂪ�ύX�ƂȂ������AOnTenshouExeSobaChange��ʉ߂���悤��
		WriteAsmJumperOnTenshouExeSobaChange();
		WriteAsmJumperOnTenshouExeSobaGunChange();
		WriteAsmJumperOnTenshouExeSobaSoldierChange();
		WriteAsmJumperOnTenshouExeSobaHorseChange();

		// �ҋ@�������o�ꂷ��{���ɂ��̒��O
		WriteAsmJumperOnTenshouExeIncrementTaikiBushouCount();
		
		// �o�ꂵ�����������
		WriteAsmJumperOnTenshouExeIncrementTaikiBushouCountAfter();

		// �ҋ@�������o�ꂷ���A�̏������s���֐�
		WriteAsmJumperOnTenshouExeTaikiAppearance();


		// ���C����ʂł̓����̌��ʂ��i�[����鎞(PC)
		WriteAsmJumperOnTenshouExeJudgeResultOfPCNaiouInMain();

		// ���C����ʂœ����̂Q�l�̕����ԍ����E����B(NPC/PC)
		WriteAsmJumperOnTenshouExeJudgeResultOfNPCNaiouInMain_Before();

		// ���C����ʂł̓����̌��ʂ��i�[����鎞(NPC)
		WriteAsmJumperOnTenshouExeJudgeResultOfNPCNaiouInMain();


		// ���G�߂ŁA�������Ƃɐ퓬�\�̘͂V���̔N���r���Ȃ���鎞
		WriteAsmJumperOnTenshouExeCompareDeclineBat();
		// ���G�߂ŁA�������Ƃɒq�d�\�̘͂V���̔N���r���Ȃ���鎞
		WriteAsmJumperOnTenshouExeCompareDeclineInt();
		// ���G�߂ŁA�������Ƃɐ����\�̘͂V���̔N���r���Ȃ���鎞
		WriteAsmJumperOnTenshouExeCompareDeclineGov();

		// �����n���x�������������鎞
		WriteAsmJumperOnTenshouExeUpdateExperienceGov();
		// �퓬�n���x�������������鎞
		WriteAsmJumperOnTenshouExeUpdateExperienceBat();
		// �q�d�n���x�������������鎞
		WriteAsmJumperOnTenshouExeUpdateExperienceInt();


		// �퓬�ŏo�w�����̍ő吔�����肵�悤�Ƃ��鎞
		WriteAsmJumperOnTenshouExeSubstituteMaxShutujinNum();
		WriteAsmJumperOnTenshouExeSubstituteMaxShutujinMsg();
		WriteAsmJumperOnTenshouExeSubstituteMaxShutujinRemain1();
		WriteAsmJumperOnTenshouExeSubstituteMaxShutujinRemain2();

		// ���n�őS�����o�w�����ꍇ�A������o�w�ł��Ȃ���Ƃ������b�Z�[�W���o�Ă��܂������}����
		WriteAsmJumperOnTenshouExeSubstituteMaxShutujinCaseAll();


		// TENSHOU.EXE�����m����ύX���悤�Ƃ������AOnTenshouExeBushouSoldierMax��ʉ߂���悤��
		WriteAsmJumperOnTenshouExeBushouSoldierMax();

		// TENSHOU.EXE���V�ċL�̃X�N���[���I����ʂ�\�����悤�Ƃ������AOnTenshouExeViewingScreenMenu��ʉ߂���悤��
		WriteAsmJumperOnTenshouExeViewingScreenMenu();

		// TENSHOU.EXE���V�ċL�̍ŏ��̃g�b�v���j���[�I����\�����悤�Ƃ������AOnTenshouExeViewingTopMenu��ʉ߂���悤��
		WriteAsmJumperOnTenshouExeViewingTopMenu();

		// TENSHOU.EXE���V�ċL�̂U�̃V�i���I�I�����j���[�̑I�����ʂ�Ԃ�����
		WriteAsmJumperOnTenshouExeSelectingScenario();

		// TENSHOU.EXE���V�ċL�̃I�[�v�j���O�Ń��[�h�����s�������AOnTenshouExeSaveSaveDataInGame��ʉ߂���悤��
		WriteAsmJumperOnTenshouExeLoadSaveDataInOpening();

		// TENSHOU.EXE���V�ċL�̃Q�[�����ł̃��[�h�����s�������AOnTenshouExeLoadSaveDataInGame��ʉ߂���悤��
		WriteAsmJumperOnTenshouExeLoadSaveDataInGame();

		// TENSHOU.EXE���V�ċL�̃Q�[�����ł̃Z�[�u�����s�������AOnTenshouExeSaveSaveDataInGame��ʉ߂���悤��
		WriteAsmJumperOnTenshouExeSaveSaveDataInGame();

		// �Z�[�u�⃍�[�h��I�񂾎�
		WriteAsmJumperOnTenshouExeSelectSaveOrLoadNo();

		// �Z�[�u�⃍�[�h�������ɁA�V�i���I�̂h�c�����肳��鎞
		WriteAsmJumperOnTenshouExeDecisionScenarioIDOnSaveOrLoad();

		// TENSHOU.EXE�����ł̒������s�������A���ԂȂǂ̐����������ɂ���ĉ�������B���̂���WriteAsmJumperOnTenshouExeSoldierDraftConditionInBattle��ʉ߂���悤��
		WriteAsmJumperOnTenshouExeSoldierDraftConditionInBattle();

		// TENSHOU.EXE�����ł̒������s�������A�u100�v�Ƃ������ȃt�B���^�[���c���Ă��܂��B������������邽�߂�OnTenshouExeSoldierDraftFilterInBattle��ʉ߂���悤��
		WriteAsmJumperOnTenshouExeSoldierDraftFilterInBattle();

		// TENSHOU.EXE�Ő퓬���Ƀg�b�v�R�}���h���j���[��I�����悤�Ƃ��������ɁA�����ɑΉ�����֐��ւƗ���
		WriteAsmJumperOnTenshouExeViewingCommandMenuInBattle();

		// TENSHOU.EXE�Ő퓬���ɋZ�\���j���[��I�����悤�Ƃ��������ɁA�����ɑΉ�����֐��ւƗ���
		WriteAsmJumperOnTenshouExeSelectingAbilityMenuInBattle();
		// TENSHOU.EXE�Ő퓬���ɐ퓬���j���[��I�����悤�Ƃ��������ɁA�����ɑΉ�����֐��ւƗ���
		WriteAsmJumperOnTenshouExeSelectingAttackMenuInBattle();

		// NPC��(�v���C���x�z���̈ϔC��Ԋ܂�)�̑I���R�}���h
		WriteAsmJumperOnTenshouExeNPCSelectingAttackPrevMoveInBattle();
		WriteAsmJumperOnTenshouExeNPCSelectingAttackPostMoveInBattle();

		WriteAsmJumperOnTenshouExePCSelectingAttackPostMoveInBattle();
		WriteAsmJumperOnTenshouExeNPCSelectingAttackPostMoveAfterInBattle();


		// TENSHOU.EXE�Ő퓬���ɐ퓬�R�}���h��I�����悤�Ƃ��������ɁA�����ɑΉ�����֐��ւƗ���
		WriteAsmJumperOnTenshouExeViewingBattleMenuInBattle1();
		WriteAsmJumperOnTenshouExeViewingBattleMenuInBattle2();

		// TENSHOU.EXE�Ő퓬���ɂ��łɐ������������ǂ����̔���B
		WriteAsmJumperOnTenshouExeSendouOneOnceJudgement();

		// TENSHOU.EXE�Ő퓬���ǂ̐�����I�Ԃ���ύX����B
		WriteAsmJumperOnTenshouExeSendouSelectingWhich();

		// TENSHOU.EXE�Ő퓬���ɐ��������Ώۂ��P�l���ǂ����̔���t���O
		WriteAsmJumperOnTenshouExeSendouKonranIsOneOrAll();

		// TENSHOU.EXE�Ő퓬���ɑ��l���̐��������ΏۂŎ��Q�����������Ȃ�����
		WriteAsmJumperOnTenshouExeSendouKonranMultiKonran();

		// TENSHOU.EXE�Ő퓬���ɐ��������őS�������̎|�̃��b�Z�[�W������������^�C�~���O
		WriteAsmJumperOnTenshouExeSendouKonranAllMessage();

		// �u�Ɓv���\�������^�C�~���O�ŌĂяo�����̇@
		WriteAsmJumperOnTenshouExeShowingIe1();
		WriteAsmJumperOnTenshouExeShowingIe2();

		// �������X�g���݂悤�Ƃ����ۂɁA���������v�����ꂽ�B
		WriteAsmJumperOnTenshouExeViewingMainBushouListButaimei();

		// �����Ґ����[�h�ɓ������`�o��
		WriteAsmJumperOnTenshouExeEditingButai();

		// �����ҏW���悤�Ƃ������A���j�b�g�̕�����ނ�I�����Ē������O���v�����ꂽ
		WriteAsmJumperOnTenshouExeSelectingMainButaiHensyuUnit();

		// TENSHOU.EXE�ŕ����ڍ׏����������ɁA���������o��^�C�~���O
		WriteAsmJumperOnTenshouExeViewingBushouDetailButaimei();

		// TENSHOU.EXE�ŕ����ڍ׏����������ɁA�E��(�o��)���o��^�C�~���O�ŁAOnTenshouExeJobInBushouDetail��ʉ߂���悤��
		WriteAsmJumperOnTenshouExeViewingJobInBushouDetail();

		// TENSHOU.EXE�ŕ����ڍ׏����������ɁA�E��(�o��)���o��^�C�~���O�ŁAOnTenshouExeJobNoInBushouDetail��ʉ߂���悤��
		WriteAsmJumperOnTenshouExeViewingJobNoInBushouDetail();

		// TENSHOU.EXE�ŕ����ڍ׏����������ɁA�u�S�C�R�n���v���^�C�~���O�ŁAOnTenshouExeBushouDetailKibaTeppou(Judge)��ʉ߂���悤��
		WriteAsmJumperOnTenshouExeBushouDetailKibaTeppouJudge();
		WriteAsmJumperOnTenshouExeBushouDetailKibaTeppou();

		// �������X�g�́u�R�n�S�C�v�����̂Ƃ���ŁA�ʉ߂���悤�ɁB
		WriteAsmJumperOnTenshouExeViewingBushouListKibaTeppou();

		// �����́u�g���v���\�������^�C�~���O�Œʉ߂���悤�ɁB
		WriteAsmJumperOnTenshouExeShowingMibun1();
		WriteAsmJumperOnTenshouExeShowingMibun2();
		WriteAsmJumperOnTenshouExeShowingMibun3();
		WriteAsmJumperOnTenshouExeShowingMibun4();
		WriteAsmJumperOnTenshouExeShowingMibun5();
		WriteAsmJumperOnTenshouExeShowingMibun6_Before();
		WriteAsmJumperOnTenshouExeShowingMibun6();
		WriteAsmJumperOnTenshouExeShowingMibun7();
		WriteAsmJumperOnTenshouExeShowingMibun8();
		WriteAsmJumperOnTenshouExeShowingMibun9();
		WriteAsmJumperOnTenshouExeShowingMibun10();
		WriteAsmJumperOnTenshouExeShowingMibun11();


		// �����́u��ԁv���\�������^�C�~���O�Œʉ߂���悤�ɁB
		WriteAsmJumperOnTenshouExeShowingJotai1();
		WriteAsmJumperOnTenshouExeShowingJotai2();
		WriteAsmJumperOnTenshouExeShowingJotai3();

		// ������`���\������鎞�ɌĂяo�����B
		WriteAsmJumperOnTenshouExeBushouRetsudenTitlePrev();
		WriteAsmJumperOnTenshouExeBushouRetsudenTitle();
		WriteAsmJumperOnTenshouExeBushouRetsudenDetail();

		// �ƕ�_�C�A���O���傫�����������ɂ��K���ĕ`�悷��悤�ɁB
		WriteAsmJumperOnTenshouExeJudgeKahouDialogBigMove();


		// TENSHOU.EXE�ŉƕ�ڍ׏����������ɁA�ƕ�ID�̏����m�ۂ��邽��
		WriteAsmJumperOnTenshouExeViewingKahouDetail();

		// �ƕ��`�̕������u���������肷�邽�߂ɁB
		WriteAsmJumperOnTenshouExeKahouMeibutsuJitenTitle();
		WriteAsmJumperOnTenshouExeKahouMeibutsuJitenDetail();

		// ���`�̕������u���������肷�邽�߂ɁB
		WriteAsmJumperOnTenshouExeCastleMeijoMonogatariTitle();
		WriteAsmJumperOnTenshouExeCastleMeijoMonogatariDetail();

		// ���`���J�����O�ƒ���
		WriteAsmJumperOnTenshouExeViewingCastleRetsudenFunc();

		// ���C���E�B���h�E�́u�G�v�u��v�u���v�̏�{�^������������
		WriteAsmJumperOnTenshouExeMainWindowCastleButton();

		// TENSHOU.EXE�ŉƕ�ڍ׏����������ɁA�ƕ󃂁[�h��ON/OFF�̏����m�ۂ��邽��
		WriteAsmJumperOnTenshouExeShowingKahouDetail();

		// TENSHOU.EXE�̉ƕ�̃A�C�R�����v�b�V�����悤�Ƃ������B
		WriteAsmJumperOnTenshouExeDetailKahouGraphicPush();

		// TENSHOU.EXE�̉ƕ󃌃C�A�E�g���莞�̏���
		WriteAsmJumperOnTenshouExeDetailKahouLayout();

		// TENSHOU.EXE�ŉƕ�ڍ׏����������ɁA�ƕ��ޖ����߂鏈���̎�
		WriteAsmJumperOnTenshouExeViewingKahouDetailKahouType();

		// TENSHOU.EXE�ŉƕ�ꗗ���������ɁA�ƕ��ޖ����߂鏈���̎�
		WriteAsmJumperOnTenshouExeViewingKahouList();

		// �P���X�g���������̏����B��Ƃ��ĕ����ڍ׏�񂩂�u�P�v�������Č������̑Ώ�
		WriteAsmJumperOnTenshouExeJudgeHimeHanteiPrev();
		WriteAsmJumperOnTenshouExeJudgeHimeHanteiPost();

		// �P�ڍ׉�ʂ̕�����̕ύX��
		WriteAsmJumperOnTenshouExeViewingHimeDetail();
		WriteAsmJumperOnTenshouExeViewingHimeDetailMusume();
		WriteAsmJumperOnTenshouExeViewingHimeDetailTsuma();
		WriteAsmJumperOnTenshouExeViewingHimeDetailShimai();

		// �R�t�̕����ԍ����v�����ꂽ��
		WriteAsmJumperOnTenshouExeGetGunshiBushouIDPrev();
		WriteAsmJumperOnTenshouExeGetGunshiBushouIDAfter();



		// �喼�̌�p�҂̔ԍ����v�����ꂽ��
		WriteAsmJumperOnTenshouExeGetDaimyoKoukeishaBushouID();

		// TENSHOU.EXE�Ő퓬���A�J���~�������ɁA����̏�����t�œS�C�����Ă�悤�ɂ���A���̂��߂�OnTenshouExeRainRangeJudge��ʉ߂���悤��
		WriteAsmJumperOnTenshouExeRainRangeJudge();

		// TENSHOU.EXE�Ő퓬���A�S�C���������͋R�n�S�C���ł��邱�ƂŁA�����[�g�U�����\���ǂ����B�����𑀍삷�邱�ƂŁA�|����|�R�n�����o���B
		WriteAsmJumperOnTenshouExeCanAnyAttackDistance();
		WriteAsmJumperOnTenshouExeSetCanTaihouAndTekkousenDistance();
		WriteAsmJumperOnTenshouExeJudgeTeppouOrTeppouKiba();
		WriteAsmJumperOnTenshouExeJudgeTeppouOrTeppouTai();


		// TENSHOU.EXE�ňÎE�R�}���h�̔��莞�ɁAOnTenshouExeAnsatsuAttackInBattle��ʉ߂���悤�ɁA�A�Z���u�����𒼐�TENSHOU.EXE�̃������֏���
		WriteAsmJumperOnTenshouExeAnsatsuAttackInBattle();

		// �펞���ɁA�o�߃^�[�����������ꍇ��OnTenshouExeDecrementTurnInBattle��ʉ߂���悤��
		WriteAsmJumperOnTenshouExeDecrementTurnInBattle();
		WriteAsmJumperOnTenshouExeSetTurnInBattle();

		// �U��풆�ɁA�o�߃^�[�����������ꍇ�ɁA������ʉ߂���悤�ɁB
		WriteAsmJumperOnTenshouExeIncrementCastleTurnInBattle();

		// �푈��ʂ�U�钆��ʂŁA�R�c�^�[�����؂�ւ�������ɁA������ʉ߂���悤�ɁB
		WriteAsmJumperOnTenshouExeChangeGundanTurnInBattle();

		// �펞���ɁA�V���ȓV�󂪃Z�b�e�B���O�����^�C�~���O
		WriteAsmJumperOnTenshouExeChangeWeather();

		// �펞���ɁA�U���S���̌R�c���͂��؂�ւ��������OnTenshouExeGundanTurnChangeInBattle��ʉ߂���悤��
		WriteAsmJumperOnTenshouExePlayerGundanTurnEndInBattle();

		// ����H��ɂ����āAOnTenshouExeEmperorManeuvers��ʉ߂���悤��
		WriteAsmJumperOnTenshouExeEmperorManeuvers();

		// ���ɂ����āAOnTenshouExeDamagedOfDoor��ʉ߂���悤��
		WriteAsmJumperOnTenshouExeDamagedOfDoor();

		// ���C����ʂňÎE��������݂��ꍇ�̏���
		WriteAsmJumperOnTenshouExeAnsatsuBushouInMain();

		// �喼�ÎE�ɐ��������ۂɃ��[�r�[�����ꂽ��
		WriteAsmJumperOnTenshouExeDaimyoAnsatsuMovieInMain();

		// ���Ǝ��ɕِ�������ǂ������W���b�W���A���l�Ȃ�΁A�ِ�������Ȃ��Ă��A�ِ�������ƂƓ����Ƃ���B
		WriteAsmJumperOnTenshouExeCommerceBenzetsuJudge();

		// ���Ǝ��Ɍ��ʁA�����������ǂ����A���l�Ȃ�΁A100����������B
		WriteAsmJumperOnTenshouExeCommerceSuccessJudge();

		// ���Ǝ��ɏ��l�̃��b�Z�[�W
		WriteAsmJumperOnTenshouExeCommerceMessageJudge();

		// ���Ǝ��Ɍ��ʁA�����������ǂ����A���l�Ȃ�΁A100���{��Ȃ��B
		// WriteAsmJumperOnTenshouExeCommerceAngryJudge();

		// �Q�l���ɌM����d���N�����A�b�v�f�[�g���鏈��
		WriteAsmJumperOnTenshouExeUpdateToRoninKunkouParam();

		// �Q�l���ɌM����d���N�����A�b�v�f�[�g���鏈���B�푈�ŁB
		WriteAsmJumperOnTenshouExeUpdateToRoninOfWarKunkouParam();

		// �l�X�ȓo�p���ɁA�M����g�������A�b�v�f�[�g���鏈���B
		WriteAsmJumperOnTenshouExeUpdateToGennekiKunkouParam();

		// ���g�����ɁA�M����g���Ȃǂ��A�b�v�f�[�g���鏈���B
		WriteAsmJumperOnTenshouExeUpdateToHataageKunkouParam();

		// �{�ې苒�������ɁAOnTenshouExeCastleFallDown��ʉ߂���悤��
		WriteAsmJumperOnTenshouExeCastleFallDown();

		// �푈���v���C���[���݂Ŕ����������ɂ�����ʉ߂���B�֐����I���܂łɂ͐푈�I��
		WriteAsmJumperOnTenshouExeBattleStartOfPlayer();

		// �푈���m�o�b�݂Ŕ����������ɂ�����ʉ߂���B
		WriteAsmJumperOnTenshouExeBattleStartOfNPCPrevious();
		WriteAsmJumperOnTenshouExeBattleStartOfNPC();

		// �ߗ��̏����̍ۂɒʉ߂���B
		WriteAsmJumperOnTenshouExePostBattleTreatment();

		// �ߗ��������ɁA���ʐ��ۂ̍ۂ̒��O�ɒʉ߂���BPC/NPC���Ȃ��B���f���鑤�̑喼�̕����ԍ��𓾂邽�߁B
		WriteAsmJumperOnTenshouExeDaimyoBushouIDOfPostBattleTreatment();

		// �ߗ��������ɁA���ʐ��ۂ̍ۂɒʉ߂���BPC/NPC���Ȃ��B
		WriteAsmJumperOnTenshouExeSuccessOrFailOfPostBattleTreatment();

		// �푈���I�����������ƌ�B�����̔\�͂̑��������邽�߂ɊĎ�����B
		WriteAsmJumperOnTenshouExeBattleAfterOfNPC();

		// �S�C���`���C�x���g�̐���
		WriteAsmJumperOnTenshouExeTeppouDenraiEvent();

		// ��C�̔��ˉ񐔂��㏑������B
		WriteAsmJumperOnTenshouExeTaihouAttackTimesRewrite();

		// ��C�������͓S�b�D�̔��ˉ񐔂��㏑������B
		WriteAsmJumperOnTenshouExeTaihouAndTekkousenAttackTimesRewrite();

		// TENSHOU.EXE���ŋR�n�K�����ǂ܂ꂽ���ɏ㏑������B
		WriteAsmJumperOnTenshouExeAptitudeKiba();
		// TENSHOU.EXE���œS�C�K�����ǂ܂ꂽ���ɏ㏑������B
		WriteAsmJumperOnTenshouExeAptitudeTeppou();

		// ���ɂ����āA���݃��j�b�g���ǂ̂悤�Ȍ`�Ԃ����`�F�b�N
		WriteAsmJumperOnTenshouExeCurTekkousenFormCheck();
	
		// �푈�ɂ����āA�S�C�𔭎˂������ɂ��̏����ɗ����
		WriteAsmJumperOnTenshouExeFireTeppou();
		WriteAsmJumperOnTenshouExeFireTeppouSE();

		// �푈�ɂ����āA�卬�����ɍU���������Ƃ��ɂ��̏����ɂȂ����B��ɓS�C���|�ɑւ���������
		WriteAsmJumperOnTenshouExeFireKonranTeppouSE();


		// �퓬����C�����Ă邩�ǂ������p�b�`����
		WriteAsmJumperOnTenshouExeJudgeHasTaihou1();
		WriteAsmJumperOnTenshouExeJudgeHasTaihou2();
		WriteAsmJumperOnTenshouExeJudgeHasTaihou3();

		// �푈�ɂ����āA�_���[�W������������ɁA�_���[�W��}����
		WriteAsmJumperOnTenshouExeDamagedOfAttack();

		// �푈�ɂ����āA�ˌ��n��������(NPC�܂�)�A�ˌ����ꂽ���̎��S����̑���
		WriteAsmJumperOnTenshouExeTotsugekiResultTest();

		// �푈�ɂ����āA�S�C�������ꂽ���ɁA���S���邩�ǂ����̔���n�B
		WriteAsmJumperOnTenshouExeJudgeTeppouDeathInBattle();
		WriteAsmJumperOnTenshouExeTeppouDeathInBattle();

		// �푈�ɂ����ē���������������
		WriteAsmJumperOnTenshouExeResultOfNaiouSuccessInBattle();
		// �푈�ɂ����ē��������s������
		WriteAsmJumperOnTenshouExeResultOfNaiouFailInBattle();


		// �푈�ɂ����āA�u�G���𐶂��߂�����v�̃��b�Z�[�W�������^�C�~���O
		WriteAsmJumperOnTenshouExeIketottariInBattle();

		// �푈�ɂ����āA�u�G����ł��������v�̃��b�Z�[�W�������^�C�~���O
		WriteAsmJumperOnTenshouExeUchitottariInBattle();

		// �푈�ɂ����āA�s���ς݂ɂ���ݒ�̃^�C�~���O
		WriteAsmJumperOnTenshouExeOnActionEndInBattle();

		// �푈�ɂ����āA�ړ��͂��ω�����^�C�~���O
		WriteAsmJumperOnTenshouExeSettingMovePowerInBattle();

		// �푈�ɂ����āA�A�j���[�V������؂�Ԃ�ׂ����ǂ����̃^�C�~���O
		WriteAsmJumperOnTenshouExeMoveUnitAnimationChangeInBattle();
		// �푈�ɂ����āA�ړ����������ۂɁA�A�j���[�V�������~�߂邩�ǂ����B
		WriteAsmJumperOnTenshouExeMoveEndAnimationInBattle();
		// �푈�ɂ����āA���Ɉړ������I���Ă��邩�ǂ����A���肷��^�C�~���O
		WriteAsmJumperOnTenshouExeMoveEndJudgeInBattle();
		// �푈�ɂ����āA�L���ȃR�}���h�����s�������ǂ������A���肷��^�C�~���O
		WriteAsmJumperOnTenshouExeExecuteBattleCommandInBattle();
		// �푈�ɂ����āA�L���ȍU�������s�������ǂ������A���肷��^�C�~���O
		WriteAsmJumperOnTenshouExeExecuteBattleAttackInBattle();

		// ���ɂ����āA���j�b�g�𓮂����I������^�C�~���O�ł̏����B�����ԍ��𓾂邱�Ƃ��\�B
		WriteAsmJumperOnTenshouExeUnitMoveEnd();

		// �ď鑤�̢��������̃J�E���g���ʁB(�o�w���Ă��Ȃ����̂��܂߂�)
		WriteAsmJumperOnTenshouExeCheckRojoBushouNum();

		// �퓬�ɂ����āA�����̉�ʂ̐؂�ς��ځB����킾�����Ƃ��͑����āA��ԍ��𓾂�B
		WriteAsmJumperOnTenshouExeNextTurnIsFieldOrCastle();
		WriteAsmJumperOnTenshouExeGetNextTurnCastleID();

		// �ړ��c�ʂ����Z����鎞�B(���^���������炱�Ȃ��̂Œ���)
		WriteAsmJumperOnTenshouExeMoveRemainPowerInBattle();


		// ��I���̕������X�g�ꗗ���ɑ喼��ǉ�����
		WriteAsmJumperOnTenshouExeHirouSelectingBushouList();

		// ���y���Đ����悤�Ƃ�����
		WriteAsmJumperOnTenshouExeN6PAudioPlayTracking();

		// ���Α叫�R�Ƃ��ĕK�v�ȏ鐔�𖞂����Ă��邩�H
		WriteAsmJumperOnTenshouExeSeiiTaishoConditionCastleNum();

		// �G���f�B���O�̏��������ɍs������
		WriteAsmJumperOnTenshouExeEndingConditionTest();
		// �G���f�B���O���Ƀ_�C�A���O�̃^�C�g�����v�b�V�����鎞
		WriteAsmJumperOnTenshouExeEndingTitlePush();

		// �G���f�B���O�ɓ������B
		WriteAsmJumperOnTenshouExeEnteringDoEnding();

		// �G���f�B���O��A�����ݒ�ɖ߂鎞�̐ؑ֗p(�f�����[�h���烆�[�U�[�喼�I���ɂ��e������)
		WriteAsmJumperOnTenshouExeEndingAfterJumper();

		// �Ō�̃��S�̏I����҂B
		WriteAsmJumperOnTenshouExeEndLogoWaiting();

		// �Q�[���I�����̃�����������ɂ��Ȃ炸�ʂ�ꏊ
		WriteAsmJumperOnTenshouExeGlobalFreeMemoryInGameEnd();

		// �ʏ�U�����󂯂������̕������O�ɂȂ��āA�ދp�����鎞�ɒʂ�t�b�N
		WriteAsmJumperOnTenshouExeDefenceBushouHeisuZeroTujou();

		// ��čU�����󂯂������̕������O�ɂȂ��āA�ދp�����鎞�ɒʂ�t�b�N
		WriteAsmJumperOnTenshouExeDefenceBushouHeisuZeroInIssei();
		WriteAsmJumperOnTenshouExeDefenceBushouHeisuZeroInIsseiLoop();

		// �S�C�U�����󂯂������̕������O�ɂȂ��āA�ދp�����鎞�ɒʂ�t�b�N
		WriteAsmJumperOnTenshouExeDefenceBushouHeisuZeroInTeppou();

		// ��C�U�����󂯂������̕������O�ɂȂ��āA�ދp�����鎞�ɒʂ�t�b�N
		WriteAsmJumperOnTenshouExeDefenceBushouHeisuZeroInTaihou();


		// �������ދp���悤�Ƃ������̃t�b�N
		WriteAsmJumperOnTenshouExeTaikyakuTry();


		// �����ދp�ɏ�Q�����邩�Ȃ����̑�P�i�K�̃t�b�N�B�������W�����v����ƁA�ދp��̌����ւƈڂ�B
		// WriteAsmJumperOnTenshouExeTaikyakuCastleInstruction();

		// ��̃q�[�v�������̏ꏊ�̂h�n���ɒʉ߂���֐�();
		WriteAsmJumperOnTenshouExeGetPointerKaikenHeapMemory();


		// �푈���̈ϔC�������d�r�b�L�[�ŏo����悤��
		WriteAsmJumperOnTenshouExeJudgeWarCommissionCancel();
		// ���C���̂ł��������d�r�b�L�[�ŏo����悤��
		WriteAsmJumperOnTenshouExeJudgeMainCommissionCancel();

		// �Z�[�u�f�[�^�t�@�C�������v�b�V�����鎞�����s���ȃ^�C�~���O�B�ꉞ�t�b�N
		WriteAsmJumperOnTenshouExePushSaveFileNameOfUnknown();
		// �Z�[�u�f�[�^�t�@�C�������v�b�V�����鎞
		WriteAsmJumperOnTenshouExePushSaveFileName();

		// �V�K�����쐬�ŁA�u���s�v����������
		WriteAsmJumperOnTenshouExeDecideMakeNewBushou();

		// �獇���Ŋ��I�񂾎��ɁA�_�����I��ł����狸���B
		WriteAsmJumperOnTenshouExeDecideOfGeneralKaoFace();

		// �G�ߓ���UI�̕������y�C���g����鎞
		WriteAsmJumperOnTenshouExeUIPaintOfMainScreenBackGround();

		// �K�v�s���͂̃Z�b�g�B�_�p�b�`����̈ڐA
		InitNeedKoudouryoku();

		// �����n�̎��s�ғ��̃_�C�A���O���ł��^�C�~���O
		WriteAsmJumperOnTenshouExeViewingNaiseiExecutantsEtc5NingumiDialog();

		// �����n�̎��s�ғ��̃_�C�A���O���ł��������s�g�̐�
		WriteAsmJumperOnTenshouExeViewingNaiseiExecutantsEtc5WakuNum();

		// ���S�p�x�̎����ւ̉e�����J�b�g���邩�ǂ����B�_�p�b�`����̈ڐA
		SetDeathFrequencyNotInfluenceLife();

		// ��Q�R�c�ȉ��ɁA�R�}���h�̎w�����\�B�_�p�b�`����̈ڐA�B
		SetCanInstructDirectOtherGundan();

		// �����\�����������̕ύX�B�_�p�b�`����̈ڐA�B
		SetIncreaseRatioCanDraftSoldier();

		// ���̃_���[�W�����邩�ǂ����B�_�p�b�`����̈ڐA�B
		SetPatchMyDamageOfDoor();

		// �b�o�t��ł̏�h��͌W���B�_�p�b�`���̈ڐA�B
		SetCastleDefenceCoefOfNPC();

		// ���̂m�o�b�喼���A�����R�}���h�����s���邩�ۂ��B�_�p�b�`����̈ڐA�B
		SetOtherDaimyoNeverThreat();

		// ���G�ߎ��S����������s���B
		SetWorkToDieEverySeason();

		// �b�o�t�o��P���P�����ɂ���B
		SetAppearanceHimeBecomeBushou();

		// ����������ɉƐb�̒����x���~
		SetDegreeLoyaltyOnCaptiveLibra();

		// �_�p�b�`�̂a�f�l�̃g���b�N�ԍ��ύX�i�P�R�ȁA�R�W�ȁj
		SetChangeBGMTrackNum();

		// �P�Ɣ��肷��̂�500�l�ڈȍ~�ł͂Ȃ��A0�l�ڈȍ~�Ƃ���B
		SetHideRangePatch();

		// �w���v�t�@�C������DVD�ł̂��̂������ꍇ�̑Ώ�
		SetHelpFileName();

		// ��ƌo�H�̏��\�z�͂����B �Q�[���J�n��A�ēx�X�V���s����B
		InitCastleNodeAndEdge();

		// DDraw.dll�̃��T�C�Y�������Ȃ�
		patchDisableDDrawResizeFunction();

		// �t���[�P���j���̊�Ō딚�ŏo������̂��ɗ͗}����(�ɗ͂����}�����Ȃ�)
		patchDisableDDrawHimeWrongAppearBug();

		// ��O���̃t�@�C�������莞�ɌĂ΂��
		WriteDDrawDrawKaoFullColorFileName();

		// KaoSwap���g������Ă邩�`�F�b�N���A�g������Ă�����A������ς���B
		checkKaoSwapExtend();

		// ���j���[�̗L���E�����̐؂�ւ�
		setMenuEnableOrDisable();

		// �t�H���g�̃J�X�^���`�F�b�N
		setFontCustomCheck();

		// INMM�̃J�X�^���`�F�b�N
		setINMMCustomCheck();

		// �_�p�b�`�̃J�X�^���`�F�b�N
		setGodPatchCustomCheck();

		// �e��t�b�N����
		HookFunctions();


		// PluginMod��Initialize���s
		// �ǂݍ���DLL�Q�ɑ΂��āAOnInitialize������΁A��������s�Ȃ���Ή��
		for ( vector<HMODULE>::iterator it = listOfTargetDLL.begin(); it != listOfTargetDLL.end(); it++ ) {

			// DLL���ǂ߂��̂ł���΁AOnInitialize�֐������݂��邩���`�F�b�N�B
			PFNTARGETMODINITIALIZE p_OnInitialize = (PFNTARGETMODINITIALIZE)GetProcAddress(*it, "OnInitialize");

			// OnInitialize�����݂��Ȃ��̂ł���΁A�ΏۊO�B����B
			if ( !p_OnInitialize ) {
				continue;
			}

			((PFNTARGETMODINITIALIZE) p_OnInitialize)(CommonGlobalHandle::hDLLModule);
		}

		// ScenarioMod�̃R���X�g���N�^
		if ( p_snOnConstructor ) {
			p_snOnConstructor();
		}


	}
}





__declspec( naked ) void WINAPI d_DirectDrawCreate() { 

	_asm{ 
		call d_DirectDrawCreateExecute

		jmp p_DirectDrawCreate
	}
}
