#pragma once

#pragma pack(1)

#include <windows.h>

#define CHMVER_HELP_FILENAME  "ten95.chm"
#define CDVER_HELP_FILENAME  "ten95.hlp"
#define DVDVER_HELP_FILENAME "Nobunaga30_06_Ntenshoki.pdf"

#define XPVER_HELPEXE_FILENAME  "winhlpxp.exe"

#define RESOURCE_MENU_ID_DVD_VER_HELP_EXECUTE			40121
#define RESOURCE_MENU_ID_CHM_VER_HELP_EXECUTE			40122
#define RESOURCE_MENU_ID_XP_VER_HELPEXE_EXECUTE			40123

void SetHelpFileName();