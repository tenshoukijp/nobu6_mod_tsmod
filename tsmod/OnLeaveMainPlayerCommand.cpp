#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "ScenarioMod.h"

/*
00410693  |.^EB F0          JMP     SHORT TENSHOU.00410685
00410695  |> E8 8F030000    CALL    TENSHOU.00410A29
0041069A  |. 8BC3           MOV     EAX, EBX
*/

void OnLeaveMainPlayerCommand() {

	// ★何かのコマンドから出た際にはここで判定可能
	// OutputDebugStream << "Leave" << endl;

	// ScenarioModで呼出し
	if ( p_snOnLeaveMainPlayerCommand ) {
		p_snOnLeaveMainPlayerCommand();
	}
}





//----------------------------------------------------------------------プレイヤターンから脱却してしまう。(ロードしたり、静観したり、ゲーム終了したり)

void OnTenshouExeLeavePlayerGameExecute() {

	OnLeaveMainPlayerCommand();
}


/*
00410693  |.^EB F0          JMP     SHORT TENSHOU.00410685
00410695  |> E8 8F030000    CALL    TENSHOU.00410A29
				└ここを JMP TSMod.OnTSExeLeavePlayerGame と書き換えてやる実際の処理
0041069A  |. 8BC3           MOV     EAX, EBX
*/
int pTenshouExeJumpFromToOnTenshouExeLeavePlayerGame		=0x410695; // 関数はこのアドレスから、OnTenshouExeLeavePlayerGameへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeLeavePlayerGame    =0x410A29; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeLeavePlayerGame	    =0x41069A; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeLeavePlayerGame() {
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
	OnTenshouExeLeavePlayerGameExecute();

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
		call pTenshouExeJumpCallFromToOnTenshouExeLeavePlayerGame

		jmp pTenshouExeReturnLblFromOnTenshouExeLeavePlayerGame
	}
}

char cmdOnTenshouExeJumpFromLeavePlayerGame[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeLeavePlayerGame() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeLeavePlayerGame;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeLeavePlayerGame + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeLeavePlayerGame  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromLeavePlayerGame+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeLeavePlayerGame), cmdOnTenshouExeJumpFromLeavePlayerGame, 5, NULL); //5バイトのみ書き込む
}


