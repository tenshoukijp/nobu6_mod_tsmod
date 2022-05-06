#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"


// 解像度の縦を得る。ウィンドウサイズというよりも、実際の天翔記としてのサイズ(1024x768や1600x768など)
int GetResolutionWidth() {
	int *pWidth = (int *)GAMEDATASTRUCT_RESOLUTION_ADDRESS;
	return *pWidth;
}

// 解像度の横を得る。ウィドウサイズというよりも、実際の天翔記としてのサイズ(1024x768や1600x768など)
int GetResolutionHeight() {
	int *pWidth = (int *)GAMEDATASTRUCT_RESOLUTION_ADDRESS;
	return *(pWidth+1);
}
