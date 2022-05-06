#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "ScenarioMod.h"
#include <map>
using namespace std;

/*
 ６つのシナリオタイトルが表示される時
 */

/*
0049D05B  |. 57                   PUSH    EDI
0049D05C  |. BE 08374D00          MOV     ESI, TENSHOU.004D3708
0049D061  |. BF 28374D00          MOV     EDI, TENSHOU.004D3728
*/



// 関数とchar **のセットを覚えておく
extern map<FARPROC, char **> psnGetProcAdressAndPointerMap;

void OnTenshouExeViewingScenarioTitleListExecute() {

	// ScenarioModの関数を呼び出す
	if ( p_snOnViewingScenarioTitleList ) {
		 p_snOnViewingScenarioTitleList();
	}

	// ここに来るたびに、シナリオ詳細説明文の外部関数とポインタとのマッピングはクリアする。
	psnGetProcAdressAndPointerMap.clear();
}


/*
0049D05B  |. 57                   PUSH    EDI
0049D05C  |. BE 08374D00          MOV     ESI, TENSHOU.004D3708
				└ここを JMP TSMod.OnTSExeViewingScenarioTitleList と書き換えてやる実際の処理
0049D061  |. BF 28374D00          MOV     EDI, TENSHOU.004D3728
*/
int pTenshouExeJumpFromToOnTenshouExeViewingScenarioTitleList	   =0x49D05C; // 関数はこのアドレスから、OnTenshouExeViewingScenarioTitleListへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeViewingScenarioTitleList   =0x49D061; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeViewingScenarioTitleList() {
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
	
	// ゲームデータをプレイヤーターンが変わった段階で読み込み更新しておく
	OnTenshouExeViewingScenarioTitleListExecute();

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

		// 元々tenshou.exeにあった処理をここで
		MOV     ESI, 0x4D3708

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingScenarioTitleList
	}
}

/*
*/
char cmdOnTenshouExeJumpFromViewingScenarioTitleList[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeViewingScenarioTitleList() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeViewingScenarioTitleList;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingScenarioTitleList + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingScenarioTitleList  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromViewingScenarioTitleList+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingScenarioTitleList), cmdOnTenshouExeJumpFromViewingScenarioTitleList, 5, NULL); //5バイトのみ書き込む
}




