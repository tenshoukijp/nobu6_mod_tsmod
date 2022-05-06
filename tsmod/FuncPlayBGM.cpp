#include "WinTarget.h"

#include "FuncPlayBGM.h"
#include "GameDataStruct.h"

/*
ＢＧＭを再生する例
0042BA2A   . 6A 08          PUSH    8
0042BA2C   . E8 B8190600    CALL    TENSHOU.0048D3E9
0042BA31   . 83C4 08        ADD     ESP, 8

*/


//---------------------ＢＧＭ停止-------------------------
int pCallFromFuncStopBGM = 0x48D483; // 元々TENSHOU.EXE内にあったCALL先

void FuncStopBGMAsm() {

	__asm {
		call pCallFromFuncStopBGM
	}
}

// 指定のＢＧＭを停止する
void FuncStopBGM() {

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
	FuncStopBGMAsm();

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



//---------------------ＢＧＭ再生-------------------------
int pCallFromFuncPlayBGM = 0x48D3E9; // 元々TENSHOU.EXE内にあったCALL先

int iBgmIdFuncPlayBGM = -1;

void FuncPlayBGMAsm() {

	__asm {
		push 0x1  // 何なのかはわからん…

		push iBgmIdFuncPlayBGM

		call pCallFromFuncPlayBGM

		add esp, 8
	}

}

// 指定のＢＧＭを再生する
void FuncPlayBGM(int iBGM) {
	// TSModや神パッチベースの番号→Tenshouki内部番号に直す。
	if ( 1 <= iBGM && iBGM < 0xFFFF ) {
		iBGM--;
	}
	iBgmIdFuncPlayBGM = iBGM;

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
	FuncPlayBGMAsm();

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


// ＢＧＭを再生中か？
int isPlayBGM() {
	WORD *pBGMID = (WORD *)GAMEDATASTRUCT_PLAYING_BGM_ID_ADDRESS;
	return 1 <= *pBGMID && *pBGMID != 0xFFFF;
}

// 再生中のBGMのIDを得る。0xFFFFだと再生中ではない。
int getPlayingBGM() {
	WORD *pBGMID = (WORD *)GAMEDATASTRUCT_PLAYING_BGM_ID_ADDRESS;
	// tenshouki内部番号→神パッチ番号に直す
	if ( 1 <= *pBGMID && *pBGMID != 0xFFFF ) {
		return (*pBGMID)+1;
	} else {
		return 0xFFFF;
	}
}
