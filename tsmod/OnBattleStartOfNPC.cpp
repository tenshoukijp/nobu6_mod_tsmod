#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"




/*
 * ＮＰＣのみで争がはじまるちょっと前。攻められた城番号を確保するためのみでここをフック
 */

// 戦闘において、攻め込まれた側の城番号(城配列番号+1の番号)
extern int iTargetCastleIDInStartOfNPCExecute;


/*
0043CC3B  |. 66:A1 38B64C00 MOV     AX, WORD PTR DS:[4CB638]
0043CC41  |. 50             PUSH    EAX
0043CC42  |. E8 C00B0300    CALL    TENSHOU.0046D807
				└ここを JMP TSMod.OnTSExeBattleStartOfNPCPrevious と書き換えてやる実際の処理
0043CC47  |. 83C4 04        ADD     ESP, 4
*/
int pTenshouExeJumpFromToOnTenshouExeBattleStartOfNPCPrevious		=0x43CC42; // 関数はこのアドレスから、OnTenshouExeBattleStartOfNPCPreviousへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeBattleStartOfNPCPrevious   =0x46D807; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeBattleStartOfNPCPrevious	=0x43CC47; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeBattleStartOfNPCPrevious() {
	// スタックにためておく
	__asm {
		// 攻められた城番号を保持しておく。
		mov iTargetCastleIDInStartOfNPCExecute, eax

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeBattleStartOfNPCPrevious

		jmp pTenshouExeReturnLblFromOnTenshouExeBattleStartOfNPCPrevious
	}
}

/*
0043CC47  |. 83C4 04        ADD     ESP, 4
0043CC4A  |. 50             PUSH    EAX                              ; |Arg1
0043CC4B  |. E8 9E220300    CALL    TENSHOU.0046EEEE                 ; \TENSHOU.0046EEEE
				└ここを JMP TSMod.OnTSExeBattleStartOfNPCPrevious と書き換えてやる実際の処理
0043CC50  |. 83C4 0C        ADD     ESP, 0C
*/
char cmdOnTenshouExeJumpFromBattleStartOfNPCPrevious[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeBattleStartOfNPCPrevious() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeBattleStartOfNPCPrevious;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeBattleStartOfNPCPrevious + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeBattleStartOfNPCPrevious  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromBattleStartOfNPCPrevious+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeBattleStartOfNPCPrevious), cmdOnTenshouExeJumpFromBattleStartOfNPCPrevious, 5, NULL); //5バイトのみ書き込む
}



//-------------------------------------------------------------------------------------------------------------




/*
 * ＮＰＣのみで争がはじまった
 */
// これは必ず戦争開始時に呼ぶこと
// 全ての武将の保存兵数のリセット
extern void EXServiceExecuteRegeneration_All_SoldierResetInBattle();
// 全ての武将の保存兵数の再計算保存
extern void EXServiceExecuteRegeneration_All_SoldierSaveInBattle();
// 保存兵数から、増加兵数を再計算
extern void EXServiceExecuteRegeneration_All_SoldierCalcInBattle();

// 戦争が終わる時に強制的に-1に設定すること
extern int iCurIncrementTurnInBattle;
extern int iPreIncrementTurnInBattle;
extern BOOL isMustCallOfDecrementTurnInBatttle;

// 全ての武将の戦場での現在のフォーム形態
extern int iCurTekkousenForm[];

// 初配置時に「行動済み」となる部分の対処。
extern bool arrIsFirstHaichiAction[];

// 戦闘において、攻め込まれた側の城番号(城配列番号+1の番号)
int iTargetCastleIDInStartOfNPCExecute = -1;

// NPCとPC共通で、都合の良いパラメータで呼び出す。
extern void OnTenshouExeBattleStartExecute(int iRecieveCastleID );

void OnTenshouExeBattleStartOfNPCExecute() {
	// 戦争開始
	CommonGlobalHandle::isBattleMode = 1;

	// 戦争開始時に全ての武将の戦場での現在のフォーム形態を-1としておく
	for ( int ix=0; ix<GAMEDATASTRUCT_BUSHOU_NUM; ix++) {
		iCurTekkousenForm[ix]=-1;
	}

	// 戦争開始時に全ての初配置を0にしておく
	for ( int ix=0; ix<GAMEDATASTRUCT_BUSHOU_NUM; ix++ ) {
		arrIsFirstHaichiAction[ix] = 0;
	}

	// 戦争を見ていない場合、熟練値が異常に上がるのを防ぐための処理。
	// 戦闘開始時に、値を保存しておく。
	EXServiceExecuteBushouValueControllerOfStartBattle();


	// ★他にもういっかしょPlayerの場所でも全く同じ計算が必要なので注意!!
	// 全ての武将の保存兵数のリセット
	EXServiceExecuteRegeneration_All_SoldierResetInBattle();
	//  全ての武将の保存兵数の再計算保存
	EXServiceExecuteRegeneration_All_SoldierSaveInBattle();
	// 保存兵数から、増加兵数を再計算
	EXServiceExecuteRegeneration_All_SoldierCalcInBattle();

	// 敵武将なら、戦闘中全ての技能が使える(パラメタIFの一種)
	EXSuperService_IfCanAllAlibityInBattleStart();

	// 都合の良いパラメータにした駆動関数を呼び出す。
	// OutputDebugStream("%d", iTargetCastleIDInStartOfNPCExecute);
	OnTenshouExeBattleStartExecute(iTargetCastleIDInStartOfNPCExecute-1 );

}

