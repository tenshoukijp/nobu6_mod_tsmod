#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"

#include "MakeIsseikougekiMembers.h"

/*
0046AA14  |. 50             PUSH    EAX
0046AA15  |. FF14BD 20D54C0>CALL    DWORD PTR DS:[EDI*4+4CD520]
0046AA1C  |. 83C4 08        ADD     ESP, 8
0046AA1F  |> 5F             POP     EDI
*/



/*
 * 戦闘時に技能の選択時
 */
extern void OnSelectingAttackEventHandlerInBattle(int iBushouID, int iTargetID, int iSelectActionID, BOOL IsMoveBefore);

int iSelectingAttackMenuID = -1;  // 選択された技能ID 0:攻撃 1:突撃 2:鉄砲 3:大砲 4:一斉攻撃
int iBushouIdOfSelectingAttack = -1;
int iTargetIdOfSelectingAttack = -1;

// 0:攻撃 1:騎馬 2:鉄砲 3:大砲 4:一斉攻撃
void OnTenshouExeSelectingAttackMenuInBattleExecute() {
	; // OutputDebugStream("%d", iSelectingAttackMenuID );
	int iBushouID = (iBushouIdOfSelectingAttack & 0xFFFF)-1;
	int iTargetID = (iTargetIdOfSelectingAttack & 0xFFFF)-1;

	OnSelectingAttackEventHandlerInBattle(iBushouID, iTargetID, iSelectingAttackMenuID, TRUE);
	// 一斉攻撃だったら
	if ( iSelectingAttackMenuID == 4 ) {
		// 巻き添えメンバを構築
		MakeIsseikougekiMembers(iBushouID, iTargetID);

		/*
		for each(int i in vListIsseiKougekiMembers) {
			OutputDebugStream << nb6bushouname[i].fastname << endl;
		}
		OutputDebugStream << "------------" << endl;
		*/
	}
}


/*
0046AA14  |. 50             PUSH    EAX
0046AA15  |. FF14BD 20D54C00       CALL    DWORD PTR DS:[EDI*4+4CD520]
				└この行を JMP TSMod.OnTSExeSelectingAttackMenuInBattle と書き換えてやる実際の処理
0046AA1C  |. 83C4 08        ADD     ESP, 8
0046AA1F  |> 5F             POP     EDI
*/
int pTenshouExeJumpFromToOnTenshouExeSelectingAttackMenuInBattle	 =0x46AA15; // 関数はこのアドレスから、OnTenshouExeSelectingAttackMenuInBattleへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeSelectingAttackMenuInBattle  =0x46AA1C; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeSelectingAttackMenuInBattle() {
	// スタックにためておく
	__asm {
		mov iSelectingAttackMenuID, edi // 選択されたＩＤを保存しておく

		mov iBushouIdOfSelectingAttack, eax	// 実行した武将
		mov iTargetIdOfSelectingAttack, esi // 作用対象の武将

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
	OnTenshouExeSelectingAttackMenuInBattleExecute();

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
		call DWORD PTR DS:[EDI*4+0x4CD520]

		jmp pTenshouExeReturnLblFromOnTenshouExeSelectingAttackMenuInBattle
	}
}

char cmdOnTenshouExeJumpFromSelectingAttackMenuInBattle[8]="\xE9\x90\x90\x90\x90\x90\x90";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeSelectingAttackMenuInBattle() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeSelectingAttackMenuInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSelectingAttackMenuInBattle + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeSelectingAttackMenuInBattle  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromSelectingAttackMenuInBattle+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSelectingAttackMenuInBattle), cmdOnTenshouExeJumpFromSelectingAttackMenuInBattle, 7, NULL); //7バイトのみ書き込む
}




