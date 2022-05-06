#define _CRT_SECURE_NO_WARNINGS
/*----------------------------------------------------------------*
 HOOK系宣言
 *----------------------------------------------------------------*/

/////////////////////////////////////////////////////////////////////
// APIフック
/////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN
#define STRICT

#include "WinTarget.h"
#include <windowsx.h>
#include <tchar.h>
#include <tlhelp32.h>
#include <dbghelp.h>
#include <mmsystem.h>
#include <shellapi.h>

#include "CommonGlobalHandle.h"
#include "TSModCommand.h"
#include "CurProcessMemory.h"
#include "HookIntersectRectMessage.h"
#include "HookInvalidateRectMessage.h"
#include "HookDefWindowProcACustom.h"
#include "HookBitBltCustom.h"
#include "HookMessageBoxACustom.h"
#include "HookTextOutACustom.h"
#include "Menu.h"
#include "GetOSVersion.h"


// ImageDirectoryEntryToData
#pragma comment(lib, "dbghelp.lib")



// ひとつのモジュールに対してAPIフックを行う関数
void ReplaceIATEntryInOneMod(
                             PCSTR pszModuleName,
                             PROC pfnCurrent,
                             PROC pfnNew,
                             HMODULE hmodCaller) 
{
    ULONG ulSize;
    PIMAGE_IMPORT_DESCRIPTOR pImportDesc;
    pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToData(
        hmodCaller, TRUE, IMAGE_DIRECTORY_ENTRY_IMPORT, &ulSize);

    if (pImportDesc == NULL)
        return;

    while(pImportDesc->Name) {
        PSTR pszModName = (PSTR) ((PBYTE) hmodCaller + pImportDesc->Name);
        if (lstrcmpiA(pszModName, pszModuleName) == 0) 
            break;
        pImportDesc++;
    }

    if (pImportDesc->Name == 0)
        return;

    PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA) 
        ((PBYTE) hmodCaller + pImportDesc->FirstThunk);

    while(pThunk->u1.Function) {
        PROC *ppfn = (PROC*) &pThunk->u1.Function;
        BOOL fFound = (*ppfn == pfnCurrent);
        if (fFound) {
            DWORD dwDummy;
            VirtualProtect(ppfn, sizeof(ppfn), PAGE_EXECUTE_READWRITE, &dwDummy);
            WriteProcessMemory(
                CommonGlobalHandle::curProcess, ppfn, &pfnNew, sizeof(pfnNew), NULL);
            return;
        }
        pThunk++;
    }
    return;
}

// すべてのモジュールに対してAPIフックを行う関数
void ReplaceIATEntryInAllMods(
                              PCSTR pszModuleName, 
                              PROC pfnCurrent, 
                              PROC pfnNew) 
{
    // モジュールリストを取得
    HANDLE hModuleSnap = CreateToolhelp32Snapshot(
        TH32CS_SNAPMODULE, GetCurrentProcessId());
    if(hModuleSnap == INVALID_HANDLE_VALUE)
        return;

    MODULEENTRY32 me;
    me.dwSize = sizeof(me);
    BOOL bModuleResult = Module32First(hModuleSnap, &me);
    // それぞれのモジュールに対してReplaceIATEntryInOneModを実行
    while(bModuleResult) { 
        ReplaceIATEntryInOneMod(pszModuleName, pfnCurrent, pfnNew, me.hModule);
        bModuleResult = Module32Next(hModuleSnap, &me);
    }
    CloseHandle(hModuleSnap);
}

// フックする関数のプロトタイプを定義
typedef BOOL (WINAPI *PFNINTERSECTRECT)(LPRECT, CONST RECT *, CONST RECT *);

/*----------------------------------------------------------------*
 具体的なフック関数!!!! 
 *----------------------------------------------------------------*/
PROC pfnOrigIntersectRect = GetProcAddress( GetModuleHandleA("user32.dll"), "IntersectRect");

