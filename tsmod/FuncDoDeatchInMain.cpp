#include "WinTarget.h"

#include "GameDataStruct.h"

/*
00403EAE   . 56             PUSH ESI			   �� ���ꂪ�����ԍ�
00403EAF   . E8 AE7A0000    CALL TENSHOU.0040B962�@�� ����ŁA���S�����{�K�v�Ȃt�h�n�������s����B
00403EB4   83C4 04          ADD     ESP, 4

*/

int pCallFromFuncDoDeathInMain = 0x403EAF; // ���XTENSHOU.EXE���ɂ�����CALL��

int iBushouIDInFuncDoDeathInMain = 1L;    // ���S����ID

// ���C����ʂŎw��̕������E�Q
void FuncDoDeathInMainAsm() {
	__asm {
		mov esi, iBushouIDInFuncDoDeathInMain
		push iBushouIDInFuncDoDeathInMain
		call pCallFromFuncDoDeathInMain
		add esp, 0x4
	}
}


// ���C����ʂŎw��̕������E�Q
void FuncDoDeathInMain(int iBushouID) {

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		// �֐��p�̂�́A1�����̂ŁA��������
		iBushouIDInFuncDoDeathInMain = iBushouID+1;
	
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
		FuncDoDeathInMainAsm();

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
