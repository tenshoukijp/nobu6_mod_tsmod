#include "TSModCommand.h"
#include "CommonGlobalHandle.h"

/*
 死亡頻度にかかわらず毎季死亡処理を「する」「しない」。神パッチよりの移植。
 */


void WritePatchWorkToDieEverySeason( int iAddress, char Value ) {
	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(iAddress), &Value, 1, NULL); //1バイトのみ書き込む
}

#define CMD_WORK_TO_DIE_EVERY_SEASON_ADDRESS_1 0x441C2D



void SetWorkToDieEverySeason() {

	/*

	死亡頻度にかかわらず毎季死亡処理を「する」「しない」

	0004102D: EB 75

	死亡処理する
	00441C2D   . EB 12          JMP     SHORT TENSHOU.00441C41

	死亡処理しない
	00441C2D   . 75 12          JNZ     SHORT TENSHOU.00441C41

	*/

	// 毎季死亡処理をする…
	if ( TSModCommand::AllBushou::cmdSetWorkToDieEverySeason ) {

		WritePatchWorkToDieEverySeason( CMD_WORK_TO_DIE_EVERY_SEASON_ADDRESS_1, (char)0xEB);

	// デフォルト
	} else {
		WritePatchWorkToDieEverySeason( CMD_WORK_TO_DIE_EVERY_SEASON_ADDRESS_1, (char)0x75);
	}
}