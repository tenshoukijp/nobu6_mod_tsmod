#include "WinTarget.h"

#include "GameDataStruct.h"

/*
0041C09A   . 57             PUSH    EDI					← 武将番号
0041C09B   . E8 198D0100    CALL    TENSHOU.00434DB9
0041C0A0   . 83C4 04        ADD     ESP, 4


*/

int pCallFromFuncDoTaikyakuInBattle = 0x434DB9; // 元々TENSHOU.EXE内にあったCALL先

int iBushouIDInFuncDoTaikyakuInBattle = 1L;    // 死亡武将ID

// 戦争画面で指定の武将を殺害
void FuncDoTaikyakuInBattleAsm() {
	__asm {
		push iBushouIDInFuncDoTaikyakuInBattle
		call pCallFromFuncDoTaikyakuInBattle
		add esp, 0x4
	}
}


// 戦争画面で指定の武将を殺害
int FuncDoTaikyakuInBattle(int iBushouID) {

	// 野戦中でも攻城戦中でもないならば、何もしない。
	if ( !Is_FieldWar() && !Is_CastleWar() ) {
		return false;
	}

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// すでに死亡しているならダメ
		if ( nb6bushouname[iBushouID].battleDeath ) {
			return FALSE;
		}
		// すでに退却しているならダメ
		if ( nb6bushouname[iBushouID].battleRetreat ) {
			return FALSE;
		}

		// 関数用のやつは、1つ多いので、足しこみ
		iBushouIDInFuncDoTaikyakuInBattle = iBushouID+1;
	
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
		FuncDoTaikyakuInBattleAsm();

		__asm {
			pop edi
			pop esi
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax
		}

		// ここの処理自体、退却判定成功時後のものなので、成功する。
		return TRUE;

	}

	// 何もしていない
	return FALSE;
}
