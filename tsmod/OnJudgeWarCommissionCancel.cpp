#include "WinTarget.h"

#include "CommonGlobalHandle.h"


/*
004103B5  |. E8 F82B0100    |CALL    TENSHOU.00422FB2
004103BA  |. 85C0           |TEST    EAX, EAX
004103BC  |. 74 23          |JE      SHORT TENSHOU.004103E1
*/

/*
 メイン中のデモキャンセル
 */


int isJudgeMainCommissionCancel = 0;

void OnTenshouExeJudgeMainCommissionCancelExecute() {
	// 天翔記がフォアグラウンドであり、ESCキーを押した時
	if ( GetForegroundWindow()==CommonGlobalHandle::tenshouWND && GetKeyState(VK_ESCAPE) < 0) {
		// 委任解除
		isJudgeMainCommissionCancel = 1;
	}
} 
/* 
004103B5  |. E8 F82B0100    |CALL    TENSHOU.00422FB2
004103BA  |. 85C0           |TEST    EAX, EAX
004103BC  |. 74 23          |JE      SHORT TENSHOU.004103E1
*/ 
int pTenshouExeJumpFromToOnTenshouExeJudgeMainCommissionCancel    =0x4103B5; 
int pTenshouExeJumpCallFromToOnTenshouExeJudgeMainCommissionCancel=0x422FB2; 
int pTenshouExeReturnLblFromOnTenshouExeJudgeMainCommissionCancel =0x4103BA; 
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeJudgeMainCommissionCancel() { 
	__asm { 

		// TENSHOU.EXE内のcall
		call pTenshouExeJumpCallFromToOnTenshouExeJudgeMainCommissionCancel

		mov isJudgeMainCommissionCancel, eax	// 結果を保存

		push eax 
		push ebx 
		push ecx 
		push edx 
		push esp 
		push ebp 
		push esi 
		push edi 
	} 

	OnTenshouExeJudgeMainCommissionCancelExecute(); 

	__asm { 
		pop edi 
		pop esi 
		pop ebp 
		pop esp 
		pop edx 
		pop ecx 
		pop ebx 
		pop eax 

		mov eax, isJudgeMainCommissionCancel	// 結果を上書き

		jmp pTenshouExeReturnLblFromOnTenshouExeJudgeMainCommissionCancel 
	} 
} 
char cmdOnTenshouExeJumpFromJudgeMainCommissionCancel[6]="\xE9"; 
void WriteAsmJumperOnTenshouExeJudgeMainCommissionCancel() { 
	int iAddress = (int)OnTenshouExeJudgeMainCommissionCancel; 
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeJudgeMainCommissionCancel + 5 ) ; 
	memcpy(cmdOnTenshouExeJumpFromJudgeMainCommissionCancel+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。 
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeJudgeMainCommissionCancel), cmdOnTenshouExeJumpFromJudgeMainCommissionCancel, 5, NULL); // 一応安全のため 
} 


