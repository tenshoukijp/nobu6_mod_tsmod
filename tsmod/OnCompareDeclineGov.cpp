#include "WinTarget.h"
#include <fstream>

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "TSModCommand.h"


/*
 * 戦才あるいは政治が老いて減少する判定がされる時
 * 毎季節で、武将ごとに政治能力の老いの年齢比較がなされる時
 */
int iCompareDeclineGovBushouID = -1;

const int iCompareDeclineGovDefault = 0x46;	 // 0x46==70才　政治は70歳以上で老い始める

int iCompareDeclineGovValue = -1;
int iCurDeclineGovOld = -1;			 // 現在の対象武将の年齢


// 対象武将の政治が衰える年齢
int getBeginDeclineGovYears(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		// ランスロットタルタロスであれば…
		if ( isLancelotTartare( iBushouID ) ) {
			return iCompareDeclineGovDefault + 30;	// 他の武将より30才遅く老い始める

		// 朱伊であれば…
		} else if ( isShuiHidechiyo(iBushouID) ) {
			return iCompareDeclineGovDefault + 400;	// 他の武将より400才遅く老い始める

		// 職業が騎士であれば…
		} else if ( nb6bushouref[iBushouID].job == 0xA0 ) {
			return iCompareDeclineGovDefault + 20;	// 他の武将より20才遅く老い始める

		// 職業が魔術師であれば…
		} else if ( nb6bushouref[iBushouID].job == 0xB0 ) {
			return iCompareDeclineGovDefault + 50;	// 他の武将より50才遅く老い始める

		// 職業が魔族であれば…
		} else if ( nb6bushouref[iBushouID].job == 0xC0 ) {
			return iCompareDeclineGovDefault + 100;	// 他の武将より100才遅く老い始める
		} else {
			return iCompareDeclineGovDefault;
		}
	}

	return iCompareDeclineGovDefault;
}

void OnTenshouExeCompareDeclineGovExecute() {
	// 必要があれば iCompareDeclineGovValue を TSModCommand等により変更することで、｢老い始める年齢｣を変更することが可能。
	// 武将単位で毎度ここはループしてくるので、武将単位で変化させることも可能。

	// 毎回デフォルト値にすること
	iCompareDeclineGovValue = iCompareDeclineGovDefault;  // 老い始めるデフォルト値は毎度代入

	// 武将ID範囲チェック
	if ( 0 <= iCompareDeclineGovBushouID-1 && iCompareDeclineGovBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM ) {
		iCompareDeclineGovValue = getBeginDeclineGovYears(iCompareDeclineGovBushouID-1);
	}

	// 老齢による能力減衰なしであれば…
	if ( TSModCommand::AllBushou::cmdNoDeclineTalentParam ) {
		iCompareDeclineGovValue = 10000; // 10000才以上であれば能力減衰
	}

}


/*
00441DFD  |> 83FF 46        CMP     EDI, 46
				└ここを JMP TSMod.OnTSExeCompareDeclineGov と書き換えてやる実際の処理
00441E00  |. 77 28          JA      SHORT TENSHOU.00441E2A
00441E02  |. 83EF 41        SUB     EDI, 41
*/
int pTenshouExeJumpFromToOnTenshouExeCompareDeclineGov	     =0x441DFD; // 関数はこのアドレスから、OnTenshouExeCompareDeclineGovへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeCompareDeclineGov   =0x441E2A; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeCompareDeclineGov    =0x441E02; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w

// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeCompareDeclineGov() {
	// スタックにためておく
	__asm {
		// 現在の対象武将の年齢をコピー
		mov iCurDeclineGovOld, edi

		// 対象の武将IDをコピー
		mov iCompareDeclineGovBushouID, esi

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
	OnTenshouExeCompareDeclineGovExecute();

	// 現在の年齢が、政治が老い始めると定義された年齢より小さい場合
	// 00441E00  |. 77 28          JA      SHORT TENSHOU.00441E2A と同じことをする。
	if ( iCurDeclineGovOld < iCompareDeclineGovValue ) {
		__asm {

			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			jmp pTenshouExeJumpCallFromToOnTenshouExeCompareDeclineGov
		}

	// 00441E02  |. 83EF 41        SUB     EDI, 41　の行へとジャンプする。
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

			jmp pTenshouExeReturnLblFromOnTenshouExeCompareDeclineGov
		}
	}
}

/*
00441DD0  |> 83FF 3C        CMP     EDI, 3C							
				└ここを JMP TSMod.OnTSExeCompareDeclineGov と書き換えてやる実際の処理
00441DD3  |. 77 28          JA      SHORT TENSHOU.00441DFD							
00441DD5  |. 8D47 C9        LEA     EAX, DWORD PTR DS:[EDI-37]							
*/
char cmdOnTenshouExeJumpFromCompareDeclineGov[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeCompareDeclineGov() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeCompareDeclineGov;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeCompareDeclineGov + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeCompareDeclineGov  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromCompareDeclineGov+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeCompareDeclineGov), cmdOnTenshouExeJumpFromCompareDeclineGov, 5, NULL); //5バイトのみ書き込む
}




