#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "FuncSelectMenuDialog.h"
#include "OutputDebugStream.h"


/*
�퓬��ʁA�ĕ`��H
*


/*
0046AFB4  |> 6A 01          PUSH    1
0046AFB6  |. B9 78E84D00    MOV     ECX, TENSHOU.004DE878
0046AFBB  |. E8 7278FFFF    CALL    TENSHOU.00462832
*/

int pCallFromFuncWarScreenRepaint = 0x462832; //

void FuncWarScreenRepaintAsm() {
	__asm {
		PUSH    0x01
		MOV     ECX, 0x4DE878
		CALL    pCallFromFuncWarScreenRepaint
	}
}

// ���@�\���Ȃ�!!
void FuncWarScreenRepaint() {
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
	FuncWarScreenRepaintAsm();

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