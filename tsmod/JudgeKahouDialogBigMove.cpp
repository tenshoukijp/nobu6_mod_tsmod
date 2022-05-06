#include "CommonGlobalHandle.h"
#include "DialogKahouMovie.h"
#include "GameDataStruct.h"

/*
以下の
「JNZ     SHORT TENSHOU.00494AE6」をNOP,NOPですりつぶしておくと、家宝ダイアログが大きく動いた時にも、必ず描画しなおす。

00494ADC   . 833E 04        CMP     DWORD PTR DS:[ESI], 4
00494ADF   . 75 05          JNZ     SHORT TENSHOU.00494AE6
*/

char cmdOnTenshouExeJumpFromJudgeKahouDialogBigMove[3]="\x90\x90";
// 元の命令が5バイト、以後の関数で生まれる命令が合計5バイトなので… 最後１つ使わない

// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeJudgeKahouDialogBigMove() {

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x494ADF), cmdOnTenshouExeJumpFromJudgeKahouDialogBigMove, 2, NULL); //2バイトのみ書き込む
}
