#include <sys/types.h>
#include <sys/stat.h>

#include "CommonGlobalHandle.h"
#include "SetHelpFileName.h"
#include "Menu.h"



void SetHelpFileName() {
	
	// �����
	struct stat  st;
	
	// ten95.chm �������
	if (stat(CHMVER_HELP_FILENAME, &st) == 0 ) {
		AddMenuItem( "����K�C�h(&H)", 121, RESOURCE_MENU_ID_CHM_VER_HELP_EXECUTE);
		RemoveMenu( CommonGlobalHandle::hMenu, 121, FALSE );
	}	
	// winhlp32.exe ���V�ċL�f�B���N�g���ɂ����
	else if (stat(XPVER_HELPEXE_FILENAME, &st) == 0 ) {
		AddMenuItem( "����K�C�h(&H)", 121, RESOURCE_MENU_ID_XP_VER_HELPEXE_EXECUTE);
		RemoveMenu( CommonGlobalHandle::hMenu, 121, FALSE );
	}	
	// ten95.hlp ������
	else if (stat(CDVER_HELP_FILENAME, &st) != 0 ) {
		// Nobunaga30_06_Ntenshoki.pdf ����������B����DVD�̐l
		if ( stat(DVDVER_HELP_FILENAME, &st) == 0 ) {
			AddMenuItem( "����K�C�h(&H)", 121, RESOURCE_MENU_ID_DVD_VER_HELP_EXECUTE);
			RemoveMenu( CommonGlobalHandle::hMenu, 121, FALSE );
		}
	}
}

