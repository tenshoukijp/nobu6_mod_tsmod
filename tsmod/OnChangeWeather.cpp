#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"



/*
00496331  |. 8B7424 08      MOV     ESI, DWORD PTR SS:[ESP+8]		← ここから天記番号がコピーされる。
00496335  |. 56             PUSH    ESI								← この天候が天気番号
00496336  |. E8 09C4FFFF    CALL    TENSHOU.00492744
0049633B  |. 83C4 04        ADD     ESP, 4
*/

/*
 新たな天候がセットされる時
 */
//  0:晴れ 1:曇り 2:雨 3:雪


vector<TReservedTenki> iReservedWeatherOnChangeWeather;  // 予約天気
int iWeatherOnChangeWeather = -1;  // 設定される予定の本来の天気

void OnTenshouExeChangeWeatherExecute() {
	// 天候が予約されているならば…
	if ( !iReservedWeatherOnChangeWeather.empty() ) {

		if ( 0<=iReservedWeatherOnChangeWeather[0].iTenki && iReservedWeatherOnChangeWeather[0].iTenki <= 3 ) {

			iWeatherOnChangeWeather = iReservedWeatherOnChangeWeather[0].iTenki; // その天候で上書きする

			// 予約してからiContinueTurnターンがたったら効果が切れた。
			if ( getRemainTurnInBattle() <= iReservedWeatherOnChangeWeather[0].iRemainTurn -  iReservedWeatherOnChangeWeather[0].iContinueTurn ) {
				iReservedWeatherOnChangeWeather.clear(); // 使ったので予約は解除
			} else if ( getRemainTurnInBattle() == 0 ) {
				iReservedWeatherOnChangeWeather.clear(); // 使ったので予約は解除
			}
		}
	}

	// カスタム条件で、天候変更AS条件
	if ( p_snOnCalcWeatherInBattle ) {

		// 天気番号																		      ┌現在予定されている天気番号   ┌残りターン数
		int isWarWeatherResult = ((PFNSNONCALCWEATHERINBATTLE) p_snOnCalcWeatherInBattle)(iWeatherOnChangeWeather, getRemainTurnInBattle() );
		if ( isWarWeatherResult != -1 ) {

			// 天気を上書きする。
			iWeatherOnChangeWeather = isWarWeatherResult;
		}
	}

}


/*
00496331  |. 8B7424 08      MOV     ESI, DWORD PTR SS:[ESP+8]		← ここから天記番号がコピーされる。
00496335  |. 56             PUSH    ESI								← この天候が天気番号
00496336  |. E8 09C4FFFF    CALL    TENSHOU.00492744
				└ここを JMP TSMod.OnTSExeChangeWeather と書き換えてやる実際の処理
0049633B  |. 83C4 04        ADD     ESP, 4
*/
int pTenshouExeJumpFromToOnTenshouExeChangeWeather	     =0x496336; // 関数はこのアドレスから、OnTenshouExeChangeWeatherへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeChangeWeather   =0x492744; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeChangeWeather    =0x49633B; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w

// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeChangeWeather() {
	// スタックにためておく
	__asm {

		mov iWeatherOnChangeWeather, esi   // 本来の天候をコピーしておく。

		pop esi								// 天候のプッシュはなかったことにする。

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
	OnTenshouExeChangeWeatherExecute();

	__asm {

		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		mov esi, iWeatherOnChangeWeather   //　天候を上書きする。

		push esi // 元々のTENSHOU.EXE内の記述

		// 元々TENSHOU.EXE内で呼び出していた関数を呼び出し
		call pTenshouExeJumpCallFromToOnTenshouExeChangeWeather

		jmp pTenshouExeReturnLblFromOnTenshouExeChangeWeather
	}
}

/*
*/
char cmdOnTenshouExeJumpFromChangeWeather[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeChangeWeather() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeChangeWeather;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeChangeWeather + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeChangeWeather  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromChangeWeather+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeChangeWeather), cmdOnTenshouExeJumpFromChangeWeather, 5, NULL); //5バイトのみ書き込む
}




