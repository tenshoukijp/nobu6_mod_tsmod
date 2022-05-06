#include <sys/types.h>
#include <sys/stat.h>


#include "GameDataStruct.h"
#include "Menu.h"

// INMMモジュールで再生する予定なのかどうか調べて、そうであれば、メニューに追加する。
void setGodPatchCustomCheck() {
	struct stat  st;
	if ( stat("nobu6.exe", &st) == 0 ) {
		AddModuleItem( (HMODULE)setGodPatchCustomCheck, "神パッチ" ); // (HMODULE)setGodPatchCustomCheckというのはユニークな番号を確保するため。
	}
}

