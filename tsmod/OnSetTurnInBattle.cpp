#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"
#include "OnChangeWeather.h"

extern int iCurIncrementTurnInBattle;
BOOL isMustCallOfDecrementTurnInBatttle = FALSE;
void OnTenshouExeSetTurnInBattleExecute() {
	if ( iCurIncrementTurnInBattle == -1 ) {
		isMustCallOfDecrementTurnInBatttle = FALSE;
	} else {
		isMustCallOfDecrementTurnInBatttle = TRUE;
	}
		
}


/*
戦争で１ターン減る度、ターンをセットしている場所
00496315  /$ 8B4424 04      MOV     EAX, DWORD PTR SS:[ESP+4]
00496319  |. A3 D2B44B00    MOV     DWORD PTR DS:[4BB4D2], EAX
0049631E  \. C3             RETN
*/
int pTenshouExeJumpFromToOnTenshouExeSetTurnInBattle	 =0x496319; // 関数はこのアドレスから、OnTenshouExeSetTurnInBattleへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeSetTurnInBattle  =0x49631E; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w

// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeSetTurnInBattle() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあった記述そのまま
		MOV     DWORD PTR DS : [GAMEDATASTRUCT_HOWMUCHTURNPROCEED_INBATTLE], EAX

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
	OnTenshouExeSetTurnInBattleExecute();

	__asm {

		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		jmp pTenshouExeReturnLblFromOnTenshouExeSetTurnInBattle
	}
}

char cmdOnTenshouExeJumpFromSetTurnInBattle[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeSetTurnInBattle() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeSetTurnInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSetTurnInBattle + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeSetTurnInBattle  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromSetTurnInBattle+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSetTurnInBattle), cmdOnTenshouExeJumpFromSetTurnInBattle, 5, NULL); //5バイトのみ書き込む
}




