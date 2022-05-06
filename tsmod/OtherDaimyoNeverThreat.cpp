#include "TSModCommand.h"
#include "CommonGlobalHandle.h"


/*
 ＣＰＵによる脅迫を無効とするか否か。デフォルトは、脅迫を無効とはしない(脅迫は有効)。
 神パッチよりの移植。
 */


void WritePatchOtherDaimyoNeverThreat( int iAddress, char Value ) {
	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(iAddress), &Value, 1, NULL);
}

#define CMD_OTHER_DAIMYO_NEVER_THREAT_ADDRESS_1 0x4A74FC
#define CMD_OTHER_DAIMYO_NEVER_THREAT_ADDRESS_2 0x4A750C
#define CMD_OTHER_DAIMYO_NEVER_THREAT_ADDRESS_3 0x4A7528


void SetOtherDaimyoNeverThreat() {

	/* ＣＰＵによる脅迫。左がなし、右があり。

	000A68FC: 00 2D
	000A690C: 00 1D
	000A6928: EB 77

	脅迫なし
	004A74FB   74 00            JE      SHORT TENSHOU.004A74FD
	004A750B   75 00            JNZ     SHORT TENSHOU.004A750D
	004A7528   EB 0E            JMP     SHORT TENSHOU.004A7538

	脅迫あり
	004A74FB   74 2D            |JE      SHORT TENSHOU_.004A752A
	004A750B   75 1D            |JNZ     SHORT TENSHOU_.004A752A
	004A7528   77 0E            |JA      SHORT TENSHOU_.004A7538
	*/

	// 脅迫無しをＯＮにしていれば…
	if ( TSModCommand::OtherDaimyo::cmdNeverThreat ) {
		WritePatchOtherDaimyoNeverThreat( CMD_OTHER_DAIMYO_NEVER_THREAT_ADDRESS_1, (char)0x00);
		WritePatchOtherDaimyoNeverThreat( CMD_OTHER_DAIMYO_NEVER_THREAT_ADDRESS_2, (char)0x00);
		WritePatchOtherDaimyoNeverThreat( CMD_OTHER_DAIMYO_NEVER_THREAT_ADDRESS_3, (char)0xEB);

	// デフォルトならば
	} else {
		WritePatchOtherDaimyoNeverThreat( CMD_OTHER_DAIMYO_NEVER_THREAT_ADDRESS_1, (char)0x2D);
		WritePatchOtherDaimyoNeverThreat( CMD_OTHER_DAIMYO_NEVER_THREAT_ADDRESS_2, (char)0x1D);
		WritePatchOtherDaimyoNeverThreat( CMD_OTHER_DAIMYO_NEVER_THREAT_ADDRESS_3, (char)0x77);
	}
}