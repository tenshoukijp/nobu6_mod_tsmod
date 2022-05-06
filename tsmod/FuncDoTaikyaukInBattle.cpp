#include "WinTarget.h"

#include "GameDataStruct.h"

/*
0041C09A   . 57             PUSH    EDI					�� �����ԍ�
0041C09B   . E8 198D0100    CALL    TENSHOU.00434DB9
0041C0A0   . 83C4 04        ADD     ESP, 4


*/

int pCallFromFuncDoTaikyakuInBattle = 0x434DB9; // ���XTENSHOU.EXE���ɂ�����CALL��

int iBushouIDInFuncDoTaikyakuInBattle = 1L;    // ���S����ID

// �푈��ʂŎw��̕������E�Q
void FuncDoTaikyakuInBattleAsm() {
	__asm {
		push iBushouIDInFuncDoTaikyakuInBattle
		call pCallFromFuncDoTaikyakuInBattle
		add esp, 0x4
	}
}


// �푈��ʂŎw��̕������E�Q
int FuncDoTaikyakuInBattle(int iBushouID) {

	// ��풆�ł��U��풆�ł��Ȃ��Ȃ�΁A�������Ȃ��B
	if ( !Is_FieldWar() && !Is_CastleWar() ) {
		return false;
	}

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���łɎ��S���Ă���Ȃ�_��
		if ( nb6bushouname[iBushouID].battleDeath ) {
			return FALSE;
		}
		// ���łɑދp���Ă���Ȃ�_��
		if ( nb6bushouname[iBushouID].battleRetreat ) {
			return FALSE;
		}

		// �֐��p�̂�́A1�����̂ŁA��������
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

		// TENSHOU.EXE���̊֐��Ăяo��
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

		// �����̏������́A�ދp���萬������̂��̂Ȃ̂ŁA��������B
		return TRUE;

	}

	// �������Ă��Ȃ�
	return FALSE;
}
