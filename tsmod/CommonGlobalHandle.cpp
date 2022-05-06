#include "CommonGlobalHandle.h"

HANDLE CommonGlobalHandle::hTenshouExe = NULL;
char CommonGlobalHandle::szTenshouExeFullPath[MAX_PATH] = "";
HANDLE CommonGlobalHandle::hDLLModule = NULL;
HWND CommonGlobalHandle::tenshouWND = NULL;
HANDLE CommonGlobalHandle::curProcess = NULL;
DWORD CommonGlobalHandle::tenshouThreadId = 0;
HDC CommonGlobalHandle::hDC = NULL;
BOOL CommonGlobalHandle::isNobunaganoYabouGameStart = false;
BOOL CommonGlobalHandle::isMainGameStart = false;
BOOL CommonGlobalHandle::isModeScenario = false;
HMODULE CommonGlobalHandle::hResourceWavMod = NULL;
HMODULE CommonGlobalHandle::hResourceWavMod2 = NULL;
HMODULE CommonGlobalHandle::hResourceWavMod3 = NULL;
BOOL CommonGlobalHandle::isNobunaganoYabouGameEnd = false;
int CommonGlobalHandle::isBattleMode = 0;
int CommonGlobalHandle::isPlayerMainScreenInputWait = 0; // メイン画面でプレイヤが城ウィンドウと軍団ウィンドウを出して、コマンド入力可能な状態。
HMENU CommonGlobalHandle::hMenu = NULL;
HMENU CommonGlobalHandle::hScenarioModPopMenu = NULL;
BOOL CommonGlobalHandle::isShiftKey = false;
BOOL CommonGlobalHandle::isNoDiskMode = false;
BOOL CommonGlobalHandle::isExistDisk = true;
BOOL CommonGlobalHandle::isEndingMode = false;
BOOL CommonGlobalHandle::isEndingModeAfter = false;
HDC CommonGlobalHandle::curHDC = NULL;
BOOL CommonGlobalHandle::IsAfterScreenSizeSelected = FALSE;
HWND CommonGlobalHandle::hWndTaskBar = NULL;


// 起動時のオプション構造体
RECT CommonGlobalHandle::latestCenterWindowRect = {0};
/*
typedef struct _STARTUPINFO {
    DWORD  cb;              // 構造体サイズ
    LPTSTR  lpReserved;     // 予約(0)
    LPTSTR  lpDesktop;      // デスクトップ
    LPTSTR  lpTitle;        // ウィンドウタイトル
    DWORD  dwX;             // x位置
    DWORD  dwY;             // y位置
    DWORD  dwXSize;         // xサイズ
    DWORD  dwYSize;         // yサイズ
    DWORD  dwXCountChars;   // 文字幅
    DWORD  dwYCountChars;   // 文字高
    DWORD  dwFillAttribute; // 文字色・背景色
    DWORD  dwFlags;         // フラグ
    WORD   wShowWindow;     // ウィンドウ表示形式 WinMain関数の引数である「nCmdShow」と同じ値
    WORD   cbReserved2;     // 予約(0)
    LPBYTE lpReserved2;     // 予約(0)
    HANDLE hStdInput;       // 標準入力
    HANDLE hStdOutput;      // 標準出力
    HANDLE hStdError;       // 標準エラー出力
} STARTUPINFO, *LPSTARTUPINFO;
*/

int WINAPI Extern_IsBattleMode() {
	return CommonGlobalHandle::isBattleMode;
}