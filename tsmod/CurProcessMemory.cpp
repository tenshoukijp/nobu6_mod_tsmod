#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"

CurProcessMemory::CurProcessMemory() {
	m_LAdress[LOC1] = (0x4D9B78); // (メッセージの)主体たる｢武将名｣が入りやすい
	m_LAdress[LOC2] = (0x4D9AF8); // ウィンドウメッセージが入りやすい
	m_LAdress[LOC3] = (0x4D91B4);
	m_LAdress[LOC4] = (0x4D97C0); // ｢武将列伝｣や｢家宝名｣や｢家宝詳細｣などが入る場所。左上に出るウィンドウ枠はこちらの場合がある。
	m_LAdress[LOC5] = (0x4D9A40);
	m_LAdress[LOC6] = (0x4D97B7); // 商人との取引時など、２人会話で利用される。
};

void CurProcessMemory::setVariableFromMemory(void) {
	// 具体的なアドレスがセットされてる範囲でメモリを読む
	for ( int iLOCIX=0; iLOCIX < LOCEND ; iLOCIX++ ) {
		// メモリから読み込み
		ReadProcessMemory( 
			CommonGlobalHandle::curProcess,
			(LPCVOID)(m_LAdress[iLOCIX]), 
			szCurMemoryData[iLOCIX], 
			sizeof(szCurMemoryData[iLOCIX]),
			NULL);

		// １つ前の文字と今の文字を比較することがあるので、有効な変化がある時、更新
		if (strlen(szCurMemoryData[iLOCIX]) > 1 && strcmp(szPreMemoryData[iLOCIX], szCurMemoryData[iLOCIX])) {
			strcpy(szPreMemoryData[iLOCIX], szCurMemoryData[iLOCIX]);
		}
	}
}

// 対象のメモリポインタの現在の値を得る
char* CurProcessMemory::getCurVarPtr( DataLocation eLOC ) {
	return szCurMemoryData[eLOC];
}

// 対象のメモリポインタの１つ前の値を得る
char* CurProcessMemory::getPreVarPtr( DataLocation eLOC ) {
	return szPreMemoryData[eLOC];
}

// 対象のメモリポインタのアドレスを得る
int CurProcessMemory::getMemoryAddress( DataLocation eLOC ) {
	return m_LAdress[eLOC];
}