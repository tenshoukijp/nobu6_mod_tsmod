#include <sys/types.h>
#include <sys/stat.h>


#include "GameDataStruct.h"
#include "Menu.h"

// INMM���W���[���ōĐ�����\��Ȃ̂��ǂ������ׂāA�����ł���΁A���j���[�ɒǉ�����B
void setGodPatchCustomCheck() {
	struct stat  st;
	if ( stat("nobu6.exe", &st) == 0 ) {
		AddModuleItem( (HMODULE)setGodPatchCustomCheck, "�_�p�b�`" ); // (HMODULE)setGodPatchCustomCheck�Ƃ����̂̓��j�[�N�Ȕԍ����m�ۂ��邽�߁B
	}
}

