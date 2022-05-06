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
extern bool CheckChangeYearDisplay(); // �����\������߂ċ����I�ɐ���\���ɐ؂�ւ���
extern void EXServieExecute_TenShougiCheck(); // �V�������v���C���Ă��Ȃ���΁A�m���œV�����̏���


// �v���C���[�̃^�[�����؂�ւ����(�����喼�S��)
void EXSuperService_OnPlayerTurnChange() {

	// �Q�[���f�[�^���v���C���[�^�[�����ς�����i�K�œǂݍ��ݍX�V���Ă���
	// ���̏����͂��̊֐��̍ŏ��ɕK���s������!!
	copyAllBushouSoldiersOriginToCopy();

	// �V�i���IMod�̃v���C���^�[���֐����Ăяo��
	if ( p_snOnPlayerTurnInMainScreen ) {
		((PFNSNPLAYERTURNINMAINSCREEN) p_snOnPlayerTurnInMainScreen)();
	}
}



// �v���C���[�̃^�[�����؂�ւ�肩�A�G�߂��ς����
// �v���C���[�̃^�[�����؂�ւ��������(EXSuperService_OnPlayerTurnChange)�͒ʂ�����ɂ����ɗ���B
void EXSuperService_OnSeasonChangeAndPlayerTurnChange() {

	// �b�葺��S�C���ӂȐl����鏈��
	EXServiceExecute_TeppouSpMakeSmith();

	// ���ۍ`���C������鏈��
	EXServiceExecute_PirateMakePort();

	// ���������ɓ������Ӑ}�I�Ɏ������鏈��
	EXServieExecute_NPCKengouBushouHasAKahouSword();

	// �V�����̏���
	EXServieExecute_TenShougiCheck();
}



// ���C���Q�[����ʂ��n�܂�����
void EXSuperService_OnMainGameStart() {

	// �����\���𐼗�\���ɂ���K�v�����邩�`�F�b�N(1454�N��ύX���Ă���ꍇ)
	CheckChangeYearDisplay();

	// �����̊g���E�Ƃ�ݒ肷��
	setJobEXOfCurAllBushou();

	// �S�C�֘A�̃C�x���g�̃R���g���[��
	CheckTeppouEventControll();

	// �R�c�ԍ����o�O���Ă�����A�����B(���V�i���I�G�f�B�^���R�c�̖�E�ԍ���15�Ɛݒ肵�Ă���o�O�𒼂��B
	patchBugOfGundanYakusyokuInSenarioEditor();

	// �����̏n���x���l�`�w�Œ�ł������ꍇ�̏���
	EXSuperService_IfFixExperienceMaxOnChangeTiming();

	// �����̗E�҂�ݒ肷��
	EXSuperService_IfBravesIsTheValue();

	// �����̎�����ݒ肷��
	EXSuperService_IfLifesIsTheValue();

	// �����̋`�����Œ�ɁA�Ɨ����ő�ɂ���B
	EXSuperService_IfNoDutiesAndMaxIndependences();

	// ���m���𑝂₷�܂��̎��O�v�Z(EXSuperService_Regeneration���O�ɂ��邱��!!)
	EXServiceExecuteRegeneration_All_SoldierCalcInMain();

	// �Q�[���X�^�[�g���ɁA���m�����񕜏������`�F�b�N�B���m���O���P�ɂ��邽��isForce�Ƃ��ĂP��n���B
	EXSuperService_Regeneration(1);

	// PluginMod�n�̂O�ȏ�i�����\�j�Ȃc�k�k�̃��C���Q�[���J�n���̃��\�b�h���Ăяo���B
	// ���s�ΏۂƂȂ��Ă���DLL�����ɑ{�����āc
	for ( vector<HMODULE>::iterator it = listOfTargetDLL.begin(); it != listOfTargetDLL.end(); it++ ) {

		// �Ώۂ�DLL��MainGameStart�֐������݂��邩���`�F�b�N�B
		PFNTARGETMODMAINGAMESTART p_MainGameStart = (PFNTARGETMODMAINGAMESTART)GetProcAddress(*it, "OnMainGameStart");

		if ( !p_MainGameStart ) {
			continue;
		}

		((PFNTARGETMODMAINGAMESTART) p_MainGameStart)();
	}

	// ScenarioMod���ɂ��郁�C���Q�[���J�n�����\�b�h���Ăяo���B
	if ( p_snOnMainGameStart ) {
		( (PFNSNONMAINGAMESTART ) p_snOnMainGameStart)();
	}


	// ��ƌo�H�̏��\�z�͂����B 
	InitCastleNodeAndEdge();
}


extern int iEnterOfClickMainScreenMainCommand;

