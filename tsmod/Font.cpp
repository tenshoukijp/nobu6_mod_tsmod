#include "Font.h"
#include "GameDataStruct.h"
#include "TSMod.h"

void setFontCustomCheck() {
	// フォントを指している場所
	char *szFontName = (char *)GAMEDATASTRUCT_FONTNAME_ADDRESS;

	// ＭＳゴシック以外なら、特別なモジュールリストに追加
	if ( strcmp(szFontName, "ＭＳ ゴシック") != 0 ) {
		AddModuleItem( (HMODULE)setFontCustomCheck, szFontName); //(HMODULE)setFontCustomCheck自体に特別な意味はない、ユニークな数値を当てるため 
	}
}

