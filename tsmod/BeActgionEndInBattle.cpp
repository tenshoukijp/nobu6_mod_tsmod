#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"

/*
　以下の行動済みとする際に通過してくる関数。
  現在OnActionEndInBattleが優秀なので未利用
*/

/*
0042DF77  /$ FF7424 08      PUSH    DWORD PTR SS:[ESP+8]             ; /Arg3
0042DF7B  |. 6A 20          PUSH    20                               ; |Arg2 = 00000020
0042DF7D  |. FF7424 0C      PUSH    DWORD PTR SS:[ESP+C]             ; |Arg1
0042DF81  |. E8 04000000    CALL    TENSHOU.0042DF8A                 ; \TENSHOU.0042DF8A
0042DF86  |. 83C4 0C        ADD     ESP, 0C
0042DF89  \. C3             RETN
*/

int iCurBushouIDOnBeActionEndInBattle = -1; // TENSHOU.EXEから渡ってくる対象の武将

// 各々の武将の、今回のターンでの残り行動数
extern int arrRemainMoveCountAllBushou[GAMEDATASTRUCT_BUSHOU_NUM];

void OnTenshouExeBeActionEndInBattleExecute() {
	int iBushouID = iCurBushouIDOnBeActionEndInBattle-1;
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
	}
}


/*
/*
0042DF77  /$ FF7424 08      PUSH    DWORD PTR SS:[ESP+8]             ; /Arg3
0042DF7B  |. 6A 20          PUSH    20                               ; |Arg2 = 00000020
0042DF7D  |. FF7424 0C      PUSH    DWORD PTR SS:[ESP+C]             ; |Arg1
0042DF81  |. E8 04000000    CALL    TENSHOU.0042DF8A                 ; \TENSHOU.0042DF8A
				└ここを JMP TSMod.OnTSExeBeActionEndInBattle と書き換えてやる実際の処理
0042DF86  |. 83C4 0C        ADD     ESP, 0C
0042DF89  \. C3             RETN
*/
int pTenshouExeJumpFromToOnTenshouExeBeActionEndInBattle	 =0x42DF81; // 関数はこのアドレスから、OnTenshouExeBeActionEndInBattleへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeBeActionEndInBattle =0x42DF8A; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeBeActionEndInBattle	 =0x42DF86; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeBeActionEndInBattle() {
	// スタックにためておく
	__asm {
		// 武将のIDを保存
		mov iCurBushouIDOnBeActionEndInBattle, esi

		// 元々TENSHOU.EXE内にあったものをここで。この中で、行動済(=0x20)へと設定される
		call pTenshouExeJumpCallFromToOnTenshouExeBeActionEndInBattle

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
	OnTenshouExeBeActionEndInBattleExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeBeActionEndInBattle
	}
}

/*
0049D7AA  |. 6A 03          |PUSH    3                               ; |Arg3 = 00000003
0049D7AC  |. 56             |PUSH    ESI                             ; |Arg2
0049D7AD  |. 56             |PUSH    ESI                             ; |Arg1
0049D7AE  |. E8 D3540000    |CALL    TENSHOU.004A2C86                ; \TENSHOU.004A2C86
				└ここを JMP TSMod.OnTSExeBeActionEndInBattle と書き換えてやる実際の処理
0049D7B3  |. 83C4 1C        |ADD     ESP, 1C
0049D7B6  |. 85C0           |TEST    EAX, EAX
*/
char cmdOnTenshouExeJumpFromBeActionEndInBattle[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeBeActionEndInBattle() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeBeActionEndInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeBeActionEndInBattle + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeBeActionEndInBattle  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromBeActionEndInBattle+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeBeActionEndInBattle), cmdOnTenshouExeJumpFromBeActionEndInBattle, 5, NULL); //5バイトのみ書き込む
}




