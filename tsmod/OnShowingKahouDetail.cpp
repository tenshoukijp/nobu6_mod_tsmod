#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"

/*
 * 家宝表示モードのＯＮとＯＦＦをつかさどる。家宝詳細情報を表示している間は、iKahouShowingMode をONにするためだ。
 */


// 家宝のShouwingモードのスタート。
int iKahouShowingMode = 0L;

void OnTenshouExeShowingKahouDetailExecute() {
	// アセンブラの中でフラグの操作を完了している。
}


/*
004538DA  |. FF75 08        PUSH    DWORD PTR SS:[EBP+8]
004538DD  |. E8 DC110400    CALL    TENSHOU.00494ABE
004538E2  |. E8 F07F0200    CALL    TENSHOU.0047B8D7
				└ここを JMP TSMod.OnTSExeShowingKahouDetail と書き換えてやる実際の処理
004538E7  |. 8D8D 7CFFFFFF  LEA     ECX, DWORD PTR SS:[EBP-84]
004538ED  |. 8935 80154D00  MOV     DWORD PTR DS:[4D1580], ESI
*/
int pTenshouExeJumpFromToOnTenshouExeShowingKahouDetail		=0x4538E2; // 関数はこのアドレスから、OnTenshouExeShowingKahouDetailへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeShowingKahouDetail =0x47B8D7; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeShowingKahouDetail	=0x4538E7; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeShowingKahouDetail() {
	// スタックにためておく
	__asm {

		// 家宝Shouwingモードに1を入れる。
		mov iKahouShowingMode, 1

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeShowingKahouDetail

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
	OnTenshouExeShowingKahouDetailExecute();

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

		// 家宝Shouwingモードに0を入れる。
		mov iKahouShowingMode, 0

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingKahouDetail
	}
}

/*
004538DA  |. FF75 08        PUSH    DWORD PTR SS:[EBP+8]
004538DD  |. E8 DC110400    CALL    TENSHOU.00494ABE
004538E2  |. E8 F07F0200    CALL    TENSHOU.0047B8D7
				└ここを JMP TSMod.OnTSExeShowingKahouDetail と書き換えてやる実際の処理
004538E7  |. 8D8D 7CFFFFFF  LEA     ECX, DWORD PTR SS:[EBP-84]
004538ED  |. 8935 80154D00  MOV     DWORD PTR DS:[4D1580], ESI
*/
char cmdOnTenshouExeJumpFromShowingKahouDetail[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeShowingKahouDetail() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeShowingKahouDetail;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingKahouDetail + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingKahouDetail  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromShowingKahouDetail+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingKahouDetail), cmdOnTenshouExeJumpFromShowingKahouDetail, 5, NULL); //5バイトのみ書き込む
}




