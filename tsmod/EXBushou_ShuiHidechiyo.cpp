#include "WinTarget.h"
#include <iostream>
#include <fstream>

using namespace std;

#include "EXBushou_ShuiHidechiyo.h"


EXBushou_ShuiHidechiyo::EXBushou_ShuiHidechiyo() : EXBushou() {
	// �R���X�g���N�^�̈����ɂ���Ɣėp���������̂Ŕ����邱�ƁB
	setName("��ɏG���");
}

bool isShuiHidechiyo(int iBushouID) {
	return (
		strcmp(nb6bushouname[iBushouID].familyname, "���")==0 &&
		strcmp(nb6bushouname[iBushouID].fastname, "�G���")==0
	);
}

