#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "TSModCommand.h"
#include "ScenarioMod.h"

/*
 * 戦闘時に内応コメントの結果が出た時
 * (その結果を覆したい場合用)
 */

/*
// ここに来ると内応は成功してると考えてよい
0043345F  |> E8 84330600    CALL    TENSHOU.004967E8
00433464  |. 85C0           TEST    EAX, EAX
00433466  |. 75 19          JNZ     SHORT TENSHOU.00433481
00433468  |. 55             PUSH    EBP                              ; /Arg3			
00433469  |. 57             PUSH    EDI                              ; |Arg2			← 下位4バイトが内応される側の武将番号  上位にいは0x18が入ることがある。
0043346A  |. 68 C30F0000    PUSH    0FC3                             ; |Arg1 = 00000FC3
0043346F  |. E8 0B900300    CALL    TENSHOU.0046C47F                 ; \TENSHOU.0046C47F
00433474  |. 83C4 0C        ADD     ESP, 0C



// この4334DEにくる時は、内応は失敗している。
004334DE  |> E8 05330600    CALL    TENSHOU.004967E8
004334E3  |. 85C0           TEST    EAX, EAX
004334E5  |. 75 19          JNZ     SHORT TENSHOU.00433500
004334E7  |. 55             PUSH    EBP                              ; /Arg3			
004334E8  |. 57             PUSH    EDI                              ; |Arg2			← 下位4バイトが内応される側の武将番号  上位にいは0x18が入ることがある。
004334E9  |. 68 C40F0000    PUSH    0FC4                             ; |Arg1 = 00000FC4
004334EE  |. E8 8C8F0300    CALL    TENSHOU.0046C47F                 ; \TENSHOU.0046C47F
004334F3  |. 83C4 0C        ADD     ESP, 0C
*/

int iGlobalResultOfNaiouInBattle = FALSE;
// 内応の結果を上書きする。
BOOL OverrideResultOfNaiouInBattle(BOOL iCurrentResult, int iBushouID, int iTargetID) {

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= iTargetID && iTargetID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ランスロットタルタロスが仕掛ける側であれば、有無を言わせず内応は成功
		if ( isLancelotTartare(iBushouID) ) {
			iCurrentResult = TRUE;
		// ランスロットタルタロスが仕掛けられる側であれば、有無を言わせず内応は成功
		} else if ( isLancelotTartare(iTargetID) ) {
			iCurrentResult = FALSE;
		// デネブが仕掛ける側であれば、有無を言わせず内応は成功
		} else if ( strcmp( nb6bushouname[iBushouID].familyname, "" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "" )==0 ) {
			iCurrentResult = TRUE;
		}

		// SceanrioModで結果を上書きするならするが良い。
		if ( p_snOnCustomCondition ) {																				  // ┌内応した側	┌内応された側	┌ 予定されてる内応結果
			int isScenarioModNaiouResult = ((PFNSNONCUSTOMCONDITION) p_snOnCustomCondition)("AS条件::戦争::内応結果可否", iBushouID+1,      iTargetID+1,    iCurrentResult, "", "", 0.0, 0.0);
			// 非修正要綱でなければ…
			if ( isScenarioModNaiouResult != CUSTOM_CONDITION_NOMODIFY ) {
				iCurrentResult = isScenarioModNaiouResult;
			}
		}
	} 

	// 現在の結果をそのままグローバルに代入
	iGlobalResultOfNaiouInBattle = iCurrentResult; // 結果を上書き。

	// 一応リターン
	return iGlobalResultOfNaiouInBattle;
}



int iBushouIDOfResultOfNaiouInBattlle = -1; // 内応の行動側(仕掛けた側)の武将番号
int iTargetIDOfResultOfNaiouInBattlle = -1; // 内応の手動側(仕掛けられた側)の武将番号



void OnTenshouExeResultOfNaiouSuccessInBattleExecute() {

	int iBushouID = (iBushouIDOfResultOfNaiouInBattlle & 0xFFFF)-1; // 下半分だけ
	int iTargetID = (iTargetIDOfResultOfNaiouInBattlle & 0xFFFF)-1; // 下半分だけ

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= iTargetID && iTargetID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		OverrideResultOfNaiouInBattle(TRUE, iBushouID, iTargetID); // TRUE = ここにきている時点で、内応の結果は成功しているが、この関数によって結果を変えたいかもしれない。
	}
}


/*
0043345F  |> E8 84330600    CALL    TENSHOU.004967E8
00433464  |. 85C0           TEST    EAX, EAX
				└ここを JMP TSMod.OnTSExeResultOfNaiouSuccessInBattle と書き換えてやる実際の処理
 */
