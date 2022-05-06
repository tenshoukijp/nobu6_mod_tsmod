#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"
#include <iostream>
#include <fstream>
#include <string>


#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"

BOOL CreateAllocConsole() {

	// デバッグウィンドウのオープン指定
	if ( TSModCommand::Debug::cmdDebugWindowOpen == 1 ) {

		// コンソールを作成し，
		BOOL retAllocConsole = AllocConsole();

		// 標準入出力に割り当てる
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