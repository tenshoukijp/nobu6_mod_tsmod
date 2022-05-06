#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "MessageN6PPatch.h"


/*
 * フォーカスの噴出しが切り替わったタイミング
 */

void OnTenshouExeFukidashiTypeBFocusChangeExecute() {
	// 新規メッセージは使われていないこととなる。
}


/*
0042312A   $ E8 F13E0400    CALL    TENSHOU.00467020
				└ここを JMP TSMod.OnTSExeFukidashiTypeBFocusChange と書き換えてやる実際の処理
0042312F   . 83F8 04        CMP     EAX, 4
00423132   . 73 13          JNB     SHORT TENSHOU.00423147
00423134   . 0FAF4424 04    IMUL    EAX, DWORD PTR SS:[ESP+4]
 */
int pTenshouExeJumpFromToOnTenshouExeFukidashiTypeBFocusChange    =0x42312A; // 関数はこのアドレスから、OnTenshouExeFukidashiTypeBFocusChangeへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeFukidashiTypeBFocusChange=0x467020; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeFukidashiTypeBFocusChange =0x42312F; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeFukidashiTypeBFocusChange() {
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
	OnTenshouExeFukidashiTypeBFocusChangeExecute();

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
		call pTenshouExeJumpCallFromToOnTenshouExeFukidashiTypeBFocusChange

		jmp pTenshouExeReturnLblFromOnTenshouExeFukidashiTypeBFocusChange
	}
}

/*
0042312A   $ E8 F13E0400    CALL    TENSHOU.00467020
				└ここを JMP TSMod.OnTSExeFukidashiTypeBFocusChange と書き換えてやる実際の処理
0042312F   . 83F8 04        CMP     EAX, 4
00423132   . 73 13          JNB     SHORT TENSHOU.00423147
00423134   . 0FAF4424 04    IMUL    EAX, DWORD PTR SS:[ESP+4]
*/
char cmdOnTenshouExeJumpFromFukidashiTypeBFocusChange[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeFukidashiTypeBFocusChange() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeFukidashiTypeBFocusChange;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeFukidashiTypeBFocusChange + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeFukidashiTypeBFocusChange  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromFukidashiTypeBFocusChange+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeFukidashiTypeBFocusChange), cmdOnTenshouExeJumpFromFukidashiTypeBFocusChange, 5, NULL); //5バイトのみ書き込む
}


//--------------------------------------------------------------------------------------------------------------------




/*
 * フォーカスの噴出しのフォーカスが切り替わる、その少し前のタイミング
 * これから噴出しのフォーカスが変わろうとしている、まだメモリマップ上のメッセージは次のメッセージへと切り替わっていない。
 */

void OnTenshouExeFukidashiTypeBMessageChangePrevExecute() {
	MessageN6PPatch::isOnTenshouExeMessageCopyUsed = false;
}


/*
0046C1CB  /$ C705 5CD74C00 >MOV     DWORD PTR DS:[4CD75C], TENSHOU.004D97C0          ;  ASCII "あいわかった
代金はここだ
早う品をよこせ"
0046C1D5  |. 33C0           XOR     EAX, EAX
0046C1D7  |. A3 68D74C00    MOV     DWORD PTR DS:[4CD768], EAX
0046C1DC  |. A3 60D74C00    MOV     DWORD PTR DS:[4CD760], EAX
0046C1E1  |. A3 64D74C00    MOV     DWORD PTR DS:[4CD764], EAX
0046C1E6  |. A3 FCD74C00    MOV     DWORD PTR DS:[4CD7FC], EAX
				└ここを JMP TSMod.OnTSExeFukidashiTypeBMessageChange と書き換えてやる実際の処理
0046C1EB  \. C3             RETN
 */
int pTenshouExeJumpFromToOnTenshouExeFukidashiTypeBMessageChange    =0x46C1E6; // 関数はこのアドレスから、OnTenshouExeFukidashiTypeBMessageChangeへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeFukidashiTypeBMessageChange =0x46C1EB; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeFukidashiTypeBMessageChange() {
	// スタックにためておく
	__asm {

		// 元々TENSHOU.EXE内にあったものをここで
		MOV     DWORD PTR DS:[0x4CD7FC], EAX

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
	OnTenshouExeFukidashiTypeBMessageChangePrevExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeFukidashiTypeBMessageChange
	}
}

