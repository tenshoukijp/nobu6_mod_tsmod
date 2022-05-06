#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"

/*
 * マウスに相当するものが接続されているかどうか。
 * ノートパソコンなどのパッドなどもマウスとみなされる。
 * 将来の完全タブレットに備えて、完全にマウスが無くとも動作するようにする。
 */


int pTenshouExeJumpFromToOnTenshouExeResultOfMouseDriverInstalledInPC = 0x417533; 


/*
00417529  |. 6A 2B          PUSH    2B                               ; /Index = SM_CMOUSEBUTTONS
0041752B  |. FF15 60764E00  CALL    DWORD PTR DS:[<&USER32.GetSystem>; \GetSystemMetrics
00417531  |. 85C0           TEST    EAX, EAX
00417533  |. 75 07          JNZ     SHORT TENSHOU.0041753C
				└ここを JMP SHORT TENSHOU.0041753C と「JNZ」→「JMP」と書き換える。★ 書き換えが間に合わないのでtenshou.exe 自体を書き換えた!!
00417535  |. BE 90C44B00    MOV     ESI, TENSHOU.004BC490            ;  ASCII "マウスがインストールされていません。"
0041753A  |. EB 6B          JMP     SHORT TENSHOU.004175A7

 */

// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeResultOfMouseDriverInstalledInPC() {

	char sz[] = "\xEB"; // jmpというアセンブラ

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeResultOfMouseDriverInstalledInPC), sz, 1, NULL); //1バイトのみ書き込む
}

