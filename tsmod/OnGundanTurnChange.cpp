#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"


/*
 メイン画面中の軍団ターンが切り替わった時。
 一季節１回ずつのみのようだ。
 セーブからのロード時には実行されない。
 その理由の考え方として、相場変更メソッドと同じだと思えばよいだろう。
 */

/*
004670C0  |. 77 08                   JA      SHORT TENSHOU.004670CA
004670C2  |. 66:A3 57934C00          MOV     WORD PTR DS:[4C9357], AX　← ここでnb6turn.gundan に値が入れられる。
004670C8  |. EB 11                   JMP     SHORT TENSHOU.004670DB
*/

int iGundanValueOfGundanTurnChange = -1;
void OnTenshouExeGundanTurnChangeExecute() {
	int iGundanID = (iGundanValueOfGundanTurnChange & 0xFFFF) - 1; // AXが値なので、下位ビットだけ利用する。

	if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
		int iBushouID = nb6gundan[iGundanID].leader - 1;
		if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

			// ScenarioModの軍団ターン切り替え目の関数の呼び出し
			if ( p_snOnGundanTurnInMainScreen ) {
				p_snOnGundanTurnInMainScreen( iGundanID + 1 ); // 軍団番号として渡すので１加える。
			}
		}
	}

}


/*
004670C0  |. 77 08                   JA      SHORT TENSHOU.004670CA
004670C2  |. 66:A3 57934C00          MOV     WORD PTR DS:[4C9357], AX　← ここでnb6turn.gundan に値が入れられる。
				└ここを JMP TSMod.OnTSExeGundanTurnChange と書き換えてやる実際の処理
004670C8  |. EB 11                   JMP     SHORT TENSHOU.004670DB
*/
int pTenshouExeJumpFromToOnTenshouExeGundanTurnChange    =0x4670C2; // 関数はこのアドレスから、OnTenshouExeGundanTurnChangeへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeGundanTurnChange =0x4670C8; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeGundanTurnChange() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		MOV     WORD PTR DS:[0x4C9357], AX

		// 軍団番号を保存しておく。
		mov iGundanValueOfGundanTurnChange, eax

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
	OnTenshouExeGundanTurnChangeExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeGundanTurnChange
	}
}



char cmdOnTenshouExeJumpFromGundanTurnChange[7]="\xE9\x90\x90\x90\x90\x90";
// 元の命令が5バイト、以後の関数で生まれる命令が合計６バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeGundanTurnChange() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeGundanTurnChange;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeGundanTurnChange + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGundanTurnChange  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromGundanTurnChange+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeGundanTurnChange), cmdOnTenshouExeJumpFromGundanTurnChange, 6, NULL); //6バイトのみ書き込む
}



//----------------------------------------------------------- 軍団ターンがクリアされる場所
void OnTenshouExeGundanTurnChangeAfterExecute() {
	// 大名A 軍団1 軍団2 0xFFFF 大名B 軍団1 0xFFFF 大名C 軍団1 軍団2 軍団3 0xFFFF みたいな処理となる。
	// int iTmp = (iGundanValueOfGundanTurnChange & 0xFFFF) - 1; // AXが値なので、下位ビットだけ利用する。
}

/*
004670DC  /$ 66:C705 57934C00 FFFF              MOV     WORD PTR DS:[4C9357], 0FFFF
*/
/*
004670DC  /$ 66:C705 57934C00 FFFF              MOV     WORD PTR DS:[4C9357], 0FFFF
				└ここを JMP TSMod.OnTSExeGundanTurnChangeAfter と書き換えてやる実際の処理
004670E5  \. C3                                 RETN
*/
int pTenshouExeJumpFromToOnTenshouExeGundanTurnChangeAfter    =0x4670DC; // 関数はこのアドレスから、OnTenshouExeGundanTurnChangeAfterへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeGundanTurnChangeAfter =0x4670E5; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeGundanTurnChangeAfter() {
	// スタックにためておく
	__asm {

		// 元々の処理をここに
		MOV     WORD PTR DS:[0x4C9357], 0xFFFF   // 軍団ターンをクリア

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
	OnTenshouExeGundanTurnChangeAfterExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeGundanTurnChangeAfter
	}
}



char cmdOnTenshouExeJumpFromGundanTurnChangeAfter[10]="\xE9\x90\x90\x90\x90\x90\x90\x90\x90";
// 元の命令が5バイト、以後の関数で生まれる命令が合計６バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeGundanTurnChangeAfter() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeGundanTurnChangeAfter;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeGundanTurnChangeAfter + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGundanTurnChangeAfter  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromGundanTurnChangeAfter+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeGundanTurnChangeAfter), cmdOnTenshouExeJumpFromGundanTurnChangeAfter, 9, NULL); //9バイトのみ書き込む
}
