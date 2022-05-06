#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"

#include "ScenarioMod.h"

extern int isCurBushouHasAUma(int iBushouID);

// iPrev現在の値、iNext変更後の値
int OnTenshouExeChangeMovePower(int iBushouID, int iPrev, int iNext) {

	int iModifiedNext = iNext;

	// 回復時
	if (iNext > iPrev) {
		// タルタロスであれば…
		if ( isLancelotTartare( iBushouID ) ) {
			if ( iNext < 12 ) {
				iModifiedNext = 15; // 最低でも15は約束される。
			}
			if ( iNext < 28 ) {
				iModifiedNext = iNext + 3; // 移動残量が3増やされる。
			}
		}
		// 朱伊であれば…
		else if ( isShuiHidechiyo(iBushouID) ) {
			if (iNext < 12) {
				iModifiedNext = 12; // 最低でも12は約束される。
			}
			if (iNext < 28) {
				iModifiedNext = iNext + 3; // 移動残量が3増やされる。
			}
		}
		// 魔族、及び、忍者であれば…
		else if ( nb6bushouref[iBushouID].job == 0xC0 || nb6bushouref[iBushouID].job ==0x10 ) {
			if ( iNext < 25 ) {
				iModifiedNext = iNext + 3; // 移動残量が3増やされる。
			}
		}

		else {
			// 馬を持っている
			int iHasUmaRank = isCurBushouHasAUma(iBushouID);
			if ( iHasUmaRank > 0 ) {
				if ( iNext < (0xF + iHasUmaRank) ) { // 通常値の15+馬ランク以下であれば…
					iModifiedNext = iNext + (1 + int(iHasUmaRank/5));  // 移動残量が1～3増やされる。
				}
			}
		}
	} // 回復時終わり

	/*
	OutputDebugStream << nb6bushouname[iBushouID].fastname << ":";
	OutputDebugStream << "現在の移動力:" << iPrev << ",";
	OutputDebugStream << "次の移動力:" << iNext << endl;
	*/

	// ScenarioModに定義されてれば、上書き
	if (p_snOnChangeUnitMovePowerInBattle) {
		iModifiedNext = p_snOnChangeUnitMovePowerInBattle(iBushouID, iPrev, iModifiedNext);
	}

	if (iNext != iModifiedNext) {
		return iModifiedNext;
	} else {
		return -1;
	}
}


// 移動力残量が変更された時。「消費のタイミング」と「回復のタイミング」がある。
static int iEAXOfSettingMovePowerBushouUnitNo = NULL;
static int pNextPowerOfSettingMovePower = 0;
void OnTenshouExeSettingMovePowerInBattleExecute() {

	int iNo = (iEAXOfSettingMovePowerBushouUnitNo & 0xFFFF) -1;
	int iBushouID = nb6bushou_hex_unit_info[iNo].bushou - 1;
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int iPrev = (int)(nb6bushou_hex_unit_info[iNo].movepower);
		int iNext = *(int *)(pNextPowerOfSettingMovePower);
		int iValueOfOverWrite = OnTenshouExeChangeMovePower(iBushouID, iPrev, iNext);

		// 変更するという意思が明示されたなら、上書きする。スタックを書き換える
		if (iValueOfOverWrite != -1) {
			*(int *)(pNextPowerOfSettingMovePower) = iValueOfOverWrite;
		}
	}
}


/*
0040C20D  |. 74 0D          JE      SHORT TENSHOU.0040C21C
0040C20F  |. FF7424 08      PUSH    DWORD PTR SS:[ESP+8]		新たな移動力残量のプッシュ
0040C213  |. 50             PUSH    EAX							下位のビットが戦争時の配列の番号(NB6BUSHOU_HEX_UNIT_INFOの配列番号+1)を指す模様
0040C214  |. E8 E9190200    CALL    TENSHOU.0042DC02
0040C219  |. 83C4 08        ADD     ESP, 8
0040C21C  \> C3             RETN

*/
int pTenshouExeJumpFromToOnTenshouExeSettingMovePowerInBattle		=0x40C214; // 関数はこのアドレスから、OnTenshouExeSettingMovePowerInBattleへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeSettingMovePowerInBattle   =0x42DC02; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeSettingMovePowerInBattle	=0x40C219; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeSettingMovePowerInBattle() {
	// スタックにためておく
	__asm {

		mov iEAXOfSettingMovePowerBushouUnitNo, EAX  // ユニットの番号ix

		// 一旦EAXにコピー
		mov eax, ESP
		add eax, 0x4

		mov pNextPowerOfSettingMovePower, EAX  // 次に設定すべき移動残量

		mov eax, iEAXOfSettingMovePowerBushouUnitNo // eaxを元に戻す

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
	OnTenshouExeSettingMovePowerInBattleExecute();

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

		// 元々の残量変更処理をここで
		call pTenshouExeJumpCallFromToOnTenshouExeSettingMovePowerInBattle

		jmp pTenshouExeReturnLblFromOnTenshouExeSettingMovePowerInBattle
	}
}

char cmdOnTenshouExeJumpFromSettingMovePowerInBattle[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeSettingMovePowerInBattle() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeSettingMovePowerInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSettingMovePowerInBattle + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeSettingMovePowerInBattle  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromSettingMovePowerInBattle+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSettingMovePowerInBattle), cmdOnTenshouExeJumpFromSettingMovePowerInBattle, 5, NULL); //5バイトのみ書き込む
}




