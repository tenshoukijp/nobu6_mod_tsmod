#include "TSModCommand.h"
#include "CommonGlobalHandle.h"


/*
 徴兵可能数増加割合の変更。神パッチよりの移植。
 */


void WritePatchIncreaseRatioCanDraftSoldierByte( int iAddress, char Value ) {
	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(iAddress), &Value, 1, NULL);
}
void WritePatchIncreaseRatioCanDraftSoldierInt( int iAddress, int Value ) {
	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(iAddress), &Value, 4, NULL); //int 1つ分の4バイト書き込む
}

#define CMD_CAN_INSTRUCT_DIRECT_OTHER_GUNDAN_ADDRESS_1 0x403236
#define CMD_CAN_INSTRUCT_DIRECT_OTHER_GUNDAN_ADDRESS_2 0x4433AF


void SetIncreaseRatioCanDraftSoldier() {

	/* 指示不可能と指示可能の比較(左が指示不可能)

	00002636: 06 02
	デフォルト
	0040322F   E8 259C0A00      CALL    TENSHOU_.004ACE59
	00403234   C1E8 02          SHR     EAX, 2
	00403237   83C4 08          ADD     ESP, 8

	1/5
	0040322F   E8 259C0A00      CALL    TENSHOU.004ACE59
	00403234   C1E8 06          SHR     EAX, 6
	00403237   83C4 08          ADD     ESP, 8


	000427AF: E8 C8
	000427B0: 03 00
	デフォルト
	004433AC   8BF0             MOV     ESI, EAX
	004433AE   B9 C8000000      MOV     ECX, 0C8
	004433B3   2BD2             SUB     EDX, EDX

	1/5
	004433AC   8BF0             MOV     ESI, EAX
	004433AE   B9 E8030000      MOV     ECX, 3E8
	004433B3   2BD2             SUB     EDX, EDX
	*/

	// 変更していれば…
	if ( TSModCommand::AllCastle::cmdIncreaseRatioCanDraftSoldier ) {

		if ( TSModCommand::AllCastle::cmdIncreaseRatioCanDraftSoldier <= 2 ) {
			WritePatchIncreaseRatioCanDraftSoldierByte( CMD_CAN_INSTRUCT_DIRECT_OTHER_GUNDAN_ADDRESS_1, (char)0x6);
		} else {
			WritePatchIncreaseRatioCanDraftSoldierByte( CMD_CAN_INSTRUCT_DIRECT_OTHER_GUNDAN_ADDRESS_1, (char)0x2);
		}

		float iCoef = 10.0f / (float)TSModCommand::AllCastle::cmdIncreaseRatioCanDraftSoldier; // 
		int iSoldierIncreaseValue = int(0xC8 * iCoef); // 0xC8にかければ、その数分の一となる。
		WritePatchIncreaseRatioCanDraftSoldierInt( CMD_CAN_INSTRUCT_DIRECT_OTHER_GUNDAN_ADDRESS_2, iSoldierIncreaseValue);

	// 変更していなければ(=0ならば)
	} else {
		WritePatchIncreaseRatioCanDraftSoldierByte( CMD_CAN_INSTRUCT_DIRECT_OTHER_GUNDAN_ADDRESS_1, (char)0x2);

		WritePatchIncreaseRatioCanDraftSoldierInt( CMD_CAN_INSTRUCT_DIRECT_OTHER_GUNDAN_ADDRESS_2, (int)0xC8);
	}
}