// 天翔記のメッセージウィンドウ系のハック
BOOL WINAPI Hook_IntersectRect(
	LPRECT lprcDst,        // 交差部分が格納されるバッファ
	CONST RECT *lprcSrc1,  // 最初の長方形
	CONST RECT *lprcSrc2   // 2 番目の長方形
)
{
	Hook_IntersectRectMessage(lprcDst, lprcSrc1, lprcSrc2);

	//-------------------------------------------- ここから下はオリジナルを呼び出す処理
	// その後、元のものを呼び出す
    BOOL bResult = ((PFNINTERSECTRECT) pfnOrigIntersectRect)(lprcDst, lprcSrc1, lprcSrc2);
	return bResult;
}

#include "DialogKahou.h"

extern DialogKahou DK;

// フックする関数のプロトタイプを定義
typedef BOOL (WINAPI *PFNINVALIDATERECT)(HWND, CONST RECT *, BOOL);

PROC pfnOrigInvalidateRect = GetProcAddress( GetModuleHandleA("user32.dll"), "InvalidateRect");

// 天翔記の再描画する必要のある範囲系のハック
BOOL WINAPI Hook_InvalidateRect(
	HWND hWnd,           // ウィンドウのハンドル
	CONST RECT *lpRect,  // 長方形の座標
	BOOL bErase          // 消去するかどうかの状態
)
{
	Hook_InvalidateRectMessage(hWnd, lpRect, bErase);

	//-------------------------------------------- ここから下はオリジナルを呼び出す処理
	// その後、元のものを呼び出す
	BOOL bResult = false;
	if ( DK.eModeKahou >= KAHOU_MODE3 ) {
		bResult = ((PFNINVALIDATERECT) pfnOrigInvalidateRect)(hWnd, NULL, bErase);
	} else {
		bResult = ((PFNINVALIDATERECT) pfnOrigInvalidateRect)(hWnd, lpRect, bErase);
	}
	return bResult;
}



// デモモードか？
extern bool is_DemoMode();

// フックする関数のプロトタイプを定義
typedef LRESULT (WINAPI *PFNDEFWINDOWPROCA)( HWND, UINT, WPARAM, LPARAM );

PROC pfnOrigDefWindowProcA = GetProcAddress( GetModuleHandleA("user32.dll"), "DefWindowProcA");

