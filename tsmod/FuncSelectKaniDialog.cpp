#define _CRT_SECURE_NO_WARNINGS

#include "GameDataStruct.h"
#include "FuncSelectKaniDIalog.h"

/*
00401B0A  |> 53             /PUSH EBX                                ; /Arg5 = 0
00401B0B  |. 53             |PUSH EBX                                ; |Arg4 = 0
00401B0C  |. 53             |PUSH EBX                                ; |Arg3 = 0
00401B0D  |. 6A 0F          |PUSH 0F                                 ; |Arg2 = 0000000F ← 選択オプション
00401B0F  |. 68 00B14D00    |PUSH TENSHOU.004DB100                   ; |Arg1 = 004DB100 ← 配列格納の場所
00401B14  |. E8 09750000    |CALL TENSHOU.00409022                   ; \TENSHOU.00409022
00401B19  |. 83C4 14        |ADD ESP,14
00401B1C  |. 8BF8           |MOV EDI,EAX
*/

//---------------------選択官位ダイアログ-------------------------
int pCallFromFuncSelectKaniDialog = 0x409022; // 元々TENSHOU.EXE内にあったCALL先
int iReturnOfSelectKaniDialog = 0; // 返り値。AXの部分のみ採用する。
int iArrayOfKaniList[GAMEDATASTRUCT_KANI_NUM+1] = {}; // 表示する対象となる官位リスト番号。最後は0xFFFFで終わりとみなす。
int iFilterOptionOfSelectKaniDialog = Filter::Kani::デフォルト;
void FuncSelectKaniDialogAsm() {

	__asm {
		push 0
		push 0
		push 0 // 選択可能数(但し官位を複数選択する要件はないはず)
		push iFilterOptionOfSelectKaniDialog   // 表示項目オプション。0x3とかいうのもある。
		push 0x4DB100 // WORDで官位の番号の配列を格納してる場所。最後0xFFFF

		call pCallFromFuncSelectKaniDialog

		mov iReturnOfSelectKaniDialog, eax

		add  esp, 0x14
	}
}

// 対象となる官位リスト。
int FuncSelectKaniDialog(vector<int> list, int iFilterOption) {

	// リストが無いならなにもしない。
	if ( list.size() == 0 ) {
		return 0xFFFF;
	}

	// 渡されたvector形式のリストをWORD形式の配列へのコピーしていく。
	int *pCopy = iArrayOfKaniList;
	// 官位のリストを配列形式にする。
	for ( vector<int>::iterator it=list.begin(); it != list.end(); it++ ) {

		int iKaniID = *it;
		// 範囲外は無視
		if ( ! (0 <= iKaniID && iKaniID < GAMEDATASTRUCT_KANI_NUM) ) {
			continue;
		}
		
		*pCopy = iKaniID+1; // 配列用→番号にして配列に格納
		pCopy++;
	}
	
	// コピー委ポインタが１つも進んでいない。==１つも有効な官位が１つもなかった
	if ( pCopy == iArrayOfKaniList ) {
		return 0xFFFF;
	}

	*pCopy = 0xFFFF; // 最後の部分に0xFFFFを足す。

	// 0x4DB100 // intで官位の番号の配列を格納してる場所。
	// 官位リストの内容をtenshou.exeメモリへと複製展開。
	memcpy( (void *)0x4DB100, iArrayOfKaniList, sizeof(iArrayOfKaniList) );

	// フィルターオプションを上書き
	iFilterOptionOfSelectKaniDialog = iFilterOption;

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
	FuncSelectKaniDialogAsm();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

	iReturnOfSelectKaniDialog = iReturnOfSelectKaniDialog & 0xFFFF; // EAXのうち、AXの部分のみ。
	if ( iReturnOfSelectKaniDialog == 0xFF ||iReturnOfSelectKaniDialog == 0xFFFF ) { // 選択しなかった
		return 0xFFFF;
	} else {
		// 何番目を選択したか？ ではなくて、対象の官位番号【配列用】を直接返す。
		return iReturnOfSelectKaniDialog-1;
	}
}