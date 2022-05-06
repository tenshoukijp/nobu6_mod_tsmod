#define _CRT_SECURE_NO_WARNINGS 1

#include "WinTarget.h"
#include <fstream>
#include <vector>
#include <string>
#include <string.h>
#include <stdio.h>


#include "CommonGlobalHandle.h"
#include "Menu.h"
#include "PluginMod.h"

vector<HMODULE> listOfTargetDLL;



// PluginMod.dllのInitialize関数を実行する。
BOOL LoadLibraryOfPluginMod() {

	// ---------------------------------------------------------------------------------------------------
	// 複数のプラグインを読み込んで
	char *filename = "PluginMod.ini";
	ifstream ifPluginModSettings(filename);

	// 設定ファイルがある場合はリストに追加
	if (ifPluginModSettings ) {

		string buf = "";
		// １行ずつ読み込む
		while( getline(ifPluginModSettings, buf ) ) {

			// DLLはある？
			HMODULE hTaretModule = LoadLibraryA(buf.c_str());

			if (!hTaretModule) {
				continue;
			}
			// 存在していたなら、読んじゃったわけでして、後で走査するため登録しておく。
			listOfTargetDLL.push_back(hTaretModule);

			char filename[512] = "";
			strcpy(filename, _strlwr( (char *)buf.c_str() ) );

			// ハッシュに保存
			AddModuleItem( hTaretModule, filename );

		}

	// 設定ファイルが存在しない場合には、自動収拾する。
	} else {

		WIN32_FIND_DATA ffd;
		// *Mod.dllという名前のファイル。
		HANDLE hFileList = FindFirstFile("*Mod.dll", &ffd);
		// 存在した場合、読み込み対象とするが、TSMod.dllとPluginMod.dllとScenarioMod.dllは除外。
		if ( hFileList != INVALID_HANDLE_VALUE ) {
			do {
				char filename[512] = "";
				strcpy(filename, _strlwr(ffd.cFileName));
				if ( strcmp( filename, "tsmod.dll")==0 ) { continue; }
				if ( strcmp( filename, "pluginmod.dll")==0 ) { continue; }
				if ( strcmp( filename, "scenariomod.dll")==0 ) { continue; }

				// DLLはある？
				HMODULE hTaretModule = LoadLibraryA(filename);

				if (!hTaretModule) {
					continue;
				}

				// 存在していたなら、読んじゃったわけでして、後で走査するため登録しておく。
				listOfTargetDLL.push_back(hTaretModule);

				// ハッシュに保存
				AddModuleItem( hTaretModule, filename );

			} while (FindNextFile(hFileList, &ffd));
		}


		hFileList = FindFirstFile("*Mod.cpl", &ffd);
		// 存在した場合、読み込み対象とする。
		if ( hFileList != INVALID_HANDLE_VALUE ) {
			do {
				char filename[512] = "";
				strcpy(filename, _strlwr(ffd.cFileName));

				// DLLはある？
				HMODULE hTaretModule = LoadLibraryA(filename);

				if (!hTaretModule) {
					continue;
				}

				// 存在していたなら、読んじゃったわけでして、後で走査するため登録しておく。
				listOfTargetDLL.push_back(hTaretModule);

				// ハッシュに保存
				AddModuleItem( hTaretModule, filename );

			} while (FindNextFile(hFileList, &ffd));
		}


		FindClose(hFileList);

	}

	// 読み込んだDLL群に対して、OnInitializeがあれば、それを実行なければ解放
	for ( vector<HMODULE>::iterator it = listOfTargetDLL.begin(); it != listOfTargetDLL.end(); it++ ) {

		// DLLが読めたのであれば、OnInitialize関数が存在するかをチェック。
		PFNTARGETMODINITIALIZE p_OnInitialize = (PFNTARGETMODINITIALIZE)GetProcAddress(*it, "OnInitialize");

		// OnInitializeが存在しないのであれば、対象外。解放。
		if ( !p_OnInitialize ) {
			FreeLibrary(*it);

			// ハッシュからも削除
			AddModuleItem(*it, "");

			continue;
		}
	}


	return TRUE;
}



BOOL FreeLibraryOfPluginMod() {
	// 登録した分解放。まぁ実際はダメかもしれん。
	// 二重起動時には明示解放ぐらいはしてくれる。
	for ( vector<HMODULE>::iterator it = listOfTargetDLL.begin(); it != listOfTargetDLL.end(); it++ ) {
		FreeLibrary(*it);
	}

	return TRUE;
}
