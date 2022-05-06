#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "FuncPlayResourceWav.h"
#include "ScenarioMod.h"
#include "WaitTimeProcedure.h"



int iTeppouAttackBushouIDInTeppouDeath = -1; // 撃った側
int iTeppouDefendBushouIDInTeppouDeath = -1; // 撃たれた側


int iTeppouResultDeathOnJudge = -1;

void OnTenshouExeJudgeTeppouDeathInBattleExecute() {

	int iDefendID = iTeppouDefendBushouIDInTeppouDeath-1;
	int iAttackID = iTeppouAttackBushouIDInTeppouDeath-1;

	if ( 0 <= iAttackID && iAttackID < GAMEDATASTRUCT_BUSHOU_NUM && 0 <= iDefendID && iDefendID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 撃たれた側がランスロット・タルタロスである。
		// 決して死なぬ
		if ( isLancelotTartare(iDefendID) ) {
			// 撃った側が足軽もしくは、騎馬ならば、弓を放ったということなので、タルタロスは死なない
			if ( nb6bushouref[iAttackID].form == 0 || nb6bushouref[iAttackID].form == 1 ) {
				iTeppouResultDeathOnJudge = 0; // 結果は常にfalse
			}
		}

		// 撃たれた側が朱伊である。
		// 決して死なぬ
		if (isShuiHidechiyo(iDefendID)) {
			// 撃った側が足軽もしくは、騎馬ならば、弓を放ったということなので、朱伊は死なない
			if (nb6bushouref[iAttackID].form == 0 || nb6bushouref[iAttackID].form == 1) {
				iTeppouResultDeathOnJudge = 0; // 結果は常にfalse
			}
		}

		// 弓を放ってる
		if ( nb6bushouref[iAttackID].form == 0 || nb6bushouref[iAttackID].form == 1 ) {
			// 関数がDLLで定義されていたら、CONDITIONとして呼び出し、その結果がTRUEであれば、死亡させ、FALSEであれば、死亡しない。
			// ScenarioModで特に何もしないということを可能とするため、
			if ( p_snOnCustomCondition ) {																						          // ┌撃った側						 ┌打たれた側					┌ 予定されてる死亡結果
				int isScenarioModDeathResult = ((PFNSNONCUSTOMCONDITION) p_snOnCustomCondition)("AS条件::戦争::弓受動者死亡可否", iTeppouAttackBushouIDInTeppouDeath, iTeppouDefendBushouIDInTeppouDeath, iTeppouResultDeathOnJudge, "", "", 0.0, 0.0);
				// 非修正要綱でなければ…
				if ( isScenarioModDeathResult != CUSTOM_CONDITION_NOMODIFY ) {
					iTeppouResultDeathOnJudge = isScenarioModDeathResult;
				}
			}
		// 鉄砲を放ってる
		} else if ( nb6bushouref[iAttackID].form == 2 || nb6bushouref[iAttackID].form == 3 ) {
			// 関数がDLLで定義されていたら、CONDITIONとして呼び出し、その結果がTRUEであれば、死亡させ、FALSEであれば、死亡しない。
			// ScenarioModで特に何もしないということを可能とするため、
			if ( p_snOnCustomCondition ) {																						          // ┌撃った側						 ┌打たれた側					┌ 予定されてる死亡結果
				int isScenarioModDeathResult = ((PFNSNONCUSTOMCONDITION) p_snOnCustomCondition)("AS条件::戦争::鉄砲受動者死亡可否", iTeppouAttackBushouIDInTeppouDeath, iTeppouDefendBushouIDInTeppouDeath, iTeppouResultDeathOnJudge, "", "", 0.0, 0.0);
				// 非修正要綱でなければ…
				if ( isScenarioModDeathResult != CUSTOM_CONDITION_NOMODIFY ) {
					iTeppouResultDeathOnJudge = isScenarioModDeathResult;
				}
			}
		}
	}

}


