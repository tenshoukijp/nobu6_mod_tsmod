// AntiDebug.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "AntiDebug.h"
#include "WinTarget.h"

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;								// 現在のインターフェイス
TCHAR szTitle[MAX_LOADSTRING];					// タイトル バーのテキスト
TCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

typedef int (WINAPI* _NtQueryInformationProcess)(
HANDLE ProcessHandle,
UINT ProcessInformationClass,
PVOID ProcessInformation,
ULONG ProcessInformationLength,
PULONG ReturnLength
);

//特殊な処理を行うので最適化を無効にする
#pragma optimize("", off)

/*
デバッグポートをチェックする、PhantOm pluginで回避可能
１が返るとデバッガ発見
*/
int AntiDebug1(void)
{
	int i;
	UINT x;
	_NtQueryInformationProcess nqip;

	nqip = (_NtQueryInformationProcess)GetProcAddress(GetModuleHandle("ntdll.dll"), "NtQueryInformationProcess");
	if(!nqip)
	{
		return 1;	
	}

	i = (int)nqip((HANDLE)-1, 7, &x, 4, 0);
	if(i)
	{
		return 1;	
	}

	if(x)
	{
		return 1;	
	}

	return 0;
}

typedef int (WINAPI* _NtSetInformationThread)(
HANDLE ThreadHandle,
int ThreadInformationClass,
PVOID ThreadInformation,
ULONG ThreadInformationLength
);

/*
自スレッドをデバッグ対象から外してデバッグできなくさせる
*/
void AntiDebug2(void)
{
	_NtSetInformationThread nsit;

	nsit = (_NtSetInformationThread)GetProcAddress(GetModuleHandle("ntdll.dll"), "NtSetInformationThread");
	if(!nsit)
	{
		return;	
	}

	nsit((HANDLE)-2, 0x11, 0, 0);
}

/*
CPUデバッグ用の例外して動作を見る
１が返るとデバッガ発見
*/

int AntiDebug3(void)
{
	int i;
	
	__try
	{
		_asm
		{
			//CPUデバッグ用の例外を発行
			int 0xF1
			//デバッガがある場合はそのまま処理が続行される
		}
		i = 1;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		//デバッガが無い場合はここで例外を受け取る
		//デバッガが有効な場合はここに来ない
		i = 0;
	}

	return i;
}

int AntiDebug4(void)
{
	int i;
		
	i = 1;

	__try
	{
		_asm
		{
			//例外を発行
			int 0x2D
			//nopは例外でスキップされる

			nop
			//デバッガがある場合はそのまま処理が続行される
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		//デバッガが無い場合はここで例外を受け取る
		//デバッガが有効な場合はここに来ない
		i = 0;
	}

	return i;
}


// 真を返す時は、デバッグハックを試みられているということ。
BOOL AntiDebugALL() {
	int i;
	
	i = AntiDebug1();
	if(i)
	{
		return 1;		
	}

	i = AntiDebug3();
	if(i)
	{
		return 1;		
	}

	i = AntiDebug4();
	if(i)
	{
		return 1;		
	}

	AntiDebug2();

	return 0;
}