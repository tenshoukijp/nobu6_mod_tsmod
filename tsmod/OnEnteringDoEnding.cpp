#include "WinTarget.h"

#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

/*
 エンディングに突入した
 */



/*
0047EC3E  |. 56             PUSH    ESI
0047EC3F  |.-E9 4E627D01    JMP     TSMOD.JmpOnTSExeEndingConditionT>
0047EC44  |. 83C4 04        ADD     ESP, 4
0047EC47  |. 85C0           TEST    EAX, EAX
0047EC49  |. 75 34          JNZ     SHORT TENSHOU.0047EC7F
0047EC4B  |. 68 00400000    PUSH    4000
0047EC50  |. E8 5E83FEFF    CALL    TENSHOU.00466FB3
0047EC55  |. 83C4 04        ADD     ESP, 4
0047EC58  |. 56             PUSH    ESI
0047EC59  |. E8 33FFFFFF    CALL    TENSHOU.0047EB91
0047EC5E  |. 83C4 04        ADD     ESP, 4
0047EC61  |. 56             PUSH    ESI
0047EC62  |. E8 006A0200    CALL    TENSHOU.004A5667
0047EC67  |. 83C4 04        ADD     ESP, 4
0047EC6A  |. 85C0           TEST    EAX, EAX
0047EC6C  |. 74 0A          JE      SHORT TENSHOU.0047EC78
0047EC6E  |. E8 7B46FAFF    CALL    TENSHOU.004232EE  ← エンディングに入ってる。
0047EC73  |. E8 4D7F0100    CALL    TENSHOU.00496BC5  ← エンディングに入ってる。
0047EC78  |> B8 01000000    MOV     EAX, 1

*/


extern void TenshouExeEndingTitlePushAddressClear();


void OnTenshouExeEnteringDoEndingExecute() {
	CommonGlobalHandle::isEndingMode = true;

	CommonGlobalHandle::isEndingModeAfter = true;

	// エンディングの寸劇などで値が変わってしまったメモリ状態をクリアしておく
	// 天翔記アプリケーションを終了することなく、再度ゲームが出来るように。
	TenshouExeEndingTitlePushAddressClear();
}


/*
0047EC6C  |. 74 0A          JE      SHORT TENSHOU.0047EC78
0047EC6E  |. E8 7B46FAFF    CALL    TENSHOU.004232EE  ← エンディングに入ってる。
0047EC73  |. E8 4D7F0100    CALL    TENSHOU.00496BC5  ← エンディングに入ってる。
└ここを JMP TSMod.OnTSExeEnteringDoEnding と書き換えてやる実際の処理
0047EC78  |> B8 01000000    MOV     EAX, 1
*/
int pTenshouExeJumpFromToOnTenshouExeEnteringDoEnding	   =0x47EC6E; // 関数はこのアドレスから、OnTenshouExeEnteringDoEndingへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeEnteringDoEnding  =0x4232EE; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeEnteringDoEnding   =0x47EC73; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeEnteringDoEnding() {
	// スタックにためておく
	__asm {

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// エンディングモードへ
	OnTenshouExeEnteringDoEndingExecute();

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
		call pTenshouExeJumpCallFromToOnTenshouExeEnteringDoEnding

		jmp pTenshouExeReturnLblFromOnTenshouExeEnteringDoEnding
	}
}

/*
*/
char cmdOnTenshouExeJumpFromEnteringDoEnding[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeEnteringDoEnding() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeEnteringDoEnding;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeEnteringDoEnding + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeEnteringDoEnding  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromEnteringDoEnding+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeEnteringDoEnding), cmdOnTenshouExeJumpFromEnteringDoEnding, 5, NULL); //5バイトのみ書き込む
}




