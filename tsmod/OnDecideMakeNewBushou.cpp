#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"
#include "ScenarioMod.h"

// 新規武将作成で「決定」した時

/*
00497983  |. E8 4F3FFEFF    CALL    TENSHOU.0047B8D7  ← ここの返り値が0なら「取消」、1なら「実行」 EBXが武将番号か
00497988  |. 8D8D 08FFFFFF  LEA     ECX, DWORD PTR SS:[EBP-F8]
0049798E  |. 8945 F0        MOV     DWORD PTR SS:[EBP-10], EAX
00497991  |. C705 84154D00 >MOV     DWORD PTR DS:[4D1584], 0FFFF
*/

extern int aptitude_coef();

extern int iBudhouNodOfPathDDrawHimekaoApeearBug;

int iNoOfNewBushouExecute = -1;
int iIsDecideOfNewBushouExecute = 0; // 「実行」:1 「取消」:0

void OnTenshouExeDecideMakeNewBushouExecute() {

	if (iIsDecideOfNewBushouExecute) {

		int iBushouID = iNoOfNewBushouExecute - 1;

		if (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {
			// OutputDebugString(nb6bushouname[iBushouID].fastname);

			int yumi = aptitude_coef() - 1;
			int yari = aptitude_coef() - 1;

			// 弓はTSMod.txtの設定には関係なく設定する。
			// 弓の適性をコピー
			// 弓が無いというフラグがONなのであれば、0で上書きする。
			if (TSModCommand::World::cmdTheYumiNoExists) {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = 0;
			}
			else {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = yumi;
			}
			// 槍はTSMod.txtの設定には関係なく設定する。
			// 槍の適性をコピー
			nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari = yari;

			// 特殊属性は、一般武将は持ち合わせないので、全てクリア
			nb6savedata_ex.bushou_ex[iBushouID].aptitudeono = 0;
			nb6savedata_ex.bushou_ex[iBushouID].aptitudemuchi = 0;
			nb6savedata_ex.bushou_ex[iBushouID].aptitudetuchi = 0;
			nb6savedata_ex.bushou_ex[iBushouID].aptitudetue = 0;
			nb6savedata_ex.bushou_ex[iBushouID].aptitudeken = 0;

			// ScenarioModへの仮想武将誕生通知
			if (p_snOnBornGeneralBushou) {
				p_snOnBornGeneralBushou(iBushouID);
			}

		}
	}
}


/*
00497983  |. E8 4F3FFEFF    CALL    TENSHOU.0047B8D7  ← ここの返り値が0なら「取消」、1なら「実行」 EBXが武将番号
└ここを JMP TSMod.OnTSExeDecideMakeNewBushou と書き換えてやる実際の処理
00497988  |. 8D8D 08FFFFFF  LEA     ECX, DWORD PTR SS:[EBP-F8]
0049798E  |. 8945 F0        MOV     DWORD PTR SS:[EBP-10], EAX
00497991  |. C705 84154D00 >MOV     DWORD PTR DS:[4D1584], 0FFFF
*/

int pTenshouExeJumpFromToOnTenshouExeDecideMakeNewBushou = 0x497983; // 関数はこのアドレスから、OnTenshouExeDecideMakeNewBushouへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeDecideMakeNewBushou = 0x47B8D7; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeDecideMakeNewBushou = 0x497988; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec(naked) void WINAPI OnTenshouExeDecideMakeNewBushou() {

	// スタックにためておく
	__asm {

		mov iBudhouNodOfPathDDrawHimekaoApeearBug, EBX

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeDecideMakeNewBushou

		mov iBudhouNodOfPathDDrawHimekaoApeearBug, -1

		// EAXは「実行」なら1、「取消」なら0
		mov iIsDecideOfNewBushouExecute, EAX

		// 決してした武将番号を保存
		mov iNoOfNewBushouExecute, EBX

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
	OnTenshouExeDecideMakeNewBushouExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeDecideMakeNewBushou
	}
}

/*
0049D7AA  |. 6A 03          |PUSH    3                               ; |Arg3 = 00000003
0049D7AC  |. 56             |PUSH    ESI                             ; |Arg2
0049D7AD  |. 56             |PUSH    ESI                             ; |Arg1
0049D7AE  |. E8 D3540000    |CALL    TENSHOU.004A2C86                ; \TENSHOU.004A2C86
└ここを JMP TSMod.OnTSExeDecideMakeNewBushou と書き換えてやる実際の処理
0049D7B3  |. 83C4 1C        |ADD     ESP, 1C
0049D7B6  |. 85C0           |TEST    EAX, EAX
*/
char cmdOnTenshouExeJumpFromDecideMakeNewBushou[6] = "\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeDecideMakeNewBushou() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeDecideMakeNewBushou;
	int SubAddress = iAddress - (pTenshouExeJumpFromToOnTenshouExeDecideMakeNewBushou + 5);
	// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeDecideMakeNewBushou  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromDecideMakeNewBushou + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory(CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeDecideMakeNewBushou), cmdOnTenshouExeJumpFromDecideMakeNewBushou, 5, NULL); //5バイトのみ書き込む
}




