#include "WinTarget.h"

#include "GameDataStruct.h"

/*
00436101  |> 56             PUSH    ESI						← 天気番号
00436102  |. E8 29020600    CALL    TENSHOU.00496330		← 天気を変更して、描画も更新の関数
00436107  |. 83C4 04        ADD     ESP, 4
*/

int pCallFromFuncSetWeather = 0x496330; // 元々TENSHOU.EXE内にあったCALL先

int iWeatherIDInFuncSetWeather = 1L;    // 死亡武将ID

// 天気を即座に変更(ただし、天候情報は更新されない。演出用)
void FuncSetWeatherAsm() {
	__asm {
		push iWeatherIDInFuncSetWeather
		call pCallFromFuncSetWeather
		add esp, 0x4
	}
}


// 天気を即座に変更(ただし、天候情報は更新されない。演出用)
void FuncSetWeather(int iWeatherID) {

	// 関数用のやつは、1つ多いので、足しこみ
	iWeatherIDInFuncSetWeather = iWeatherID;

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
	FuncSetWeatherAsm();

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
