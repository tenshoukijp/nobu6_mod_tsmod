#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSMod.h"
#include "KaoSwapExtend.h"

#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

// DDrawオリジナルハンドル
extern HINSTANCE hDDrawMod;

#define DDRAW_DLL_ENTRYPOINT_TO_DIRECT_DRAW_CREATE						 0x41B0					// DDraw.DLLの先頭アドレスから、DirectDrawCreateまでのアドレスの差分
#define DDRAW_DLL_BUSHOU_NUM_ADDRESS_FROM_DIRECT_DRAW_CREATE			-0x1F9F					// DDraw.DLLのDirectDrawCreateメソッドアドレスから武将の人数制限のアドレスまでの差分。
#define DDRAW_DLL_KAOSWAP_EXTEND_FILENAME_REF_FROM_DIRECT_DRAW_CREATE	-0x1FF2					// DDraw.DLLのDirectDrawCreateメソッドアドレスからKAOSWAPのファイル名までの差分。

#define KAOSWAP_EXTEND_FILENAME									"KAOSWAPEX.N6P"					// 拡張型kaoswap.extendのファイル名。これが存在するとモード発動
#define KAOSWAP_ULTRA_FILENAME									"KAOSWAPUX.N6P"					// 拡張型kaoswap.ultraのファイル名。これが存在するとモード発動
#define TENSHOU_EXE_KAOSWAP_FILENAME_PATCH_ADDRESS				0x4C8D62						// TENSHOU.EXEのKAOSWAP.N6P対象のIOをKAOSWAPEX.N6Pと変更する
																								// DDRAW.DLLの45521BDで、BA 10F355504 のところを置き換えるために使う。


#define TENSHOU_EXE_BUSHOU_NUM_PATCH_ADDRESS1					0x4057DB						// TENSHOU.EXEの1332→1932その１
#define TENSHOU_EXE_BUSHOU_NUM_PATCH_ADDRESS2					0x405837						// TENSHOU.EXEの1332→1932その２
#define TENSHOU_EXE_BUSHOU_NUM_PATCH_ADDRESS3					0x4058E9						// TENSHOU.EXEの1332→1932その３
#define TENSHOU_EXE_BUSHOU_NUM_PATCH_ADDRESS4					0x498A50						// TENSHOU.EXEの1332→1932その４



