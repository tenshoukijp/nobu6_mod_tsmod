#include "TSModCommand.h"
#include "CommonGlobalHandle.h"

/*
 浪人になった時に、勲功値を０としない。
 */


void WritePatchKunkouIsNotResetOnRonin5Byte( int iAddress, char *pValue ) {
	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(iAddress),pValue, 5, NULL); //int 1つ分の5バイト書き込む
}
void WritePatchKunkouIsNotResetOnRonin4Byte( int iAddress, char *pValue ) {
	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(iAddress),pValue, 4, NULL); //int 1つ分の4バイト書き込む
}

#define CMD_KUNKOU_IS_NOT_RESET_ON_RONIN_ADDRESS_1 0x40B423
#define CMD_KUNKOU_IS_NOT_RESET_ON_RONIN_ADDRESS_2 0x40BC4C
#define CMD_KUNKOU_IS_NOT_RESET_ON_RONIN_ADDRESS_3 0x42943A
#define CMD_KUNKOU_IS_NOT_RESET_ON_RONIN_ADDRESS_4 0x442913
#define CMD_KUNKOU_IS_NOT_RESET_ON_RONIN_ADDRESS_5 0x4C8EC0 // 勲功の文字列のアドレス
#define CMD_KUNKOU_IS_NOT_RESET_ON_RONIN_ADDRESS_6 0x4CC154 // 勲功の文字列のアドレス
#define CMD_KUNKOU_IS_NOT_RESET_ON_RONIN_ADDRESS_7 0x4CC160 // 勲功の文字列のアドレス





void InitKunkouIsNotResetOnRonin() {

	/*  浪人になった時に、勲功値を０としない。
		神パッチで何を設定していようとも無効。

		[デフォルト]
		0040B423  |. E8 7DE5FFFF    CALL    TENSHOU.004099A5	← 浪人化時


		0040BC4C  |. E8 54DDFFFF    CALL    TENSHOU.004099A5	← 登用時

		0042943A  |. E8 6605FEFF    CALL    TENSHOU.004099A5	← 戦場で浪人

		00442913  |. E8 8D70FCFF    CALL    TENSHOU.004099A5	← 旗揚げ(大名かも)


		[神パッチ後]
		0040B423  |. 9090909090    NOP x 5

		0040BC4C  |. 9090909090    NOP x 5

		0042943A  |. 9090909090    NOP x 5

		00442913  |. 9090909090    NOP x 5

	*/

	char patch1[] = "\xE8\x7D\xE5\xFF\xFF";
	WritePatchKunkouIsNotResetOnRonin5Byte( CMD_KUNKOU_IS_NOT_RESET_ON_RONIN_ADDRESS_1, patch1);

	char patch2[] = "\xE8\x54\xDD\xFF\xFF";
	WritePatchKunkouIsNotResetOnRonin5Byte( CMD_KUNKOU_IS_NOT_RESET_ON_RONIN_ADDRESS_2, patch2);

	char patch3[] = "\xE8\x66\x05\xFE\xFF";
	WritePatchKunkouIsNotResetOnRonin5Byte( CMD_KUNKOU_IS_NOT_RESET_ON_RONIN_ADDRESS_3, patch3);

	char patch4[] = "\xE8\x8D\x70\xFC\xFF";
	WritePatchKunkouIsNotResetOnRonin5Byte( CMD_KUNKOU_IS_NOT_RESET_ON_RONIN_ADDRESS_4, patch4);

	return;

	// 神パッチで「勲功」→「名声」とされた文字列を、「勲功」に直す。
	char szKunkou[] = "勲功";
	WritePatchKunkouIsNotResetOnRonin4Byte( CMD_KUNKOU_IS_NOT_RESET_ON_RONIN_ADDRESS_5, szKunkou);
	WritePatchKunkouIsNotResetOnRonin4Byte( CMD_KUNKOU_IS_NOT_RESET_ON_RONIN_ADDRESS_6, szKunkou);
	WritePatchKunkouIsNotResetOnRonin4Byte( CMD_KUNKOU_IS_NOT_RESET_ON_RONIN_ADDRESS_7, szKunkou);

}