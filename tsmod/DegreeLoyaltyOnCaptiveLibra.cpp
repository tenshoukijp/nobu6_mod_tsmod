#include "TSModCommand.h"
#include "CommonGlobalHandle.h"

/*
 武将解放時に、家臣の忠誠度が低下する。神パッチよりの移植。
 */


void WritePatchDegreeLoyaltyOnCaptiveLibra( int iAddress, char Value ) {
	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(iAddress), &Value, 1, NULL); //1バイトのみ書き込む
}
void WritePatchDegreeLoyaltyOnCaptiveLibra5Byte( int iAddress, char *pValue ) {
	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(iAddress),pValue, 5, NULL); //int 1つ分の5バイト書き込む
}

#define CMD_DEGREE_LOYALTY_ON_CAPTIVE_LIBRA_ADDRESS_1 0x42997D
#define CMD_DEGREE_LOYALTY_ON_CAPTIVE_LIBRA_ADDRESS_2 0x429BCA
#define CMD_DEGREE_LOYALTY_ON_CAPTIVE_LIBRA_ADDRESS_3 0x429C33
#define CMD_DEGREE_LOYALTY_ON_CAPTIVE_LIBRA_ADDRESS_4 0x4B2901



void SetDegreeLoyaltyOnCaptiveLibra() {

	/*  武将解放時に、家臣の忠誠度が低下する。神パッチよりの移植。
	左が[する]。右が[しない]。デフォルトは[しない]

	00028D7D: E9 89
	00028D7E: 64 5D
	00028D7F: 8F F0
	00028D80: 08 EB
	00028D81: 00 38

	[する]
	00429976   . C745 E4 FFFF00>MOV     DWORD PTR SS:[EBP-1C], 0FFFF
	0042997D   . E9 648F0800    JMP     TENSHOU.004B28E6
	00429982   > C745 E8 DF0B00>MOV     DWORD PTR SS:[EBP-18], 0BDF

	[しない]
	00429976  |. C745 E4 FFFF00>MOV     DWORD PTR SS:[EBP-1C], 0FFFF
	0042997D  |. 895D F0        MOV     DWORD PTR SS:[EBP-10], EBX
	00429980  |. EB 38          JMP     SHORT TENSHOU_.004299BA
	00429982  |> C745 E8 DF0B00>MOV     DWORD PTR SS:[EBP-18], 0BDF



	00028FCA: 66 6C

	[する]
	00429BC9  |. EB 66          JMP     SHORT TENSHOU.00429C31

	[しない]
	00429BC9  |. EB 6C          JMP     SHORT TENSHOU_.00429C37



	00029033: 00 01
	[する]
	00429C31  |> 83F8 00        CMP     EAX, 0

	[しない]
	00429C31  |. 83F8 01        CMP     EAX, 1



	000B1D01: 50 53
	[する]
	004B2901   . 50             PUSH    EAX

	[しない]
	004B2901   . 53             PUSH    EBX
	*/

	// 指示可能であれば…
	if ( TSModCommand::AllBushou::cmdDegreeLoyaltyOnCaptiveLibra ) {
		char patch[] = "\xE9\x64\x8F\x08\x00";
		WritePatchDegreeLoyaltyOnCaptiveLibra5Byte( CMD_DEGREE_LOYALTY_ON_CAPTIVE_LIBRA_ADDRESS_1, patch);
		WritePatchDegreeLoyaltyOnCaptiveLibra( CMD_DEGREE_LOYALTY_ON_CAPTIVE_LIBRA_ADDRESS_2, (char)0x66);
		WritePatchDegreeLoyaltyOnCaptiveLibra( CMD_DEGREE_LOYALTY_ON_CAPTIVE_LIBRA_ADDRESS_3, (char)0x00);
		WritePatchDegreeLoyaltyOnCaptiveLibra( CMD_DEGREE_LOYALTY_ON_CAPTIVE_LIBRA_ADDRESS_4, (char)0x50);

	// 不可能であれば…
	} else {
		char patch[] = "\x89\x5D\xF0\xEB\x38";
		WritePatchDegreeLoyaltyOnCaptiveLibra5Byte( CMD_DEGREE_LOYALTY_ON_CAPTIVE_LIBRA_ADDRESS_1, patch);
		WritePatchDegreeLoyaltyOnCaptiveLibra( CMD_DEGREE_LOYALTY_ON_CAPTIVE_LIBRA_ADDRESS_2, (char)0x6C);
		WritePatchDegreeLoyaltyOnCaptiveLibra( CMD_DEGREE_LOYALTY_ON_CAPTIVE_LIBRA_ADDRESS_3, (char)0x01);
		WritePatchDegreeLoyaltyOnCaptiveLibra( CMD_DEGREE_LOYALTY_ON_CAPTIVE_LIBRA_ADDRESS_4, (char)0x53);
	}
}