#include "WinTarget.h"

#include "GameDataStruct.h"

/*
ESI 000000D8
EDI 000000BD

00431729  |. 6A 02          PUSH    2                                ; /Arg3 = 00000002 (=死亡。固定値。他の値だと捕縛などになる。)
0043172B  |. 57             PUSH    EDI                              ; |Arg2 = 殺した側(殺した人がいなければ、殺された人と同じ番号にしておく)
0043172C  |. 56             PUSH    ESI                              ; |Arg1 = 殺された側
0043172D  |. E8 43F3FFFF    CALL    TENSHOU.00430A75                 ; \TENSHOU.00430A75
00431732  |. 83C4 0C        ADD     ESP, 0C

00431741  |. 68 00040000    PUSH    400
00431746  |. 56             PUSH    ESI
00431747  |. E8 068CFDFF    CALL    TENSHOU.0040A352
0043174C  |. 83C4 08        ADD     ESP, 8
*/

int pCallFromFuncDoDeathInBattle = 0x430A75; // 元々TENSHOU.EXE内にあったCALL先。Unit系情報系メモリの修正。戦場からの退去
int pCallFromFuncDoDeathInBattle2 = 0x40A352; // 元々TENSHOU.EXE内にあったCALL先。NB6BUSHOU系メモリで戦場戦死フラグを立てたり

int iKilledBushouIDInFuncDoDeathInBattle = 1L;    // 殺される武将ID
int iCriminalBushouIDInFuncDoDeathInBattle = 1L;    // 犯人の武将ID

// 戦闘画面で指定の武将を殺害
void FuncDoDeathInBattleAsm() {
	__asm {
		// Unit系情報系メモリの修正。戦場からの退去
		push 0x2
		push iCriminalBushouIDInFuncDoDeathInBattle
		push iKilledBushouIDInFuncDoDeathInBattle
		call pCallFromFuncDoDeathInBattle
		add esp, 0xC

		// NB6BUSHOU系メモリで戦場戦死フラグを立てたり
		push 0x400
		push iKilledBushouIDInFuncDoDeathInBattle
		call pCallFromFuncDoDeathInBattle2
		add esp, 0x8
	}
}


// 戦闘画面で指定の武将を殺害

void FuncDoDeathInBattle(int iKilledBushouID, int iCriminalBushouID) {

	// 犯人を指定しなければ、自殺ということにする。
	if ( iCriminalBushouID == -1 ) {
		iCriminalBushouID = iKilledBushouID;	
	}

	if ( 0 <= iKilledBushouID && iKilledBushouID < GAMEDATASTRUCT_BUSHOU_NUM && 0 <= iCriminalBushouID && iCriminalBushouID < GAMEDATASTRUCT_BUSHOU_NUM  ) {

		// すでに戦死していたら何もしない
		if ( nb6bushouname[iKilledBushouID].battleDeath ) {
			return;
		}

		// 関数用のやつは、1つ多いので、足しこみ
		iKilledBushouIDInFuncDoDeathInBattle = iKilledBushouID+1;		// 殺される側の武将ID
		iCriminalBushouIDInFuncDoDeathInBattle = iCriminalBushouID+1;    // 犯人の武将ID

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
		FuncDoDeathInBattleAsm();

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
}
