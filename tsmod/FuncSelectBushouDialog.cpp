#define _CRT_SECURE_NO_WARNINGS

#include "GameDataStruct.h"
#include "FuncSelectBushouDialog.h"
/*
0040199E  |. 6A 00          PUSH 0                                   ; /Arg9 = 00000000
004019A0  |. 6A 00          PUSH 0                                   ; |Arg8 = 00000000
004019A2  |. 57             PUSH EDI                                 ; |Arg7 = 0xFFFF
004019A3  |. 6A 0A          PUSH 0A                                  ; |Arg6 = 0000000A
004019A5  |. 68 80100000    PUSH 1080                                ; |Arg5 = 00001080
004019AA  |. 6A 00          PUSH 0                                   ; |Arg4 = 00000000
004019AC  |. 68 103C0900    PUSH 93C10                               ; |Arg3 = 00093C10 (表示選択オプション。3FC93F9Dだと全部.pythonのbinで調査か。)
004019B1  |. FF75 FC        PUSH DWORD PTR SS:[EBP-4]                ; |Arg2 = 5 ( 選択可能人数 )
004019B4  |. FF76 70        PUSH DWORD PTR DS:[ESI+70]               ; |Arg1 = 0x4DBD40(固定)
004019B7  |. E8 797C0300    CALL TENSHOU.00439635                    ; \TENSHOU.00439635
004019BC  |. 83C4 24        ADD ESP,24
004019BF  |. 8BD8           MOV EBX,EAX


004397B3  /$ 55             PUSH EBP
004397B4  |. 8BEC           MOV EBP,ESP
004397B6  |. FF7424 24      PUSH DWORD PTR SS:[ESP+24]               ; /Arg9
004397BA  |. FF75 20        PUSH DWORD PTR SS:[EBP+20]               ; |Arg8
004397BD  |. FF75 1C        PUSH DWORD PTR SS:[EBP+1C]               ; |Arg7
004397C0  |. FF75 18        PUSH DWORD PTR SS:[EBP+18]               ; |Arg6
004397C3  |. FF75 14        PUSH DWORD PTR SS:[EBP+14]               ; |Arg5
004397C6  |. FF75 10        PUSH DWORD PTR SS:[EBP+10]               ; |Arg4
004397C9  |. FF75 0C        PUSH DWORD PTR SS:[EBP+C]                ; |Arg3	
004397CC  |. 6A 01          PUSH 1                                   ; |Arg2 = 00000001
004397CE  |. FF75 08        PUSH DWORD PTR SS:[EBP+8]                ; |Arg1
004397D1  |. E8 5FFEFFFF    CALL TENSHOU.00439635                    ; \TENSHOU.00439635
004397D6  |. 83C4 24        ADD ESP,24
004397D9  |. 83F8 01        CMP EAX,1
004397DC  |. 66:B8 FFFF     MOV AX,0FFFF
004397E0  |. 75 0A          JNZ SHORT TENSHOU.004397EC
004397E2  |. 6A 00          PUSH 0
004397E4  |. FF75 08        PUSH DWORD PTR SS:[EBP+8]				0x4DBD40(固定)
004397E7  |. E8 724EFDFF    CALL TENSHOU.0040E65E					この関数の結果は選択武将のIDとなる。
004397EC  |> 8BE5           MOV ESP,EBP
004397EE  |. 5D             POP EBP
004397EF  \. C3             RETN

0018FE0C   004DBD40  |Arg1 = 004DBD40
0018FE10   00000001  |Arg2 = 00000001
0018FE14   3FC93F9D  |Arg3 = 3FC93F9D
0018FE18   00000000  |Arg4 = 00000000
0018FE1C   00000000  |Arg5 = 00000000
0018FE20   0000FFFF  |Arg6 = 0000FFFF
0018FE24   0000FFFF  |Arg7 = 0000FFFF
0018FE28   00000000  |Arg8 = 00000000
0018FE2C   00000000  \Arg9 = 00000000
*/



