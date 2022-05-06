#include <windows.h>
#include <windowsx.h>

#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"

LRESULT OnMouseEventHandler( HWND hWnd,      // ウィンドウのハンドル
							 UINT Msg,       // メッセージの識別子
							 WPARAM wParam,  // メッセージの最初のパラメータ
							 LPARAM lParam   // メッセージの 2 番目のパラメータ
)
{
	// マウス処理はここ
	// OutputDebugStream("%d, %d, %ld", Msg, wParam, lParam);
	// int x = GET_X_LPARAM(lParam);
	// int y = GET_Y_LPARAM(lParam);

	// マウスイベントの例
	switch (Msg) {
		case WM_LBUTTONDBLCLK : {
			/*
			 ここはゲーム無いでは流れてきている。
			 */
			return 0;
								}
		case WM_RBUTTONDOWN: {
			/*
			// クライアントクリック座標をワールド座標へと変換する。
			POINT pt;

			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);

			ClientToScreen(hWnd, &pt);

			// OutputDebugStream("%d,%d", pt.x, pt.y);
			*/
			return 0;

		}
		case WM_LBUTTONDOWN: {
			// ここは来ない
			return 0;
		}
	}

	return 0;
}
