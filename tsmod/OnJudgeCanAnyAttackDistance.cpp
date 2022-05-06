#include "WinTarget.h"

#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXBushou_All.h"

/*
 * 戦争時にユニットから攻撃しようとするときに、攻撃可能距離内かどうかを判定する。
 * この値を大きくすれば、非常に遠くの敵でも攻撃することが可能だ。
 */

/*
0042B428  |. 57             PUSH    EDI                              ; |Arg1
0042B429  |. E8 8A000000    CALL    TENSHOU.0042B4B8                 ; \TENSHOU.0042B4B8
0042B42E  |. 83C4 08        ADD     ESP, 8


0042B431  |. 85C0           TEST    EAX, EAX
0042B433  |. 74 14          JE      SHORT TENSHOU.0042B449
0042B435  |. 3B5D F0        CMP     EBX, DWORD PTR SS:[EBP-10] ←EDIの武将の鉄砲の適性をEBXにコピー
0042B438  |. 72 0F          JB      SHORT TENSHOU.0042B449
0042B43A  |. 837D EC 02     CMP     DWORD PTR SS:[EBP-14], 2 ←鉄砲隊かどうかの判定
0042B43E  |. 74 06          JE      SHORT TENSHOU.0042B446
0042B440  |. 837D EC 03     CMP     DWORD PTR SS:[EBP-14], 3 ←騎馬鉄砲隊かどうかの判定
0042B444  |. 75 03          JNZ     SHORT TENSHOU.0042B449  ←騎馬鉄砲たいでもないならば…
*/

/*
 * 
 */
//
int isBushouIDJudgeRemoteAttack = -1; // リモート攻撃可能かどうか判定対象となる武将番号
int iRemoteAptituteOfJudgeTorTK = 0;
int iRemoteJohekiTsukinuke = 0;

void OnTenshouExeCanAnyAttackDistanceExecute() {

	iRemoteJohekiTsukinuke = 0; // 普通城壁は突き抜けられない

	int iBushouID = (isBushouIDJudgeRemoteAttack & 0xFFFF) -1;

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 対象武将が(城に居れば)位置を得る。
		HEX_POSITION p = GetFieldHexUnitPos( iBushouID );
		// 武将の位置の役割を得る
		int iHexRole = GetFieldHexRole( p );

		// ★以下では距離を減らしているが、距離を増やすことも可能だ!!
		// 足軽で弓がＢ以上であれば…
		if ( nb6bushouref[iBushouID].form == 0 && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3) { // 足軽で弓がＢ以上であれば…
			
			if ( iHexRole == FIELD_HEX_ROLE::海 || iHexRole == FIELD_HEX_ROLE::湖 ) {
				iRemoteAptituteOfJudgeTorTK = 0; // 弓適性としては３枡までＯＫ。
			} else if (nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi == 5 ) {
				iRemoteAptituteOfJudgeTorTK = 3; // 弓適性としては３枡までＯＫ。
			} else {
				iRemoteAptituteOfJudgeTorTK = 2; // 弓適性としては２枡までＯＫ。
			}

		// 騎馬で弓がＢ以上であれば…
		// 騎馬適正がＢ以上で、弓適性もＢ以上ならば、リモート攻撃可能
		} else if ( nb6bushouref[iBushouID].form == 1 && nb6bushouref[iBushouID].aptitudekib >= 3 && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3) {

			if ( iHexRole == FIELD_HEX_ROLE::海 || iHexRole == FIELD_HEX_ROLE::湖 ) {
				iRemoteAptituteOfJudgeTorTK = 0; // 弓適性としては３枡までＯＫ。
			} else if (nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi == 5 ) {
				iRemoteAptituteOfJudgeTorTK = 3; // 弓適性としては３枡までＯＫ。
			} else {
				iRemoteAptituteOfJudgeTorTK = 2; // 弓適性としては２枡までＯＫ。
			}

		// 鉄砲が打てる
		}
	}

}


/*
0042B428  |. 57             PUSH    EDI                              ; |Arg1
0042B429  |. E8 8A000000    CALL    TENSHOU.0042B4B8                 ; \TENSHOU.0042B4B8
							└ここを JMP TSMod.OnTSExeCanAnyAttackDistance と書き換えてやる実際の処理
0042B42E  |. 83C4 08        ADD     ESP, 8
*/
int pTenshouExeJumpFromToOnTenshouExeCanAnyAttackDistance		=0x42B429; // 関数はこのアドレスから、OnTenshouExePreviousBattleMenuInBattleへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeCanAnyAttackDistance =0x42B4B8; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeCanAnyAttackDistance	=0x42B42E; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeCanAnyAttackDistance() {
	// スタックにためておく
	__asm {

		mov isBushouIDJudgeRemoteAttack, EDI // EDIに現在対象となる武将番号が入る。

		mov iRemoteAptituteOfJudgeTorTK, EBX // 鉄砲の値

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	OnTenshouExeCanAnyAttackDistanceExecute();

	// 城壁突き抜けＯＫならば…
	if ( iRemoteJohekiTsukinuke ) {

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

			call pTenshouExeJumpCallFromToOnTenshouExeCanAnyAttackDistance // ←★この行を実行しなければ、城壁の高い方へと撃つことが可能となる。

			mov EBX, iRemoteAptituteOfJudgeTorTK; // 弓であれば、鉄砲射程を短くすることで、対処する。

			jmp pTenshouExeReturnLblFromOnTenshouExeCanAnyAttackDistance;
		}

	// 城壁突き抜け出来ない普通の状態
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

			call pTenshouExeJumpCallFromToOnTenshouExeCanAnyAttackDistance // ←★この行を実行しなければ、城壁の高い方へと撃つことが可能となる。

			mov EBX, iRemoteAptituteOfJudgeTorTK; // 弓であれば、鉄砲射程を短くすることで、対処する。

			jmp pTenshouExeReturnLblFromOnTenshouExeCanAnyAttackDistance;
		}
	}
}





char cmdOnTenshouExeJumpFromCanAnyAttackDistance[6]="\xE9";
// 元の命令が５バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeCanAnyAttackDistance() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeCanAnyAttackDistance;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeCanAnyAttackDistance + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeCanAnyAttackDistance  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromCanAnyAttackDistance+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeCanAnyAttackDistance), cmdOnTenshouExeJumpFromCanAnyAttackDistance, 5, NULL); //5バイトのみ書き込む
}


