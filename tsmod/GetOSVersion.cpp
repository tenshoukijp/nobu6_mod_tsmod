#include "WinTarget.h"




double getOSVersion() {

	OSVERSIONINFOA osVerInfo;

	/* OSの取得 */	
	osVerInfo.dwOSVersionInfoSize = sizeof(osVerInfo);
	GetVersionExA(&osVerInfo);

	/* OS情報の取得 */
	switch(osVerInfo.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_NT:		//Windows NT系(NT/2000/XP/vista/7/8)
		return osVerInfo.dwMajorVersion + 0.1*(osVerInfo.dwMinorVersion);
	case VER_PLATFORM_WIN32_WINDOWS: //Windows 9X系
		return 2;
	default:                         //(windows3.1や、Windows CEの場合ここに来ます。)
		return 1;
	}

}
