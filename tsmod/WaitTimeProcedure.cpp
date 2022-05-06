#include "WinTarget.h"

#include "CommonGlobalHandle.h"

// ミリ秒分待つ。メッセージ処理は継続される。
void WaitTimeProcedure(int iWaitTime) {
	DWORD preTimeGetTime = timeGetTime();

	MSG msg;
	while(1) {
		BOOL ret = GetMessage( &msg, NULL, 0, 0 );  // メッセージを取得する

		DWORD curTimeGetTime = timeGetTime();
		// 規定の間隔時間が過ぎた
		if ( int(curTimeGetTime - preTimeGetTime) > int(iWaitTime) ) {
			break;
		} 

		// メッセージを処理する
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
}

/*
BOOL GetMessage(
  LPMSG lpMsg,
  HWND hWnd,
  UINT wMsgFilterMin,
  UINT wMsgFilterMax
);


lpMsgは、MSG構造体のアドレスを指定します。 
関数から制御が返ると、この構造体に取得したメッセージの情報が格納されます。 

hWndは、メッセージを取得するウインドウのハンドルを指定します。
NULLを指定すると、呼び出し側スレッドに関連付けられている全ての
ウインドウへのメッセージを取得できます。 

wMsgFilterMinとwMsgFilterMaxは、取得対象のメッセージの範囲を指定します。
両方の引数に0を指定した場合、全てのメッセージが取得されることになります。

戻り値は、WM_QUIT以外のメッセージを取得した場合は0以外の値が返り、
WM_QUITを取得した場合は0が返ります。 引数のアドレスが無効である場合などは、-1が返ることもあります。 
*/


/*
メッセージを取得したら、次はそれをウインドウプロシージャに送らなければなりません。
このことをメッセージのディスパッチと呼び、DispatchMessageで行うことになります。
DispatchMessageは、内部でMSG構造体のhwndメンバを用いて ウインドウプロシージャのアドレスを取得する関数を呼び出し、
その後、MSG構造体のメンバを引数としてウインドウプロシージャを呼び出します。 

LRESULT DispatchMessage(
  CONST MSG *lpmsg
);


lpmsgは、MSG構造体のアドレスを指定します。
一般には、GetMessageで初期化したMSG構造体をそのまま指定することになるでしょう。
戻り値は、ウインドウプロシージャが返した値となります。 

メッセージの取得とディスパッチの方法を見てきたところで、
この2つの動作とメッセージキューの仕組みを今一度確認してみましょう。 

今、ウインドウでマウスの左ボタンが押されたとして、
システムはそのウインドウが属するアプリケーションの
メッセージキューにWM_LBUTTONDOWNを格納します。

アプリケーションは、この格納されたメッセージをGetMessageで取得し、
それをDispatchMessageでウインドウプロシージャに送ることになります。
このとき、たとえウインドウに新たなイベントが発生してメッセージが生成されたとしても、
システムはそれをメッセージキューに格納するだけですから、このメッセージが処理されるのは、
再びアプリケーションがGetMessageとDispatchMessageを呼び出すときとなります。 

これらのことから分かるように、アプリケーションがメッセージを処理するためには、
絶えずGetMessageとDispatchMessageを呼び出さなければなりませんから、
この2つの処理はループ文として実行することになります。 
*/

/*
TranslateMessageについて 

メッセージループで呼ばれているTranslateMessageは、
キーボードメッセージから文字メッセージを生成するために存在します。 

通常、アプリケーションがキーを押すと、WM_KEYDOWNというキーボードメッセージが送られ、
たとえばAキーならwParamにはAキーの仮想キーコードである0x41が格納されます。

しかし、仮想キーコードだけは、押したAが大文字のAなのか小文字のaなのか分かりませんから、
このような場合は文字コードをwParamとして持つ文字メッセージが必要になります。

TranslateMessageがWM_KEYDOWNを検出すると、文字メッセージであるWM_CHARがメッセージキューに格納され、
次にGetMessageを呼び出した際に取得されることになります。 
*/ 
