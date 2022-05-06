#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


/*
 * 戦闘時に技能の選択時
 */

// 0 = 一喝, 1 = 徴兵, 2 = 混乱, 3 = 暗殺, 4 = 煽動, 5 = 挑発
int iSelectingAlibityMenuID = -1;  // 選択された技能ID

void OnTenshouExeSelectingAbilityMenuInBattleExecute() {
	; // OutputDebugStream("%d", iSelectingAlibityMenuID );
}


/*
0041BDCD  |. EB 02               JMP     SHORT TENSHOU.0041BDD1
0041BDCF  |> 8BD1                MOV     EDX, ECX
0041BDD1  |> FF75 08             PUSH    DWORD PTR SS:[EBP+8]
				└この2行を JMP TSMod.OnTSExeSelectingAbilityMenuInBattle と書き換えてやる実際の処理
0041BDD4  |. FF1495 68C84B00     CALL    DWORD PTR DS:[EDX*4+4BC868]
0041BDDB  |. 83C4 04             ADD     ESP, 4
*/
int pTenshouExeJumpFromToOnTenshouExeSelectingAbilityMenuInBattle	  =0x41BDCF; // 関数はこのアドレスから、OnTenshouExeSelectingAbilityMenuInBattleへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeSelectingAbilityMenuInBattle  =0x41BDD4; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeSelectingAbilityMenuInBattle() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		mov edx, ecx
		push dword ptr ss:[ebp+8]

		// 選択したコマンドIDをコピーしておく
		mov iSelectingAlibityMenuID, ecx

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
	OnTenshouExeSelectingAbilityMenuInBattleExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeSelectingAbilityMenuInBattle
	}
}

/*
0041BDCD  |. EB 02               JMP     SHORT TENSHOU.0041BDD1
0041BDCF  |> 8BD1                MOV     EDX, ECX
0041BDD1  |> FF75 08             PUSH    DWORD PTR SS:[EBP+8]
				└この2行を JMP TSMod.OnTSExeSelectingAbilityMenuInBattle と書き換えてやる実際の処理
0041BDD4  |. FF1495 68C84B00     CALL    DWORD PTR DS:[EDX*4+4BC868]
0041BDDB  |. 83C4 04             ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromSelectingAbilityMenuInBattle[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeSelectingAbilityMenuInBattle() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeSelectingAbilityMenuInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSelectingAbilityMenuInBattle + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeSelectingAbilityMenuInBattle  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromSelectingAbilityMenuInBattle+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSelectingAbilityMenuInBattle), cmdOnTenshouExeJumpFromSelectingAbilityMenuInBattle, 5, NULL); //5バイトのみ書き込む
}




