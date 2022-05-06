#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "MessageN6PPatch.h"
#include "ScenarioMod.h"

/*
004A0F94   8946 70          MOV     DWORD PTR DS:[ESI+70], EAX
004A0F97   C603 00          MOV     BYTE PTR DS:[EBX], 0
004A0F9A   74 19            JE      SHORT TENSHOU.004A0FB5
*/

int EBXOfRareMessageWriteMemory = -1;

void OnTenshouExeRareMessageWriteMemoryExecute() {
}


/*
004A0F94   8946 70          MOV     DWORD PTR DS:[ESI+70], EAX
004A0F97   C603 00          MOV     BYTE PTR DS:[EBX], 0
				└ここを JMP TSMod.OnTSExeRareMessageWriteMemory と書き換えてやる実際の処理
004A0F9A   74 19            JE      SHORT TENSHOU.004A0FB5
00499038  |. 5E             POP     ESI
*/
int pTenshouExeJumpFromToOnTenshouExeRareMessageWriteMemory		=0x4A0F94; // 関数はこのアドレスから、OnTenshouExeRareMessageWriteMemoryへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeRareMessageWriteMemory	=0x4A0F9A; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeRareMessageWriteMemory() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		MOV     DWORD PTR DS:[ESI+0x70], EAX
		MOV     BYTE PTR DS:[EBX], 0		// ← このEBXが	0x4D91B4 の時、軍師などがしゃべっているので他とはメッセージの位置が異なりLOC3となる。 ASCII "かような手土産では家は洛中の
		mov EBXOfRareMessageWriteMemory, EBX  // EBXを保存
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
	OnTenshouExeRareMessageWriteMemoryExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeRareMessageWriteMemory
	}
}

/*
0049902C  |. 8BCE           MOV     ECX, ESI
0049902E  |. 68 B8334D00    PUSH    TENSHOU.004D33B8                 ;  ASCII "画面サイズを選択してください"
00499033  |. E8 16550100    CALL    TENSHOU.004AE54E
				└ここを JMP TSMod.OnTSExeRareMessageWriteMemory と書き換えてやる実際の処理
00499038  |. 5E             POP     ESI
*/
char cmdOnTenshouExeJumpFromRareMessageWriteMemory[7]="\xE9\x90\x90\x90\x90\x90";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeRareMessageWriteMemory() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeRareMessageWriteMemory;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeRareMessageWriteMemory + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeRareMessageWriteMemory  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromRareMessageWriteMemory+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeRareMessageWriteMemory), cmdOnTenshouExeJumpFromRareMessageWriteMemory, 6, NULL); //6バイトのみ書き込む
}




