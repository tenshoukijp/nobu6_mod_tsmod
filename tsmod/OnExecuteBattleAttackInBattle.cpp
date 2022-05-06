#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


/*
 * 有効なバトル攻撃を実行した時。この処理に来る。
 * 選択してキャンセルした時や、結果がともなわないと決定している時(すでに兵士MAXなのに徴兵しようとしたなど)は、eaxが0もしくは通過しない。
 */

// 有効なコマンドを実行した武将ID
int iExecuteBattleAttackInBattleBushouID = -1;

// 有効なコマンドを実行したかどうか
int iExecuteBattleAttackInBattleValidCommand = -1;

// 各武将の１ターンあたりの残り実行数
extern int arrRemainMoveCountAllBushou[GAMEDATASTRUCT_BUSHOU_NUM];

void OnTenshouExeExecuteBattleAttackInBattleExecute() {

	// 武将範囲チェック
	if ( 0 <= iExecuteBattleAttackInBattleBushouID-1 && iExecuteBattleAttackInBattleBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 有効なコマンドが実行されたのであれば…
		if ( iExecuteBattleAttackInBattleValidCommand ) {
		}
	}
}


/*
0046AECE  |. E8 ADFAFFFF    |CALL    TENSHOU.0046A980                ; \TENSHOU.0046A980
0046AED3  |. 83C4 04        |ADD     ESP, 4
				└ここを JMP TSMod.OnTSExeExecuteBattleAttackInBattle と書き換えてやる実際の処理
0046AED6  |. 85C0           |TEST    EAX, EAX						EAXが１であったら、なんらかの攻撃行動を実際に行ったということ。
 */
int pTenshouExeJumpFromToOnTenshouExeExecuteBattleAttackInBattle    =0x46AECE; // 関数はこのアドレスから、OnTenshouExeExecuteBattleAttackInBattleへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeExecuteBattleAttackInBattle=0x46A980; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeExecuteBattleAttackInBattle =0x46AED3; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeExecuteBattleAttackInBattle() {
	// スタックにためておく
	__asm {
		// コマンドを実行した武将IDをコピー
		mov iExecuteBattleAttackInBattleBushouID, esi

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeExecuteBattleAttackInBattle

		// 有効なコマンドを実行したなら、eaxに有効な値が入っている。eaxが0ならば、コマンドは実行されたとはいえない。
		mov iExecuteBattleAttackInBattleValidCommand, eax;

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
	OnTenshouExeExecuteBattleAttackInBattleExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeExecuteBattleAttackInBattle
	}
}

/*
00444769  |. 50             PUSH    EAX
0044476A  |. E8 3A2C0200    CALL    TENSHOU.004673A9
				└ここを JMP TSMod.OnTSExeExecuteBattleAttackInBattle と書き換えてやる実際の処理
0044476F  |. 83C4 04        ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromExecuteBattleAttackInBattle[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeExecuteBattleAttackInBattle() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeExecuteBattleAttackInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeExecuteBattleAttackInBattle + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeExecuteBattleAttackInBattle  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromExecuteBattleAttackInBattle+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeExecuteBattleAttackInBattle), cmdOnTenshouExeJumpFromExecuteBattleAttackInBattle, 5, NULL); //5バイトのみ書き込む
}




