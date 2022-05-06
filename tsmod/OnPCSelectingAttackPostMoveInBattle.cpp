#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"

extern void OnSelectingAttackEventHandlerInBattle(int iBushouID, int iTargetID, int iSelectActionID, BOOL isMoveBefore);

#include "MakeIsseikougekiMembers.h"

// 戦闘中PCが実際の行動をとる際のIDが決定された時。ユニットが動く後に行動するものの場合。

/*
0044864A  |. 57             PUSH    EDI
0044864B  |. 56             PUSH    ESI
0044864C  |. FF149D C8AE4C0>CALL    DWORD PTR DS:[EBX*4+4CAEC8]                                        ;  TENSHOU.00431ECD
00448653  |. E9 80000000    JMP     TENSHOU.004486D8

実際に実行する直前のジャンプ所(NPC)
0044864C  |. FF149D C8AE4C0>CALL    DWORD PTR DS:[EBX*4+4CAEC8]                                        ;  TENSHOU.00431ECD
*/


int iPCSelectingAttackPostMoveID = -1;		// 0:攻撃 1:突撃
int iBushouIdOfPCSelectingAttackPostMove = -1;
int iTargetIdOfPCSelectingAttackPostMove = -1;

// 0:攻撃 1:騎馬 2:鉄砲 3:大砲 4:一斉攻撃
void OnTenshouExePCSelectingAttackPostMoveInBattleExecute() {
	; // OutputDebugStream("%d", iSelectingAttackMenuID );
	int iBushouID = (iBushouIdOfPCSelectingAttackPostMove & 0xFFFF)-1;
	int iTargetID = (iTargetIdOfPCSelectingAttackPostMove & 0xFFFF)-1;

	OnSelectingAttackEventHandlerInBattle(iBushouID, iTargetID, iPCSelectingAttackPostMoveID, FALSE);

}

/*
0044864A  |. 57             PUSH    EDI
0044864B  |. 56             PUSH    ESI
0044864C  |. FF149D C8AE4C00                  CALL    DWORD PTR DS:[EBX*4+4CAEC8]                                        ;  TENSHOU.00431ECD
				└この行を JMP TSMod.OnTSExePCSelectingAttackPostMoveInBattle と書き換えてやる実際の処理
00448653  |. E9 80000000    JMP     TENSHOU.004486D8
*/
int pTenshouExeJumpFromToOnTenshouExePCSelectingAttackPostMoveInBattle		=0x44864C; // 関数はこのアドレスから、OnTenshouExePCSelectingAttackPostMoveInBattleへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExePCSelectingAttackPostMoveInBattle  =0x448653; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExePCSelectingAttackPostMoveInBattle() {
	// スタックにためておく
	__asm {

		mov iPCSelectingAttackPostMoveID, ebx
		mov iBushouIdOfPCSelectingAttackPostMove, esi // 自分の武将番号
		mov iTargetIdOfPCSelectingAttackPostMove, edi // 相手の武将番号

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 技能選択時に行う処理
	OnTenshouExePCSelectingAttackPostMoveInBattleExecute();

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

		// 元々天翔記にあったジャンプ場所
		CALL    DWORD PTR DS:[EBX*4+0x4CAEC8]

		jmp pTenshouExeReturnLblFromOnTenshouExePCSelectingAttackPostMoveInBattle
	}
}

char cmdOnTenshouExeJumpFromPCSelectingAttackPostMoveInBattle[8]="\xE9\x90\x90\x90\x90\x90\x90";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExePCSelectingAttackPostMoveInBattle() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExePCSelectingAttackPostMoveInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExePCSelectingAttackPostMoveInBattle + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExePCSelectingAttackPostMoveInBattle  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromPCSelectingAttackPostMoveInBattle+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExePCSelectingAttackPostMoveInBattle), cmdOnTenshouExeJumpFromPCSelectingAttackPostMoveInBattle, 7, NULL); //7バイトのみ書き込む
}
