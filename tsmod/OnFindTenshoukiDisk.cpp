#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "TSModCommand.h"

/*
0043CDC7  |> BB 80954C00    MOV     EBX, TENSHOU.004C9580			→ ここが天翔記のＣＤディスクのアドレス 
0043CDCC  |> 6A 01          /PUSH    1                               ; /ErrorMode = SEM_FAILCRITICALERRORS
0043CDCE  |. FF15 18754E00  |CALL    DWORD PTR DS:[<&KERNEL32.SetErr>; \SetErrorMode
0043CDD4  |. 53             |PUSH    EBX                             ; /Arg1
0043CDD5  |. 8945 FC        |MOV     DWORD PTR SS:[EBP-4], EAX       ; |
0043CDD8  |. E8 1CCD0200    |CALL    TENSHOU.00469AF9                ; \TENSHOU.00469AF9
0043CDDD  |. 83C4 04        |ADD     ESP, 4
0043CDE0  |. 85C0           |TEST    EAX, EAX
*/
int iTenshouExeFindTenshoukiDisk = TRUE;
void OnTenshouExeFindTenshoukiDiskExecute() {

	// ディスクを実際に入れているのかいないのかについて、ここに結果を格納しておく。
	CommonGlobalHandle::isExistDisk = iTenshouExeFindTenshoukiDisk;

	// ノーディスクモードであれば、常に見つかったとする。
	if ( CommonGlobalHandle::isNoDiskMode ) {
		// 見つかってなくても見つかったということにしておく。
		iTenshouExeFindTenshoukiDisk = TRUE;

		// ドライブ名のポインタ。特に今はなにもしない。
		char *szDiskName = (char *)GAMEDATASTRUCT_TENSHOUKI_DRIVENAME_ADDRESS;
	}

}


/*
0043CDC7  |> BB 80954C00    MOV     EBX, TENSHOU.004C9580


0043CDD4  |. 53             |PUSH    EBX                             ; /Arg1
0043CDD5  |. 8945 FC        |MOV     DWORD PTR SS:[EBP-4], EAX       ; |
0043CDD8  |. E8 1CCD0200    |CALL    TENSHOU.00469AF9                ; \TENSHOU.00469AF9
				└ここを JMP TSMod.OnTSExeFindTenshoukiDisk と書き換えてやる実際の処理
0043CDDD  |. 83C4 04        |ADD     ESP, 4
0043CDE0  |. 85C0           |TEST    EAX, EAX
 */

int pTenshouExeJumpFromToOnTenshouExeFindTenshoukiDisk    =0x43CDD8; // 関数はこのアドレスから、OnTenshouExeFindTenshoukiDiskへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeFindTenshoukiDisk=0x469AF9; // 130城以上だった場合のジャンプ先のアドレス
int pTenshouExeReturnLblFromOnTenshouExeFindTenshoukiDisk =0x43CDDD; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w

// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeFindTenshoukiDisk() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXEにあった処理をここに
		call pTenshouExeJumpCallFromToOnTenshouExeFindTenshoukiDisk

		// ディスクが見つかったか？ の結果を書き込んでおく
		mov iTenshouExeFindTenshoukiDisk, eax

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
	OnTenshouExeFindTenshoukiDiskExecute();

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

		// 結果を上書きする。
		mov eax, iTenshouExeFindTenshoukiDisk

		jmp pTenshouExeReturnLblFromOnTenshouExeFindTenshoukiDisk
	}
}

/*
00444769  |. 50             PUSH    EAX
0044476A  |. E8 3A2C0200    CALL    TENSHOU.004673A9
				└ここを JMP TSMod.OnTSExeFindTenshoukiDisk と書き換えてやる実際の処理
0044476F  |. 83C4 04        ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromFindTenshoukiDisk[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeFindTenshoukiDisk() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeFindTenshoukiDisk;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeFindTenshoukiDisk + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeFindTenshoukiDisk  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromFindTenshoukiDisk+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeFindTenshoukiDisk), cmdOnTenshouExeJumpFromFindTenshoukiDisk, 5, NULL); //5バイトのみ書き込む
}













