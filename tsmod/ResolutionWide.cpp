
#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "TSModCommand.h"
#include "TSMod.h"
#include "KaoSwapExtend.h"

#include <sys/types.h>
#include <sys/stat.h>

/*
大解像度に変更する。横幅を1600等にする
0049918E     BF 00040000    MOV     EDI, 400 → 640(1600)や780(1920)への書き換え
*/

// DDrawオリジナルハンドル
extern HINSTANCE hDDrawMod;

#define DDRAW_DLL_ENTRYPOINT_TO_DIRECT_DRAW_CREATE						 0x41B0					// DDraw.DLLの先頭アドレスから、DirectDrawCreateまでのアドレスの差分
#define DDRAW_DLL_RESOLUTION1_ADDRESS_FROM_DIRECT_DRAW_CREATE			-0x29b8					// DDraw.DLLのDirectDrawCreateメソッドアドレスから解像度の大きさの値が入ってるアドレス１
#define DDRAW_DLL_RESOLUTION2_ADDRESS_FROM_DIRECT_DRAW_CREATE			-0xdcc					// DDraw.DLLのDirectDrawCreateメソッドアドレスから解像度の大きさの値が入ってるアドレス２
#define DDRAW_DLL_RESOLUTION3_ADDRESS_FROM_DIRECT_DRAW_CREATE			-0x2b6					// DDraw.DLLのDirectDrawCreateメソッドアドレスから解像度の大きさの値が入ってるアドレス３
#define DDRAW_DLL_RESOLUTION4_ADDRESS_FROM_DIRECT_DRAW_CREATE			-0xa07					// DDraw.DLLのDirectDrawCreateメソッドアドレスから解像度の大きさの値が入ってるアドレス４
#define DDRAW_DLL_RESOLUTION5_ADDRESS_FROM_DIRECT_DRAW_CREATE			-0x92b					// DDraw.DLLのDirectDrawCreateメソッドアドレスから解像度の大きさの値が入ってるアドレス５

/* 以下の400→600へ
5DEF41B0 > 833D 8C39505E 00       CMP     DWORD PTR DS:[5E50398C], 0		DirectDraw

5DEF17F7   68 00040000                                          PUSH    400

5DEF33E3   68 00040000                                          PUSH    400

5DEF3EF4   C705 6C354F5E 00040000                               MOV     DWORD PTR DS:[5E4F356C], 400

5DEF37A7   81C3 00040000										ADD     EBX, 400

5DEF3881   814424 10 00040000									ADD     DWORD PTR SS:[ESP+10], 400
*/

int iConstWideResolutionSize = 0x600; // 1536, 2720   (MAXは0xAA0まで可能。常に全マップ表示される)



void setWideResolution() {
	if ( TSModCommand::Environment::cmdWideResolution ) {

		// TENSHOU.EXEの解像度関連の書き換え
		int iWidthOfSettings = iConstWideResolutionSize;
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x49918F), &iWidthOfSettings, 4, NULL);

		char szWidthtOf1600[5];
		wsprintf(szWidthtOf1600, "%d", iWidthOfSettings);
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x4D32CE), szWidthtOf1600, 4, NULL);

		// DirectDrawの解像度関連の書き換え
		struct stat  st;
		int o_DirectDrawCreate = (int)hDDrawMod + DDRAW_DLL_ENTRYPOINT_TO_DIRECT_DRAW_CREATE; // DDraw.dllの先頭アドレスから、0x41B0足しこんだ場所が、

		// DDraw.DLLがある場合
		if (stat(DDRAW_DLL_FILENAME, &st) == 0) {

			// DDraw.DLLの解像度を変更する
			int DDrawDllResulution1Address = (int)o_DirectDrawCreate + DDRAW_DLL_RESOLUTION1_ADDRESS_FROM_DIRECT_DRAW_CREATE;

			WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(DDrawDllResulution1Address), &iWidthOfSettings, 2, NULL);

			// DDraw.DLLの解像度を変更する
			int DDrawDllResulution2Address = (int)o_DirectDrawCreate + DDRAW_DLL_RESOLUTION2_ADDRESS_FROM_DIRECT_DRAW_CREATE;

			WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(DDrawDllResulution2Address), &iWidthOfSettings, 2, NULL);

			// DDraw.DLLの解像度を変更する
			int DDrawDllResulution4Address = (int)o_DirectDrawCreate + DDRAW_DLL_RESOLUTION4_ADDRESS_FROM_DIRECT_DRAW_CREATE;

			WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(DDrawDllResulution4Address), &iWidthOfSettings, 2, NULL);


			// DDraw.DLLの解像度を変更する
			int DDrawDllResulution5Address = (int)o_DirectDrawCreate + DDRAW_DLL_RESOLUTION5_ADDRESS_FROM_DIRECT_DRAW_CREATE;

			WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(DDrawDllResulution5Address), &iWidthOfSettings, 2, NULL);

			// これをすると、顔グラがフルカラーでなくなる。
			// DDraw.DLLの解像度を変更する
			int DDrawDllResulution3Address = (int)o_DirectDrawCreate + DDRAW_DLL_RESOLUTION3_ADDRESS_FROM_DIRECT_DRAW_CREATE;

			WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(DDrawDllResulution3Address), &iWidthOfSettings, 2, NULL);

		}
	}
}

/*
DDraw.sysの下４桁==****3EFAのアドレスの数値を書き換えることが運命の分かれ道となる。

***4016   890D 6C352C56    MOV     DWORD PTR DS:[562C356C], ECX  実際のクライアントのサイズ
55CC1344   A3 6C352C56      MOV     DWORD PTR DS:[562C356C], EAX　←ここのEAXは実際には300固定のもよう
59F43EF4   C705 6C35545A 00>MOV     DWORD PTR DS:[5A54356C], 600
*/

