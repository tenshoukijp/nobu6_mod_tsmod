#include "WinTarget.h"

#include "GameDataStruct.h"

/*
ESI 000000D8
EDI 000000BD

00431729  |. 6A 02          PUSH    2                                ; /Arg3 = 00000002 (=���S�B�Œ�l�B���̒l���ƕߔ��ȂǂɂȂ�B)
0043172B  |. 57             PUSH    EDI                              ; |Arg2 = �E������(�E�����l�����Ȃ���΁A�E���ꂽ�l�Ɠ����ԍ��ɂ��Ă���)
0043172C  |. 56             PUSH    ESI                              ; |Arg1 = �E���ꂽ��
0043172D  |. E8 43F3FFFF    CALL    TENSHOU.00430A75                 ; \TENSHOU.00430A75
00431732  |. 83C4 0C        ADD     ESP, 0C

00431741  |. 68 00040000    PUSH    400
00431746  |. 56             PUSH    ESI
00431747  |. E8 068CFDFF    CALL    TENSHOU.0040A352
0043174C  |. 83C4 08        ADD     ESP, 8
*/

int pCallFromFuncDoDeathInBattle = 0x430A75; // ���XTENSHOU.EXE���ɂ�����CALL��BUnit�n���n�������̏C���B��ꂩ��̑ދ�
int pCallFromFuncDoDeathInBattle2 = 0x40A352; // ���XTENSHOU.EXE���ɂ�����CALL��BNB6BUSHOU�n�������Ő��펀�t���O�𗧂Ă���

int iKilledBushouIDInFuncDoDeathInBattle = 1L;    // �E����镐��ID
int iCriminalBushouIDInFuncDoDeathInBattle = 1L;    // �Ɛl�̕���ID

// �퓬��ʂŎw��̕������E�Q
void FuncDoDeathInBattleAsm() {
	__asm {
		// Unit�n���n�������̏C���B��ꂩ��̑ދ�
		push 0x2
		push iCriminalBushouIDInFuncDoDeathInBattle
		push iKilledBushouIDInFuncDoDeathInBattle
		call pCallFromFuncDoDeathInBattle
		add esp, 0xC

		// NB6BUSHOU�n�������Ő��펀�t���O�𗧂Ă���
		push 0x400
		push iKilledBushouIDInFuncDoDeathInBattle
		call pCallFromFuncDoDeathInBattle2
		add esp, 0x8
	}
}


// �퓬��ʂŎw��̕������E�Q

void FuncDoDeathInBattle(int iKilledBushouID, int iCriminalBushouID) {

	// �Ɛl���w�肵�Ȃ���΁A���E�Ƃ������Ƃɂ���B
	if ( iCriminalBushouID == -1 ) {
		iCriminalBushouID = iKilledBushouID;	
	}

	if ( 0 <= iKilledBushouID && iKilledBushouID < GAMEDATASTRUCT_BUSHOU_NUM && 0 <= iCriminalBushouID && iCriminalBushouID < GAMEDATASTRUCT_BUSHOU_NUM  ) {

		// ���łɐ펀���Ă����牽�����Ȃ�
		if ( nb6bushouname[iKilledBushouID].battleDeath ) {
			return;
		}

		// �֐��p�̂�́A1�����̂ŁA��������
		iKilledBushouIDInFuncDoDeathInBattle = iKilledBushouID+1;		// �E����鑤�̕���ID
		iCriminalBushouIDInFuncDoDeathInBattle = iCriminalBushouID+1;    // �Ɛl�̕���ID

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
