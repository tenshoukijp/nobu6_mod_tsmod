#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"


// 退却もしくは捕縛付近を変更する

static int iDefenceBushouIDOnHeisuZero = -1; // 兵数が０になった退却することになった武将のiBushouID
static int iAttackBushouIDOnHeisuZero = -1; // 上の時に攻撃した人のiBushouID
static int iIsOverrideDeathFuncOnHeisuZero = false;


namespace  攻撃方法 {
	enum { 攻撃 = 0, 突撃 = 1, 鉄砲 = 2, 大砲 = 3, 一斉攻撃 = 4, 弓 = 7 };
}
void OnTenshouExeDefenceBushouHeisuZeroExecute(int i攻撃方法番号) {
	iIsOverrideDeathFuncOnHeisuZero = false;

	int iAttackID = iAttackBushouIDOnHeisuZero & 0xFFFF - 1;
	int iDefendID = iDefenceBushouIDOnHeisuZero & 0xFFFF - 1;
	if (0 <= iAttackID && iAttackID < GAMEDATASTRUCT_BUSHOU_NUM &&
		0 <= iDefendID && iDefendID < GAMEDATASTRUCT_BUSHOU_NUM) {
		// すでに退却か死亡
		if (nb6bushouname[iDefendID].battleRetreat || nb6bushouname[iDefendID].battleDeath) {
			return;
		}

		if (nb6settingsex.deathfreq == TRUE) {
			// OutputDebugStream << "攻撃" << nb6bushouname[iAttackID].familyname << endl;
			// OutputDebugStream << "防御" << nb6bushouname[iDefendID].familyname << endl;

			// 攻撃したのはランスロットタルタロスである。
			if (isLancelotTartare(iAttackID)) {

				// コントロールが効いた攻撃。相手は死なない
				iIsOverrideDeathFuncOnHeisuZero = false;


			// 攻撃したのは朱伊である。
			} else if (isShuiHidechiyo(iAttackID)) {

				// コントロールが効いた攻撃。相手は死なない
				iIsOverrideDeathFuncOnHeisuZero = false;

			}

			// 攻撃側のジョブが上級、なら、攻撃を受けた方の死亡率が少し高い
			else if (nb6bushouref[iAttackID].job >= 0xA0) {
				// 相手側のジョブが上級未満
				if (nb6bushouref[iDefendID].job < 0xA0) {
					// 75～-75ぐらい
					int coef = getBushouBat(iAttackID) - getBushouBat(iDefendID);
					coef = coef / 20; // 3～-3ぐらい
					coef = 5 - coef;  // 2～8ぐらい
					if (coef && rand() % coef == 0) {
						iIsOverrideDeathFuncOnHeisuZero = true;
					}
				}
			}
			// 他、一般なら、上級の半分の確率
			else {
				// 相手側のジョブが上級以外
				if (nb6bushouref[iDefendID].job < 0xA0) {
					// 75～-75ぐらい
					int coef = getBushouBat(iAttackID) - getBushouBat(iDefendID);
					coef = coef / 20; // 3～-3ぐらい
					coef = 5 - coef;  // 2～8ぐらい
					// OutputDebugStream << coef*2 << endl;
					if (coef && rand() % coef == 0) {
						if (rand() % 2 == 0) { // 1/4～1/16ぐらい
							iIsOverrideDeathFuncOnHeisuZero = true;
						}
					}
				}
			}

			// 防御側がランスロットタルタロスなら死亡がない。
			if (isLancelotTartare(iDefendID)) {
				iIsOverrideDeathFuncOnHeisuZero = false;
			}

			// 防御側が朱伊なら死亡がない。
			if (isShuiHidechiyo(iDefendID)) {
				iIsOverrideDeathFuncOnHeisuZero = false;
			}

		}

		// 関数がDLLで定義されていたら、CONDITIONとして呼び出し、その結果がTRUEであれば、死亡させ、FALSEであれば、死亡しない。
		// ScenarioModで特に何もしないということを可能とするため、
		if (p_snOnCustomCondition) {																						      // ┌攻撃した側  ┌攻撃された側    ┌ 予定されてる死亡結果
			int isScenarioModDeathResult = ((PFNSNONCUSTOMCONDITION)p_snOnCustomCondition)("AS条件::戦争::非戦死攻撃受動者兵士消失時死亡可否", iAttackID + 1, iDefendID, iIsOverrideDeathFuncOnHeisuZero, "", "", (float)i攻撃方法番号, 0.0);
			// 非修正要綱でなければ…
			if (isScenarioModDeathResult != CUSTOM_CONDITION_NOMODIFY) {
				iIsOverrideDeathFuncOnHeisuZero = isScenarioModDeathResult;
			}
		}
	}
}

