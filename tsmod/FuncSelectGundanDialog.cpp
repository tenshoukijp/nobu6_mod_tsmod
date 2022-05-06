#define _CRT_SECURE_NO_WARNINGS

#include "GameDataStruct.h"
#include "FuncSelectGundanDialog.h"
#include "CommonGlobalHandle.h"
/*
0045036E  |> 6A 00          /PUSH 0                                  ; /Arg5 = 00000000
00450370  |. 6A 00          |PUSH 0                                  ; |Arg4 = 00000000
00450372  |. 56             |PUSH ESI                                ; |Arg3
00450373  |. 68 7FCD0000    |PUSH 0CD7F                              ; |Arg2 = 0000CD7F (軍団選択用固定。表示項目を選べる)
00450378  |. 68 D8934D00    |PUSH TENSHOU.004D93D8                   ; |Arg1 = 004D93D8
0045037D  |. E8 D7B30000    |CALL TENSHOU.0045B759                   ; \TENSHOU.0045B759
00450382  |. 66:8BF8        |MOV DI,AX
00450385  |. 83C4 14        |ADD ESP,14
*/

/*
0045B763  |. FF75 10        PUSH    DWORD PTR SS:[EBP+10]            ; |Arg4
0045B766  |. FF75 0C        PUSH    DWORD PTR SS:[EBP+C]             ; |Arg3
0045B769     6A 01          PUSH    1										この場所の人数が、選択可能人数 1固定で書いてあるが、0に切り替え可能とする。
0045B76B  |. FF75 08        PUSH    DWORD PTR SS:[EBP+8]             ; |Arg1
0045B76E  |. E8 EEFEFFFF    CALL    TENSHOU.0045B661                 ; \TENSHOU.0045B661
*/


//---------------------選択軍団ダイアログ-------------------------
int pCallFromFuncSelectGundanDialog = 0x45B759; // 元々TENSHOU.EXE内にあったCALL先
int iReturnOfSelectGundanDialog = 0; // 返り値。AXの部分のみ採用する。
WORD iArrayOfGundanList[GAMEDATASTRUCT_GUNDAN_NUM+1] = {}; // 表示する対象となる軍団リスト番号。最後は0xFFFFで終わりとみなす。
int iFilterOptionOfSelectGundanDialog = Filter::Gundan::デフォルト;
static int iMultiGundanNumOfSelectGundanDialog = 1; // 選択可能な軍団の最大数。

void FuncSelectGundanDialogAsm() {

	__asm {
		push 0
		push 0
		push 1 
		push iFilterOptionOfSelectGundanDialog // フィルター値
		push 0x4D93D8 // WORDで軍団の番号の配列を格納してる場所。最後0xFFFF

		call pCallFromFuncSelectGundanDialog

		mov iReturnOfSelectGundanDialog, eax

		add  esp, 0x14
	}
}

// 対象となる軍団リスト。
int FuncSelectGundanDialog(vector<int> list, int iFilterOption ) {

	// リストが無いならなにもしない。
	if ( list.size() == 0 ) {
		return 0xFFFF;
	}

	// 渡されたvector形式のリストをWORD形式の配列へのコピーしていく。
	WORD *pCopy = iArrayOfGundanList;
	// 軍団のリストを配列形式にする。
	for ( vector<int>::iterator it=list.begin(); it != list.end(); it++ ) {

		int iGundanID = *it;
		// 範囲外は無視
		if ( ! (0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM) ) {
			continue;
		}
		// 軍団長が居ない番号は無視
		if ( nb6gundan[iGundanID].leader == 0xFFFF ) {
			continue;
		}
		
		*pCopy = (WORD)iGundanID+1; // 配列用→番号にして配列に格納
		pCopy++;
	}
	
	// コピー委ポインタが１つも進んでいない。==１つも有効な軍団が居なかった
	if ( pCopy == iArrayOfGundanList ) {
		return 0xFFFF;
	}

	*pCopy = 0xFFFF; // 最後の部分に0xFFFFを足す。

	// 0x4D93D8 // WORDで軍団の番号の配列を格納してる場所。
	// 軍団リストの内容をtenshou.exeメモリへと複製展開。
	memcpy( (void *)0x4D93D8, iArrayOfGundanList, sizeof(iArrayOfGundanList) );

	// フィルターオプションを上書き
	iFilterOptionOfSelectGundanDialog = iFilterOption;

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
	FuncSelectGundanDialogAsm();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

	iReturnOfSelectGundanDialog = iReturnOfSelectGundanDialog & 0xFFFF; // EAXのうち、AXの部分のみ。
	if ( iReturnOfSelectGundanDialog == 0xFF || iReturnOfSelectGundanDialog == 0xFFFF ) { // 選択しなかった
		return 0xFFFF;
	} else {
		// 何番目を選択したか？ ではなくて、対象の軍団番号【配列用】を直接返す。
		return iReturnOfSelectGundanDialog-1;
	}
}