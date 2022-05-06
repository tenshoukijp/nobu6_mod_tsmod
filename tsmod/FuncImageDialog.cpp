#include "WinTarget.h"

/*
004194AB  |. 50             PUSH    EAX                              ; /Arg6
004194AC  |. 68 30BB4D00    PUSH    TENSHOU.004DBB30                 ; |Arg5 = 004DBB30
004194B1  |. 6A 60          PUSH    60                               ; |Arg4 = 00000060
004194B3  |. B8 FFFF0000    MOV     EAX, 0FFFF                       ; |
004194B8  |. 68 D8000000    PUSH    0D8                              ; |Arg3 = 000000D8
004194BD  |. 50             PUSH    EAX                              ; |Arg2 => 0000FFFF
004194BE  |. 50             PUSH    EAX                              ; |Arg1 => 0000FFFF
004194BF  |. E8 38AC0600    CALL    TENSHOU.004840FC                 ; \TENSHOU.004840FC
*/


//---------------------画像ダイアログ-------------------------
/*
int pCallFromFuncImageDialog = 0x44FAD4; // 元々TENSHOU.EXE内にあったCALL先
int pCallFromFuncImageJunbi = 0x4840FC; // 元々TENSHOU.EXE内にあったCALL先

void FuncImageDialogAsm() {

	__asm {
		 PUSH    EBP
		 call pCallFromFuncImageJunbi
		 ADD     ESP, 4
		 INC     EAX

		// 引数1～3 2バイトずつ
		push eax;
		push 0x4DBB30;
		push 60;
		mov eax, 0xFFFF
		push 0xD8;
		push eax;
		push eax;

		call pCallFromFuncImageDialog
		// スタックがずれ込んだ分(2*3+4 = 10)
		add  esp, 18
	}
}
*/

/*
00412AD2   . BB 907C4D00    MOV     EBX, TENSHOU.004D7C90
00412AD7   . 6A 02          PUSH    2
00412AD9   . 56             PUSH    ESI
00412ADA   . E8 278D0100    CALL    TENSHOU.0042B806
00412ADF   . 83C4 08        ADD     ESP, 8


0048C1E9  |. 6A 2C          PUSH    2C
0048C1EB  |. 68 30BB4D00    PUSH    TENSHOU.004DBB30
0048C1F0  |. E8 11F6F9FF    CALL    TENSHOU.0042B806
0048C1F5  |. 83C4 08        ADD     ESP, 8
*/





int pCallFromFuncImage = 0x4D7C90; // 元々TENSHOU.EXE内にあったCALL先
int pCallFromFuncOpenImageDialog = 0x42b806; // 元々TENSHOU.EXE内にあったCALL先

void FuncOpenImageDialogAsm() {
	__asm {
		push 1
		push [0x4DC590]
		call pCallFromFuncOpenImageDialog
		add esp, 8
	}
}



int pCallFromFuncCloseImageDialog = 0x47D438; // 元々TENSHOU.EXE内にあったCALL先
void FuncCloseImageDialogAsm() {
	__asm {
		mov ecx, [0x4D99F0]
		call pCallFromFuncCloseImageDialog
	}
}


// ★バグってて使えない!!
void FuncOpenImageDialog() {
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
	FuncOpenImageDialogAsm();

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

void FuncCloseImageDialog() {
	__asm {
		push eax
		push ebx
		push ecx
		push edx
		push esp
		push esi
		push edi
	}

	FuncCloseImageDialogAsm();

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