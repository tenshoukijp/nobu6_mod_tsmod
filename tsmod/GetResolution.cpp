#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"


// �𑜓x�̏c�𓾂�B�E�B���h�E�T�C�Y�Ƃ��������A���ۂ̓V�ċL�Ƃ��ẴT�C�Y(1024x768��1600x768�Ȃ�)
int GetResolutionWidth() {
	int *pWidth = (int *)GAMEDATASTRUCT_RESOLUTION_ADDRESS;
	return *pWidth;
}

// �𑜓x�̉��𓾂�B�E�B�h�E�T�C�Y�Ƃ��������A���ۂ̓V�ċL�Ƃ��ẴT�C�Y(1024x768��1600x768�Ȃ�)
int GetResolutionHeight() {
	int *pWidth = (int *)GAMEDATASTRUCT_RESOLUTION_ADDRESS;
	return *(pWidth+1);
}
