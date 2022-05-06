#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


// 戦争において、移動力の残量に加算が加えられる時。
// (満タン==0F だったらこの処理まで来ないので注意。こない理由は複数回行動を実装するためにリターンしてしまっているからだ。元来はくるはずなのだ。)

/*
0042DC02  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0042DC06  |. E8 CEFEFFFF    CALL    TENSHOU.0042DAD9
0042DC0B  |. 8A4C24 0C      MOV     CL, BYTE PTR SS:[ESP+C]			// この瞬間のESP+C のメモリアドレスに、移動力が入っている。
0042DC0F  |. 83C4 04        ADD     ESP, 4
0042DC12  |. 8848 0C        MOV     BYTE PTR DS:[EAX+C], CL
0042DC15  \. C3             RETN
*/

// return 馬の上昇値(1～10)
int isCurBushouHasAUma(int iBushouID) {
	int iUmaMaxRank = 0;

	// 家宝を一通り調べる
	for (int iKahouID=0; iKahouID<GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {

		// 所有者の武将IDが一致する場合のみ判定の対象
		if (nb6kahou[iKahouID].attach-1 == iBushouID) {

			// 対象の家宝が「馬」である。
			if ( nb6kahou[iKahouID].type == 8 ) { // 改造しているかもしれないので対応する。

				// 他に持っていた刀剣よりもランクが高ければ、それが本人が所持するよりより家宝上昇値
				if ( iUmaMaxRank < nb6kahou[iKahouID].param ) {
					iUmaMaxRank = nb6kahou[iKahouID].param;
				}
			}
		}
	}
	// ランクを返す。最小0(所持していない)、最大10
	return iUmaMaxRank;
}










char iCurMoveRemainPower = 0;  // 
void OnTenshouExeMoveRemainPowerInBattleExecute() {
}

/*
0042DC02  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0042DC06  |. E8 CEFEFFFF    CALL    TENSHOU.0042DAD9
0042DC0B  |. 8A4C24 0C      MOV     CL, BYTE PTR SS:[ESP+C]			// この瞬間のESP+C のメモリアドレスに、移動力が入っている。CLにコピーされる。
0042DC0F  |. 83C4 04        ADD     ESP, 4
0042DC12  |. 8848 0C        MOV     BYTE PTR DS:[EAX+C], CL
0042DC15  \. C3             RETN
*/
int pTenshouExeJumpFromToOnTenshouExeMoveRemainPowerInBattle    =0x42DC06; // 関数はこのアドレスから、OnTenshouExeMoveRemainPowerInBattleへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeMoveRemainPowerInBattle=0x42DAD9; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeMoveRemainPowerInBattle =0x42DC15; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeMoveRemainPowerInBattle() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeMoveRemainPowerInBattle

		// 元々TENSHOU.EXE内にあったものをここで
		mov     cl, byte ptr ss:[esp+0xC]
		add     esp, 4

		// 今回計算結果による移動力残り量をコピー
		mov iCurMoveRemainPower, cl

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
	OnTenshouExeMoveRemainPowerInBattleExecute();

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

		// 元々TENSHOU.EXE内にあったものをここで
		mov     byte ptr ds:[eax+0xC], cl

		jmp pTenshouExeReturnLblFromOnTenshouExeMoveRemainPowerInBattle
	}
}

/*
00410467  |. 66:8BF0        MOV     SI, AX
0041046A  |. BF FFFF0000    MOV     EDI, 0FFFF
				└ここを JMP TSMod.OnTSExeMoveRemainPowerInBattle と書き換えてやる実際の処理
0041046F  |. E8 71CA0900    CALL    TENSHOU.004ACEE5
*/
char cmdOnTenshouExeJumpFromMoveRemainPowerInBattle[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeMoveRemainPowerInBattle() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeMoveRemainPowerInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeMoveRemainPowerInBattle + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeMoveRemainPowerInBattle  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromMoveRemainPowerInBattle+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeMoveRemainPowerInBattle), cmdOnTenshouExeJumpFromMoveRemainPowerInBattle, 5, NULL); //5バイトのみ書き込む
}




