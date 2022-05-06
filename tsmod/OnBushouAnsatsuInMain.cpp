#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"
#include "FuncPlayBGM.h"



/*
00403DBB   . 8B4424 04      MOV     EAX, DWORD PTR SS:[ESP+4]                      ;  TENSHOU.004DDF38  ← 武将暗殺コマンドのスタート位置
00403DBF   . 53             PUSH    EBX
00403DC0   . 66:8B58 0C     MOV     BX, WORD PTR DS:[EAX+C]
00403DC4   . 56             PUSH    ESI
00403DC5   . 66:8B70 04     MOV     SI, WORD PTR DS:[EAX+4]
00403DC9   . 57             PUSH    EDI
00403DCA   . 55             PUSH    EBP
00403DCB   . 53             PUSH    EBX
00403DCC   . E8 FC5C0000    CALL    TENSHOU.00409ACD

00403DE7   . 56             PUSH    ESI
00403DE8   . E8 9DFDFFFF    CALL    TENSHOU.00403B8A
00403DED   . 83C4 08        ADD     ESP, 8
00403DF0   . 85C0           TEST    EAX, EAX									← ここのEAXが暗殺成功したか否か
																			       この瞬間 ESI が実行武将の武将番号、EBXが暗殺対象武将の武将番号
00403DF2   . 74 13          JE      SHORT TENSHOU.00403E07
*/



int iAnsatuJikkouBushouInMain = -1; // 実行武将
int iAnsatuSareruBushouInMain = -1; // 暗殺対象武将
int iAnsatuResultBushouInMain = -1; // 暗殺結果

// メイン画面の暗殺時に大名をやると、音楽が途切れてしまう問題を解消する
int iAnsatuBGMOfPatchInAnsatsuBushouInMain = -1;

void OnTenshouExeAnsatsuBushouInMainExecute() {
	int iJikkouBushouID = iAnsatuJikkouBushouInMain - 1;

	if ( 0 <= iJikkouBushouID && iJikkouBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		// 実行武将がランスロットタルタロスであれば、
		if (isLancelotTartare(iJikkouBushouID)) {
			// どれほど分が悪くとも、1/2の暗殺成功率は約束されている。
			if (rand() % 2 == 0) {
				iAnsatuResultBushouInMain = 1; // 暗殺の結果は成功
			}

		// 実行武将が秀千代であれば、
		} else if (isShuiHidechiyo(iJikkouBushouID)) {
			
			// どれほど分が悪くとも、1/2の暗殺成功率は約束されている。
			if (rand() % 2 == 0) {
				iAnsatuResultBushouInMain = 1; // 暗殺の結果は成功
			}

		// 魔族であれば、暗殺が上手い
		} else if ( nb6bushouref[iJikkouBushouID].job == 0xC0 ) { // 魔族
			// どれほど分が悪くとも、1/2の暗殺成功率は約束されている。
			if ( rand() % 2 == 0 ) {
				iAnsatuResultBushouInMain = 1; // 暗殺の結果は成功
			}
		}
	}


	int iSareruBushouID = iAnsatuSareruBushouInMain - 1;

	if ( 0 <= iSareruBushouID && iSareruBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		// 暗殺される対象がランスロットタルタロスであれば、
		if (isLancelotTartare(iSareruBushouID)) {
			iAnsatuResultBushouInMain = 0; // 試みは絶対に失敗である。

		// 暗殺される対象が朱伊であれば、
		} else if ( isShuiHidechiyo(iSareruBushouID) ) {
			iAnsatuResultBushouInMain = 0; // 試みは絶対に失敗である。

		} else if ( nb6bushouref[iSareruBushouID].job >= 0xA0 ) { // 騎士以上の上級職
			// 75%は絶対に防御してしまう。
			if ( rand() % 4 != 0 ) {
				iAnsatuResultBushouInMain = 0; // 試みはほとんど失敗である。
			}
		}
	}

	if ( 0 <= iJikkouBushouID && iJikkouBushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
	     0 <= iSareruBushouID && iSareruBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ScenarioModのカスタム条件で軍師を設定する
		if ( p_snOnCustomCondition ) {
			// メイン画面での暗殺の成否																									┌実行者	　┌受動者			┌現在予定されている結果
			int isScenarioModDeathResult = ((PFNSNONCUSTOMCONDITION) p_snOnCustomCondition)("AS条件::メイン::暗殺受動者死亡可否", iJikkouBushouID+1, iSareruBushouID+1, iAnsatuResultBushouInMain, "", "", 0.0, 0.0);
			if ( isScenarioModDeathResult != CUSTOM_CONDITION_NOMODIFY ) {
				iAnsatuResultBushouInMain = isScenarioModDeathResult;
			}
		}
	}

	if (iAnsatuResultBushouInMain) {
		iAnsatuBGMOfPatchInAnsatsuBushouInMain = getPlayingBGM();
	}

}


/*
00403DE8   . E8 9DFDFFFF    CALL    TENSHOU.00403B8A
00403DED   . 83C4 08        ADD     ESP, 8
00403DF0   . 85C0           TEST    EAX, EAX									← ここのEAXが暗殺成功したか否か
																			       この瞬間 ESI が実行武将の武将番号、EBXが暗殺対象武将の武将番号
*/

int pTenshouExeJumpFromToOnTenshouExeAnsatsuBushouInMain    =0x403DE8; // 関数はこのアドレスから、OnTenshouExeAnsatsuBushouInMainへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeAnsatsuBushouInMain=0x403B8A; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeAnsatsuBushouInMain =0x403DED; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeAnsatsuBushouInMain() {
	// スタックにためておく
	__asm {

		call pTenshouExeJumpCallFromToOnTenshouExeAnsatsuBushouInMain

		mov iAnsatuJikkouBushouInMain, esi // ecx が実行武将のＩＤ
		mov iAnsatuSareruBushouInMain, ebx // ebx が暗殺対象武将のＩＤ

		mov iAnsatuResultBushouInMain, eax // eax が暗殺成功か、失敗か

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
	OnTenshouExeAnsatsuBushouInMainExecute();

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

		mov eax, iAnsatuResultBushouInMain	// eax に暗殺結果を逆反映

		jmp pTenshouExeReturnLblFromOnTenshouExeAnsatsuBushouInMain
	}
}

/*
00403DE8   . E8 9DFDFFFF    CALL    TENSHOU.00403B8A
00403DED   . 83C4 08        ADD     ESP, 8
00403DF0   . 85C0           TEST    EAX, EAX									← ここのEAXが暗殺成功したか否か
																			       この瞬間 ESI が実行武将の武将番号、EBXが暗殺対象武将の武将番号
*/
char cmdOnTenshouExeJumpFromAnsatsuBushouInMain[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeAnsatsuBushouInMain() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeAnsatsuBushouInMain;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeAnsatsuBushouInMain + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeAnsatsuBushouInMain  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromAnsatsuBushouInMain+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeAnsatsuBushouInMain), cmdOnTenshouExeJumpFromAnsatsuBushouInMain, 5, NULL); //5バイトのみ書き込む
}




