#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "FuncPlayMovie.h"

void OnTenshouExeMovieBlackBack1Execute() {
}


/*
0043CE65  |. 56             PUSH    ESI
0043CE66     E8 07010000    CALL    TENSHOU.0043CF72
				└ここを JMP TSMod.OnTSExeMovieBlackBack1 と書き換えてやる実際の処理
0043CE6B  |. 83C4 04        ADD     ESP, 4
*/
int pTenshouExeJumpFromToOnTenshouExeMovieBlackBack1	 =0x43CE66; // 関数はこのアドレスから、OnTenshouExeMovieBlackBack1へとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeMovieBlackBack1 =0x43CF72; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeMovieBlackBack1	 =0x43CE6B; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeMovieBlackBack1() {
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
	
	// ゲームデータをプレイヤーターンが変わった段階で読み込み更新しておく
	OnTenshouExeMovieBlackBack1Execute();

	// ブラックバックするならこっち。
	if ( isMovieWithBlackBack ) {
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
			call pTenshouExeJumpCallFromToOnTenshouExeMovieBlackBack1

			jmp pTenshouExeReturnLblFromOnTenshouExeMovieBlackBack1
		}
	
	// ブラックバックしないならこっち
	} else {
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

			jmp pTenshouExeReturnLblFromOnTenshouExeMovieBlackBack1
		}
	}
}

char cmdOnTenshouExeJumpFromMovieBlackBack1[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeMovieBlackBack1() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeMovieBlackBack1;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeMovieBlackBack1 + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeMovieBlackBack1  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromMovieBlackBack1+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeMovieBlackBack1), cmdOnTenshouExeJumpFromMovieBlackBack1, 5, NULL); //5バイトのみ書き込む
}




//------------------------------------------------------------------------------------


void OnTenshouExeMovieBlackBack2Execute() {
}


/*
0044AB0D  |. FF15 34764E00  CALL    DWORD PTR DS:[<&USER32.EnableMen>; \EnableMenuItem
0044AB13  |. E8 B022FDFF    CALL    TENSHOU.0041CDC8
				└ここを JMP TSMod.OnTSExeMovieBlackBack2 と書き換えてやる実際の処理
0044AB18  |. 6A 01          PUSH    1                                ; /Erase = TRUE
*/
int pTenshouExeJumpFromToOnTenshouExeMovieBlackBack2		=0x44AB13; // 関数はこのアドレスから、OnTenshouExeMovieBlackBack2へとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeMovieBlackBack2    =0x41CDC8; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeMovieBlackBack2	    =0x44AB18; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeMovieBlackBack2() {
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
	
	// ゲームデータをプレイヤーターンが変わった段階で読み込み更新しておく
	OnTenshouExeMovieBlackBack2Execute();

	// ブラックバックするならこっち。
	if ( isMovieWithBlackBack ) {
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
			call pTenshouExeJumpCallFromToOnTenshouExeMovieBlackBack2

			jmp pTenshouExeReturnLblFromOnTenshouExeMovieBlackBack2
		}
	// ブラックバックしないならこっち
	} else {
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

			jmp pTenshouExeReturnLblFromOnTenshouExeMovieBlackBack2
		}
	}
}

char cmdOnTenshouExeJumpFromMovieBlackBack2[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeMovieBlackBack2() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeMovieBlackBack2;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeMovieBlackBack2 + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeMovieBlackBack2  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromMovieBlackBack2+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeMovieBlackBack2), cmdOnTenshouExeJumpFromMovieBlackBack2, 5, NULL); //5バイトのみ書き込む
}



