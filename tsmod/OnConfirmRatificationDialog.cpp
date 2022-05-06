#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "ScenarioMod.h"


/*
 * 情報・確認・警告などの「追認系」ダイアログの「確認」ボタンをおした時(ボタンがないものも含む)
 */

/*
004A17D1  |. FF75 10        PUSH    DWORD PTR SS:[EBP+10]            ; |Arg4
004A17D4  |. 56             PUSH    ESI                              ; |Arg3
004A17D5  |. FF75 0C        PUSH    DWORD PTR SS:[EBP+C]             ; |Arg2	← タイトル
004A17D8  |. FF75 08        PUSH    DWORD PTR SS:[EBP+8]             ; |Arg1	← メッセージ
004A17DB  |. E8 89F7FFFF    CALL    TENSHOU.004A0F69                 ; \TENSHOU.004A0F69
004A17E0  |. 85F6           TEST    ESI, ESI
 */

int iPointerOfTitleOnConfirmRatificationDialog = NULL; // タイトルの保存用
int iPointerOfMsgConfirmRatificationDialog = NULL; // メッセージの保存用

int iSaveOfEAXConfirmRatificationDialog = -1; // 一旦保存用

void OnTenshouExeConfirmRatificationDialogExecute() {
	if ( p_snOnRatificationDialog ) {
		char *pszTitle = (char *)iPointerOfTitleOnConfirmRatificationDialog;
		char *pszMessage = (char *)iPointerOfMsgConfirmRatificationDialog;
		p_snOnRatificationDialog( pszMessage, pszTitle );
	}
}



/*
004A17D1  |. FF75 10        PUSH    DWORD PTR SS:[EBP+10]            ; |Arg4
004A17D4  |. 56             PUSH    ESI                              ; |Arg3
004A17D5  |. FF75 0C        PUSH    DWORD PTR SS:[EBP+C]             ; |Arg2	← タイトル
004A17D8  |. FF75 08        PUSH    DWORD PTR SS:[EBP+8]             ; |Arg1	← メッセージ
004A17DB  |. E8 89F7FFFF    CALL    TENSHOU.004A0F69                 ; \TENSHOU.004A0F69
				└ここを JMP TSMod.OnTSExeConfirmRatificationDialog と書き換えてやる実際の処理
004A17E0  |. 85F6           TEST    ESI, ESI
*/
int pTenshouExeJumpFromToOnTenshouExeConfirmRatificationDialog		=0x4A17DB; // 関数はこのアドレスから、OnTenshouExeConfirmRatificationDialogへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeConfirmRatificationDialog  =0x4A0F69; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeConfirmRatificationDialog	=0x4A17E0; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeConfirmRatificationDialog() {
	// スタックにためておく
	__asm {

		mov iSaveOfEAXConfirmRatificationDialog, eax // 一旦保存

		mov eax, DWORD PTR SS:[EBP+0xC]						// タイトルの保存
		mov iPointerOfTitleOnConfirmRatificationDialog, eax

		mov eax,  DWORD PTR SS:[EBP+0x8]
		mov iPointerOfMsgConfirmRatificationDialog, eax		// メッセージの保存

		mov eax, iSaveOfEAXConfirmRatificationDialog		// EAXを元へと戻す

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeConfirmRatificationDialog

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
	OnTenshouExeConfirmRatificationDialogExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeConfirmRatificationDialog
	}
}

char cmdOnTenshouExeJumpFromConfirmRatificationDialog[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeConfirmRatificationDialog() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeConfirmRatificationDialog;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeConfirmRatificationDialog + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeConfirmRatificationDialog  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromConfirmRatificationDialog+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeConfirmRatificationDialog), cmdOnTenshouExeJumpFromConfirmRatificationDialog, 5, NULL); //5バイトのみ書き込む
}


