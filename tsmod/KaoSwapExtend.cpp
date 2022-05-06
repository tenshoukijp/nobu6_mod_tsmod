#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSMod.h"
#include "KaoSwapExtend.h"

#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

// DDraw�I���W�i���n���h��
extern HINSTANCE hDDrawMod;

#define DDRAW_DLL_ENTRYPOINT_TO_DIRECT_DRAW_CREATE						 0x41B0					// DDraw.DLL�̐擪�A�h���X����ADirectDrawCreate�܂ł̃A�h���X�̍���
#define DDRAW_DLL_BUSHOU_NUM_ADDRESS_FROM_DIRECT_DRAW_CREATE			-0x1F9F					// DDraw.DLL��DirectDrawCreate���\�b�h�A�h���X���畐���̐l�������̃A�h���X�܂ł̍����B
#define DDRAW_DLL_KAOSWAP_EXTEND_FILENAME_REF_FROM_DIRECT_DRAW_CREATE	-0x1FF2					// DDraw.DLL��DirectDrawCreate���\�b�h�A�h���X����KAOSWAP�̃t�@�C�����܂ł̍����B

#define KAOSWAP_EXTEND_FILENAME									"KAOSWAPEX.N6P"					// �g���^kaoswap.extend�̃t�@�C�����B���ꂪ���݂���ƃ��[�h����
#define KAOSWAP_ULTRA_FILENAME									"KAOSWAPUX.N6P"					// �g���^kaoswap.ultra�̃t�@�C�����B���ꂪ���݂���ƃ��[�h����
#define TENSHOU_EXE_KAOSWAP_FILENAME_PATCH_ADDRESS				0x4C8D62						// TENSHOU.EXE��KAOSWAP.N6P�Ώۂ�IO��KAOSWAPEX.N6P�ƕύX����
																								// DDRAW.DLL��45521BD�ŁABA 10F355504 �̂Ƃ����u�������邽�߂Ɏg���B


#define TENSHOU_EXE_BUSHOU_NUM_PATCH_ADDRESS1					0x4057DB						// TENSHOU.EXE��1332��1932���̂P
#define TENSHOU_EXE_BUSHOU_NUM_PATCH_ADDRESS2					0x405837						// TENSHOU.EXE��1332��1932���̂Q
#define TENSHOU_EXE_BUSHOU_NUM_PATCH_ADDRESS3					0x4058E9						// TENSHOU.EXE��1332��1932���̂R
#define TENSHOU_EXE_BUSHOU_NUM_PATCH_ADDRESS4					0x498A50						// TENSHOU.EXE��1332��1932���̂S



