#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


/*
 * 騎馬適正の取得時
 */
int iApptitudeKibaBushouID = -1;

void OnTenshouExeAptitudeKibaExecute() {
}


/*
0040A185     E8 44320A00    CALL    TENSHOU.004AD3CE
				└ここを JMP TSMod.OnTSExeAptitudeKiba と書き換えてやる実際の処理
0040A18A  |. 0FB7C0         MOVZX   EAX, AX
0040A18D  |. 83C4 08        ADD     ESP, 8

*/
int pTenshouExeJumpFromToOnTenshouExeAptitudeKiba	    =0x40A185; // 関数はこのアドレスから、OnTenshouExeAptitudeKibaへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeAptitudeKiba   =0x4AD3CE; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeAptitudeKiba    =0x40A18D; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w

// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeAptitudeKiba() {
	// スタックにためておく
	__asm {
		// 武将IDをコピー
		mov iApptitudeKibaBushouID, edi

		// 元々TENSHOU.EXE内にあった処理
		call pTenshouExeJumpCallFromToOnTenshouExeAptitudeKiba

		movzx eax, ax

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
	OnTenshouExeAptitudeKibaExecute();

	__asm {

		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		jmp pTenshouExeReturnLblFromOnTenshouExeAptitudeKiba
	}
}

/*
0040A185     E8 44320A00    CALL    TENSHOU.004AD3CE
				└ここを JMP TSMod.OnTSExeAptitudeKiba と書き換えてやる実際の処理
0040A18A  |. 0FB7C0         MOVZX   EAX, AX
*/
char cmdOnTenshouExeJumpFromAptitudeKiba[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeAptitudeKiba() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeAptitudeKiba;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeAptitudeKiba + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeAptitudeKiba  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromAptitudeKiba+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeAptitudeKiba), cmdOnTenshouExeJumpFromAptitudeKiba, 5, NULL); //5バイトのみ書き込む
}




