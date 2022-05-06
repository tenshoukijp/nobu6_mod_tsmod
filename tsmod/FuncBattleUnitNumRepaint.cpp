#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "FuncSelectMenuDialog.h"
#include "GameDataStruct.h"
#include "OutputDebugStream.h"


/*
戦闘画面、ユニットの再描画。兵数部分が中心
*


/*
00433F8C  |. 56             PUSH    ESI
00433F8D  |. E8 FE6E0600    CALL    TENSHOU.0049AE90
00433F92  |. 83C4 04        ADD     ESP, 4
*/

int pCallFromFuncBattleUnitNumRepaint = 0x49AE90; //
int iBushouIDOfBattleUnitNumRepaint = -1;

void FuncBattleUnitNumRepaintAsm() {
	__asm {
		PUSH    iBushouIDOfBattleUnitNumRepaint
		CALL    pCallFromFuncBattleUnitNumRepaint
		ADD     ESP, 4
	}
}

// 指定の武将の数値を再描画する。(問題なく機能する)
void FuncBattleUnitNumRepaint(int iBushouID) {
	if ( !(0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) ) {
		return;
	}

	iBushouIDOfBattleUnitNumRepaint = iBushouID+1;

	__asm {
		push eax
		push ebx
		push ecx
		push edx
		push esp
		push esi
		push edi
	}

	// TENSHOU.EXE内の関数呼び出し
	FuncBattleUnitNumRepaintAsm();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}
}




// 範囲攻撃の方は
/*
00432BA0  |. 6A 01          |PUSH    1                               ; /Arg5 = 00000001		← １固定
00432BA2  |. 8B45 FC        |MOV     EAX, DWORD PTR SS:[EBP-4]       ; |
00432BA5  |. 48             |DEC     EAX                             ; |
00432BA6  |. 8D4D C6        |LEA     ECX, DWORD PTR SS:[EBP-3A]      ; |
00432BA9  |. 50             |PUSH    EAX                             ; |Arg4				←　１固定
00432BAA  |. 51             |PUSH    ECX                             ; |Arg3				←　複数の武将のリストが入っている配列(武将番号[+1の方]）のリスト。リストの最後は0xFFFFであること。
00432BAB  |. FF75 E8        |PUSH    DWORD PTR SS:[EBP-18]           ; |Arg2				←　目標の着弾点のＹ座標
00432BAE  |. FF75 E4        |PUSH    DWORD PTR SS:[EBP-1C]           ; |Arg1				←　目標の着弾点のＸ座標
00432BB1  |. E8 F4800600    |CALL    TENSHOU.0049ACAA                ; \TENSHOU.0049ACAA
00432BB6  |. 83C4 14        |ADD     ESP, 14



0049ACAA  /$ 55             PUSH    EBP
～
0049AE8F  \. C3             RETN
*/


int pCallFromFuncBattlePosAroundUnitNumRepaint = 0x49ACAA; //
int iPoxXOfBattlePosAroundUnitNumRepaint = -1;
int iPoxYOfBattlePosAroundUnitNumRepaint = -1;

WORD arrayOfBushouIDOfBattlePosAroundUnitNumRepaint[16] = {0}; // 最大でも６人のはずだけど一応念のために、１６人分の配列。

void FuncBattlePosAroundUnitNumRepaintAsm() {
	__asm {
		PUSH    1
		PUSH    1
		PUSH    arrayOfBushouIDOfBattlePosAroundUnitNumRepaint
		PUSH	iPoxXOfBattlePosAroundUnitNumRepaint
		PUSH	iPoxYOfBattlePosAroundUnitNumRepaint
		CALL    pCallFromFuncBattleUnitNumRepaint
		ADD     ESP, 14 //4 * 5
	}
}

// 指定座標周辺の描画を更新する。(★機能しない!!)
void FuncBattlePosAroundUnitNumRepaint(HEX_POSITION pos) {

	int x = 0xFFFF;
	int y = 0xFFFF;

	if ( Is_FieldWar() ) {

		// 攻城戦時のヘックス系の座標を、配列としての実値のindexへと変更する。
		ConvertFieldHexPosToFieldIndexPos(pos.x, pos.y, &x, &y);

	} else if ( Is_CastleWar() ) {

		// 攻城戦時のヘックス系の座標を、配列としての実値のindexへと変更する。
		ConvertCastleHexPosToCastleIndexPos(pos.x, pos.y, &x, &y);

	}

	// どちらかが足でてたらアウト
	if ( x >= 0xFFFF || y >= 0xFFFF ) {
		return;
	}

	// 座標を転写
	iPoxXOfBattlePosAroundUnitNumRepaint = x;
	iPoxYOfBattlePosAroundUnitNumRepaint = y;

	// 0xFFFFで埋めておく
	for ( int i=0; i<int( sizeof(arrayOfBushouIDOfBattlePosAroundUnitNumRepaint)/sizeof(arrayOfBushouIDOfBattlePosAroundUnitNumRepaint[0]) ); i++ ) {
		arrayOfBushouIDOfBattlePosAroundUnitNumRepaint[i] = 0xFFFF;
	}

	__asm {
		push eax
		push ebx
		push ecx
		push edx
		push esp
		push esi
		push edi
	}

	// TENSHOU.EXE内の関数呼び出し
	FuncBattlePosAroundUnitNumRepaintAsm();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}
}

