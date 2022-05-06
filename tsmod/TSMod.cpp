// TSMod.cpp : TSMod�̃G���g���|�C���g
//
#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <locale>
#include <string>


#include "TSMod.h"

#include "CommonGlobalHandle.h"
#include "TSModPatchProcessMemory.h"
#include "KaoSwapExtend.h"
#include "WinArgv.h"
#include "AntiDebug.h"
#include "WriteSoundMap.h"

#include "FuncImagePngDialog.h"
#include "DarkModeTitleBarMod.h"

#include "lua/lua_interface.h"

using namespace std;


// DDraw�I���W�i���n���h��
HINSTANCE hDDrawMod = NULL;

// DDraw�I���W�i�����\�b�h�|�C���^
FARPROC p_DirectDrawCreate;

/*
 * �R�}���h���C�������́A__argc, __argv�ɓ����Ă��āA�O���[�o���ϐ��ł���A�ǂ�����ł��Q�Ɖ\�B
 */


BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved ) {
	switch( ul_reason_for_call )
	{
	case DLL_PROCESS_ATTACH:

		srand( (unsigned int)time( NULL ) ); // �����̎��ς���

		// �ꉞ�O���[�o���ɎQ�Ƃł���悤�ɕۑ�
		CommonGlobalHandle::hDLLModule = hModule;

		// �A���`�f�o�b�O����
		// if ( AntiDebugALL() ) { break; }

		// ���[�P��������̐ݒ�̂݁B
		// ���C�h������̃��P�[��(wcout�n)�ւ̐ݒ�B
		// Activate std::wcout.
		locale::global(std::locale("")); 
		wcout.imbue(std::locale(""));

		// �S�Ԃ̏�����
		onig_init();

		LoadLibraryOfDarkModeTitleBarMod();

		// LUA�C���^�v���^�̋N����BGMmm�����BTSMod�̓C���^�v���^���N�����Ă��Ȃ��B
		// �N���ς̃C���^�v���^��luaState=L ��Ⴄ�B
		L = luaL_getstate();

		// DDraw��ǂ݂ɂ�����B���Ȃ��Ƃ�Windows�ł̑��݂��K�{
		
		{
			// �����
			struct stat  st;
			// ddraw.dll�����݂��A
			if (stat(OLD_DDRAW_DLL_FILENAME, &st) == 0 ) {
				// ddraw3.dll�����݂��Ȃ��Ȃ�΁A���O�ύX
				if ( stat(DDRAW_DLL_FILENAME, &st) != 0 ) {
					::MoveFile( OLD_DDRAW_DLL_FILENAME, DDRAW_DLL_FILENAME);

				// �������݂��Ă�Ȃ�ADDraw.dll�̕��͍폜
				} else if ( stat(DDRAW_DLL_FILENAME, &st) == 0 ) {
					::DeleteFile( OLD_DDRAW_DLL_FILENAME);
				}
			}

		}
		
		hDDrawMod = LoadLibrary( DDRAW_DLL_FILENAME );

		if ( hDDrawMod == NULL ) {
			return FALSE;
		} else {
			// ����DLL�ɓo�^
			
			AddModuleItem( hDDrawMod, "ddraw");

			// �����
			struct stat  st;
			// kao_24bit�f�B���N�g��������΁c
			if (stat("kao_24bit", &st) == 0) {
				AddModuleItem( (HMODULE)hDDrawMod+3+24, "kao_24bit"); //hDDrawMod+3+24 �ȂǂƂ������l�ɈӖ��͂Ȃ��B���j�[�N�ɂ��邽�߂����B
			}
			// kahou_24bit�f�B���N�g��������΁c
			if (stat("kahou_24bit", &st) == 0) {
				AddModuleItem( (HMODULE)hDDrawMod+5+24, "kahou_24bit"); //hDDrawMod+24 �ȂǂƂ������l�ɈӖ��͂Ȃ��B���j�[�N�ɂ��邽�߂����B
			}
		}

		p_DirectDrawCreate = GetProcAddress( hDDrawMod, "DirectDrawCreate" );

		struct stat  st;
		// �t�����ʉ��L�b�g�𓱓����Ă���΁c
		if (stat("n6pwav1.dll", &st) == 0) {
			CommonGlobalHandle::hResourceWavMod=GetModuleHandle("n6pwav1.dll");
			CommonGlobalHandle::hResourceWavMod2=GetModuleHandle("n6pwav2.dll");
			CommonGlobalHandle::hResourceWavMod3=GetModuleHandle("n6pwav3.dll");

			// ����DLL�ɓo�^
			AddModuleItem( CommonGlobalHandle::hResourceWavMod, "n6pwav");

		// �ʏ��ԃV���[�Y�ł���΁c
		} else {
			CommonGlobalHandle::hResourceWavMod=GetModuleHandle("n6pwav.dll");
		}

		// �v���O�C��MOD��OnInitialize()�̌Ăяo��
		LoadLibraryOfPluginMod();

		// �V�i���IMOD
		LoadLibraryOfScenarioMod();

		// �V�i���IMod�J�n�̃��\�b�h
		if ( p_snOnDllMainBgn ) {
			p_snOnDllMainBgn();
		}

		break;
	case DLL_THREAD_ATTACH:

		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:

		// �V�i���I�̕����
		FreeLibraryOfScenarioMod();
		// �v���O�C��Mod�̉��
		FreeLibraryOfPluginMod();

		// DDraw.DLL�̊J��
		FreeLibrary( hDDrawMod );

		// �摜�_�C�A���O�̉��
		FreeImageDialog();

		// SoundModServer�p��SoundIOMap�̉��
		CloseSoundFMap();

		// �����łR�c���n����B�X���b�h�҂�����
		// DestructX3DAudioThread();

		// Lua�C���^�v���^�̏I����BGMmm�����BTSMod�̓C���^�v���^���N�����Ă��Ȃ��B

		// �S�Ԃ̏I��
		onig_end();

		break;
	default:
		break;
	}
	return TRUE;
}
