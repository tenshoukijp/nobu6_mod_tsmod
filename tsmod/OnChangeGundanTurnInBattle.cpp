#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


// 戦闘中や攻城戦中に、軍団ターンが切り替わる時
/*
0046A6B7   . E8 C59F0300    CALL    TENSHOU.004A4681
0046A6BC   . 83C4 04        ADD     ESP, 4
0046A6BF   . 85C0           TEST    EAX, EAX
0046A6C1   . 75 04          JNZ     SHORT TENSHOU.0046A6C7
0046A6C3   . 33C0           XOR     EAX, EAX
0046A6C5   . EB 52          JMP     SHORT TENSHOU.0046A719
0046A6C7   > 66:8B0D 545D4D>MOV     CX, WORD PTR DS:[4D5D54]　← これ今の軍団ターン
0046A6CE   . 8D4424 0C      LEA     EAX, DWORD PTR SS:[ESP+C]
0046A6D2   . 50             PUSH    EAX
0046A6D3   . 6A 08          PUSH    8
0046A6D5   . 51             PUSH    ECX
0046A6D6   . E8 CC14FEFF    CALL    TENSHOU.0044BBA7
0046A6DB   . 83C4 0C        ADD     ESP, 0C
*/





extern int arrRemainMoveCountAllBushou[GAMEDATASTRUCT_BUSHOU_NUM];


// 戦闘中や攻城戦中に、軍団ターンが切り替わる時
void OnTenshouExeChangeGundanTurnInBattleExecute() {
	// 移動力を若干回復
	for ( int i=0; i < GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_NUM; i++ ) {
		// 現在ターンが終了しようとしている軍団に属する武将について…
		if ( nb6bushou_hex_unit_info[i].junban == nb6gundan_hex_info.junban ) {
			// 名前が長いので…
			int iBushouID = nb6bushou_hex_unit_info[i].bushou - 1;
			if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
				// カスタムな残り行動数を０とする。
				arrRemainMoveCountAllBushou[iBushouID] = 0;
			}
		}
	}
}


/*
0046A6D5   . 51             PUSH    ECX
0046A6D6   . E8 CC14FEFF    CALL    TENSHOU.0044BBA7
				└ここを JMP TSMod.OnTSExeChangeGundanTurnInBattle と書き換えてやる実際の処理
0046A6DB   . 83C4 0C        ADD     ESP, 0C
*/
int pTenshouExeJumpFromToOnTenshouExeChangeGundanTurnInBattle	  =0x46A6D6; // 関数はこのアドレスから、OnTenshouExeChangeGundanTurnInBattleへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeChangeGundanTurnInBattle =0x44BBA7;
int pTenshouExeReturnLblFromOnTenshouExeChangeGundanTurnInBattle  =0x46A6DB; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w

// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeChangeGundanTurnInBattle() {
	// スタックにためておく
	__asm {

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
	OnTenshouExeChangeGundanTurnInBattleExecute();

	__asm {

		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	
		call pTenshouExeJumpCallFromToOnTenshouExeChangeGundanTurnInBattle // TENSHOU.EXEに元々あった処理

		jmp pTenshouExeReturnLblFromOnTenshouExeChangeGundanTurnInBattle
	}
}

/*
0046A6D5   . 51             PUSH    ECX
0046A6D6   . E8 CC14FEFF    CALL    TENSHOU.0044BBA7
				└ここを JMP TSMod.OnTSExeChangeGundanTurnInBattle と書き換えてやる実際の処理
0046A6DB   . 83C4 0C        ADD     ESP, 0C
*/
char cmdOnTenshouExeJumpFromChangeGundanTurnInBattle[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeChangeGundanTurnInBattle() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeChangeGundanTurnInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeChangeGundanTurnInBattle + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeChangeGundanTurnInBattle  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromChangeGundanTurnInBattle+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeChangeGundanTurnInBattle), cmdOnTenshouExeJumpFromChangeGundanTurnInBattle, 5, NULL); //5バイトのみ書き込む
}




