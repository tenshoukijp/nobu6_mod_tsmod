#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


/*
 * 現在鉄甲船のフォーム状態かどうか。実際には戦場において、現在ユニットがどのような形態かをチェックし保存する
 */
extern int iCurTekkousenForm[];

// 対象武将のフォーム
int iCurTekkousenFormValue = -1;

// フォームをチェックする武将ID
int iCurTekkousenFormBushouID = -1;

void OnTenshouExeCurTekkousenFormCheckExecute() {

	// 武将の範囲チェックをして…
	if ( 0 < iCurTekkousenFormBushouID-1 && iCurTekkousenFormBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM) {

		// 対象武将のフォームを保存しておく
		iCurTekkousenForm[iCurTekkousenFormBushouID-1] = iCurTekkousenFormValue;
	}
}


/*
00499F1B  |. E8 C10FF9FF    CALL    TENSHOU.0042AEE1                 ; \TENSHOU.0042AEE1     ここで鉄甲船状態かをEAXに代入
				└ここを JMP TSMod.OnTSExeCurTekkousenFormCheck と書き換えてやる実際の処理
00499F20  |. 83C4 04        ADD     ESP, 4
00499F23  |. 8BE8           MOV     EBP, EAX
00499F25  |. 83FD 04        CMP     EBP, 4													鉄甲船状態なら～ という感じ
00499F28  |. 75 0E          JNZ     SHORT TENSHOU.00499F38

*/
int pTenshouExeJumpFromToOnTenshouExeCurTekkousenFormCheck	     =0x499F1B; // 関数はこのアドレスから、OnTenshouExeCurTekkousenFormCheckへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeCurTekkousenFormCheck   =0x42AEE1; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeCurTekkousenFormCheck    =0x499F20; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w

// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeCurTekkousenFormCheck() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内で呼び出していた巻数を呼び出し
		call pTenshouExeJumpCallFromToOnTenshouExeCurTekkousenFormCheck

		// 武将IDをコピー
		mov iCurTekkousenFormBushouID, esi

		// 現在のユニットのフォームの状態をコピー(4=鉄甲船、0～3, 足軽,騎馬,鉄砲,騎馬鉄砲,などが来る)
		mov iCurTekkousenFormValue, eax;

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
	OnTenshouExeCurTekkousenFormCheckExecute();

	__asm {

		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		jmp pTenshouExeReturnLblFromOnTenshouExeCurTekkousenFormCheck
	}
}

/*
0040A185     E8 44320A00    CALL    TENSHOU.004AD3CE
				└ここを JMP TSMod.OnTSExeCurTekkousenFormCheck と書き換えてやる実際の処理
0040A18A  |. 0FB7C0         MOVZX   EAX, AX
*/
char cmdOnTenshouExeJumpFromCurTekkousenFormCheck[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeCurTekkousenFormCheck() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeCurTekkousenFormCheck;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeCurTekkousenFormCheck + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeCurTekkousenFormCheck  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromCurTekkousenFormCheck+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeCurTekkousenFormCheck), cmdOnTenshouExeJumpFromCurTekkousenFormCheck, 5, NULL); //5バイトのみ書き込む
}