void checkKaoSwapExtend() {

	struct stat  st;
	int o_DirectDrawCreate = (int)hDDrawMod + DDRAW_DLL_ENTRYPOINT_TO_DIRECT_DRAW_CREATE; // DDraw.dll�̐擪�A�h���X����A0x41B0�������񂾏ꏊ���A

	// �t�@�C��KAOSWAPEX.N6P�̑��݂̃`�F�b�N���đ��݂���ꍇ
	if (stat(KAOSWAP_ULTRA_FILENAME, &st) == 0) {

		// KAOSWAPEX.N6P�����ꃂ�W���[���Ƃ��ĔF������B
		AddModuleItem( (HMODULE)KAOSWAP_ULTRA_FILENAME, "kaoswapux.n6p");

		WORD bNum = INT_KAOSWAPUX_BUSHOU_NUM; // 2932�l 


		// TENSHOU.EXE��KAOSWAP.N6P�Ώۂ�IO��KAOSWAPUX.N6P�ƕύX����
		WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(TENSHOU_EXE_KAOSWAP_FILENAME_PATCH_ADDRESS), KAOSWAP_ULTRA_FILENAME, strlen(KAOSWAP_ULTRA_FILENAME), NULL);

		// TENSHOU.EXE��1332��2932���̂P
		WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(TENSHOU_EXE_BUSHOU_NUM_PATCH_ADDRESS1), &bNum, 2, NULL);
		// TENSHOU.EXE��1332��2932���̂Q
		WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(TENSHOU_EXE_BUSHOU_NUM_PATCH_ADDRESS2), &bNum, 2, NULL);
		// TENSHOU.EXE��1332��2932���̂R
		WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(TENSHOU_EXE_BUSHOU_NUM_PATCH_ADDRESS3), &bNum, 2, NULL);
		// TENSHOU.EXE��1332��2932���̂S
		WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(TENSHOU_EXE_BUSHOU_NUM_PATCH_ADDRESS4), &bNum, 2, NULL);

		// DDraw.DLL������ꍇ
		if (stat(DDRAW_DLL_FILENAME, &st) == 0) {

			// DDraw.DLL�̐l����ύX����
			int DDrawDllBushouNumAddress = (int)o_DirectDrawCreate + DDRAW_DLL_BUSHOU_NUM_ADDRESS_FROM_DIRECT_DRAW_CREATE;
			WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(DDrawDllBushouNumAddress), &bNum, 2, NULL);

			// DDraw.DLL���ɂ���"KAOSWAP.N6P"�Ƃ�����������w���Ă���Q�ƃA�h���X��TENSHOU.EXE���̊�p�b�N��("KAOSWAPEX.N6P")���w���A�h���X�ւƕύX���Ă��܂��B
			// �������邱�ƂłQ�̎Q�ƃt�@�C���͈�v����B�Ƃ���
			// ���܂����p�b�`�c
			int DDrawDllKaoswapExtFileNameRefAddress = (int)o_DirectDrawCreate + DDRAW_DLL_KAOSWAP_EXTEND_FILENAME_REF_FROM_DIRECT_DRAW_CREATE;
			int TenshouExeKaoswapFilenamePatchAddress = TENSHOU_EXE_KAOSWAP_FILENAME_PATCH_ADDRESS;
			WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(DDrawDllKaoswapExtFileNameRefAddress), &TenshouExeKaoswapFilenamePatchAddress, 4, NULL);
		}
	}

	// �t�@�C��KAOSWAPEX.N6P�̑��݂̃`�F�b�N���đ��݂���ꍇ
	else if (stat(KAOSWAP_EXTEND_FILENAME, &st) == 0) {

		// KAOSWAPEX.N6P�����ꃂ�W���[���Ƃ��ĔF������B
		AddModuleItem( (HMODULE)KAOSWAP_EXTEND_FILENAME, "kaoswapex.n6p");

		WORD bNum = INT_KAOSWAPEX_BUSHOU_NUM; // 1932�l 


		// TENSHOU.EXE��KAOSWAP.N6P�Ώۂ�IO��KAOSWAPEX.N6P�ƕύX����
		WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(TENSHOU_EXE_KAOSWAP_FILENAME_PATCH_ADDRESS), KAOSWAP_EXTEND_FILENAME, strlen(KAOSWAP_EXTEND_FILENAME), NULL);

		// TENSHOU.EXE��1332��1932���̂P
		WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(TENSHOU_EXE_BUSHOU_NUM_PATCH_ADDRESS1), &bNum, 2, NULL);
		// TENSHOU.EXE��1332��1932���̂Q
		WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(TENSHOU_EXE_BUSHOU_NUM_PATCH_ADDRESS2), &bNum, 2, NULL);
		// TENSHOU.EXE��1332��1932���̂R
		WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(TENSHOU_EXE_BUSHOU_NUM_PATCH_ADDRESS3), &bNum, 2, NULL);
		// TENSHOU.EXE��1332��1932���̂S
		WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(TENSHOU_EXE_BUSHOU_NUM_PATCH_ADDRESS4), &bNum, 2, NULL);

		// DDraw.DLL������ꍇ
		if (stat(DDRAW_DLL_FILENAME, &st) == 0) {

			// DDraw.DLL�̐l����ύX����
			int DDrawDllBushouNumAddress = (int)o_DirectDrawCreate + DDRAW_DLL_BUSHOU_NUM_ADDRESS_FROM_DIRECT_DRAW_CREATE;
			WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(DDrawDllBushouNumAddress), &bNum, 2, NULL);

			// DDraw.DLL���ɂ���"KAOSWAP.N6P"�Ƃ�����������w���Ă���Q�ƃA�h���X��TENSHOU.EXE���̊�p�b�N��("KAOSWAPEX.N6P")���w���A�h���X�ւƕύX���Ă��܂��B
			// �������邱�ƂłQ�̎Q�ƃt�@�C���͈�v����B�Ƃ���
			// ���܂����p�b�`�c
			int DDrawDllKaoswapExtFileNameRefAddress = (int)o_DirectDrawCreate + DDRAW_DLL_KAOSWAP_EXTEND_FILENAME_REF_FROM_DIRECT_DRAW_CREATE;
			int TenshouExeKaoswapFilenamePatchAddress = TENSHOU_EXE_KAOSWAP_FILENAME_PATCH_ADDRESS;
			WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(DDrawDllKaoswapExtFileNameRefAddress), &TenshouExeKaoswapFilenamePatchAddress, 4, NULL);
		}
	}



}

