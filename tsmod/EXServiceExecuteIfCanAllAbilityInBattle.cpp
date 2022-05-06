#include "WinTarget.h"
#include <fstream>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"




/*
 * TSMod.txt�Ŏw�肷��ƁA�퓬���Ƀv���C�����͂̕����ȊO�͑S�Z�\�����p�\�ł���B
 */

struct OriginalTechPreviousBattle {
	byte tech_ikkatsu:1;	//�Z�\�E�ꊅ
	byte tech_ansatsu:1;	//�Z�\�E�ÎE
	byte tech_ryusyutsu:1;	//�Z�\�E���o
	byte tech_sendou:1;		//�Z�\�E����
	byte tech_ryugen:1;		//�Z�\�E����
	byte tech_benzetsu:1;	//�Z�\�E�ِ�
	byte tech_yakiuchi:1;	//�Z�\�E�ē�
	byte tech_chohatsu:1;	//�Z�\�E����
} OriginalTachOf_IfCanAllAlibityInBattleStart[GAMEDATASTRUCT_BUSHOU_NUM];

// �o�w�O�ɂm�o�b�����ł��������ǂ����B
bool isNPCBushouOfPreviousBattle[GAMEDATASTRUCT_BUSHOU_NUM];

// �퓬�ɓ���O�ɁA���̋Z�\���o�b�N�A�b�v����B
void EXSuperService_IfCanAllAlibityInBattleStart() {

	// �퓬���Ƀv���C�����͂̕����ȊO�͑S�Z�\�����p�\�ł���B���n�e�e���ǂ����B
	if ( ! TSModCommand::OtherBushou::cmdCanUseAllAbilityInBattle ) {
		return;
	}

	for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

		if ( !isPlayerBushou( iBushouID ) ) { 
			isNPCBushouOfPreviousBattle[iBushouID] = true;

			OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_ikkatsu  = nb6bushouref[iBushouID].tech_ikkatsu;
			OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_ansatsu  = nb6bushouref[iBushouID].tech_ansatsu;
			OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_ryusyutsu= nb6bushouref[iBushouID].tech_ryusyutsu;
			OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_sendou   = nb6bushouref[iBushouID].tech_sendou;
			OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_ryugen   = nb6bushouref[iBushouID].tech_ryugen;
			OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_benzetsu = nb6bushouref[iBushouID].tech_benzetsu;
			OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_yakiuchi = nb6bushouref[iBushouID].tech_yakiuchi;
			OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_chohatsu = nb6bushouref[iBushouID].tech_chohatsu;

			nb6bushouref[iBushouID].tech_ikkatsu = 1; 
			nb6bushouref[iBushouID].tech_ansatsu = 1;  
			nb6bushouref[iBushouID].tech_ryusyutsu = 1;
			nb6bushouref[iBushouID].tech_sendou = 1;	
			nb6bushouref[iBushouID].tech_ryugen = 1;	
			nb6bushouref[iBushouID].tech_benzetsu = 1; 
			nb6bushouref[iBushouID].tech_yakiuchi = 1; 
			nb6bushouref[iBushouID].tech_chohatsu = 1; 
		} else {
			isNPCBushouOfPreviousBattle[iBushouID] = false;
		}
	}
}


// �퓬���I�������̂Ō��ւƖ߂�
void EXSuperService_IfCanAllAlibityInBattleEnd() {

	// �퓬���Ƀv���C�����͂̕����ȊO�͑S�Z�\�����p�\�ł���B���n�e�e���ǂ����B
	if ( ! TSModCommand::OtherBushou::cmdCanUseAllAbilityInBattle ) {
		return;
	}

	for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

		if ( isNPCBushouOfPreviousBattle[iBushouID] ) { 

			nb6bushouref[iBushouID].tech_ikkatsu   = OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_ikkatsu;
			nb6bushouref[iBushouID].tech_ansatsu   = OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_ansatsu;
			nb6bushouref[iBushouID].tech_ryusyutsu = OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_ryusyutsu;
			nb6bushouref[iBushouID].tech_sendou    = OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_sendou;
			nb6bushouref[iBushouID].tech_ryugen    = OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_ryugen;
			nb6bushouref[iBushouID].tech_benzetsu  = OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_benzetsu;
			nb6bushouref[iBushouID].tech_yakiuchi  = OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_yakiuchi; 
			nb6bushouref[iBushouID].tech_chohatsu  = OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_chohatsu; 

		}
	}
}


// �G���͉�����A�v���C�����͉��ɂȂ����̂ŁA�\�͂����ւƖ߂��B
// iIamBushou ���o���̎�A�@iUreBushou ���蕐��
void EXSuperService_IfCanAllAlibityInBattleSeiryokuChange(int iIamBushou, int iUreBushou) { //

	// �퓬���Ƀv���C�����͂̕����ȊO�͑S�Z�\�����p�\�ł���B���n�e�e���ǂ����B
	if ( TSModCommand::OtherBushou::cmdCanUseAllAbilityInBattle == 0) {
		return;
	}

	// ���������肩�ǂ��炩�������͈̔̓`�F�b�N�𖞂����Ȃ���Ή������Ȃ��B
	if ( 0 <= iIamBushou && iIamBushou < GAMEDATASTRUCT_BUSHOU_NUM ) {
	} else {
		return;
	}
	if ( 0 <= iUreBushou && iUreBushou < GAMEDATASTRUCT_BUSHOU_NUM ) {
	} else {
		return;
	}

	// ���؂�̑���敐�����v���C�����͂��ǂ���
	if ( isPlayerBushou( iUreBushou ) ) {
		int iBushouID = iIamBushou;

		nb6bushouref[iBushouID].tech_ikkatsu   = OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_ikkatsu;
		nb6bushouref[iBushouID].tech_ansatsu   = OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_ansatsu;
		nb6bushouref[iBushouID].tech_ryusyutsu = OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_ryusyutsu;
		nb6bushouref[iBushouID].tech_sendou    = OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_sendou;
		nb6bushouref[iBushouID].tech_ryugen    = OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_ryugen;
		nb6bushouref[iBushouID].tech_benzetsu  = OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_benzetsu;
		nb6bushouref[iBushouID].tech_yakiuchi  = OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_yakiuchi; 
		nb6bushouref[iBushouID].tech_chohatsu  = OriginalTachOf_IfCanAllAlibityInBattleStart[iBushouID].tech_chohatsu; 
	}
}