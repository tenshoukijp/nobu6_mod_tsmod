#include "WinTarget.h"

#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXBushou_All.h"
#include "ScenarioMod.h"

/*
 * 戦争時にユニットから攻撃しようとするときに、「鉄砲隊」か、もしくは「鉄砲騎馬隊」であるかを判定する。
 * どちらかであればリモート攻撃が可能だからだ。
 */


/*
 * 
 */
//
extern int isBushouIDJudgeRemoteAttack;
int isJudgeCanRemoteAttackValue = -1;   // 結果としてリモート攻撃かのうかどうか。これでジャンプ先を振り分ける

int iTargetHeikaOfJudgeTeppouOrTeppouTaiExecute = -1;   // 結果としてリモート攻撃かのうかどうか。これでジャンプ先を振り分ける

void OnTenshouExeJudgeTeppouOrTeppouTaiExecute() {

	isJudgeCanRemoteAttackValue = iTargetHeikaOfJudgeTeppouOrTeppouTaiExecute==2 ? 1 : 0;		  // リモートアタックは最初の値のまま。

	int iBushouID = (isBushouIDJudgeRemoteAttack & 0xFFFF) -1;

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {
		// 鉄砲隊に限り、ScenarioModで、鉄砲攻撃不可能という上書きが可能
		if ( nb6bushouref[iBushouID].form == 2 ) {

			isJudgeCanRemoteAttackValue = 1; // リモート攻撃可能。

			// ScenarioModで天候やそのほかに関わらず、鉄砲を撃てるか否かの条件を変更してしまう。
			if ( p_snOnCustomCondition ) {
				// メイン画面での内応の成否																			┌実行者    
				int isCanTeppouResult = ((PFNSNONCUSTOMCONDITION) p_snOnCustomCondition)("OR条件::戦争::鉄砲不可", iBushouID+1, 0,   0, "", "", 0.0, 0.0);
				if ( isCanTeppouResult == TRUE ) {
					isJudgeCanRemoteAttackValue = 0; // 鉄砲を不可とする
				}
			}
		}
	}
}


/*
0042B431  |. 85C0           TEST    EAX, EAX
0042B433  |. 74 14          JE      SHORT TENSHOU.0042B449
0042B435  |. 3B5D F0        CMP     EBX, DWORD PTR SS:[EBP-10] ←EDIの武将の鉄砲の適性をEBXにコピー
0042B438  |. 72 0F          JB      SHORT TENSHOU.0042B449
0042B43A  |. 837D EC 02     CMP     DWORD PTR SS:[EBP-14], 2 ←鉄砲隊かどうかの判定
							└ここを JMP TSMod.OnTSExeJudgeTeppouOrTeppouTai と書き換えてやる実際の処理
0042B43E  |. 74 06          JE      SHORT TENSHOU.0042B446
0042B440  |. 837D EC 03     CMP     DWORD PTR SS:[EBP-14], 3 ←騎馬鉄砲隊かどうかの判定
0042B444  |. 75 03          JNZ     SHORT TENSHOU.0042B449  ←騎馬鉄砲隊でもないならば…
0042B446  |> FE45 FA        INC     BYTE PTR SS:[EBP-6]
*/
int pTenshouExeJumpFromToOnTenshouExeJudgeTeppouOrTeppouTai	=0x42B43A; // 関数はこのアドレスから、OnTenshouExeJudgeTeppouOrTeppouTaiへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeJudgeTeppouOrTeppouTai	= 0x42B440; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w

int pTenshouExeReturnRemoteJmpFromOnTenshouExeJudgeTeppouOrTeppouTai	=0x42B446; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。

