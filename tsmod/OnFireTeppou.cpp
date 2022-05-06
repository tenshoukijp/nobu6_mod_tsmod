#include "WinTarget.h"

#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

/*
 * 鉄砲を撃った時や、弓を射た時
 * それを実行した武将の番号を確保するため。
 */
/*
004328B2  |. 57             PUSH    EDI                              ; /Arg2
004328B3  |. 56             PUSH    ESI                              ; |Arg1
004328B4  |. E8 F78F0600    CALL    TENSHOU.0049B8B0                 ; \TENSHOU.0049B8B0
004328B9  |. 83C4 08        ADD     ESP, 8
*/

int iFireTeppouAttackBushouID = -1; // 鉄砲を撃った武将
int iFireTeppouTargetBushouID = -1; // 鉄砲を撃たれた武将

void OnTenshouExeFireTeppouExecute() {
	// 通常時は、0xFFFFはいらないのだが、大混乱時(該当ユニットの意思で動けないコマンド発行の際には
	// 0x[0018][0BID]というような形になるようだ。
	iFireTeppouAttackBushouID = iFireTeppouAttackBushouID & 0xFFFF;
	iFireTeppouTargetBushouID = iFireTeppouTargetBushouID & 0xFFFF;
	; // 特になし
}



/*
004328B2  |. 57             PUSH    EDI                              ; /Arg2　 ←鉄砲を撃たれた武将
004328B3  |. 56             PUSH    ESI                              ; |Arg1   ← 鉄砲を撃つ武将
004328B4  |. E8 F78F0600    CALL    TENSHOU.0049B8B0                 ; \TENSHOU.0049B8B0 ← 鉄砲を撃った際に呼ばれる関数
				└ここを JMP TSMod.OnTSExeFireTeppou と書き換えてやる実際の処理
004328B9  |. 83C4 08        ADD     ESP, 8
*/
int pTenshouExeJumpFromToOnTenshouExeFireTeppou		=0x4328B4; // 関数はこのアドレスから、OnTenshouExeFireTeppouへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeFireTeppou =0x49B8B0; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeFireTeppou	=0x4328B9; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeFireTeppou() {
	// スタックにためておく
	__asm {
		// 武将のIDを保存
		mov iFireTeppouAttackBushouID, esi // ← 鉄砲を撃つ武将
		mov iFireTeppouTargetBushouID, edi // ←鉄砲を撃たれた武将

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
	OnTenshouExeFireTeppouExecute();

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

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeFireTeppou

		jmp pTenshouExeReturnLblFromOnTenshouExeFireTeppou
	}
}




char cmdOnTenshouExeJumpFromFireTeppou[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeFireTeppou() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeFireTeppou;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeFireTeppou + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeFireTeppou  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromFireTeppou+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeFireTeppou), cmdOnTenshouExeJumpFromFireTeppou, 5, NULL); //5バイトのみ書き込む
}




