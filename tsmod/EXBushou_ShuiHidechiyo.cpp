#include "WinTarget.h"
#include <iostream>
#include <fstream>

using namespace std;

#include "EXBushou_ShuiHidechiyo.h"


EXBushou_ShuiHidechiyo::EXBushou_ShuiHidechiyo() : EXBushou() {
	// コンストラクタの引数にすると汎用性を失うので避けること。
	setName("朱伊秀千代");
}

bool isShuiHidechiyo(int iBushouID) {
	return (
		strcmp(nb6bushouname[iBushouID].familyname, "朱伊")==0 &&
		strcmp(nb6bushouname[iBushouID].fastname, "秀千代")==0
	);
}