// 画像系のハック
LRESULT WINAPI Hook_DefWindowProcA(
	HWND hWnd,      // ウィンドウのハンドル
	UINT Msg,       // メッセージの識別子
	WPARAM wParam,  // メッセージの最初のパラメータ
	LPARAM lParam   // メッセージの 2 番目のパラメータ
)
{
	Hook_DefWindowProcACustom( hWnd, Msg, wParam, lParam );

	// スクリーンセイバーの起動を防止する。スクリーンセイバーになりそうな時に、起動しているアプリのいずれかが、1を返せば
	// スクリーンセイバーの起動は防止されるようだ。
	if ( Msg == WM_SYSCOMMAND && wParam == SC_SCREENSAVE ) {
		// 「デモモードならスクリーンセイバーを起動しない」のフラグが立っていて、現在デモモードならば、
		if ( TSModCommand::Environment::cmdNoScreenSaverInDemo == 1 && is_DemoMode() ) {
			// スクリーンセイバーは起動しない。
			return 1;

		}
	}

    // 全てのマウスメッセージの最初にこれが発生する。
	// マウスボタンが常にタイトルバー上で押されたことにする
 	if ( Msg == WM_NCHITTEST ) {

		// それ以外はウィンドウを動かす。(タイトルにマウスを置いていると同じ処理をする)
		
		// 以下、ボーダー付近の座標はタイトルバーをマウスでクリックしていることと同じこととする。
		// これにより、動かすことが可能となる。

		HWND hWnd = CommonGlobalHandle::tenshouWND;

		// スクリーン上の座標
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);

		// ウィンドウのスクリーン領域を得る
		RECT rect_move;
		GetWindowRect( hWnd, &rect_move );

		// ボーダーよりほんの内側も周辺とみなす。ボーダー幅が1しかなくても最小でも5ピクセルは動かせる幅を持つ
		int iBorderX = GetSystemMetrics( SM_CXBORDER );
		int iBorderY = GetSystemMetrics( SM_CYBORDER );
		iBorderX = max( iBorderX, 8 );
		iBorderY = max( iBorderY, 8 );
		rect_move.left += iBorderX;
		rect_move.right -= iBorderX;
		rect_move.top += iBorderY;
		rect_move.bottom -= iBorderY;

		// マウスのクリック点が、ボーダー枠付近にあったら、タイトルバーをクリックしたものとみなす。
		if ( !PtInRect( &rect_move, pt ) ) {

			// メニューバーとタイトルバーを隠すモードで、かつ、スクリーンのサイズ選択を終えている(メニューの「終了」がValildになった後
			if ( TSModCommand::Environment::cmdNoTitleBarAndMenuBar && CommonGlobalHandle::IsAfterScreenSizeSelected ) {

				// ウィンドウのスクリーン領域を得る
				RECT rect_menu;
				GetWindowRect( hWnd, &rect_menu );

				// トップに関してのみボーダーよりほんの内側も周辺とみなす。
				rect_menu.top += iBorderY;

				// 上部のバー付近でクリックしている。
				if ( !PtInRect( &rect_menu, pt ) ) {

					// 現在のメニューは？
					HMENU curMenu = GetMenu(hWnd);

					if ( GetAsyncKeyState(VK_LBUTTON) < 0 ) {

						// メニューが無い
						if ( !curMenu ) {
							SetMenu( CommonGlobalHandle::tenshouWND, CommonGlobalHandle::hMenu);
							// メニュー表示維持タイマーを上書きする。５秒維持
							SetTimer( CommonGlobalHandle::tenshouWND, TIMER_EVENT_ID_MENU_HIDE, 3000, TenshouMenuHide );
						} else {
							SetMenu( CommonGlobalHandle::tenshouWND, NULL);
						}

					}
				}
			}

			// 左クリックしていたら･･･
			if (GetAsyncKeyState(VK_LBUTTON) < 0 ) {

				return HTCAPTION;
			// 右クリックしていたら･･･
			} else if (GetAsyncKeyState(VK_RBUTTON) < 0 ) {
				// もしアイコン化していたら元に戻す
				if ( IsIconic(hWnd) ) {

					// 元に戻す
					OpenIcon( hWnd );
				} else {
					ShowWindow( hWnd, SW_MINIMIZE);
				}

				return HTCAPTION;
			}
		}

	}

	//-------------------------------------------- ここから下はオリジナルを呼び出す処理
	// その後、元のものを呼び出す
    LRESULT LPResult = ((PFNDEFWINDOWPROCA) pfnOrigDefWindowProcA)(hWnd, Msg, wParam, lParam );

    return LPResult;
}

// フックする関数のプロトタイプを定義
typedef LRESULT (WINAPI *PFNSENDMESSAGEA)( HWND, UINT, WPARAM, LPARAM );

PROC pfnOrigSendMessageA = GetProcAddress( GetModuleHandleA("user32.dll"), "SendMessageA");

// 画像系のハック
LRESULT WINAPI Hook_SendMessageA(
	HWND hWnd,      // ウィンドウのハンドル
	UINT Msg,       // メッセージの識別子
	WPARAM wParam,  // メッセージの最初のパラメータ
	LPARAM lParam   // メッセージの 2 番目のパラメータ
)
{
	//-------------------------------------------- ここから下はオリジナルを呼び出す処理
	// その後、元のものを呼び出す
    LRESULT LPResult = ((PFNSENDMESSAGEA) pfnOrigSendMessageA)(hWnd, Msg, wParam, lParam );

    return LPResult;
}




// フックする関数のプロトタイプを定義
typedef BOOL (WINAPI *PFNBITBLT)( HDC, int, int, int, int, HDC, int, int, DWORD );

PROC pfnOrigBitBlt = GetProcAddress( GetModuleHandleA("gdi32.dll"), "BitBlt");

