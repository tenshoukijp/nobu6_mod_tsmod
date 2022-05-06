#define _CRT_SECURE_NO_WARNINGS

#include "GameDataStruct.h"
#include "FuncSelectKahouDIalog.h"

/*
00446673  |> 6A 00          /PUSH    0                               ; /Arg5 = 00000000
00446675  |. 6A 00          |PUSH    0                               ; |Arg4 = 00000000
00446677  |. 57             |PUSH    EDI                             ; |Arg3
00446678  |. 6A 1F          |PUSH    1F                              ; |Arg2 = 0000001F
0044667A  |. 68 50C84D00    |PUSH    TENSHOU.004DC850                ; |Arg1 = 004DC850
0044667F  |. E8 0FD70300    |CALL    TENSHOU.00483D93                ; \TENSHOU.00483D93
00446684  |. 83C4 14        |ADD     ESP, 14
00446687  |. 8BF0           |MOV     ESI, EAX

*/

//---------------------選択家宝ダイアログ-------------------------
int pCallFromFuncSelectKahouDialog = 0x483D93; // 元々TENSHOU.EXE内にあったCALL先
int iReturnOfSelectKahouDialog = 0; // 返り値。AXの部分のみ採用する。
int iArrayOfKahouList[GAMEDATASTRUCT_KAHOU_NUM+1] = {}; // 表示する対象となる家宝リスト番号。最後は0xFFFFで終わりとみなす。
int iFilterOptionOfSelectKahouDialog = Filter::Kahou::デフォルト;
void FuncSelectKahouDialogAsm() {

	__asm {
		push 0
		push 0
		push 1 // 選択可能数(但し家宝を複数選択する要件はないはず)
		push iFilterOptionOfSelectKahouDialog   // 表示項目オプション。0x3とかいうのもある。
		push 0x4DC850 // WORDで家宝の番号の配列を格納してる場所。最後0xFFFF

		call pCallFromFuncSelectKahouDialog

		mov iReturnOfSelectKahouDialog, eax

		add  esp, 0x14
	}
}

// 対象となる家宝リスト。
int FuncSelectKahouDialog(vector<int> list, int iFilterOption) {

	// リストが無いならなにもしない。
	if ( list.size() == 0 ) {
		return 0xFFFF;
	}

	// 渡されたvector形式のリストをWORD形式の配列へのコピーしていく。
	int *pCopy = iArrayOfKahouList;
	// 家宝のリストを配列形式にする。
	for ( vector<int>::iterator it=list.begin(); it != list.end(); it++ ) {

		int iKahouID = *it;
		// 範囲外は無視
		if ( ! (0 <= iKahouID && iKahouID < GAMEDATASTRUCT_KAHOU_NUM) ) {
			continue;
		}
		
		*pCopy = iKahouID+1; // 配列用→番号にして配列に格納
		pCopy++;
	}
	
	// コピー委ポインタが１つも進んでいない。==１つも有効な家宝が１つもなかった
	if ( pCopy == iArrayOfKahouList ) {
		return 0xFFFF;
	}

	*pCopy = 0xFFFF; // 最後の部分に0xFFFFを足す。

	// 0x4DC850 // intで家宝の番号の配列を格納してる場所。
	// 家宝リストの内容をtenshou.exeメモリへと複製展開。
	memcpy( (void *)0x4DC850, iArrayOfKahouList, sizeof(iArrayOfKahouList) );

	// フィルターオプションを上書き
	iFilterOptionOfSelectKahouDialog = iFilterOption;

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
	FuncSelectKahouDialogAsm();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

	iReturnOfSelectKahouDialog = iReturnOfSelectKahouDialog & 0xFFFF; // EAXのうち、AXの部分のみ。
	if ( iReturnOfSelectKahouDialog == 0xFF ||iReturnOfSelectKahouDialog == 0xFFFF ) { // 選択しなかった
		return 0xFFFF;
	} else {
		// 何番目を選択したか？ ではなくて、対象の家宝番号【配列用】を直接返す。
		return iReturnOfSelectKahouDialog-1;
	}
}