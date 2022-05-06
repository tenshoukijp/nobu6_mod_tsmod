#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "FuncSelectMenuDialog.h"
#include "OutputDebugStream.h"


/*
00419AF4  |. 33DB           XOR     EBX, EBX
00419AF6  |. BF FFFF0000    MOV     EDI, 0FFFF
00419AFB  |> 53             /PUSH    EBX                             ; /Arg7				← 0 固定
00419AFC  |. 6A 70          |PUSH    70                              ; |Arg6 = 00000070		？
00419AFE  |. 53             |PUSH    EBX                             ; |Arg5
00419AFF  |. 68 A0B64C00    |PUSH    TENSHOU.004CB6A0                ; |Arg4 = 004CB6A0		文字列ポインタの配列のアドレス
00419B04  |. 6A 03          |PUSH    3                               ; |Arg3 = 00000003		選択肢の数
00419B06     68 A0000000    PUSH    0A0														Ｙ座標
00419B0B     68 18020000    PUSH    118														Ｘ座標
00419B10  |. E8 6C920800    |CALL    TENSHOU.004A2D81                ; \TENSHOU.004A2D81
00419B15  |. 83C4 1C        |ADD     ESP, 1C


RETURNであるEAXは選択肢順に0, 1, 2,...
選択しなかった場合(右クリックなど) 0xFFFF がEAXに入る。
*/

// メニューの数の最大
#define SELECT_MENU_NUM_MAX  36

// １つのメニューの中の文字列の最大は全角３１文字。半角63
#define SELECT_MENU_STRING_LENGTH_MAX	64

char *pSelectMenuStringArrayPointer[SELECT_MENU_NUM_MAX] = {NULL}; // メニューは最大で３６個。こちらは格メニューも自列へのポインタ配列
// 家宝種類のカスタム文字列

// １つのセレクトメニュー型
struct SELECT_MENU {
	char szKahouTypeString[SELECT_MENU_STRING_LENGTH_MAX]; // 全角３１文字。
};
SELECT_MENU select_menu_string[SELECT_MENU_NUM_MAX] = {""}; // 各々の文字列メニュー


//---------------------二者選択ダイアログ-------------------------
int pCallFromFuncSelectMenuDialogAsm = 0x4A2D81; // 元々TENSHOU.EXE内にあったCALL先
int iResultOfFuncSelectMenuDialog = 0;	// 選択結果
int iXPosOfSelectMenuDialog = 0xA0;		// Ｘ座標 
int iYPosOfSelectMenuDialog = 0x118;	// Ｙ座標
int iMenuNumOfSelectMenuDialog = 3;		// メニューの個数
int iMenuMinWidthOfSelectMenuDialog = 0x70; // メニュの最低幅

int iSelectMenuStringArrayPointerOfSelectMenuDialog = 0x4CB6A0; // デフォルトは披露の配列

void FuncSelectMenuDialogAsm() {
	__asm {
		push 0xF
		push iMenuMinWidthOfSelectMenuDialog // おそらくメニューの最低幅
		push 0
		push iSelectMenuStringArrayPointerOfSelectMenuDialog
		push iMenuNumOfSelectMenuDialog
		push iYPosOfSelectMenuDialog
		push iXPosOfSelectMenuDialog

		call pCallFromFuncSelectMenuDialogAsm

		// 選択結果の番号がeaxに入る
		mov iResultOfFuncSelectMenuDialog, eax

		// pushした分をなしにする
		pop eax
		pop eax
		pop eax
		pop eax
		pop eax
		pop eax
		pop eax

		mov eax, iResultOfFuncSelectMenuDialog;
	}
}


bool isInitSelectMenuPointer = false; // 未初期化フラグ
void InitSelectMenuPointer() {
	if ( !isInitSelectMenuPointer ) {
		// 最初に各々の文字列のポインタは指しておく
		for ( int i=0; i<SELECT_MENU_NUM_MAX; i++ ) {
			pSelectMenuStringArrayPointer[i] = select_menu_string[i].szKahouTypeString;
		}

		// ポインタの配列の先頭のアドレスをint型として控えておく。メニューダイアログにはこの値を渡す必要がある。
		iSelectMenuStringArrayPointerOfSelectMenuDialog = (int)pSelectMenuStringArrayPointer;

		isInitSelectMenuPointer = true;
	}
}

int SelectMenuDialog(vector<string> menu_list, int posX, int posY, int MinMenuWidth) {
	// メニューの個数が最大数を超えていれば、
	if ( menu_list.size() > SELECT_MENU_NUM_MAX ) {
		return 0xFFFF; // 何もせず閉じたのと同じこととする。
	}
	// メニュー自体１つも存在しない。
	if ( menu_list.size() == 0 ) {
		return 0xFFFF; // 何もせず閉じたのと同じこととする。
	}

	InitSelectMenuPointer();

	iMenuMinWidthOfSelectMenuDialog = MinMenuWidth;

	// メニューの個数を控えておく
	iMenuNumOfSelectMenuDialog = menu_list.size();
	// メニュー数が、最大数をオーバーしていたら、最大数に訂正
	if ( iMenuNumOfSelectMenuDialog > SELECT_MENU_NUM_MAX ) { iMenuNumOfSelectMenuDialog = SELECT_MENU_NUM_MAX; }

	// 一番長い文字を探す。
	int max_string_length = iMenuMinWidthOfSelectMenuDialog/8; // ダイアログの最低幅を半角文字の幅で割った値

	// 各メニューの文字列を、グローバル変数にコピーしていく
	for ( int i=0; i < iMenuNumOfSelectMenuDialog ; i++ ) {

		strncpy( select_menu_string[i].szKahouTypeString, menu_list[i].c_str(), SELECT_MENU_STRING_LENGTH_MAX);

		// 一番最後をNULLにして安全弁
		select_menu_string[i].szKahouTypeString[SELECT_MENU_STRING_LENGTH_MAX-1] = NULL;

		// 一番長い文字更新チェック
		int cur_len = strlen( menu_list[i].c_str() );
		if ( max_string_length < cur_len ) {
			max_string_length = cur_len;
		}

	}

	RECT rc; // クライアント領域のサイズ
	::GetClientRect(CommonGlobalHandle::tenshouWND, &rc);

	int width = rc.right-rc.left;
	int height = rc.bottom-rc.top;

	// 指定されていない → 中央に
	if ( posX == -1 ) {
		int left = width/2 - 4*(max_string_length); // 中央になるように左を計算。（最大の文字列数）＊８／２のピクセル分 真ん中より左に配置 
		iXPosOfSelectMenuDialog = left;
	} else {
		iXPosOfSelectMenuDialog = posX;
	}

	// 指定されていない → 中央に
	if ( posY == -1 ) {
		int top = height/2 - 8*(iMenuNumOfSelectMenuDialog+1); // 中央になるようにトップを計算。（メニュー数＋１）＊１６／２のピクセル分 真ん中より上に配置 
		iYPosOfSelectMenuDialog = top;
	} else {
		iYPosOfSelectMenuDialog = posY;
	}

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
	FuncSelectMenuDialogAsm();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

	// 結果を返す。
	return iResultOfFuncSelectMenuDialog;
}


