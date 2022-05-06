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



// PluginMod.dll��Initialize�֐������s����B
BOOL LoadLibraryOfPluginMod() {

	// ---------------------------------------------------------------------------------------------------
	// �����̃v���O�C����ǂݍ����
	char *filename = "PluginMod.ini";
	ifstream ifPluginModSettings(filename);

	// �ݒ�t�@�C��������ꍇ�̓��X�g�ɒǉ�
	if (ifPluginModSettings ) {

		string buf = "";
		// �P�s���ǂݍ���
		while( getline(ifPluginModSettings, buf ) ) {

			// DLL�͂���H
			HMODULE hTaretModule = LoadLibraryA(buf.c_str());

			if (!hTaretModule) {
				continue;
			}
			// ���݂��Ă����Ȃ�A�ǂ񂶂�����킯�ł��āA��ő������邽�ߓo�^���Ă����B
			listOfTargetDLL.push_back(hTaretModule);

			char filename[512] = "";
			strcpy(filename, _strlwr( (char *)buf.c_str() ) );

			// �n�b�V���ɕۑ�
			AddModuleItem( hTaretModule, filename );

		}

	// �ݒ�t�@�C�������݂��Ȃ��ꍇ�ɂ́A�������E����B
	} else {

		WIN32_FIND_DATA ffd;
		// *Mod.dll�Ƃ������O�̃t�@�C���B
		HANDLE hFileList = FindFirstFile("*Mod.dll", &ffd);
		// ���݂����ꍇ�A�ǂݍ��ݑΏۂƂ��邪�ATSMod.dll��PluginMod.dll��ScenarioMod.dll�͏��O�B
		if ( hFileList != INVALID_HANDLE_VALUE ) {
			do {
				char filename[512] = "";
				strcpy(filename, _strlwr(ffd.cFileName));
				if ( strcmp( filename, "tsmod.dll")==0 ) { continue; }
				if ( strcmp( filename, "pluginmod.dll")==0 ) { continue; }
				if ( strcmp( filename, "scenariomod.dll")==0 ) { continue; }

				// DLL�͂���H
				HMODULE hTaretModule = LoadLibraryA(filename);

				if (!hTaretModule) {
					continue;
				}

				// ���݂��Ă����Ȃ�A�ǂ񂶂�����킯�ł��āA��ő������邽�ߓo�^���Ă����B
				listOfTargetDLL.push_back(hTaretModule);

				// �n�b�V���ɕۑ�
				AddModuleItem( hTaretModule, filename );

			} while (FindNextFile(hFileList, &ffd));
		}


		hFileList = FindFirstFile("*Mod.cpl", &ffd);
		// ���݂����ꍇ�A�ǂݍ��ݑΏۂƂ���B
		if ( hFileList != INVALID_HANDLE_VALUE ) {
			do {
				char filename[512] = "";
				strcpy(filename, _strlwr(ffd.cFileName));

				// DLL�͂���H
				HMODULE hTaretModule = LoadLibraryA(filename);

				if (!hTaretModule) {
					continue;
				}

				// ���݂��Ă����Ȃ�A�ǂ񂶂�����킯�ł��āA��ő������邽�ߓo�^���Ă����B
				listOfTargetDLL.push_back(hTaretModule);

				// �n�b�V���ɕۑ�
				AddModuleItem( hTaretModule, filename );

			} while (FindNextFile(hFileList, &ffd));
		}


		FindClose(hFileList);

	}

	// �ǂݍ���DLL�Q�ɑ΂��āAOnInitialize������΁A��������s�Ȃ���Ή��
	for ( vector<HMODULE>::iterator it = listOfTargetDLL.begin(); it != listOfTargetDLL.end(); it++ ) {

		// DLL���ǂ߂��̂ł���΁AOnInitialize�֐������݂��邩���`�F�b�N�B
		PFNTARGETMODINITIALIZE p_OnInitialize = (PFNTARGETMODINITIALIZE)GetProcAddress(*it, "OnInitialize");

		// OnInitialize�����݂��Ȃ��̂ł���΁A�ΏۊO�B����B
		if ( !p_OnInitialize ) {
			FreeLibrary(*it);

			// �n�b�V��������폜
			AddModuleItem(*it, "");

			continue;
		}
	}


	return TRUE;
}



BOOL FreeLibraryOfPluginMod() {
	// �o�^����������B�܂����ۂ̓_�����������B
	// ��d�N�����ɂ͖���������炢�͂��Ă����B
	for ( vector<HMODULE>::iterator it = listOfTargetDLL.begin(); it != listOfTargetDLL.end(); it++ ) {
		FreeLibrary(*it);
	}

	return TRUE;
}
