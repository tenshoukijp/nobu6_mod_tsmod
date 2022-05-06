#pragma once


#include "WinTarget.h"
#include <vector>
#include <string>

using namespace std;

extern vector<HMODULE> listOfTargetDLL;


// TargetのdllのOnInitializeメソッド用の型宣言
typedef void (WINAPI *PFNTARGETMODINITIALIZE)(HANDLE);


// TargetのdllのOnMainGameStartメソッド用の型宣言
typedef void (WINAPI *PFNTARGETMODMAINGAMESTART)();


// TargetのdllのOnSelectingScenarioDaimyoStartメソッド用の型宣言
typedef void (WINAPI *PFNTARGETMODSELECTINGSCENARIODAIMYOSTART)();


// TargetのdllのOnChangeSobaメソッド用の型宣言
typedef void (WINAPI *PFNTARGETMODCHANGESOBA)();


// TargetのdllのOnWritingScenarioDetailメソッド用の型宣言
typedef char ** (WINAPI *PFNTARGETMODWRITINGSCENARIODETAIL)(int iScenarioNo);


// TargetのdllのOnFinalizeメソッド用の型宣言
typedef void (WINAPI *PFNTARGETMODFINALISE)();


// TargetのdllのOnApplicatonSelectMenuメソッド用の型宣言
typedef void (WINAPI *PFNTARGETMODAPPLICATIONSELECTMENU)(int iSelectMenu);


// TargetのdllのOnViewingTopMenuメソッド用の型宣言
typedef void (WINAPI *PFNTARGETMODVIEWINGTOPMENU)();


// TargetのdllのOnAfterChangeWindowSizeメソッド用の型宣言
typedef void (WINAPI *PFNTARGETMODAFTERCHANGEWINDOWSIZE)();



// シナリオモッドDLLのロード
BOOL LoadLibraryOfPluginMod();
// シナリオモッドDLLの解放
BOOL FreeLibraryOfPluginMod();