// 画像系のハック
BOOL WINAPI Hook_BitBlt(
	HDC hdcDest, // コピー先デバイスコンテキストのハンドル
	int nXDest,  // コピー先長方形の左上隅の x 座標
	int nYDest,  // コピー先長方形の左上隅の y 座標
	int nWidth,  // コピー先長方形の幅
	int nHeight, // コピー先長方形の高さ
	HDC hdcSrc,  // コピー元デバイスコンテキストのハンドル
	int nXSrc,   // コピー元長方形の左上隅の x 座標
	int nYSrc,   // コピー元長方形の左上隅の y 座標
	DWORD dwRop  // ラスタオペレーションコード
)
{
	//-------------------------------------------- ここから下はオリジナルを呼び出す処理
	// その後、元のものを呼び出す
    BOOL bResult = ((PFNBITBLT) pfnOrigBitBlt)(hdcDest, nXDest, nYDest, nWidth, nHeight, hdcSrc, nXSrc, nYSrc, dwRop );

	return bResult;
}

extern CurProcessMemory CPM;

// フックする関数のプロトタイプを定義
typedef BOOL (WINAPI *PFNPATBLT)( HDC, int, int, int, int, DWORD );

PROC pfnOrigPatBlt = GetProcAddress( GetModuleHandleA("gdi32.dll"), "PatBlt");

// 画像系のハック
BOOL WINAPI Hook_PatBlt(
	HDC hdc,      // デバイスコンテキストのハンドル
	int nXLeft,   // 長方形の左上隅の x 座標
	int nYLeft,   // 長方形の左上隅の y 座標
	int nWidth,   // 長方形の幅
	int nHeight,  // 長方形の高さ
	DWORD dwRop   // ラスタオペレーションコード
)
{
	// 現在のデバイスコンテキストのハンドル値をコピー
	//-------------------------------------------- ここから下はオリジナルを呼び出す処理
    BOOL bResult = ((PFNPATBLT) pfnOrigPatBlt)(hdc, nXLeft, nYLeft, nWidth, nHeight, dwRop );

	return bResult;
}


// フックする関数のプロトタイプを定義
typedef int (WINAPI *PFNMESSAGEBOXA)(HWND, PCSTR, PCSTR, UINT);

PROC pfnOrigMessageBoxA = GetProcAddress( GetModuleHandleA("user32.dll"), "MessageBoxA");

int WINAPI Hook_MessageBoxA(
	HWND hWnd,          // オーナーウィンドウのハンドル
	PCTSTR lpText,     // メッセージボックス内のテキスト
	PCTSTR lpCaption,  // メッセージボックスのタイトル
	UINT uType          // メッセージボックスのスタイル
)
{
    // 元のものを呼び出す
    int nResult = ((PFNMESSAGEBOXA) pfnOrigMessageBoxA)(hWnd, lpText, lpCaption, uType );
	
	// 特別に後で呼び出す。(閉じたことを知るため)
	Hook_MessageBoxACustom( hWnd, lpText, lpCaption, uType, nResult );
	
	return nResult;
}

// フックする関数のプロトタイプを定義
typedef int (WINAPI *PFNTIMEGETTIME)(VOID);

PROC pfnOrigtimeGetTime = GetProcAddress( GetModuleHandleA("winmm.dll"), "timeGetTime");

int WINAPI Hook_TimeGetTime(VOID)
{

    // 元のものを呼び出す
    int nResult = ((PFNTIMEGETTIME) pfnOrigtimeGetTime)();
	
	return nResult;
}

// フックする関数のプロトタイプを定義
typedef HANDLE (WINAPI *PFNCREATEFILEA)(LPCTSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);

PROC pfnCreateFileA = GetProcAddress( GetModuleHandleA("kernel32.dll"), "CreateFileA");

