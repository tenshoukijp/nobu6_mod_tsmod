#pragma pack(1)
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>

// C ランタイム ヘッダー ファイル
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <Tlhelp32.h>
#include <mmsystem.h>

// 指定のファイル名のプロセス名が何個実行されいるかを得る
int GetTargetProcessCount(char* process) {
	int iMainReturnFlag = 0;

	HANDLE hthSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hthSnapshot == INVALID_HANDLE_VALUE) {
		return -1;
	}

	PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };
	BOOL bProcess = Process32First(hthSnapshot, &pe);

	for (; bProcess; bProcess = Process32Next(hthSnapshot, &pe)) {
		/*
		printf("PROCESSENTRY32.dwSize              = %d\n", pe.dwSize);
		printf("PROCESSENTRY32.cntUsage            = %d\n", pe.cntUsage);
		printf("PROCESSENTRY32.th32ProcessID       = %d\n", pe.th32ProcessID);
		printf("PROCESSENTRY32.th32DefaultHeapID   = %p\n", pe.th32DefaultHeapID);
		printf("PROCESSENTRY32.th32ModuleID        = %d\n", pe.th32ModuleID); 
		printf("PROCESSENTRY32.cntThreads          = %d\n", pe.cntThreads ); 
		printf("PROCESSENTRY32.th32ParentProcessID = %d\n", pe.th32ParentProcessID ); 
		printf("PROCESSENTRY32.pcPriClassBase      = %d\n", pe.pcPriClassBase ); 
		printf("PROCESSENTRY32.dwFlags             = 0x%.8x\n", pe.dwFlags ); 
		printf("PROCESSENTRY32.szExeFile           = %S\n\n", pe.szExeFile );
		*/
		if ( strcmp( pe.szExeFile, process ) == 0) {
			iMainReturnFlag++;
		}
	}

	CloseHandle(hthSnapshot);

	return iMainReturnFlag;
}




//------------------------------------------------
// プロセスID ⇒ HWND への変換
// 引数:プロセスID
// 戻り値: HWND もしくは NULL
//------------------------------------------------
HWND GetWindowHandleFromProcessId( const DWORD TargetID) {
	HWND hWnd = GetTopWindow(NULL);
	do {
		if(GetWindowLong( hWnd, GWL_HWNDPARENT) != 0 || !IsWindowVisible(hWnd)) {
			continue;
		}
		DWORD ProcessID;
		GetWindowThreadProcessId( hWnd, &ProcessID);
		if(TargetID == ProcessID) {
			return hWnd;
		}
	} while((hWnd = GetNextWindow( hWnd, GW_HWNDNEXT)) != NULL);

	return NULL;
}


//------------------------------------------------
// 現在のウィンドウハンドルを得る
//------------------------------------------------
HWND GetCurrentWindowHandle() {
	// 実行中のプロセスIDを得る
	DWORD pID = GetCurrentProcessId();
	// プロセスID⇒ウィンドウハンドル
	HWND hWnd = GetWindowHandleFromProcessId(pID);
	return hWnd;
}
