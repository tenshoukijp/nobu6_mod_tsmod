#include "WinTarget.h"
#include <iostream>
#include <fstream>

using namespace std;

#include "EXBushou_LancelotTartare.h"



EXBushou_LancelotTartare::EXBushou_LancelotTartare() : EXBushou() {
	// �R���X�g���N�^�̈����ɂ���Ɣėp���������̂Ŕ����邱�ƁB
	setName("�C�D�E�@�A�B");
}

bool isLancelotTartare(int iBushouID) {
	return (
		strcmp(nb6bushouname[iBushouID].familyname, "�C�D�E")==0 &&
		strcmp(nb6bushouname[iBushouID].fastname, "�@�A�B")==0
	);
}

