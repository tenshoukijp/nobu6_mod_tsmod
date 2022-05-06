
#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "TSModCommand.h"


/*
 武将が浪人から現役になったり、戦後処理等で、捕虜登場時に、勲功等の操作
*/
/*
0040BC4A  |. 50             PUSH    EAX
0040BC4B  |> 57             PUSH    EDI
0040BC4C  |. E8 54DDFFFF    CALL    TENSHOU.004099A5	EDIが対象の武将
0040BC51  |. 83C4 08        ADD     ESP, 8
*/


int iCurBushouIDInUpdateToGennekiKunkouParam = -1;
int iCurBushouKunkouValueInUpdateToGennekiKunkouParam = -1;

void OnTenshouExeUpdateToGennekiKunkouParamExecuteBefore() { // 前

	int iBushouID = iCurBushouIDInUpdateToGennekiKunkouParam-1;

	// 勲功リセットしない
	if ( TSModCommand::AllBushou::cmdNonMeritoriousReset ) {
		// 武将が一定の範囲であること
		if (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {
			if ( nb6bushouname[iBushouID].State == 4 ) { // 浪人からの登用の時
				iCurBushouKunkouValueInUpdateToGennekiKunkouParam = nb6bushouref[iBushouID].merits; // 勲功値を一端保存
			}
		}
	}
}

void OnTenshouExeUpdateToGennekiKunkouParamExecuteAfter() { // 後

	int iBushouID = iCurBushouIDInUpdateToGennekiKunkouParam-1;

	// 勲功リセットしない
	if ( TSModCommand::AllBushou::cmdNonMeritoriousReset ) {
		// 武将が一定の範囲であること
		if (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {
			if ( nb6bushouname[iBushouID].State == 4 ) { // 浪人からの登用の時
				nb6bushouref[iBushouID].merits = iCurBushouKunkouValueInUpdateToGennekiKunkouParam; // 勲功値を復元。

				// 今足軽ならば… 勲功に応じて、昇進させる。
				if ( nb6bushouname[iBushouID].position == 6 ) { // 今足軽なら、昇進させても大丈夫

					/*
					#define GAMEDATASTRUCT_REQUIREDMERITS_NUM		5
					// [宿老・家老・武将・侍大将・足軽頭]の５種類が定義されている。
					nb6requiredmerits[i] 
					//身分		(隠居=0, 大名=1, 宿老=2, 家老=3, 部将=4, 侍大将=5, 足軽頭=6)
					*/
					if ( nb6bushouref[iBushouID].merits >= nb6requiredmerits[0].merits ) { // 宿老級勲功を持つ。
						nb6bushouname[iBushouID].position = 2; // 宿老へ
					} else if ( nb6bushouref[iBushouID].merits >= nb6requiredmerits[1].merits ) { // 家老級勲功を持つ。
						nb6bushouname[iBushouID].position = 3; // 家老へ
					} else if ( nb6bushouref[iBushouID].merits >= nb6requiredmerits[2].merits ) { // 部将級勲功を持つ。
						nb6bushouname[iBushouID].position = 4; // 部将へ
					} else if ( nb6bushouref[iBushouID].merits >= nb6requiredmerits[3].merits ) { // 侍大将級勲功を持つ。
						nb6bushouname[iBushouID].position = 5; // 侍大将へ
					}
				}
			}
		}
	}
}

/*
0040BC4A  |. 50             PUSH    EAX
0040BC4B  |> 57             PUSH    EDI
0040BC4C  |. E8 54DDFFFF    CALL    TENSHOU.004099A5	EDIが対象の武将
				└ここを JMP TSMod.OnTSExeUpdateToGennekiKunkouParam と書き換えてやる実際の処理
0040BC51  |. 83C4 08        ADD     ESP, 8
*/
int pTenshouExeJumpFromToOnTenshouExeUpdateToGennekiKunkouParam	     =0x40BC4C; // 関数はこのアドレスから、OnTenshouExeUpdateToGennekiKunkouParamへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeUpdateToGennekiKunkouParam  =0x4099A5; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeUpdateToGennekiKunkouParam   =0x40BC51; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeUpdateToGennekiKunkouParam() {
	// スタックにためておく
	__asm {
		// 武将IDを保存
		mov iCurBushouIDInUpdateToGennekiKunkouParam, edi

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
	OnTenshouExeUpdateToGennekiKunkouParamExecuteBefore();

	__asm {
		// スタックを元に戻して実行
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeUpdateToGennekiKunkouParam

		// 再びスタックにためる
		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnTenshouExeUpdateToGennekiKunkouParamExecuteAfter();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeUpdateToGennekiKunkouParam
	}
}

/*
*/
char cmdOnTenshouExeJumpFromUpdateToGennekiKunkouParam[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeUpdateToGennekiKunkouParam() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeUpdateToGennekiKunkouParam;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeUpdateToGennekiKunkouParam + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeUpdateToGennekiKunkouParam  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromUpdateToGennekiKunkouParam+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeUpdateToGennekiKunkouParam), cmdOnTenshouExeJumpFromUpdateToGennekiKunkouParam, 5, NULL); //5バイトのみ書き込む
}




