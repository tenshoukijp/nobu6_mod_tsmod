// メニューそのものを最後に追加する。

#include "GameDataStruct.h"
#include "TSMod.h"
#include "Menu.h"
#include "Resource.h"
#include "GetOSVersion.h"
#include "WinTarget.h"
#include "TSModCommand.h"

#include <shellapi.h>

map<HMODULE, string> mapListEspecialModule;


// メニューアイテムを追加済かどうか
bool isAddMenuItems = false;

// 特殊モジュールとしてのリストに登録する。
void AddModuleItem( HMODULE hModule, string modulename ) {

	// モジュールとして登録する。
	mapListEspecialModule.insert( map<HMODULE, string>::value_type(hModule, modulename) );
}

// メニューアイテムの追加
// menuname メニューアイテムの文字列。"---"だとセパレータとなる。
void AddMenuItem(string menuname, int position, int wID) {

	MENUITEMINFO info;

	// 作成したメニュー項目を追加する
	ZeroMemory( &info, sizeof(info) );
	info.cbSize = sizeof( info );

	// ID番号の指定が無い場合
	if ( wID == -1 ) {
		info.fMask = MIIM_TYPE;
	// ID番号の指定がある場合
	} else {
		info.fMask = MIIM_TYPE | MIIM_ID;
		info.wID = wID;
	}
	if ( menuname == "---" ) {
		info.fType = MFT_SEPARATOR;
	} else {
		info.fType = MFT_STRING;
	}
	info.dwTypeData =(LPSTR)menuname.c_str();
	InsertMenuItem( CommonGlobalHandle::hMenu, position, FALSE, &info );
	DrawMenuBar( CommonGlobalHandle::tenshouWND );
}

// 上記の外部参照用
void WINAPI Extern_AddMenuItem(char *menuname, int position, int wID) {
	position = MENU_ITEM_MODIFY_BGN_ID; // 改造メニューの先頭のID。これより上に加えていく

	AddMenuItem(menuname, position, wID);
}

// メインメニューの追加
void AddMenu(string menuname, int position) {

	MENUITEMINFO info;

	// 作成したメニュー項目を追加する
	ZeroMemory( &info, sizeof(info) );
	info.cbSize = sizeof( info );
	info.fMask = MIIM_TYPE;
	info.fType = MFT_STRING;
	info.dwTypeData = (LPSTR)menuname.c_str();
	InsertMenuItem( CommonGlobalHandle::hMenu, 0xFFFF, TRUE, &info );
	DrawMenuBar( CommonGlobalHandle::tenshouWND );
}

// メニューの状態の変更。使い方
// ChangeMenuItem(hmenuPopup, ID_MENU, MFS_CHECKED);
// ChangeMenuItem(hmenuPopup, ID_MENU, MFS_UNCHECKED);
// ChangeMenuItem(hmenuPopup, ID_EXIT, MFS_GRAYED);
// ChangeMenuItem(hmenuPopup, ID_EXIT, MFS_ENABLED);
void ChangeMenuItem(HMENU hmenu, int nId, ULONG uState)
{
	MENUITEMINFO mii;
	
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask  = MIIM_ID | MIIM_STATE;
	mii.fState = uState;
	mii.wID    = nId;

	SetMenuItemInfo(hmenu, nId, FALSE, &mii);
}



/*
    GetItemDataは内部でGetMenuItemInfoを呼び出す自作関数で、
	第2引数と第3引数は、GetMenuItemInfoのそれと対応しています。
	LOWORD(wParam)は、メニューがポップアップメニューを持つかどうかでその値の意味が異なりますが、
	第3引数をHIWORD(wParam) & MF_POPUPとすれば、 ポップアップメニューを持つ場合は値がTUREとなり、
	そうでない場合はFALSEとなるため、
	第2引数との値の意味に矛盾はなくなります。 
*/
LPTSTR GetItemData(HMENU hmenu, int nParam, BOOL bByPosition)
{
	MENUITEMINFO mii;

	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask  = MIIM_DATA;

	GetMenuItemInfo(hmenu, nParam, bByPosition, &mii);

	return (LPTSTR)mii.dwItemData;
}



/*
SetMenuItemFromIconという自作関数は、アイコンのハンドルからビットマップを取得し、
 それをメニュー項目に設定する関数です。 
 第1引数は項目を含むメニューハンドルであり、
 第2引数はビットマップを設定したい項目のIDです。
 第3引数はアイコンのハンドルであり、これはSHGetStockIconInfoで取得しています。
 
 この関数はシステムで用意されているアイコンを返す関数であり、
 第1引数にSIID_SHIELDを指定した場合はシールドアイコンを取得できます。

	SHSTOCKICONINFO sii;
	sii.cbSize = sizeof(SHSTOCKICONINFO);
	SHGetStockIconInfo(SIID_SHIELD, SHGFI_ICON | SHGFI_SMALLICON, &sii);

今回はアイコンをハンドルとして取得したいので、第2引数にはSHGFI_ICONを指定し、さらに小さいアイコンとして取得するためにSHGFI_SMALLICONを指定しています。 
 */
