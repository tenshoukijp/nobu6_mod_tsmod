#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


/*
 * 戦闘中にプレイヤーの攻撃担当軍団のターンが終わった時
 */


void OnTenshouExePlayerGundanTurnEndInBattleExecute() {
}


/*
0046B237  |> 5F             POP     EDI								
				└ここを JMP TSMod.OnTSExePlayerGundanTurnEndInBattle と書き換えてやる実際の処理

0046B238  |. 5E             POP     ESI								
0046B239  |. 8BE5           MOV     ESP, EBP					勢力のターンが切り替わる最後で、ここに流れてくる。
0046B23B  |. 5D             POP     EBP						
0046B23C  |. C3             RETN
*/
int pTenshouExeJumpFromToOnTenshouExePlayerGundanTurnEndInBattle	  =0x46B237; // 関数はこのアドレスから、OnTenshouExePlayerGundanTurnEndInBattleへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExePlayerGundanTurnEndInBattle  =0x46B23C; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w

// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExePlayerGundanTurnEndInBattle() {
	// スタックにためておく
	__asm {
		// 以下４行は、元々TENSHOU.EXE内にあった記述そのまま
		pop edi
		pop esi
		mov esp, ebp
		pop ebp


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
	OnTenshouExePlayerGundanTurnEndInBattleExecute();

	__asm {

		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		jmp pTenshouExeReturnLblFromOnTenshouExePlayerGundanTurnEndInBattle
	}
}

/*
0046B237  |> 5F             POP     EDI								
				└ここを JMP TSMod.OnTSExePlayerGundanTurnEndInBattle と書き換えてやる実際の処理

0046B238  |. 5E             POP     ESI								
0046B239  |. 8BE5           MOV     ESP, EBP					勢力のターンが切り替わる最後で、ここに流れてくる。
0046B23B  |. 5D             POP     EBP						
0046B23C  |. C3             RETN
*/
char cmdOnTenshouExeJumpFromPlayerGundanTurnEndInBattle[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExePlayerGundanTurnEndInBattle() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExePlayerGundanTurnEndInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExePlayerGundanTurnEndInBattle + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExePlayerGundanTurnEndInBattle  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromPlayerGundanTurnEndInBattle+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExePlayerGundanTurnEndInBattle), cmdOnTenshouExeJumpFromPlayerGundanTurnEndInBattle, 5, NULL); //5バイトのみ書き込む
}




