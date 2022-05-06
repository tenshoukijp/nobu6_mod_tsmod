#include "WinTarget.h"

#include "GameDataStruct.h"

/*
00457D90  |. 83C4 04        ADD     ESP, 4
00457D93     E8 56B5FCFF    CALL    TENSHOU.004232EE                    ←フェイドアウト

00457D98     6A 01          PUSH    1
00457D9A     B9 18C24D00    MOV     ECX, TENSHOU.004DC218
00457D9F  |. 6A 01          PUSH    1                                ; |Arg2 = 00000001
00457DA1  |. 56             PUSH    ESI                              ; |Arg1
00457DA2  |. E8 E8F40200    CALL    TENSHOU.0048728F                 ; \TENSHOU.0048728F　　　カメラ移動

00457DA7     E8 28B5FCFF    CALL    TENSHOU.004232D4　　　　　　　　　　←フェイドイン
*/

int pCallFromFuncMoveCamera = 0x48728F; // 元々TENSHOU.EXE内にあったCALL先

WORD iCastleIdInFuncMoveCamera = 1L;

// メイン画面でのカメラ移動
void FuncMoveCameraAsm() {
	__asm {
		push ecx  // ecx の値を保存

		// 引数1～3 2バイトずつ
		mov ecx, 0x4DC218  // (中身は2710=10000か。なにやら必要らしい);
		push 1
		push 1
		push dword ptr iCastleIdInFuncMoveCamera;
		call pCallFromFuncMoveCamera

		pop ecx  // ecxの値を元へと戻す
	}
}


// カメラを指定の城IDへと移動
void FuncMoveCamera(int iCastleID) {

	if ( 0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM ) {
		// 関数用のやつは、1つ多いので、足しこみ
		iCastleIdInFuncMoveCamera = (WORD)iCastleID+1;
	
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
		FuncMoveCameraAsm();

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
}
