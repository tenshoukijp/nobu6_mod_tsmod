
#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "FuncSelectMenuDialog.h"
#include "OutputDebugStream.h"


/*
�R�c�E�B���h�E�{��E�B���h�E�̕`��X�V
*


/*
0041064C  |. E8 F6030000    |CALL    TENSHOU.00410A47		���R�c�E�B���h�E�{��E�B���h�E�̕`��X�V
*/

int pCallFromFuncGundanCastleWindowRepaint = 0x410A47;

void FuncGundanCastleWindowRepaintAsm() {
	__asm {
		CALL    pCallFromFuncGundanCastleWindowRepaint
	}
}

void FuncGundanCastleWindowRepaint() {
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
	FuncGundanCastleWindowRepaintAsm();

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