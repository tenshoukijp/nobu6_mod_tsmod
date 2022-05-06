#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"
#include <iostream>
#include <fstream>
#include <string>


#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"

BOOL CreateAllocConsole() {

	// �f�o�b�O�E�B���h�E�̃I�[�v���w��
	if ( TSModCommand::Debug::cmdDebugWindowOpen == 1 ) {

		// �R���\�[�����쐬���C
		BOOL retAllocConsole = AllocConsole();

		// �W�����o�͂Ɋ��蓖�Ă�
		freopen("CONOUT$", "w", stdout);
		freopen("CONIN$", "r", stdin);

		return retAllocConsole;
	}

	return false;
}

BOOL DeleteFreeConsole() {
	BOOL retFreeConsole = FreeConsole();
	return retFreeConsole;
}