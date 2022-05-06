// TSMod.cpp : TSModのエントリポイント
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


// DDrawオリジナルハンドル
HINSTANCE hDDrawMod = NULL;

// DDrawオリジナルメソッドポインタ
FARPROC p_DirectDrawCreate;

/*
 * コマンドライン引数は、__argc, __argvに入っていて、グローバル変数であり、どこからでも参照可能。
 */


BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved ) {
	switch( ul_reason_for_call )
	{
	case DLL_PROCESS_ATTACH:

		srand( (unsigned int)time( NULL ) ); // 乱数の種を変える

		// 一応グローバルに参照できるように保存
		CommonGlobalHandle::hDLLModule = hModule;

		// アンチデバッグ処理
		// if ( AntiDebugALL() ) { break; }

		// ローケル文字列の設定のみ。
		// ワイド文字列のロケール(wcout系)への設定。
		// Activate std::wcout.
		locale::global(std::locale("")); 
		wcout.imbue(std::locale(""));

		// 鬼車の初期化
		onig_init();

		LoadLibraryOfDarkModeTitleBarMod();

		// LUAインタプリタの起動はBGMmmがやる。TSModはインタプリタを起動していない。
		// 起動済のインタプリタのluaState=L を貰う。
		L = luaL_getstate();

		// DDrawを読みにかかる。少なくともWindows版の存在が必須
		
		{
			// さらに
			struct stat  st;
			// ddraw.dllが存在し、
			if (stat(OLD_DDRAW_DLL_FILENAME, &st) == 0 ) {
				// ddraw3.dllが存在しないならば、名前変更
				if ( stat(DDRAW_DLL_FILENAME, &st) != 0 ) {
					::MoveFile( OLD_DDRAW_DLL_FILENAME, DDRAW_DLL_FILENAME);

				// 両方存在してるなら、DDraw.dllの方は削除
				} else if ( stat(DDRAW_DLL_FILENAME, &st) == 0 ) {
					::DeleteFile( OLD_DDRAW_DLL_FILENAME);
				}
			}

		}
		
		hDDrawMod = LoadLibrary( DDRAW_DLL_FILENAME );

		if ( hDDrawMod == NULL ) {
			return FALSE;
		} else {
			// 特殊DLLに登録
			
			AddModuleItem( hDDrawMod, "ddraw");

			// さらに
			struct stat  st;
			// kao_24bitディレクトリがあれば…
			if (stat("kao_24bit", &st) == 0) {
				AddModuleItem( (HMODULE)hDDrawMod+3+24, "kao_24bit"); //hDDrawMod+3+24 などという数値に意味はない。ユニークにするためだけ。
			}
			// kahou_24bitディレクトリがあれば…
			if (stat("kahou_24bit", &st) == 0) {
				AddModuleItem( (HMODULE)hDDrawMod+5+24, "kahou_24bit"); //hDDrawMod+24 などという数値に意味はない。ユニークにするためだけ。
			}
		}

		p_DirectDrawCreate = GetProcAddress( hDDrawMod, "DirectDrawCreate" );

		struct stat  st;
		// フル効果音キットを導入していれば…
		if (stat("n6pwav1.dll", &st) == 0) {
			CommonGlobalHandle::hResourceWavMod=GetModuleHandle("n6pwav1.dll");
			CommonGlobalHandle::hResourceWavMod2=GetModuleHandle("n6pwav2.dll");
			CommonGlobalHandle::hResourceWavMod3=GetModuleHandle("n6pwav3.dll");

			// 特殊DLLに登録
			AddModuleItem( CommonGlobalHandle::hResourceWavMod, "n6pwav");

		// 通常定番シリーズであれば…
		} else {
			CommonGlobalHandle::hResourceWavMod=GetModuleHandle("n6pwav.dll");
		}

		// プラグインMODのOnInitialize()の呼び出し
		LoadLibraryOfPluginMod();

		// シナリオMOD
		LoadLibraryOfScenarioMod();

		// シナリオMod開始のメソッド
		if ( p_snOnDllMainBgn ) {
			p_snOnDllMainBgn();
		}

		break;
	case DLL_THREAD_ATTACH:

		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:

		// シナリオの方解放
		FreeLibraryOfScenarioMod();
		// プラグインModの解放
		FreeLibraryOfPluginMod();

		// DDraw.DLLの開放
		FreeLibrary( hDDrawMod );

		// 画像ダイアログの解放
		FreeImageDialog();

		// SoundModServer用のSoundIOMapの解放
		CloseSoundFMap();

		// ここで３Ｄ音系解放。スレッド待ちあり
		// DestructX3DAudioThread();

		// Luaインタプリタの終了はBGMmmがやる。TSModはインタプリタを起動していない。

		// 鬼車の終了
		onig_end();

		break;
	default:
		break;
	}
	return TRUE;
}