/*
00432900  |. E8 10EDFFFF    CALL    TENSHOU.00431615                 ; \TENSHOU.00431615
							└ここを JMP TSMod.OnTSExeeJudgeTeppouDeathInBattle と書き換えてやる実際の処理
00432905  |. 83C4 0C        ADD     ESP, 0C
00432908  |. 85C0           TEST    EAX, EAX						← このEAXが死んだかどうかの判定
*/
int pTenshouExeJumpFromToOnTenshouExeJudgeTeppouDeathInBattle    =0x432900; // 関数はこのアドレスから、OnTenshouExeJudgeTeppouDeathInBattleへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeJudgeTeppouDeathInBattle=0x431615; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeJudgeTeppouDeathInBattle =0x432905; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeJudgeTeppouDeathInBattle() {
	// スタックにためておく
	__asm {

		mov iTeppouAttackBushouIDInTeppouDeath, esi		// 撃ったわがのＩＤをコピー
		mov iTeppouDefendBushouIDInTeppouDeath, edi		// 撃たれたがわＩＤをコピー

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeJudgeTeppouDeathInBattle;

		// 死亡の結果を保存
		mov iTeppouResultDeathOnJudge, eax

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
	OnTenshouExeJudgeTeppouDeathInBattleExecute();

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

		mov eax, iTeppouResultDeathOnJudge; // 死亡結果を上書き

		jmp pTenshouExeReturnLblFromOnTenshouExeJudgeTeppouDeathInBattle
	}
}

/*
00410467  |. 66:8BF0        MOV     SI, AX
0041046A  |. BF FFFF0000    MOV     EDI, 0FFFF
				└ここを JMP TSMod.OnTSExeJudgeTeppouDeathInBattle と書き換えてやる実際の処理
0041046F  |. E8 71CA0900    CALL    TENSHOU.004ACEE5
*/
char cmdOnTenshouExeJumpFromJudgeTeppouDeathInBattle[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeJudgeTeppouDeathInBattle() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeJudgeTeppouDeathInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeJudgeTeppouDeathInBattle + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeJudgeTeppouDeathInBattle  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromJudgeTeppouDeathInBattle+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeJudgeTeppouDeathInBattle), cmdOnTenshouExeJumpFromJudgeTeppouDeathInBattle, 5, NULL); //5バイトのみ書き込む
}








//--------------------------------------鉄砲返しの処理


int iTeppouHanekaesiInTeppouDeath = 0L;

void OnTenshouExeTeppouDeathInBattleExecute() {

	// 撃たれて死に予定の側がランスロット・タルタロスである。
	if (isLancelotTartare(iTeppouDefendBushouIDInTeppouDeath - 1)) {

		// 撃った側のＩＤ
		int iAttackID = iTeppouAttackBushouIDInTeppouDeath - 1;

		// 撃った側が鉄砲隊もしくは騎馬鉄砲隊であれば…
		// 鉄砲玉を弾き返して撃った側を死なせる処理
		if (nb6bushouref[iAttackID].form == 2 || nb6bushouref[iAttackID].form == 3) {

			int iBushouID = iTeppouDefendBushouIDInTeppouDeath - 1;

			// 鉄砲跳ね返し音
			FuncPlaySoundWav(721);

			WaitTimeProcedure(250);

			iTeppouHanekaesiInTeppouDeath = 1;

			// 撃った側と撃たれた側の交換
			int tmp = iTeppouDefendBushouIDInTeppouDeath;
			iTeppouDefendBushouIDInTeppouDeath = iTeppouAttackBushouIDInTeppouDeath;
			iTeppouAttackBushouIDInTeppouDeath = tmp;

			// 撃ったのが鉄砲ではなく、弓やであった場合
		}
		else if (nb6bushouref[iAttackID].form == 0 || nb6bushouref[iAttackID].form == 1) {
			iTeppouHanekaesiInTeppouDeath = 0;

			// ここには流れてこない
		}

	// 撃たれて死に予定の側が朱伊である。
	} else if (isShuiHidechiyo(iTeppouDefendBushouIDInTeppouDeath - 1)) {

		// 撃った側のＩＤ
		int iAttackID = iTeppouAttackBushouIDInTeppouDeath - 1;

		// 撃った側が鉄砲隊もしくは騎馬鉄砲隊であれば…
		// 鉄砲玉を弾き返して撃った側を死なせる処理
		if (nb6bushouref[iAttackID].form == 2 || nb6bushouref[iAttackID].form == 3) {

			int iBushouID = iTeppouDefendBushouIDInTeppouDeath - 1;

			// 鉄砲跳ね返し音
			FuncPlaySoundWav(721);

			WaitTimeProcedure(250);

			iTeppouHanekaesiInTeppouDeath = 1;

			// 撃った側と撃たれた側の交換
			int tmp = iTeppouDefendBushouIDInTeppouDeath;
			iTeppouDefendBushouIDInTeppouDeath = iTeppouAttackBushouIDInTeppouDeath;
			iTeppouAttackBushouIDInTeppouDeath = tmp;

			// 撃ったのが鉄砲ではなく、弓やであった場合
		}
		else if (nb6bushouref[iAttackID].form == 0 || nb6bushouref[iAttackID].form == 1) {
			iTeppouHanekaesiInTeppouDeath = 0;

			// ここには流れてこない
		}

	// それ以外の人が間に挟まった場合は、０に戻す
	} else {
		iTeppouHanekaesiInTeppouDeath = 0;
	}
}


