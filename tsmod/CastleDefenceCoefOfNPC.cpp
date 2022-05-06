#include "TSModCommand.h"
#include "CommonGlobalHandle.h"

/*
 ＣＰＵ戦での城防御力係数。神パッチよりの移植。
 */


void WritePatchCastleDefenceCoefOfNPC( int iAddress, char Value ) {
	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(iAddress), &Value, 1, NULL); //1バイトのみ書き込む
}

#define CMD_CASTLE_DEFENCE_COEF_OF_NPC_1 0x432FC3



void SetCastleDefenceCoefOfNPC() {

	/* 
	ＣＰＵ戦での城防御力。普通(3), 強い(4)

	000323C3: 03 04

	普通(3)
	00432FC2  |. B9 03000000    MOV     ECX, 3

	強い(4)
	00432FC2  |. B9 04000000    MOV     ECX, 4

	*/

	// ＣＰＵ戦での城防御力係数を変更していれば…
	if ( TSModCommand::AllCastle::cmdCastleDefenceCoefOfNPC ) {

		char iCoef = (char)TSModCommand::AllCastle::cmdCastleDefenceCoefOfNPC;
		if ( iCoef <= 0 ) { iCoef = 1; }
		if ( iCoef >= 7 ) { iCoef = 7; }

		WritePatchCastleDefenceCoefOfNPC( CMD_CASTLE_DEFENCE_COEF_OF_NPC_1, (char)iCoef);

	// デフォルト(=3)
	} else {
		WritePatchCastleDefenceCoefOfNPC( CMD_CASTLE_DEFENCE_COEF_OF_NPC_1, (char)0x3);
	}
}