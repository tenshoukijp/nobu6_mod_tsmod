
#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"
#include "SoundServer.h"
#include "FuncPlayBGM.h"


int iNextPlayTrackingNo = -1;

void OnTenshouExeN6PAudioPlayTrackingExecute() {

	// ScenarioModのオーディオトラックのすり替え関数が存在する。
	if ( p_snOnN6PAudioPlayTrack ) { 
		int iNewPlayTrackingNo = ((PFNSNONN6PAUDIOPLAYTRACK) p_snOnN6PAudioPlayTrack)( iNextPlayTrackingNo ); // 神パッチと同じ値。
		// 帰り値が-1以外の時に差し替える
		if ( iNewPlayTrackingNo != -1 ) {
			iNextPlayTrackingNo = iNewPlayTrackingNo; // 有効な値が返って来ていたら代入。
		}
	}
}


/*
0048D448  |. FF7424 0C      PUSH    DWORD PTR SS:[ESP+C]

				└ここを JMP TSMod.OnTSExeN6PAudioPlayTracking と書き換えてやる実際の処理
0048D44C  |. 8D4E 01        LEA     ECX, DWORD PTR DS:[ESI+1]
0048D44F  |. 51             PUSH    ECX
 */
int pTenshouExeJumpFromToOnTenshouExeN6PAudioPlayTracking    =0x48D448; // 関数はこのアドレスから、OnTenshouExeN6PAudioPlayTrackingへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeN6PAudioPlayTracking =0x48D44F; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeN6PAudioPlayTracking() {
	// スタックにためておく
	__asm {
		// TENSHOU.EXEに元々あった処理をここに
		PUSH    DWORD PTR SS:[ESP+0xC]
		LEA     ECX, DWORD PTR DS:[ESI+1]

		mov iNextPlayTrackingNo, ecx

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
	OnTenshouExeN6PAudioPlayTrackingExecute();

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

		mov ecx, iNextPlayTrackingNo

		jmp pTenshouExeReturnLblFromOnTenshouExeN6PAudioPlayTracking
	}
}

/*
00444769  |. 50             PUSH    EAX
0044476A  |. E8 3A2C0200    CALL    TENSHOU.004673A9
				└ここを JMP TSMod.OnTSExeN6PAudioPlayTracking と書き換えてやる実際の処理
0044476F  |. 83C4 04        ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromN6PAudioPlayTracking[8]="\xE9\x90\x90\x90\x90\x90\x90";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeN6PAudioPlayTracking() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeN6PAudioPlayTracking;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeN6PAudioPlayTracking + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeN6PAudioPlayTracking  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromN6PAudioPlayTracking+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeN6PAudioPlayTracking), cmdOnTenshouExeJumpFromN6PAudioPlayTracking, 7, NULL); //7バイトのみ書き込む
}




