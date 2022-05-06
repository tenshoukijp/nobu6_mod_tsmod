#pragma once

enum	{ TIMER_EVENT_ID_MENU_HIDE = 30 };

// TSMod.txt�̓���ݒ�ύX�R�}���h�w��
namespace TSModCommand {

	extern int gurdian1[1000];

	// ���̐��Ɂc 
	namespace World {
		// �S�C�����݂��Ȃ������Ȃ��
		extern int cmdTheTeppouNoExists;
		// �|�����݂��Ȃ������Ȃ��
		extern int cmdTheYumiNoExists;
		// �J�X�^���ƕ󂪑��݂���Ȃ��
		extern int cmdCustomKahouExists;
		// ���z�������o�ꂷ�邩
		extern int cmdVirtualBushouAppear;
	}

	namespace AllCastle {
		/// �����\����������
		extern int cmdIncreaseRatioCanDraftSoldier;

		// �b�o�t��ł̏�h��͌W���B
		extern int cmdCastleDefenceCoefOfNPC;

		// ����V���[�g�J�b�g�����B
		extern int cmdShortCutFallCastle;
	}


	// �S�Ă̐��͂��c 
	namespace AllGundan {
		// �o�w���ɏo�w�\�ȕ������͈ȉ��ƒ�߂���
		extern int cmdTheNumberOfTheMaxGoToWarFix;

		// �u�u�`�v�ɕK�v�ȍs���l
		extern int cmdNeedKoudouValueKougi;
		// �u��I�v�ɕK�v�ȍs���l
		extern int cmdNeedKoudouValueHirou;
		// �u�Ґ��v�ɕK�v�ȍs���l
		extern int cmdNeedKoudouValueHensei;
		// �u�����v�ɕK�v�ȍs���l
		extern int cmdNeedKoudouValueChohei;
		// �u�o�p�v�ɕK�v�ȍs���l
		extern int cmdNeedKoudouValueTouyou;
		// �u�J���v�ɕK�v�ȍs���l
		extern int cmdNeedKoudouValueHoubi;
		// �u�����v�ɕK�v�ȍs���l
		extern int cmdNeedKoudouValueBusshi;
		// �u�푈�v�ɕK�v�ȍs���l
		extern int cmdNeedKoudouValueSensou;
		// �u�����v�ɕK�v�ȍs���l
		extern int cmdNeedKoudouValueDoumei;
		// �u�����v�ɕK�v�ȍs���l
		extern int cmdNeedKoudouValueKonin;
		// �u��؂�v�ɕK�v�ȍs���l
		extern int cmdNeedKoudouValueTegire;
		// �u�����v�ɕK�v�ȍs���l
		extern int cmdNeedKoudouValueKyohaku;
		// �u����v�ɕK�v�ȍs���l
		extern int cmdNeedKoudouValueChotei;
		// �u�����v�ɕK�v�ȍs���l
		extern int cmdNeedKoudouValueNaiou;
		// �u�d���v�ɕK�v�ȍs���l
		extern int cmdNeedKoudouValueBouryaku;
		// �u�]��v�ɕK�v�ȍs���l
		extern int cmdNeedKoudouValueHyoujo;
		// �u�R�c�v=>�u�C���v�ɕK�v�ȍs���l
		extern int cmdNeedKoudouValueGundanNinmei;
		// �u�R�c�v=>���z�v�u�����v�u�ĔC�v�ɕK�v�ȍs���l
		extern int cmdNeedKoudouValueGundanSubCmd;
		// �u�����E�P���E���i�v�ɕK�v�ȍs���l
		extern int cmdNeedKoudouValue5Ningumi;
	}

	// �����̑喼���͂��c
	namespace MyselfGundan {
		// ���ƕĂ��ĂƓ~�ɂ��炦��
		extern int cmdGetMoneyAndRiseOnSeasonChangeLv;	

		// ���͂������I�ɉ񕜂���
		extern int cmdRegenerationSoldierAutomatically;

		// ���Y�i�Ɋ֘A�������ڂ������I�ɑ�����(�S�C�E�n�E���ۍ`)
		extern int cmdIncreaseSpecialProductByTheThing;

	};

	// ���̑喼���͂��c
	namespace OthersGundan {
		// ���ƕĂ��ĂƓ~�ɂ��炦��
		extern int cmdGetMoneyAndRiseOnSeasonChangeLv;

		// �v���C���̐i���ƁA�v���C���ȊO�̑S�̂̌R�c�̐i�������A�v���C���ȊO�̌R�c�݂͌��Ɏ����o�����X����낤�Ƃ��܂��B
		extern int cmdPlayerAndTheWholeInfluenceEachOtherData;

		// ���͂������I�ɉ񕜂���
		extern int cmdRegenerationSoldierAutomatically;

		// ���Y�i�Ɋ֘A�������ڂ������I�ɑ�����(�S�C�E�n�E���ۍ`)
		extern int cmdIncreaseSpecialProductByTheThing;
	};


	// �S�Ă̑喼���c
	namespace AllDaimyo {
		// ���Α叫�R
		extern int cmdSeiiTaishogunNeedCastleNum;

		// �N�������ꂵ�Ă��G���f�B���O���}���Ȃ��B
		extern int cmdTheSengokouPeriodIsEndless;
	};

	// �����̑喼���c
	namespace MyselfDaimyo {
		// ��E�A�C�������ăQ�[���G���h�Ƃ���B
		extern int cmdGameEndConditionIsGettingPosition;
		// ��Q�R�c�ȉ��ɒ��ڎw�����\�B
		extern int cmdCanInstructDirectOtherGundan;
		// �]��͂P�G�߂ɕ�����\
		extern int cmdCanMultiHyoujoInSeason;
	};

