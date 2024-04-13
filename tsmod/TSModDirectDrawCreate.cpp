

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
 アセンブラ書き換え系の Extern
 *----------------------------------------------------------------*/
// 非ジャンパー
extern void InitFixExperienceDefault();
extern void InitAutoUpExperienceDefault();

extern void WriteAsmJumperOnTenshouExeCallBackWindowProc();
extern void WriteAsmOnTenshouExeRetsuDenExistNumOfBFileN6P();
extern void WriteAsmJumperOnTenshouExeTeppouDenraiEvent(); // これはジャンパーではなく、リターン
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
extern void SetHideRangePatch(); // 姫と判定するのを500人目以降ではなく、0人目以降とする。
extern void InitModifyBushouListTitle();
extern void InitKunkouIsNotResetOnRonin();
extern void SetChangeBGMTrackNum();
extern void patchDisableDDrawResizeFunction();
extern void patchDisableDDrawHimeWrongAppearBug();
extern void WriteDDrawDrawKaoFullColorFileName();
extern void SetUnviewingEndingLogo();
extern int setHexMapNB6ToHexMapArray();
extern void setNoCaptionAndMenuBarBefore();


// ジャンパー
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
extern void WriteAsmJumperOnTenshouExeDDrawDllWindowProcA(); // DDraw.dllのWinProc補足
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
 DDRAW系処理
 *----------------------------------------------------------------*/

bool isExeDirectDrawCreate = false;


