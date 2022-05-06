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
#include "ScenarioMod.h"


// sndata.n6p �̒��g
vector<vector<byte>> vSnTaikiBushouN6P;


/*
1534 0  0
1560 2  1
1571 3  2
1582 4  3
1546 1  4
1599 5  5
*/
// sndata.n6p�̃f�[�^���AvSnDataN6P�ւƊi�[����B
int LoadSnTaikiBushouN6PToVector() {

	// taiki.n6p��vSnDataN6P�Ɋi�[
	int error = ls11_DecodePack("taiki.n6p", NULL, &vSnTaikiBushouN6P);

	// �G���[�Ȃ炱���ŁB
	if (error == -1) { return error; }

	// sndata.n6p�ƁA���ۂ̌����ڏ�̕��т����ƂȂ邽�߁B�v����ɃV�i���I�̂h�c�͌����ڂǂ���̏����ł͂Ȃ��Ƃ������ƁB
	swap(vSnTaikiBushouN6P[3], vSnTaikiBushouN6P[4]);
	swap(vSnTaikiBushouN6P[2], vSnTaikiBushouN6P[3]);
	swap(vSnTaikiBushouN6P[1], vSnTaikiBushouN6P[2]);

	if (p_snOnTaikiBushouDataTransfer) {
		p_snOnTaikiBushouDataTransfer(
			&(vSnTaikiBushouN6P[0][0]), vSnTaikiBushouN6P[0].size(),
			&(vSnTaikiBushouN6P[1][0]), vSnTaikiBushouN6P[1].size(),
			&(vSnTaikiBushouN6P[2][0]), vSnTaikiBushouN6P[2].size(),
			&(vSnTaikiBushouN6P[3][0]), vSnTaikiBushouN6P[3].size(),
			&(vSnTaikiBushouN6P[4][0]), vSnTaikiBushouN6P[4].size(),
			&(vSnTaikiBushouN6P[5][0]), vSnTaikiBushouN6P[5].size()
			);
	}

	return 1;
}


// ���ݐi�s�`�̃V�i���I�̑ҋ@�����̓o����𓾂�
NB6TAIKIBUSHOUAPPEAR* GetTaikiBushouAppearArrayPointer() {
	int iScenarioLocation = getCurScenarioLocation();
	if (iScenarioLocation < 0) {
		return NULL;
	}

	if (0 <= iScenarioLocation && iScenarioLocation < (int)vSnTaikiBushouN6P.size()) {
		auto varr = vSnTaikiBushouN6P[iScenarioLocation];
		byte *pvarr = &(varr[0]);

		return (NB6TAIKIBUSHOUAPPEAR *)pvarr;
	}
	else {
		return NULL;
	}

}



// ���ݐi�s�`�̃V�i���I�̑ҋ@�����̓o����𓾂�
NB6TAIKIBUSHOU* GetTaikiBushouArrayPointer() {
	int iScenarioLocation = getCurScenarioLocation();
	if (iScenarioLocation < 0) {
		return NULL;
	}

	if (0 <= iScenarioLocation && iScenarioLocation < (int)vSnTaikiBushouN6P.size()) {
		auto varr = vSnTaikiBushouN6P[iScenarioLocation];
		byte *pvarr = &(varr[0]);
		int add = sizeof(NB6TAIKIBUSHOUAPPEAR) * getTaikiBushouDataNum(); // ������񕔕����܂�܂�p�X�������B
		// �����ɂ͊m���Ƀf�[�^������B
		if (add < (int)varr.size()) {
			pvarr = pvarr + add;
			return (NB6TAIKIBUSHOU *)pvarr;
		}
		else {
			return NULL;
		}
	}
	else {
		return NULL;
	}
}



// ���ݐi�s���Ă���V�i���I�̑ҋ@�����̌ːЏ��𓾂�
NB6TAIKIBUSHOUNAME* GetTaikiBushouNameArrayPointer() {
	int iScenarioLocation = getCurScenarioLocation();
	if (iScenarioLocation < 0) {
		return NULL;
	}

	if (0 <= iScenarioLocation && iScenarioLocation < (int)vSnTaikiBushouN6P.size()) {
		auto varr = vSnTaikiBushouN6P[iScenarioLocation];
		byte *pvarr = &(varr[0]);
		int add = (sizeof(NB6TAIKIBUSHOUAPPEAR) + sizeof(NB6TAIKIBUSHOU)) * getTaikiBushouDataNum(); // ������񕔕����܂�܂�p�X�������B
		// �����ɂ͊m���Ƀf�[�^������B
		if (add < (int)varr.size()) {
			pvarr = pvarr + add;
			return (NB6TAIKIBUSHOUNAME *)pvarr;
		}
		else {
			return NULL;
		}
	}
	else {
		return NULL;
	}
}


