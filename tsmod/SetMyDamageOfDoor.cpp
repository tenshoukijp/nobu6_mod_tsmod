#include "TSModCommand.h"
#include "CommonGlobalHandle.h"

#include "SetMyDamageOfDoor.h"

/*
  門壊した際に、自分がダメージを被るかどうか。
  神パッチよりの移植。
*/

void WritePatchMyDamageOfDoor( int iAddress, char *szbuf ) {
	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(iAddress), szbuf, 5, NULL); //5バイトのみ書き込む
}


void SetPatchMyDamageOfDoor() {

	/*
	fc /b TENSHOU.org.exe tenshou.exe
	//------------------------------
	00032328: E8 E9
	00032329: 36 9F
	0003232A: 38 D1
	0003232B: 06 07

	★ダメージなし
	00432F25  |. 55             PUSH    EBP
	00432F26  |. 57             PUSH    EDI
	00432F27  |. 56             PUSH    ESI
	■00432F28  |. E8 36380600    CALL    TENSHOU_.00496763
	00432F2D  |> 66:8B6C24 1C   MOV     BP, WORD PTR SS:[ESP+1C]
	00432F32  |. 83C4 08        ADD     ESP, 8
	00432F35  |. 8BD8           MOV     EBX, EAX
	00432F37  |. A1 485D4D00    MOV     EAX, DWORD PTR DS:[4D5D48]


	★ダメージあり
	00432F25   . 55             PUSH    EBP
	00432F26   . 57             PUSH    EDI
	00432F27   . 56             PUSH    ESI
	■00432F28   . E9 9FD10700    JMP     TENSHOU.004B00CC
	00432F2D   > 66:8B6C24 1C   MOV     BP, WORD PTR SS:[ESP+1C]
	00432F32   . 83C4 08        ADD     ESP, 8
	00432F35   . 8BD8           MOV     EBX, EAX

	//------------------------------
	000323E4: E8 E9
	000323E5: 0E 26
	000323E6: 86 F9
	000323E7: 01 07

	000323F5: B9 E9
	000323F6: 05 A4
	000323F7: 00 A4
	000323F8: 00 05

	★ダメージなし
	00432FE2  |. 56             PUSH    ESI
	00432FE3  |. 55             PUSH    EBP
	■00432FE4  |. E8 0E860100    CALL    TENSHOU_.0044B5F7
	00432FE9  |> 83C4 08        ADD     ESP, 8
	00432FEC  |. 55             PUSH    EBP
	00432FED  |. E8 D9E8FFFF    CALL    TENSHOU_.004318CB
	00432FF2  |. 83C4 04        ADD     ESP, 4
	■00432FF5  |. B9 05000000    MOV     ECX, 5
	00432FFA  |. 2BD2           SUB     EDX, EDX
	00432FFC  |. BD 03000000    MOV     EBP, 3

	★ダメージあり
	00432FE2   . 56             PUSH    ESI
	00432FE3   . 55             PUSH    EBP
	■00432FE4   . E9 26F90700    JMP     TENSHOU.004B290F
	00432FE9   > 83C4 08        ADD     ESP, 8
	00432FEC   . 55             PUSH    EBP
	00432FED   . E8 D9E8FFFF    CALL    TENSHOU.004318CB
	00432FF2   . 83C4 04        ADD     ESP, 4
	■00432FF5   . E9 A4A40500    JMP     TENSHOU.0048D49E
	00432FFA   . 2BD2           SUB     EDX, EDX
	00432FFC   . BD 03000000    MOV     EBP, 3




	//------------------------------

	00032412: 83 03
	00032413: C4 C5
	00032414: 04 83
	00032415: 03 C4
	00032416: C5 04

	★ダメージなし
	0043300C   50               PUSH    EAX
	0043300D   E8 D99E0700      CALL    TENSHOU_.004ACEEB
	■00433012   83C4 04          ADD     ESP, 4
	■00433015   03C5             ADD     EAX, EBP
	00433017   50               PUSH    EAX
	00433018   53               PUSH    EBX

	★ダメージあり
	0043300C   . 50             PUSH    EAX
	0043300D   . E8 D99E0700    CALL    TENSHOU.004ACEEB
	■00433012   . 03C5           ADD     EAX, EBP
	■00433014   . 83C4 04        ADD     ESP, 4
	00433017   . 50             PUSH    EAX
	00433018   . 53             PUSH    EBX


	//------------------------------

	0003247E: 17 28
	0003247F: FF D0
	00032480: FF 07
	00032481: FF 00

	★ダメージなし
	0043307B   50               PUSH    EAX
	0043307C   56               PUSH    ESI
	■0043307D   E8 17FFFFFF      CALL    TENSHOU_.00432F99
	00433082   83C4 08          ADD     ESP, 8
	00433085   8BF8             MOV     EDI, EAX

	★ダメージあり
	0043307B   . 50             PUSH    EAX
	0043307C   . 56             PUSH    ESI
	■0043307D   . E8 28D00700    CALL    TENSHOU.004B00AA
	00433082   . 83C4 08        ADD     ESP, 8
	00433085   . 8BF8           MOV     EDI, EAX

	//------------------------------

	000324AB: E8 E9
	000324AC: 85 74
	000324AD: 77 F8
	000324AE: 06 07

	★ダメージなし
	004330A5   8955 FC          MOV     DWORD PTR SS:[EBP-4], EDX
	004330A8   FF75 F8          PUSH    DWORD PTR SS:[EBP-8]
	■004330AB   E8 85770600      CALL    TENSHOU_.0049A835
	004330B0   83C4 10          ADD     ESP, 10
	004330B3   56               PUSH    ESI

	★ダメージあり
	004330A5   . 8955 FC        MOV     DWORD PTR SS:[EBP-4], EDX
	004330A8   . FF75 F8        PUSH    DWORD PTR SS:[EBP-8]
	■004330AB   . E9 74F80700    JMP     TENSHOU.004B2924
	004330B0   > 83C4 10        ADD     ESP, 10
	004330B3   . 56             PUSH    ESI

	//------------------------------

	00034C7B: 1A 1F
	00034C7C: D7 A8
	00034C7D: FF 07
	00034C7E: FF 00

	★ダメージなし
	00435878   57               PUSH    EDI
	00435879   53               PUSH    EBX
	■0043587A   E8 1AD7FFFF      CALL    TENSHOU_.00432F99
	0043587F   83C4 08          ADD     ESP, 8
	00435882   57               PUSH    EDI

	★ダメージあり
	00435878  |. 57             PUSH    EDI
	00435879  |. 53             PUSH    EBX
	■0043587A  |. E8 1FA80700    CALL    TENSHOU.004B009E
	0043587F  |. 83C4 08        ADD     ESP, 8
	00435882  |> 57             PUSH    EDI

	//------------------------------
	0006A051: 83 E9
	0006A052: C4 7A
	0006A053: 0C 7C
	0006A054: EB 04
	0006A055: 4A 00

	ダメージなし
	0046AC4C  |. E8 C982FCFF    CALL    TENSHOU_.00432F1A
	■0046AC51  |. 83C4 0C        ADD     ESP, 0C
	■0046AC54  |. EB 4A          JMP     SHORT TENSHOU_.0046ACA0
	0046AC56  |> 33DB           XOR     EBX, EBX
	0046AC58  |. 53             PUSH    EBX                              ; /Arg8 => 00000000
	0046AC59  |. 53             PUSH    EBX                              ; |Arg7 => 00000000
	0046AC5A  |. 53             PUSH    EBX                              ; |Arg6 => 00000000
	0046AC5B  |. 53             PUSH    EBX                              ; |Arg5 => 00000000
	0046AC5C  |. 53             PUSH    EBX                              ; |Arg4 => 00000000
	0046AC5D  |. 53             PUSH    EBX                              ; |Arg3 => 00000000
	0046AC5E  |. 53             PUSH    EBX                              ; |Arg2 => 00000000
	0046AC5F  |. 68 B60F0000    PUSH    0FB6                             ; |Arg1 = 00000FB6
	0046AC64  |. E8 F7130000    CALL    TENSHOU_.0046C060                ; \TENSHOU_.0046C060

	ダメージあり
	0046AC4C   . E8 C982FCFF    CALL    TENSHOU.00432F1A
	■0046AC51   . E9 7A7C0400    JMP     TENSHOU.004B28D0
	0046AC56   > 33DB           XOR     EBX, EBX
	0046AC58   . 53             PUSH    EBX                              ; /Arg8 => 00000000
	0046AC59   . 53             PUSH    EBX                              ; |Arg7 => 00000000
	0046AC5A   . 53             PUSH    EBX                              ; |Arg6 => 00000000
	0046AC5B   . 53             PUSH    EBX                              ; |Arg5 => 00000000
	0046AC5C   . 53             PUSH    EBX                              ; |Arg4 => 00000000
	0046AC5D   . 53             PUSH    EBX                              ; |Arg3 => 00000000
	0046AC5E   . 53             PUSH    EBX                              ; |Arg2 => 00000000
	0046AC5F   . 68 B60F0000    PUSH    0FB6                             ; |Arg1 = 00000FB6
	0046AC64   . E8 F7130000    CALL    TENSHOU.0046C060                 ; \TENSHOU.0046C060

	//------------------------------
	00087DE0: E8 E9
	00087DE1: 35 46
	00087DE2: A5 77
	00087DE3: FA 02
	00087DE4: FF 00

	ダメージなし
	004889DF   56               PUSH    ESI
	■004889E0   E8 35A5FAFF      CALL    TENSHOU_.00432F1A
	004889E5   EB 0C            JMP     SHORT TENSHOU_.004889F3
	004889E7   FF75 F8          PUSH    DWORD PTR SS:[EBP-8]
	004889EA   FF75 FC          PUSH    DWORD PTR SS:[EBP-4]

	ダメージあり
	004889DF   . 56             PUSH    ESI
	■004889E0   . E9 46770200    JMP     TENSHOU.004B012B
	004889E5   > EB 0C          JMP     SHORT TENSHOU.004889F3
	004889E7   > FF75 F8        PUSH    DWORD PTR SS:[EBP-8]
	004889EA   . FF75 FC        PUSH    DWORD PTR SS:[EBP-4]
	004889ED   . 56             PUSH    ESI
	*/

	int CMD_PATCH_MYDAMAGEOFDOOR_ADDRESS_01 = 0x432F28;
	int CMD_PATCH_MYDAMAGEOFDOOR_ADDRESS_02 = 0x432FE4;
	int CMD_PATCH_MYDAMAGEOFDOOR_ADDRESS_03 = 0x432FF5;
	int CMD_PATCH_MYDAMAGEOFDOOR_ADDRESS_04 = 0x433012;
	int CMD_PATCH_MYDAMAGEOFDOOR_ADDRESS_05 = 0x43307D;
	int CMD_PATCH_MYDAMAGEOFDOOR_ADDRESS_06 = 0x4330AB;
	int CMD_PATCH_MYDAMAGEOFDOOR_ADDRESS_07 = 0x43587A;
	int CMD_PATCH_MYDAMAGEOFDOOR_ADDRESS_08 = 0x46AC51;
	int CMD_PATCH_MYDAMAGEOFDOOR_ADDRESS_09 = 0x4889E0;


	// ダメージが有の場合
	if ( TSModCommand::AllBushou::cmdReceiveDamageOfBreakDoor ) {

		char szdmg1[] = "\xE9\x9F\xD1\x07\x00";
		WritePatchMyDamageOfDoor( CMD_PATCH_MYDAMAGEOFDOOR_ADDRESS_01, szdmg1 );

		char szdmg2[] = "\xE9\x26\xF9\x07\x00";
		WritePatchMyDamageOfDoor( CMD_PATCH_MYDAMAGEOFDOOR_ADDRESS_02, szdmg2 );

		char szdmg3[] = "\xE9\xA4\xA4\x05\x00";
		WritePatchMyDamageOfDoor( CMD_PATCH_MYDAMAGEOFDOOR_ADDRESS_03, szdmg3 );

		char szdmg4[] = "\x03\xC5\x83\xC4\x04";
		WritePatchMyDamageOfDoor( CMD_PATCH_MYDAMAGEOFDOOR_ADDRESS_04, szdmg4 );

		char szdmg5[] = "\xE8\x28\xD0\x07\x00";
		WritePatchMyDamageOfDoor( CMD_PATCH_MYDAMAGEOFDOOR_ADDRESS_05, szdmg5 );

		char szdmg6[] = "\xE9\x74\xF8\x07\x00";
		WritePatchMyDamageOfDoor( CMD_PATCH_MYDAMAGEOFDOOR_ADDRESS_06, szdmg6 );

		char szdmg7[] = "\xE8\x1F\xA8\x07\x00";
		WritePatchMyDamageOfDoor( CMD_PATCH_MYDAMAGEOFDOOR_ADDRESS_07, szdmg7 );

		char szdmg8[] = "\xE9\x7A\x7C\x04\x00";
		WritePatchMyDamageOfDoor( CMD_PATCH_MYDAMAGEOFDOOR_ADDRESS_08, szdmg8 );

		char szdmg9[] = "\xE9\x46\x77\x02\x00";
		WritePatchMyDamageOfDoor( CMD_PATCH_MYDAMAGEOFDOOR_ADDRESS_09, szdmg9 );


	// ダメージが無しの場合
	} else {
		char szdmg1[] = "\xE8\x36\x38\x06\x00";
		WritePatchMyDamageOfDoor( CMD_PATCH_MYDAMAGEOFDOOR_ADDRESS_01, szdmg1 );

		char szdmg2[] = "\xE8\x0E\x86\x01\x00";
		WritePatchMyDamageOfDoor( CMD_PATCH_MYDAMAGEOFDOOR_ADDRESS_02, szdmg2 );

		char szdmg3[] = "\xB9\x05\x00\x00\x00";
		WritePatchMyDamageOfDoor( CMD_PATCH_MYDAMAGEOFDOOR_ADDRESS_03, szdmg3 );

		char szdmg4[] = "\x83\xC4\x04\x03\xC5";
		WritePatchMyDamageOfDoor( CMD_PATCH_MYDAMAGEOFDOOR_ADDRESS_04, szdmg4 );

		char szdmg5[] = "\xE8\x17\xFF\xFF\xFF";
		WritePatchMyDamageOfDoor( CMD_PATCH_MYDAMAGEOFDOOR_ADDRESS_05, szdmg5 );

		char szdmg6[] = "\xE8\x85\x77\x06\x00";
		WritePatchMyDamageOfDoor( CMD_PATCH_MYDAMAGEOFDOOR_ADDRESS_06, szdmg6 );

		char szdmg7[] = "\xE8\x1A\xD7\xFF\xFF";
		WritePatchMyDamageOfDoor( CMD_PATCH_MYDAMAGEOFDOOR_ADDRESS_07, szdmg7 );

		char szdmg8[] = "\x83\xC4\x0C\xEB\x4A";
		WritePatchMyDamageOfDoor( CMD_PATCH_MYDAMAGEOFDOOR_ADDRESS_08, szdmg8 );

		char szdmg9[] = "\xE8\x35\xA5\xFA\xFF";
		WritePatchMyDamageOfDoor( CMD_PATCH_MYDAMAGEOFDOOR_ADDRESS_09, szdmg9 );
	}
}