__declspec( naked ) void WINAPI OnTenshouExeJudgeTeppouOrTeppouTai() {
	// スタックにためておく
	__asm {

		mov isBushouIDJudgeRemoteAttack, EDI // EDIに現在対象となる武将番号が入る。下位ビットに絞っておくこと

		// ついでにEDIレジスタを間借りして、兵科値を保存しておく
		mov EDI, DWORD PTR SS:[EBP-0x14]

		mov iTargetHeikaOfJudgeTeppouOrTeppouTaiExecute, EDI

		// EDIを元へと戻しておく
		mov EDI, isBushouIDJudgeRemoteAttack

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	OnTenshouExeJudgeTeppouOrTeppouTaiExecute();

	// リモート攻撃が可能なのであれば…
	if ( isJudgeCanRemoteAttackValue ) {
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

			// 元の判定をここに入れる
			CMP     DWORD PTR SS:[EBP-0x14], 2 //  ←鉄砲隊かどうかの判定

			jmp pTenshouExeReturnRemoteJmpFromOnTenshouExeJudgeTeppouOrTeppouTai;
		}

	// リモート攻撃が不可能な通常状態なのであれば…
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

			// 元の判定をここに入れる
			CMP     DWORD PTR SS:[EBP-0x14], 2 //  ←鉄砲隊かどうかの判定

			jmp pTenshouExeReturnLblFromOnTenshouExeJudgeTeppouOrTeppouTai // リモート攻撃が不可能ならば処理を飛ばす。｢JNZ SHORT TENSHOU.0042B449｣  相当
		}
	}
}





char cmdOnTenshouExeJumpFromJudgeTeppouOrTeppouTai[7]="\xE9\x90\x90\x90\x90\x90";
// 元の命令が6バイト、以後の関数で生まれる命令が合計６バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeJudgeTeppouOrTeppouTai() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeJudgeTeppouOrTeppouTai;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeJudgeTeppouOrTeppouTai + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeJudgeTeppouOrTeppouTai  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromJudgeTeppouOrTeppouTai+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeJudgeTeppouOrTeppouTai), cmdOnTenshouExeJumpFromJudgeTeppouOrTeppouTai, 6, NULL); //6バイトのみ書き込む
}







void OnTenshouExeJudgeTeppouOrTeppouKibaExecute() {

	isJudgeCanRemoteAttackValue = iTargetHeikaOfJudgeTeppouOrTeppouTaiExecute==3 ? 1 : 0;		  // リモートアタックは最初の値のまま。

	int iBushouID = (isBushouIDJudgeRemoteAttack & 0xFFFF)-1;
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		//兵態(0=足軽,1=騎馬,2=鉄砲,3=騎馬鉄砲,18まで)

		if ( nb6bushouref[iBushouID].form == 3 ) {
			isJudgeCanRemoteAttackValue = 1; // リモート攻撃可能。

		// 足軽で弓がＢ以上であれば…
		} else if ( nb6bushouref[iBushouID].form == 0 && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3) { // 足軽で弓がＢ以上であれば…
			isJudgeCanRemoteAttackValue = 1;


		// 騎馬で弓がＢ以上であれば…
		// 騎馬適正がＢ以上で、弓適性もＢ以上ならば、リモート攻撃可能
		} else if ( nb6bushouref[iBushouID].form == 1 && nb6bushouref[iBushouID].aptitudekib >= 3 && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3) {
			isJudgeCanRemoteAttackValue = 1;
		}

		// 騎馬鉄砲隊に限り、ScenarioModで、鉄砲攻撃不可能という上書きが可能
		if ( nb6bushouref[iBushouID].form == 3 ) {

			// ScenarioModで天候やそのほかに関わらず、鉄砲を撃てるか否かの条件を変更してしまう。
			if ( p_snOnCustomCondition ) {
				// メイン画面での内応の成否																			┌実行者    
				int isCanTeppouResult = ((PFNSNONCUSTOMCONDITION) p_snOnCustomCondition)("OR条件::戦争::鉄砲不可", iBushouID+1, 0,   0, "", "", 0.0, 0.0);
				if ( isCanTeppouResult == TRUE ) {
					isJudgeCanRemoteAttackValue = 0; // 鉄砲を不可とする
				}
			}
		}
	}
}






