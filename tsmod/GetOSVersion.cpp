#include "WinTarget.h"




double getOSVersion() {

	OSVERSIONINFOA osVerInfo;

	/* OS‚Ìæ“¾ */	
	osVerInfo.dwOSVersionInfoSize = sizeof(osVerInfo);
	GetVersionExA(&osVerInfo);

	/* OSî•ñ‚Ìæ“¾ */
	switch(osVerInfo.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_NT:		//Windows NTŒn(NT/2000/XP/vista/7/8)
		return osVerInfo.dwMajorVersion + 0.1*(osVerInfo.dwMinorVersion);
	case VER_PLATFORM_WIN32_WINDOWS: //Windows 9XŒn
		return 2;
	default:                         //(windows3.1‚âAWindows CE‚Ìê‡‚±‚±‚É—ˆ‚Ü‚·B)
		return 1;
	}

}
