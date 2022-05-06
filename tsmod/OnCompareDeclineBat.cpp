#include "WinTarget.h"
#include <fstream>

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "TSModCommand.h"



/*
 * 戦才あるいは戦闘が老いて減少する判定がされる時
 * 毎季節で、武将ごとに戦闘能力の老いの年齢比較がなされる時
 */
int iCompareDeclineBatBushouID = -1;

const int iCompareDeclineBatDefault = 0x32;	 // 0x32==50才　戦闘は50歳以上で老い始める

int iCompareDeclineBatValue = -1;
int iCurDeclineBatOld = -1;			 // 現在の対象武将の年齢


// 対象武将の戦闘が衰える年齢
int getBeginDeclineBatYears(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		// ランスロットタルタロスであれば…
		if ( isLancelotTartare( iBushouID ) ) {
			return iCompareDeclineBatDefault + 30;	// 他の武将より30才遅く老い始める

		// 朱伊であれば…
		} else if ( isShuiHidechiyo(iBushouID) ) {
			return iCompareDeclineBatDefault + 400;	// 他の武将より400才遅く老い始める

		// 職業が騎士であれば…
		} else if ( nb6bushouref[iBushouID].job == 0xA0 ) {
			return iCompareDeclineBatDefault + 20;	// 他の武将より20才遅く老い始める

		// 職業が魔術師であれば…
		} else if ( nb6bushouref[iBushouID].job == 0xB0 ) {
			return iCompareDeclineBatDefault + 50;	// 他の武将より50才遅く老い始める

		// 職業が魔族であれば…
		} else if ( nb6bushouref[iBushouID].job == 0xC0 ) {
			return iCompareDeclineBatDefault + 100;	// 他の武将より100才遅く老い始める
		} else {
			return iCompareDeclineBatDefault;
		}
	}

	return iCompareDeclineBatDefault;
}
void OnTenshouExeCompareDeclineBatExecute() {
	// 必要があれば iCompareDeclineBatValue を TSModCommand等により変更することで、｢老い始める年齢｣を変更することが可能。
	// 武将単位で毎度ここはループしてくるので、武将単位で変化させることも可能。

	// 毎回デフォルト値にすること
	iCompareDeclineBatValue = iCompareDeclineBatDefault;  // 老い始めるデフォルト値は毎度代入

	// 武将ID範囲チェック
	if ( 0 <= iCompareDeclineBatBushouID-1 && iCompareDeclineBatBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM ) {
		iCompareDeclineBatValue = getBeginDeclineBatYears(iCompareDeclineBatBushouID-1);
	}

	// 老齢による能力減衰なしであれば…
	if ( TSModCommand::AllBushou::cmdNoDeclineTalentParam ) {
		iCompareDeclineBatValue = 10000; // 10000才以上であれば能力減衰
	}

}


/*
00441DA3  |. 83FF 32        CMP     EDI, 32
				└ここを JMP TSMod.OnTSExeCompareDeclineBat と書き換えてやる実際の処理
00441DA6  |. 77 28          JA      SHORT TENSHOU.00441DD0
00441DA8  |. 8D47 D3        LEA     EAX, DWORD PTR DS:[EDI-2D]
*/
int pTenshouExeJumpFromToOnTenshouExeCompareDeclineBat	     =0x441DA3; // 関数はこのアドレスから、OnTenshouExeCompareDeclineBatへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeCompareDeclineBat   =0x441DD0; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeCompareDeclineBat    =0x441DA8; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w

// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeCompareDeclineBat() {
	// スタックにためておく
	__asm {
		// 現在の対象武将の年齢をコピー
		mov iCurDeclineBatOld, edi

		// 対象の武将IDをコピー
		mov iCompareDeclineBatBushouID, esi

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
	OnTenshouExeCompareDeclineBatExecute();

	// 現在の年齢が、戦闘が老い始めると定義された年齢より小さい場合
	// 00441DA6  |. 77 28          JA      SHORT TENSHOU.00441DD0 と同じことをする。
	if ( iCurDeclineBatOld < iCompareDeclineBatValue ) {
		__asm {

			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			jmp pTenshouExeJumpCallFromToOnTenshouExeCompareDeclineBat
		}

	// 00441DA8  |. 8D47 D3        LEA     EAX, DWORD PTR DS:[EDI-2D]　の行へとジャンプする。
	} else {

		__asm {

			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			jmp pTenshouExeReturnLblFromOnTenshouExeCompareDeclineBat
		}
	}
}

/*
00441DA3  |. 83FF 32        CMP     EDI, 32
				└ここを JMP TSMod.OnTSExeCompareDeclineBat と書き換えてやる実際の処理
00441DA6  |. 77 28          JA      SHORT TENSHOU.00441DD0
00441DA8  |. 8D47 D3        LEA     EAX, DWORD PTR DS:[EDI-2D]
*/
char cmdOnTenshouExeJumpFromCompareDeclineBat[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeCompareDeclineBat() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeCompareDeclineBat;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeCompareDeclineBat + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeCompareDeclineBat  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromCompareDeclineBat+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeCompareDeclineBat), cmdOnTenshouExeJumpFromCompareDeclineBat, 5, NULL); //5バイトのみ書き込む
}




