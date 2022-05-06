#include "WinTarget.h"
#include <time.h>

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "FuncPlayResourceWav.h"
#include "ScenarioMod.h"



// 00431FB8  |. 85C0           TEST    EAX, EAX	← 残り兵数の判定。０だったら捕縛関数へ飛ぶ


// 00432022  |> E8 71F5FFFF    CALL    TENSHOU.00431598		← 捕縛関数


/*
 * ダメージを食らった際の被ダメージを控えめにする処理
 */
// 攻撃をくらってる側の武将IDをコピー
int iDamagedOfAttackBushouID = -1;

// 攻撃をくらっている武将の残り兵数
int iRemainOfNumOfSoldier = -1;


// 現在の戦場におけるターン経過数。戦場以外では-1となっている。
extern int iCurIncrementTurnInBattle;

void OnTenshouExeDamagedOfAttackExecute() {

	// 戦争中でなければ何もしない
	if ( !CommonGlobalHandle::isBattleMode ) { return; }

	// 以下、戦争中という条件の下、
	// 武将IDチェック
	if ( 0 <= iDamagedOfAttackBushouID-1 && iDamagedOfAttackBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM ) {

		int iDefBushouID = iDamagedOfAttackBushouID-1;

		// 兵数を取り出す。
		byte iCurrSoldierNum = nb6bushouref[iDefBushouID].soldier;
		byte iPrevSoldierNum = nb6bushoucopy[iDefBushouID].soldier;

		// ダメージ差分
		int iDamagedValueu = iPrevSoldierNum - iCurrSoldierNum;

		if ( isLancelotTartare( iDefBushouID ) ) {


			if ( iCurrSoldierNum >= 1 && iPrevSoldierNum >= 2 ) {

				// もし兵数が４以上減っていたら、減りを３に抑える
				if ( iPrevSoldierNum - iCurrSoldierNum >= 4 ) {

					// 実際に戦場フィールドにいる時のみ鉄壁音を出す ( 50% )。(メインマップ上では鉄壁処理は行うが、音は出さない）
					if ( iCurIncrementTurnInBattle >= 0 && rand() % 2 == 0 ) {
						// 鉄壁音
						FuncPlaySoundWav(717);
					}

					int iModifySoldierNum = iPrevSoldierNum - 3;
					iDamagedValueu = 3; // 他へと伝達するダメージ値を変更する

					if (iModifySoldierNum < 0 ) { iModifySoldierNum = 0; }
					nb6bushouref[iDefBushouID].soldier = iModifySoldierNum;
					nb6bushoucopy[iDefBushouID].soldier = nb6bushouref[iDefBushouID].soldier;

				}
			}
		}

		else if (isShuiHidechiyo(iDefBushouID)) {


			if (iCurrSoldierNum >= 1 && iPrevSoldierNum >= 2) {

				// もし兵数が４以上減っていたら、減りを３に抑える
				if (iPrevSoldierNum - iCurrSoldierNum >= 4) {

					// 実際に戦場フィールドにいる時のみシュシュシュ音を出す ( 50% )。(メインマップ上では鉄壁処理は行うが、音は出さない）
					if (iCurIncrementTurnInBattle >= 0 && rand() % 2 == 0) {
						// シュシュシュ音
						FuncPlaySoundWav(617); // ★ここの音の部分が違う！
					}

					int iModifySoldierNum = iPrevSoldierNum - 3;
					iDamagedValueu = 3; // 他へと伝達するダメージ値を変更する

					if (iModifySoldierNum < 0) { iModifySoldierNum = 0; }
					nb6bushouref[iDefBushouID].soldier = iModifySoldierNum;
					nb6bushoucopy[iDefBushouID].soldier = nb6bushouref[iDefBushouID].soldier;

				}
			}
		}



		if ( p_snOnBattleDamaged ) {
			p_snOnBattleDamaged( iDefBushouID+1, iDamagedValueu , NULL );
		}

		nb6bushoucopy[iDefBushouID].soldier = nb6bushouref[iDefBushouID].soldier;

		// 変更された残り兵数をこちらにもコピー
		iRemainOfNumOfSoldier = nb6bushouref[iDefBushouID].soldier;
	}
}


/*
ここで1回当たりの被攻撃で減ってゆく兵数(GAMEDATA_STRUCTの先)をある程度、ヘリを抑えれば鉄壁は完了する。
00409C37  |. E8 6E320A00    CALL    TENSHOU.004ACEAA
00409C3C  |. 83C4 08        ADD     ESP, 8
00409C3F  |. 8846 1D        MOV     BYTE PTR DS:[ESI+1D], AL
00409C42  |. 0FB6F0         MOVZX   ESI, AL
				└この２行 JMP TSMod.OnTSExeDamagedOfAttack と書き換えてやる実際の処理
00409C45  |. 85F6           TEST    ESI, ESI					ESIは突撃をされるたびに減ってゆく。ESIを操作しても意味はない。
00409C47  |. 75 21          JNZ     SHORT TENSHOU.00409C6A

*/
int pTenshouExeJumpFromToOnTenshouExeDamagedOfAttack	   =0x409C3F; // 関数はこのアドレスから、OnTenshouExeDamagedOfAttackへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeDamagedOfAttack    =0x409C45; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w

// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeDamagedOfAttack() {
	// スタックにためておく
	__asm {
		// 元々の処理
		mov byte ptr ds:[esi+0x1D], al

		// 元々の処理
		movzx   esi, al

		// 攻撃をくらってる側の武将IDをコピー
		mov iDamagedOfAttackBushouID, edi

		// それをコピーしておく
		mov iRemainOfNumOfSoldier, esi

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
	OnTenshouExeDamagedOfAttackExecute();

	__asm {

		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		// 一応その数値はesiに上書きしておく。多分兵数0か非ゼロだけなので不要な気がするが…
		mov esi, iRemainOfNumOfSoldier

		jmp pTenshouExeReturnLblFromOnTenshouExeDamagedOfAttack
	}
}

/*
0040A185     E8 44320A00    CALL    TENSHOU.004AD3CE
				└ここを JMP TSMod.OnTSExeDamagedOfAttack と書き換えてやる実際の処理
0040A18A  |. 0FB7C0         MOVZX   EAX, AX
*/
char cmdOnTenshouExeJumpFromDamagedOfAttack[7]="\xE9\x90\x90\x90\x90\x90";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeDamagedOfAttack() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeDamagedOfAttack;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeDamagedOfAttack + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeDamagedOfAttack  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromDamagedOfAttack+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeDamagedOfAttack), cmdOnTenshouExeJumpFromDamagedOfAttack, 6, NULL); //6バイトのみ書き込む
}




