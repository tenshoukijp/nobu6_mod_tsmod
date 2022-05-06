#include "WinTarget.h"
#include <fstream>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"



/*
 * TSMod.txt�Ŏw�肷��ƁA�f�t�H�����̏n���x���l�`�w�Ƃ���B
 */

// �V�i���I�I����ʎ��ɂl�`�w�Ƃ���B
void EXSuperService_IfFixExperienceMaxOnChangeTiming() {
	
	if ( ! TSModCommand::AllBushou::cmdFixExperienceMax ) {
		return;
	}

	// �����E�퓬�E�q�d �̏n���x���l�`�w��2000�Ƃ���B
	for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

		nb6bushouref[iBushouID].expgov = 2000; 
		nb6bushouref[iBushouID].expbat = 2000; 
		nb6bushouref[iBushouID].expint = 2000; 
	}
}




// �Q�[���X�^�[�g���ɂ��l�`�w�Ƃ���B�Z�[�u����̃��[�h�ȂǂɑΉ����邽�߁B
void EXSuperService_IfFixExperienceMaxOnAlwaysMainGame(int iBushouID) {

	// �Z�\�����J�����n�m�ɂ���R�}���h��L���Ƃ��Ă��Ȃ���Ή������Ȃ��B
	if ( ! TSModCommand::AllBushou::cmdFixExperienceMax ) {
		return;
	}

	// �܂����C���Q�[���܂���ĂȂ���΁A���s���Ȃ�
	if ( !CommonGlobalHandle::isMainGameStart ) {
		return;
	}

	nb6bushouref[iBushouID].expgov = 2000; 
	nb6bushouref[iBushouID].expbat = 2000; 
	nb6bushouref[iBushouID].expint = 2000; 
}