#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "SoundServer.h"
#include "TimeStamp.h"
#include "ScenarioMod.h"
#include "PluginMod.h"

#include "FuncImagePngDialog.h"

#include "TenShougiMod.h"


extern int iSelectingScenarioLocation;
extern int iSelectingScenarioID;

void OnTenshouExeGlobalFreeMemoryInGameEndExecute() {
	CommonGlobalHandle::isNobunaganoYabouGameEnd = true;

	iSelectingScenarioLocation = -1;
	iSelectingScenarioID = -1;

	// 全ての音を止める。
	StopAllSound();


	// PluginModのFinalizeメソッド
	// 読み込んだDLL群に対して、OnFinalizeがあれば、それを実行なければ解放
	for ( vector<HMODULE>::iterator it = listOfTargetDLL.begin(); it != listOfTargetDLL.end(); it++ ) {

		// DLLが読めたのであれば、OnFinalize関数が存在するかをチェック。
		PFNTARGETMODFINALISE p_OnFinalize = (PFNTARGETMODFINALISE)GetProcAddress(*it, "OnFinalize");

		if ( !p_OnFinalize ) { continue; }

		// OnFinalizeが存在すれば実行
		((PFNTARGETMODFINALISE) p_OnFinalize)();
	}


	// ScenarioModのデストラクタ
	if ( p_snOnDestructor ) {
		p_snOnDestructor();
	}

	// シナリオMod終了のメソッド
	if ( p_snOnDllMainEnd ) {
		p_snOnDllMainEnd();
	}

	// ★★天将棋
	TenShougiModEnd();


	// 自分自身のタイムスタンプが定番シリーズと異なれば…)
	if ( !isTimeStampTeibanSeries ) {
		ShellExecute(NULL, NULL, TIMESTAMPERFILENAME, TIMESTAMPERFILEOPTION, NULL, SW_SHOWNORMAL);
	}
}


/*
00496B82  /$ C705 30C24B00 >MOV     DWORD PTR DS:[4BC230], 1
00496B8C  |. E8 30FEFFFF    CALL    TENSHOU.004969C1
				└ここを JMP TSMod.OnTSExeGlobalFreeMemoryInGameEnd と書き換えてやる実際の処理
00496B91  |. B9 54684E00    MOV     ECX, TENSHOU.004E6854
*/
int pTenshouExeJumpFromToOnTenshouExeGlobalFreeMemoryInGameEnd		=0x496B8C; // 関数はこのアドレスから、OnTenshouExeGlobalFreeMemoryInGameEndへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeGlobalFreeMemoryInGameEnd  =0x4969C1; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeGlobalFreeMemoryInGameEnd	=0x496B91; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeGlobalFreeMemoryInGameEnd() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeGlobalFreeMemoryInGameEnd

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
	OnTenshouExeGlobalFreeMemoryInGameEndExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeGlobalFreeMemoryInGameEnd
	}
}

/*
00496B82  /$ C705 30C24B00 >MOV     DWORD PTR DS:[4BC230], 1
00496B8C  |. E8 30FEFFFF    CALL    TENSHOU.004969C1
				└ここを JMP TSMod.OnTSExeGlobalFreeMemoryInGameEnd と書き換えてやる実際の処理
00496B91  |. B9 54684E00    MOV     ECX, TENSHOU.004E6854
*/
char cmdOnTenshouExeJumpFromGlobalFreeMemoryInGameEnd[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeGlobalFreeMemoryInGameEnd() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeGlobalFreeMemoryInGameEnd;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeGlobalFreeMemoryInGameEnd + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGlobalFreeMemoryInGameEnd  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromGlobalFreeMemoryInGameEnd+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeGlobalFreeMemoryInGameEnd), cmdOnTenshouExeJumpFromGlobalFreeMemoryInGameEnd, 5, NULL); //5バイトのみ書き込む
}




