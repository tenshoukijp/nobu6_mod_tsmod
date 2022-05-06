#include "CommonGlobalHandle.h"
#include "Menu.h"

#define SOUNDSERVER_FILENAME  "SNDmm.dll"


// ３Ｄ音響サーバーを起動
void OnTenshouExeCallSoundModServer() {
	// SoundMMが呼ばれてる？
	HMODULE h = GetModuleHandle(SOUNDSERVER_FILENAME);

	// ここしか呼び出していないので、無視してロード
	HINSTANCE irtnValue = ShellExecute(NULL, NULL, "rundll32.exe", SOUNDSERVER_FILENAME " EntryPoint", NULL, SW_HIDE);

	if ( (int)irtnValue > 32 ) { // 関数が成功すると、32 より大きい値が返ります。関数が失敗すると、32 以下の値が返ります。
		// AddModuleItem( (HMODULE)OnTenshouExeCallSoundModServer, "sndmm" ); // (HMODULE)CheckINMMModuleというのはユニークな番号を確保するため。
	}

}

