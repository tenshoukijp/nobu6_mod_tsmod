#include "TSModCommand.h"
#include "CommonGlobalHandle.h"

/*
 ����������ɁA�Ɛb�̒����x���ቺ����B�_�p�b�`���̈ڐA�B
 */


void WritePatchDegreeLoyaltyOnCaptiveLibra( int iAddress, char Value ) {
	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(iAddress), &Value, 1, NULL); //1�o�C�g�̂ݏ�������
}
void WritePatchDegreeLoyaltyOnCaptiveLibra5Byte( int iAddress, char *pValue ) {
	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(iAddress),pValue, 5, NULL); //int 1����5�o�C�g��������
}

#define CMD_DEGREE_LOYALTY_ON_CAPTIVE_LIBRA_ADDRESS_1 0x42997D
#define CMD_DEGREE_LOYALTY_ON_CAPTIVE_LIBRA_ADDRESS_2 0x429BCA
#define CMD_DEGREE_LOYALTY_ON_CAPTIVE_LIBRA_ADDRESS_3 0x429C33
#define CMD_DEGREE_LOYALTY_ON_CAPTIVE_LIBRA_ADDRESS_4 0x4B2901



void SetDegreeLoyaltyOnCaptiveLibra() {

	/*  ����������ɁA�Ɛb�̒����x���ቺ����B�_�p�b�`���̈ڐA�B
	����[����]�B�E��[���Ȃ�]�B�f�t�H���g��[���Ȃ�]

	00028D7D: E9 89
	00028D7E: 64 5D
	00028D7F: 8F F0
	00028D80: 08 EB
	00028D81: 00 38

	[����]
	00429976   . C745 E4 FFFF00>MOV     DWORD PTR SS:[EBP-1C], 0FFFF
	0042997D   . E9 648F0800    JMP     TENSHOU.004B28E6
	00429982   > C745 E8 DF0B00>MOV     DWORD PTR SS:[EBP-18], 0BDF

	[���Ȃ�]
	00429976  |. C745 E4 FFFF00>MOV     DWORD PTR SS:[EBP-1C], 0FFFF
	0042997D  |. 895D F0        MOV     DWORD PTR SS:[EBP-10], EBX
	00429980  |. EB 38          JMP     SHORT TENSHOU_.004299BA
	00429982  |> C745 E8 DF0B00>MOV     DWORD PTR SS:[EBP-18], 0BDF



	00028FCA: 66 6C

	[����]
	00429BC9  |. EB 66          JMP     SHORT TENSHOU.00429C31

	[���Ȃ�]
	00429BC9  |. EB 6C          JMP     SHORT TENSHOU_.00429C37



	00029033: 00 01
	[����]
	00429C31  |> 83F8 00        CMP     EAX, 0

	[���Ȃ�]
	00429C31  |. 83F8 01        CMP     EAX, 1



	000B1D01: 50 53
	[����]
	004B2901   . 50             PUSH    EAX

	[���Ȃ�]
	004B2901   . 53             PUSH    EBX
	*/

	// �w���\�ł���΁c
	if ( TSModCommand::AllBushou::cmdDegreeLoyaltyOnCaptiveLibra ) {
		char patch[] = "\xE9\x64\x8F\x08\x00";
		WritePatchDegreeLoyaltyOnCaptiveLibra5Byte( CMD_DEGREE_LOYALTY_ON_CAPTIVE_LIBRA_ADDRESS_1, patch);
		WritePatchDegreeLoyaltyOnCaptiveLibra( CMD_DEGREE_LOYALTY_ON_CAPTIVE_LIBRA_ADDRESS_2, (char)0x66);
		WritePatchDegreeLoyaltyOnCaptiveLibra( CMD_DEGREE_LOYALTY_ON_CAPTIVE_LIBRA_ADDRESS_3, (char)0x00);
		WritePatchDegreeLoyaltyOnCaptiveLibra( CMD_DEGREE_LOYALTY_ON_CAPTIVE_LIBRA_ADDRESS_4, (char)0x50);

	// �s�\�ł���΁c
	} else {
		char patch[] = "\x89\x5D\xF0\xEB\x38";
		WritePatchDegreeLoyaltyOnCaptiveLibra5Byte( CMD_DEGREE_LOYALTY_ON_CAPTIVE_LIBRA_ADDRESS_1, patch);
		WritePatchDegreeLoyaltyOnCaptiveLibra( CMD_DEGREE_LOYALTY_ON_CAPTIVE_LIBRA_ADDRESS_2, (char)0x6C);
		WritePatchDegreeLoyaltyOnCaptiveLibra( CMD_DEGREE_LOYALTY_ON_CAPTIVE_LIBRA_ADDRESS_3, (char)0x01);
		WritePatchDegreeLoyaltyOnCaptiveLibra( CMD_DEGREE_LOYALTY_ON_CAPTIVE_LIBRA_ADDRESS_4, (char)0x53);
	}
}