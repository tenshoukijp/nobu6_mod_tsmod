#include "WinTarget.h"

#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "SetNeedKoudouryoku.h"

// 
// 内政など５人選択するための入力ダイアログが出た時の「枠の数」を決める


/*
0042F1AC  /$ 53             PUSH    EBX
0042F1AD  |. 56             PUSH    ESI
0042F1AE  |. 57             PUSH    EDI
0042F1AF  |. 8BD9           MOV     EBX, ECX
0042F1B1  |. 33F6           XOR     ESI, ESI
0042F1B3  |. 8DBB AC000000  LEA     EDI, DWORD PTR DS:[EBX+AC]		← このEBX+ACの値が、ダイアログから選択可能な人数となる。
0042F1B9  |. 3937           CMP     DWORD PTR DS:[EDI], ESI
0042F1BB  |. 76 0D          JBE     SHORT TENSHOU.0042F1CA
0042F1BD  |> 56             /PUSH    ESI
0042F1BE  |. 8BCB           |MOV     ECX, EBX
0042F1C0  |. 46             |INC     ESI
0042F1C1  |. E8 95FFFFFF    |CALL    TENSHOU.0042F15B
0042F1C6  |. 3937           |CMP     DWORD PTR DS:[EDI], ESI
0042F1C8  |.^77 F3          \JA      SHORT TENSHOU.0042F1BD
0042F1CA  |> 5F             POP     EDI
0042F1CB  |. 5E             POP     ESI
0042F1CC  |. 5B             POP     EBX
0042F1CD  \. C3             RETN

*/
#include "OnEnterMainPlayerCommand.h"

extern int iEnterOfClickMainScreenMainCommand; // プレイヤの際に実行しているコマンド


