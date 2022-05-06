#include "WinTarget.h"

#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "ScenarioMessageN6PPatch.h"
#include "ScenarioDataStruct.h"
#include "ls11_mod.h"


extern CurProcessMemory CPM;
extern HANDLE g_hModule;


// sndata.n6p �̒��g
vector<vector<byte>> vSnDataN6P;


/*
1534 0  0
1560 2  1
1571 3  2
1582 4  3
1546 1  4
1599 5  5
*/
// sndata.n6p�̃f�[�^���AvSnDataN6P�ւƊi�[����B
int LoadSnDataN6PToVector() {

	// sndata.n6p��vSnDataN6P�Ɋi�[
	int error = ls11_DecodePack("sndata.n6p", NULL, &vSnDataN6P);

	// �G���[�Ȃ炱���ŁB
	if ( error == -1 ) { return error; }

	// sndata.n6p�ƁA���ۂ̌����ڏ�̕��т����ƂȂ邽�߁B�v����ɃV�i���I�̂h�c�͌����ڂǂ���̏����ł͂Ȃ��Ƃ������ƁB
	swap(vSnDataN6P[3],vSnDataN6P[4]);
	swap(vSnDataN6P[2],vSnDataN6P[3]);
	swap(vSnDataN6P[1],vSnDataN6P[2]);

	return 1;
}