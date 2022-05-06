#pragma once


#include "WinTarget.h"
#include <vector>
#include <string>

using namespace std;

extern vector<HMODULE> listOfTargetDLL;


// Target��dll��OnInitialize���\�b�h�p�̌^�錾
typedef void (WINAPI *PFNTARGETMODINITIALIZE)(HANDLE);


// Target��dll��OnMainGameStart���\�b�h�p�̌^�錾
typedef void (WINAPI *PFNTARGETMODMAINGAMESTART)();


// Target��dll��OnSelectingScenarioDaimyoStart���\�b�h�p�̌^�錾
typedef void (WINAPI *PFNTARGETMODSELECTINGSCENARIODAIMYOSTART)();


// Target��dll��OnChangeSoba���\�b�h�p�̌^�錾
typedef void (WINAPI *PFNTARGETMODCHANGESOBA)();


// Target��dll��OnWritingScenarioDetail���\�b�h�p�̌^�錾
typedef char ** (WINAPI *PFNTARGETMODWRITINGSCENARIODETAIL)(int iScenarioNo);


// Target��dll��OnFinalize���\�b�h�p�̌^�錾
typedef void (WINAPI *PFNTARGETMODFINALISE)();


// Target��dll��OnApplicatonSelectMenu���\�b�h�p�̌^�錾
typedef void (WINAPI *PFNTARGETMODAPPLICATIONSELECTMENU)(int iSelectMenu);


// Target��dll��OnViewingTopMenu���\�b�h�p�̌^�錾
typedef void (WINAPI *PFNTARGETMODVIEWINGTOPMENU)();


// Target��dll��OnAfterChangeWindowSize���\�b�h�p�̌^�錾
typedef void (WINAPI *PFNTARGETMODAFTERCHANGEWINDOWSIZE)();



// �V�i���I���b�hDLL�̃��[�h
BOOL LoadLibraryOfPluginMod();
// �V�i���I���b�hDLL�̉��
BOOL FreeLibraryOfPluginMod();