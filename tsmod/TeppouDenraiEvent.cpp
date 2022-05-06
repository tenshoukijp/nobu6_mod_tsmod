
#include "WinTarget.h"

#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"


// 鉄砲関連のイベントのコントロール
void CheckTeppouEventControll() {

	// メインゲーム開始時に、鉄砲の存在を調整する。
	// もしこの世に鉄砲が存在しないならば…
	if ( TSModCommand::World::cmdTheTeppouNoExists ) {

		// 鉄砲伝来は無かったこととなる。
		UnsetTeppouDenrai();

		// 全ての勢力の鉄砲の値を0とする。
		for ( int g=0; g < GAMEDATASTRUCT_GUNDAN_NUM; g++ ) {
			nb6gundan[g].gun = 0;
		}

		// 全ての武将の鉄砲の値を0とする。
		for ( int i=0; i < GAMEDATASTRUCT_BUSHOU_NUM; i++ ) {

			//兵態(0=足軽,1=騎馬,2=鉄砲,3=騎馬鉄砲,18まで)
			if ( nb6bushouref[i].form == 2 ) {
				nb6bushouref[i].form = 0; // 足軽へ
			}
			else if ( nb6bushouref[i].form == 3 ) {
				nb6bushouref[i].form = 1; // 騎馬へ
			}

			nb6bushouref[i].biggun = 0;		//大砲消す
			nb6bushouref[i].horsegun = 0;	//騎馬鉄砲消す
			nb6bushouref[i].steelship = 0;	//鉄甲船消す
		}

		for (int c=0; c < GAMEDATASTRUCT_CASTLE_NUM; c++) {
			nb6castle[c].bSmith = 0;		// 鍛冶を消す
		}

	}
}



/*
0042B889     837C24 04 00   CMP     DWORD PTR SS:[ESP+4], 0
0042B88E   . 56             PUSH    ESI
0042B88F   . 57             PUSH    EDI

上が鉄砲伝来イベントのの最初の頭。これを RETN としてしまえば発生しなくなる。
*/


int pTenshouExeJumpFromToOnTenshouExeTeppouDenraiEvent     =0x42B889; // 関数はこのアドレスから、OnTenshouExeTeppouDenraiJudgementへとジャンプしてくる。

char cmdOnTenshouExeJumpFromTeppouDenraiEvent[6]="\xC3\x90\x90\x90\x90"; // C3 = RETN
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeTeppouDenraiEvent() {

	if ( TSModCommand::World::cmdTheTeppouNoExists ) {
		// 鉄砲伝来のイベント開始の先頭部分を潰してRETNにしてしまう。

		// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeTeppouDenraiEvent), cmdOnTenshouExeJumpFromTeppouDenraiEvent, 5, NULL); // 5バイトのみ書き込む
	}
}





