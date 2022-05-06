#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "MessageN6PPatch.h"


/*
00419D61  |. E8 4FFEFFFF    CALL    TENSHOU.00419BB5　　　　← これが「披露」の中身の関数

８人が何かいう？
00457A39  /$ 56             PUSH    ESI
00457A3A  |. 8BF1           MOV     ESI, ECX
00457A3C  |. E8 C3F4FFFF    CALL    TENSHOU.00456F04
00457A41  |. 8BCE           MOV     ECX, ESI
00457A43  |. E8 D7F5FFFF    CALL    TENSHOU.0045701F
00457A48  |. 8BCE           MOV     ECX, ESI
00457A4A  |. E8 4BF6FFFF    CALL    TENSHOU.0045709A
00457A4F  |. 8BCE           MOV     ECX, ESI
00457A51  |. E8 B8F6FFFF    CALL    TENSHOU.0045710E
00457A56  |. 8BCE           MOV     ECX, ESI
00457A58  |. E8 00FCFFFF    CALL    TENSHOU.0045765D
00457A5D  |. 8BCE           MOV     ECX, ESI
00457A5F  |. E8 11FCFFFF    CALL    TENSHOU.00457675
00457A64  |. 8BCE           MOV     ECX, ESI
00457A66  |. E8 49FFFFFF    CALL    TENSHOU.004579B4
00457A6B  |. 8BCE           MOV     ECX, ESI
00457A6D  |. E8 EFFDFFFF    CALL    TENSHOU.00457861
00457A72  |. 8BCE           MOV     ECX, ESI
00457A74  |. E8 4CFEFFFF    CALL    TENSHOU.004578C5
00457A79  |. 5E             POP     ESI
00457A7A  \. C3             RETN


*/

/*
00456EB1  /$ 8B4424 08      MOV     EAX, DWORD PTR SS:[ESP+8]
00456EB5  |. 85C0           TEST    EAX, EAX
00456EB7  |. 74 36          JE      SHORT TENSHOU.00456EEF
	↓
00456EB1　					JMP		OnTenshouExeHirouFukidashi とする。
00456EB6					NOP(=90)
00456EB7  |. 74 36          JE      SHORT TENSHOU.00456EEF
*/
/*
 * ｢披露｣の吹き出し時
 */

void OnTenshouExeHirouFukidashiExecute() {
	MessageN6PPatch::isOnTenshouExeMessageCopyUsed = false;

	// メモリ⇒CurProcessMemoryのCPM値をセット
	CPM.setVariableFromMemory();

	// 武将名、メッセージ、付属情報
	ReWritePatchMessageN6P( CPM.getCurVarPtr(LOC1), CPM.getCurVarPtr(LOC4), NULL );
}


int pTenshouExeJumpFromToOnTenshouExeHirouFukidashi    =0x456EB1; // 関数はこのアドレスから、OnTenshouExeHirouFukidashiへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeHirouFukidashi =0x456EB7; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeHirouFukidashi() {
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
	OnTenshouExeHirouFukidashiExecute();

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
		mov     eax, dword ptr ss:[esp+8]
		test    eax, eax

// 		call pTenshouExeJumpCallFromToOnTenshouExeHirouFukidashi

		jmp pTenshouExeReturnLblFromOnTenshouExeHirouFukidashi
	}
}


char cmdOnTenshouExeJumpFromHirouFukidashi[7]="\xE9\x90\x90\x90\x90\x90";
// 元の命令が5バイト、最後にNOPを１つ足して、以後の関数で生まれる命令が合計６バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeHirouFukidashi() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeHirouFukidashi;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeHirouFukidashi + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeHirouFukidashi  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromHirouFukidashi+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeHirouFukidashi), cmdOnTenshouExeJumpFromHirouFukidashi, 6, NULL); //6バイトのみ書き込む
}