	// ���̑喼���c
	namespace OtherDaimyo {
		// ���̐��͂̑喼�́A�����R�}���h�����s���Ȃ��B
		extern int cmdNeverThreat;
	};

	namespace OtherBushou {

		// �퓬���͑S�Ă̋Z�\���g������
		extern int cmdCanUseAllAbilityInBattle;
	};

	// �S�Ă̕������c
	namespace AllBushou {

		// �E�҂�������ő啺�m���������̈�ɐ��������
		extern int cmdNinjaMustBeAShadow;	

		// �E�҂͌M�����オ��Ȃ�
		extern int cmdNinjaNotHaveAWayOfPromotion; 

		// ���ʂ��E������΁A���ꂪ���m�̍ő吔�̑����Ɍq����
		extern int cmdPositionInfluenceSoldierMax;	

		// �喼�̕����̃x�[�X
		extern int cmdSoldierBaseMaxOfDaimy;	
		// �h�V�̕����̃x�[�X
		extern int cmdSoldierBaseMaxOfSyuku;
		// �ƘV�̕����̃x�[�X
		extern int cmdSoldierBaseMaxOfKarou;
		// �����̕����̃x�[�X
		extern int cmdSoldierBaseMaxOfBusyo;
		// ���叫�̕����̃x�[�X
		extern int cmdSoldierBaseMaxOfSamra;	
		// ���y���̕����̃x�[�X
		extern int cmdSoldierBaseMaxOfAshig;	


		// �g�����ꂽ�V�E��(�o��)�𓱓�����B
		extern int cmdExtendedJob; 

		// �S�Ă̕����͐펞�ɈÎE�Ԃ������邱�Ƃ�����B
		extern int cmdAntiAssassinationInBattle;

		// �������V��Ă��\�͂��������Ȃ�
		extern int cmdNoDeclineTalentParam;

		// ���������ׂĂ̕����̗E�҂�����̒l�ł�������ȁc
		extern int cmdBraveIsTheValue;

		// �������S�Ă̕����̎������Z�������Ȃ�c���邢�͒��������Ȃ�c
		extern int cmdLifeIsTheValue;

		// �������S�Ă̕����̍΂����̂��x���Ȃ����Ȃ�c
		extern int cmdLifeIsTheTimes;

		// �������S�Ă̕����ɋ`���ȂǂȂ��Ɨ�����ł������Ȃ�c
		extern int cmdNoDutiesAndMaxIndependences;

	    // �������Z�\�����J���ł������Ȃ�c
		extern int cmdAbilityIsUnderdeveloped;

		// ���������n�E��n�E�q�n���l�`�w�ł������Ȃ�c
		extern int cmdFixExperienceMax;
		
		// �n���x��Z�\�������ŏK�����Ă䂭�B
		extern int cmdAutoUpExperienceAndTech;

		// �}�ɂ͐������Ȃ��B
		extern int cmdNotGrowUpSuddenly;

		// �㒷�Ƃ̑����̗ǂ��������A���G�߁A�����ɉe������
		extern int cmdAffinityWithSuperiorInfluenceLoyalty;

		// (���S�p�x�ɂ�����炸)���G���S����������B
		extern int cmdSetWorkToDieEverySeason;

		// �b�o�t�o��P���P�����ɂ���B
		extern int cmdAppearanceHimeBecomeBushou;

		// ��Ȃ����������g������p�b�`�B
		extern int cmdNonFaceBushouIndependences;

		// �Q�l�ɂȂ������A�M����������c��
		extern int cmdNonMeritoriousReset;

		// �푈���݂Ă��Ȃ����̐퓬�n���E�q�d�n���̉ߏ�ȏ㏸��}����B
		extern int cmdSuppressRiseOfExpValueInBattle;

		// ����󂷍ۂɃ_���[�W���󂯂邩�ǂ����B
		extern int cmdReceiveDamageOfBreakDoor;

		// ���S�p�x�̎����ւ̉e���͂��J�b�g����B
		extern int cmdDeathFrequencyNotInfluenceLife;

		// ���̔\�͂͗��V���[�Y���l���������̂ł���B
		extern int cmdParamConsiderNobunagaSeries;

		// ����������ɉƐb�̒����x���~
		extern int cmdDegreeLoyaltyOnCaptiveLibra;
	};


	namespace UltraBushou {

		// �E���g������������
		extern int cmdExists;

		// �ˌ��ɂ�鑊��̐펀�L��(�f�t�H���g�L��)
		extern int cmdTotsugekiTargetDeath;
	};


	namespace Environment {
		// �^�C�g���o�[�ƃ��j���[�o�[�͔�\��
		extern int cmdNoTitleBarAndMenuBar;

		// �𑜓x��傫�����郂�[�h
		extern int cmdWideResolution;

		// �𑜓x�����o�C���p�ɂ���
		extern int cmdMobileResolution;

		// �a�f�l�̃��[�h
		extern int cmdBGMMode;

		// �C���^�[�l�b�g�ւ̐ڑ��̋���
		extern int cmdInternetConnectPermission;

		// �f�����[�h�ł̓X�N���[���Z�C�o�[�̔��s��h�~����
		extern int cmdNoScreenSaverInDemo;

	};

	namespace Debug {

		// AllocConsole�^(�S�����g�������)�f�o�b�O�E�B���h�E���I�[�v���ɂ���B
		extern int cmdDebugWindowOpen;
	};


	// �R�}���h���Z�b�g����
	void setCommandSwitch(char *szCurCmdTokenBuf, int byteCurCmdLv);

	extern int gurdian2[1000];

};


