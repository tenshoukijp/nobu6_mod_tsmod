#include "WinTarget.h"
#include <fstream>

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "TSModCommand.h"



/*
 * 戦才あるいは智謀が老いて減少する判定がされる時
 * 毎季節で、武将ごとに智謀能力の老いの年齢比較がなされる時
 */
int iCompareDeclineIntBushouID = -1;

const int iCompareDeclineIntDefault = 0x3C;	 // 0x3C==60才　智謀は60歳以上で老い始める

int iCompareDeclineIntValue = -1;
int iCurDeclineIntOld = -1;			 // 現在の対象武将の年齢

// 対象武将の智謀が衰える年齢
int getBeginDeclineIntYears(int iBushouID) {

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		// ランスロットタルタロスであれば…
		if ( isLancelotTartare( iBushouID ) ) {
			return iCompareDeclineIntDefault + 30;	// 他の武将より30才遅く老い始める

		// 朱伊であれば…
		} else if ( isShuiHidechiyo(iBushouID) ) {
			return iCompareDeclineIntDefault + 400;	// 他の武将より400才遅く老い始める

		// 職業が騎士であれば…
		} else if ( nb6bushouref[iBushouID].job == 0xA0 ) {
			return iCompareDeclineIntDefault + 20;	// 他の武将より20才遅く老い始める

		// 職業が魔術師であれば…
		} else if ( nb6bushouref[iBushouID].job == 0xB0 ) {
			return iCompareDeclineIntDefault + 50;	// 他の武将より50才遅く老い始める

		// 職業が魔族であれば…
		} else if ( nb6bushouref[iBushouID].job == 0xC0 ) {
			return iCompareDeclineIntDefault + 100;	// 他の武将より100才遅く老い始める
		} else {
			return iCompareDeclineIntDefault;
		}
	}

	return iCompareDeclineIntDefault;
}

void OnTenshouExeCompareDeclineIntExecute() {
	// 必要があれば iCompareDeclineIntValue を TSModCommand等により変更することで、｢老い始める年齢｣を変更することが可能。
	// 武将単位で毎度ここはループしてくるので、武将単位で変化させることも可能。

	// 毎回デフォルト値にすること
	iCompareDeclineIntValue = iCompareDeclineIntDefault;  // 老い始めるデフォルト値は毎度代入

	// 武将ID範囲チェック
	if ( 0 <= iCompareDeclineIntBushouID-1 && iCompareDeclineIntBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM ) {
		iCompareDeclineIntValue = getBeginDeclineIntYears(iCompareDeclineIntBushouID-1);
	}

	// 老齢による能力減衰なしであれば…
	if ( TSModCommand::AllBushou::cmdNoDeclineTalentParam ) {
		iCompareDeclineIntValue = 10000; // 10000才以上であれば能力減衰
	}
}


/*
00441DD0  |> 83FF 3C        CMP     EDI, 3C							
				└ここを JMP TSMod.OnTSExeCompareDeclineInt と書き換えてやる実際の処理
00441DD3  |. 77 28          JA      SHORT TENSHOU.00441DFD							
00441DD5  |. 8D47 C9        LEA     EAX, DWORD PTR DS:[EDI-37]							
*/
int pTenshouExeJumpFromToOnTenshouExeCompareDeclineInt	     =0x441DD0; // 関数はこのアドレスから、OnTenshouExeCompareDeclineIntへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeCompareDeclineInt   =0x441DFD; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeCompareDeclineInt    =0x441DD5; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w

// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeCompareDeclineInt() {
	// スタックにためておく
	__asm {
		// 現在の対象武将の年齢をコピー
		mov iCurDeclineIntOld, edi

		// 対象の武将IDをコピー
		mov iCompareDeclineIntBushouID, esi

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
	OnTenshouExeCompareDeclineIntExecute();

	// 現在の年齢が、智謀が老い始めると定義された年齢より小さい場合
	// 00441DD3  |. 77 28          JA      SHORT TENSHOU.00441DFD と同じことをする。
	if ( iCurDeclineIntOld < iCompareDeclineIntValue ) {
		__asm {

			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			jmp pTenshouExeJumpCallFromToOnTenshouExeCompareDeclineInt
		}

	// 00441DD5  |. 8D47 C9        LEA     EAX, DWORD PTR DS:[EDI-37]　の行へとジャンプする。
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

			jmp pTenshouExeReturnLblFromOnTenshouExeCompareDeclineInt
		}
	}
}

/*
00441DD0  |> 83FF 3C        CMP     EDI, 3C							
				└ここを JMP TSMod.OnTSExeCompareDeclineInt と書き換えてやる実際の処理
00441DD3  |. 77 28          JA      SHORT TENSHOU.00441DFD							
00441DD5  |. 8D47 C9        LEA     EAX, DWORD PTR DS:[EDI-37]							
*/
char cmdOnTenshouExeJumpFromCompareDeclineInt[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeCompareDeclineInt() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeCompareDeclineInt;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeCompareDeclineInt + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeCompareDeclineInt  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromCompareDeclineInt+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeCompareDeclineInt), cmdOnTenshouExeJumpFromCompareDeclineInt, 5, NULL); //5バイトのみ書き込む
}