/*
00432900  |. E8 10EDFFFF    CALL    TENSHOU.00431615                 ; \TENSHOU.00431615
00432905  |. 83C4 0C        ADD     ESP, 0C
00432908  |. 85C0           TEST    EAX, EAX
0043290A  |. 74 0C          JE      SHORT TENSHOU.00432918
0043290C  |. 56             PUSH    ESI                              ; /Arg2	← 攻撃した側の武将番号
0043290D  |. 57             PUSH    EDI                              ; |Arg1	← 攻撃された側の武将番号
0043290E  |. E8 A5EDFFFF    CALL    TENSHOU.004316B8                 ; \TENSHOU.004316B8
							└ここを JMP TSMod.OnTSExeeTeppouDeathInBattle と書き換えてやる実際の処理
00432913  |. 83C4 08        ADD     ESP, 8
*/
int pTenshouExeJumpFromToOnTenshouExeTeppouDeathInBattle    =0x43290E; // 関数はこのアドレスから、OnTenshouExeTeppouDeathInBattleへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeTeppouDeathInBattle=0x4316B8; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeTeppouDeathInBattle =0x432913; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeTeppouDeathInBattle() {
	// スタックにためておく
	__asm {
		mov iTeppouAttackBushouIDInTeppouDeath, esi		// 撃ったわがのＩＤをコピー
		mov iTeppouDefendBushouIDInTeppouDeath, edi		// 撃たれたがわＩＤをコピー

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
	OnTenshouExeTeppouDeathInBattleExecute();

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

		/*
		0043290C  |. 56             PUSH    ESI                              ; /Arg2	← 攻撃した側の武将番号
		0043290D  |. 57             PUSH    EDI                              ; |Arg1	← 攻撃された側の武将番号
		*/
		// 上の２つをまずは、無いこととする。
		pop edi
		pop esi

		// 値をすりかえた(かもしれないう)値で、元々の処理をここに
		push iTeppouAttackBushouIDInTeppouDeath
		push iTeppouDefendBushouIDInTeppouDeath

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeTeppouDeathInBattle;

		mov iTeppouHanekaesiInTeppouDeath, 0

		jmp pTenshouExeReturnLblFromOnTenshouExeTeppouDeathInBattle
	}
}

/*
00410467  |. 66:8BF0        MOV     SI, AX
0041046A  |. BF FFFF0000    MOV     EDI, 0FFFF
				└ここを JMP TSMod.OnTSExeTeppouDeathInBattle と書き換えてやる実際の処理
0041046F  |. E8 71CA0900    CALL    TENSHOU.004ACEE5
*/
char cmdOnTenshouExeJumpFromTeppouDeathInBattle[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeTeppouDeathInBattle() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeTeppouDeathInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeTeppouDeathInBattle + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeTeppouDeathInBattle  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromTeppouDeathInBattle+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeTeppouDeathInBattle), cmdOnTenshouExeJumpFromTeppouDeathInBattle, 5, NULL); //5バイトのみ書き込む
}




