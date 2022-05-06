#include "GameDataStruct.h"
#include "Menu.h"

// INMMモジュールで再生する予定なのかどうか調べて、そうであれば、メニューに追加する。
void setINMMCustomCheck() {
	if ( is_InmmMode() ) {
		AddModuleItem( (HMODULE)setINMMCustomCheck, "_inmm.dll" ); // (HMODULE)CheckINMMModuleというのはユニークな番号を確保するため。
	}
}