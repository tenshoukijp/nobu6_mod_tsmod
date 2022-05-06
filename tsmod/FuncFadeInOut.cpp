#include "WinTarget.h"


/*
00457D90  |. 83C4 04        ADD     ESP, 4
00457D93  |. E8 56B5FCFF    CALL    TENSHOU.004232EE
00457D98  |. 6A 01          PUSH    1                                ; /Arg3 = 00000001
00457D9A  |. B9 18C24D00    MOV     ECX, TENSHOU.004DC218            ; |



00457DA7  |. E8 28B5FCFF    CALL    TENSHOU.004232D4
*/


//---------------------フェイドアウト-------------------------
int pCallFromFuncFadeOut = 0x4232EE; // 元々TENSHOU.EXE内にあったCALL先

void FuncFadeOutAsm() {

	__asm {
		// 引数なし
		call pCallFromFuncFadeOut
		// 引数なしなのでスタックのずれ込みは無し
	}

}

void FuncFadeOut() {
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
	FuncFadeOutAsm();

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


//---------------------フェイドイン-------------------------
int pCallFromFuncFadeIn = 0x4232D4; // 元々TENSHOU.EXE内にあったCALL先

void FuncFadeInAsm() {

	__asm {
		push eax
		push ebx
		push ecx
		push edx
		push esp
		push esi
		push edi
	}

	__asm {
		// 引数なし
		call pCallFromFuncFadeIn
		// 引数なしなのでスタックのずれ込みは無し
	}

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

void FuncFadeIn() {
	// TENSHOU.EXE内の関数呼び出し
	FuncFadeInAsm();
}


void FuncScreenUpdate() {
	__asm {
		push eax
		push ebx
		push ecx
		push edx
		push esp
		push esi
		push edi
	}

	__asm {
		SUB ESP, 0x2A0
		LEA EAX, DWORD PTR SS : [ESP]
		PUSH 0x0
		PUSH 0x2A0
		PUSH EAX
		LEA EAX, DWORD PTR SS : [ESP + 0xC]
		ADD ESP, 0xC
		PUSH EAX
		PUSH 0xE0
		PUSH 0x10
		ADD ESP, 0x2AC
	}

	__asm {
		// 引数なし
		call pCallFromFuncFadeIn
		// 引数なしなのでスタックのずれ込みは無し
	}

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