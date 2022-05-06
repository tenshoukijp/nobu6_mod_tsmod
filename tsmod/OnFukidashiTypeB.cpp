#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "MessageN6PPatch.h"


// 転送されたメッセージは利用していないというフラグ立てる

/*
 * 噴出しTypeB型(グレーの枠が付いている方)の左上が表示される直前
 */

void OnTenshouExeFukidashiTypeB_LeftExecute() {
	// 転送されたメッセージは利用していないというフラグ立てる
}


/*
004A1FA9  |. B9 F0994D00    MOV     ECX, TENSHOU.004D99F0            ; |ASCII "???"
004A1FAE  |. 68 6CBA4B00    PUSH    TENSHOU.004BBA6C                 ; |Arg1 = 004BBA6C
004A1FB3  |. E8 41FEFFFF    CALL    TENSHOU.004A1DF9                 ; \TENSHOU.004A1DF9
				└ここを JMP TSMod.OnTSExeFukidashiTypeB_Left と書き換えてやる実際の処理
004A1FB8  |. 6A 00          PUSH    0                                ; /Arg6 = 00000000
 */
int pTenshouExeJumpFromToOnTenshouExeFukidashiTypeB_Left    =0x4A1FB3; // 関数はこのアドレスから、OnTenshouExeFukidashiTypeB_Leftへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeFukidashiTypeB_Left=0x4A1DF9; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeFukidashiTypeB_Left =0x4A1FB8; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeFukidashiTypeB_Left() {
	// スタックにためておく
	__asm {

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeFukidashiTypeB_LeftExecute();

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

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeFukidashiTypeB_Left

		jmp pTenshouExeReturnLblFromOnTenshouExeFukidashiTypeB_Left
	}
}

/*
00444769  |. 50             PUSH    EAX
0044476A  |. E8 3A2C0200    CALL    TENSHOU.004673A9
				└ここを JMP TSMod.OnTSExeFukidashiTypeB_Left と書き換えてやる実際の処理
0044476F  |. 83C4 04        ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromFukidashiTypeB_Left[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeFukidashiTypeB_Left() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeFukidashiTypeB_Left;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeFukidashiTypeB_Left + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeFukidashiTypeB_Left  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromFukidashiTypeB_Left+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeFukidashiTypeB_Left), cmdOnTenshouExeJumpFromFukidashiTypeB_Left, 5, NULL); //5バイトのみ書き込む
}


///-----------------------------------------------------------------------





/*
 * 噴出しTypeB型(グレーの枠が付いている方)の右下が表示される直前
 */

void OnTenshouExeFukidashiTypeB_RightExecute() {
	// 転送されたメッセージは利用していないというフラグ立てる
}


/*
004A1FFF  |. B9 A89A4D00    MOV     ECX, TENSHOU.004D9AA8            ; |ASCII "K"
004A2004  |. 68 6CBA4B00    PUSH    TENSHOU.004BBA6C                 ; |Arg1 = 004BBA6C
004A2009  |. E8 EBFDFFFF    CALL    TENSHOU.004A1DF9                 ; \TENSHOU.004A1DF9
				└ここを JMP TSMod.OnTSExeFukidashiTypeB_Right と書き換えてやる実際の処理
004A200E  |. 6A 00          PUSH    0                                ; /Arg6 = 00000000
 */
int pTenshouExeJumpFromToOnTenshouExeFukidashiTypeB_Right    =0x4A2009; // 関数はこのアドレスから、OnTenshouExeFukidashiTypeB_Rightへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeFukidashiTypeB_Right=0x4A1DF9; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeFukidashiTypeB_Right =0x4A200E; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeFukidashiTypeB_Right() {
	// スタックにためておく
	__asm {

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeFukidashiTypeB_RightExecute();

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

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeFukidashiTypeB_Right

		jmp pTenshouExeReturnLblFromOnTenshouExeFukidashiTypeB_Right
	}
}

/*
00444769  |. 50             PUSH    EAX
0044476A  |. E8 3A2C0200    CALL    TENSHOU.004673A9
				└ここを JMP TSMod.OnTSExeFukidashiTypeB_Right と書き換えてやる実際の処理
0044476F  |. 83C4 04        ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromFukidashiTypeB_Right[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeFukidashiTypeB_Right() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeFukidashiTypeB_Right;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeFukidashiTypeB_Right + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeFukidashiTypeB_Right  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromFukidashiTypeB_Right+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeFukidashiTypeB_Right), cmdOnTenshouExeJumpFromFukidashiTypeB_Right, 5, NULL); //5バイトのみ書き込む
}




