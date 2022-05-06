#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"

// 戦争時の徴兵コマンドを選んだ際に、なぜか兵士が100にフィルターされてしまう関数がある。
// それが呼ばれた時の処理




// 徴兵後の予定の兵力
int iSoldierNewDraftedValue = 0;

// 徴兵前の兵力
int iSoldierOrgDraftedValue = 0;

// 徴兵した対象の武将ID(配列上ではなく、属性の値である(配列より+1多い数))
int iDraftedBushouID = 0;



void OnTenshouExeSoldierDraftFilterInBattleExecute() {
	/*
		iSoldierOrgDraftedValue,iSoldierNewDraftedValue,(iDraftedBushouID-1)で
		何か判定したり？ 士気を特別に増やしたり？
	 */
}


/*
00433F33  |. 56             PUSH    ESI
00433F34  |. E8 A25CFDFF    CALL    TENSHOU.00409BDB
				└ここを JMP TSMod.OnTSExeSoldierDraftFilterInBattle と書き換えてやる実際の処理
00433F39  |. 83C4 08        ADD     ESP, 8
*/
int pTenshouExeJumpFromToOnTenshouExeSoldierDraftFilterInBattle		=0x433F34; // 関数はこのアドレスから、OnTenshouExeSoldierDraftFilterInBattleへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeSoldierDraftFilterInBattle =0x409BDB; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeSoldierDraftFilterInBattle	=0x433F39; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeSoldierDraftFilterInBattle() {
	// スタックにためておく
	__asm {
		// このecxの値が目的の値なので保存
		mov iSoldierNewDraftedValue, ecx

		// このebxの値が目的の値なので保存
		mov iSoldierOrgDraftedValue, ebx

		// 対象の武将ID
		mov iDraftedBushouID, esi

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 徴兵時に何かするならば…
	OnTenshouExeSoldierDraftFilterInBattleExecute();

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

		// 徴兵後の値のみ改変の可能性があるのでコピー
		mov ecx, iSoldierNewDraftedValue

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeSoldierDraftFilterInBattle

		jmp pTenshouExeReturnLblFromOnTenshouExeSoldierDraftFilterInBattle
	}
}

/*
00433F33  |. 56             PUSH    ESI
00433F34  |. E8 A25CFDFF    CALL    TENSHOU.00409BDB
				└ここを JMP TSMod.OnTSExeSoldierDraftFilterInBattle と書き換えてやる実際の処理
00433F39  |. 83C4 08        ADD     ESP, 8
00433F3C  |. FF75 F8        PUSH    DWORD PTR SS:[EBP-8]
*/
char cmdOnTenshouExeJumpFromSoldierDraftFilterInBattle[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeSoldierDraftFilterInBattle() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeSoldierDraftFilterInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSoldierDraftFilterInBattle + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeSoldierDraftFilterInBattle  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromSoldierDraftFilterInBattle+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSoldierDraftFilterInBattle), cmdOnTenshouExeJumpFromSoldierDraftFilterInBattle, 5, NULL); //5バイトのみ書き込む
}