void checkKaoSwapExtend() {

	struct stat  st;
	int o_DirectDrawCreate = (int)hDDrawMod + DDRAW_DLL_ENTRYPOINT_TO_DIRECT_DRAW_CREATE; // DDraw.dllの先頭アドレスから、0x41B0足しこんだ場所が、

	// ファイルKAOSWAPEX.N6Pの存在のチェックして存在する場合
	if (stat(KAOSWAP_ULTRA_FILENAME, &st) == 0) {

		// KAOSWAPEX.N6Pも特殊モジュールとして認識する。
		AddModuleItem( (HMODULE)KAOSWAP_ULTRA_FILENAME, "kaoswapux.n6p");

		WORD bNum = INT_KAOSWAPUX_BUSHOU_NUM; // 2932人 


		// TENSHOU.EXEのKAOSWAP.N6P対象のIOをKAOSWAPUX.N6Pと変更する
		WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(TENSHOU_EXE_KAOSWAP_FILENAME_PATCH_ADDRESS), KAOSWAP_ULTRA_FILENAME, strlen(KAOSWAP_ULTRA_FILENAME), NULL);

		// TENSHOU.EXEの1332→2932その１
		WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(TENSHOU_EXE_BUSHOU_NUM_PATCH_ADDRESS1), &bNum, 2, NULL);
		// TENSHOU.EXEの1332→2932その２
		WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(TENSHOU_EXE_BUSHOU_NUM_PATCH_ADDRESS2), &bNum, 2, NULL);
		// TENSHOU.EXEの1332→2932その３
		WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(TENSHOU_EXE_BUSHOU_NUM_PATCH_ADDRESS3), &bNum, 2, NULL);
		// TENSHOU.EXEの1332→2932その４
		WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(TENSHOU_EXE_BUSHOU_NUM_PATCH_ADDRESS4), &bNum, 2, NULL);

		// DDraw.DLLがある場合
		if (stat(DDRAW_DLL_FILENAME, &st) == 0) {

			// DDraw.DLLの人数を変更する
			int DDrawDllBushouNumAddress = (int)o_DirectDrawCreate + DDRAW_DLL_BUSHOU_NUM_ADDRESS_FROM_DIRECT_DRAW_CREATE;
			WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(DDrawDllBushouNumAddress), &bNum, 2, NULL);

			// DDraw.DLL内にある"KAOSWAP.N6P"という文字列を指している参照アドレスをTENSHOU.EXE内の顔パック名("KAOSWAPEX.N6P")を指すアドレスへと変更してしまう。
			// そうすることで２つの参照ファイルは一致する。という
			// 凄まじいパッチ…
			int DDrawDllKaoswapExtFileNameRefAddress = (int)o_DirectDrawCreate + DDRAW_DLL_KAOSWAP_EXTEND_FILENAME_REF_FROM_DIRECT_DRAW_CREATE;
			int TenshouExeKaoswapFilenamePatchAddress = TENSHOU_EXE_KAOSWAP_FILENAME_PATCH_ADDRESS;
			WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(DDrawDllKaoswapExtFileNameRefAddress), &TenshouExeKaoswapFilenamePatchAddress, 4, NULL);
		}
	}

	// ファイルKAOSWAPEX.N6Pの存在のチェックして存在する場合
	else if (stat(KAOSWAP_EXTEND_FILENAME, &st) == 0) {

		// KAOSWAPEX.N6Pも特殊モジュールとして認識する。
		AddModuleItem( (HMODULE)KAOSWAP_EXTEND_FILENAME, "kaoswapex.n6p");

		WORD bNum = INT_KAOSWAPEX_BUSHOU_NUM; // 1932人 


		// TENSHOU.EXEのKAOSWAP.N6P対象のIOをKAOSWAPEX.N6Pと変更する
		WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(TENSHOU_EXE_KAOSWAP_FILENAME_PATCH_ADDRESS), KAOSWAP_EXTEND_FILENAME, strlen(KAOSWAP_EXTEND_FILENAME), NULL);

		// TENSHOU.EXEの1332→1932その１
		WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(TENSHOU_EXE_BUSHOU_NUM_PATCH_ADDRESS1), &bNum, 2, NULL);
		// TENSHOU.EXEの1332→1932その２
		WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(TENSHOU_EXE_BUSHOU_NUM_PATCH_ADDRESS2), &bNum, 2, NULL);
		// TENSHOU.EXEの1332→1932その３
		WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(TENSHOU_EXE_BUSHOU_NUM_PATCH_ADDRESS3), &bNum, 2, NULL);
		// TENSHOU.EXEの1332→1932その４
		WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(TENSHOU_EXE_BUSHOU_NUM_PATCH_ADDRESS4), &bNum, 2, NULL);

		// DDraw.DLLがある場合
		if (stat(DDRAW_DLL_FILENAME, &st) == 0) {

			// DDraw.DLLの人数を変更する
			int DDrawDllBushouNumAddress = (int)o_DirectDrawCreate + DDRAW_DLL_BUSHOU_NUM_ADDRESS_FROM_DIRECT_DRAW_CREATE;
			WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(DDrawDllBushouNumAddress), &bNum, 2, NULL);

			// DDraw.DLL内にある"KAOSWAP.N6P"という文字列を指している参照アドレスをTENSHOU.EXE内の顔パック名("KAOSWAPEX.N6P")を指すアドレスへと変更してしまう。
			// そうすることで２つの参照ファイルは一致する。という
			// 凄まじいパッチ…
			int DDrawDllKaoswapExtFileNameRefAddress = (int)o_DirectDrawCreate + DDRAW_DLL_KAOSWAP_EXTEND_FILENAME_REF_FROM_DIRECT_DRAW_CREATE;
			int TenshouExeKaoswapFilenamePatchAddress = TENSHOU_EXE_KAOSWAP_FILENAME_PATCH_ADDRESS;
			WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(DDrawDllKaoswapExtFileNameRefAddress), &TenshouExeKaoswapFilenamePatchAddress, 4, NULL);
		}
	}



}

