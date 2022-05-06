#pragma once

#include <Windows.h>

using PFMDARKMODETITLEBAR_ONWMSIZE = void (WINAPI *)();

extern HMODULE pDarkModeTitleBarDll;
extern PFMDARKMODETITLEBAR_ONWMSIZE pDarkModeTitleBar_OnWmSize;

extern void LoadLibraryOfDarkModeTitleBarMod();