#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"


int iNextTurnIsFieldOrCastle = -1;		// 野戦なら0、城内戦なら1
extern int iNextTurnCastleID;	// 直後のターンが城の場合の城番号。城でない場合には、0xFFFFが入る。
void OnTenshouExeNextTurnIsFieldOrCastleExecute() {
	// 次は野戦である。
	if ( iNextTurnIsFieldOrCastle == 0 ) {
		iNextTurnCastleID = 0xFFFF;

		// 野戦だったので、そのままScenarioModに伝達
		if ( p_snSetCurrentCastleIDInBattle ) {
			((PFNSNSETCURRENTCASTLEIDINBATTLE) p_snSetCurrentCastleIDInBattle)(iNextTurnCastleID);
		}
	}

	
}


/*
0046B575 . E8 A502FEFF CALL TENSHOU.0044B81F			少なくともEAXに、野戦か城内戦かの情報が入る 
				└ここを JMP TSMod.OnTSExeNextTurnIsFieldOrCastle と書き換えてやる実際の処理
0046B57A . 83C4 0C ADD ESP, 0C 
0046B57D . 85C0 TEST EAX, EAX							EAXが0なら野戦、1なら城内戦 
 */
int pTenshouExeJumpFromToOnTenshouExeNextTurnIsFieldOrCastle    =0x46B575; // 関数はこのアドレスから、OnTenshouExeNextTurnIsFieldOrCastleへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeNextTurnIsFieldOrCastle=0x44B81F; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeNextTurnIsFieldOrCastle =0x46B57A; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeNextTurnIsFieldOrCastle() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeNextTurnIsFieldOrCastle

		mov iNextTurnIsFieldOrCastle, eax

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
	OnTenshouExeNextTurnIsFieldOrCastleExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeNextTurnIsFieldOrCastle
	}
}

/*
0046B575 . E8 A502FEFF CALL TENSHOU.0044B81F			少なくともEAXに、野戦か城内戦かの情報が入る 
				└ここを JMP TSMod.OnTSExeNextTurnIsFieldOrCastle と書き換えてやる実際の処理
0046B57A . 83C4 0C ADD ESP, 0C 
0046B57D . 85C0 TEST EAX, EAX							EAXが1なら野戦、0なら城内戦 
*/
char cmdOnTenshouExeJumpFromNextTurnIsFieldOrCastle[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeNextTurnIsFieldOrCastle() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeNextTurnIsFieldOrCastle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeNextTurnIsFieldOrCastle + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeNextTurnIsFieldOrCastle  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromNextTurnIsFieldOrCastle+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeNextTurnIsFieldOrCastle), cmdOnTenshouExeJumpFromNextTurnIsFieldOrCastle, 5, NULL); //5バイトのみ書き込む
}








//---------------------------------------------------------------------------------------------


int iNextTurnCastleID = 0xFFFF;	// 直後のターンが城の場合の城番号。城でない場合には、0xFFFFが入る。
void OnTenshouExeGetNextTurnCastleIDExecute() {
	iNextTurnCastleID = iNextTurnCastleID & 0xFFFF; // EAX中、AXだけが城番号

	if ( 0 <= iNextTurnCastleID-1 && iNextTurnCastleID-1 < GAMEDATASTRUCT_CASTLE_NUM ) {

		// 城戦だったので、ここでScenarioModに伝達
		if ( p_snSetCurrentCastleIDInBattle ) {
			((PFNSNSETCURRENTCASTLEIDINBATTLE) p_snSetCurrentCastleIDInBattle)(iNextTurnCastleID);
		}
	}
}

/*
0046B589 . E8 B0FEFDFF CALL TENSHOU.0044B43E		上のOnTenshouExeNextTurnIsFieldOrCastleでのEAX判定があるので、城内の時のみ来る。城内のデータをメモリやレジスタにセットか何か。 
0046B58E . 83C4 04 ADD ESP, 4						└これのリターン段階で、EAXのうち、AXが城番号 
0046B591 . 56 PUSH ESI
*/
int pTenshouExeJumpFromToOnTenshouExeGetNextTurnCastleID    =0x46B589; // 関数はこのアドレスから、OnTenshouExeGetNextTurnCastleIDへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeGetNextTurnCastleID=0x44B43E; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeGetNextTurnCastleID =0x46B58E; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeGetNextTurnCastleID() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeGetNextTurnCastleID

		mov iNextTurnCastleID, eax		// このEAX中AXは(この直後の)城戦の城番号

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
	OnTenshouExeGetNextTurnCastleIDExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeGetNextTurnCastleID
	}
}

/*
0046B589 . E8 B0FEFDFF CALL TENSHOU.0044B43E		上のOnTenshouExeNextTurnIsFieldOrCastleでのEAX判定があるので、城内の時のみ来る。城内のデータをメモリやレジスタにセットか何か。 
0046B58E . 83C4 04 ADD ESP, 4						└これのリターン段階で、EAXのうち、AXが城番号 
0046B591 . 56 PUSH ESI
*/
char cmdOnTenshouExeJumpFromGetNextTurnCastleID[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeGetNextTurnCastleID() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeGetNextTurnCastleID;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeGetNextTurnCastleID + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGetNextTurnCastleID  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromGetNextTurnCastleID+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeGetNextTurnCastleID), cmdOnTenshouExeJumpFromGetNextTurnCastleID, 5, NULL); //5バイトのみ書き込む
}




//---------------------------------------------------------------------------------------------

