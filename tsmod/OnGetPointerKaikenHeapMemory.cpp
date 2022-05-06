#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

/*
 各武将の会見系のデータが展開されているヒープメモリのアドレスを得る
 */

/*
00470292  |. 56             PUSH    ESI
00470293  |. E8 B6E8FFFF    CALL    TENSHOU.0046EB4E
00470298  |. 0FB678 03      MOVZX   EDI, BYTE PTR DS:[EAX+3]	← このEAXの位置が対象のヒープの
0047029C  |. 83C4 04        ADD     ESP, 4
*/

/*
 * 会見系のデータが展開されているヒープメモリのアドレスを得る
 */
int iAddressOfGetPointerKaikenHeapMemoryExecute = NULL;
void OnTenshouExeGetPointerKaikenHeapMemoryExecute() {
}


/*
00470292  |. 56             PUSH    ESI
00470293  |. E8 B6E8FFFF    CALL    TENSHOU.0046EB4E
				└ここを JMP TSMod.OnTSExeGetPointerKaikenHeapMemory と書き換えてやる実際の処理
00470298  |. 0FB678 03      MOVZX   EDI, BYTE PTR DS:[EAX+3]	← このEAXの位置が対象のヒープの
0047029C  |. 83C4 04        ADD     ESP, 4
*/
int pTenshouExeJumpFromToOnTenshouExeGetPointerKaikenHeapMemory		=0x470293; // 関数はこのアドレスから、OnTenshouExeGetPointerKaikenHeapMemoryへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeGetPointerKaikenHeapMemory =0x46EB4E; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeGetPointerKaikenHeapMemory	=0x470298; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeGetPointerKaikenHeapMemory() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeGetPointerKaikenHeapMemory

		mov iAddressOfGetPointerKaikenHeapMemoryExecute, eax; // 取得した会見のヒープメモリの位置を得る

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
	OnTenshouExeGetPointerKaikenHeapMemoryExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeGetPointerKaikenHeapMemory
	}
}

/*
00419DED   . 50             PUSH    EAX                              ; |Arg1
00419DEE   . E8 B843FFFF    CALL    TENSHOU.0040E1AB                 ; \TENSHOU.0040E1AB  ここのCallで披露で選択武将リスト一覧になりえる武将リストが、4DBD40に作られている
				└ここを JMP TSMod.OnTSExeGetPointerKaikenHeapMemory と書き換えてやる実際の処理
00419DF3   . 83C4 18        ADD     ESP, 18
*/
char cmdOnTenshouExeJumpFromGetPointerKaikenHeapMemory[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeGetPointerKaikenHeapMemory() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeGetPointerKaikenHeapMemory;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeGetPointerKaikenHeapMemory + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGetPointerKaikenHeapMemory  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromGetPointerKaikenHeapMemory+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeGetPointerKaikenHeapMemory), cmdOnTenshouExeJumpFromGetPointerKaikenHeapMemory, 5, NULL); //5バイトのみ書き込む
}




