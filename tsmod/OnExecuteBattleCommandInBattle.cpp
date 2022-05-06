#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


/*
 * 有効なバトルコマンドを実行した時。この処理に来る。
 * 選択してキャンセルした時や、結果がともなわないと決定している時(すでに兵士MAXなのに徴兵しようとしたなど)は、ここは通過しない。
 */

// 有効なコマンドを実行した武将ID
int iExecuteBattleCommandInBattleBushouID = -1;

// 有効なコマンドを実行したかどうか
int iExecuteBattleCommandInBattleValidCommand = -1;

// 各武将の１ターンあたりの残り実行数
extern int arrRemainMoveCountAllBushou[GAMEDATASTRUCT_BUSHOU_NUM];

void OnTenshouExeExecuteBattleCommandInBattleExecute() {
	// 武将範囲チェック
	if ( 0 <= iExecuteBattleCommandInBattleBushouID-1 && iExecuteBattleCommandInBattleBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 有効なコマンドが実行されたのであれば…
		if ( iExecuteBattleCommandInBattleValidCommand ) {
		}
	}
}


/*
0046AE59  |. E8 71F9FFFF    |CALL    TENSHOU.0046A7CF
				└ここを JMP TSMod.OnTSExeExecuteBattleCommandInBattle と書き換えてやる実際の処理
0046AE5E  |. 85C0           |TEST    EAX, EAX
0046AE60  |. 0F85 4E010000  |JNZ     TENSHOU.0046AFB4
0046AE66  |.^E9 02FFFFFF    |JMP     TENSHOU.0046AD6D
 */
int pTenshouExeJumpFromToOnTenshouExeExecuteBattleCommandInBattle    =0x46AE59; // 関数はこのアドレスから、OnTenshouExeExecuteBattleCommandInBattleへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeExecuteBattleCommandInBattle=0x46A7CF; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeExecuteBattleCommandInBattle =0x46AE5E; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeExecuteBattleCommandInBattle() {
	// スタックにためておく
	__asm {
		// コマンドを実行した武将IDをコピー
		mov iExecuteBattleCommandInBattleBushouID, ebx

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeExecuteBattleCommandInBattle

		// 有効なコマンドを実行したなら、eaxに有効な値が入っている。eaxが0ならば、コマンドは実行されたとはいえない。
		mov iExecuteBattleCommandInBattleValidCommand, eax;

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
	OnTenshouExeExecuteBattleCommandInBattleExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeExecuteBattleCommandInBattle
	}
}

/*
00444769  |. 50             PUSH    EAX
0044476A  |. E8 3A2C0200    CALL    TENSHOU.004673A9
				└ここを JMP TSMod.OnTSExeExecuteBattleCommandInBattle と書き換えてやる実際の処理
0044476F  |. 83C4 04        ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromExecuteBattleCommandInBattle[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeExecuteBattleCommandInBattle() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeExecuteBattleCommandInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeExecuteBattleCommandInBattle + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeExecuteBattleCommandInBattle  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromExecuteBattleCommandInBattle+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeExecuteBattleCommandInBattle), cmdOnTenshouExeJumpFromExecuteBattleCommandInBattle, 5, NULL); //5バイトのみ書き込む
}




