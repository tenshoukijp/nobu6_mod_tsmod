#include "GameDataStruct.h"
#include "Menu.h"

// INMM���W���[���ōĐ�����\��Ȃ̂��ǂ������ׂāA�����ł���΁A���j���[�ɒǉ�����B
void setINMMCustomCheck() {
	if ( is_InmmMode() ) {
		AddModuleItem( (HMODULE)setINMMCustomCheck, "_inmm.dll" ); // (HMODULE)CheckINMMModule�Ƃ����̂̓��j�[�N�Ȕԍ����m�ۂ��邽�߁B
	}
}