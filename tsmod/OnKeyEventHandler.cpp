#include <windows.h>
#include <windowsx.h>

#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"


LRESULT OnKeyEventHandler( HWND hWnd,      // ウィンドウのハンドル
						   UINT Msg,       // メッセージの識別子
						   WPARAM wParam,  // メッセージの最初のパラメータ
						   LPARAM lParam   // メッセージの 2 番目のパラメータ
)
{
	// WM_KEYDOWNがDDrawによって、奪われているので、WM_CHAR(何か通常の文字キーボードが押された)で我慢。DDrawの方に書いても良い
	// 画面サイズの選択フェイズは終了していること。(自動含む)
	if ( Msg == WM_CHAR ) {

		if ( GetKeyState('M')<0 && GetKeyState(VK_CONTROL) < 0 ) {

			// メニューバーとタイトルバーを隠すモードで、かつ、スクリーンのサイズ選択を終えている(メニューの「終了」がValildになった後
			if ( TSModCommand::Environment::cmdNoTitleBarAndMenuBar && CommonGlobalHandle::IsAfterScreenSizeSelected ) {

				// 現在のメニューは？
				HMENU curMenu = GetMenu(hWnd);

				// メニューが無い
				if ( !curMenu ) {
					SetMenu( CommonGlobalHandle::tenshouWND, CommonGlobalHandle::hMenu);
					// メニュー表示維持タイマーを上書きする。５秒維持
					SetTimer( CommonGlobalHandle::tenshouWND, TIMER_EVENT_ID_MENU_HIDE, 3000, TenshouMenuHide );

				} else {
					SetMenu( CommonGlobalHandle::tenshouWND, NULL );
				}
			}

		}
	}
	return 0;
}


