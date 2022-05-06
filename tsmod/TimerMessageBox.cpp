

#include "TimerMessageBox.h"


// TimerMessageBox.cpp : アプリケーションのエントリ ポイントを定義します。
//

/***********************************************************************
一般に、時間を制限してメッセージ ボックスを表示させる場合、
指定した時間が経過した後で自動的に閉じる通常のダイアログ
ボックスを実装する必要があります。

この方法の問題点は、Windows が提供する標準メッセージ
ボックスの機能が失われてしまうことです。

以下の例では、MessageBox 関数を使用して、指定した時間の経過後に
自動的に閉じるメッセージ ボックスを作成する方法を示します。

この例に関して、次の点に注意してください。
・この例では、指定した時間が経過した後にイベントを発生させる
　Windows タイマーを使用します。
・タイマー イベントが発生すると、PostQuitMessage API を使用して、
　MessageBox が使用しているモーダル メッセージ ループを抜けます。
・注 : WM_QUIT は、メイン メッセージ キューで取得されないように、
　メッセージ キューから削除する必要があります。

サンプル コード
注 : このサンプルでは、サービスから MessageBox を呼び出す場合の
MB_SERVICE_NOTIFICATION フラグの使用はサポートしていません。
MB_SERVICE_NOTIFICATION が指定された場合、メッセージ ボックスウィンドウは、
MessageBox を呼び出すプロセスによって所有されません。

***********************************************************************/



#include "WinTarget.h"




/***********************************************************************
*
*  Global variables
*
***********************************************************************/
HWND g_hwndTimedOwner;
BOOL g_bTimedOut;


/***********************************************************************
*
*  MessageBoxTimer
*
*      The timer callback function that posts the fake quit message.
*      This function causes the message box to exit because the message box
*      has determined that the application is exiting.
*
***********************************************************************/
void CALLBACK MessageBoxTimer(HWND hwnd,
							  UINT uiMsg,
							  UINT idEvent,
							  DWORD dwTime)
{
	g_bTimedOut = TRUE;
	if (g_hwndTimedOwner)
		EnableWindow(g_hwndTimedOwner, TRUE);
	PostQuitMessage(0);
}


/***********************************************************************
*
*  TimedMessageBox
*
*      The same as the standard MessageBox, except that TimedMessageBox
*      also accepts a timeout. If the user does not respond within the
*      specified timeout, the value 0 is returned instead of one of the
*      ID* values.
*
***********************************************************************/
int TimedMessageBox(HWND hwndOwner,
					LPCTSTR pszMessage,
					LPCTSTR pszTitle,
					UINT flags,
					DWORD dwTimeout)
{
	UINT idTimer;
	int iResult;

	g_hwndTimedOwner = NULL;
	g_bTimedOut = FALSE;

	if (hwndOwner && IsWindowEnabled(hwndOwner))
		g_hwndTimedOwner = hwndOwner;

	//
	// Set a timer to dismiss the message box.
	idTimer = SetTimer(NULL, 0, dwTimeout, (TIMERPROC)MessageBoxTimer);

	iResult = MessageBox(hwndOwner, pszMessage, pszTitle, flags);

	//
	// Finished with the timer.
	KillTimer(NULL, idTimer);

	//
	// See if there is a WM_QUIT message in the queue if we timed out.
	// Eat the message so we do not quit the whole application.
	if (g_bTimedOut)
	{
		MSG msg;
		PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE);
		iResult = -1;
	}

	return iResult;
}



/***********************************************************************
*
*  WinMain
*
*      Program entry point. Demonstrate TimedMessageBox().
*
***********************************************************************/


void TimerMessageBoxTest(void) {

	// タイマーメッセージボックス
	//--------------------------------------------------------------------------
	UINT uiResult = 0;

	//
	// Ask the user a question. Give the user five seconds to
	// answer the question.
	uiResult = TimedMessageBox(NULL,
		"メッセージ文",
		"タイトル",
		MB_OK,
		// NULL first parameter is important.
		3000); // 自動で消えるミリ秒。(この場合3秒)
}

