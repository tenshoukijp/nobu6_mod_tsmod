#include "WinTarget.h"

#include "GameDataStruct.h"

/*
00403EAE   . 56             PUSH ESI			   ← これが武将番号
00403EAF   . E8 AE7A0000    CALL TENSHOU.0040B962　← これで、死亡処理＋必要なＵＩ系処理も行われる。
00403EB4   83C4 04          ADD     ESP, 4

*/

int pCallFromFuncDoDeathInMain = 0x403EAF; // 元々TENSHOU.EXE内にあったCALL先

int iBushouIDInFuncDoDeathInMain = 1L;    // 死亡武将ID

// メイン画面で指定の武将を殺害
void FuncDoDeathInMainAsm() {
	__asm {
		mov esi, iBushouIDInFuncDoDeathInMain
		push iBushouIDInFuncDoDeathInMain
		call pCallFromFuncDoDeathInMain
		add esp, 0x4
	}
}


// メイン画面で指定の武将を殺害
void FuncDoDeathInMain(int iBushouID) {

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		// 関数用のやつは、1つ多いので、足しこみ
		iBushouIDInFuncDoDeathInMain = iBushouID+1;
	
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
		FuncDoDeathInMainAsm();

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
