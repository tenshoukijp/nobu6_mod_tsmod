
#include <sys/types.h>
#include <sys/stat.h>


#include "GameDataStruct.h"
#include "CommonGlobalHandle.h"
#include "TenShougiMod.h"



// �V����Mod.dll�̃��W���[���n���h��
HMODULE hTenShougiModDll = NULL;


// ���S�N��(TSMod����͂��Ȃ��ATenShougiModServer���s��)
PFNTENSHOUGIMODBGN p_sgTenShougiModBgn = NULL;

// ���S�I��(TSMod����͂��Ȃ��ATenShougiModServer���s��)
PFNTENSHOUGIMODEND p_sgTenShougiModEnd = NULL;


// �t�H���g����]������B
PFNSETTENSHOUGIFONTNAME p_sgSetTenShougiFontName = NULL;

// �����̊��͂�]��
PFNSETTENSHOUGIBUSHOUPOWER p_sgSetTenShougiBushouPower = NULL;

// �V�����E�B���h�E���\������Ă��邩�ǂ�����V������TSMod�𓾂�B
PFNISTENSHOUGIFORMSHOWN p_sgIsTenShougiFormShown = NULL;


// �V�����ɃR�}���h�𑗂�B
PFNSENDCOMMAND p_sgSendCommand = NULL;


bool IsTenShougiModFilesExists() {

	// �����
	struct stat st;
	// 
	if (stat("TenShougiMod.dll", &st) != 0) {
		return false;
	}

	if (stat("TenShougiModServer.exe", &st) != 0) {
		return false;
	}
	if (stat("TenShougiMod���Պ���.bin", &st) != 0) {
		return false;
	}
	if (stat("TenShougiMod�]���֐�.bin", &st) != 0) {
		return false;
	}

	return true;
}


// �V�ċL�����Ŏg�p����Ă���t�H���g�����A�V�����֓n��
void SetTenshougiFontName() {

	// �t�H���g���w���Ă���ꏊ
	char *szTenShoukiFontName = (char *)GAMEDATASTRUCT_FONTNAME_ADDRESS;

	p_sgSetTenShougiFontName = (PFNSETTENSHOUGIFONTNAME)GetProcAddress(hTenShougiModDll, "sgSetTenShougiFontName");
	if ( p_sgSetTenShougiFontName ) {
		// �V�ċL�̃t�H���g�����A�V������SharedMemory�p�̈�ɓ`�B
		p_sgSetTenShougiFontName( szTenShoukiFontName );
	}
}


// �Q�l�̊��͂�]������B
void SetShougiBushouPower(int iBBushouID, int iWBushouID) {

	p_sgSetTenShougiBushouPower = (PFNSETTENSHOUGIBUSHOUPOWER)GetProcAddress(hTenShougiModDll, "sgSetTenShougiBushouPower");

	if ( p_sgSetTenShougiBushouPower ) {

		// ���͍\���́A���p�A���p
		TBUshouDaniPowerInfo BPower = {NULL};
		TBUshouDaniPowerInfo WPower = {NULL};

		// ���p
		BPower.iPower = GetShogiPower(iBBushouID);
		strcpy( BPower.szDaniName, getSpecialDaniStringOfBushou(iBBushouID) );
		BPower.iBushouID = iBBushouID;

		// ���p
		WPower.iPower = GetShogiPower(iWBushouID);
		strcpy( WPower.szDaniName, getSpecialDaniStringOfBushou(iWBushouID) );
		WPower.iBushouID = iWBushouID;

		// �V�����ւƓn��
		p_sgSetTenShougiBushouPower( (void *)&BPower, (void *)&WPower );
	}
}


// �V�����̃����������X�V����B
void Update_TenShougiMemoryInfo(int iBBushouID, int iWBushouID) {
	// �V���������ĂȂ��B�_���B
	if (!hTenShougiModDll ) { return; }

	// ���ݓV�����̃E�B���h�E���J���֐����Z�b�g�������Ƃ��Ȃ���΁A�_���B�X�V���Ȃ��B
	if (!p_sgIsTenShougiFormShown ) { return; }

	if ( 0 <= iBBushouID && iBBushouID < GAMEDATASTRUCT_BUSHOU_NUM && 
	     0 <= iWBushouID && iWBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// �t�H���g����`�B
		SetTenshougiFontName();

		// �Q�l�̊��͂�]������B
		SetShougiBushouPower(iBBushouID, iWBushouID);
	}
}


// �V�����J�n
void TenShougiModBgn(int iBBushouID, int iWBushouID) {

	// ���ݓV�����̃E�B���h�E���J���Ă����Ԃ��ǂ����̊֐�
	if (!p_sgIsTenShougiFormShown) {
		p_sgIsTenShougiFormShown = (PFNISTENSHOUGIFORMSHOWN)GetProcAddress(hTenShougiModDll, "sgIsTenShougiFormShown");
	}
	if (!p_sgIsTenShougiFormShown ) { return; }

	if (!p_sgSendCommand) {
		p_sgSendCommand = (PFNSENDCOMMAND)GetProcAddress(hTenShougiModDll, "sgSendCommand");
	}
	if (!p_sgSendCommand) { return; }


	// �܂��V�������N���������Ƃ��Ȃ��B�{�i���UPipe���\�z����Ă��Ȃ��B
	if (p_sgTenShougiModBgn==NULL)  {

		p_sgTenShougiModBgn = (PFNTENSHOUGIMODBGN)GetProcAddress(hTenShougiModDll, "sgTenShougiModBgn");
		if ( p_sgTenShougiModBgn) {
			// �����Ȃ��B�J�n�́ATenShougiServer���s���B
		}

		// �������̃����������X�V�B
		Update_TenShougiMemoryInfo(iBBushouID, iWBushouID);

		// �V���������`���[���N��
		ShellExecute(NULL, NULL, "TenShougiModServer.exe", "launch", NULL, SW_HIDE);
	}

	// �N���������Ƃ͂���B���V�����̃E�B���h�E���J���Ă��Ȃ��B
	else if ( ! p_sgIsTenShougiFormShown() ) {

		// �������̃����������X�V�B
		Update_TenShougiMemoryInfo(iBBushouID, iWBushouID);

		p_sgSendCommand( "open_window_and_new_game" );
		// �������Ń��j���[�Q�[�����邾���B�E�B���h�E�͊J���Ȃ��B
	} else {
	}

}

void TenShougiModHide() {
	// �V���������ĂȂ��B�_���B
	if (!hTenShougiModDll ) { return; }

	// �E�B���h�E���\������Ă邩�ǂ����̊֐������݂���
	if ( p_sgIsTenShougiFormShown ) {
		// �E�B���h�E���\������Ă���Ȃ��
		if ( p_sgIsTenShougiFormShown() ) {
			// �B��
			p_sgSendCommand( "hide_window" );
		}
	}
}

// �V�����I��
void TenShougiModEnd() {

	// �V���������ĂȂ��B�_���B
	if (!hTenShougiModDll ) { return; }

	p_sgTenShougiModEnd = (PFNTENSHOUGIMODBGN)GetProcAddress(hTenShougiModDll, "sgTenShougiModEnd");
	if ( p_sgTenShougiModEnd ) {
		// �����Ȃ��B����́ATenShougiServer���s���B
	}

	// DLL�̉��
	FreeLibrary( hTenShougiModDll );
}


