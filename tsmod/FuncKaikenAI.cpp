#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"


//---------------------会見のＡＩを得る-------------------------
int pCallFromFuncGetKaikenAIAsm = 0x47028B; // 元々TENSHOU.EXE内にあったCALL先

int iParamGetKaikenAIBushouID = -1;
int iReturnGetKaikenAIID = -1;

void FuncGetKaikenAIAsm() {

	__asm {
		// 引数1～3 2バイトずつ
		push iParamGetKaikenAIBushouID;
		// 引数4 文字列へのポインタ(ポインタ=4バイト)
		call pCallFromFuncGetKaikenAIAsm

		mov iReturnGetKaikenAIID, EAX // EAXが対象武将のAI
		// スタックがずれ込んだ分
		add  esp, 4
	}

}

// 対象武将の会見意向ＡＩを得る。
int getKaikenIkouAI(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		iParamGetKaikenAIBushouID = iBushouID + 1;

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
		FuncGetKaikenAIAsm();

		__asm {
			pop edi
			pop esi
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax
		}

		return iReturnGetKaikenAIID;
	} else {
		return 0; // 武将がイレギュラーなら、静観相当を返しておく。
	}
}

int WINAPI Extern_getKaikenIkouAI(int iBushouID) {
	return getKaikenIkouAI(iBushouID);
}

// 対象武将の会見意向ＡＩを設定する。
extern int iAddressOfGetPointerKaikenHeapMemoryExecute;
void setKaikenIkouAI(int iBushouID, int iAI) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		getKaikenIkouAI(iBushouID); // 一度空実行してヒープメモリの位置(iAddressOfGetPointerKaikenHeapMemoryExecute)を確定させる。

		NB6KAIKENIKOU *nb6kaiken = (NB6KAIKENIKOU *)iAddressOfGetPointerKaikenHeapMemoryExecute;
		nb6kaiken->ai = iAI;
	}
}

void WINAPI Extern_setKaikenIkouAI(int iBushouID, int iAI) {
	setKaikenIkouAI(iBushouID, iAI);
}