void OnTenshouExeDefenceBushouHeisuZeroInTujouExecute() {
	OnTenshouExeDefenceBushouHeisuZeroExecute(攻撃方法::攻撃);
}

/*
// 通常攻撃の結果、「攻撃を受けた武将が兵数０となり、捕縛もしくは退却」発生となった時
00432020  |> 56             PUSH    ESI
00432021  |. 57             PUSH    EDI
00432022  |> E8 71F5FFFF    CALL    TENSHOU.00431598 　// 退却もしくは捕縛が発生する関数
00432027  |. 83C4 08        ADD     ESP, 8
*/
int pTenshouExeJumpFromToOnTenshouExeDefenceBushouHeisuZeroTujou = 0x432022;
int pTenshouExeJumpCallTaikyakuFuncFromToOnTenshouExeDefenceBushouHeisuZeroTujou = 0x431598; // 元々の退却用の関数
int pTenshouExeJumpCallDeathFuncFromToOnTenshouExeDefenceBushouHeisuZeroTujou = 0x4316B8; // 同じタイミングでこちらを呼ぶと死亡する
int pTenshouExeReturnLblFromOnTenshouExeDefenceBushouHeisuZeroTujou = 0x432027;
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec(naked) void WINAPI OnTenshouExeDefenceBushouHeisuZeroTujou() {
	__asm {
		mov iDefenceBushouIDOnHeisuZero, EDI			// 守備武将
		mov iAttackBushouIDOnHeisuZero, ESI			// 攻撃武将
		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnTenshouExeDefenceBushouHeisuZeroInTujouExecute();

	// 得に指定がないままであった。
	if (iIsOverrideDeathFuncOnHeisuZero == false) {
		__asm {

			// スタック復活
			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			// 元の関数を呼ぶ
			call pTenshouExeJumpCallTaikyakuFuncFromToOnTenshouExeDefenceBushouHeisuZeroTujou

			jmp pTenshouExeReturnLblFromOnTenshouExeDefenceBushouHeisuZeroTujou
		}
	}
	else {
		__asm {
			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			// 死亡の関数を呼ぶ
			call pTenshouExeJumpCallDeathFuncFromToOnTenshouExeDefenceBushouHeisuZeroTujou

			jmp pTenshouExeReturnLblFromOnTenshouExeDefenceBushouHeisuZeroTujou
		}
	}

}
char cmdOnTenshouExeJumpFromDefenceBushouHeisuZeroTujou[6] = "\xE9";
void WriteAsmJumperOnTenshouExeDefenceBushouHeisuZeroTujou() {
	int iAddress = (int)OnTenshouExeDefenceBushouHeisuZeroTujou;
	int SubAddress = iAddress - (pTenshouExeJumpFromToOnTenshouExeDefenceBushouHeisuZeroTujou + 5);
	memcpy(cmdOnTenshouExeJumpFromDefenceBushouHeisuZeroTujou + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。 
	WriteProcessMemory(CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeDefenceBushouHeisuZeroTujou), cmdOnTenshouExeJumpFromDefenceBushouHeisuZeroTujou, 5, NULL); // 一応安全のため 
}

















/*
// 一斉攻撃の結果、「攻撃を受けた武将が兵数０となり、捕縛もしくは退却」発生となった時
004321D0 | .FF75 08        PUSH    DWORD PTR SS : [EBP + 8]
004321D3 | .FF75 0C        PUSH    DWORD PTR SS : [EBP + C]
004321D6 | .E8 BDF3FFFF    CALL    TENSHOU.00431598
004321DB | . 83C4 08        ADD     ESP, 8
*/
void OnTenshouExeDefenceBushouHeisuZeroInIsseiExecute() {
	OnTenshouExeDefenceBushouHeisuZeroExecute(攻撃方法::一斉攻撃);
}

int EAXOfDefenceBushouHeisuZeroInIssei = 0; // 一時保存用

int pTenshouExeJumpFromToOnTenshouExeDefenceBushouHeisuZeroInIssei = 0x4321D6;
int pTenshouExeJumpCallTaikyakuFuncFromToOnTenshouExeDefenceBushouHeisuZeroInIssei = 0x431598; // 元々の退却用の関数
int pTenshouExeJumpCallDeathFuncFromToOnTenshouExeDefenceBushouHeisuZeroInIssei = 0x4316B8; // 同じタイミングでこちらを呼ぶと死亡する
int pTenshouExeReturnLblFromOnTenshouExeDefenceBushouHeisuZeroInIssei = 0x4321DB;
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec(naked) void WINAPI OnTenshouExeDefenceBushouHeisuZeroInIssei() {
	__asm {
		mov EAXOfDefenceBushouHeisuZeroInIssei, EAX // 一旦退避

		mov EAX, [EBP + 0xC]
		mov iDefenceBushouIDOnHeisuZero, EAX			// 守備武将
		mov EAX, [EBP + 0x8]
		mov iAttackBushouIDOnHeisuZero, EAX			// 攻撃武将

		mov EAX, EAXOfDefenceBushouHeisuZeroInIssei // 基へと戻す

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnTenshouExeDefenceBushouHeisuZeroInIsseiExecute();

	// 得に指定がないままであった。
	if (iIsOverrideDeathFuncOnHeisuZero == false) {
		__asm {

			// スタック復活
			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			// 元の関数を呼ぶ
			call pTenshouExeJumpCallTaikyakuFuncFromToOnTenshouExeDefenceBushouHeisuZeroInIssei

			jmp pTenshouExeReturnLblFromOnTenshouExeDefenceBushouHeisuZeroInIssei
		}
	}
	else {
		__asm {
			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			// 死亡の関数を呼ぶ
			call pTenshouExeJumpCallDeathFuncFromToOnTenshouExeDefenceBushouHeisuZeroInIssei

			jmp pTenshouExeReturnLblFromOnTenshouExeDefenceBushouHeisuZeroInIssei
		}
	}

}
char cmdOnTenshouExeJumpFromDefenceBushouHeisuZeroInIssei[6] = "\xE9";
void WriteAsmJumperOnTenshouExeDefenceBushouHeisuZeroInIssei() {
	int iAddress = (int)OnTenshouExeDefenceBushouHeisuZeroInIssei;
	int SubAddress = iAddress - (pTenshouExeJumpFromToOnTenshouExeDefenceBushouHeisuZeroInIssei + 5);
	memcpy(cmdOnTenshouExeJumpFromDefenceBushouHeisuZeroInIssei + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。 
	WriteProcessMemory(CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeDefenceBushouHeisuZeroInIssei), cmdOnTenshouExeJumpFromDefenceBushouHeisuZeroInIssei, 5, NULL); // 一応安全のため 
}








/*
// 一斉攻撃の結果、「攻撃を受けた武将が兵数０となり、捕縛もしくは退却」発生となった時、ループ中
00432236 | .FF75 0C | PUSH    DWORD PTR SS : [EBP + C]
00432239 | .FF75 F6 | PUSH    DWORD PTR SS : [EBP - A]
0043223C | .E8 57F3FFFF | CALL    TENSHOU.00431598
00432241 | . 83C4 08 | ADD     ESP, 8
*/
int EAXOfDefenceBushouHeisuZeroInIsseiLoop = 0; // 一時保存用
int pTenshouExeJumpFromToOnTenshouExeDefenceBushouHeisuZeroInIsseiLoop = 0x43223C;
int pTenshouExeJumpCallTaikyakuFuncFromToOnTenshouExeDefenceBushouHeisuZeroInIsseiLoop = 0x431598; // 元々の退却用の関数
int pTenshouExeJumpCallDeathFuncFromToOnTenshouExeDefenceBushouHeisuZeroInIsseiLoop = 0x4316B8; // 同じタイミングでこちらを呼ぶと死亡する
int pTenshouExeReturnLblFromOnTenshouExeDefenceBushouHeisuZeroInIsseiLoop = 0x432241;
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec(naked) void WINAPI OnTenshouExeDefenceBushouHeisuZeroInIsseiLoop() {
	__asm {
		mov EAXOfDefenceBushouHeisuZeroInIsseiLoop, EAX // 一旦退避

		mov EAX, [EBP - 0xA]
		mov iDefenceBushouIDOnHeisuZero, EAX			// 守備武将
		mov EAX, [EBP + 0xC]
		mov iAttackBushouIDOnHeisuZero, EAX			// 攻撃武将

		mov EAX, EAXOfDefenceBushouHeisuZeroInIsseiLoop // 基へと戻す

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnTenshouExeDefenceBushouHeisuZeroInIsseiExecute();

	// 得に指定がないままであった。
	if (iIsOverrideDeathFuncOnHeisuZero == false) {
		__asm {

			// スタック復活
			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			// 元の関数を呼ぶ
			call pTenshouExeJumpCallTaikyakuFuncFromToOnTenshouExeDefenceBushouHeisuZeroInIsseiLoop

			jmp pTenshouExeReturnLblFromOnTenshouExeDefenceBushouHeisuZeroInIsseiLoop
		}
	}
	else {
		__asm {
			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			// 死亡の関数を呼ぶ
			call pTenshouExeJumpCallDeathFuncFromToOnTenshouExeDefenceBushouHeisuZeroInIsseiLoop

			jmp pTenshouExeReturnLblFromOnTenshouExeDefenceBushouHeisuZeroInIsseiLoop
		}
	}

}
char cmdOnTenshouExeJumpFromDefenceBushouHeisuZeroInIsseiLoop[6] = "\xE9";
void WriteAsmJumperOnTenshouExeDefenceBushouHeisuZeroInIsseiLoop() {
	int iAddress = (int)OnTenshouExeDefenceBushouHeisuZeroInIsseiLoop;
	int SubAddress = iAddress - (pTenshouExeJumpFromToOnTenshouExeDefenceBushouHeisuZeroInIsseiLoop + 5);
	memcpy(cmdOnTenshouExeJumpFromDefenceBushouHeisuZeroInIsseiLoop + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。 
	WriteProcessMemory(CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeDefenceBushouHeisuZeroInIsseiLoop), cmdOnTenshouExeJumpFromDefenceBushouHeisuZeroInIsseiLoop, 5, NULL); // 一応安全のため 
}







/*
// 鉄砲攻撃の結果、「攻撃を受けた武将が兵数０となり、捕縛もしくは退却」発生となった時、
00432925 | . 56             PUSH    ESI
00432926 | . 57             PUSH    EDI
00432927 | .E8 6CECFFFF    CALL    TENSHOU.00431598
0043292C | . 83C4 08        ADD     ESP, 8

OnTenshouExeJudgeTeppouDeathInBattleで処理するのでここでは何もしない
*/
void OnTenshouExeDefenceBushouHeisuZeroInTeppouExecute() {
	iIsOverrideDeathFuncOnHeisuZero = false;
}

int pTenshouExeJumpFromToOnTenshouExeDefenceBushouHeisuZeroInTeppou = 0x432927;
int pTenshouExeJumpCallTaikyakuFuncFromToOnTenshouExeDefenceBushouHeisuZeroInTeppou = 0x431598; // 元々の退却用の関数
int pTenshouExeJumpCallDeathFuncFromToOnTenshouExeDefenceBushouHeisuZeroInTeppou = 0x4316B8; // 同じタイミングでこちらを呼ぶと死亡する
int pTenshouExeReturnLblFromOnTenshouExeDefenceBushouHeisuZeroInTeppou = 0x43292C;
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec(naked) void WINAPI OnTenshouExeDefenceBushouHeisuZeroInTeppou() {
	__asm {
		mov iDefenceBushouIDOnHeisuZero, EDI			// 守備武将
			mov iAttackBushouIDOnHeisuZero, ESI			// 攻撃武将
			push eax
			push ebx
			push ecx
			push edx
			push esp
			push ebp
			push esi
			push edi
	}

	OnTenshouExeDefenceBushouHeisuZeroInTeppouExecute();

	// 得に指定がないままであった。
	if (iIsOverrideDeathFuncOnHeisuZero == false) {
		__asm {

			// スタック復活
			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			// 元の関数を呼ぶ
			call pTenshouExeJumpCallTaikyakuFuncFromToOnTenshouExeDefenceBushouHeisuZeroInTeppou

			jmp pTenshouExeReturnLblFromOnTenshouExeDefenceBushouHeisuZeroInTeppou
		}
	}
	else {
		__asm {
			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			// 死亡の関数を呼ぶ
			call pTenshouExeJumpCallDeathFuncFromToOnTenshouExeDefenceBushouHeisuZeroInTeppou

			jmp pTenshouExeReturnLblFromOnTenshouExeDefenceBushouHeisuZeroInTeppou
		}
	}

}
char cmdOnTenshouExeJumpFromDefenceBushouHeisuZeroInTeppou[6] = "\xE9";
void WriteAsmJumperOnTenshouExeDefenceBushouHeisuZeroInTeppou() {
	int iAddress = (int)OnTenshouExeDefenceBushouHeisuZeroInTeppou;
	int SubAddress = iAddress - (pTenshouExeJumpFromToOnTenshouExeDefenceBushouHeisuZeroInTeppou + 5);
	memcpy(cmdOnTenshouExeJumpFromDefenceBushouHeisuZeroInTeppou + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。 
	WriteProcessMemory(CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeDefenceBushouHeisuZeroInTeppou), cmdOnTenshouExeJumpFromDefenceBushouHeisuZeroInTeppou, 5, NULL); // 一応安全のため 
}





/* 大砲攻撃の結果、「攻撃を受けた武将が兵数０となり、捕縛もしくは退却」発生となった時、ループ中
00432BE8 | . 56 || PUSH    ESI
00432BE9 | . 53 || PUSH    EBX
00432BEA | .E8 A9E9FFFF || CALL    TENSHOU.00431598
00432BEF | .E9 85000000 || JMP     TENSHOU.00432C79
*/
void OnTenshouExeDefenceBushouHeisuZeroInTaihouExecute() {
	iIsOverrideDeathFuncOnHeisuZero = false;
}

int pTenshouExeJumpFromToOnTenshouExeDefenceBushouHeisuZeroInTaihou = 0x432BEA;
int pTenshouExeJumpCallTaikyakuFuncFromToOnTenshouExeDefenceBushouHeisuZeroInTaihou = 0x431598; // 元々の退却用の関数
int pTenshouExeJumpCallDeathFuncFromToOnTenshouExeDefenceBushouHeisuZeroInTaihou = 0x4316B8; // 同じタイミングでこちらを呼ぶと死亡する
int pTenshouExeReturnLblFromOnTenshouExeDefenceBushouHeisuZeroInTaihou = 0x432BEF;
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec(naked) void WINAPI OnTenshouExeDefenceBushouHeisuZeroInTaihou() {
	__asm {
		mov iDefenceBushouIDOnHeisuZero, EBX			// 守備武将
		mov iAttackBushouIDOnHeisuZero, ESI			// 攻撃武将
		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnTenshouExeDefenceBushouHeisuZeroInTaihouExecute();

	// 得に指定がないままであった。
	if (iIsOverrideDeathFuncOnHeisuZero == false) {
		__asm {

			// スタック復活
			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			// 元の関数を呼ぶ
			call pTenshouExeJumpCallTaikyakuFuncFromToOnTenshouExeDefenceBushouHeisuZeroInTaihou

			jmp pTenshouExeReturnLblFromOnTenshouExeDefenceBushouHeisuZeroInTaihou
		}
	}
	else {
		__asm {
			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			// 死亡の関数を呼ぶ
			call pTenshouExeJumpCallDeathFuncFromToOnTenshouExeDefenceBushouHeisuZeroInTaihou

			jmp pTenshouExeReturnLblFromOnTenshouExeDefenceBushouHeisuZeroInTaihou
		}
	}

}
char cmdOnTenshouExeJumpFromDefenceBushouHeisuZeroInTaihou[6] = "\xE9";
void WriteAsmJumperOnTenshouExeDefenceBushouHeisuZeroInTaihou() {
	int iAddress = (int)OnTenshouExeDefenceBushouHeisuZeroInTaihou;
	int SubAddress = iAddress - (pTenshouExeJumpFromToOnTenshouExeDefenceBushouHeisuZeroInTaihou + 5);
	memcpy(cmdOnTenshouExeJumpFromDefenceBushouHeisuZeroInTaihou + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。 
	WriteProcessMemory(CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeDefenceBushouHeisuZeroInTaihou), cmdOnTenshouExeJumpFromDefenceBushouHeisuZeroInTaihou, 5, NULL); // 一応安全のため 
}



/* 
突撃の結果、「攻撃を受けた武将が兵数０となり、捕縛もしくは退却」発生となった時
00432694  |. 83C4 04        ADD     ESP, 4
00432697  |. 837D F0 01     CMP     DWORD PTR SS:[EBP-10], 1 // 死亡情報が入ってる場所
0043269B  |. 57             PUSH    EDI
0043269C  |. 53             PUSH    EBX
0043269D  |. 75 07          JNZ     SHORT TENSHOU.004326A6   // 死んでなければ捕縛もしくは退却へ
0043269F  |. E8 14F0FFFF    CALL    TENSHOU.004316B8         // 死亡関数
004326A4  |. EB 05          JMP     SHORT TENSHOU.004326AB   // 捕縛もしくは退却はパス
004326A6  |> E8 EDEEFFFF    CALL    TENSHOU.00431598         // 捕縛もしくは退却関数
004326AB  |> 83C4 08        ADD     ESP, 8

OnTenshouExeTotsugekiResultTest　で操作するのでここでは何もしない
*/



/*
戦争をみていない時に、「攻撃を受けた武将が兵数０となり、捕縛もしくは退却」発生となった時
004351CC  /$ FF7424 08      PUSH    DWORD PTR SS:[ESP+8]
004351D0  |. FF7424 08      PUSH    DWORD PTR SS:[ESP+8]
004351D4  |. E8 BFC3FFFF    CALL    TENSHOU.00431598
004351D9  |. 83C4 08        ADD     ESP, 8

戦争をみていない時に、「攻撃を受けた武将が兵戦死」発生となった時
004351DD  /$ FF7424 08      PUSH    DWORD PTR SS:[ESP+8]             ; /Arg2
004351E1  |. FF7424 08      PUSH    DWORD PTR SS:[ESP+8]             ; |Arg1
004351E5  |. E8 CEC4FFFF    CALL    TENSHOU.004316B8                 ; \TENSHOU.004316B8
004351EA  |. 83C4 08        ADD     ESP, 8
004351ED  \. C3             RETN
*/