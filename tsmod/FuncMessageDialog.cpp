#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1 

#include "WinTarget.h"


static char szFuncDialog[255] = "";
static char *pszFuncDialog = szFuncDialog;



/* // フレーム用のダイアログ
0043666B  |> 68 FFFF0000    PUSH    0FFFF
00436670  |. 6A 01          PUSH    1
00436672  |. 68 C0974D00    PUSH    TENSHOU.004D97C0                                  ;  ASCII "上泉信綱隊は
混乱状態から回復しました"
00436677  |. E8 23E30600    CALL    TENSHOU.004A499F
0043667C  |. 83C4 0C        ADD     ESP, 0C
*/


//---------------------戦闘中などで良くでる確認フレームダイアログ-------------------------
int pCallFromFuncCondirmFrameDialog = 0x4A499F; // 元々TENSHOU.EXE内にあったCALL先

void FuncInfoFrameDialogAsm() {

	__asm {
		// 引数1～3 2バイトずつ
		push 0xffff;
		push 1;

		// 引数4 文字列へのポインタ(ポインタ=4バイト)
		push pszFuncDialog;
		call pCallFromFuncCondirmFrameDialog
		// スタックがずれ込んだ分(2*3+4 = 10)
		add  esp, 0xC
	}

}

void FuncInfoFrameDialogAsm(char *szCondirmFrameDialogMessage) {

	strcpy( szFuncDialog, szCondirmFrameDialogMessage );

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
	FuncInfoFrameDialogAsm();

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



/*
004A1831  /$ 55             PUSH    EBP
004A1832  |. 8BEC           MOV     EBP, ESP
004A1834  |. FF7424 14      PUSH    DWORD PTR SS:[ESP+14]
004A1838  |. FF75 10        PUSH    DWORD PTR SS:[EBP+10]
004A183B  |. FF75 0C        PUSH    DWORD PTR SS:[EBP+C]
004A183E  |. 6A 01          PUSH    1
004A1840  |. 6A 0F          PUSH    0F
004A1842  |. 6A 0A          PUSH    0A
004A1844  |. 68 48384D00    PUSH    TENSHOU.004D3848                 ;  ASCII "警告！"
004A1849  |. FF75 08        PUSH    DWORD PTR SS:[EBP+8]
004A184C  |. E8 4DFFFFFF    CALL    TENSHOU.004A179E
004A1851  |. 8BE5           MOV     ESP, EBP
004A1853  |. 5D             POP     EBP
004A1854  \. C3             RETN

004A1855  /$ 55             PUSH    EBP
004A1856  |. 8BEC           MOV     EBP, ESP
004A1858  |. FF7424 14      PUSH    DWORD PTR SS:[ESP+14]
004A185C  |. FF75 10        PUSH    DWORD PTR SS:[EBP+10]
004A185F  |. FF75 0C        PUSH    DWORD PTR SS:[EBP+C]
004A1862  |. 6A 00          PUSH    0
004A1864  |. 6A 0F          PUSH    0F
004A1866  |. 6A 00          PUSH    0
004A1868  |. 68 50384D00    PUSH    TENSHOU.004D3850                 ;  ASCII "情報"
004A186D  |. FF75 08        PUSH    DWORD PTR SS:[EBP+8]
004A1870  |. E8 29FFFFFF    CALL    TENSHOU.004A179E
004A1875  |. 8BE5           MOV     ESP, EBP
004A1877  |. 5D             POP     EBP
004A1878  \. C3             RETN
*/
/*
Yes/Noダイアログ
004A18D0  /$ 56             PUSH    ESI
004A18D1  |. B8 FFFF0000    MOV     EAX, 0FFFF
004A18D6  |. 50             PUSH    EAX                              ; /Arg8 => 0000FFFF
004A18D7  |. 50             PUSH    EAX                              ; |Arg7 => 0000FFFF
004A18D8  |. 50             PUSH    EAX                              ; |Arg6 => 0000FFFF
004A18D9  |. B9 38914D00    MOV     ECX, TENSHOU.004D9138            ; |ASCII "XWK"
004A18DE  |. 6A 0F          PUSH    0F                               ; |Arg5 = 0000000F
004A18E0  |. 6A 00          PUSH    0                                ; |Arg4 = 00000000
004A18E2  |. 6A 03          PUSH    3                                ; |Arg3 = 00000003
004A18E4  |. 68 58384D00    PUSH    TENSHOU.004D3858                 ; |Arg2 = 004D3858
004A18E9  |. FF7424 24      PUSH    DWORD PTR SS:[ESP+24]            ; |Arg1
004A18ED  |. E8 77F6FFFF    CALL    TENSHOU.004A0F69                 ; \TENSHOU.004A0F69
004A18F2  |. 6A 02          PUSH    2                                ; /Arg1 = 00000002
004A18F4  |. E8 BB90FAFF    CALL    TENSHOU.0044A9B4                 ; \TENSHOU.0044A9B4
004A18F9  |. 83C4 04        ADD     ESP, 4
004A18FC  |. E8 D69FFDFF    CALL    TENSHOU.0047B8D7
004A1901  |. 8BF0           MOV     ESI, EAX
004A1903  |. B9 38914D00    MOV     ECX, TENSHOU.004D9138            ;  ASCII "XWK"
004A1908  |. E8 44F9FFFF    CALL    TENSHOU.004A1251
004A190D  |. 83FE 01        CMP     ESI, 1
004A1910  |. 5E             POP     ESI
004A1911  |. 1BC0           SBB     EAX, EAX
004A1913  |. 40             INC     EAX
004A1914  \. C3             RETN
*/




//---------------------二者選択ダイアログ-------------------------
int pCallFromFuncOkCancelDialogAsm = 0x4A18D0; // 元々TENSHOU.EXE内にあったCALL先
int iResultOfFuncOkCancelDialog = 0;
void FuncOkCancelDialogAsm() {
	__asm {
		// 引数1～3 2バイトずつ
		// 引数4 文字列へのポインタ(ポインタ=4バイト)
		push pszFuncDialog;
		call pCallFromFuncOkCancelDialogAsm
		// スタックがずれ込んだ分(2*3+4 = 10)
		add  esp, 4
		mov iResultOfFuncOkCancelDialog, eax;
	}

}

int FuncOkCancelDialog(char *szInfoDialogMessage) {

	strcpy( szFuncDialog, szInfoDialogMessage );

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
	FuncOkCancelDialogAsm();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

	// 返答がYesだったかNoだったか返す。
	return iResultOfFuncOkCancelDialog;
}






//---------------------普通に消えてしまうダイアログ-------------------------
int pCallFromFuncInfoDialogAsm = 0x4A1855; // 元々TENSHOU.EXE内にあったCALL先

void FuncInfoDialogAsm() {
	__asm {
		// 引数1～3 2バイトずつ
		push word ptr 0xffff;
		push word ptr 0xffff;
		push word ptr 0xffff;
		// 引数4 文字列へのポインタ(ポインタ=4バイト)
		push pszFuncDialog;
		call pCallFromFuncInfoDialogAsm
		// スタックがずれ込んだ分(2*3+4 = 10)
		add  esp, 0xA
	}

}

void FuncInfoDialog(char *szInfoDialogMessage) {

	strcpy( szFuncDialog, szInfoDialogMessage );

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
	FuncInfoDialogAsm();

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







//---------------------確認押すまで消えないダイアログ-------------------------
int pCallFromFuncConfirmDialogAsm = 0x4A187A; // 元々TENSHOU.EXE内にあったCALL先

void FuncConfirmDialogAsm() {

	__asm {
		// 引数1～3 2バイトずつ
		push word ptr 0xffff;
		push word ptr 0xffff;
		push word ptr 0xffff;
		// 引数4 文字列へのポインタ(ポインタ=4バイト)
		push pszFuncDialog;
		call pCallFromFuncConfirmDialogAsm
		// スタックがずれ込んだ分(2*3+4 = 10)
		add  esp, 0xA
	}

}

void FuncConfirmDialog(char *szInfoDialogMessage) {

	strcpy( szFuncDialog, szInfoDialogMessage );

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
	FuncConfirmDialogAsm();

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







//---------------------警告系消えてしまうダイアログ-------------------------
int pCallFromFuncAssertDialog = 0x4A1831; // 元々TENSHOU.EXE内にあったCALL先

void FuncAssertDialogAsm() {

	__asm {
		// 引数1～3 2バイトずつ
		push word ptr 0xffff;
		push word ptr 0xffff;
		push word ptr 0xffff;
		// 引数4 文字列へのポインタ(ポインタ=4バイト)
		push pszFuncDialog;
		call pCallFromFuncAssertDialog
		// スタックがずれ込んだ分(2*3+4 = 10)
		add  esp, 0xA
	}

}

void FuncAssertDialog(char *szInfoDialogMessage) {

	strcpy( szFuncDialog, szInfoDialogMessage );

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
	FuncAssertDialogAsm();

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