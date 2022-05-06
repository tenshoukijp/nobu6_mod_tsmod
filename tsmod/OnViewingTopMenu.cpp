#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "PluginMod.h"
#include "ScenarioMod.h"

/*
 トップメニューのどれかを選択した時
 */

// 年・季節がどこかで宣言されている。
extern int nb6progressseason;

// １つ前の季節の状態
extern char pre_check_season;

// 選択しているシナリオ番号
extern int iSelectingScenarioLocation;
extern int iSelectingScenarioID;



void OnTenshouExeViewingTopMenuExecute() {

	// 季節増加カウントをリセット
	nb6progressseason = 1L;

	// １つ前の季節をあり得ない数値にしておく
	pre_check_season = 100;

	iSelectingScenarioLocation = -1;
	iSelectingScenarioID = -1;

	// 拡張データ部分をリセット
	memset( (void *)&nb6savedata_ex, 0, sizeof(nb6savedata_ex) );
	memset( (void *)&nb6savedata_ex_common, 0, sizeof(nb6savedata_ex_common) );

	// ここに来るたびにエンディングモード終了
	CommonGlobalHandle::isEndingMode = false;

	CommonGlobalHandle::isEndingModeAfter = false;

	// プラグインModのメソッド
	// 実行対象となっているDLLを順に捜査して…
	for ( vector<HMODULE>::iterator it = listOfTargetDLL.begin(); it != listOfTargetDLL.end(); it++ ) {

		// 対象のDLLにOnSelectingScenarioDaimyoStart関数が存在するかをチェック。
		PFNTARGETMODVIEWINGTOPMENU p_ViewingTopMenu = (PFNTARGETMODVIEWINGTOPMENU)GetProcAddress(*it, "OnViewingTopMenu");

		if ( !p_ViewingTopMenu ) {
			continue;
		}

		((PFNTARGETMODVIEWINGTOPMENU) p_ViewingTopMenu)();
	}

	// シナリオModの同様のメソッド
	if ( p_snOnViewingTopMenu ) {
		p_snOnViewingTopMenu();
	}
}


/*
0049D7AA  |. 6A 03          |PUSH    3                               ; |Arg3 = 00000003
0049D7AC  |. 56             |PUSH    ESI                             ; |Arg2
0049D7AD  |. 56             |PUSH    ESI                             ; |Arg1
0049D7AE  |. E8 D3540000    |CALL    TENSHOU.004A2C86                ; \TENSHOU.004A2C86
				└ここを JMP TSMod.OnTSExeViewingTopMenu と書き換えてやる実際の処理
0049D7B3  |. 83C4 1C        |ADD     ESP, 1C
0049D7B6  |. 85C0           |TEST    EAX, EAX
*/
int pTenshouExeJumpFromToOnTenshouExeViewingTopMenu		=0x49D7AE; // 関数はこのアドレスから、OnTenshouExeViewingTopMenuへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeViewingTopMenu =0x4A2C86; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeViewingTopMenu	=0x49D7B3; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeViewingTopMenu() {
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
	
	// ゲームデータをプレイヤーターンが変わった段階で読み込み更新しておく
	OnTenshouExeViewingTopMenuExecute();

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
		call pTenshouExeJumpCallFromToOnTenshouExeViewingTopMenu

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingTopMenu
	}
}

/*
0049D7AA  |. 6A 03          |PUSH    3                               ; |Arg3 = 00000003
0049D7AC  |. 56             |PUSH    ESI                             ; |Arg2
0049D7AD  |. 56             |PUSH    ESI                             ; |Arg1
0049D7AE  |. E8 D3540000    |CALL    TENSHOU.004A2C86                ; \TENSHOU.004A2C86
				└ここを JMP TSMod.OnTSExeViewingTopMenu と書き換えてやる実際の処理
0049D7B3  |. 83C4 1C        |ADD     ESP, 1C
0049D7B6  |. 85C0           |TEST    EAX, EAX
*/
char cmdOnTenshouExeJumpFromViewingTopMenu[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeViewingTopMenu() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeViewingTopMenu;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingTopMenu + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingTopMenu  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromViewingTopMenu+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingTopMenu), cmdOnTenshouExeJumpFromViewingTopMenu, 5, NULL); //5バイトのみ書き込む
}




