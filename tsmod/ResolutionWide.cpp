
#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "TSModCommand.h"
#include "TSMod.h"
#include "KaoSwapExtend.h"

#include <sys/types.h>
#include <sys/stat.h>

/*
��𑜓x�ɕύX����B������1600���ɂ���
0049918E     BF 00040000    MOV     EDI, 400 �� 640(1600)��780(1920)�ւ̏�������
*/

// DDraw�I���W�i���n���h��
extern HINSTANCE hDDrawMod;

#define DDRAW_DLL_ENTRYPOINT_TO_DIRECT_DRAW_CREATE						 0x41B0					// DDraw.DLL�̐擪�A�h���X����ADirectDrawCreate�܂ł̃A�h���X�̍���
#define DDRAW_DLL_RESOLUTION1_ADDRESS_FROM_DIRECT_DRAW_CREATE			-0x29b8					// DDraw.DLL��DirectDrawCreate���\�b�h�A�h���X����𑜓x�̑傫���̒l�������Ă�A�h���X�P
#define DDRAW_DLL_RESOLUTION2_ADDRESS_FROM_DIRECT_DRAW_CREATE			-0xdcc					// DDraw.DLL��DirectDrawCreate���\�b�h�A�h���X����𑜓x�̑傫���̒l�������Ă�A�h���X�Q
#define DDRAW_DLL_RESOLUTION3_ADDRESS_FROM_DIRECT_DRAW_CREATE			-0x2b6					// DDraw.DLL��DirectDrawCreate���\�b�h�A�h���X����𑜓x�̑傫���̒l�������Ă�A�h���X�R
#define DDRAW_DLL_RESOLUTION4_ADDRESS_FROM_DIRECT_DRAW_CREATE			-0xa07					// DDraw.DLL��DirectDrawCreate���\�b�h�A�h���X����𑜓x�̑傫���̒l�������Ă�A�h���X�S
#define DDRAW_DLL_RESOLUTION5_ADDRESS_FROM_DIRECT_DRAW_CREATE			-0x92b					// DDraw.DLL��DirectDrawCreate���\�b�h�A�h���X����𑜓x�̑傫���̒l�������Ă�A�h���X�T

/* �ȉ���400��600��
5DEF41B0 > 833D 8C39505E 00       CMP     DWORD PTR DS:[5E50398C], 0		DirectDraw

5DEF17F7   68 00040000                                          PUSH    400

5DEF33E3   68 00040000                                          PUSH    400

5DEF3EF4   C705 6C354F5E 00040000                               MOV     DWORD PTR DS:[5E4F356C], 400

5DEF37A7   81C3 00040000										ADD     EBX, 400

5DEF3881   814424 10 00040000									ADD     DWORD PTR SS:[ESP+10], 400
*/

int iConstWideResolutionSize = 0x600; // 1536, 2720   (MAX��0xAA0�܂ŉ\�B��ɑS�}�b�v�\�������)



void setWideResolution() {
	if ( TSModCommand::Environment::cmdWideResolution ) {

		// TENSHOU.EXE�̉𑜓x�֘A�̏�������
		int iWidthOfSettings = iConstWideResolutionSize;
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x49918F), &iWidthOfSettings, 4, NULL);

		char szWidthtOf1600[5];
		wsprintf(szWidthtOf1600, "%d", iWidthOfSettings);
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x4D32CE), szWidthtOf1600, 4, NULL);

		// DirectDraw�̉𑜓x�֘A�̏�������
		struct stat  st;
		int o_DirectDrawCreate = (int)hDDrawMod + DDRAW_DLL_ENTRYPOINT_TO_DIRECT_DRAW_CREATE; // DDraw.dll�̐擪�A�h���X����A0x41B0�������񂾏ꏊ���A

		// DDraw.DLL������ꍇ
		if (stat(DDRAW_DLL_FILENAME, &st) == 0) {

			// DDraw.DLL�̉𑜓x��ύX����
			int DDrawDllResulution1Address = (int)o_DirectDrawCreate + DDRAW_DLL_RESOLUTION1_ADDRESS_FROM_DIRECT_DRAW_CREATE;

			WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(DDrawDllResulution1Address), &iWidthOfSettings, 2, NULL);

			// DDraw.DLL�̉𑜓x��ύX����
			int DDrawDllResulution2Address = (int)o_DirectDrawCreate + DDRAW_DLL_RESOLUTION2_ADDRESS_FROM_DIRECT_DRAW_CREATE;

			WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(DDrawDllResulution2Address), &iWidthOfSettings, 2, NULL);

			// DDraw.DLL�̉𑜓x��ύX����
			int DDrawDllResulution4Address = (int)o_DirectDrawCreate + DDRAW_DLL_RESOLUTION4_ADDRESS_FROM_DIRECT_DRAW_CREATE;

			WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(DDrawDllResulution4Address), &iWidthOfSettings, 2, NULL);


			// DDraw.DLL�̉𑜓x��ύX����
			int DDrawDllResulution5Address = (int)o_DirectDrawCreate + DDRAW_DLL_RESOLUTION5_ADDRESS_FROM_DIRECT_DRAW_CREATE;

			WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(DDrawDllResulution5Address), &iWidthOfSettings, 2, NULL);

			// ���������ƁA��O�����t���J���[�łȂ��Ȃ�B
			// DDraw.DLL�̉𑜓x��ύX����
			int DDrawDllResulution3Address = (int)o_DirectDrawCreate + DDRAW_DLL_RESOLUTION3_ADDRESS_FROM_DIRECT_DRAW_CREATE;

			WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(DDrawDllResulution3Address), &iWidthOfSettings, 2, NULL);

		}
	}
}

/*
DDraw.sys�̉��S��==****3EFA�̃A�h���X�̐��l�����������邱�Ƃ��^���̕����ꓹ�ƂȂ�B

***4016   890D 6C352C56    MOV     DWORD PTR DS:[562C356C], ECX  ���ۂ̃N���C�A���g�̃T�C�Y
55CC1344   A3 6C352C56      MOV     DWORD PTR DS:[562C356C], EAX�@��������EAX�͎��ۂɂ�300�Œ�̂��悤
59F43EF4   C705 6C35545A 00>MOV     DWORD PTR DS:[5A54356C], 600
*/