static HBITMAP CreateBitmapARGB(int nWidth, int nHeight)
{
	LPVOID           lpBits;
	BITMAPINFO       bmi;
	BITMAPINFOHEADER bmiHeader;

	ZeroMemory(&bmiHeader, sizeof(BITMAPINFOHEADER));
	bmiHeader.biSize      = sizeof(BITMAPINFOHEADER);
	bmiHeader.biWidth     = nWidth;
	bmiHeader.biHeight    = nHeight;
	bmiHeader.biPlanes    = 1;
	bmiHeader.biBitCount  = 32;

	bmi.bmiHeader = bmiHeader;
	
	return CreateDIBSection(NULL, (LPBITMAPINFO)&bmi, DIB_RGB_COLORS, &lpBits, NULL, 0);
}
/*
GetSystemMetricsで小さいアイコンのサイズを取得し、それを基にCreateBitmapARGBという自作関数を呼び出しています。
 この関数は32ビットのビットマップをDIBセクションとして作成し、そのハンドル返します。
 ただし、ビットマップのハンドルだけではそのビットマップにアイコンのピクセルを格納できないため、
 SelectObjectでメモリデバイスコンテキストに選択するようにしています。
 
 このメモリデバイスコンテキストに対してDrawIconExを呼び出せば、
 選択されているビットマップにアイコンが描画されることになり、
 ビットマップにはアイコンのピクセルが格納されたことになります。
 
 後はこれをMENUITEMINFO構造体のhbmpItemに指定してSetMenuItemInfoを呼び出せば、
 メニュー項目にビットマップが設定されることになります。 
*/
void SetMenuItemFromIcon(HMENU hmenu, int nId, HICON hicon)
{
	UINT         uWidth, uHeight;
	HDC          hdcMem;
	HBITMAP      hbmp, hbmpPrev;
	MENUITEMINFO mii;

	uWidth = GetSystemMetrics(SM_CXSMICON);
	uHeight = GetSystemMetrics(SM_CYSMICON);
	hbmp = CreateBitmapARGB(uWidth, uHeight);
	
	hdcMem = CreateCompatibleDC(NULL);
	hbmpPrev = (HBITMAP)SelectObject(hdcMem, hbmp);
	DrawIconEx(hdcMem, 0, 0, hicon, uWidth, uHeight, 0, NULL, DI_NORMAL);
	SelectObject(hdcMem, hbmpPrev);
	DeleteDC(hdcMem);

	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_BITMAP;
	mii.hbmpItem = hbmp;
	SetMenuItemInfo(hmenu, nId, FALSE, &mii);	
}



// トップの設定メニューにぶら下がるポップメニューのメニューハンドルを得る
HMENU GetTenshoukiSetMenu() {
	return ::GetSubMenu(CommonGlobalHandle::hMenu, 1); // nPosが0ベースで1のメニュー
}


// トップの改造メニューにぶら下がるポップメニューのメニューハンドルを得る
HMENU GetTenshoukiModMenu() {
	return ::GetSubMenu(CommonGlobalHandle::hMenu, 2); // nPosが0ベースで2のメニュー
}

// トップのヘルプメニューにぶら下がるポップメニューのメニューハンドルを得る
HMENU GetTenshoukiHelpMenu() {
	return ::GetSubMenu(CommonGlobalHandle::hMenu, 3); // nPosが0ベースで3のメニュー
}


// ヘルプメニューをビットマップタイプにする。
static void SetBitmapTypeHelpMenu() {

	// Vista未満はここで処理終了。メニューのアイコンとかは変更しない。
	if ( getOSVersion() < 6.0 ) { // Vista 未満
		return;
	}
/*
	// ヘルプメニューのメニューハンドルを得る
	HMENU hmenuPopup = GetTenshoukiModMenu();

	SHSTOCKICONINFO sii;
	sii.cbSize = sizeof(SHSTOCKICONINFO);
	SHGetStockIconInfo(SIID_SHIELD, SHGFI_ICON | SHGFI_SMALLICON, &sii);
	SetMenuItemFromIcon(hmenuPopup, 121, sii.hIcon); // 121==操作ガイド

	// 既定のメニューでは、項目の左横にビットマップとチェックマークの2つのスペースが用意されますが、
	// 今回のようにビットマップだけを表示するつもりの場合は、1つのスペースだけで十分といえます。
	// このような場合は、メニュー項目ではなくメニュー自体の情報を設定するSetMenuInfoを呼び出し、
	// メニューのスタイルを変更することになります。 
	MENUINFO menuInfo;
	menuInfo.cbSize = sizeof(MENUINFO);
	menuInfo.fMask = MIM_STYLE; 
	GetMenuInfo(hmenuPopup, &menuInfo);
	menuInfo.dwStyle = (menuInfo.dwStyle & ~MNS_NOCHECK) | MNS_CHECKORBMP;
	SetMenuInfo(hmenuPopup, &menuInfo);
*/
}

// ヘルプメニューをビットマップタイプにする。
void SetApplicationMenuBitmapType() {
	SetBitmapTypeHelpMenu();
}


// タイトルバーの矩形領域を取得
TITLEBARINFO GetTenshouTitleBarInfo() {
	static TITLEBARINFO info;

	info.cbSize = sizeof(TITLEBARINFO);
	GetTitleBarInfo( CommonGlobalHandle::tenshouWND, &info );
	return info;
}

// タイトルとメニューなし
void setNoCaptionAndMenuBarBefore() {

}

// タイトルとメニューなし
void setNoCaptionAndMenuBarAfter() {
	// タイトルバーとメニュー無しフラグがONならば
	if ( TSModCommand::Environment::cmdNoTitleBarAndMenuBar ) {
		// 	メニューなし
		SetMenu( CommonGlobalHandle::tenshouWND, NULL );

		// タイトルバーなし
		LONG lStyle;
		lStyle  = GetWindowLong( CommonGlobalHandle::tenshouWND, GWL_STYLE );
		lStyle = WS_OVERLAPPED | WS_CLIPSIBLINGS | WS_VISIBLE | WS_THICKFRAME | WS_DLGFRAME ;
		SetWindowLong( CommonGlobalHandle::tenshouWND, GWL_STYLE, lStyle );
	}
}

// メニューをタイマーで隠す
VOID CALLBACK TenshouMenuHide( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
	SetMenu( hwnd, NULL );
}
