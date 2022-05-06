#pragma once


#include "WinTarget.h"
#include "EXBushou.h"


class EXBushou_LancelotTartare : public EXBushou {
	public:
		EXBushou_LancelotTartare();
};

// 該当の武将IDはランスロットタルタロスである。
bool isLancelotTartare ( int iBushouID );