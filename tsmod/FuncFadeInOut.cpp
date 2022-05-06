#include "WinTarget.h"


/*
00457D90  |. 83C4 04        ADD     ESP, 4
00457D93  |. E8 56B5FCFF    CALL    TENSHOU.004232EE
00457D98  |. 6A 01          PUSH    1                                ; /Arg3 = 00000001
00457D9A  |. B9 18C24D00    MOV     ECX, TENSHOU.004DC218            ; |



00457DA7  |. E8 28B5FCFF    CALL    TENSHOU.004232D4
*/


//---------------------�t�F�C�h�A�E�g-------------------------
int pCallFromFuncFadeOut = 0x4232EE; // ���XTENSHOU.EXE���ɂ�����CALL��

void FuncFadeOutAsm() {

	__asm {
		// �����Ȃ�
		call pCallFromFuncFadeOut
		// �����Ȃ��Ȃ̂ŃX�^�b�N�̂��ꍞ�݂͖���
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
		
	// TENSHOU.EXE���̊֐��Ăяo��
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


//---------------------�t�F�C�h�C��-------------------------
int pCallFromFuncFadeIn = 0x4232D4; // ���XTENSHOU.EXE���ɂ�����CALL��

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
		// �����Ȃ�
		call pCallFromFuncFadeIn
		// �����Ȃ��Ȃ̂ŃX�^�b�N�̂��ꍞ�݂͖���
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
	// TENSHOU.EXE���̊֐��Ăяo��
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
		// �����Ȃ�
		call pCallFromFuncFadeIn
		// �����Ȃ��Ȃ̂ŃX�^�b�N�̂��ꍞ�݂͖���
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