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


0042F405  |> 6A 00          PUSH    0                                ; /Arg9 = 00000000
0042F407  |. 6A 00          PUSH    0                                ; |Arg8 = 00000000
0042F409  |. 50             PUSH    EAX                              ; |Arg7 = 0xFFFF
0042F40A  |. 55             PUSH    EBP                              ; |Arg6 = 0000000D
0042F40B  |. 52             PUSH    EDX                              ; |Arg5 = 00000080
0042F40C  |. 51             PUSH    ECX                              ; |Arg4 = 00000000
0042F40D  |. FFB7 C0000000  PUSH    DWORD PTR DS:[EDI+C0]            ; |Arg3 = 00093C10 (表示選択オプション。)
0042F413  |. FFB7 AC000000  PUSH    DWORD PTR DS:[EDI+AC]            ; |Arg2 = 5 ( 選択可能人数 )
0042F419  |. FF33           PUSH    DWORD PTR DS:[EBX]               ; |Arg1
0042F41B  |. E8 15A20000    CALL    TENSHOU.00439635                 ; \TENSHOU.00439635
0042F420  |. 83C4 24        ADD     ESP, 24
0042F423  |. 3D FFFF0000    CMP     EAX, 0FFFF
0042F428  |. 74 12          JE      SHORT TENSHOU.0042F43C
0042F42A  |. FFB7 A8000000  PUSH    DWORD PTR DS:[EDI+A8]		「0x4DDF58で固定」。ここに選択された武将番号がWORD配列でコピーされる。
0042F430  |. FF33           PUSH    DWORD PTR DS:[EBX]
0042F432  |. E8 12000000    CALL    TENSHOU.0042F449
0042F437  |. 83C4 08        ADD     ESP, 8
0042F43A  |. 8906           MOV     DWORD PTR DS:[ESI], EAX
0042F43C  |> B8 01000000    MOV     EAX, 1
0042F441  |. 5D             POP     EBP
0042F442  |. 5F             POP     EDI
0042F443  |. 5E             POP     ESI
0042F444  |. 5B             POP     EBX
0042F445  |. 83C4 08        ADD     ESP, 8


0018FE0C   004DBD40  |Arg1 = 004DBD40
0018FE10   00000001  |Arg2 = 00000005
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
static int pCallFromFuncSelectBushouDialog2 = 0x42F449; // 元々TENSHOU.EXE内にあったCALL先 。複数人数の場合に、指定のポインタに値を複製する。
static int iReturnOfSelectBushouDialog = 0; // 返り値。AXの部分のみ採用する。
static WORD iArrayOfBushouList[GAMEDATASTRUCT_BUSHOU_NUM+1] = {}; // 表示する対象となる武将リスト番号。最後は0xFFFFで終わりとみなす。
static int iFilterOptionOfSelectBushouDialog = Filter::Bushou::デフォルト;
static int iFilterExOptionOfSelectBushouDialog = 0; // こちらは33bit目以降のオプション。
static int iMultiBushouNumOfSelectBushouDialog = 5; // 選択可能な武将の最大人数。
void FuncSelectBushouMultiDialogAsm() {

	__asm {
		push 0
		push 0
		push 0xFFFF
		push 0xFFFF
		push 0
		push iFilterExOptionOfSelectBushouDialog // 33bit目以降の拡張オプション
		push iFilterOptionOfSelectBushouDialog // フィルター値
		push iMultiBushouNumOfSelectBushouDialog // 選択可能数(但し武将を複数選択する要件はないはず)
		push 0x4DBD40 // WORDで武将の番号の配列を格納してる場所。最後0xFFFF

		call pCallFromFuncSelectBushouDialog
		add  esp, 0x24
		cmp eax, 0xFFFF	// この段階でのeaxはちゃんと選んだかどうかのみ。
		je labelEndOfFuncSelectBushouDialogAsm

		push 0x4DDF58	// この場所に選択武将結果がWORD配列でコピーされる。ここにも最後0xFFFFが入る。
		push 0x4DBD40   // ここがリストの場所
		call pCallFromFuncSelectBushouDialog2
		add  esp, 0x8

labelEndOfFuncSelectBushouDialogAsm:
		mov eax, 1
		;
	}
}



// 対象となる武将リスト。
vector<int> FuncSelectBushouMultiDialog(vector<int> list, __int64 iFilterOption, int iSelectableMaxBushouNum ) {

	char *szFill0xFF = "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
	memcpy((char *)0x4DDF58, szFill0xFF, 2*17); // 複数選択結果の武将番号排列が格納される場所。ここにも最後0xFFFFが入る。WORDで17個分FFで埋めておく。


	// 選択可能最大人数がオーバーしてたら直す。
	if ( iSelectableMaxBushouNum > 16 ) { 
		iSelectableMaxBushouNum = 16;
	}
	iMultiBushouNumOfSelectBushouDialog = iSelectableMaxBushouNum;

	vector<int> vResultList;
	// リストが無いならなにもしない。
	if ( list.size() == 0 ) {
		return vResultList;
	}

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
		return vResultList;
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
	FuncSelectBushouMultiDialogAsm();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

	WORD *pResult = (WORD *)0x4DDF58; // 複数選択結果の武将番号排列が格納される場所。ここにも最後0xFFFFが入る。

	// 最大で指定した数までコピー
	for ( int i=0; i < iSelectableMaxBushouNum; i++ ) {
		if ( *pResult == 0xFFFF ) {
			break;
		}
		int iBushouID = (*pResult)-1; // 武将番号→武将番号配列用にして格納
		if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			vResultList.push_back(iBushouID);
		}
		pResult++;
	}

	return vResultList;
}


template<int iSelectableMaxBushouNum>
vector<int> FuncSelectBushouDialog(vector<int> list, __int64 iFilterOption ) {
    return FuncSelectBushouMultiDialog(list, iFilterOption, int iSelectableMaxBushouNum);
}