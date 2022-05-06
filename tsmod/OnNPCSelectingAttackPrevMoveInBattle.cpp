#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


#include "MakeIsseikougekiMembers.h"

// 戦闘中NPCが実際の行動をとる際のIDが決定された時。ユニットが動く前に行動するものの場合。
// 移動してかつ攻撃の場合はパススルーしてしまうので注意(ただし、この前後を分析すればそれも問題は無い)

/*
00489D77  |. E8 A3E6FBFF              CALL    TENSHOU.0044841F
00489D7C  |. 83C4 04                  ADD     ESP, 4
00489D7F  |. 8945 F8                  MOV     DWORD PTR SS:[EBP-8], EAX　　← さかのぼって、ここで攻撃の種類は決まる。(EBXが相手武将、[EBP-2]が自分の武将番号)
00489D82  |> 33C0                     XOR     EAX, EAX



00489DFD  |. FF75 FE        PUSH    DWORD PTR SS:[EBP-2]
00489E00  |. FF75 08        PUSH    DWORD PTR SS:[EBP+8]
00489E03  |. FF149D 401F4D00          CALL    DWORD PTR DS:[EBX*4+4D1F40]	← PCとそっくり。NPCの戦闘行動種類が入る。
00489E0A  |. 83C4 08        ADD     ESP, 8

00489D2E  |. 66:8BD8                  MOV     BX, AX



*/
extern void OnSelectingAttackEventHandlerInBattle(int iBushouID, int iTargetID, int iSelectActionID, BOOL isMoveBefore);

int iNPCSelectingAttackPrevMoveID = -1;		// 0:攻撃 1:騎馬 2:鉄砲 3:大砲 4:一斉攻撃
int iBushouIdOfNPCSelectingAttackPrevMove = -1;
int iTargetIdOfNPCSelectingAttackPrevMove = -1;

// 0:攻撃 1:騎馬 2:鉄砲 3:大砲 4:一斉攻撃
void OnTenshouExeNPCSelectingAttackPrevMoveInBattleExecute() {
	// OutputDebugStream("%d", iSelectingAttackMenuID );
	int iBushouID = (iBushouIdOfNPCSelectingAttackPrevMove & 0xFFFF)-1;
	int iTargetID = (iTargetIdOfNPCSelectingAttackPrevMove & 0xFFFF)-1;

	OnSelectingAttackEventHandlerInBattle(iBushouID, iTargetID, iNPCSelectingAttackPrevMoveID, TRUE);

	// 一斉攻撃だったら
	if ( iNPCSelectingAttackPrevMoveID == 4 ) {
		// 巻き添えメンバを構築
		MakeIsseikougekiMembers(iBushouID, iTargetID);

		/*
		for each(int i in vListIsseiKougekiMembers) {
			OutputDebugStream << nb6bushouname[i].fastname << endl;
		}
		OutputDebugStream << "------------" << endl;
		*/
	}
}

/*
00489D77  |. E8 A3E6FBFF              CALL    TENSHOU.0044841F
				└この2行を JMP TSMod.OnTSExeNPCSelectingAttackPrevMoveInBattle と書き換えてやる実際の処理
00489D7C  |. 83C4 04                  ADD     ESP, 4
00489D7F  |. 8945 F8                  MOV     DWORD PTR SS:[EBP-8], EAX　　← さかのぼって、ここで攻撃の種類は決まる。(EBXが相手武将、[EBP-2]が自分の武将番号)
00489D82  |> 33C0                     XOR     EAX, EAX
*/
int pTenshouExeJumpFromToOnTenshouExeNPCSelectingAttackPrevMoveInBattle	 =0x489D77; // 関数はこのアドレスから、OnTenshouExeNPCSelectingAttackPrevMoveInBattleへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeNPCSelectingAttackPrevMoveInBattle=0x44841F; 
int pTenshouExeReturnLblFromOnTenshouExeNPCSelectingAttackPrevMoveInBattle  =0x489D7C; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeNPCSelectingAttackPrevMoveInBattle() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeNPCSelectingAttackPrevMoveInBattle

		mov iNPCSelectingAttackPrevMoveID, eax  // 行動選択番号

		// eaxは保存したので、eaxを維持し変数に利用する。
		mov eax, DWORD PTR SS:[ebp+8]
		mov iBushouIdOfNPCSelectingAttackPrevMove, eax // 自分の武将を保存
		
		mov eax, DWORD PTR SS:[ebp-0x2]
		mov iTargetIdOfNPCSelectingAttackPrevMove, eax	// 相手の武将番号

		mov eax, iNPCSelectingAttackPrevMoveID  // ebxを元へと戻す

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 技能選択時に行う処理
	OnTenshouExeNPCSelectingAttackPrevMoveInBattleExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeNPCSelectingAttackPrevMoveInBattle
	}
}

char cmdOnTenshouExeJumpFromNPCSelectingAttackPrevMoveInBattle[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeNPCSelectingAttackPrevMoveInBattle() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeNPCSelectingAttackPrevMoveInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeNPCSelectingAttackPrevMoveInBattle + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeNPCSelectingAttackPrevMoveInBattle  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromNPCSelectingAttackPrevMoveInBattle+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeNPCSelectingAttackPrevMoveInBattle), cmdOnTenshouExeJumpFromNPCSelectingAttackPrevMoveInBattle, 5, NULL); //5バイトのみ書き込む
}