// ���ꂪ�ύX�ƂȂ�����
void EXSuperService_OnSobaChange() {

	iEnterOfClickMainScreenMainCommand = 0;

	// ���ꂪ�ς�����i�K�ł�����Ă����K�v������B
	// �v���C���[�^�[��������Ă���Ԃł��󋵂��ς��̂ŁA�Q����K�v������B
	copyAllBushouSoldiersOriginToCopy();

	// �G�߂̕ς��ڂňÎE�J�E���g�����Z�b�g�����B
	EXSuperService_AnsatsuCntReset();

	// �G�߂̕ς��ڂŏn���l�̗ݐς����Z�b�g�����B
	EXSuperService_ExperienceLimitReset();

	// �R�c���Ƒ������ǂ������͒����x���オ��A���������͒����x��������
	EXSuperService_AishoCalc();

	// ����ύX���ɁA�n���x�����b�B����B(�Z�\���o����)
	EXSuperService_AutoUpExperienceAndTechOnSobaChange();

	// ���G�ߘQ�l�̌M���������Ă䂭
	EXSuperService_RoninKunkouDecrement();

	// �����̏n���x���l�`�w�Œ�ł������ꍇ�̏���
	EXSuperService_IfFixExperienceMaxOnChangeTiming();

	// ���ׂĂ̕����̗E�҂����̒l�ɏo����B�o�ꕐ�������l�B
	EXSuperService_IfBravesIsTheValue();

	// ���ׂĂ̕����̎��������̒l�ɏo����B�o�ꕐ�������l�B
	EXSuperService_IfLifesIsTheValue();

	// ���ׂĂ̕����̋`�����ŏ��ɁA�Ɨ����ő�ɂ���B�o�ꕐ�������l�B
	EXSuperService_IfNoDutiesAndMaxIndependences();

	// �������|�����݂��Ȃ�������Ƃ�������
	EXSuperService_IfYumiNoExists();

	// ���G�߁A���ƕĂ�]���ɒ���
	EXSuperService_CheckOtherDaimyosGetMoneyAndRiseOnSeasonChange();

	// �b��E�n�Y�E���ۍ`�Ȃǂ̏��ɏ]���āA���ꂼ��̍��ڂ𑝂₷
	EXSuperService_CheckIncreaseSpecialProductByTheThing();

	// �v���C����S�̂̃o�����X�ɏ]���Ċe���͂̎����E���͂ɒ���������
	EXSuperService_PlayerDaimyoDataInfluenceOtherDaimyoData();
	
	// ���m���𑝂₷�܂��̎��O�v�Z(EXSuperService_Regeneration���O�ɂ��邱��!!)
	EXServiceExecuteRegeneration_All_SoldierCalcInMain();

	// ����ύX���ɁA���m�����񕜏������`�F�b�N�B���m���O���P�ɂ��邽��isForce�Ƃ��ĂP��n���B
	EXSuperService_Regeneration(1);
}

// �V�i���I�喼�I�����̓��ꏈ�����s���B
void EXSuperService_OnSelectingScenarioAfter() {
	// �����̋Z�p�����J���ł������ꍇ�̏����B�P�񂾂������镐���̋Z�\�𔍒D
	EXSuperService_IfAbilityIsUnderdevelopedOnSelectingScenarioDaimyo();

	// �����̏n���x���l�`�w�Œ�ł������ꍇ�̏���
	EXSuperService_IfFixExperienceMaxOnChangeTiming();

	// �V�i���I�G�f�B�^�[�������Ă���A��R�c�ԍ����15�ԂƂȂ��Ă���o�O�𒼂��B
	patchBugOfGundanYakusyokuInSenarioEditor();

	// �S�C�`���C�x���g�R���g���[���[
	CheckTeppouEventControll();

	// �V�ċL�ȍ~�̕����̃p�����^�𔽉f����B
	SetBushouAvgParam(1); // 1�̓V�i���I�I����
	SetExBushouAvgParam(-1); // -1�͑S���ɑ΂��čs���Ƃ������ƁB���ꕐ���p

	// ������ScenarioMod.dll�ŁAsnOnSelectingScenarioDaimyo(void *)���\�b�h�������Ă���Ȃ�΁c
	if (p_snOnSelectingScenarioAfter) {
		// �Ƃ肠������UNULL����Ă����B
		((PFNSNONSELECTINGSCENARIOAFTER)p_snOnSelectingScenarioAfter)(NULL);
	}


}


// �V�i���I�喼�I�����̏���
void EXSuperService_OnSelectingScenarioDaimyo() {

	// ������PluginMod.dll�ŁAOnTenshouExeSelectingScenarioDaimyoExecute()���\�b�h�������Ă���Ȃ�΁c
	// ���s�ΏۂƂȂ��Ă���DLL�����ɑ{�����āc
	for ( vector<HMODULE>::iterator it = listOfTargetDLL.begin(); it != listOfTargetDLL.end(); it++ ) {

		// �Ώۂ�DLL��OnSelectingScenarioDaimyoStart�֐������݂��邩���`�F�b�N�B
		PFNTARGETMODSELECTINGSCENARIODAIMYOSTART p_SelectingScenarioDaimyoStart = (PFNTARGETMODSELECTINGSCENARIODAIMYOSTART)GetProcAddress(*it, "OnSelectingScenarioDaimyoStart");

		if ( !p_SelectingScenarioDaimyoStart ) {
			continue;
		}

		((PFNTARGETMODSELECTINGSCENARIODAIMYOSTART) p_SelectingScenarioDaimyoStart)();
	}

	// ������ScenarioMod.dll�ŁAsnOnSelectingScenarioDaimyo(void *)���\�b�h�������Ă���Ȃ�΁c
	if ( p_snOnSelectingScenarioDaimyo ) {
		// �Ƃ肠������UNULL����Ă����B
		((PFNSNONSELECTINGSCENARIODAIMYO) p_snOnSelectingScenarioDaimyo)( NULL );
	}


}