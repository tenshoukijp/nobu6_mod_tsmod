#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"


/*
0047D24A   . 56             PUSH    ESI
0047D24B   . 57             PUSH    EDI
0047D24C   . 8B39           MOV     EDI, DWORD PTR DS:[ECX]
0047D24E   . 8BF1           MOV     ESI, ECX
0047D250   . FF57 30        CALL    DWORD PTR DS:[EDI+30]		← 関数ポインタジャンプ
0047D253   . 8BCE           MOV     ECX, ESI
0047D255   . FF57 34        CALL    DWORD PTR DS:[EDI+34]		← 関数ポインタジャンプ
0047D258   . 5F             POP     EDI
0047D259   . 5E             POP     ESI
0047D25A   . C3             RETN
*/

extern int iOffsetOfUIPaintOfMainScreenBackGround; // 上部UIの桜とかの右へのOffset値
extern int iConstWideResolutionSize;

int IsOnUIPaintOfMainScreeenBackGround = FALSE;

int iEDIOfTenshouExeFunctionPointerTableJump = 0; // EDIの値。この値が「EDI==0x4B5608」の時、メイン画面の上部のＵＩを表示する時の番号

void OnTenshouExeFunctionPointerTableJumpExecute() {

	IsOnUIPaintOfMainScreeenBackGround = FALSE;

	// まずメイン画面のUIの上部の関数ポインタへと飛んできていること
	// 横幅が1600であること。
	if ( iEDIOfTenshouExeFunctionPointerTableJump == 0x4B5608 && GetResolutionWidth()==iConstWideResolutionSize ) {
		IsOnUIPaintOfMainScreeenBackGround = TRUE; // ペイントを繰り返すモード
	}
}


/*
0047D253   . 8BCE           MOV     ECX, ESI
0047D255   . FF57 34        CALL    DWORD PTR DS:[EDI+34]		← 関数ポインタジャンプ
				└ここを JMP TSMod.OnFunctionPointerTableJump と書き換えてやる実際の処理
0047D258   . 5F             POP     EDI
*/

int pTenshouExeJumpFromToOnTenshouExeFunctionPointerTableJump		=0x47D253; // 関数はこのアドレスから、OnTenshouExeFunctionPointerTableJumpへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeFunctionPointerTableJump	=0x47D258; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeFunctionPointerTableJump() {
	// スタックにためておく
	__asm {

		mov iOffsetOfUIPaintOfMainScreenBackGround, 0 // 丈夫UI背景のOffset値

		// 天翔記の元々の処理
		MOV     ECX, ESI
		CALL    DWORD PTR DS:[EDI+0x34]	// 	← 関数ポインタジャンプ

		mov iEDIOfTenshouExeFunctionPointerTableJump, EDI // EDIで関数ポインタへとジャンプするので、この値が全て

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	OnTenshouExeFunctionPointerTableJumpExecute();

	if ( IsOnUIPaintOfMainScreeenBackGround ) {

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

			mov iOffsetOfUIPaintOfMainScreenBackGround, 256;
			// 天翔記の元々の処理
			MOV     ECX, ESI
			CALL    DWORD PTR DS:[EDI+0x34]	// 	← 関数ポインタジャンプ


			mov iOffsetOfUIPaintOfMainScreenBackGround, 512;
			// 天翔記の元々の処理
			MOV     ECX, ESI
			CALL    DWORD PTR DS:[EDI+0x34]	// 	← 関数ポインタジャンプ

			push eax
			push ebx
			push ecx
			push edx
			push esp
			push ebp
			push esi
			push edi
		}
	}

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

		jmp pTenshouExeReturnLblFromOnTenshouExeFunctionPointerTableJump
	}
}


char cmdOnTenshouExeJumpFromFunctionPointerTableJump[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeFunctionPointerTableJump() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeFunctionPointerTableJump;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeFunctionPointerTableJump + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeFunctionPointerTableJump  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromFunctionPointerTableJump+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeFunctionPointerTableJump), cmdOnTenshouExeJumpFromFunctionPointerTableJump, 5, NULL); //5バイトのみ書き込む
}

