#include <sys/types.h>
#include <sys/stat.h>

#include "CommonGlobalHandle.h"
#include "SetHelpFileName.h"
#include "Menu.h"



void SetHelpFileName() {
	
	// さらに
	struct stat  st;
	
	// ten95.chm があれば
	if (stat(CHMVER_HELP_FILENAME, &st) == 0 ) {
		AddMenuItem( "操作ガイド(&H)", 121, RESOURCE_MENU_ID_CHM_VER_HELP_EXECUTE);
		RemoveMenu( CommonGlobalHandle::hMenu, 121, FALSE );
	}	
	// winhlp32.exe が天翔記ディレクトリにあれば
	else if (stat(XPVER_HELPEXE_FILENAME, &st) == 0 ) {
		AddMenuItem( "操作ガイド(&H)", 121, RESOURCE_MENU_ID_XP_VER_HELPEXE_EXECUTE);
		RemoveMenu( CommonGlobalHandle::hMenu, 121, FALSE );
	}	
	// ten95.hlp が無く
	else if (stat(CDVER_HELP_FILENAME, &st) != 0 ) {
		// Nobunaga30_06_Ntenshoki.pdf だけがある。即ちDVDの人
		if ( stat(DVDVER_HELP_FILENAME, &st) == 0 ) {
			AddMenuItem( "操作ガイド(&H)", 121, RESOURCE_MENU_ID_DVD_VER_HELP_EXECUTE);
			RemoveMenu( CommonGlobalHandle::hMenu, 121, FALSE );
		}
	}
}

