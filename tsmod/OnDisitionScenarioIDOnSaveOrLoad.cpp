#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "SetBushouAvgParam.h"

//-------------------------------------------------------------------------------------

// 選択しているシナリオ番号
extern int iSelectingScenarioLocation;
extern int iSelectingScenarioID;

/*

00422D72   MOVZX   ECX, WORD PTR DS:[EAX*2+4D3718]
00427B6A   MOVZX   EAX, WORD PTR DS:[EDI*2+4D3718]
00436F98   MOVZX   EAX, WORD PTR DS:[EBP*2+4D3718]
00478A21   MOVZX   EAX, WORD PTR DS:[EAX*2+4D3718]

004D3718  00 00 02 00 03 00 04 00 01 00 05 00              .......　← シナリオのLocationとシナリオIDの対応表


00478A29  |. FF3485 28374D0>PUSH    DWORD PTR DS:[EAX*4+4D3728]                          ;  TENSHOU.004D36A4　ここのEAXはシナリオ番号

00479A71   . E8 8BFEFFFF    CALL    TENSHOU.00479901
00479A76   . 85C0           TEST    EAX, EAX												EAXが１だったら、ロードしたということ。
00479A78   . 74 0F          JE      SHORT TENSHOU.00479A89
*/

// 本当にセーブやロードで「実行」を押したか？
BOOL isExecuteOfOnSaveOrLoad = FALSE;

void OnTenshouExeDecisionScenarioIDOnSaveOrLoadExecute() {

	if ( isExecuteOfOnSaveOrLoad ) {
		// 選択したシナリオ
		byte *iLoadScenario = (byte *)GAMEDATASTRUCT_SAVE_OR_LOAD_SCENARIO_ID_ADDRESS;
		iSelectingScenarioID = *iLoadScenario;

		// シナリオはちゃんと選択された時
		if ( 0 <= iSelectingScenarioID && iSelectingScenarioID <= 5 ) {
			// 選択シナリオロケーションを設定
			if ( iSelectingScenarioID == 0 ) {
				iSelectingScenarioLocation = 0;
			} else if ( iSelectingScenarioID == 1 ) {
				iSelectingScenarioLocation = 2;
			} else if ( iSelectingScenarioID == 2 ) {
				iSelectingScenarioLocation = 3;
			} else if ( iSelectingScenarioID == 3 ) {
				iSelectingScenarioLocation = 4;
			} else if ( iSelectingScenarioID == 4 ) {
				iSelectingScenarioLocation = 1;
			} else if ( iSelectingScenarioID == 5 ) {
				iSelectingScenarioLocation = 5;
			} else {
				iSelectingScenarioLocation = -1;
			}
		}
	}
}


/*
00479A71   . E8 8BFEFFFF    CALL    TENSHOU.00479901
				└ここを JMP TSMod.OnTSExeDecisionScenarioIDOnSaveOrLoad と書き換えてやる実際の処理
00479A76   . 85C0           TEST    EAX, EAX												EAXが１だったら、ロードしたということ。
00479A78   . 74 0F          JE      SHORT TENSHOU.00479A89
*/
int pTenshouExeJumpFromToOnTenshouExeDecisionScenarioIDOnSaveOrLoad		=0x479A71; // 関数はこのアドレスから、OnTenshouExeDecisionScenarioIDOnSaveOrLoadへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeDecisionScenarioIDOnSaveOrLoad =0x479901; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeDecisionScenarioIDOnSaveOrLoad	=0x479A76; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeDecisionScenarioIDOnSaveOrLoad() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeDecisionScenarioIDOnSaveOrLoad

		mov isExecuteOfOnSaveOrLoad, eax // EAXの答えが1なら本当に実行された。

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// ゲーム内画面でロード時に何かするならば…
	OnTenshouExeDecisionScenarioIDOnSaveOrLoadExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeDecisionScenarioIDOnSaveOrLoad
	}
}

char cmdOnTenshouExeJumpFromDecisionScenarioIDOnSaveOrLoad[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeDecisionScenarioIDOnSaveOrLoad() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeDecisionScenarioIDOnSaveOrLoad;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeDecisionScenarioIDOnSaveOrLoad + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeDecisionScenarioIDOnSaveOrLoad  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromDecisionScenarioIDOnSaveOrLoad+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeDecisionScenarioIDOnSaveOrLoad), cmdOnTenshouExeJumpFromDecisionScenarioIDOnSaveOrLoad, 5, NULL); //5バイトのみ書き込む
}




