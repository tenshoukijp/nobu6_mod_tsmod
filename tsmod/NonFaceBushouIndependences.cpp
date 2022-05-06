#include "TSModCommand.h"
#include "CommonGlobalHandle.h"

/*
 顔なし武将が旗揚げするか否か。
 */


void WritePatchNonFaceBushouIndependences( int iAddress, char *pValue ) {
	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(iAddress), pValue, 6, NULL); //1バイトのみ書き込む
}

#define CMD_NON_FACE_BUSHOU_INDEPENDENCES_ADDRESS_1 0x4424C9



void WriteAsmJumperOnNonFaceBushouIndependences() {

	/*
	デフォルト(顔無しは旗揚げしない)
	004424C9  |. 0F84 B7010000  JE      TENSHOU.00442686

	旗揚げする
	004424C9  |. 74 04          JE      SHORT TENSHOU.004424CF
	004424CB  |? 90             NOP
	004424CC  |? 90             NOP
	004424CD  |? 90             NOP
	004424CE  |? 90             NOP
	*/

	// 旗揚げする
	if ( TSModCommand::AllBushou::cmdNonFaceBushouIndependences ) {
		char szCmd1[] = "\x74\x04\x90\x90\x90\x90";
		WritePatchNonFaceBushouIndependences( CMD_NON_FACE_BUSHOU_INDEPENDENCES_ADDRESS_1, szCmd1);

	}
}