HANDLE WINAPI Hook_CreateFileA(
	LPCTSTR lpFileName,                         // ファイル名
	DWORD dwDesiredAccess,                      // アクセスモード
	DWORD dwShareMode,                          // 共有モード
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, // セキュリティ記述子
	DWORD dwCreationDisposition,                // 作成方法
	DWORD dwFlagsAndAttributes,                 // ファイル属性
	HANDLE hTemplateFile                        // テンプレートファイルのハンドル
)
{

    // 元のものを呼び出す
    HANDLE nResult = ((PFNCREATEFILEA) pfnCreateFileA)(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	
	return nResult;
}



// フックする関数のプロトタイプを定義
typedef BOOL (WINAPI *PFNTEXTOUTA)(HDC, int, int, LPCTSTR, int );

PROC pfnOrigTextOutA = GetProcAddress( GetModuleHandleA("gdi32.dll"), "TextOutA");

BOOL WINAPI Hook_TextOutA(
	HDC hdc,           // デバイスコンテキストのハンドル
	int nXStart,       // 開始位置（基準点）の x 座標
	int nYStart,       // 開始位置（基準点）の y 座標
	LPCTSTR lpString,  // 文字列
	int cbString       // 文字数
) {
	// 先にカスタムの方を実行。
	Hook_TextOutACustom(hdc, nXStart, nYStart, lpString, cbString);

    // 元のものを呼び出す
    int nResult = ((PFNTEXTOUTA) pfnOrigTextOutA)(hdc, nXStart, nYStart, lpString, cbString);
	
	return nResult;
}

// フックする関数のプロトタイプを定義
typedef BOOL (WINAPI *PFGETVOLUMEINFORMATIONA)(LPCTSTR, LPTSTR, DWORD, LPDWORD, LPDWORD, LPDWORD, LPTSTR, DWORD );

PROC pfnOrigGetVolumeInformationA = GetProcAddress( GetModuleHandleA("kernel32.dll"), "GetVolumeInformationA");

BOOL WINAPI Hook_GetVolumeInformationA(
  LPCTSTR lpRootPathName,           // ルートディレクトリ
  LPTSTR lpVolumeNameBuffer,        // ボリューム名バッファ
  DWORD nVolumeNameSize,            // 名前バッファのサイズ
  LPDWORD lpVolumeSerialNumber,     // ボリュームのシリアル番号
  LPDWORD lpMaximumComponentLength, // ファイル名の最大の長さ
  LPDWORD lpFileSystemFlags,        // ファイルシステムのオプション
  LPTSTR lpFileSystemNameBuffer,    // ファイルシステム名を格納するバッファ
  DWORD nFileSystemNameSize         // ファイルシステム名を格納するバッファのサイズ
) {
    // 元のものを呼び出す
    int nResult = ((PFGETVOLUMEINFORMATIONA) pfnOrigGetVolumeInformationA)(
		lpRootPathName,
		lpVolumeNameBuffer, nVolumeNameSize,
		lpVolumeSerialNumber,
		lpMaximumComponentLength,
		lpFileSystemFlags,
		lpFileSystemNameBuffer,
		nFileSystemNameSize
		);
	
	if ( lpVolumeNameBuffer && strcmp(lpVolumeNameBuffer, "Audio CD") == 0) {
		strncpy(lpVolumeNameBuffer, "TENSHOUKI95", nVolumeNameSize-1);
	}

	return nResult;
}



// フックする関数のプロトタイプを定義
typedef BOOL (WINAPI *PFWINHELPA)(HWND, LPCTSTR, UINT, DWORD );

PROC pfnOrigWinHelpA = GetProcAddress( GetModuleHandleA("user32.dll"), "WinHelpA");

BOOL WINAPI Hook_WinHelpA(
    HWND hWndMain, // ウィンドウのハンドル
    LPCTSTR lpszHelp, // ヘルプファイル名
    UINT uCommand, // ヘルプの種類
    DWORD dwData // 追加のデータ
) {
    // 元のものを呼び出す
    int nResult = ((PFWINHELPA) pfnOrigWinHelpA)(
		hWndMain,
		lpszHelp,
		uCommand,
		dwData
		);

	if (nResult == 0) {
		if (getOSVersion() >= 6.0 ) { // Vista以降
			ShellExecute(CommonGlobalHandle::tenshouWND, "open", "http://xn--rssu31gj1g.jp/?page=nobu_install_winhlp32", NULL, NULL, SW_SHOWNORMAL);
		}
	}
	
	return nResult;
}


/*----------------------------------------------------------------*
 HOOK系処理
 *----------------------------------------------------------------*/
bool isHookIntersectRect = false;
bool isHookInvalidateRect = false;
bool isHookDefWindowProcA = false;
bool isHookSendMessageA = false;
bool isHookBitBlt = false;
bool isHookPatBlt = false;
bool isHookMessageBoxA = false;
bool isHookTimeGetTime = false;
bool isHookCreateFileA = false;
bool isHookTextOutA = false;
bool isHookGetVolumeInformationA = false;
bool isHookWinHelpA = false;


void HookFunctions() {
	PROC pfnOrig;
	if ( !isHookIntersectRect ) {
		isHookIntersectRect = true;

		pfnOrig = ::GetProcAddress( GetModuleHandleA("user32.dll"), "IntersectRect");
		ReplaceIATEntryInAllMods( "user32.dll", pfnOrig, (PROC)Hook_IntersectRect);
	}
	if ( !isHookDefWindowProcA ) {
		isHookDefWindowProcA = true;
		pfnOrig = ::GetProcAddress( GetModuleHandleA("user32.dll"), "DefWindowProcA");
		ReplaceIATEntryInAllMods( "user32.dll", pfnOrig, (PROC)Hook_DefWindowProcA);
	}
	/*
	if ( !isHookInvalidateRect ) {
		isHookInvalidateRect = true;

		pfnOrig = ::GetProcAddress( GetModuleHandleA("user32.dll"), "InvalidateRect");
		ReplaceIATEntryInAllMods( "user32.dll", pfnOrig, (PROC)Hook_InvalidateRect);
	}
	*/
	/*
	if ( !isHookSendMessageA ) {
		isHookSendMessageA = true;
		pfnOrig = ::GetProcAddress( GetModuleHandleA("user32.dll"), "SendMessageA");
		ReplaceIATEntryInAllMods( "user32.dll", pfnOrig, (PROC)Hook_SendMessageA);
	}
	*/
	/*
	if ( !isHookBitBlt ) {
		isHookBitBlt = true;
		pfnOrig = ::GetProcAddress( GetModuleHandleA("gdi32.dll"), "BitBlt");
		ReplaceIATEntryInAllMods( "gdi32.dll", pfnOrig, (PROC)Hook_BitBlt);
	}
	*/
	/*
	if ( !isHookPatBlt ) {
		isHookPatBlt = true;
		pfnOrig = ::GetProcAddress( GetModuleHandleA("gdi32.dll"), "PatBlt");
		ReplaceIATEntryInAllMods( "gdi32.dll", pfnOrig, (PROC)Hook_PatBlt);
	}
	*/
	if ( !isHookMessageBoxA ) {
		isHookMessageBoxA = true;
	    pfnOrig = ::GetProcAddress( GetModuleHandleA("user32.dll"), "MessageBoxA");
		ReplaceIATEntryInAllMods( "user32.dll", pfnOrig, (PROC)Hook_MessageBoxA);
	}
	/*
	if ( !isHookTimeGetTime ) {
		isHookTimeGetTime = true;
	    pfnOrig = ::GetProcAddress( GetModuleHandleA("winmm.dll"), "timeGetTime");
		ReplaceIATEntryInAllMods( "winmm.dll", pfnOrig, (PROC)Hook_TimeGetTime);
	}
	*/
	/*
	if ( !isHookCreateFileA ) {
		isHookCreateFileA = true;
	    pfnOrig = ::GetProcAddress( GetModuleHandleA("kernel32.dll"), "CreateFileA");
		ReplaceIATEntryInAllMods( "kernel32.dll", pfnOrig, (PROC)Hook_CreateFileA);
	}
	*/
	/*
	if ( !isHookTextOutA ) {
		isHookTextOutA = true;
		pfnOrig = ::GetProcAddress( GetModuleHandleA("gdi32.dll"), "TextOutA");
		ReplaceIATEntryInAllMods( "gdi32.dll", pfnOrig, (PROC)Hook_TextOutA);
	}
	*/
	/*
	if ( !isHookGetVolumeInformationA ) {
		isHookGetVolumeInformationA = true;
	    pfnOrig = ::GetProcAddress( GetModuleHandleA("kernel32.dll"), "GetVolumeInformationA");
		ReplaceIATEntryInAllMods( "kernel32.dll", pfnOrig, (PROC)Hook_GetVolumeInformationA);
	}
	*/
	if ( !isHookWinHelpA ) {
		isHookWinHelpA = true;
	    pfnOrig = ::GetProcAddress( GetModuleHandleA("user32.dll"), "WinHelpA");
		ReplaceIATEntryInAllMods( "user32.dll", pfnOrig, (PROC)Hook_WinHelpA);
	}
}





