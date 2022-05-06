#include "TSModCommand.h"
#include "CommonGlobalHandle.h"

/*
 死亡頻度の寿命への影響をカットするかどうか。神パッチよりの移植。
 */


void WritePatchDeathFrequencyNotInfluenceLife( int iAddress, char Value ) {
	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(iAddress), &Value, 1, NULL); //1バイトのみ書き込む
}


#define CMD_DEATH_FREQUENCY_NOT_INFLUENCE_LIFE_ADDRESS 0x441C0B

void SetDeathFrequencyNotInfluenceLife() {

	// 0004100B: 02 01
	// 死亡頻度の寿命への影響。影響無しなら「２」。有りなら「１」。デフォルトは有り。

	/*
	// 影響力を持つ状態(=デフォルト)
	00441C06   83C4 04          ADD     ESP, 4
	00441C09   83F8 01          CMP     EAX, 1
	00441C0C   1BDB             SBB     EBX, EBX

	// 影響力が無い態(=比較値を２に変更して、かわしてる)
	00441C06   . 83C4 04        ADD     ESP, 4
	00441C09   . 83F8 02        CMP     EAX, 2
	00441C0C   . 1BDB           SBB     EBX, EBX
	*/

	// 影響をカットする、というフラグが立っているならば…
	if ( TSModCommand::AllBushou::cmdDeathFrequencyNotInfluenceLife == 1 ) {
		WritePatchDeathFrequencyNotInfluenceLife(CMD_DEATH_FREQUENCY_NOT_INFLUENCE_LIFE_ADDRESS, 2 );
	} else {
		WritePatchDeathFrequencyNotInfluenceLife(CMD_DEATH_FREQUENCY_NOT_INFLUENCE_LIFE_ADDRESS, 1 );
	}
}