//---------------------選択武将ダイアログ-------------------------
static int pCallFromFuncSelectBushouDialog = 0x439635; // 元々TENSHOU.EXE内にあったCALL先
static int pCallFromFuncSelectBushouDialog2 = 0x40E65E; // 元々TENSHOU.EXE内にあったCALL先
static int iReturnOfSelectBushouDialog = 0; // 返り値。AXの部分のみ採用する。
static WORD iArrayOfBushouList[GAMEDATASTRUCT_BUSHOU_NUM+1] = {}; // 表示する対象となる武将リスト番号。最後は0xFFFFで終わりとみなす。
static int iFilterOptionOfSelectBushouDialog = Filter::Bushou::デフォルト;
static int iFilterExOptionOfSelectBushouDialog = 0; // こちらは33bit目以降のオプション。

void FuncSelectBushouDialogAsm() {

	__asm {
		push 0
		push 0
		push 0xFFFF
		push 0xFFFF
		push 0
		push iFilterExOptionOfSelectBushouDialog // 33bit目以降の拡張オプション
		push iFilterOptionOfSelectBushouDialog // フィルター値
		push 1 // 選択可能数(但し武将を複数選択する要件はないはず)
		push 0x4DBD40 // WORDで武将の番号の配列を格納してる場所。最後0xFFFF

		call pCallFromFuncSelectBushouDialog
		add  esp, 0x24
		cmp eax, 1	// この段階でのeaxはちゃんと選んだかどうかのみ。
		mov ax, 0xFFFF
//		jnz labelEndOfFuncSelectBushouDialogAsm

		push 0
		push 0x4DBD40
		call pCallFromFuncSelectBushouDialog2
		mov iReturnOfSelectBushouDialog, eax // これが実際の選択武将番号
		add  esp, 0x8

//labelEndOfFuncSelectBushouDialogAsm:
//		;
	}
}



// 対象となる武将リスト。
int FuncSelectBushouDialog(vector<int> list, __int64 iFilterOption) {

	// リストが無いならなにもしない。
	if ( list.size() == 0 ) {
		return 0xFFFF;
	}

	// 返り値はデフォルトは「何も選択しなかった」
	iReturnOfSelectBushouDialog = 0xFFFF;

	// 渡されたvector形式のリストをWORD形式の配列へのコピーしていく。
	WORD *pCopy = iArrayOfBushouList;
	// 武将のリストを配列形式にする。
	for ( vector<int>::iterator it=list.begin(); it != list.end(); it++ ) {

		int iBushouID = *it;
		// 範囲外は無視
		if ( ! (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) ) {
			continue;
		}
		
		*pCopy = (WORD)iBushouID+1; // 配列用→番号にして配列に格納
		pCopy++;
	}
	
	// コピー委ポインタが１つも進んでいない。==１つも有効な武将が居なかった
	if ( pCopy == iArrayOfBushouList ) {
		return 0xFFFF;
	}

	*pCopy = 0xFFFF; // 最後の部分に0xFFFFを足す。

	// 0x4DBD40 // WORDで武将の番号の配列を格納してる場所。
	// 武将リストの内容をtenshou.exeメモリへと複製展開。
	memcpy( (void *)0x4DBD40, iArrayOfBushouList, sizeof(iArrayOfBushouList) );

	// フィルターオプションを上書き
	iFilterOptionOfSelectBushouDialog = iFilterOption & 0xFFFFFFFF;
	iFilterOption = iFilterOption >> 32; // 上位32bitを下位に持ってくる。(拡張オプションを下位に)
	iFilterExOptionOfSelectBushouDialog = iFilterOption & 0xFFFFFFFF;

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
	FuncSelectBushouDialogAsm();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

	iReturnOfSelectBushouDialog = iReturnOfSelectBushouDialog & 0xFFFF; // EAXのうち、AXの部分のみ。
	if ( iReturnOfSelectBushouDialog == 0xFFFF ) { // 選択しなかった
		return 0xFFFF;
	} else {
		// 何番目を選択したか？ ではなくて、対象の武将番号【配列用】を直接返す。
		return int(iReturnOfSelectBushouDialog)-1;
	}
}