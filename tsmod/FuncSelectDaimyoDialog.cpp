#define _CRT_SECURE_NO_WARNINGS

#include "GameDataStruct.h"
#include "FuncSelectDaimyoDialog.h"
/*
0042534D  /$ FF7424 08      PUSH    DWORD PTR SS:[ESP+8]
00425351  |. 8B4C24 08      MOV     ECX, DWORD PTR SS:[ESP+8]
00425355  |. E8 63010500    CALL    TENSHOU.004754BD
0042535A  |. 33C0           XOR     EAX, EAX
0042535C  |. 50             PUSH    EAX
0042535D  |. 50             PUSH    EAX
0042535E  |. 6A 08          PUSH    8
00425360  |. 6A 02          PUSH    2
00425362  |. 50             PUSH    EAX
00425363  |. 68 9F071103    PUSH    311079F
00425368  |. E8 031D0400    CALL    TENSHOU.00467070
0042536D  |. 50             PUSH    EAX                              ; |Arg2
0042536E  |. 68 D8A54D00    PUSH    TENSHOU.004DA5D8                 ; |Arg1 = 004DA5D8
00425373  |. E8 F7D10200    CALL    TENSHOU.0045256F                 ; \TENSHOU.0045256F
00425378  |. 66:A3 FCE54B00 MOV     WORD PTR DS:[4BE5FC], AX
0042537E  |. 83C4 20        ADD     ESP, 20
00425381  \. C3             RETN
*/

/*
00415B93  |> 33C0           XOR     EAX, EAX
00415B95  |. B9 FFFF0000    MOV     ECX, 0FFFF
00415B9A  |. 50             PUSH    EAX
00415B9B  |. 50             PUSH    EAX
00415B9C  |. 51             PUSH    ECX
00415B9D  |. 51             PUSH    ECX
00415B9E  |. 50             PUSH    EAX
00415B9F  |. 68 9F071103    PUSH    311079F
00415BA4  |. E8 C7140500    CALL    TENSHOU.00467070
00415BA9  |. 50             PUSH    EAX                              ; |Arg2
00415BAA  |. 68 D8A54D00    PUSH    TENSHOU.004DA5D8                 ; |Arg1 = 004DA5D8
00415BAF  |. E8 BBC90300    CALL    TENSHOU.0045256F                 ; \TENSHOU.0045256F
00415BB4  |. 66:8BD8        MOV     BX, AX   ← AXが選択大名番号

*/

//---------------------選択大名ダイアログ-------------------------
int pCallFromFuncSelectDaimyoDialog = 0x45256F; // 元々TENSHOU.EXE内にあったCALL先
int pCallFromFuncSelectDaimyoDialogSub = 0x467070; // 元々TENSHOU.EXE内にあったCALL先
int iDaimyoIDOfSelectDaimyoDialog = -1; // 主観となるiDaimyoID。友好値などがあるので、主観大名が必要。
int iReturnOfSelectDaimyoDialog = 0; // 返り値。eaxの部分のみ採用する。
int iFilterOptionOfSelectDaimyoDialog = Filter::Daimyo::デフォルト; // フィルターオプション
WORD iArrayOfDaimyoList[GAMEDATASTRUCT_DAIMYO_NUM+1] = {}; // 表示する対象となる大名リスト番号。最後は0xFFFFで終わりとみなす。

void FuncSelectDaimyoDialogAsm() {

	__asm {
		xor eax, eax
		mov ecx, 0xFFFF
	
		push eax      // 別のやり方 0 
		push eax	  //            0 
		push ecx	  //            8 
		push ecx	  //            2 
		push 0x4001	  //            0 
		push iFilterOptionOfSelectDaimyoDialog

		// call pCallFromFuncSelectDaimyoDialogSub // 自分の大名勢力の番号をeaxにセット
		mov eax, iDaimyoIDOfSelectDaimyoDialog
		push eax
		push 0x4DA5D8 // WORDで大名の番号の配列を格納してる場所。最後0xFFFF

		call pCallFromFuncSelectDaimyoDialog

		mov iReturnOfSelectDaimyoDialog, eax

		add  esp, 0x20
	}
}

// 対象となる大名リスト。
int FuncSelectDaimyoDialog(vector<int> list, int iFilterOption, int iMainDaimyoID ) {

	// リストが無いならなにもしない。
	if ( list.size() == 0 ) {
		return 0xFFFF;
	}

	if ( iMainDaimyoID == -1 ) {
		iMainDaimyoID = nb6turn.daimyo-1;
	}
	if ( !(0 <= iMainDaimyoID && iMainDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM ) ) {
		return 0xFFFF;
	}
	iDaimyoIDOfSelectDaimyoDialog = iMainDaimyoID+1;

	// 渡されたvector形式のリストをWORD形式の配列へのコピーしていく。
	WORD *pCopy = iArrayOfDaimyoList;
	// 大名のリストを配列形式にする。
	for ( vector<int>::iterator it=list.begin(); it != list.end(); it++ ) {

		int iDaimyoID = *it;
		// 範囲外は無視
		if ( ! (0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM) ) {
			continue;
		}
		// 大名が居ない番号は無視
		if ( nb6daimyo[iDaimyoID].attach == 0xFFFF ) {
			continue;
		}
		
		*pCopy = (WORD)iDaimyoID+1; // 配列用→番号にして配列に格納
		pCopy++;
	}
	
	// コピー委ポインタが１つも進んでいない。==１つも有効な大名が居なかった
	if ( pCopy == iArrayOfDaimyoList ) {
		return 0xFFFF;
	}

	*pCopy = 0xFFFF; // 最後の部分に0xFFFFを足す。

	// 0x4DA5D8 // WORDで大名の番号の配列を格納してる場所。
	// 大名リストの内容をtenshou.exeメモリへと複製展開。
	memcpy( (void *)0x4DA5D8, iArrayOfDaimyoList, sizeof(iArrayOfDaimyoList) );

	// フィルターオプションを上書き
	// ★コメントアウト!! tenshou.exe 側でこのフィルターオプションが実装されていない!!
	iFilterOptionOfSelectDaimyoDialog = iFilterOption;

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
	FuncSelectDaimyoDialogAsm();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

	iReturnOfSelectDaimyoDialog = iReturnOfSelectDaimyoDialog & 0xFFFF; // EAXのうち、AXの部分のみ。
	if ( iReturnOfSelectDaimyoDialog == 0xFF || iReturnOfSelectDaimyoDialog == 0xFFFF ) { // 選択しなかった
		return 0xFFFF;
	} else {
		// 何番目を選択したか？ ではなくて、対象の大名番号【配列用】を直接返す。
		return iReturnOfSelectDaimyoDialog-1;
	}
}