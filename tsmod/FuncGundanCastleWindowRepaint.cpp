
#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "FuncSelectMenuDialog.h"
#include "OutputDebugStream.h"


/*
軍団ウィンドウ＋城ウィンドウの描画更新
*


/*
0041064C  |. E8 F6030000    |CALL    TENSHOU.00410A47		←軍団ウィンドウ＋城ウィンドウの描画更新
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

	// TENSHOU.EXE内の関数呼び出し
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