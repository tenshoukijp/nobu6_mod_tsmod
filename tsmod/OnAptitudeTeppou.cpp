#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


/*
 * 鉄砲適正の取得時
 */

int iApptitudeTeppouBushouID = -1;

int iAddtionApptitudeTeppouValue = 0;

void OnTenshouExeAptitudeTeppouExecute() {

	// 余分に足す鉄砲適正は毎度0
	iAddtionApptitudeTeppouValue = 0;

	int iBushouID = iApptitudeTeppouBushouID-1;
}


/*
0040A1C6     E8 44320A00    CALL    TENSHOU.004AD3CE
				└ここを JMP TSMod.OnTSExeAptitudeTeppou と書き換えてやる実際の処理
0040A1CB  |. 0FB7C0         MOVZX   EAX, AX
0040A1CE  |. 83C4 08        ADD     ESP, 8

*/
int pTenshouExeJumpFromToOnTenshouExeAptitudeTeppou	      =0x40A1C6; // 関数はこのアドレスから、OnTenshouExeAptitudeTeppouへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeAptitudeTeppou   =0x4AD3CE; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeAptitudeTeppou    =0x40A1CE; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w

// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeAptitudeTeppou() {
	// スタックにためておく
	__asm {
		// 武将IDをコピー
		mov iApptitudeTeppouBushouID, edi

		// 元々TENSHOU.EXE内にあった処理
		call pTenshouExeJumpCallFromToOnTenshouExeAptitudeTeppou

		// TENSHOU.EXE内の元の処理をここに
		movzx   eax, ax

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
	OnTenshouExeAptitudeTeppouExecute();

	__asm {

		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax


		// 鉄砲適正を足しこむ(このeaxは下半分のexだけが鉄砲適正なので注意!!)
		add eax, iAddtionApptitudeTeppouValue

		jmp pTenshouExeReturnLblFromOnTenshouExeAptitudeTeppou
	}
}

/*
0040A185     E8 44320A00    CALL    TENSHOU.004AD3CE
				└ここを JMP TSMod.OnTSExeAptitudeTeppou と書き換えてやる実際の処理
0040A18A  |. 0FB7C0         MOVZX   EAX, AX
*/
char cmdOnTenshouExeJumpFromAptitudeTeppou[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeAptitudeTeppou() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeAptitudeTeppou;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeAptitudeTeppou + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeAptitudeTeppou  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromAptitudeTeppou+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeAptitudeTeppou), cmdOnTenshouExeJumpFromAptitudeTeppou, 5, NULL); //5バイトのみ書き込む
}




