#include "WinTarget.h"

#include "CommonGlobalHandle.h"


/*
0046B4A0  |. E8 0D7BFBFF    |CALL    TENSHOU.00422FB2
0046B4A5  |. 85C0           |TEST    EAX, EAX
0046B4A7  |. 74 3C          |JE      SHORT TENSHOU.0046B4E5
*/

/*
 戦争中の委任キャンセル
 */
/*
GetKeyStateの戻り値はどうやら1回押すと-127で離すと0に戻り2回目（連打でかなぁ？）押すと-128そして離すと1に戻っているような気がするんですが、
これとJaritenCatさんの言われる数字ff80などが関係しているのでしょうか？でもff80と-127は同じなんでしょうか？？

 ＞最下位ビットは押して離すたびに0と1が入れ替わりま＞す。（トグルするといいます）
ということはこれが1回目押すと0で2回目押すと1で3回目押すと0というようになっている原因でしょうか。
*/


int isJudgeWarCommissionCancel = 0;

void OnTenshouExeJudgeWarCommissionCancelExecute() {
	// 天翔記がフォアグラウンドであり、ESCキーを押した時
	if ( GetForegroundWindow()==CommonGlobalHandle::tenshouWND && GetKeyState(VK_ESCAPE) < 0) {
		// 委任解除
		isJudgeWarCommissionCancel = 1;
	}
} 
/* 
0046B4A0  |. E8 0D7BFBFF    |CALL    TENSHOU.00422FB2
0046B4A5  |. 85C0           |TEST    EAX, EAX						EAXが1なら委任キャンセル
0046B4A7  |. 74 3C          |JE      SHORT TENSHOU.0046B4E5
*/ 
int pTenshouExeJumpFromToOnTenshouExeJudgeWarCommissionCancel    =0x46B4A0; 
int pTenshouExeJumpCallFromToOnTenshouExeJudgeWarCommissionCancel=0x422FB2; 
int pTenshouExeReturnLblFromOnTenshouExeJudgeWarCommissionCancel =0x46B4A5; 
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeJudgeWarCommissionCancel() { 
	__asm { 

		// TENSHOU.EXE内のcall
		call pTenshouExeJumpCallFromToOnTenshouExeJudgeWarCommissionCancel

		mov isJudgeWarCommissionCancel, eax	// 結果を保存

		push eax 
		push ebx 
		push ecx 
		push edx 
		push esp 
		push ebp 
		push esi 
		push edi 
	} 

	OnTenshouExeJudgeWarCommissionCancelExecute(); 

	__asm { 
		pop edi 
		pop esi 
		pop ebp 
		pop esp 
		pop edx 
		pop ecx 
		pop ebx 
		pop eax 

		mov eax, isJudgeWarCommissionCancel	// 結果を上書き

		jmp pTenshouExeReturnLblFromOnTenshouExeJudgeWarCommissionCancel 
	} 
} 
char cmdOnTenshouExeJumpFromJudgeWarCommissionCancel[6]="\xE9"; 
void WriteAsmJumperOnTenshouExeJudgeWarCommissionCancel() { 
	int iAddress = (int)OnTenshouExeJudgeWarCommissionCancel; 
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeJudgeWarCommissionCancel + 5 ) ; 
	memcpy(cmdOnTenshouExeJumpFromJudgeWarCommissionCancel+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。 
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeJudgeWarCommissionCancel), cmdOnTenshouExeJumpFromJudgeWarCommissionCancel, 5, NULL); // 一応安全のため 
} 


