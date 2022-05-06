#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

#include "ScenarioMod.h"


// 捕虜処遇前に実行される関数
void OnTenshouExeOnPostBattleTreatment() {

	// 合戦画面が終了した後、捕虜処遇が始まる前
	if (p_snOnBattleScreenEnd) {
		p_snOnBattleScreenEnd();
	}
}

/*
0042A29B   |. E8 26F2FFFF    CALL    TENSHOU.004294C6
0042A2A0   |. E8 79F3FFFF    CALL    TENSHOU.0042961E
0042A2A5   |. E8 79FDFFFF    CALL    TENSHOU.0042A023	← この関数が捕虜処遇のタイミング
0042A2AA   |. E8 D8FDFFFF    CALL    TENSHOU.0042A087
0042A2AF   |. E8 4CFEFFFF    CALL    TENSHOU.0042A100
0042A2B4   |. E8 E1FEFFFF    CALL    TENSHOU.0042A19A
0042A2B9   |. E8 ACFFFFFF    CALL    TENSHOU.0042A26A
*/


BOOL PostBattleTreatmentPeriod = FALSE; // 捕虜処遇のタイミング

/*
0042A2A0   |. E8 79F3FFFF    CALL    TENSHOU.0042961E
0042A2A5   |. E8 79FDFFFF    CALL    TENSHOU.0042A023	← この関数が捕虜処遇のタイミング
				└ここを JMP TSMod.OnTSExePostBattleTreatment と書き換えてやる実際の処理
0042A2AA   |. E8 D8FDFFFF    CALL    TENSHOU.0042A087
*/
int pTenshouExeJumpFromToOnTenshouExePostBattleTreatment	 =0x42A2A5; // 関数はこのアドレスから、OnTenshouExePostBattleTreatmentへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExePostBattleTreatment =0x42A023; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExePostBattleTreatment  =0x42A2AA; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExePostBattleTreatment() {
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

	OnTenshouExeOnPostBattleTreatment();

	// スタックにためておく
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		mov PostBattleTreatmentPeriod, 1	// 捕虜処遇モードスタート
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExePostBattleTreatment

		mov PostBattleTreatmentPeriod, 0	// 捕虜処遇モード終了

		jmp pTenshouExeReturnLblFromOnTenshouExePostBattleTreatment
	}
}

char cmdOnTenshouExeJumpFromPostBattleTreatment[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExePostBattleTreatment() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExePostBattleTreatment;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExePostBattleTreatment + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExePostBattleTreatment  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromPostBattleTreatment+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExePostBattleTreatment), cmdOnTenshouExeJumpFromPostBattleTreatment, 5, NULL); //5バイトのみ書き込む
}




