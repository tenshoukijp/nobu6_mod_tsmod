#include "WinTarget.h"

#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

/*
 エンディング後に、強制終了にするか初期設定に戻るかの切替。
 しかし、単純にコールしないといったことをやると、デモモードからユーザー選択しても1回目無視されてしまうので、それも考慮する。
 */

/*
004103DF  |. 74 28          |JE      SHORT TENSHOU.00410409
004103E1  |> E8 4FE80600    |CALL    TENSHOU.0047EC35
004103E6  |. 85C0           |TEST    EAX, EAX
*/

void OnTenshouExeEndingAfterJumperExecute() {
}


/*
004103DF  |. 74 28          |JE      SHORT TENSHOU.00410409
004103E1  |> E8 4FE80600    |CALL    TENSHOU.0047EC35
└ここを JMP TSMod.OnTSExeEndingAfterJumper と書き換えてやる実際の処理
004103E6  |. 85C0           |TEST    EAX, EAX
*/
int pTenshouExeJumpFromToOnTenshouExeEndingAfterJumper		=0x4103E1; // 関数はこのアドレスから、OnTenshouExeEndingAfterJumperへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeEndingAfterJumper  =0x47EC35; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeEndingAfterJumper	=0x4103E6; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeEndingAfterJumper() {
	// スタックにためておく
	__asm {

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeEndingAfterJumper

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// エンディングだよ
	OnTenshouExeEndingAfterJumperExecute();

	if ( CommonGlobalHandle::isEndingMode ) {
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

			mov eax, 0 // エンディングだと1に設定されて、1だと強制終了ようがそうしない

			jmp pTenshouExeReturnLblFromOnTenshouExeEndingAfterJumper
		}
	// エンディングモード以外は、何も変更せず、そのまま戻す
	} else {
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

			jmp pTenshouExeReturnLblFromOnTenshouExeEndingAfterJumper
		}
	}
}

/*
*/
char cmdOnTenshouExeJumpFromEndingAfterJumper[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeEndingAfterJumper() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeEndingAfterJumper;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeEndingAfterJumper + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeEndingAfterJumper  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromEndingAfterJumper+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeEndingAfterJumper), cmdOnTenshouExeJumpFromEndingAfterJumper, 5, NULL); //5バイトのみ書き込む
}




