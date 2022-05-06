#include "WinTarget.h"
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "SetHelpFileName.h"
#include "PluginMod.h"
#include "ScenarioMod.h"
#include "Menu.h"

using namespace std;

#define SAVEDATAEDITOR_FILENAME  "SaveDataEditor.exe"
#define MODDEBUGGER_FILENAME  "ModDebugger.exe"
#define SAKURAEDITOR_FILENAME  "Sakura.exe"
#define MEMORYEDITOR_FILENAME  "MemoryEditorMod.dll"

extern LRESULT OnMouseEventHandler(	HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
extern LRESULT OnKeyEventHandler( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
extern LRESULT OnViewingApplicationPopUpMenu( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
extern LRESULT OnSelectingApplicationTopMenu( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, int nPos );
HRESULT OnSelectingApplicationSystemMenu(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

/*
WM_ACTIVATE                 ウインドウがアクティブ・非アクティブになるとき 
WM_ACTIVATEAPP              別のアプリケーションのウインドウがアクティブ・非アクティブになるとき 
WM_ASKCBFORMATNAME          クリップボードのフォーマット名を文字列バッファにコピーを要求 
WM_CANCELMODEJOURNAL        ジャーナル処理がキャンセルされた 
WM_CANCELMODE               メッセージボックス等のダイアログボックスが表示されようとしている 
WM_CAPTURECHAINGED          マウスのキャプチャが変化した 
WM_CHANGECBCHAIN            クリップボ－ドビューワチェーンが変更される 
WM_CHAR                     キーボードから文字入力を受け取った 
WM_CHARTOITEM               WM_CHARに応じてオーナー描画リストボックスが受け取る 
WM_CHILDACTIVATE            MDIコントロールの子ウインドウがアクティブ化 
WM_CHOOSEFONT_GETLOGFONT    [フォントの指定]ダイアログボックスに送るメッセージ 
WM_CLEAR                    テキストボックス・コンボボクスの選択テキストを削除 
WM_CLOSE                    ウインドウあるいはアプリケーションをクローズされた 
WM_COMMAND                  メニューが選択されたあるいはコントロールにイベントが発生した 
WM_COMPAREITEM              並び替えが指定された新しい項目のリストボックスなどで相対的な位置を決定する。 
WM_COMPACTING               システムがメモリ不足の状態にある 
WM_COPY                     テキストボックス・コンボボックスの選択テキストをクリップボードにコピーしたとき 
WM_COPYDATA                 別のプロセス上のウィンドウにデータを送信する 
WM_CREATE                   ウインドウが作成中 
WM_CTLCOLORBTN              コマンドボタンが描画されようとしている 
WM_CTLCOLORDLG              ダイアログが描画されようとしている 
WM_CTLCOLOREDIT             テキストボックスが描画されようとしている 
WM_CTLCOLORLISTBOX          リストボックスが描画されようとしている 
WM_CTLCOLORMSGBOX           メッセージボックスが描画されようとしている 
WM_CTLCOLORSCROLLBAR        スクロールバーが描画されようとしている 
WM_CTLCOLORSTATIC           ラベルが描画されようとしている 
WM_CUT                      テキストボックス・コンボボックスの選択テキストをクリップボードにコピー 
WM_DEADCHAR                 アクセント記号のような文字と組合せた文字を受け取った 
WM_DELETEITEM               オーナ描画リストボックスの項目が削除された 
WM_DESTROY                  ウインドウが破棄された 
WM_DESTROYCLIPBOARD         EmptyClipboard()によってクリップボードが空になった 
WM_DEVICECHANGE             ハードウェアの構成が変更された 
WM_DEVMODECHANGE            WIN.INI のデフォルトのデバイス名が変更された 
WM_DISPLAYCHANGE            ディスプレイの解像度が変更された 
WM_DRAWCLIPBOARD            クリップボードの内容が変わった 
WM_DRAWITEM                 オーナー描画コントロールの外観が変わった 
WM_DROPFILES                ファイルマネージャからの Drag & Drop 
WM_ENABLE                   ウインドウが有効・無効になった 
WM_ENDSESSION               Windows が終了する 
WM_ENTERIDLE                ダイアログボックス・メニューが入力待ち 
WM_ENTERMENULOOP            モーダルなメニューループに入ったとき 
WM_ENTERSIZEMOVE            ウィンドウが移動・サイズ変更した 
WM_ERASEBKGND               バックグラウンドを消去(再ペイント) 
WM_EXITMENULOOP             モーダルなメニューから抜け出した 
WM_EXITSIZEMOVE             ウィンドウの移動・サイズ変更の処理が終了したとき 
WM_FONTCHANGE               システムフォントが変更された 
WM_GETDLGCODE               コントロールに関連付けられたウインドウに送られるメッセージ 
WM_GETFONT                  テキストボックスラベル等が現在使っているフォントのハンドルを設定・取得する 
WM_GETHOTKEY                ウィンドウに関連付けられているホットキーを取得する 
WM_GETICON                  ウインドウに関連付けられているアイコンのハンドルを取得 
WM_GETMINMAXINFO            フォームの最大・最小化時に可能なサイズを取得 
WM_GETTEXT                  コントロールのキャプション・テキストボックスのテキストをバッファにコピー 
WM_GETTEXTLENGTH            ウィンドウに関連付けられているテキストのサイズ(バイト数)を取得 
WM_HELP                     [F1] or [?]の押された場合 
WM_HOTKEY                   RegisterHotKey関数で登録されたホットキーが押されたとき 
WM_HSCROLL                  水平スクロールバーを調整している 
WM_HSCROLLCLIPBOARD         クリップボードビューワで水平スルクールバーが使われた 
WM_ICONERASEBKGND           クラスアイコンがある時、アイコンの描画前に通知 
WM_IME_CHAR                 IMEが変換後の文字を受取った 
WM_IME_COMPOSITION          キー入力によってIMEが確定後の状態を変更した 
WM_IME_COMPOSITIONFULL      確定後の文字を表示するウインドウにスペースがない 
WM_IME_CONTROL              IMEにコマンドを送る 
WM_IME_ENDCOMPOSITION       文字を確定した 
WM_IME_KEYDOWN              キーを押した 
WM_IME_KEYUP                キーを離した 
WM_IME_NOTIFY               IMEウインドウの変化をアプリケーションに知らせる 
WM_IME_REQUEST              システムがIMEを変更しようとしている 
WM_IME_SELECT               システムがIMEを変更しようとしている 
WM_IME_SETCONTEXT           IMEの入力ウインドウがアクティブになった時アプリケーションに通知する 
WM_IME_STARTCOMPOSITION     確定する前に通知する 
WM_INITDIALOG               ダイアログボックスが表示される 
WM_INITMENU                 メニューが表示される直前に通知 
WM_INITMENUPOPUP            ポップアップメニューが表示される直前に通知 
WM_INPUTLANGCHANGE          言語ロケールが変更されたときトップレベルウィンドウに通知 
WM_INPUTLANGCHANGEREQUEST   言語ロケールが変更されたとき、入力フォーカスを持つウィンドウに通知 
WM_KEYDOWN                  キーボードのキーが押された 
WM_KEYUP                    キーが解放された 
WM_KILLFOCUS                フォーカスがなくなる直前に通知 
WM_LBUTTONDBLCLK            左のマウスボタンをダブルクリックした 
WM_LBUTTONDOWN              左のマウスボタンを押した 
WM_LBUTTONUP                左のマウスボタンが解放された 
WM_MBUTTONDBLCLK            中央のマウスボタンがダブルクリックされた 
WM_MBUTTONDOWN              中央のマウスボタンが押された 
WM_MBUTTONUP                中央のマウスボタンが解放された 
WM_MDIACTIVATE              MDIで指定した子ウインドウをアクティブにする 
WM_MDICASCADE               MDI子ウインドウをカスケード表示にする 
WM_MDICREATE                MDI子ウインドウを作成する 
WM_MDIDESTROY               MDI子ウインドウを破棄する 
WM_MDIGETACTIVE             現在アクティブなMDI子ウインドウのハンドルを取得 
WM_MDIICONARRANGE           MDI子ウインドウのアイコンを整列 
WM_MDIMAXIMIZE              MDI子ウインドウを最大化 
WM_MDINEXT                  次の子ウインドウをアクティブにする 
WM_MDIRESTORE               最大化・最小化されている子ウインドウを元のサイズに戻す 
WM_MDISETMENU               メニューをMDIフレームウインドウにリンク 
WM_MDITILE                  MDI子ウインドウをタイル整列 
WM_MEASUREITEM              オーナー描画コントロールが作成されたとき 
WM_MENUCHAR                 メニューのアクセスキーの英文字と一致しない文字を受け取った 
WM_MENUSELECT               メニューが強調表示・選択された 
WM_MOUSEACTIVATE            マウスクリックによりアクティブウインドウが移った 
WM_MOUSEMOVE                マウスが移動した 
WM_MOUSEWHELL               マウスホイールが回転した 
WM_MOVE                     ウインドウが移動した 
WM_MOVING                   リサイズ中にウィンドウに送られる 
WM_NCACTIVATE               非クライアント領域がアクティブ・非アクティブである 
WM_NCCALCSIZE               ウインドウのサイズを再計算する 
WM_NCCREATE                 ウインドウのスタイルが変更された時に最初に送られるメッセージ 
WM_NCDESTROY                非クライアント領域が破棄 
WM_NCHITTEST                非クライアント領域のどの部分でマウスが操作されたか調べる 
WM_NCLBUTTONDBCLK           非クライアント領域で左ボタンをダブルクリック 
WM_NCLBUTTONDOWN            非クライアント領域で左ボタンを押す 
WM_NCLBUTTONUP              非クライアント領域で左ボタンを解放 
WM_NCMBUTTONDBCLK           非クライアント領域で中央ボタンをダブルクリック 
WM_NCMBUTTONDOWN            非クライアント領域で中央ボタンを押す 
WM_NCMBUTTONUP              非クライアント領域で中央ボタンを解放 
WM_NCMOUSEMOVE              非クライアント領域でマウスが移動 
WM_NCPAINT                  非クライアント領域で再ペイントが必要 
WM_NCRBUTTONDBCLK           非クライアント領域で右ボタンをダブルクリック 
WM_NCRBUTTONDOWN            非クライアント領域で右ボタンを押す 
WM_NCRBUTTONUP              非クライアント領域で右ボタンを解放 
WM_NEXTDLGCTL               入力フォーカスを別の子ウインドウに移動 
WM_NEXTMENU                 メニューバーまたはシステムメニュー内を左右の矢印キーを使用して移動したとき 
WM_NOTIFY                   コントロールにイベントが発生した時、またはコントロールがデータを要求している 
WM_NULL                     これ自体は何もしないがメッセージを操作する場合に使う 
WM_PAINT                    ウインドウのクライアント領域を再描画しなければならない 
WM_PAINTCLIPBOARD           ウインドウのクライアント領域を再描画しなければならない 
WM_PAINTICON                ウインドウのクライアント領域を再描画しなければならない 
WM_PALETTECHANGED           システムパレットが変更された 
WM_PALETTEISCHANGING        システムパレットが別のウインドウによって変更されようとしている 
WM_PASTE                    テキストボックス、コンボボックスにおいてクリップボードからコピーするとき送る 
WM_PARENTNOTIFY             あるイベントが子ウインドウで発生したことを親ウインドウに通知する 
WM_POWER                    システムがパワー節約状態に入る・出ること通知する 
WM_POWERBROADCAST           パワー管理イベントを検出するために送られてくる 
WM_PRINT                    指定のデバイスコンテキストの再描画を要求 
WM_PRINTCLIENT              指定のデバイスコンテキスト内のウィンドウのクライアント領域の描画を要求 
WM_QUERYDRAGICON            アイコンを表示しようとする時にクラスアイコンを持たないウインドウに送る 
WM_QUERYENDSESSION          Windowsを終了する時に全てのウインドウに問合わせる 
WM_QUERYNEWPALETTE          入力フォーカスを受取る時、論理パレットで置換えるかどうか問合わせる 
WM_QUERYOPEN                アイコン化されたウインドウが復元する時、直前の位置とサイズを要求されているかどうか問合わせる 
WM_QUEUESYNC                ユーザー入力メッセージとフックプロシージャからのメッセージを区別する 
WM_QUIT                     アプリケーションが処理する最後のメッセージ 
WM_RBUTTONDBCLK             右のマウスボタンをダブルクリックした 
WM_RBUTTONDOWN              右マウスボタンを押した 
WM_RBUTTONUP                右マウスボタンを解放した 
WM_RENDERALLFORMATS         ディレイドレンダリングしているとき、クリップボードのオーナーが破棄される前に送られる 
WM_RENDERFORMAT             ディレイドレンダリングをしているデータをアプリケーションが要求したとき 
WM_SETCURSOR                マウスをキャプチャしているウインドウがない時、マウスがあるウインドウに送られる 
WM_SETFOCUS                 ウインドウが入力フォーカスを得た 
WM_SETFONT                  論理フォントを設定する 
WM_SETHOTKEY                ホットキーをウィンドウに関連付ける 
WM_SETICON                  アイコンをウインドウに関連付ける 
WM_SETREDRAW                コントロールの再描画操作を制御する 
WM_SETTEXT                  ウインドウ(コントロール)のタイトル・テキストを変更 
WM_SETTINGCHANGE            システム全体に影響する設定変更をしたとき 
WM_SHOWWINDOW               ウインドウが表示・非表示になった 
WM_SIZE                     ウインドウのサイズが変更 
WM_SIZECLIPBOARD            クリップボードビューワのクライアント領域のサイズが変更されるとき 
WM_SIZING                   ウィンドウのサイズが変更中 
WM_SPOOLERSTATUS            印刷キューに印刷ジョブが追加・削除された 
WM_STYLECHANGED             SetWindowLong関数によってウインドウスタイルを変更した 
WM_STYLECHANGING            SetWindowLong関数によってウインドウスタイルを変更されそう 
WM_SYNCPAINT                別のスレッドで作成されたウィンドウが上に重なったとき 
WM_SYSCHAR                  [Alt]キーとの組合せ以外はWM_CHARと同じ 
WM_SYSCOLORCHANGE           システムカラーが変更された 
WM_SYSCOMMAND               システムメニューが操作された 
WM_SYSDEADCHAR              アクセント記号のような別の文字と組合せた文字入力を受け取った 
WM_SYSKEYDOWN               システム文字を意味するキーを押した 
WM_SYSKEYUP                 システム文字を意味するキーが解放された 
WM_TIMECHANGE               システムタイマが変更された 
WM_TIMER                    SetTimer()で設定した時間を経過 
WM_UNDO                     テキストボックスで Undo 操作を実行する 
WM_USER                     ユーザーが定義できるメッセージの使用領域を表すだけでこれ自体に意味はない 
WM_USERCHANGED              ログオン・ログオフ時に全てのウィンドウに送られる 
WM_VKEYTOITEM               リストボックスがWM_KEYDOWNに応答したとき 
WM_VSCROLL                  ユーザーが垂直スクロールバーを調整した 
WM_VSCROLLCLIPBOARD         クリップボードビューワ内でスクロールイベント発生 
WM_WINDOWPOSCHANGED         SetWindowPos()後に位置とサイズを変更したウインドウに送られる 
WM_WINDOWPOSCHANGING        SetWindowPos()で位置とサイズを変更しているウインドウに送られる 
WM_WININICHANGE             WIN.INIが変更された 
*/

/*
メッセージの範囲 

メッセージを通じて何らかの通知や要求を行いたい場合、 WM_COPYDATAを利用するのが必ずしも効果的とは限りません。
どのようなメッセージであれ、それを当事者間が通信の合図と認識できればよいのですから、
独自に定義した数値をメッセージとして利用するのもまた一興です。

ただし、このような場合、その定義した数値が既存のメッセージと重複してはなりませんから、
予め利用可能なメッセージの範囲を知っておかなければなりません。 


メッセージの範囲

意味

0からWM_USER - 1    システムによって定義されているメッセージ。 
WM_USERから0x7FFF   個々のウインドウクラスのためのメッセージ。 たとえば、ツールチップコントロールをアクティブにするTTM_ACTIVATEは、 WM_USER + 1として定義されている。 
WM_APPから0xBFFF    アプリケーションが自由に使ってよいとされる範囲。 
0xC000から0xFFFF    RegisterWindowMessageが返す値。 
0xFFFFより大きい値  将来のために予約されている。 

注目すべきところはWM_APPです。 このメッセージから0xBFFFまでの値はアプリケーションが自由に使うことが許されているため、
オリジナルなメッセージとしてアプリケーション間の通信に利用できます。
WM_COPYDATAはデータの送信を目的としたメッセージですが、単純に通知や要求などを行いたい場合はWM_APPのほうが適切でしょう。 

#define WM_LAYOUTCHANGE WM_APP
・
・
PostMessage(hwndTarget, WM_LAYOUTCHANGE, 0, 0); // SendMessageでもよい


このコードでは、WM_APPを意図的にWM_LAYOUTCHANGEという形で定義していますが、
これはメッセージの役割を名前から推測できるようにするためです。
たとえば、対象となるウインドウにレイアウトの変更を促すのであれば、
それに即した名前を使ったほうがコードは分かりやすくなります。
wParamとlParamは自由に使うことができますが、
あくまで送れるのは4バイトのデータが2つまでとなります。
複数のオリジナルメッセージが必要な場合は、WM_APP+n(nは0から0xBFFF)という形で定義します。 
*/ 

// 画像系のハック
LRESULT Hook_DefWindowProcACustom(
	HWND hWnd,      // ウィンドウのハンドル
	UINT Msg,       // メッセージの識別子
	WPARAM wParam,  // メッセージの最初のパラメータ
	LPARAM lParam   // メッセージの 2 番目のパラメータ
)
{
	if ( Msg == WM_CREATE ) {
		// DefWindowProcに流れてくること無くリターンされている。

	} else if (Msg == WM_COMMAND ) {
		// ここは来ない。DDrawの方を使うこと。

	// マウス系の処理全て
	} else if ( WM_MOUSEFIRST <= Msg && Msg <= WM_MOUSELAST ) {
		// ここは有効
		return OnMouseEventHandler( hWnd, Msg, wParam, lParam );

	// アプリ左上のアイコンを右クリした時に出てくるシステムコマンドを選んだとき
	} else if ( Msg == WM_SYSCOMMAND ) {
		// ここは来るものもある。DDrawのフックの方は来ない。
		return OnSelectingApplicationSystemMenu( hWnd, Msg, wParam, lParam );


	// ポップアップメニュー(メインウィンドウにくっついてるメニュー[=ファイル・改造・など]のメインメニュー]以外のメニューのこと)表示時はここが有効となる。*/
	} else if ( Msg == WM_NCMOUSEMOVE ) {

		/*WM_MENUSELECTは、メインメニューの項目の選択時には送られてくることはありません。
		  クリックした場合は送られるのですが、簡単な概要を先に示しておくためにも、 項目の選択を擬似的に再現する価値はあるでしょう。
		  マウスカーソルがメインメニューやウインドウタイトルなどの非クライアント領域を 通過するときにはWM_NCMOUSEMOVEというメッセージが送られるため、
		  これを利用した項目の選択方法を考えてみます。*/

		int   nPos;
		POINT pt;
		HMENU hmenu;

		hmenu = GetMenu(CommonGlobalHandle::tenshouWND);

		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);

		// その座標にトップメニューの項目はあるか？
		nPos = MenuItemFromPoint(CommonGlobalHandle::tenshouWND, hmenu, pt);
		if (nPos != -1) { // 存在するならば
			 OnSelectingApplicationTopMenu( hWnd, Msg, wParam, lParam, nPos );
		}

		/*
		MenuItemFromPointは、第3引数で表される位置に項目があるかどうかを調べ、 
		項目がなければ-1を返し、あれば項目の位置を返します。
		この第3引数にlParamで表されるカーソルの位置を指定すれば、
		カーソルが項目を選択しているかどうかを確認できるため、
		後の処理はWM_MENUSELECTと同じように書くことができます。
		MenuItemFromPointが返すのは項目の識別子ではなく位置ですから、
		GetItemDataの第3引数はTRUEにします。 
		*/
	} else if ( Msg == WM_MENUSELECT ) {
		// ここは来ない、後述の場所で別途フックしている。

	// ポップアップメニュー(メインメニューのサブとしてぶら下がってるものや、さらにそのサブのもの)が表示される直前に実行される。
	} else if ( Msg == WM_INITMENUPOPUP ) {
		return OnViewingApplicationPopUpMenu( hWnd, Msg, wParam, lParam );

	} else if ( WM_KEYFIRST <= Msg && Msg <= WM_KEYLAST  ) {
		// ここは有効。ただし、WM_KEYDOWNは天翔記によって奪われている。
		return OnKeyEventHandler( hWnd, Msg, wParam, lParam );

	// 右クリックとほぼ同義。lparamなどに入る値が違うだけ。
	} else if ( Msg == WM_CONTEXTMENU ) {
		/*
		POINT pt;

		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);

		// OutputDebugStream("%d%d", pt.x, pt.y);	

		return 0;
		*/


	// 最小化状態から元へと戻すとき
	} else if ( Msg == WM_QUERYOPEN ) {
		// ここは来ない、どこかでリターンされてしまっている。

		// WM_QUERYOPENで0を返した場合、ウインドウは最小化の状態から元に戻れません。
		// そのため、このメッセージでメッセージボックスを表示すれば、 ウインドウを元に戻してよいかの確認が可能となります。
		// 0以外の値を返した場合は、ウインドウは元のサイズに戻ります。 DefWindowProcは、このメッセージでは常にTRUEを返します。 

		// return 0;

		// ゲーム起動時と、終了時だけ、来る。他はDDraw.dllがWM_PAINTを含めた部分をフックしているので来ない。
	} else if ( Msg == WM_PAINT ) {
		// ここは来ない。DDrawでフックしてる方でやること

	// フォーカスがなくなる直前に通知 
	} else if ( Msg == WM_KILLFOCUS ) {
		// ここはちゃんと来る

	// ウインドウが入力フォーカスを得た
	} else if ( Msg == WM_SETFOCUS ) {
		// ここはちゃんと来る

	// PCOPYDATASTRUCT型データの転送を得た
	} else if ( Msg == WM_COPYDATA ) {
		// OutputDebugString("CPYDTA");
		/*
		受信側の例:

			PCOPYDATASTRUCT pData = (PCOPYDATASTRUCT)lParam;

			if (pData->dwData == 1) dwDataは送信チャンネルとでも言うべきもの
				SetWindowText((HWND)wParam, (LPTSTR)pData->lpData);
			}
		*/


		/*
		送信側の説明:

			WM_COPYDATAは、必ずSendMessageで送ります。

				SendMessage(hwndTarget, WM_COPYDATA, (WPARAM)hwnd, (LPARAM)&data);

			wParamは、送信元のウインドウハンドルを指定します。
			lParamは、COPYDATASTRUCT構造体のアドレスです。
			COPYDATASTRUCT構造体は、以下のように定義されています。

				typedef struct tagCOPYDATASTRUCT {
					ULONG_PTR dwData;	// 送信チャンネルとでもいうもの
					DWORD cbData;		// lpDataのデータサイズ(sizeof)
					PVOID lpData;       // 送りたいデータのアドレス
				} COPYDATASTRUCT, *PCOPYDATASTRUCT;


			dwDataは、送るべきデータの識別子を指定します。
			WM_COPYDATAを受け取った側はこの値を参照して、 どの系統のデータが送られてきたのかを確認できます。
			cbDataは、lpDataが指すメモリのサイズです。
			COPYDATASTRUCT構造体のサイズを指定するのではありません。
			lpDataは、送りたいデータを格納するメモリへのアドレスを指定します。
			このメンバには、NULLを指定することもできます。(データを送らなくて何かの合図だけ送る場合)


		送信側の例:
			TCHAR szData[] = TEXT("hello");
			COPYDATASTRUCT data;

			data.dwData = 1; // 送信チャンネル番号とでもいうもの
			data.cbData = sizeof(szData);
			data.lpData = szData;
			
			SendMessage(hwndTarget, WM_COPYDATA, (WPARAM)hwnd, (LPARAM)&data);
		*/

	// アクティブ化、非アクティブ化の切り替わりの時
	} else if ( WM_ACTIVATE ) {
		int fActive =  wParam & 0xFFFF;         // アクティブ化フラグ 
		/*
		 0 (WA_INACTIVE) 非アクティブ化されます。
		 1 (WA_ACTIVE) マウスクリック以外の方法（例えば、キーボードインターフェースによる選択や、 SetActiveWindow 関数によるもの）でアクティブ化されます。
		 2 (WA_CLICKACTIVE) マウスクリック
		 */
		// マウスによってアクティブになった
		if (fActive == WA_CLICKACTIVE ) {
			if ( IsIconic(CommonGlobalHandle::tenshouWND) ) {
				OpenIcon(CommonGlobalHandle::tenshouWND);
			}
		}

	}


	return 0;
}


// ポップアップメニュー(メインウィンドウにくっついてる項目以外のメニューのこと)表示時はここが有効となる。
HRESULT OnViewingApplicationPopUpMenu( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam ) {
	return 0;
}

// トップメニュー(天翔記の場合「終了」など)を選んだ際に実行される。
HRESULT OnSelectingApplicationTopMenu(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, int nPos ) {
	// OutputDebugStream("%d", nPos);
	return 0;
}


// アプリの左上アイコンを右クリした時にでる「システムメニュー」を実行した時。
HRESULT OnSelectingApplicationSystemMenu(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam ) {

	int nId = LOWORD(wParam);

	// システムメニューに追加している「中央の位置に戻す」の項目
	if (nId == RESOURCE_MENU_ID_CENTER_POS_IN_SYSTEM_MENU ) {

		SetCenterWindow(CommonGlobalHandle::tenshouWND);

	// スクリーンセイバーが起動しようとしている
	} else if ( wParam == SC_SCREENSAVE ) {
		// ここは有効。
		
	// 最大化しようとしている
	} else if ( wParam == SC_MAXIMIZE ) {
		// ここはこない。天翔記がリターンしてしまっている。

	// 最小化しようとしている
	} else if ( wParam == SC_MAXIMIZE ) {
		// ここはこない。天翔記がリターンしてしまっている。

	// 元のサイズへと戻そうとしている。
	} else if ( wParam == SC_RESTORE ) {
		// ここは有効。

	}

	return 0;
}





void setMenuEnableOrDisable() {

	struct stat  st;

	// メモリエディタの場合に、ここにメニュー足し込み。IDを付ける。
	if ( stat( MEMORYEDITOR_FILENAME , &st) == 0 ) {
		AddMenuItem( "メモリエディタ", RESOURCE_MENU_ID_MODDEBUGGER_EXECUTE, RESOURCE_MENU_ID_MEMORY_EDITOR_MOD);
	}

	// セーブデータエディタが無い
	if ( stat( SAVEDATAEDITOR_FILENAME , &st) != 0 ) {
		// セーブデータエディタ起動のメニューを使えないようにする。
		EnableMenuItem(CommonGlobalHandle::hMenu, RESOURCE_MENU_ID_SAVEDATA_EXECUTE, MF_GRAYED);
	}

	if ( stat( MODDEBUGGER_FILENAME, &st) != 0 ) {
		// ＭＯＤデバッガー起動のメニューを削除する。
		DeleteMenu(CommonGlobalHandle::hMenu, RESOURCE_MENU_ID_MODDEBUGGER_EXECUTE, MF_BYCOMMAND);
	}

	// サクラエディターが存在しなかったら…
	if ( stat( SAKURAEDITOR_FILENAME, &st) != 0 ) {
		// サクラエディタ起動のメニューを削除する。
		DeleteMenu(CommonGlobalHandle::hMenu, RESOURCE_MENU_ID_SAKURA_EXECUTE, MF_BYCOMMAND);
	}

}


int idSelectMenu = -1;
// セーブデータエディタを起動
void WINAPI OnTenshouExeWmMessageSaveDataEditor() {
	ShellExecute(CommonGlobalHandle::tenshouWND, NULL, SAVEDATAEDITOR_FILENAME, psznb6savedatafilename, NULL, SW_SHOWNORMAL);
}

// ＭＯＤデバッガーを起動
void WINAPI OnTenshouExeWmMessageModDebugger() {
	ShellExecute(CommonGlobalHandle::tenshouWND, NULL, MODDEBUGGER_FILENAME, NULL, NULL, SW_SHOWNORMAL);
}

// サクラエディタを起動
void WINAPI OnTenshouExeWmMessageSakuraEditor() {
	ShellExecute(CommonGlobalHandle::tenshouWND, NULL, SAKURAEDITOR_FILENAME, NULL, NULL, SW_SHOWNORMAL);
}

// ＤＶＤ版のヘルプを起動
void WINAPI OnTenshouExeWmMessageDvdVerHelp() {
	ShellExecute(CommonGlobalHandle::tenshouWND, NULL, DVDVER_HELP_FILENAME, NULL, NULL, SW_SHOWNORMAL);
}

// ＣＨＭ版のヘルプを起動
void WINAPI OnTenshouExeWmMessageChmVerHelp() {
	ShellExecute(CommonGlobalHandle::tenshouWND, NULL, CHMVER_HELP_FILENAME, NULL, NULL, SW_SHOWNORMAL);
}

// ＸＰ版のWinHlp32.exeを起動
void WINAPI OnTenshouExeWmMessageXpVerExeHelp() {
	ShellExecute(CommonGlobalHandle::tenshouWND, "open", XPVER_HELPEXE_FILENAME, CDVER_HELP_FILENAME, NULL, SW_SHOWNORMAL);
}


// メニュー選択
void SwitchSelectingMenuExecute() {

	// 以下改造の項目に追加したモノ
	if (idSelectMenu == RESOURCE_MENU_ID_SAVEDATA_EXECUTE) {
		OnTenshouExeWmMessageSaveDataEditor();	
	} else if (idSelectMenu == RESOURCE_MENU_ID_MODDEBUGGER_EXECUTE) {
		OnTenshouExeWmMessageModDebugger();	
	} else if (idSelectMenu == RESOURCE_MENU_ID_SAKURA_EXECUTE) {
		OnTenshouExeWmMessageSakuraEditor();	
	} else if (idSelectMenu == RESOURCE_MENU_ID_DVD_VER_HELP_EXECUTE) {
		OnTenshouExeWmMessageDvdVerHelp();	
	} else if (idSelectMenu == RESOURCE_MENU_ID_CHM_VER_HELP_EXECUTE) {
		OnTenshouExeWmMessageChmVerHelp();	
	} else if (idSelectMenu == RESOURCE_MENU_ID_XP_VER_HELPEXE_EXECUTE) {
		OnTenshouExeWmMessageXpVerExeHelp();	
	}

	// PluginModのApplicationSelectMenuItemメソッド
	// 読み込んだDLL群に対して、OnApplicationSelectMenuItemがあれば、それを実行なければ解放
	for ( vector<HMODULE>::iterator it = listOfTargetDLL.begin(); it != listOfTargetDLL.end(); it++ ) {

		// DLLが読めたのであれば、OnFinalize関数が存在するかをチェック。
		PFNTARGETMODAPPLICATIONSELECTMENU p_OnApplicationSelectMenuItem = (PFNTARGETMODAPPLICATIONSELECTMENU)GetProcAddress(*it, "OnApplicationSelectMenuItem");

		// OnFinalizeが存在すれば実行
		if ( !p_OnApplicationSelectMenuItem ) { continue; }

		((PFNTARGETMODAPPLICATIONSELECTMENU) p_OnApplicationSelectMenuItem)(idSelectMenu);
	}

	// ScenarioModで増やす処理の呼出し
	if (p_snOnApplicationSelectMenuItem) {
		p_snOnApplicationSelectMenuItem(idSelectMenu);
	}


}


/*
WM_COMMAND時のIDの判定
00417013  |. 0FB74424 10    MOVZX   EAX, WORD PTR SS:[ESP+10]
00417018  |. 8BEC           MOV     EBP, ESP
0041701A  |. 83F8 64        CMP     EAX, 64
0041701D  |. 74 1F          JE      SHORT TENSHOU.0041703E
0041701F  |. 83F8 6F        CMP     EAX, 6F
00417022  |. 74 31          JE      SHORT TENSHOU.00417055
00417024  |. 83F8 70        CMP     EAX, 70
00417027  |. 74 49          JE      SHORT TENSHOU.00417072
00417029  |. 83F8 79        CMP     EAX, 79
0041702C  |. 74 6B          JE      SHORT TENSHOU.00417099
0041702E  |. 3D 81000000    CMP     EAX, 81
00417033  |. 0F84 85000000  JE      TENSHOU.004170BE
00417039  |. E9 9B000000    JMP     TENSHOU.004170D9
*/
/*
上のID分岐の各々で処理すべき内容を実行した後は、以下の行にジャンプしてくる。よってプログラムでもこの流れに習う。
004170D9  |> 33C0           XOR     EAX, EAX
004170DB  |. 5D             POP     EBP
004170DC  \. C3             RETN
*/

int pTenshouExeJumpFromToOnTenshouExeWmMessage			=0x41702E;	// 関数はこのアドレスから、OnTenshouExeCallWmMessageへとジャンプしてくる。
int pTenshouExeJumpVersionFromToOnTenshouExeWmMessage	=0x4170BE;  // 「天翔記について」のバージョン表示用
int pTenshouExeReturnLblFromOnTenshouExeWmMessage		=0x4170D9;	// 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w


// ヘルプのバージョン表示が来た場合にここに一端ジャンプして、さらにすぐジャンプ
// __asm の jeコマンドでポインタでないとダメなため。
__declspec( naked ) void WINAPI OnTenshouExeWmMessageVersionInfo() {
	__asm {
		jmp pTenshouExeJumpVersionFromToOnTenshouExeWmMessage
	}
}
#pragma warning(push)
#pragma warning(disable:4414)
// 以下「C4414」のワーニングの発生するコードがあっても、コンパイル時には出力されなくなる。

// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeWmMessage() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		// バージョン情報だった場合は、OnTenshouExeWmMessageVersionInfo 経由でそのままバージョン表示ダイアログのアドレスへ
		cmp eax, 0x81							// メニューリソースの129 == 0x81 はバージョン情報
		je OnTenshouExeWmMessageVersionInfo		// JE  TENSHOU.004170BE 

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	// eaxには CallWindowProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )のうちのwParamの値が入っている
	// これを保存しておく
	__asm {
		mov idSelectMenu, eax		// eaxをコピーしておく
	}

	// どのメニューが選ばれたのか判定。TENSHOU.EXEにデフォであったものは、TENSHOU.EXEと __asmの先頭２行でやってる。
	SwitchSelectingMenuExecute();


	// スタックに引き出す
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop ecx
		pop edx
		pop ebx
		pop eax

		jmp pTenshouExeReturnLblFromOnTenshouExeWmMessage
	}
}

// 以上「C4414」のワーニングの発生するコードがあっても、コンパイル時には出力されなくなる。
#pragma warning(pop)

/*
0041702E  |. 3D 81000000    CMP     EAX, 81							; ⇒ OnTenshouExeWmMessageへ
							└ ここを実行時に書き換えてやる必要がある。
00417033  |. 0F84 85000000  JE      TENSHOU.004170BE				; ⇒ nop(=90) が５つへ
*/

char cmdOnTenshouExeJumpFromWmMessage[6]="\xE9";


void WriteAsmJumperOnTenshouExeWmMessage() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeWmMessage;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeWmMessage + 5 )	;

	// ５というのは、0041702E  -E9 ????????  JMP TSMod.JmpOnTenshouExeWmMessage  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromWmMessage+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。


	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeWmMessage), cmdOnTenshouExeJumpFromWmMessage, 5, NULL); // 5バイトのみ書き込む
}
