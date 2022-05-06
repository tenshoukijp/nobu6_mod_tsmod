#include <Windows.h>
#include "DarkModeTitleBarMod.h"

HMODULE pDarkModeTitleBarDll = NULL;
PFMDARKMODETITLEBAR_ONWMSIZE pDarkModeTitleBar_OnWmSize = NULL;

void LoadLibraryOfDarkModeTitleBarMod() {
	if (!pDarkModeTitleBarDll) {
		pDarkModeTitleBarDll = LoadLibrary("DarkModeTitleBarMod.dll");

		if (pDarkModeTitleBarDll) {
			pDarkModeTitleBar_OnWmSize = (PFMDARKMODETITLEBAR_ONWMSIZE)GetProcAddress(pDarkModeTitleBarDll, "OnWmSize");
		}
	}
}