BOOL isMustOverrideOfViewingNaiseiExecutantsEtc5WakuNum = FALSE;
int iWakuCntOfViewingNaiseiExecutantsEtc5WakuNum = 5;
void OnTenshouExeViewingNaiseiExecutantsEtc5WakuNumExecute() {
	isMustOverrideOfViewingNaiseiExecutantsEtc5WakuNum = FALSE;
	iWakuCntOfViewingNaiseiExecutantsEtc5WakuNum = 5;

	// ５人組で消費する行動値がデフォルトではない
	if (TSModCommand::AllGundan::cmdNeedKoudouValue5Ningumi != CMD_NEED_KOUDOUVALUE_5NINGUMI_DEFAULT ) {
		// 今回は書き換える必要がある
		isMustOverrideOfViewingNaiseiExecutantsEtc5WakuNum = TRUE;

		// 現在のターンの城番号
		int iCastleID = nb6turn.castle - 1;
		if ( 0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM) {
			int iValidSelectableBushouCnt = 0;

			// その城の所属軍団
			int iGundanID = nb6castle[iCastleID].attach - 1;
			if ( 0 <= iGundanID &&  iGundanID < GAMEDATASTRUCT_GUNDAN_NUM) {
				int iKoudouPowerRest = nb6gundan[iGundanID].act;

				// 選択可能なカウント数 
				int iCanSelectCnt = 0;
				int iCurLostPower = TSModCommand::AllGundan::cmdNeedKoudouValue5Ningumi; // 最初は消費値そのものからのスタート

				// その城に所属する武将
				for (int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {
					// 行動値の残りが、消費値より少ないならもうダメ
					if ( iKoudouPowerRest < iCurLostPower ) {
						break;
					}

					// ５人がＭＡＸ(UI的にも)
					if ( iCanSelectCnt >= 5 ) {
						break;
					}
/*					// 0消費はダメ
					if ( iCurLostPower == 0 ) {
						break;
					}
*/
					// その城に居ない。次の人
					if (nb6bushouref[iBushouID].castle-1 != iCastleID) {
						continue;
					}


					// 大名か軍団長か現役のいずれかである
					if (nb6bushouname[iBushouID].State == 0 || nb6bushouname[iBushouID].State == 1 || nb6bushouname[iBushouID].State == 2) {

						// 死亡とかなっていない
						if(nb6bushouname[iBushouID].battleDeath == 0) {
							// (計算上の)残り行動値を引く
							iKoudouPowerRest -= iCurLostPower;

							// 次回消費する行動値を引く
							iCurLostPower--;
							if (iCurLostPower < 0 ) {
								iCurLostPower = 0;
							}

							// １人選択候補
							iCanSelectCnt++;
						}
					}
				}

				iWakuCntOfViewingNaiseiExecutantsEtc5WakuNum = iCanSelectCnt;


				// 枠の操作をすると、選択すると、以前の残骸が残ることがある。
				// よって枠の掃除
				WORD *pWakuClean = (WORD *)GAMEDATASTRUCT_VARIABLE_VALUE_GROUP2_DIALOG;
				BOOL isMustBeAfterClean = false;
				for (int i=0; i<5; i++) {
					if (isMustBeAfterClean) {
						pWakuClean[i] = 0xFFFF;
					}
					if (pWakuClean[i]==0xFFFF) {
						isMustBeAfterClean = true;
					}
				}
				
			}
		}
	}
}

static int iTmpEAXOfViewingNaiseiExecutantsEtc5WakuNum = 0; // EAXの値の一次保存用

/*
0042F1B1  |. 33F6           XOR     ESI, ESI
0042F1B3  |. 8DBB AC000000  LEA     EDI, DWORD PTR DS:[EBX+AC]		← このEBX+ACの値が、ダイアログから選択可能な人数となる。
0042F1B9  |. 3937           CMP     DWORD PTR DS:[EDI], ESI
*/
int pTenshouExeJumpFromToOnTenshouExeViewingNaiseiExecutantsEtc5WakuNum    =0x42F1B3; // 関数はこのアドレスから、OnTenshouExeViewingNaiseiExecutantsEtc5WakuNumへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeViewingNaiseiExecutantsEtc5WakuNum =0x42F1B9; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeViewingNaiseiExecutantsEtc5WakuNum() {
	// スタックにためておく
	__asm {

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
	OnTenshouExeViewingNaiseiExecutantsEtc5WakuNumExecute();

	// 書き換えなければならない
	if (isMustOverrideOfViewingNaiseiExecutantsEtc5WakuNum) {
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

			mov iTmpEAXOfViewingNaiseiExecutantsEtc5WakuNum, eax // eaxをいったん保存

			mov eax, iWakuCntOfViewingNaiseiExecutantsEtc5WakuNum // 枠の数をEAXに
			mov DWORD PTR DS:[EBX+0xAC], eax					// メモリ上に上書き
			LEA     EDI, DWORD PTR DS:[EBX+0xAC]	// 天翔記にあった元々の処理

			mov eax, iTmpEAXOfViewingNaiseiExecutantsEtc5WakuNum // eaxを戻す

			jmp pTenshouExeReturnLblFromOnTenshouExeViewingNaiseiExecutantsEtc5WakuNum
		}
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

			LEA     EDI, DWORD PTR DS:[EBX+0xAC]	// 天翔記にあった元々の処理

			jmp pTenshouExeReturnLblFromOnTenshouExeViewingNaiseiExecutantsEtc5WakuNum
		}
	}
}


char cmdOnTenshouExeJumpFromViewingNaiseiExecutantsEtc5WakuNum[7]="\xE9\x90\x90\x90\x90\x90";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeViewingNaiseiExecutantsEtc5WakuNum() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeViewingNaiseiExecutantsEtc5WakuNum;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingNaiseiExecutantsEtc5WakuNum + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingNaiseiExecutantsEtc5WakuNum  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromViewingNaiseiExecutantsEtc5WakuNum+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingNaiseiExecutantsEtc5WakuNum), cmdOnTenshouExeJumpFromViewingNaiseiExecutantsEtc5WakuNum, 6, NULL); //6バイトのみ書き込む
}




