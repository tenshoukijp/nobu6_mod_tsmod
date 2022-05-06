#include "WinTarget.h"
#include <fstream>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"

using namespace std;

#pragma pack(1)


// ���݂̃Q�[���V�i���I��̊J�n�N�𓾂�B
int getStartYearADOfScenarios() {
	int iSnID = getCurScenarioLocation();
	// �����ƑI��������ł���B
	if ( 0 <= iSnID && iSnID <= 5 ) {
		// ���łɂ����Ƒ������Ă���B
		if ( iSnID < (int)vSnDataN6P.size() ) {
			int ret = 0;
			try {
				ret = vSnDataN6P[iSnID][0xb7];
			} catch (exception e) {
			}
			return ret + getStartYearAD();
		}
	}
	return 0;
}
