#include "WinTarget.h"
#include <fstream>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"



/*
 * TSMod.txt�Ŏw�肷��ƁA�f�t�H�ł͑S�Ă̕������Z�\�������Ȃ��B���̌�擾���Ă䂭���Ƃ͏o����B
 */

// �����̋Z�p�����J���ł������ꍇ�̏����B�P�񂾂������镐���̋Z�\�𔍒D
void EXSuperService_IfAbilityIsUnderdevelopedOnSelectingScenarioDaimyo() {

	// �Z�\�����J�����n�m�ɂ���R�}���h��L���Ƃ��Ă��Ȃ���Ή������Ȃ��B
	if ( ! TSModCommand::AllBushou::cmdAbilityIsUnderdeveloped ) {
		return;
	}

	for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

		// �����A�Z�\���P�ł��C�����Ă���Ȃ�΁c
		if ( nb6bushouref[iBushouID].tech_ikkatsu    ||
			 nb6bushouref[iBushouID].tech_ansatsu    ||
			 nb6bushouref[iBushouID].tech_ryusyutsu  ||
			 nb6bushouref[iBushouID].tech_sendou	 ||
			 nb6bushouref[iBushouID].tech_ryugen	 ||
			 nb6bushouref[iBushouID].tech_benzetsu   ||
			 nb6bushouref[iBushouID].tech_yakiuchi   ||
			 nb6bushouref[iBushouID].tech_chohatsu ) {

			nb6bushouref[iBushouID].tech_ikkatsu = 0; 
			nb6bushouref[iBushouID].tech_ansatsu = 0;  
			nb6bushouref[iBushouID].tech_ryusyutsu = 0;
			nb6bushouref[iBushouID].tech_sendou = 0;	
			nb6bushouref[iBushouID].tech_ryugen = 0;	
			nb6bushouref[iBushouID].tech_benzetsu = 0; 
			nb6bushouref[iBushouID].tech_yakiuchi = 0; 
			nb6bushouref[iBushouID].tech_chohatsu = 0; 
		}
	}
}





// �����̋Z�p�����J���ł������ꍇ�̏����B
// �Q�[���X�^�[�g�Ȍ�͏�ɏ�������B
// ��`ID���ω����Ă�����A���̐l���ω������Ƃ�������(�V���ȕ������o�ꂵ���Ƃ�������)�ŁA�Z�\����������B
void EXSuperService_IfAbilityIsUnderdevelopedOnAlwaysMainGame(int iBushouID) {

	// �Z�\�����J�����n�m�ɂ���R�}���h��L���Ƃ��Ă��Ȃ���Ή������Ȃ��B
	if ( ! TSModCommand::AllBushou::cmdAbilityIsUnderdeveloped ) {
		return;
	}

	// �܂����C���Q�[���܂���ĂȂ���΁A���s���Ȃ�
	if ( !CommonGlobalHandle::isMainGameStart ) {
		return;
	}


	// �V�����l���c
	// �����A�Z�\���P�ł��C�����Ă���Ȃ�΁c

	// �S���Z�\������
	nb6bushouref[iBushouID].tech_ikkatsu = 0; 
	nb6bushouref[iBushouID].tech_ansatsu = 0;  
	nb6bushouref[iBushouID].tech_ryusyutsu = 0;
	nb6bushouref[iBushouID].tech_sendou = 0;	
	nb6bushouref[iBushouID].tech_ryugen = 0;	
	nb6bushouref[iBushouID].tech_benzetsu = 0; 
	nb6bushouref[iBushouID].tech_yakiuchi = 0; 
	nb6bushouref[iBushouID].tech_chohatsu = 0; 
}