void OnTenshouExeBattleEndOfNPCExecute() {
	// 敵武将なら、戦闘中全ての技能が使える(パラメタIFの一種)
	EXSuperService_IfCanAllAlibityInBattleEnd();
}

/*
0043CC53  |. E8 F13C0600    CALL    TENSHOU.004A0949						
				└ここを JMP TSMod.OnTSExeBattleStartOfNPC と書き換えてやる実際の処理
0043CC58  |. 57             PUSH    EDI						
*/
int pTenshouExeJumpFromToOnTenshouExeBattleStartOfNPC	    =0x43CC53; // 関数はこのアドレスから、OnTenshouExeBattleStartOfNPCへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeBattleStartOfNPC   =0x4A0949; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeBattleStartOfNPC    =0x43CC58; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w

// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeBattleStartOfNPC() {
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
	OnTenshouExeBattleStartOfNPCExecute();

	__asm {

		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		// 元々TENSHOU.EXE内にあった記述そのまま
		call  pTenshouExeJumpCallFromToOnTenshouExeBattleStartOfNPC
	}

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
	OnTenshouExeBattleEndOfNPCExecute();

	__asm {

		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		// 戦争終わり
		mov CommonGlobalHandle::isBattleMode, 0

		// 戦争終わったのでターンを-1に
		mov iCurIncrementTurnInBattle, -1
		mov iPreIncrementTurnInBattle, -1
		mov isMustCallOfDecrementTurnInBatttle, 0

		jmp pTenshouExeReturnLblFromOnTenshouExeBattleStartOfNPC
	}
}

/*
0043CC53  |. E8 F13C0600    CALL    TENSHOU.004A0949						
				└ここを JMP TSMod.OnTSExeBattleStartOfNPC と書き換えてやる実際の処理
0043CC58  |. 57             PUSH    EDI						
*/
char cmdOnTenshouExeJumpFromBattleStartOfNPC[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeBattleStartOfNPC() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeBattleStartOfNPC;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeBattleStartOfNPC + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeBattleStartOfNPC  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromBattleStartOfNPC+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeBattleStartOfNPC), cmdOnTenshouExeJumpFromBattleStartOfNPC, 5, NULL); //5バイトのみ書き込む
}




//----------------------------------------------------------------------------------------------


/*
 * ＮＰＣのみで争が終わったちょっと後
 */

void OnTenshouExeBattleAfterOfNPCExecute() {
	// 戦争を見ていない場合、熟練値が異常に上がるのを防ぐための処理。
	// 戦闘終了後に、値を比較して制御する。
	EXServiceExecuteBushouValueControllerOfEndBattle();

	// ScenarioModの戦争終了時のメソッド呼出し。
	if ( p_snOnBattleEnd ) {
		((PFNSNONBATTLEEND) p_snOnBattleEnd)();
	}
}

/*
0043CC65  |. 56             PUSH    ESI
0043CC66  |. E8 72770100    CALL    TENSHOU.004543DD
				└ここを JMP TSMod.OnTSExeBattleAfterOfNPC と書き換えてやる実際の処理
0043CC6B  |. 83C4 04        ADD     ESP, 4
0043CC6E  |. 85C0           TEST    EAX, EAX
*/
int pTenshouExeJumpFromToOnTenshouExeBattleAfterOfNPC	    =0x43CC66; // 関数はこのアドレスから、OnTenshouExeBattleAfterOfNPCへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeBattleAfterOfNPC   =0x4543DD; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeBattleAfterOfNPC    =0x43CC6B; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w

// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeBattleAfterOfNPC() {

	// スタックにためておく
	__asm {

		// 元々TENSHOU.EXE内にあった記述そのまま
		call  pTenshouExeJumpCallFromToOnTenshouExeBattleAfterOfNPC

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
	OnTenshouExeBattleAfterOfNPCExecute();

	__asm {

		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		jmp pTenshouExeReturnLblFromOnTenshouExeBattleAfterOfNPC
	}
}


char cmdOnTenshouExeJumpFromBattleAfterOfNPC[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeBattleAfterOfNPC() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeBattleAfterOfNPC;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeBattleAfterOfNPC + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeBattleAfterOfNPC  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromBattleAfterOfNPC+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeBattleAfterOfNPC), cmdOnTenshouExeJumpFromBattleAfterOfNPC, 5, NULL); //5バイトのみ書き込む
}




