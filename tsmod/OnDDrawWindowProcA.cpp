#include "WinTarget.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"
#include "ScenarioMod.h"
#include "DarkModeTitleBarMod.h"



// DDrawオリジナルハンドル
extern HINSTANCE hDDrawMod;

#define DDRAW_DLL_ENTRYPOINT_TO_WINPROC							 0x2DE0					// DDrawの先頭から、上記の「CMP EAX, 20(WM_SETCURSOR)」の次のジャンプに相当するアドレス(カーソル変更のため)

extern const char *DDRAW_DLL_FILENAME;





//-----------------------------------------------------------------------------------------------------------
int static uMsgOfDDrawDllWindowProcA = 0;
int static lParamOfDDrawDllWindowProcA = 0;
int static wParamOfDDrawDllWindowProcA = 0;
int static hWndOfDDrawDllWindowProcA = 0;


extern PFMDARKMODETITLEBAR_ONWMSIZE pDarkModeTitleBar_OnWmSize;

void OnTenshouExeDDrawDllWindowProcAExecute() {
	HWND hWnd = (HWND)hWndOfDDrawDllWindowProcA;
	UINT Msg = (UINT)uMsgOfDDrawDllWindowProcA;
	WPARAM wParam = (WPARAM)wParamOfDDrawDllWindowProcA;
	LPARAM lParam = (LPARAM)lParamOfDDrawDllWindowProcA;

	/*
	char szClass[512];
    GetClassName(  hWnd, szClass, sizeof(szClass) );    // クラス文字列の取得 → TenshoukiのメインウィンドウならhWndとなるはずだが･･･
	*/

	if ( Msg == WM_COMMAND ) {
	// ここは来る。

	} else if ( Msg == WM_SYSCOMMAND ) {
	// ここは来ない。

	} else if ( Msg == WM_MENUSELECT ) {
		// メニューを表示しない。
		if ( TSModCommand::Environment::cmdNoTitleBarAndMenuBar && CommonGlobalHandle::IsAfterScreenSizeSelected ) {
			// メニュー表示維持タイマーを上書きする。５秒維持
			SetTimer( CommonGlobalHandle::tenshouWND, TIMER_EVENT_ID_MENU_HIDE, 3000, TenshouMenuHide );
		}

	} else if ( Msg == WM_TIMER ) {
		// ここは来る。
		/*
		wParam
		wParamパラメータはタイマーIDです。
		lParam
		lParamパラメータは、タイマー登録時にSetTimer関数に渡されたアプリケーション定義のコールバック関数へのポインタです。
		*/

	} else if ( Msg == WM_PAINT ) {
		// ここは来る。
	}
	else if (Msg == WM_SIZE) {
		if (pDarkModeTitleBar_OnWmSize) {
			pDarkModeTitleBar_OnWmSize();
		}
		/*
		int iWidth = LOWORD(lParam); // 幅
		if (iWidth >= 780) {
			// MessageBox(NULL, "WM_SETTINGCHANGE", "WM_SETTINGCHANGE", NULL);
		}
		*/


		// ここは来る。

		// サイズ変更のタイプが指定されます。以下の値のいずれかになります。
		/*

		値 意味

		0 (SIZE_RESTORED) 		ウィンドウがサイズ変更されました。ただし最小化または最大化ではありません。
		1 (SIZE_MINIMIZED)		ウィンドウが最小化されました。
		2 (SIZE_MAXIMIZED)		ウィンドウが最大化されました。
		3 (SIZE_MAXSHOW) 		ある他のウィンドウが元のサイズに戻されたとき、すべてのポップアップウィンドウに送られます。
		4 (SIZE_MAXHIDE) 		ある他のウィンドウが最大化されたとき、すべてのポップアップウィンドウに送られます。

		WPARAM fwSizeType = wParam;   // パラメータの値。
		int iWidth  = LOWORD(lParam); // 幅
		int iHeight = HIWORD(lParam); // 高さ
		*/
	} else if ( Msg == WM_SETTINGCHANGE) {

	} else if ( Msg == WM_LBUTTONDOWN ) {
		// ここは来る
	}
}

/*
LRESULT Hook_DefWindowProcACustom(
	HWND hWnd,      // ウィンドウのハンドル
	UINT Msg,       // メッセージの識別子
	WPARAM wParam,  // メッセージの最初のパラメータ
	LPARAM lParam   // メッセージの 2 番目のパラメータ
)
*/

/*
52982DDF   53               PUSH    EBX


52982DE0   56               PUSH    ESI
52982DE1   8B75 10          MOV     ESI, DWORD PTR SS:[EBP+10]
52982DE4   57               PUSH    EDI
				└この３行を JMP TSMod.OnTSExeDDrawDllWindowProcA と書き換えてやる実際の処理
52982DE5   83F9 7E          CMP     ECX, 7E
*/
int pTenshouExeJumpFromToOnTenshouExeDDrawDllWindowProcA = NULL; // DDrawが読み込まれてから場所計算
int pTenshouExeReturnLblFromOnTenshouExeDDrawDllWindowProcA = NULL; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeDDrawDllWindowProcA() {
	// スタックにためておく
	__asm {

		// DDraw.dllに元々あった処理を記述
		PUSH ESI
		MOV ESI, DWORD PTR SS:[EBP+0x10]
		PUSH EDI

		mov wParamOfDDrawDllWindowProcA, ESI // wParamのコピー

		mov uMsgOfDDrawDllWindowProcA, ECX   // uMsgのコピー

		mov lParamOfDDrawDllWindowProcA, EDX // lParamのコピー

		mov EDX, DWORD PTR SS:[EBP+0x8]		// hWndをEDXに上書きする。
		mov hWndOfDDrawDllWindowProcA, EDX // hWndのコピー

		mov EDX, lParamOfDDrawDllWindowProcA // EDXの値を元へと戻す。

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnTenshouExeDDrawDllWindowProcAExecute();

	// スタックに引き出す
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		jmp pTenshouExeReturnLblFromOnTenshouExeDDrawDllWindowProcA
	}
}



char cmdOnTenshouExeJumpFromDDrawDllWindowProcA[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeDDrawDllWindowProcA() {

	pTenshouExeJumpFromToOnTenshouExeDDrawDllWindowProcA = (int)hDDrawMod + DDRAW_DLL_ENTRYPOINT_TO_WINPROC; // DDrawの先頭から、WinProcに相当する関数のアドレス
	pTenshouExeReturnLblFromOnTenshouExeDDrawDllWindowProcA = pTenshouExeJumpFromToOnTenshouExeDDrawDllWindowProcA + 5; // リターン先は５バイト先
	// カーソルを変更するのをやめる

	int iAddress = (int)OnTenshouExeDDrawDllWindowProcA;

	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeDDrawDllWindowProcA + 5 )	;

	// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeDDrawDllWindowProcA  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromDDrawDllWindowProcA+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeDDrawDllWindowProcA), cmdOnTenshouExeJumpFromDDrawDllWindowProcA, 5, NULL); //5バイトのみ書き込む
}

