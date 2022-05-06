#define _CRT_SECURE_NO_WARNINGS

#include "GameDataStruct.h"
#include "FuncSelectCastleDialog.h"
#include "CommonGlobalHandle.h"
/*
城の引数


0018FE14   004D8F50  |Arg1 = 004D8F50			武将の格納場所
0018FE18   00000001  |Arg2 = 00000001
0018FE1C   0033D57B  |Arg3 = 0033D57B			カラムオプション
0018FE20   00000000  |Arg4 = 00000000
0018FE24   0000FFFF  |Arg5 = 0000FFFF
0018FE28   0000FFFF  |Arg6 = 0000FFFF
0018FE2C   00000000  |Arg7 = 00000000
0018FE30   00000000  \Arg8 = 00000000

城の単発用
0045040D  |> 6A 00          /PUSH 0                                  ; /Arg7 = 00000000
0045040F  |. 6A 00          |PUSH 0                                  ; |Arg6 = 00000000
00450411  |. 53             |PUSH EBX                                ; |Arg5
00450412  |. 53             |PUSH EBX                                ; |Arg4
00450413  |. 57             |PUSH EDI                                ; |Arg3
00450414  |. 68 7BD53300    |PUSH 33D57B                             ; |Arg2 = 0033D57B		カラムオプション
00450419  |. 68 508F4D00    |PUSH TENSHOU.004D8F50                   ; |Arg1 = 004D8F50		ここ固定。リストの武将の格納場所
0045041E  |. E8 B8E10200    |CALL TENSHOU.0047E5DB                   ; \TENSHOU.0047E5DB
00450423  |. 66:8BF0        |MOV SI,AX
00450426  |. 83C4 1C        |ADD     ESP, 1C


城の単発用
呼出し先。城選択は１しかない模様。
0047E5DB  /$ 55             PUSH EBP
0047E5DC  |. 8BEC           MOV EBP,ESP
0047E5DE  |. FF7424 20      PUSH DWORD PTR SS:[ESP+20]               ; /Arg8
0047E5E2  |. FF75 1C        PUSH DWORD PTR SS:[EBP+1C]               ; |Arg7
0047E5E5  |. FF75 18        PUSH DWORD PTR SS:[EBP+18]               ; |Arg6
0047E5E8  |. FF75 14        PUSH DWORD PTR SS:[EBP+14]               ; |Arg5
0047E5EB  |. FF75 10        PUSH DWORD PTR SS:[EBP+10]               ; |Arg4
0047E5EE  |. FF75 0C        PUSH DWORD PTR SS:[EBP+C]                ; |Arg3
0047E5F1  |. 6A 01          PUSH 1                                   ; |Arg2 = 00000001
0047E5F3  |. FF75 08        PUSH DWORD PTR SS:[EBP+8]                ; |Arg1
0047E5F6  |. E8 6BFEFFFF    CALL TENSHOU.0047E466                    ; \TENSHOU.0047E466
0047E5FB  |. 83C4 20        ADD ESP,20
0047E5FE  |. 83F8 01        CMP EAX,1
0047E601  |. 66:B8 FFFF     MOV AX,0FFFF
0047E605  |. 75 0A          JNZ SHORT TENSHOU.0047E611
0047E607  |. 6A 00          PUSH 0
0047E609  |. FF75 08        PUSH DWORD PTR SS:[EBP+8]			
0047E60C  |. E8 6408F9FF    CALL TENSHOU.0040EE75
0047E611  |> 8BE5           MOV ESP,EBP
0047E613  |. 5D             POP EBP
0047E614  \. C3             RETN
*/


//---------------------選択城ダイアログ-------------------------
int pCallFromFuncSelectCastleDialog = 0x47E5DB; // 元々TENSHOU.EXE内にあったCALL先
int iReturnOfSelectCastleDialog = 0; // 返り値。AXの部分のみ採用する。
WORD iArrayOfCastleList[GAMEDATASTRUCT_CASTLE_NUM+1] = {}; // 表示する対象となる城リスト番号。最後は0xFFFFで終わりとみなす。
int iFilterOptionOfSelectCastleDialog = Filter::Castle::デフォルト;
static int iMultiCastleNumOfSelectCastleDialog = 1; // 選択可能な城の最大数。

void FuncSelectCastleDialogAsm() {

	__asm {
		push 0 
		push 0 
		push 0xFFFF
		push 0xFFFF
		push 0 
		push iFilterOptionOfSelectCastleDialog // フィルター値
		push 0x4D8F50 // WORDで城の番号の配列を格納してる場所。最後0xFFFF

		call pCallFromFuncSelectCastleDialog

		mov iReturnOfSelectCastleDialog, eax

		add  esp, 0x1C
	}
}

// 対象となる城リスト。
int FuncSelectCastleDialog(vector<int> list, int iFilterOption ) {

	// リストが無いならなにもしない。
	if ( list.size() == 0 ) {
		return 0xFFFF;
	}

	// 渡されたvector形式のリストをWORD形式の配列へのコピーしていく。
	WORD *pCopy = iArrayOfCastleList;
	// 城のリストを配列形式にする。
	for ( vector<int>::iterator it=list.begin(); it != list.end(); it++ ) {

		int iCastleID = *it;
		// 範囲外は無視
		if ( ! (0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM) ) {
			continue;
		}
		
		*pCopy = (WORD)iCastleID+1; // 配列用→番号にして配列に格納
		pCopy++;
	}
	
	// コピー委ポインタが１つも進んでいない。==１つも有効な城が居なかった
	if ( pCopy == iArrayOfCastleList ) {
		return 0xFFFF;
	}

	*pCopy = 0xFFFF; // 最後の部分に0xFFFFを足す。

	// 0x4D8F50 // WORDで城の番号の配列を格納してる場所。
	// 城リストの内容をtenshou.exeメモリへと複製展開。
	memcpy( (void *)0x4D8F50, iArrayOfCastleList, sizeof(iArrayOfCastleList) );

	// フィルターオプションを上書き
	iFilterOptionOfSelectCastleDialog = iFilterOption;

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
	FuncSelectCastleDialogAsm();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

	iReturnOfSelectCastleDialog = iReturnOfSelectCastleDialog & 0xFFFF; // EAXのうち、AXの部分のみ。
	if ( iReturnOfSelectCastleDialog == 0xFF || iReturnOfSelectCastleDialog == 0xFFFF ) { // 選択しなかった
		return 0xFFFF;
	} else {
		// 何番目を選択したか？ ではなくて、対象の城番号【配列用】を直接返す。
		return iReturnOfSelectCastleDialog-1;
	}
}