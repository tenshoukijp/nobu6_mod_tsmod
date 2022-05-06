
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXBushou_All.h"
#include "ScenarioMod.h"
#include "TSModCommand.h"


static int iEAXOfShowingIe1 = -1;
static int iBushouBangouOfShowingIe1 = -1;
void OnTenshouExeShowingIe1Execute() {

	int iBushouID = (iBushouBangouOfShowingIe1 & 0xFFFF) - 1; // 下半分が武将番号

	if (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		if (p_snOnViewingIe) {
			bool isUpdate = p_snOnViewingIe(iBushouID+1, (char *)iEAXOfShowingIe1);
			// OutputDebugStream << nb6bushouname[iBushouID].fastname << endl;
			// OutputDebugStream << (int)isUpdate << endl;
		}

	}
}

/*
0040D226  |. 68 ACB84B00    PUSH    TENSHOU.004BB8AC                 ;  ASCII "家"
0040D22B  |. 56             PUSH    ESI
0040D22C  |. E8 DCFF0900    CALL    TENSHOU.004AD20D					// EDIの下位ビットがEDI 004D00BD TENSHOU.004D00BD
0040D231  |. 83C4 08        ADD     ESP, 8
*/

int pTenshouExeJumpFromToOnTenshouExeShowingIe1     = 0x40D22C; // 関数はこのアドレスから、OnTenshouExeShowingIe1へとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeShowingIe1 = 0x4AD20D; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeShowingIe1  = 0x40D231; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec(naked) void WINAPI OnTenshouExeShowingIe1() {
	// スタックにためておく
	__asm {
		// ここが文字列の書き換え先
		mov iEAXOfShowingIe1, EAX;

		// EDIの下位に武将の番号。
		mov iBushouBangouOfShowingIe1, EDI;

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeShowingIe1

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
	OnTenshouExeShowingIe1Execute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingIe1
	}
}

char cmdOnTenshouExeJumpFromShowingIe1[6] = "\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計８バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeShowingIe1() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeShowingIe1;
	int SubAddress = iAddress - (pTenshouExeJumpFromToOnTenshouExeShowingIe1 + 5);
	// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromShowingIe1 + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory(CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingIe1), cmdOnTenshouExeJumpFromShowingIe1, 5, NULL); //5バイトのみ書き込む
}













static int iEAXOfShowingIe2 = -1;
static int iBushouBangouOfShowingIe2 = -1;
void OnTenshouExeShowingIe2Execute() {

	int iBushouID = (iBushouBangouOfShowingIe2 & 0xFFFF) - 1; // 下半分が武将番号

	if (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {
		if (p_snOnViewingIe) {
			bool isUpdate = p_snOnViewingIe(iBushouID+1, (char *)iEAXOfShowingIe2);
			// OutputDebugStream << nb6bushouname[iBushouID].fastname << endl;
			// OutputDebugStream << (int)isUpdate << endl;
		}
	}
}

/*
0040D2A6  |. 68 ACB84B00    PUSH    TENSHOU.004BB8AC                 ;  ASCII "家"
0040D2AB  |. 57             PUSH    EDI
0040D2AC  |. E8 5CFF0900    CALL    TENSHOU.004AD20D                    // EBPの下位ビットがEBP 001900BD TENSHOU.001900BD
0040D2B1  |. 83C4 08        ADD     ESP, 8
*/

int pTenshouExeJumpFromToOnTenshouExeShowingIe2     = 0x40D2AC; // 関数はこのアドレスから、OnTenshouExeShowingIe2へとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeShowingIe2 = 0x4AD20D; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeShowingIe2  = 0x40D2B1; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec(naked) void WINAPI OnTenshouExeShowingIe2() {
	// スタックにためておく
	__asm {

		// ここが文字列の書き換え先
		mov iEAXOfShowingIe2, EAX;

		// EDIの下位に武将の番号。
		mov iBushouBangouOfShowingIe2, EBP;

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeShowingIe2

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
	OnTenshouExeShowingIe2Execute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingIe2
	}
}

char cmdOnTenshouExeJumpFromShowingIe2[6] = "\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計８バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeShowingIe2() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeShowingIe2;
	int SubAddress = iAddress - (pTenshouExeJumpFromToOnTenshouExeShowingIe2 + 5);
	// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromShowingIe2 + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory(CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingIe2), cmdOnTenshouExeJumpFromShowingIe2, 5, NULL); //5バイトのみ書き込む
}




