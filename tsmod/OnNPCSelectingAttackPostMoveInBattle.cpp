#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"

extern void OnSelectingAttackEventHandlerInBattle(int iBushouID, int iTargetID, int iSelectActionID, BOOL isMoveBefore);

#include "MakeIsseikougekiMembers.h"

// 戦闘中NPCが実際の行動をとる際のIDが決定された時。ユニットが動く後に行動するものの場合。

/*
00489DBA  |. 83F8 01        CMP     EAX, 1							←ここがEAXなら移動後で突撃。それ以外(多分0)なら攻撃。EBXが相手の武将番号、ESIが自分の武将番号
00489DBD  |. 75 11          JNZ     SHORT TENSHOU.00489DD0
00489DBF  |. 6A 01          PUSH    1                                ; /Arg5 = 00000001


実際に実行する直前のジャンプ所(NPC)
★004486D1  |. FF14BD C8AE4C00             CALL    DWORD PTR DS:[EDI*4+4CAEC8]
*/


int iNPCSelectingAttackPostMoveID = -1;		// 0:攻撃 1:突撃
int iBushouIdOfNPCSelectingAttackPostMove = -1;
int iTargetIdOfNPCSelectingAttackPostMove = -1;

// 0:攻撃 1:騎馬 2:鉄砲 3:大砲 4:一斉攻撃
void OnTenshouExeNPCSelectingAttackPostMoveInBattleExecute() {
	// OutputDebugStream("%d", iSelectingAttackMenuID );
	int iBushouID = (iBushouIdOfNPCSelectingAttackPostMove & 0xFFFF)-1;
	int iTargetID = (iTargetIdOfNPCSelectingAttackPostMove & 0xFFFF)-1;

	OnSelectingAttackEventHandlerInBattle(iBushouID, iTargetID, iNPCSelectingAttackPostMoveID, FALSE);

}

/*
00489DBA  |. 83F8 01        CMP     EAX, 1							←ここがEAXなら移動後で突撃。それ以外(多分0)なら攻撃。EBXが相手の武将番号、ESIが自分の武将番号
00489DBD  |. 75 11          JNZ     SHORT TENSHOU.00489DD0
				└この2行を JMP TSMod.OnTSExeNPCSelectingAttackPostMoveInBattle と書き換えてやる実際の処理
00489DBF  |. 6A 01          PUSH    1                                ; /Arg5 = 00000001
*/
int pTenshouExeJumpFromToOnTenshouExeNPCSelectingAttackPostMoveInBattle		=0x489DBA; // 関数はこのアドレスから、OnTenshouExeNPCSelectingAttackPostMoveInBattleへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeNPCSelectingAttackPostMoveInBattle =0x489DBD; 
int pTenshouExeReturnLblFromOnTenshouExeNPCSelectingAttackPostMoveInBattle  =0x489DBF; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
int pTenshouExeReturnLblFromOnTenshouExeNPCSelectingAttackPostMoveInBattleNormal  =0x489DD0; // 普通の攻撃の際のジャンプ先  JNZ     SHORT TENSHOU.00489DD0
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeNPCSelectingAttackPostMoveInBattle() {
	// スタックにためておく
	__asm {

		mov iNPCSelectingAttackPostMoveID, eax			// 選択行動(0=攻撃, 1=突撃)
		mov iTargetIdOfNPCSelectingAttackPostMove, ebx	// 相手の武将番号

		// eaxは保存したので、eaxを維持し変数に利用する。
		mov eax, DWORD PTR SS:[ebp+8]
		mov iBushouIdOfNPCSelectingAttackPostMove, eax // 自分の武将を保存

		mov eax, iNPCSelectingAttackPostMoveID  // ebxを元へと戻す

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
	OnTenshouExeNPCSelectingAttackPostMoveInBattleExecute();

	if ( iNPCSelectingAttackPostMoveID == 1 ) {

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

			jmp pTenshouExeReturnLblFromOnTenshouExeNPCSelectingAttackPostMoveInBattle
		}
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

			jmp pTenshouExeReturnLblFromOnTenshouExeNPCSelectingAttackPostMoveInBattleNormal
		}
	}
}

char cmdOnTenshouExeJumpFromNPCSelectingAttackPostMoveInBattle[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeNPCSelectingAttackPostMoveInBattle() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeNPCSelectingAttackPostMoveInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeNPCSelectingAttackPostMoveInBattle + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeNPCSelectingAttackPostMoveInBattle  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromNPCSelectingAttackPostMoveInBattle+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeNPCSelectingAttackPostMoveInBattle), cmdOnTenshouExeJumpFromNPCSelectingAttackPostMoveInBattle, 5, NULL); //5バイトのみ書き込む
}
