#include "WinTarget.h"

#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "SetNeedKoudouryoku.h"

// 内政など５人選択するための入力ダイアログが出た時


/*
0042F21C  |. FFB1 B0000000  PUSH    DWORD PTR DS:[ECX+B0]
0042F222     FFB6 BC000000  PUSH    DWORD PTR DS:[ESI+BC]
0042F228  |. E8 0E420500    CALL    TENSHOU.0048343B
*/

void OnTenshouExeViewingNaiseiExecutantsEtc5NingumiDialogExecute() {
}
static int iTmpEAXOfViewingNaiseiExecutantsEtc5NingumiDialog = 0; // EAXの値の一次保存用

int pTenshouExeJumpFromToOnTenshouExeViewingNaiseiExecutantsEtc5NingumiDialog    =CMD_NEED_KOUDOUVALUE_5NINGUMI_ADDRESS_06; // 関数はこのアドレスから、OnTenshouExeViewingNaiseiExecutantsEtc5NingumiDialogへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeViewingNaiseiExecutantsEtc5NingumiDialog =0x42F228; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeViewingNaiseiExecutantsEtc5NingumiDialog() {
	// スタックにためておく
	__asm {

		mov iTmpEAXOfViewingNaiseiExecutantsEtc5NingumiDialog, eax // EAX一次使用するので、いったん保持
		mov eax, TSModCommand::AllGundan::cmdNeedKoudouValue5Ningumi // ５人組の行動力をここにEAXにコピー
		mov DWORD PTR DS:[ESI+0xBC], EAX // EAXの値を上書き

		PUSH    DWORD PTR DS:[ESI+0xBC] // 元々天翔記にあった処理をここに

		mov eax, iTmpEAXOfViewingNaiseiExecutantsEtc5NingumiDialog // EAXの値を元へと戻す

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
	OnTenshouExeViewingNaiseiExecutantsEtc5NingumiDialogExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingNaiseiExecutantsEtc5NingumiDialog
	}
}


char cmdOnTenshouExeJumpFromViewingNaiseiExecutantsEtc5NingumiDialog[7]="\xE9\x90\x90\x90\x90\x90";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeViewingNaiseiExecutantsEtc5NingumiDialog() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeViewingNaiseiExecutantsEtc5NingumiDialog;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingNaiseiExecutantsEtc5NingumiDialog + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingNaiseiExecutantsEtc5NingumiDialog  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromViewingNaiseiExecutantsEtc5NingumiDialog+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingNaiseiExecutantsEtc5NingumiDialog), cmdOnTenshouExeJumpFromViewingNaiseiExecutantsEtc5NingumiDialog, 6, NULL); //6バイトのみ書き込む
}