int pTenshouExeJumpFromToOnTenshouExeResultOfNaiouSuccessInBattle    =0x43345F; // 関数はこのアドレスから、OnTenshouExeResultOfNaiouSuccessInBattleへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeResultOfNaiouSuccessInBattle=0x4967E8; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeResultOfNaiouSuccessInBattle =0x433464; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
extern int pTenshouExeReturnLblFromOnTenshouExeResultOfNaiouFailInBattle;
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeResultOfNaiouSuccessInBattle() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeResultOfNaiouSuccessInBattle

		// 内応する側の武将番号を保存
		mov iBushouIDOfResultOfNaiouInBattlle,  ESI

		// 内応された側の武将番号を保存
		mov iTargetIDOfResultOfNaiouInBattlle,  EDI

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
	OnTenshouExeResultOfNaiouSuccessInBattleExecute();

	// もし結果が成功であったなら、通常通りの場所にリターンする。
	if ( iGlobalResultOfNaiouInBattle == TRUE ) {
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

			jmp pTenshouExeReturnLblFromOnTenshouExeResultOfNaiouSuccessInBattle
		}

	// もし内応結果をFALSEへと上書きされたのであれば、失敗した場合の場所へとリターンする。
	} else {
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

			jmp pTenshouExeReturnLblFromOnTenshouExeResultOfNaiouFailInBattle
		}
	}
}

char cmdOnTenshouExeJumpFromResultOfNaiouSuccessInBattle[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeResultOfNaiouSuccessInBattle() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeResultOfNaiouSuccessInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeResultOfNaiouSuccessInBattle + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeResultOfNaiouSuccessInBattle  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromResultOfNaiouSuccessInBattle+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeResultOfNaiouSuccessInBattle), cmdOnTenshouExeJumpFromResultOfNaiouSuccessInBattle, 5, NULL); //5バイトのみ書き込む
}













void OnTenshouExeResultOfNaiouFailInBattleExecute() {

	int iBushouID = (iBushouIDOfResultOfNaiouInBattlle & 0xFFFF)-1; // 下半分だけ
	int iTargetID = (iTargetIDOfResultOfNaiouInBattlle & 0xFFFF)-1; // 下半分だけ

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= iTargetID && iTargetID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		OverrideResultOfNaiouInBattle(FALSE, iBushouID, iTargetID); // FALSE = ここにきている時点で、内応の結果は失敗しているが、この関数によって結果を変えたいかもしれない。
	}
}


/*
004334DE  |> E8 05330600    CALL    TENSHOU.004967E8
004334E3  |. 85C0           TEST    EAX, EAX
				└ここを JMP TSMod.OnTSExeResultOfNaiouFailInBattle と書き換えてやる実際の処理
 */
int pTenshouExeJumpFromToOnTenshouExeResultOfNaiouFailInBattle    =0x4334DE; // 関数はこのアドレスから、OnTenshouExeResultOfNaiouFailInBattleへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeResultOfNaiouFailInBattle=0x4967E8; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeResultOfNaiouFailInBattle =0x4334E3; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
extern int pTenshouExeReturnLblFromOnTenshouExeResultOfNaiouSuccessInBattle;
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeResultOfNaiouFailInBattle() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeResultOfNaiouFailInBattle

		// 内応する側の武将番号を保存
		mov iBushouIDOfResultOfNaiouInBattlle,  ESI

		// 内応された側の武将番号を保存
		mov iTargetIDOfResultOfNaiouInBattlle,  EDI

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
	OnTenshouExeResultOfNaiouFailInBattleExecute();

	// もし結果が失敗であったなら、通常通りの場所にリターンする。
	if ( iGlobalResultOfNaiouInBattle == FALSE ) {

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

			jmp pTenshouExeReturnLblFromOnTenshouExeResultOfNaiouFailInBattle
		}

	// もし内応結果がTRUEへと上書きされたのであれば、成功した場合の場所へとリターンする。
	} else {
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

			jmp pTenshouExeReturnLblFromOnTenshouExeResultOfNaiouSuccessInBattle
		}
	}
}

char cmdOnTenshouExeJumpFromResultOfNaiouFailInBattle[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeResultOfNaiouFailInBattle() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeResultOfNaiouFailInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeResultOfNaiouFailInBattle + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeResultOfNaiouFailInBattle  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromResultOfNaiouFailInBattle+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeResultOfNaiouFailInBattle), cmdOnTenshouExeJumpFromResultOfNaiouFailInBattle, 5, NULL); //5バイトのみ書き込む
}
