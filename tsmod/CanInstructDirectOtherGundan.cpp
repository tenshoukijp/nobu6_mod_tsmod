#include "TSModCommand.h"
#include "CommonGlobalHandle.h"

/*
 第２軍団以下に、コマンドの指示が可能。神パッチよりの移植。
 */


void WritePatchCanInstructDirectOtherGundan( int iAddress, char Value ) {
	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(iAddress), &Value, 1, NULL); //1バイトのみ書き込む
}

#define CMD_CAN_INSTRUCT_DIRECT_OTHER_GUNDAN_ADDRESS_1 0x476880
#define CMD_CAN_INSTRUCT_DIRECT_OTHER_GUNDAN_ADDRESS_2 0x476898



void SetCanInstructDirectOtherGundan() {

	/* 指示不可能と指示可能の比較(左が指示不可能)

	指示可能
	00476880  |. EB 02          JMP     SHORT TENSHOU.00476884
	00476898  |. EB 02          JMP     SHORT TENSHOU.0047689C

	指示不可能
	00476880  |. 75 02          JNZ     SHORT TENSHOU.00476884
	00476898  |. 75 02          JNZ     SHORT TENSHOU.0047689C
	*/

	// 指示可能であれば…
	if ( TSModCommand::MyselfDaimyo::cmdCanInstructDirectOtherGundan ) {
		WritePatchCanInstructDirectOtherGundan( CMD_CAN_INSTRUCT_DIRECT_OTHER_GUNDAN_ADDRESS_1, (char)0xEB);
		WritePatchCanInstructDirectOtherGundan( CMD_CAN_INSTRUCT_DIRECT_OTHER_GUNDAN_ADDRESS_2, (char)0xEB);

	// 不可能であれば…
	} else {
		WritePatchCanInstructDirectOtherGundan( CMD_CAN_INSTRUCT_DIRECT_OTHER_GUNDAN_ADDRESS_1, (char)0x75);
		WritePatchCanInstructDirectOtherGundan( CMD_CAN_INSTRUCT_DIRECT_OTHER_GUNDAN_ADDRESS_2, (char)0x75);
	}
}