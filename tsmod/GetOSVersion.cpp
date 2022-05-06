#include "WinTarget.h"




double getOSVersion() {

	OSVERSIONINFOA osVerInfo;

	/* OS�̎擾 */	
	osVerInfo.dwOSVersionInfoSize = sizeof(osVerInfo);
	GetVersionExA(&osVerInfo);

	/* OS���̎擾 */
	switch(osVerInfo.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_NT:		//Windows NT�n(NT/2000/XP/vista/7/8)
		return osVerInfo.dwMajorVersion + 0.1*(osVerInfo.dwMinorVersion);
	case VER_PLATFORM_WIN32_WINDOWS: //Windows 9X�n
		return 2;
	default:                         //(windows3.1��AWindows CE�̏ꍇ�����ɗ��܂��B)
		return 1;
	}

}
