#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"

/*
　以下の行動済みとする際に通過してくる関数。
  ここはＰＣ/ＮＰＣ共通でやってくる。

  ★ユニットを配置した時も、行動済みと見なされるが、その場合、ediやesiの値の「e」の部分にシグナルがあるようなので、利用できるかも。
*/

/*
0042DC89  |. E8 4BFEFFFF    CALL    TENSHOU.0042DAD9
				└ここを JMP TSMod.OnTSExeOnActionEndInBattle と書き換えてやる実際の処理
0042DC8E  |. 8B4C24 0C      MOV     ECX, DWORD PTR SS:[ESP+C]
0042DC92  |. 83C4 04        ADD     ESP, 4
0042DC95     0948 18        OR      DWORD PTR DS:[EAX+18], ECX
0042DC98  \. C3             RETN
*/

// 行動メイン武将のポインタ
int iCurMainBushouPointerOnActionEndInBattle = NULL;

// 関係武将
int iCurBushouAIDOnOnActionEndInBattle = -1; // 武将Ａ
int iCurBushouBIDOnOnActionEndInBattle = -1; // 武将Ｂ

// 各々の武将の、今回のターンでの残り行動数
extern int arrRemainMoveCountAllBushou[GAMEDATASTRUCT_BUSHOU_NUM];

bool arrIsFirstHaichiAction[GAMEDATASTRUCT_BUSHOU_NUM];

extern int OnTenshouExeChangeMovePower(int iBushouID, int iPrev, int iNext);

void DoTenshouExeOnActonEndInBattleExecute(int iBushouID) {
	/*
	 *  ここに各ユニットが行動しをえた時の具体的な記述を書く。
	 */
	// 混乱や大混乱や蒸発状態でなければ…
	if ( GetKonranStatus(iBushouID) != KONRAN_STATUS::混乱 && GetKonranStatus(iBushouID) != KONRAN_STATUS::大混乱 && GetKonranStatus(iBushouID) != KONRAN_STATUS::挑発混乱 ) {

		// 武将のIDが上級職であれば… もしくは ランスロット・タルタロスであれば… あるいは朱伊ならば…
		if ( nb6bushouref[iBushouID].job >= 0xA0 || isLancelotTartare(iBushouID) || isShuiHidechiyo(iBushouID) ) {
			if ( arrRemainMoveCountAllBushou[iBushouID] > 1 ) { // まだ未行動カウントが残っていれば
				setIsActionEndInBattle(iBushouID, 0);

				// 移動力を若干回復
				for ( int i=0; i < GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_NUM; i++ ) {
					if ( nb6bushou_hex_unit_info[i].bushou == iBushouID+1 ) {
						if ( nb6bushou_hex_unit_info[i].movepower < 6 ) {
							int iNewValue = OnTenshouExeChangeMovePower(iBushouID, nb6bushou_hex_unit_info[i].movepower, 6 );
							if (iNewValue != -1 ) {
								nb6bushou_hex_unit_info[i].movepower = iNewValue;  // 行動パワー回復。
							}
							break;
						}
					}
				}
			}
		}
		arrRemainMoveCountAllBushou[iBushouID]--; // カウントを１つ減らす
		if ( arrRemainMoveCountAllBushou[iBushouID] < 0 ) {
			arrRemainMoveCountAllBushou[iBushouID] = 0;
		}
	}
}


void OnTenshouExeOnActionEndInBattleExecute() {

	WORD *pBushouID = (WORD *)iCurMainBushouPointerOnActionEndInBattle;
	WORD iBushouID = (*pBushouID) -1;

	if ( getRemainTurnInBattle() <= 30 ) {
		if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			// 初配置済み以降であれば、デバッグ出力
			// 初配置の時は、配置だけで「行動した」と見なされてしまうため、その対処。
			if ( arrIsFirstHaichiAction[iBushouID] ) {
				// 各々のユニットが実際に行動した際の処理
				DoTenshouExeOnActonEndInBattleExecute(iBushouID);
			}

			// 初配置情報を確保。
			// 初配置の時は、配置だけで「行動した」と見なされてしまうため、その対処。
			arrIsFirstHaichiAction[iBushouID] = true;
		}
	}
}


/*
0042DC89  |. E8 4BFEFFFF    CALL    TENSHOU.0042DAD9
				└ここを JMP TSMod.OnTSExeOnActionEndInBattle と書き換えてやる実際の処理
0042DC8E  |. 8B4C24 0C      MOV     ECX, DWORD PTR SS:[ESP+C]
0042DC92  |. 83C4 04        ADD     ESP, 4
0042DC95     0948 18        OR      DWORD PTR DS:[EAX+18], ECX
0042DC98  \. C3             RETN
*/
int pTenshouExeJumpFromToOnTenshouExeOnActionEndInBattle	 =0x42DC89; // 関数はこのアドレスから、OnTenshouExeOnActionEndInBattleへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeOnActionEndInBattle =0x42DAD9; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeOnActionEndInBattle	 =0x42DC98; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeOnActionEndInBattle() {
	// スタックにためておく
	__asm {

		// 元々TENSHOU.EXE内にあったものをここで。この中で、行動済(=0x20)へと設定される
		call pTenshouExeJumpCallFromToOnTenshouExeOnActionEndInBattle

		MOV     ECX, DWORD PTR SS:[ESP+0xC]
		ADD     ESP, 4
		OR      DWORD PTR DS:[EAX+0x18], ECX


		// 武将のIDを保存 SIの部分のみが武将番号
		mov iCurBushouAIDOnOnActionEndInBattle, esi
		mov iCurBushouBIDOnOnActionEndInBattle, edi

		mov iCurMainBushouPointerOnActionEndInBattle, eax // 0x4D5D58など、例の戦争時の各武将情報へのポインタが入ってくる。

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
	OnTenshouExeOnActionEndInBattleExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeOnActionEndInBattle
	}
}

/*
0049D7AA  |. 6A 03          |PUSH    3                               ; |Arg3 = 00000003
0049D7AC  |. 56             |PUSH    ESI                             ; |Arg2
0049D7AD  |. 56             |PUSH    ESI                             ; |Arg1
0049D7AE  |. E8 D3540000    |CALL    TENSHOU.004A2C86                ; \TENSHOU.004A2C86
				└ここを JMP TSMod.OnTSExeOnActionEndInBattle と書き換えてやる実際の処理
0049D7B3  |. 83C4 1C        |ADD     ESP, 1C
0049D7B6  |. 85C0           |TEST    EAX, EAX
*/
char cmdOnTenshouExeJumpFromOnActionEndInBattle[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeOnActionEndInBattle() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeOnActionEndInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeOnActionEndInBattle + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeOnActionEndInBattle  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromOnActionEndInBattle+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeOnActionEndInBattle), cmdOnTenshouExeJumpFromOnActionEndInBattle, 5, NULL); //5バイトのみ書き込む
}




