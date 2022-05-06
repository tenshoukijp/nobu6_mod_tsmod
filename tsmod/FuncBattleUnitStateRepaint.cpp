#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "FuncSelectMenuDialog.h"
#include "GameDataStruct.h"
#include "OutputDebugStream.h"

/*
�퓬��ʁA���j�b�g�̍�����卬���ȂǁA�A�C�R����ԍX�V
/*
00430619  |. 56             PUSH    ESI                              ; /Arg1
0043061A  |. E8 EB9A0600    CALL    TENSHOU.0049A10A                 ; \TENSHOU.0049A10A
0043061F  |. 83C4 04        ADD     ESP, 4
*/
int pfuncAddress = 0x49A10A;
int pCallFromFuncBattleUnitStatusRepaint = 0x49A10A; //
int iBushouIDOfBattleUnitStatusRepaint = -1;

void FuncBattleUnitStatusRepaintAsm() {
	__asm {
		push iBushouIDOfBattleUnitStatusRepaint
		call pCallFromFuncBattleUnitStatusRepaint
		add esp, 4
	}
}


// �w��̕����̃A�C�R����Ԃ��ĕ`�悷��B(���Ȃ��@�\����)
void FuncBattleUnitStatusRepaint(int iBushouID) {

	if ( !(0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) ) {
		return;
	}

	iBushouIDOfBattleUnitStatusRepaint = iBushouID+1;

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
	FuncBattleUnitStatusRepaintAsm();

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
