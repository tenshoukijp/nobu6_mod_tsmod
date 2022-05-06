#include "WinTarget.h"
#include "CommonGlobalHandle.h"

BOOL Hook_TextOutACustom(
	HDC hdc,           // デバイスコンテキストのハンドル
	int nXStart,       // 開始位置（基準点）の x 座標
	int nYStart,       // 開始位置（基準点）の y 座標
	LPCTSTR lpString,  // 文字列
	int cbString       // 文字数
) {
	// 天翔記本体で利用しているデバイスコンテキストをコピーしておく。
	CommonGlobalHandle::hDC = hdc;

	return TRUE;
}
