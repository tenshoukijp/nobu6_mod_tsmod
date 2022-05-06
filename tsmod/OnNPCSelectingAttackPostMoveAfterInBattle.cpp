#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"



// 戦闘中NPCが実際の行動をとる際のIDが決定されて、実際の関数へとジャンプする所。

/*

004486D0  |. 56                              PUSH    ESI
004486D1  |. FF14BD C8AE4C00                 CALL    DWORD PTR DS:[EDI*4+4CAEC8]
004486D8  |> 83C4 08                         ADD     ESP, 8
004486DB  |. B8 01000000                     MOV     EAX, 1

*/


int iNPCSelectingAttackPostMoveAfterID = -1;		// 0:攻撃 1:突撃
int iBushouIdOfNPCSelectingAttackPostMoveAfter = -1;
int iTargetIdOfNPCSelectingAttackPostMoveAfter = -1;

// 0:攻撃 1:騎馬 2:鉄砲 3:大砲 4:一斉攻撃
void OnTenshouExeNPCSelectingAttackPostMoveAfterInBattleExecute() {
	// OutputDebugStream("%d", iSelectingAttackMenuID );
	int iBushouID = (iBushouIdOfNPCSelectingAttackPostMoveAfter & 0xFFFF)-1;
	int iTargetID = (iTargetIdOfNPCSelectingAttackPostMoveAfter & 0xFFFF)-1;

	// ここはイベントハンドラ―を呼ばなくて良い。(なぜならここは行動時のメッセージダイアログが出た後だから。
	// もっと早いタイミングで別の場所で呼び出している。

}

/*
004486D0  |. 56                              PUSH    ESI
004486D1  |. FF14BD C8AE4C00                 CALL    DWORD PTR DS:[EDI*4+4CAEC8]
				└この行を JMP TSMod.OnTSExeNPCSelectingAttackPostMoveAfterInBattle と書き換えてやる実際の処理
004486D8  |> 83C4 08                         ADD     ESP, 8
004486DB  |. B8 01000000                     MOV     EAX, 1

*/
int pTenshouExeJumpFromToOnTenshouExeNPCSelectingAttackPostMoveAfterInBattle	 =0x4486D1; // 関数はこのアドレスから、OnTenshouExeNPCSelectingAttackPostMoveAfterInBattleへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeNPCSelectingAttackPostMoveAfterInBattle  =0x4486D8; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeNPCSelectingAttackPostMoveAfterInBattle() {
	// スタックにためておく
	__asm {

		mov iNPCSelectingAttackPostMoveAfterID, edi
		mov iBushouIdOfNPCSelectingAttackPostMoveAfter, esi // 自分の武将番号
		mov iTargetIdOfNPCSelectingAttackPostMoveAfter, ebx // 相手の武将番号

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
	OnTenshouExeNPCSelectingAttackPostMoveAfterInBattleExecute();

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
		CALL    DWORD PTR DS:[EDI*4+0x4CAEC8]

		jmp pTenshouExeReturnLblFromOnTenshouExeNPCSelectingAttackPostMoveAfterInBattle
	}
}

char cmdOnTenshouExeJumpFromNPCSelectingAttackPostMoveAfterInBattle[8]="\xE9\x90\x90\x90\x90\x90\x90";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeNPCSelectingAttackPostMoveAfterInBattle() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeNPCSelectingAttackPostMoveAfterInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeNPCSelectingAttackPostMoveAfterInBattle + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeNPCSelectingAttackPostMoveAfterInBattle  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromNPCSelectingAttackPostMoveAfterInBattle+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeNPCSelectingAttackPostMoveAfterInBattle), cmdOnTenshouExeJumpFromNPCSelectingAttackPostMoveAfterInBattle, 7, NULL); //7バイトのみ書き込む
}
