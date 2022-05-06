#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"

int iSelectingScenarioLocation = -1;
int iSelectingScenarioID = -1;

/*

00422D72   MOVZX   ECX, WORD PTR DS:[EAX*2+4D3718]
00427B6A   MOVZX   EAX, WORD PTR DS:[EDI*2+4D3718]
00436F98   MOVZX   EAX, WORD PTR DS:[EBP*2+4D3718]
00478A21   MOVZX   EAX, WORD PTR DS:[EAX*2+4D3718]

004D3718  00 00 02 00 03 00 04 00 01 00 05 00              .......　← シナリオのLocationとシナリオIDの対応表
*/


// ６つのシナリオのどれを選択したのか。
/*
1534 0  0
1560 2  1
1571 3  2
1582 4  3
1546 1  4
1599 5  5
*/
void OnTenshouExeSelectingScenarioExecute() {

	// シナリオ選択のモードとなったら、このフラグを立てる
	CommonGlobalHandle::isModeScenario = true;

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


/*
0049D7CE  |> E8 75F8FFFF    |CALL    TENSHOU.0049D048
				└ここを JMP TSMod.OnTSExeSelectingScenario と書き換えてやる実際の処理
0049D7D3  |. 3BC6           |CMP     EAX, ESI
 */
int pTenshouExeJumpFromToOnTenshouExeSelectingScenario    =0x49D7CE; // 関数はこのアドレスから、OnTenshouExeSelectingScenarioへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeSelectingScenario=0x49D048; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeSelectingScenario =0x49D7D3; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeSelectingScenario() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeSelectingScenario
		mov iSelectingScenarioID, eax // 選択したシナリオ番号。順番がバラバラ。選択してない時には0xFFFF

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
	OnTenshouExeSelectingScenarioExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeSelectingScenario
	}
}

/*
0049D7CE  |> E8 75F8FFFF    |CALL    TENSHOU.0049D048
				└ここを JMP TSMod.OnTSExeSelectingScenario と書き換えてやる実際の処理
0049D7D3  |. 3BC6           |CMP     EAX, ESI
*/
char cmdOnTenshouExeJumpFromSelectingScenario[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeSelectingScenario() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeSelectingScenario;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSelectingScenario + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeSelectingScenario  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromSelectingScenario+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSelectingScenario), cmdOnTenshouExeJumpFromSelectingScenario, 5, NULL); //5バイトのみ書き込む
}




