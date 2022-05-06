#include "WinTarget.h"
#include <time.h>


#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "FuncPlayResourceWav.h"

void OnTenshouExeCastleFallDownExecute() {

	// ３回に１回ぐらいだけ鳴らす
	if ( rand() % 3 == 0 ) {

		// 音を鳴らす。	714	勝ち鬨（本丸占拠） 
		FuncPlaySoundWav(714);
	}
}


/*
004487DB   . 57             PUSH    EDI
004487DC   . 57             PUSH    EDI
004487DD   . 57             PUSH    EDI
004487DE   . 6A 0A          PUSH    0A
004487E0   > E8 044C0400    CALL    TENSHOU.0048D3E9
				└ここを JMP TSMod.OnTSExeCastleFallDown と書き換えてやる実際の処理
004487E5   . 83C4 10        ADD     ESP, 10
004487E8   . EB 0B          JMP     SHORT TENSHOU.004487F5
*/
int pTenshouExeJumpFromToOnTenshouExeCastleFallDown		=0x4487E0; // 関数はこのアドレスから、OnTenshouExeCastleFallDownへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeCastleFallDown =0x48D3E9; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeCastleFallDown	=0x4487E5; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeCastleFallDown() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeCastleFallDown

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
	OnTenshouExeCastleFallDownExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeCastleFallDown
	}
}

/*
004487DB   . 57             PUSH    EDI
004487DC   . 57             PUSH    EDI
004487DD   . 57             PUSH    EDI
004487DE   . 6A 0A          PUSH    0A
004487E0   > E8 044C0400    CALL    TENSHOU.0048D3E9
				└ここを JMP TSMod.OnTSExeCastleFallDown と書き換えてやる実際の処理
004487E5   . 83C4 10        ADD     ESP, 10
004487E8   . EB 0B          JMP     SHORT TENSHOU.004487F5
*/
char cmdOnTenshouExeJumpFromCastleFallDown[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeCastleFallDown() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeCastleFallDown;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeCastleFallDown + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeCastleFallDown  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromCastleFallDown+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeCastleFallDown), cmdOnTenshouExeJumpFromCastleFallDown, 5, NULL); //5バイトのみ書き込む
}




