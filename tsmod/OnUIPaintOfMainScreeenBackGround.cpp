#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

/*
0046568F     BE 80010000    MOV     ESI, 180
00465694   . 50             PUSH    EAX                              ; /Arg3
00465695   . 33FF           XOR     EDI, EDI                         ; |
00465697   . 6A 30          PUSH    30                               ; |/Arg4 = 00000030
00465699   . 8D4D 9C        LEA     ECX, DWORD PTR SS:[EBP-64]       ; ||
0046569C   . 56             PUSH    ESI                              ; ||Arg3 => 00000180
0046569D   . 57             PUSH    EDI                              ; ||Arg2 => 00000000
0046569E   . 68 80020000    PUSH    280                              ; ||Arg1 = 00000280
004656A3   . E8 52610400    CALL    TENSHOU.004AB7FA                 ; |\TENSHOU.004AB7FA
004656A8   . 50             PUSH    EAX                              ; |Arg2
004656A9   . 8D4D 8C        LEA     ECX, DWORD PTR SS:[EBP-74]       ; |
004656AC   . 51             PUSH    ECX                              ; |Arg1
004656AD   . E8 A5650400    CALL    TENSHOU.004ABC57                 ; \TENSHOU.004ABC57

*/

/*
004231AD  /$ 55             PUSH    EBP

0046568F   . BE 80010000    MOV     ESI, 180

0046568F   . BE 80010000    MOV     ESI, 180
00465694   . 50             PUSH    EAX                              ; /Arg3
00465695   . 33FF           XOR     EDI, EDI                         ; |
00465697   . 6A 30          PUSH    30                               ; |/Arg4 = 00000030

0046575C     68 C0000000    PUSH    0C0


*/

/*
この２つの部分の座標を変更することで、位置を変えることが出来るようだ。
0046569E     68 80030000    PUSH    380
00465736     68 80030000    PUSH    380
*/

/*
004655BB     B8 24584600    MOV     EAX, TENSHOU.00465824		関数のはじめ。ここ以降～RETURNまで仮にまったく処理をしなくてもUIの飾りの背景が描画されないだけである。

00465823   . C3             RETN		ここまでが関数

*/

// メインスクリーンの上部の桜とかを横にずらすOffset
int iOffsetOfUIPaintOfMainScreenBackGround = 0;

void OnTenshouExeUIPaintOfMainScreenBackGroundExecute() {
	if ( iOffsetOfUIPaintOfMainScreenBackGround == 0 ) {
		int iOffset = 0x280;
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x46569F), &iOffset, 2, NULL); //2バイトのみ書き込む
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x465737), &iOffset, 2, NULL); //2バイトのみ書き込む

	} else if ( iOffsetOfUIPaintOfMainScreenBackGround >= 1 ) {
		// メイン画面上部のOFFSET値を設定しなおす。普通は0x280そのまま
		int iOffset1 = 0x280 +iOffsetOfUIPaintOfMainScreenBackGround+20;
		int iOffset2 = 0x280 +iOffsetOfUIPaintOfMainScreenBackGround;
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x46569F), &iOffset1, 2, NULL); //2バイトのみ書き込む
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x465737), &iOffset2, 2, NULL); //2バイトのみ書き込む
	}
}


/*
004655BB     B8 24584600    MOV     EAX, TENSHOU.00465824
				└ここを JMP TSMod.OnUIPaintOfMainScreenBackGround と書き換えてやる実際の処理
004655C0   . E8 B7AE0400    CALL    TENSHOU.004B047C
*/

int pTenshouExeJumpFromToOnTenshouExeUIPaintOfMainScreenBackGround		=0x4655BB; // 関数はこのアドレスから、OnTenshouExeUIPaintOfMainScreenBackGroundへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeUIPaintOfMainScreenBackGround	=0x4655C0; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeUIPaintOfMainScreenBackGround() {
	// スタックにためておく
	__asm {

		// 天翔記の元々の処理
		MOV     EAX, 0x465824

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	OnTenshouExeUIPaintOfMainScreenBackGroundExecute();

	// スタックに引き出す
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		jmp pTenshouExeReturnLblFromOnTenshouExeUIPaintOfMainScreenBackGround
	}
}


char cmdOnTenshouExeJumpFromUIPaintOfMainScreenBackGround[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeUIPaintOfMainScreenBackGround() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeUIPaintOfMainScreenBackGround;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeUIPaintOfMainScreenBackGround + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeUIPaintOfMainScreenBackGround  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromUIPaintOfMainScreenBackGround+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeUIPaintOfMainScreenBackGround), cmdOnTenshouExeJumpFromUIPaintOfMainScreenBackGround, 5, NULL); //5バイトのみ書き込む
}