/*
0042B431  |. 85C0           TEST    EAX, EAX
0042B433  |. 74 14          JE      SHORT TENSHOU.0042B449
0042B435  |. 3B5D F0        CMP     EBX, DWORD PTR SS:[EBP-10] ←EDIの武将の鉄砲の適性をEBXにコピー
0042B438  |. 72 0F          JB      SHORT TENSHOU.0042B449
0042B43A  |. 837D EC 02     CMP     DWORD PTR SS:[EBP-14], 2 ←鉄砲隊かどうかの判定
0042B43E  |. 74 06          JE      SHORT TENSHOU.0042B446
0042B440  |. 837D EC 03     CMP     DWORD PTR SS:[EBP-14], 3 ←騎馬鉄砲隊かどうかの判定
							└ここを JMP TSMod.OnTSExeJudgeTeppouOrTeppouKiba と書き換えてやる実際の処理
0042B444  |. 75 03          JNZ     SHORT TENSHOU.0042B449  ←騎馬鉄砲たいでもないならば…
0042B446  |> FE45 FA        INC     BYTE PTR SS:[EBP-6]
*/
int pTenshouExeJumpFromToOnTenshouExeJudgeTeppouOrTeppouKiba	=0x42B440; // 関数はこのアドレスから、OnTenshouExeJudgeTeppouOrTeppouKibaへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeJudgeTeppouOrTeppouKiba	=0x42B446; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w

int pTenshouExeReturnRemoteJmpFromOnTenshouExeJudgeTeppouOrTeppouKiba	=0x42B449; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeJudgeTeppouOrTeppouKiba() {
	// スタックにためておく
	__asm {

		mov isBushouIDJudgeRemoteAttack, EDI // EDIに現在対象となる武将番号が入る。下位ビットに絞っておくこと

		// ついでにEDIレジスタを間借りして、兵科値を保存しておく
		mov EDI, DWORD PTR SS:[EBP-0x14]

		mov iTargetHeikaOfJudgeTeppouOrTeppouTaiExecute, EDI

		// EDIを元へと戻しておく
		mov EDI, isBushouIDJudgeRemoteAttack

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	OnTenshouExeJudgeTeppouOrTeppouKibaExecute();

	// リモート攻撃が可能なのであれば…
	if ( isJudgeCanRemoteAttackValue ) {
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

			// 元の判定をここに入れる
			CMP     DWORD PTR SS:[EBP-0x14], 3 //  ←騎馬鉄砲隊かどうかの判定

			jmp pTenshouExeReturnLblFromOnTenshouExeJudgeTeppouOrTeppouKiba;
		}

	// リモート攻撃が不可能な通常状態なのであれば…
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

			// 元の判定をここに入れる
			CMP     DWORD PTR SS:[EBP-0x14], 3 //  ←騎馬鉄砲隊かどうかの判定

			jmp pTenshouExeReturnRemoteJmpFromOnTenshouExeJudgeTeppouOrTeppouKiba // リモート攻撃が不可能ならば処理を飛ばす。｢JNZ SHORT TENSHOU.0042B449｣  相当
		}
	}
}





char cmdOnTenshouExeJumpFromJudgeTeppouOrTeppouKiba[7]="\xE9\x90\x90\x90\x90\x90";
// 元の命令が6バイト、以後の関数で生まれる命令が合計６バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeJudgeTeppouOrTeppouKiba() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeJudgeTeppouOrTeppouKiba;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeJudgeTeppouOrTeppouKiba + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeJudgeTeppouOrTeppouKiba  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromJudgeTeppouOrTeppouKiba+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeJudgeTeppouOrTeppouKiba), cmdOnTenshouExeJumpFromJudgeTeppouOrTeppouKiba, 6, NULL); //6バイトのみ書き込む
}


