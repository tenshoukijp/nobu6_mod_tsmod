
#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"
#include "PluginMod.h"
#include "SetBushouAvgParam.h"
#include "EXBushou_All.h"


extern bool isEnteringFromScenarioDaimyoExecute;
extern bool isEnteringFromLoadDialogExecute;
extern void OnTenshouExeEnteringFromScenarioDaimyoExecute();
extern void EXBushou_All_CheckLifeTimes();

extern int iSelectingScenarioLocation; // シナリオの位置の番号。上から０．一番下が５

extern int iEnterOfClickMainScreenMainCommand;


extern int iIsGameStartFlagEAX = 0; // ロードやゲーム新規開始など時は1、そうではなく、季節が移り変わっただけの時は0

void OnTenshouExeMainGameStartExecute() {

	CommonGlobalHandle::isMainGameStart = true;

	// シナリオ選択のモードではないのは確実なので、このフラグを降ろす
	CommonGlobalHandle::isModeScenario = false;

	// メイン画面のコマンドに侵入してるかどうか。
	iEnterOfClickMainScreenMainCommand = 0;

	// シナリオ選択からきた、もしくはロード画面からきた
	if ( isEnteringFromLoadDialogExecute || isEnteringFromScenarioDaimyoExecute ) {

		// このタイミングで、シナリオ選択からやってきた場合のみシナリオ解説ムービーを流す。
		if ( isEnteringFromScenarioDaimyoExecute ) {
			// シナリオ選択からやってきたフラグを下す。
			isEnteringFromScenarioDaimyoExecute = false;
		} else if (isEnteringFromLoadDialogExecute)  {
			isEnteringFromLoadDialogExecute = false;

			// シナリオ選択以外からやってきた場合は(普通はロード経由のみ)、nb6savedata_ex.bushou_exで必要なパラメタを入れる。
			SetBushouAvgParam(2); // 2というのはロード時のこと。
			SetExBushouAvgParam(-1); // -1は全員に対して行えということ。特殊武将用(シナリオ選択時経由は実行済みなのではずしている)
		}

		// シナリオ選択からでもロードからでも、以下は共通の処理
		copyAllBushouSoldiersOriginToCopy();
	
		EXSuperService_OnMainGameStart();
	}

	// ロード後やゲーム開始時など、「初めてのその季節」の時だけ来る。
	if (iIsGameStartFlagEAX) {
		// ;
	}

	// 毎季節くる。ただし、ロードしなおした際は来ない。
	else {
		// 冬から春に変わる時
		if (nb6year.season == 3) {

			// オプションに関わらず、EX武将が歳を取るのが遅い処理
			EXBushou_All_CheckLifeTimes();

			// もし武将が歳を取るのが数倍遅かったなら…
			EXSuperService_IfLifesIsTheTimes();

		}
	}
}


/*
0044193D  /$ 83EC 10        SUB     ESP, 10				←ここからメインゲームスタートと考えて良い
00441940  |. 56             PUSH    ESI
00441941  |. 57             PUSH    EDI
00441942  |. 68 00020000    PUSH    200
00441947  |. E8 55560200    CALL    TENSHOU.00466FA1
				└ここを JMP TSMod.OnTSExeMainGameStart と書き換えてやる実際の処理
0044194C  |. 83C4 04        ADD     ESP, 4
*/
int pTenshouExeJumpFromToOnTenshouExeMainGameStart		=0x441947; // 関数はこのアドレスから、OnTenshouExeMainGameStartへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeMainGameStart  =0x466FA1; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeMainGameStart	=0x44194C; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeMainGameStart() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeMainGameStart

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi

		mov iIsGameStartFlagEAX, eax    // ロードやゲーム新規開始など時は1、そうではなく、季節が移り変わっただけの時は0
	}
	
	OnTenshouExeMainGameStartExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeMainGameStart
	}
}


char cmdOnTenshouExeJumpFromMainGameStart[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeMainGameStart() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeMainGameStart;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeMainGameStart + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeMainGameStart  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromMainGameStart+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeMainGameStart), cmdOnTenshouExeJumpFromMainGameStart, 5, NULL); //5バイトのみ書き込む
}




