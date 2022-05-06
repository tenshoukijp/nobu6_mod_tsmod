#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"



int iWeatherRainRangeJudge = 0;
bool isUseCurBushouAmeDeppou = false; // この武将が雨鉄砲を使うかどうか

int iCurBushouIDOnRainRangeJudge = -1; // TENSHOU.EXEから渡ってくる対象の武将


bool isCurBushouHasAHoujutsusyo(int iBushouID) {
	// 家宝を一通り調べる
	for (int iKahouID=0; iKahouID<GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {

		// 所有者の武将IDが一致する場合のみ判定の対象
		if (nb6kahou[iKahouID].attach-1 == iBushouID) {

			// 対象の家宝が「砲術書」という文字、もしくは「砲書」という文字を含んでいる場合 true
			if ( strstr(nb6kahou[iKahouID].name, "砲術書") ) {
				return true;
			}
			if ( strstr(nb6kahou[iKahouID].name, "砲書") ) {
				return true;
			}
		}
	}
	// 見つからなければfalse
	return false;
}

// 対象武将は雨鉄砲が可能かどうか
bool IsCanAmeDeppou(int iBushouID ) {

	if (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		// タルタロスならばありえない天気と比較する。すなわち、鉄砲は必ず撃てる。
		if (isLancelotTartare(iBushouID)) {

			iWeatherRainRangeJudge = 7;
			isUseCurBushouAmeDeppou = true;
			return true;

		// 朱伊ならばありえない天気と比較する。すなわち、鉄砲は必ず撃てる。
		} else if (isShuiHidechiyo(iBushouID)) {

			iWeatherRainRangeJudge = 7;
			isUseCurBushouAmeDeppou = true;
			return true;

		// 砲術書・砲書を持っている武将も雨鉄砲が撃てるものとする。
		} else if ( isCurBushouHasAHoujutsusyo(iBushouID) ) {

			iWeatherRainRangeJudge = 7;
			isUseCurBushouAmeDeppou = true;
			return true;

		} else {

			// 関数がDLLで定義されていたら、CONDITIONとして呼び出し、その結果もTRUEであれば… 雨鉄砲撃つことが可能。
			if ( p_snOnCustomCondition ) {
				int iAmeDeppouCanResult = ((PFNSNONCUSTOMCONDITION) p_snOnCustomCondition)("OR条件::戦争::雨鉄砲可否", iBushouID+1, 0, 0, "", "", 0.0, 0.0);
				if ( iAmeDeppouCanResult == TRUE ) {
					iWeatherRainRangeJudge = 7;
					isUseCurBushouAmeDeppou = true;
					return true;
				}
			}

			return false;
		}
	}

	return false;
}

void OnTenshouExeRainRangeJudgeExecute() {

	//	cmp eax, 2
	// この2を別の値にする必要があるかどうかを判断する。2ではなく7などにすれば、「雨との比較ではなく、存在しない天気との比較」ことになるから、鉄砲や大砲が撃てる。

	// まず２．これは状況にかかわりなく必要。
	iWeatherRainRangeJudge = 2;
	isUseCurBushouAmeDeppou = false;

	int iBushouID = iCurBushouIDOnRainRangeJudge-1;

	if (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		if (IsCanAmeDeppou(iBushouID)) {
			iWeatherRainRangeJudge = 7;
			isUseCurBushouAmeDeppou = true;
		}

		// 弓隊もしくは弓騎馬隊であれば、雨鉄砲可能となる
		// 足軽で弓がＢ以上であれば…
		if ( nb6bushouref[iBushouID].form == 0 && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3) { // 足軽で弓がＢ以上であれば…
			iWeatherRainRangeJudge = 7;

		// 騎馬適正がＢ以上で、弓適性もＢ以上ならば、リモート攻撃可能
		} else if ( nb6bushouref[iBushouID].form == 1 && nb6bushouref[iBushouID].aptitudekib >= 3 && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3) {
			iWeatherRainRangeJudge = 7;
		}

	}
}


/*
/*
0042B3F0  |. FE45 FC        INC     BYTE PTR SS:[EBP-4]
0042B3F3  |> E8 32AF0600    CALL    TENSHOU.0049632A		← ここでEAXに天気情報を入れている 0が晴れ、1が曇り、2が雨
				└ここを JMP TSMod.OnTSExeRainRangeJudge と書き換えてやる実際の処理
0042B3F8  |. 83F8 02        CMP     EAX, 2					← ここで雨かどうか判定している0が晴れ、1が曇り、2が雨
0042B3FB  |. 72 10          JB      SHORT TENSHOU.0042B40D
0042B3FD  |. 837D F4 1B     CMP     DWORD PTR SS:[EBP-C], 1B
*/
int pTenshouExeJumpFromToOnTenshouExeRainRangeJudge		=0x42B3F3; // 関数はこのアドレスから、OnTenshouExeRainRangeJudgeへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeRainRangeJudge =0x49632A; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeRainRangeJudge	=0x42B3FB; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeRainRangeJudge() {
	// スタックにためておく
	__asm {
		// 武将のIDを保存
		mov iCurBushouIDOnRainRangeJudge, edi

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeRainRangeJudge

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
	OnTenshouExeRainRangeJudgeExecute();

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

		cmp eax, iWeatherRainRangeJudge		// デフォルトでは、iWeatherRainRangeJudgeには２(雨)が入るが、7(存在しえない天気)などにすれば必ず鉄砲が撃てることとなる。

		jmp pTenshouExeReturnLblFromOnTenshouExeRainRangeJudge
	}
}

/*
0049D7AA  |. 6A 03          |PUSH    3                               ; |Arg3 = 00000003
0049D7AC  |. 56             |PUSH    ESI                             ; |Arg2
0049D7AD  |. 56             |PUSH    ESI                             ; |Arg1
0049D7AE  |. E8 D3540000    |CALL    TENSHOU.004A2C86                ; \TENSHOU.004A2C86
				└ここを JMP TSMod.OnTSExeRainRangeJudge と書き換えてやる実際の処理
0049D7B3  |. 83C4 1C        |ADD     ESP, 1C
0049D7B6  |. 85C0           |TEST    EAX, EAX
*/
char cmdOnTenshouExeJumpFromRainRangeJudge[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeRainRangeJudge() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeRainRangeJudge;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeRainRangeJudge + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeRainRangeJudge  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromRainRangeJudge+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeRainRangeJudge), cmdOnTenshouExeJumpFromRainRangeJudge, 5, NULL); //5バイトのみ書き込む
}




