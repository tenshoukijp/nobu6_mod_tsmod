#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"

/*
 籠城時、守備側に何人いるのか(出陣していないのも含める)。
 籠城時の画面に出る｢武将数｣の人数を得ておくため。
 */

int iCheckRojoBushouNum = -1;
void OnTenshouExeCheckRojoBushouNumExecute() {
}

/*
004620A2  |. 56             PUSH    ESI                              ; |Arg3
004620A3  |. 6A 27          PUSH    27                               ; |Arg2 = 00000027
004620A5  |. 57             PUSH    EDI                              ; |Arg1
004620A6  |. E8 62BFFAFF    CALL    TENSHOU.0040E00D                 ; \TENSHOU.0040E00D  ← この関数でESIに(籠城時の画面に出る｢武将数｣の人数が出る)
004620AB  |. 83C4 18        ADD     ESP, 18
*/ 
int pTenshouExeJumpFromToOnTenshouExeCheckRojoBushouNum    =0x4620A6; 
int pTenshouExeJumpCallFromToOnTenshouExeCheckRojoBushouNum=0x40E00D; 
int pTenshouExeReturnLblFromOnTenshouExeCheckRojoBushouNum =0x4620AB; 
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeCheckRojoBushouNum() { 
	__asm { 

		// TENSHOU.EXE内のcall
		call pTenshouExeJumpCallFromToOnTenshouExeCheckRojoBushouNum
		
		// 結果の籠城人数を得ておく。
		mov iCheckRojoBushouNum, esi;

		push eax 
		push ebx 
		push ecx 
		push edx 
		push esp 
		push ebp 
		push esi 
		push edi 
	} 

	OnTenshouExeCheckRojoBushouNumExecute(); 

	__asm { 
		pop edi 
		pop esi 
		pop ebp 
		pop esp 
		pop edx 
		pop ecx 
		pop ebx 
		pop eax 

		jmp pTenshouExeReturnLblFromOnTenshouExeCheckRojoBushouNum  // 使わずジャンプ
	} 
} 
char cmdOnTenshouExeJumpFromCheckRojoBushouNum[6]="\xE9"; 
void WriteAsmJumperOnTenshouExeCheckRojoBushouNum() { 
	int iAddress = (int)OnTenshouExeCheckRojoBushouNum; 
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeCheckRojoBushouNum + 5 ) ; 
	memcpy(cmdOnTenshouExeJumpFromCheckRojoBushouNum+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。 
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeCheckRojoBushouNum), cmdOnTenshouExeJumpFromCheckRojoBushouNum, 5, NULL); // 一応安全のため 
} 


