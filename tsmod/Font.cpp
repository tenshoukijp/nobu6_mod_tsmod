#include "Font.h"
#include "GameDataStruct.h"
#include "TSMod.h"

void setFontCustomCheck() {
	// �t�H���g���w���Ă���ꏊ
	char *szFontName = (char *)GAMEDATASTRUCT_FONTNAME_ADDRESS;

	// �l�r�S�V�b�N�ȊO�Ȃ�A���ʂȃ��W���[�����X�g�ɒǉ�
	if ( strcmp(szFontName, "�l�r �S�V�b�N") != 0 ) {
		AddModuleItem( (HMODULE)setFontCustomCheck, szFontName); //(HMODULE)setFontCustomCheck���̂ɓ��ʂȈӖ��͂Ȃ��A���j�[�N�Ȑ��l�𓖂Ă邽�� 
	}
}