/*
0046C1CB  /$ C705 5CD74C00 >MOV     DWORD PTR DS:[4CD75C], TENSHOU.004D97C0          ;  ASCII "あいわかった
代金はここだ
早う品をよこせ"
0046C1D5  |. 33C0           XOR     EAX, EAX
0046C1D7  |. A3 68D74C00    MOV     DWORD PTR DS:[4CD768], EAX
0046C1DC  |. A3 60D74C00    MOV     DWORD PTR DS:[4CD760], EAX
0046C1E1  |. A3 64D74C00    MOV     DWORD PTR DS:[4CD764], EAX
0046C1E6  |. A3 FCD74C00    MOV     DWORD PTR DS:[4CD7FC], EAX
				└ここを JMP TSMod.OnTSExeFukidashiTypeBMessageChange と書き換えてやる実際の処理
0046C1EB  \. C3             RETN
*/
char cmdOnTenshouExeJumpFromFukidashiTypeBMessageChange[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeFukidashiTypeBMessageChange() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeFukidashiTypeBMessageChange;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeFukidashiTypeBMessageChange + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeFukidashiTypeBMessageChange  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromFukidashiTypeBMessageChange+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeFukidashiTypeBMessageChange), cmdOnTenshouExeJumpFromFukidashiTypeBMessageChange, 5, NULL); //5バイトのみ書き込む
}








/*
 * フォーカスの噴出しの１発目、その少し前のタイミング
 * これから噴出しのフォーカスが変わろうとしている、まだメモリマップ上のメッセージは次のメッセージへと切り替わっていない。
 */

void OnTenshouExeFukidashiTypeBMessageStartPrevExecute() {
	MessageN6PPatch::isOnTenshouExeMessageCopyUsed = false;
}



/*
0046C1A8  |. FF05 5CD74C00  INC     DWORD PTR DS:[4CD75C]                         ;  TENSHOU.004D97C0
0046C1AE  |. FF05 68D74C00  INC     DWORD PTR DS:[4CD768]
0046C1B4  |. 813D 68D74C00 >CMP     DWORD PTR DS:[4CD768], 1FE
0046C1BE  |. 72 0A          JB      SHORT TENSHOU.0046C1CA
0046C1C0  |. C705 FCD74C00 >MOV     DWORD PTR DS:[4CD7FC], 1
				└ここを JMP TSMod.OnTSExeFukidashiTypeBMessageStart と書き換えてやる実際の処理
0046C1CA  \> C3             RETN
 */
int pTenshouExeJumpFromToOnTenshouExeFukidashiTypeBMessageStart    =0x46C1C0; // 関数はこのアドレスから、OnTenshouExeFukidashiTypeBMessageStartへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeFukidashiTypeBMessageStart =0x46C1CA; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeFukidashiTypeBMessageStart() {
	// スタックにためておく
	__asm {

		// 元々TENSHOU.EXE内にあったものをここで
		MOV     DWORD PTR DS:[0x4CD7FC], 1

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
	OnTenshouExeFukidashiTypeBMessageStartPrevExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeFukidashiTypeBMessageStart
	}
}

/*
0046C1CB  /$ C705 5CD74C00 >MOV     DWORD PTR DS:[4CD75C], TENSHOU.004D97C0          ;  ASCII "あいわかった
代金はここだ
早う品をよこせ"
0046C1D5  |. 33C0           XOR     EAX, EAX
0046C1D7  |. A3 68D74C00    MOV     DWORD PTR DS:[4CD768], EAX
0046C1DC  |. A3 60D74C00    MOV     DWORD PTR DS:[4CD760], EAX
0046C1E1  |. A3 64D74C00    MOV     DWORD PTR DS:[4CD764], EAX
0046C1E6  |. A3 FCD74C00    MOV     DWORD PTR DS:[4CD7FC], EAX
				└ここを JMP TSMod.OnTSExeFukidashiTypeBMessageStart と書き換えてやる実際の処理
0046C1EB  \. C3             RETN
*/
char cmdOnTenshouExeJumpFromFukidashiTypeBMessageStart[7]="\xE9\x90\x90\x90\x90\x90";
// 元の命令が5バイト、以後の関数で生まれる命令が合計６バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeFukidashiTypeBMessageStart() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeFukidashiTypeBMessageStart;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeFukidashiTypeBMessageStart + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeFukidashiTypeBMessageStart  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromFukidashiTypeBMessageStart+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeFukidashiTypeBMessageStart), cmdOnTenshouExeJumpFromFukidashiTypeBMessageStart, 6, NULL); //6バイトのみ書き込む
}

