#include <windows.h>
#include "Menu.h"

#include "CommonGlobalHandle.h"

/*
システムメニューとは、ウインドウタイトルの左横にあるアイコンを クリックしたときに表示されるメニューのことで、
ウインドウメニューや コントロールメニューと呼ばれることもあります。

このメニューには独自の項目を追加することが可能で、
自分の好きなようにカスタマイズしても問題ありません。
システムメニューのハンドルは、GetSystemMenuで取得します。 
*/
/*
win32api

HMENU GetSystemMenu(
    HWND hWnd,
    BOOL bRevert
);
*/


static void InitializeMenuItem(HMENU hmenu, LPTSTR lpszItemName, int nId, int nPos) {
	MENUITEMINFO mii;
	
	mii.cbSize     = sizeof(MENUITEMINFO);
	mii.fMask      = MIIM_ID | MIIM_TYPE;
	mii.fType      = MFT_STRING;
	mii.wID        = nId;
	mii.dwTypeData = lpszItemName;

	InsertMenuItem(hmenu, nPos, TRUE, &mii);
}

// システムメニューに「中央配置」という項目を入れる
void addCenterItemInApplicationSystemMenu(void) {
	HMENU hmenu = GetSystemMenu(CommonGlobalHandle::tenshouWND, FALSE);
	InitializeMenuItem(hmenu, "中央の位置に戻す(&P)", RESOURCE_MENU_ID_CENTER_POS_IN_SYSTEM_MENU, 1);
}

// ウィンドウを中央へと配置しなおす
void SetCenterWindow(HWND hwnd) {
	/*
	int  x, y;
	int  nScreenWidth, nScreenHeight;
	RECT rc;
	GetWindowRect(hwnd, &rc);
	
	nScreenWidth  = GetSystemMetrics(SM_CXFULLSCREEN);
	nScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	x = (nScreenWidth - (rc.right - rc.left)) / 2;
	y = (nScreenHeight - (rc.bottom - rc.top)) / 2;
	*/

	int x = CommonGlobalHandle::latestCenterWindowRect.left;
	int y = CommonGlobalHandle::latestCenterWindowRect.top;

	SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOSIZE);
}