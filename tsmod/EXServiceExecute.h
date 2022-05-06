#pragma once

#include "EXBushou.h"
#include "EXBushou_LancelotTartare.h"
#include "EXBushou_ShuiHidechiyo.h"


// �v���C���[�̃^�[�����؂�ւ����(�����喼�S��)
void EXSuperService_OnPlayerTurnChange();

// �v���C���[�̃^�[�����؂�ւ�肩�A�G�߂��ς����
void EXSuperService_OnSeasonChangeAndPlayerTurnChange();

// IntersectRectMessage() �Ƃ�����r�I�Z���^�C�~���O�ōX�V�`�F�b�N���s������T�[�r�X
void EXSuperService_OnIntersectRectMessage();

// ���C���Q�[����ʂ��n�܂�����
void EXSuperService_OnMainGameStart();


// ���G�߁A���ʂɋ���Ă����Z�����B
void EXSuperService_CheckOtherDaimyosGetMoneyAndRiseOnSeasonChange();

// ���G�߁A�v���C�����͂̐i�x��S�̂̐i�x�ɍ��킹�āA�o�����X�������B
void EXSuperService_PlayerDaimyoDataInfluenceOtherDaimyoData();

// ���Y�i�Ɋ֘A�������ڂ������I�ɑ�����(�S�C�E�n�E���ۍ`)
void EXSuperService_CheckIncreaseSpecialProductByTheThing();

// �S�Ă̕����̕��m�������񕜂���B�������v���C���[�����삷��R�c�z���͏��O
void EXSuperService_Regeneration(int isForce = 0);

// �M���Ɋւ������ȃt�B���^�[�T�[�r�X
void EXSuperService_FilterMerits();

// �G�߂��ς��x�ɂS�����ȉ��̌��ƕ�ɂ��āA�����������킹�Ă��Ȃ����������ɁA�����Ⴂ�m���œn��悤�ɂ���B
void EXServieExecute_NPCKengouBushouHasAKahouSword();

// �S�C�����ӂȐl���b�葺����鏈��
void EXServiceExecute_TeppouSpMakeSmith();

// �C�������ۍ`����鏈��
void EXServiceExecute_PirateMakePort();

// �����̗E�҂������I�Ɉ��̒l�ɂ��鏈��
void EXSuperService_IfBravesIsTheValue();

// �����̎����������I�Ɉ��̒l�ɂ��鏈��
void EXSuperService_IfLifesIsTheValue();

// �������S���|���g���Ȃ�������
void EXSuperService_IfYumiNoExists();

// �������΂����̂��x��������
void EXSuperService_IfLifesIsTheTimes();

// �����̋`�����Œ�l�ɁA�Ɨ����ő�l�ɂ���
void EXSuperService_IfNoDutiesAndMaxIndependences();

// ����ύX�ύX���ɍs���T�[�r�X
void EXSuperService_OnSobaChange();

// �G�߂̕ς��ڂňÎE�J�E���g�����Z�b�g�����B
void EXSuperService_AnsatsuCntReset();

// �Z�[�u�f�[�^�Ɏ������Ă���A�G�߂̕ς��ڂɃt�B���^�[�̂��߂̗ݐς̏n���x��0�ɂȂ�B
void EXSuperService_ExperienceLimitReset();

// �����̋Z�p�����J���ł������ꍇ�̏���
void EXSuperService_IfAbilityIsUnderdevelopedOnSelectingScenarioDaimyo();
void EXSuperService_IfAbilityIsUnderdevelopedOnAlwaysMainGame();

// �����̏n���x���l�`�w�Œ�ł������ꍇ�̏���
void EXSuperService_IfFixExperienceMaxOnChangeTiming();
void EXSuperService_IfFixExperienceMaxOnAlwaysMainGame();

// ����ύX���ɁA�n���x�����b�B����B(�Z�\���o����)
void EXSuperService_AutoUpExperienceAndTechOnSobaChange();

// �R�c���Ƃ̑����ɂ���Ė��G�ߒ����x���㉺������
void EXSuperService_AishoCalc();

// �V�i���I�������̌�ŁA�V�i���I�喼�I�����̑O�B�u���b�N�A�E�g��
void EXSuperService_OnSelectingScenarioAfter();

// �V�i���I�喼�I�����̏���
void EXSuperService_OnSelectingScenarioDaimyo();
// EXServiceExecuteRegeneration_All_Regeneration()������O�ɂ���K�v�����鎖�O�v�Z�B
void EXServiceExecuteRegeneration_All_SoldierCalcInMain();

// �G�����Ȃ�A�퓬���S�Ă̋Z�\���g����B
void EXSuperService_IfCanAllAlibityInBattleStart();
void EXSuperService_IfCanAllAlibityInBattleEnd();
void EXSuperService_IfCanAllAlibityInBattleSeiryokuChange(int iIamBushouID, int iUreBushouID );


// �푈�����Ă��Ȃ��ꍇ�ɁA�n���l������I�ɏ㏸���邽�߁A��r�p�ɐ퓬�O�ɕۑ�
void EXServiceExecuteBushouValueControllerOfStartBattle();

// �푈�����Ă��Ȃ��ꍇ�ɁA�n���l������I�ɏ㏸���Ă�ꍇ�A�}����
void EXServiceExecuteBushouValueControllerOfEndBattle();


// �V�i���I�G�f�B�^�̃o�O�Ή��B�R�c��E��15�������Ă���Ƃ����o�O
void patchBugOfGundanYakusyokuInSenarioEditor();



// �S�C�C�x���g�̃R���g���[���[
void CheckTeppouEventControll();


// �V�ċL�ȍ~�̕����̃p�����^�𔽉f����B
void SetBushouAvgParam(int iFlagFalue);

// �Q�l�̌M�������G�߂Ƀ_�E������
void EXSuperService_RoninKunkouDecrement();


