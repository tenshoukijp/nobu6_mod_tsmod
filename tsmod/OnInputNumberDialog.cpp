#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "ScenarioMod.h"

/*
 * 数値入力ダイアログ、実行ボタン決定時
 */
int iMinOfInputNumberDialogExecute = -1;	// 最小値
int iMaxOfInputNumberDialogExecute = -1;	// 最大値
int iValueOfInputNumberDialogExecute = -1;	// 入れた値
int iPointerOfMessageOfInputNumberDialogExecute = -1; // メッセージのポインタ

void OnTenshouExeInputNumberDialogExecute() {
	// メッセージの内容
	char *pMessage = (char *)iPointerOfMessageOfInputNumberDialogExecute;

	// シナリオModの呼び出し
	if ( p_snOnDecisionInputNumberDialog ) {

		数値入力ダイアログパラメタ型 param;
		param.最小値 = iMinOfInputNumberDialogExecute;
		param.最大値 = iMaxOfInputNumberDialogExecute;
		param.入力値 = iValueOfInputNumberDialogExecute;

		p_snOnDecisionInputNumberDialog( pMessage, &param);
	}

}


/*
004A39D8  |. FF75 10        PUSH    DWORD PTR SS:[EBP+10]            ; /Arg3
004A39DB  |. 57             PUSH    EDI                              ; |Arg2		入力の最大値
004A39DC  |. 56             PUSH    ESI                              ; |Arg1		入力の最小値
004A39DD  |. 8D8D F8FEFFFF  LEA     ECX, DWORD PTR SS:[EBP-108]      ; |
004A39E3  |. E8 F3F6FFFF    CALL    TENSHOU.004A30DB                 ; \TENSHOU.004A30DB	この関数の結果(EAX)が入力して決定した値
004A39E8  |. E8 EA7EFDFF    CALL    TENSHOU.0047B8D7
				└ここを JMP TSMod.OnTSExeInputNumberDialog と書き換えてやる実際の処理
004A39ED  |. 8D8D F8FEFFFF  LEA     ECX, DWORD PTR SS:[EBP-108]
*/
int pTenshouExeJumpFromToOnTenshouExeInputNumberDialog		=0x4A39E8; // 関数はこのアドレスから、OnTenshouExeInputNumberDialogへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeInputNumberDialog  =0x47B8D7; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeInputNumberDialog	=0x4A39ED; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeInputNumberDialog() {
	// スタックにためておく
	__asm {
		mov iMinOfInputNumberDialogExecute, esi		// 最小値の保存
		mov iMaxOfInputNumberDialogExecute, edi		// 最大値の保存

		mov edi, DWORD PTR SS:[EBP+0x10]				// メッセージの内容のポインタをediに保存
		mov iPointerOfMessageOfInputNumberDialogExecute, edi // 変数に保存
		mov edi, iMaxOfInputNumberDialogExecute		// ediの値を元へと戻す。


		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeInputNumberDialog

		mov iValueOfInputNumberDialogExecute, eax

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
	OnTenshouExeInputNumberDialogExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeInputNumberDialog
	}
}

char cmdOnTenshouExeJumpFromInputNumberDialog[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeInputNumberDialog() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeInputNumberDialog;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeInputNumberDialog + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeInputNumberDialog  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromInputNumberDialog+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeInputNumberDialog), cmdOnTenshouExeJumpFromInputNumberDialog, 5, NULL); //5バイトのみ書き込む
}