void d_DirectDrawCreateExecute() {
	// ウィンドウハンドルを得ておく
	if ( !CommonGlobalHandle::tenshouWND ) {
		CommonGlobalHandle::curProcess = ::GetCurrentProcess();
		CommonGlobalHandle::tenshouWND = ::GetCurrentWindowHandle();
		CommonGlobalHandle::hWndTaskBar = ::FindWindow("Shell_TrayWnd", NULL); // タスクバーのウィンドウ
		if ( !CommonGlobalHandle::tenshouWND ) { CommonGlobalHandle::tenshouWND = FindWindow("Tenshouki95", NULL); }
		CommonGlobalHandle::tenshouThreadId = GetWindowThreadProcessId(CommonGlobalHandle::tenshouWND, NULL);

		// tenshou.exeのフルネーム
		GetModuleFileName(NULL, CommonGlobalHandle::szTenshouExeFullPath, sizeof(CommonGlobalHandle::szTenshouExeFullPath) );
		// tenshou.exeの方のモジュールハンドル
		CommonGlobalHandle::hTenshouExe = GetModuleHandle(CommonGlobalHandle::szTenshouExeFullPath);

		// メニューハンドルのセット
		CommonGlobalHandle::hMenu =  GetMenu(CommonGlobalHandle::tenshouWND);

		// このパッチは特別なのでいち早く補足する。
		WriteAsmJumperOnTenshouExeDDrawDllWindowProcA();

		// 起動時の位置情報を入れる。
		GetWindowRect( CommonGlobalHandle::tenshouWND, &CommonGlobalHandle::latestCenterWindowRect );

		// マウスが存在しなくても、起動するようにする。
		WriteAsmJumperOnTenshouExeResultOfMouseDriverInstalledInPC();

		// システムメニューに「中央配置」という項目を入れる
		addCenterItemInApplicationSystemMenu();

		// 自分自身のタイムスタンプをチェックする。
		CheckTimeStampMyself();

		// Bfile.nb6に存在する列伝数を入れる
		// bfile.n6p の 情報を NB6BushouExoptionに格納する。
		iExistRetudenNumOfBFileN6P = setBfileN6PToNB6BUSHOUEXOPTION();

		// message.n6p の 情報のうち、家宝名や家宝列伝をNB6KahouExoptionに格納する。
		setBKahouN6PToNB6KAHOUEXOPTION();

		// hexmap.nb6のデータを格納
		// ★★これデフォルトのhexmap.nb6だと、メモリ展開が大きすぎてtenshou.exeが実行できなくなるので、コメントアウト
		// setHexMapNB6ToHexMapArray();


		// 熟練度MAXにする、が神パッチで設定されており、tenshou.exeのバイナリが変更されているのをとりあえず元へと戻す。
		InitFixExperienceDefault();

		// 自動で熟練度が増えていく、が神パッチで設定されてると、tenshou.exeのバイナリが変更されているのでとりあえず元へと戻す。
		InitAutoUpExperienceDefault();

		// 浪人になったとき、勲功が０になったり、登用した時に、勲功が０になったりするのを抑える処理。
		InitKunkouIsNotResetOnRonin();

		// 最後のロゴを消す。
		SetUnviewingEndingLogo();

		// 城強度の設定
		// setHexMapNB6ToHexMapArray();

		// シフトキーが押されながら起動したのであれば…
		if ( GetKeyState(VK_SHIFT) ) {
			CommonGlobalHandle::isShiftKey = true;
			CommonGlobalHandle::isNoDiskMode = true;

			// 有効な天翔記系ドライブを探す。
			// CheckValidLogicalDrive(); 20130407 ちょっと重いので一時的にコメントアウト

			// ノーディスク化
			NoCompactDisk();
		}

		// ３Ｄ音響関連ＥＸＥを実行。
		OnTenshouExeCallSoundModServer();
		CreateSoundFMap();

		// n6pwav系読み込んだので、ここで３Ｄ音系初期化
		// ConstructX3DAudioThread();
	}

	// 以下、アセンブラレベルでの、TENSHOU.EXEへの書き換え処理。
	// 下記の各々は、TENSHOU.EXEの各々の場所から、TSMod.dll内の対応する受け皿関数(__declspec naked WINAPI系)へと直接ジャンプするように、ニーモニックを書き換えてしまう。
	if ( !isExeDirectDrawCreate ) {

		isExeDirectDrawCreate = true;

		//----------------------------------------------
		// TSMod.txtで書いたパッチの適応
		TDModPatchProcessMemory(TSModReadMode::MAIN);
		//----------------------------------------------

		// TSMod.txtで指定があった場合にコンソールウィンドウの作成。
		// CreateAllocConsole();

		// タイトルバーなし、メニューバーなし。
		setNoCaptionAndMenuBarBefore();

		// モバイル用の解像度の設定チェック
		setMobileResolution();

		// ワイド解像度モードのチェック
		setWideResolution();


		// 武将リストのタイトルを修正。
		InitModifyBushouListTitle();

		// ウィンドウサイズを選択した時
		WriteAsmJumperOnTenshouExeSelectWindowSize();

		// ウィンドウサイズが実際に切り替わろうとしている時、準フルスクリーンモード
		WriteAsmJumperOnTenshouExeAfterChangeWindowSize();

		// LRESULT CALLBACK WindowProc関数相当を通過した時
		WriteAsmJumperOnTenshouExeCallBackWindowProc();

		// ディスクスペースのチェックの時に通過。オーバーフロー対策
		WriteAsmJumperOnTenshouExeCheckDiskSpace();

	    // 列伝の人数のところのニーモニックをbfile.n6pを元に書き換え。これはJumperではない。
		WriteAsmOnTenshouExeRetsuDenExistNumOfBFileN6P();

		// 顔なし武将が旗揚げするパッチ
		WriteAsmJumperOnNonFaceBushouIndependences();

		// sndata.n6pのデータをVectorに格納しておく。これはJumperではない。
		LoadSnDataN6PToVector();

		// taiki.n6pのデータをVectorに格納しておく。これはJumperではない。
		LoadSnTaikiBushouN6PToVector();

		// TENSHOU.EXEが個々メッセージをロードし終えた時、OnTenshouExeMessageLoadを通過するように
		WriteAsmJumperOnTenshouExeMessageLoad();

		// TENSHOU.EXEが個々メッセージをメモリコピーし終えた時、OnTenshouExeMessageCopyを通過するように
		WriteAsmJumperOnTenshouExeMessageCopy();

		// 軍師のメッセージなど、他の場合とメッセージのメモリの書き込む位置が異なる時	
		WriteAsmJumperOnTenshouExeGunshiFukidashiMessage();


		// 関数ポインタのジャンパーを通過した時
		WriteAsmJumperOnTenshouExeFunctionPointerTableJump();

		// ---------------------------------------------------------------
		// 以下はしばらく封印シリーズ
		// ---------------------------------------------------------------
		// TENSHOU.EXE メッセージをメモリにコピーした時
		// WriteAsmJumperOnTenshouExeMessageCopyMem();

		// メッセージ中に相手の名前が必要となった時、そのID
		// ★ WriteAsmJumperOnTenshouExeWsprintfOppositeIDAndStr();

		// タイプB型の噴出しを表示する直前に通過する場所。
		// WriteAsmJumperOnTenshouExeFukidashiTypeB_Left();
		//WriteAsmJumperOnTenshouExeFukidashiTypeB_Right();

		// タイプB型の噴出し(出っ放しななかで)フォーカスが切り替わった。
		// ★ WriteAsmJumperOnTenshouExeFukidashiTypeBFocusChange();
		//
		// ---------------------------------------------------------------

		// 噴出しで、新規ではなく、「セリフ」だけ更新する際に通過する関数の「左上」
		WriteAsmJumperOnTenshouExeFukidashiLeftUpdateMsgPointer();
		// 噴出しで、新規ではなく、「セリフ」だけ更新する際に通過する関数の「右下」
		WriteAsmJumperOnTenshouExeFukidashiRightUpdateMsgPointer();

		// 披露タイプの吹き出しの書き換え対応。
		WriteAsmJumperOnTenshouExeHirouFukidashi();

		// 数値入力ダイアログで決定した時
		WriteAsmJumperOnTenshouExeInputNumberDialog();

		// 追認ダイアログ系で確認ボタンした時
		WriteAsmJumperOnTenshouExeConfirmRatificationDialog();

		// 今期すでに軍議を行ったかどうかの判定
		WriteAsmJumperOnTenshouExeJudgeGungiDone();

		// 軍議中かどうかの判定用。
		WriteAsmJumperOnTenshouExeGundiDoing();

		// 軍議時に、マウスせんたくした軍団番号を取得するため。
		WriteAsmJumperOnTenshouExeGungiGudanNo();
		// 軍議タイプの吹き出しの書き換え対応。
		WriteAsmJumperOnTenshouExeGungiFukidashi();

		// これから噴出しのフォーカスが変わろうとしている、まだメモリマップ上のメッセージは次のメッセージへと切り替わっていない。
		WriteAsmJumperOnTenshouExeFukidashiTypeBMessageChange();

		// これから噴出しのフォーカスが開始されようとしている、まだメモリマップ上のメッセージは次のメッセージへと切り替わっていない。
		WriteAsmJumperOnTenshouExeFukidashiTypeBMessageStart();


		// TENSHOU.EXEがWM_MESSAGEを処理した時、OnTenshouExeWmMessageを通過するように
		WriteAsmJumperOnTenshouExeWmMessage();

		// ムービーの引数がプッシュされた時、OnTenshouExeMovieWsprintfPushを通過するように
		WriteAsmJumperOnTenshouExeMovieWsprintfPush();
		// ムービーが再生されている時、サイズや位置を調整する。
		WriteAsmJumperOnTenshouExeMovieMoveWindow();

		// ムービー再生時に黒バックになることの制御
		WriteAsmJumperOnTenshouExeMovieBlackBack1();
		WriteAsmJumperOnTenshouExeMovieBlackBack2();

		// ムービー再生時にＣＤＢＧＭがストップすることの制御
		WriteAsmJumperOnTenshouExeMovieBGMNonStop();

		// ６つのシナリオタイトルリストが表示される時
		WriteAsmJumperOnTenshouExeViewingScenarioTitleList();

		// 戦争時に成らすＢＧＭを、特定の番号以上か以下かでフィルタリングしているアルゴリズム
		WriteAsmJumperOnTenshouExeWarBGMCheck();

		// シナリオ詳細文が表示される時。上部・中部・下部
		WriteAsmJumperOnTenshouExeWritingScenarioDetail1st();
		WriteAsmJumperOnTenshouExeWritingScenarioDetail2nd();
		WriteAsmJumperOnTenshouExeWritingScenarioDetail3rd();

		// シナリオ詳細説明文表示の後で、シナリオ大名選択の前のブラックアウト中
		WriteAsmJumperOnTenshouExeSelectingScenarioAfter();

		// シナリオ選択後の大名選択画面が表示される前に、OnTenshouExeSelectingScenarioDaimyoを通過するように
		WriteAsmJumperOnTenshouExeSelectingScenarioDaimyo();

		// シナリオ選択後の大名選択画面も決定され、いよいよシナリオ→ゲームスタートという時の間。
		WriteAsmJumperOnTenshouExeEnteringFromScenarioDaimyo();
		WriteAsmJumperOnTenshouExeEnteringAfterScenarioDaimyo();

		// メインのゲーム開始時、OnTenshouExMainGameStarteを通過するように
		WriteAsmJumperOnTenshouExeMainGameStart();

		// メイン画面でプレイヤがコマンドを入力する控え
		WriteAsmJumperOnTenshouExePrevMainPlayerCommandWait();

		// プレイヤがメインコマンド入力に侵入
		WriteAsmJumperOnTenshouExeClickMainScreenMainCommand();

		// プレイヤがサブコマンド入力に侵入
		WriteAsmJumperOnTenshouExeClickMainScreenSubCommand();

		// プレイヤが静観コマンドに侵入
		WriteAsmJumperOnTenshouExeClickMainScreenSeikanCommand();

		// プレイヤターンからの脱却(静観したり、ロードしたり、ゲーム抜けたり)
		WriteAsmJumperOnTenshouExeLeavePlayerGame();

		// メイン画面中の軍団ターンが切り替わった場合。
		WriteAsmJumperOnTenshouExeGundanTurnChange();

		// メイン画面中の軍団ターンが0xFFFFへと切り替わった時。
		WriteAsmJumperOnTenshouExeGundanTurnChangeAfter();

		// メイン画面中の城ターンが切り変わった場合。
		WriteAsmJumperOnTenshouExeCastleTurnChange();

		// TENSHOU.EXEがプレイヤーにターンがまわってきた時、OnTenshouExePlayerTurnChangeを通過するように
		WriteAsmJumperOnTenshouExePlayerTurnChange();

		//  TENSHOU.EXEが相場が変更となった時、OnTenshouExeSobaChangeを通過するように
		WriteAsmJumperOnTenshouExeSobaChange();
		WriteAsmJumperOnTenshouExeSobaGunChange();
		WriteAsmJumperOnTenshouExeSobaSoldierChange();
		WriteAsmJumperOnTenshouExeSobaHorseChange();

		// 待機武将が登場する本当にその直前
		WriteAsmJumperOnTenshouExeIncrementTaikiBushouCount();
		
		// 登場しちゃった直後
		WriteAsmJumperOnTenshouExeIncrementTaikiBushouCountAfter();

		// 待機武将が登場する一連の処理を行う関数
		WriteAsmJumperOnTenshouExeTaikiAppearance();


		// メイン画面での内応の結果が格納される時(PC)
		WriteAsmJumperOnTenshouExeJudgeResultOfPCNaiouInMain();

		// メイン画面で内応の２人の武将番号が拾える。(NPC/PC)
		WriteAsmJumperOnTenshouExeJudgeResultOfNPCNaiouInMain_Before();

		// メイン画面での内応の結果が格納される時(NPC)
		WriteAsmJumperOnTenshouExeJudgeResultOfNPCNaiouInMain();


		// 毎季節で、武将ごとに戦闘能力の老いの年齢比較がなされる時
		WriteAsmJumperOnTenshouExeCompareDeclineBat();
		// 毎季節で、武将ごとに智謀能力の老いの年齢比較がなされる時
		WriteAsmJumperOnTenshouExeCompareDeclineInt();
		// 毎季節で、武将ごとに政治能力の老いの年齢比較がなされる時
		WriteAsmJumperOnTenshouExeCompareDeclineGov();

		// 政治熟練度が書き換えられる時
		WriteAsmJumperOnTenshouExeUpdateExperienceGov();
		// 戦闘熟練度が書き換えられる時
		WriteAsmJumperOnTenshouExeUpdateExperienceBat();
		// 智謀熟練度が書き換えられる時
		WriteAsmJumperOnTenshouExeUpdateExperienceInt();


		// 戦闘で出陣武将の最大数を決定しようとする時
		WriteAsmJumperOnTenshouExeSubstituteMaxShutujinNum();
		WriteAsmJumperOnTenshouExeSubstituteMaxShutujinMsg();
		WriteAsmJumperOnTenshouExeSubstituteMaxShutujinRemain1();
		WriteAsmJumperOnTenshouExeSubstituteMaxShutujinRemain2();

		// 平地で全員が出陣した場合、｢もう出陣できない｣というメッセージが出てしまう流れを抑える
		WriteAsmJumperOnTenshouExeSubstituteMaxShutujinCaseAll();


		// TENSHOU.EXEが兵士数を変更しようとした時、OnTenshouExeBushouSoldierMaxを通過するように
		WriteAsmJumperOnTenshouExeBushouSoldierMax();

		// TENSHOU.EXEが天翔記のスクリーン選択画面を表示しようとした時、OnTenshouExeViewingScreenMenuを通過するように
		WriteAsmJumperOnTenshouExeViewingScreenMenu();

		// TENSHOU.EXEが天翔記の最初のトップメニュー選択を表示しようとした時、OnTenshouExeViewingTopMenuを通過するように
		WriteAsmJumperOnTenshouExeViewingTopMenu();

		// TENSHOU.EXEが天翔記の６つのシナリオ選択メニューの選択結果を返した時
		WriteAsmJumperOnTenshouExeSelectingScenario();

		// TENSHOU.EXEが天翔記のオープニングでロードを実行した時、OnTenshouExeSaveSaveDataInGameを通過するように
		WriteAsmJumperOnTenshouExeLoadSaveDataInOpening();

		// TENSHOU.EXEが天翔記のゲーム内でのロードを実行した時、OnTenshouExeLoadSaveDataInGameを通過するように
		WriteAsmJumperOnTenshouExeLoadSaveDataInGame();

		// TENSHOU.EXEが天翔記のゲーム内でのセーブを実行した時、OnTenshouExeSaveSaveDataInGameを通過するように
		WriteAsmJumperOnTenshouExeSaveSaveDataInGame();

		// セーブやロードを選んだ時
		WriteAsmJumperOnTenshouExeSelectSaveOrLoadNo();

		// セーブやロードした時に、シナリオのＩＤが決定される時
		WriteAsmJumperOnTenshouExeDecisionScenarioIDOnSaveOrLoad();

		// TENSHOU.EXEが戦場での徴兵を行った時、兵態などの制限を条件によって解除する。このためWriteAsmJumperOnTenshouExeSoldierDraftConditionInBattleを通過するように
		WriteAsmJumperOnTenshouExeSoldierDraftConditionInBattle();

		// TENSHOU.EXEが戦場での徴兵を行った時、「100」という妙なフィルターが残ってしまう。これを除去するためのOnTenshouExeSoldierDraftFilterInBattleを通過するように
		WriteAsmJumperOnTenshouExeSoldierDraftFilterInBattle();

		// TENSHOU.EXEで戦闘中にトップコマンドメニューを選択しようとしたさいに、これらに対応する関数へと流す
		WriteAsmJumperOnTenshouExeViewingCommandMenuInBattle();

		// TENSHOU.EXEで戦闘中に技能メニューを選択しようとしたさいに、これらに対応する関数へと流す
		WriteAsmJumperOnTenshouExeSelectingAbilityMenuInBattle();
		// TENSHOU.EXEで戦闘中に戦闘メニューを選択しようとしたさいに、これらに対応する関数へと流す
		WriteAsmJumperOnTenshouExeSelectingAttackMenuInBattle();

		// NPCが(プレイヤ支配下の委任状態含む)の選択コマンド
		WriteAsmJumperOnTenshouExeNPCSelectingAttackPrevMoveInBattle();
		WriteAsmJumperOnTenshouExeNPCSelectingAttackPostMoveInBattle();

		WriteAsmJumperOnTenshouExePCSelectingAttackPostMoveInBattle();
		WriteAsmJumperOnTenshouExeNPCSelectingAttackPostMoveAfterInBattle();


		// TENSHOU.EXEで戦闘中に戦闘コマンドを選択しようとしたさいに、これらに対応する関数へと流す
		WriteAsmJumperOnTenshouExeViewingBattleMenuInBattle1();
		WriteAsmJumperOnTenshouExeViewingBattleMenuInBattle2();

		// TENSHOU.EXEで戦闘中にすでに煽動をしたかどうかの判定。
		WriteAsmJumperOnTenshouExeSendouOneOnceJudgement();

		// TENSHOU.EXEで戦闘中どの煽動を選ぶかを変更する。
		WriteAsmJumperOnTenshouExeSendouSelectingWhich();

		// TENSHOU.EXEで戦闘中に煽動混乱対象が１人かどうかの判定フラグ
		WriteAsmJumperOnTenshouExeSendouKonranIsOneOrAll();

		// TENSHOU.EXEで戦闘中に多人数の煽動混乱対象で自群を混乱させない制御
		WriteAsmJumperOnTenshouExeSendouKonranMultiKonran();

		// TENSHOU.EXEで戦闘中に煽動混乱で全員混乱の旨のメッセージを書き換えるタイミング
		WriteAsmJumperOnTenshouExeSendouKonranAllMessage();

		// 「家」が表示されるタイミングで呼び出すその①
		WriteAsmJumperOnTenshouExeShowingIe1();
		WriteAsmJumperOnTenshouExeShowingIe2();

		// 武将リストをみようとした際に、部隊名が要求された。
		WriteAsmJumperOnTenshouExeViewingMainBushouListButaimei();

		// 部隊編成モードに入った～出た
		WriteAsmJumperOnTenshouExeEditingButai();

		// 部隊編集しようとした時、ユニットの部隊種類を選択して長い名前が要求された
		WriteAsmJumperOnTenshouExeSelectingMainButaiHensyuUnit();

		// TENSHOU.EXEで武将詳細情報を見た時に、部隊名が出るタイミング
		WriteAsmJumperOnTenshouExeViewingBushouDetailButaimei();

		// TENSHOU.EXEで武将詳細情報を見た時に、職業(出自)が出るタイミングで、OnTenshouExeJobInBushouDetailを通過するように
		WriteAsmJumperOnTenshouExeViewingJobInBushouDetail();

		// TENSHOU.EXEで武将詳細情報を見た時に、職業(出自)が出るタイミングで、OnTenshouExeJobNoInBushouDetailを通過するように
		WriteAsmJumperOnTenshouExeViewingJobNoInBushouDetail();

		// TENSHOU.EXEで武将詳細情報を見た時に、「鉄砲騎馬隊」がタイミングで、OnTenshouExeBushouDetailKibaTeppou(Judge)を通過するように
		WriteAsmJumperOnTenshouExeBushouDetailKibaTeppouJudge();
		WriteAsmJumperOnTenshouExeBushouDetailKibaTeppou();

		// 武将リストの「騎馬鉄砲」相当のところで、通過するように。
		WriteAsmJumperOnTenshouExeViewingBushouListKibaTeppou();

		// 武将の「身分」が表示されるタイミングで通過するように。
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


		// 武将の「状態」が表示されるタイミングで通過するように。
		WriteAsmJumperOnTenshouExeShowingJotai1();
		WriteAsmJumperOnTenshouExeShowingJotai2();
		WriteAsmJumperOnTenshouExeShowingJotai3();

		// 武将列伝が表示される時に呼び出される。
		WriteAsmJumperOnTenshouExeBushouRetsudenTitlePrev();
		WriteAsmJumperOnTenshouExeBushouRetsudenTitle();
		WriteAsmJumperOnTenshouExeBushouRetsudenDetail();

		// 家宝ダイアログが大きく動いた時にも必ず再描画するように。
		WriteAsmJumperOnTenshouExeJudgeKahouDialogBigMove();


		// TENSHOU.EXEで家宝詳細情報を見た時に、家宝IDの情報を確保するため
		WriteAsmJumperOnTenshouExeViewingKahouDetail();

		// 家宝列伝の文字列を置き換えたりするために。
		WriteAsmJumperOnTenshouExeKahouMeibutsuJitenTitle();
		WriteAsmJumperOnTenshouExeKahouMeibutsuJitenDetail();

		// 城列伝の文字列を置き換えたりするために。
		WriteAsmJumperOnTenshouExeCastleMeijoMonogatariTitle();
		WriteAsmJumperOnTenshouExeCastleMeijoMonogatariDetail();

		// 城列伝を開く直前と直後
		WriteAsmJumperOnTenshouExeViewingCastleRetsudenFunc();

		// メインウィンドウの「絵」「城」「将」の城ボタンを押した時
		WriteAsmJumperOnTenshouExeMainWindowCastleButton();

		// TENSHOU.EXEで家宝詳細情報を見た時に、家宝モードのON/OFFの情報を確保するため
		WriteAsmJumperOnTenshouExeShowingKahouDetail();

		// TENSHOU.EXEの家宝のアイコンをプッシュしようとした時。
		WriteAsmJumperOnTenshouExeDetailKahouGraphicPush();

		// TENSHOU.EXEの家宝レイアウト決定時の処理
		WriteAsmJumperOnTenshouExeDetailKahouLayout();

		// TENSHOU.EXEで家宝詳細情報を見た時に、家宝種類名を定める処理の時
		WriteAsmJumperOnTenshouExeViewingKahouDetailKahouType();

		// TENSHOU.EXEで家宝一覧を見た時に、家宝種類名を定める処理の時
		WriteAsmJumperOnTenshouExeViewingKahouList();

		// 姫リストを見た時の処理。主として武将詳細情報から「姫」を押して見た時の対処
		WriteAsmJumperOnTenshouExeJudgeHimeHanteiPrev();
		WriteAsmJumperOnTenshouExeJudgeHimeHanteiPost();

		// 姫詳細画面の文字列の変更等
		WriteAsmJumperOnTenshouExeViewingHimeDetail();
		WriteAsmJumperOnTenshouExeViewingHimeDetailMusume();
		WriteAsmJumperOnTenshouExeViewingHimeDetailTsuma();
		WriteAsmJumperOnTenshouExeViewingHimeDetailShimai();

		// 軍師の武将番号が要求された時
		WriteAsmJumperOnTenshouExeGetGunshiBushouIDPrev();
		WriteAsmJumperOnTenshouExeGetGunshiBushouIDAfter();



		// 大名の後継者の番号が要求された時
		WriteAsmJumperOnTenshouExeGetDaimyoKoukeishaBushouID();

		// TENSHOU.EXEで戦闘中、雨が降った時に、特定の条件を付で鉄砲を撃てるようにする、そのためのOnTenshouExeRainRangeJudgeを通過するように
		WriteAsmJumperOnTenshouExeRainRangeJudge();

		// TENSHOU.EXEで戦闘中、鉄砲隊もしくは騎馬鉄砲隊であることで、リモート攻撃が可能かどうか。ここを操作することで、弓隊や弓騎馬を作り出す。
		WriteAsmJumperOnTenshouExeCanAnyAttackDistance();
		WriteAsmJumperOnTenshouExeSetCanTaihouAndTekkousenDistance();
		WriteAsmJumperOnTenshouExeJudgeTeppouOrTeppouKiba();
		WriteAsmJumperOnTenshouExeJudgeTeppouOrTeppouTai();


		// TENSHOU.EXEで暗殺コマンドの判定時に、OnTenshouExeAnsatsuAttackInBattleを通過するように、アセンブラをを直接TENSHOU.EXEのメモリへ書く
		WriteAsmJumperOnTenshouExeAnsatsuAttackInBattle();

		// 戦時中に、経過ターンが増えた場合にOnTenshouExeDecrementTurnInBattleを通過するように
		WriteAsmJumperOnTenshouExeDecrementTurnInBattle();
		WriteAsmJumperOnTenshouExeSetTurnInBattle();

		// 攻城戦中に、経過ターンが増えた場合に、ここを通過するように。
		WriteAsmJumperOnTenshouExeIncrementCastleTurnInBattle();

		// 戦争画面や攻城中画面で、軍団ターンが切り替わった時に、ここを通過するように。
		WriteAsmJumperOnTenshouExeChangeGundanTurnInBattle();

		// 戦時中に、新たな天候がセッティングされるタイミング
		WriteAsmJumperOnTenshouExeChangeWeather();

		// 戦時中に、攻撃担当の軍団勢力が切り替わった時にOnTenshouExeGundanTurnChangeInBattleを通過するように
		WriteAsmJumperOnTenshouExePlayerGundanTurnEndInBattle();

		// 朝廷工作において、OnTenshouExeEmperorManeuversを通過するように
		WriteAsmJumperOnTenshouExeEmperorManeuvers();

		// 門壊において、OnTenshouExeDamagedOfDoorを通過するように
		WriteAsmJumperOnTenshouExeDamagedOfDoor();

		// メイン画面で暗殺をこころみた場合の処理
		WriteAsmJumperOnTenshouExeAnsatsuBushouInMain();

		// 大名暗殺に成功した際にムービーが流れた時
		WriteAsmJumperOnTenshouExeDaimyoAnsatsuMovieInMain();

		// 商業時に弁舌をもつかどうかをジャッジし、商人ならば、弁舌をもたなくても、弁舌をもつことと同じとする。
		WriteAsmJumperOnTenshouExeCommerceBenzetsuJudge();

		// 商業時に結果、成功したかどうか、商人ならば、100％成功する。
		WriteAsmJumperOnTenshouExeCommerceSuccessJudge();

		// 商業時に商人のメッセージ
		WriteAsmJumperOnTenshouExeCommerceMessageJudge();

		// 商業時に結果、成功したかどうか、商人ならば、100％怒らない。
		// WriteAsmJumperOnTenshouExeCommerceAngryJudge();

		// 浪人時に勲功や仕官年数をアップデートする処理
		WriteAsmJumperOnTenshouExeUpdateToRoninKunkouParam();

		// 浪人時に勲功や仕官年数をアップデートする処理。戦争版。
		WriteAsmJumperOnTenshouExeUpdateToRoninOfWarKunkouParam();

		// 様々な登用時に、勲功や身分等をアップデートする処理。
		WriteAsmJumperOnTenshouExeUpdateToGennekiKunkouParam();

		// 旗揚げ時に、勲功や身分などをアップデートする処理。
		WriteAsmJumperOnTenshouExeUpdateToHataageKunkouParam();

		// 本丸占拠した時に、OnTenshouExeCastleFallDownを通過するように
		WriteAsmJumperOnTenshouExeCastleFallDown();

		// 戦争がプレイヤー絡みで発生した時にここを通過する。関数が終わるまでには戦争終了
		WriteAsmJumperOnTenshouExeBattleStartOfPlayer();

		// 戦争がＮＰＣみで発生した時にここを通過する。
		WriteAsmJumperOnTenshouExeBattleStartOfNPCPrevious();
		WriteAsmJumperOnTenshouExeBattleStartOfNPC();

		// 捕虜の処遇の際に通過する。
		WriteAsmJumperOnTenshouExePostBattleTreatment();

		// 捕虜処遇時に、結果成否の際の直前に通過する。PC/NPC問わない。処断する側の大名の武将番号を得るため。
		WriteAsmJumperOnTenshouExeDaimyoBushouIDOfPostBattleTreatment();

		// 捕虜処遇時に、結果成否の際に通過する。PC/NPC問わない。
		WriteAsmJumperOnTenshouExeSuccessOrFailOfPostBattleTreatment();

		// 戦争が終わったちょっと後。武将の能力の増減を見るために監視する。
		WriteAsmJumperOnTenshouExeBattleAfterOfNPC();

		// 鉄砲が伝来イベントの制御
		WriteAsmJumperOnTenshouExeTeppouDenraiEvent();

		// 大砲の発射回数を上書きする。
		WriteAsmJumperOnTenshouExeTaihouAttackTimesRewrite();

		// 大砲もしくは鉄甲船の発射回数を上書きする。
		WriteAsmJumperOnTenshouExeTaihouAndTekkousenAttackTimesRewrite();

		// TENSHOU.EXE内で騎馬適正が読まれた時に上書きする。
		WriteAsmJumperOnTenshouExeAptitudeKiba();
		// TENSHOU.EXE内で鉄砲適正が読まれた時に上書きする。
		WriteAsmJumperOnTenshouExeAptitudeTeppou();

		// 戦場において、現在ユニットがどのような形態かをチェック
		WriteAsmJumperOnTenshouExeCurTekkousenFormCheck();
	
		// 戦争において、鉄砲を発射した時にこの処理に流れる
		WriteAsmJumperOnTenshouExeFireTeppou();
		WriteAsmJumperOnTenshouExeFireTeppouSE();

		// 戦争において、大混乱時に攻撃をしたときにこの処理にながれる。主に鉄砲→弓に替えたい処理
		WriteAsmJumperOnTenshouExeFireKonranTeppouSE();


		// 戦闘中大砲が撃てるかどうかをパッチする
		WriteAsmJumperOnTenshouExeJudgeHasTaihou1();
		WriteAsmJumperOnTenshouExeJudgeHasTaihou2();
		WriteAsmJumperOnTenshouExeJudgeHasTaihou3();

		// 戦争において、ダメージをくらった時に、ダメージを抑える
		WriteAsmJumperOnTenshouExeDamagedOfAttack();

		// 戦争において、突撃系をした際(NPC含む)、突撃された側の死亡判定の操作
		WriteAsmJumperOnTenshouExeTotsugekiResultTest();

		// 戦争において、鉄砲を撃たれた時に、死亡するかどうかの判定系。
		WriteAsmJumperOnTenshouExeJudgeTeppouDeathInBattle();
		WriteAsmJumperOnTenshouExeTeppouDeathInBattle();

		// 戦争において内応が成功した時
		WriteAsmJumperOnTenshouExeResultOfNaiouSuccessInBattle();
		// 戦争において内応が失敗した時
		WriteAsmJumperOnTenshouExeResultOfNaiouFailInBattle();


		// 戦争において、「敵将を生け捕ったり」のメッセージをいうタイミング
		WriteAsmJumperOnTenshouExeIketottariInBattle();

		// 戦争において、「敵将を打ち取ったり」のメッセージをいうタイミング
		WriteAsmJumperOnTenshouExeUchitottariInBattle();

		// 戦争において、行動済みにする設定のタイミング
		WriteAsmJumperOnTenshouExeOnActionEndInBattle();

		// 戦争において、移動力が変化するタイミング
		WriteAsmJumperOnTenshouExeSettingMovePowerInBattle();

		// 戦争において、アニメーションを切り返るべきかどうかのタイミング
		WriteAsmJumperOnTenshouExeMoveUnitAnimationChangeInBattle();
		// 戦争において、移動しをえた際に、アニメーションを止めるかどうか。
		WriteAsmJumperOnTenshouExeMoveEndAnimationInBattle();
		// 戦争において、既に移動をし終えているかどうか、判定するタイミング
		WriteAsmJumperOnTenshouExeMoveEndJudgeInBattle();
		// 戦争において、有効なコマンドを実行したかどうかを、判定するタイミング
		WriteAsmJumperOnTenshouExeExecuteBattleCommandInBattle();
		// 戦争において、有効な攻撃を実行したかどうかを、判定するタイミング
		WriteAsmJumperOnTenshouExeExecuteBattleAttackInBattle();

		// 戦場において、ユニットを動かし終わったタイミングでの処理。役割番号を得ることが可能。
		WriteAsmJumperOnTenshouExeUnitMoveEnd();

		// 籠城側の｢武将数｣のカウント結果。(出陣していないものも含める)
		WriteAsmJumperOnTenshouExeCheckRojoBushouNum();

		// 戦闘において、野戦や城の画面の切り変わり目。城内戦だったときは続いて、城番号を得る。
		WriteAsmJumperOnTenshouExeNextTurnIsFieldOrCastle();
		WriteAsmJumperOnTenshouExeGetNextTurnCastleID();

		// 移動残量が加算される時。(満タンだったらこないので注意)
		WriteAsmJumperOnTenshouExeMoveRemainPowerInBattle();


		// 披露時の武将リスト一覧時に大名を追加する
		WriteAsmJumperOnTenshouExeHirouSelectingBushouList();

		// 音楽を再生しようとした時
		WriteAsmJumperOnTenshouExeN6PAudioPlayTracking();

		// 征夷大将軍として必要な城数を満たしているか？
		WriteAsmJumperOnTenshouExeSeiiTaishoConditionCastleNum();

		// エンディングの条件を見に行った時
		WriteAsmJumperOnTenshouExeEndingConditionTest();
		// エンディング時にダイアログのタイトルをプッシュする時
		WriteAsmJumperOnTenshouExeEndingTitlePush();

		// エンディングに入った。
		WriteAsmJumperOnTenshouExeEnteringDoEnding();

		// エンディング後、初期設定に戻る時の切替用(デモモードからユーザー大名選択にも影響あり)
		WriteAsmJumperOnTenshouExeEndingAfterJumper();

		// 最後のロゴの終わりを待つ。
		WriteAsmJumperOnTenshouExeEndLogoWaiting();

		// ゲーム終了時のメモリ解放時にかならず通る場所
		WriteAsmJumperOnTenshouExeGlobalFreeMemoryInGameEnd();

		// 通常攻撃を受けた武将の兵数が０になって、退却等する時に通るフック
		WriteAsmJumperOnTenshouExeDefenceBushouHeisuZeroTujou();

		// 一斉攻撃を受けた武将の兵数が０になって、退却等する時に通るフック
		WriteAsmJumperOnTenshouExeDefenceBushouHeisuZeroInIssei();
		WriteAsmJumperOnTenshouExeDefenceBushouHeisuZeroInIsseiLoop();

		// 鉄砲攻撃を受けた武将の兵数が０になって、退却等する時に通るフック
		WriteAsmJumperOnTenshouExeDefenceBushouHeisuZeroInTeppou();

		// 大砲攻撃を受けた武将の兵数が０になって、退却等する時に通るフック
		WriteAsmJumperOnTenshouExeDefenceBushouHeisuZeroInTaihou();


		// 武将が退却しようとした時のフック
		WriteAsmJumperOnTenshouExeTaikyakuTry();


		// 武将退却に障害があるかないかの第１段階のフック。ここをジャンプすると、退却城の検索へと移る。
		// WriteAsmJumperOnTenshouExeTaikyakuCastleInstruction();

		// 会見のヒープメモリの場所のＩＯ時に通過する関数();
		WriteAsmJumperOnTenshouExeGetPointerKaikenHeapMemory();


		// 戦争中の委任解除をＥＳＣキーで出来るように
		WriteAsmJumperOnTenshouExeJudgeWarCommissionCancel();
		// メインのでも解除をＥＳＣキーで出来るように
		WriteAsmJumperOnTenshouExeJudgeMainCommissionCancel();

		// セーブデータファイル名をプッシュする時だが不明なタイミング。一応フック
		WriteAsmJumperOnTenshouExePushSaveFileNameOfUnknown();
		// セーブデータファイル名をプッシュする時
		WriteAsmJumperOnTenshouExePushSaveFileName();

		// 新規武将作成で、「実行」を押した時
		WriteAsmJumperOnTenshouExeDecideMakeNewBushou();

		// 顔合成で顔を選んだ時に、ダメ顔を選んでいたら矯正。
		WriteAsmJumperOnTenshouExeDecideOfGeneralKaoFace();

		// 季節等のUIの部分がペイントされる時
		WriteAsmJumperOnTenshouExeUIPaintOfMainScreenBackGround();

		// 必要行動力のセット。神パッチからの移植
		InitNeedKoudouryoku();

		// 内政系の実行者等のダイアログがでたタイミング
		WriteAsmJumperOnTenshouExeViewingNaiseiExecutantsEtc5NingumiDialog();

		// 内政系の実行者等のダイアログがでた武将実行枠の数
		WriteAsmJumperOnTenshouExeViewingNaiseiExecutantsEtc5WakuNum();

		// 死亡頻度の寿命への影響をカットするかどうか。神パッチからの移植
		SetDeathFrequencyNotInfluenceLife();

		// 第２軍団以下に、コマンドの指示が可能。神パッチからの移植。
		SetCanInstructDirectOtherGundan();

		// 徴兵可能数増加割合の変更。神パッチからの移植。
		SetIncreaseRatioCanDraftSoldier();

		// 門壊のダメージがあるかどうか。神パッチからの移植。
		SetPatchMyDamageOfDoor();

		// ＣＰＵ戦での城防御力係数。神パッチよりの移植。
		SetCastleDefenceCoefOfNPC();

		// 他のＮＰＣ大名が、脅迫コマンドを実行するか否か。神パッチからの移植。
		SetOtherDaimyoNeverThreat();

		// 毎季節死亡処理判定を行う。
		SetWorkToDieEverySeason();

		// ＣＰＵ登場姫も姫武将にする。
		SetAppearanceHimeBecomeBushou();

		// 武将解放時に家臣の忠誠度下降
		SetDegreeLoyaltyOnCaptiveLibra();

		// 神パッチのＢＧＭのトラック番号変更（１３曲、３８曲）
		SetChangeBGMTrackNum();

		// 姫と判定するのを500人目以降ではなく、0人目以降とする。
		SetHideRangePatch();

		// ヘルプファイル名がDVD版のものだった場合の対処
		SetHelpFileName();

		// 城と経路の情報構築はここ。 ゲーム開始後、再度更新が行われる。
		InitCastleNodeAndEdge();

		// DDraw.dllのリサイズを許さない
		patchDisableDDrawResizeFunction();

		// フリー姫が男性の顔で誤爆で出現するのを極力抑える(極力しか抑えられない)
		patchDisableDDrawHimeWrongAppearBug();

		// 顔グラのファイル名決定時に呼ばれる
		WriteDDrawDrawKaoFullColorFileName();

		// KaoSwapが拡張されてるかチェックし、拡張されていたら、挙動を変える。
		checkKaoSwapExtend();

		// メニューの有効・無効の切り替え
		setMenuEnableOrDisable();

		// フォントのカスタムチェック
		setFontCustomCheck();

		// INMMのカスタムチェック
		setINMMCustomCheck();

		// 神パッチのカスタムチェック
		setGodPatchCustomCheck();

		// 各種フック処理
		HookFunctions();


		// PluginModのInitialize実行
		// 読み込んだDLL群に対して、OnInitializeがあれば、それを実行なければ解放
		for ( vector<HMODULE>::iterator it = listOfTargetDLL.begin(); it != listOfTargetDLL.end(); it++ ) {

			// DLLが読めたのであれば、OnInitialize関数が存在するかをチェック。
			PFNTARGETMODINITIALIZE p_OnInitialize = (PFNTARGETMODINITIALIZE)GetProcAddress(*it, "OnInitialize");

			// OnInitializeが存在しないのであれば、対象外。解放。
			if ( !p_OnInitialize ) {
				continue;
			}

			((PFNTARGETMODINITIALIZE) p_OnInitialize)(CommonGlobalHandle::hDLLModule);
		}

		// ScenarioModのコンストラクタ
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
