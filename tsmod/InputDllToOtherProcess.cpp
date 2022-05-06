#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

// 「なんでUNICODEやねん！」という突っ込み以外はとくに問題ないと思います(^^;　
#define UNICODE
#define _UNICODE

#include "WinTarget.h"
#include <tlhelp32.h>
#include <tchar.h>

// tlhelp32.hはプロセスの検索を行うために使う関数の定義なのですが、詳しくはのちほど説明します。
// TARGET_EXE_NAMEはDLLを注入するプロセスの名前、DLL_FILE_NAMEは注入するDLLのファイル名です。DLLはexeファイルと同じフォルダにいれなければなりません。
#define TARGET_EXE_NAME  "IEXPLORE.EXE"
#define DLL_FILE_NAME    "ClipBoard.dll"

/*
int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
*/
int InputDllToOtherProcess()
{
	// さっそくプロセスの検索を行います。
	// CreateToolhelp32Snapshotはプロセスの列挙をおこなうタスクマネージャー的なプログラムを作る際に利用されるプログラムで、
	// たいていの場合Process32FirstやProcess32Nextとセットにして使い、PROCESSENTRY32構造体のpeにプロセスの情報が入ります。
    HANDLE hSnap;
    if((hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)) == INVALID_HANDLE_VALUE){
        MessageBox(NULL, _TEXT("CreateToolhelp32Snapshot"), _TEXT("Error"), MB_OK);
        return -1;
    }

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(pe);

    DWORD dwProcessId = 0;
    BOOL bResult = Process32First(hSnap, &pe);
    while(bResult){
        if(!lstrcmp(pe.szExeFile, _TEXT(TARGET_EXE_NAME))){
            dwProcessId = pe.th32ProcessID;
            break;
        }
        bResult = Process32Next(hSnap, &pe);
    }
    CloseHandle(hSnap);

    if(dwProcessId == 0){
        MessageBox(NULL, _TEXT("Process not found."), _TEXT("ERROR"), MB_OK);
        return -1;
    }

	// これはプロセスを開く関数です（みればわかりますが）。
	// プロセスIDを渡せばそのプロセスを開いてくれるので、
	// さっきProcess32Nextなどを使って検索してきたTARGET_EXE_NAMEのプロセスID（つまりDLLを注入するターゲットのプロセスID）を渡しています。
	// 戻り値はそのプロセスのハンドルです。
    HANDLE hProcess;
    hProcess = OpenProcess(
         PROCESS_QUERY_INFORMATION |
         PROCESS_CREATE_THREAD     |
         PROCESS_VM_OPERATION      |
         PROCESS_VM_WRITE,
         FALSE, dwProcessId);

    if(hProcess == NULL){
        MessageBox(NULL, _TEXT("OpenProcess"), _TEXT("ERROR"), MB_OK);
        return -1;
    }

	// DLLのパスを作成しています。
	// まずGetModuleFileNameで自分自身（つまりexeファイル）の絶対パスを取得して、
	//その文字列の後ろから探して最初に「\」が発見されたところから以後DLLファイルの名前に置き換えています。
	// つまり自分と同じフォルダにあるDLLの絶対パスを作成したわけです。
    TCHAR szLibFile[256];
    GetModuleFileName(NULL, szLibFile, sizeof(szLibFile));
    _tcscpy(_tcsrchr(szLibFile, _TEXT('\\')) + 1, _TEXT(DLL_FILE_NAME));

    int szLibFileLen;
    szLibFileLen = lstrlen(szLibFile) + 1;
    szLibFileLen = szLibFileLen * sizeof(TCHAR);

	// まずVirtualAllocExでターゲットプロセスにメモリ空間を作成しています。
	// これはDLLの絶対パスを入れるメモリ空間です。
	// そしてWriteProcessMemoryでDLLの絶対パスを書き込んでいます。
	// するとVirtualAllocExの戻り値がDLLの絶対パスの位置（ポインタ）ということになるので、これをCreateRemoteThreadの引数として渡します。
	// 要するに別プロセス内にメモリを確保して、確保した先頭アドレスを返す
    PWSTR RemoteProcessMemory;
    RemoteProcessMemory = (PWSTR)VirtualAllocEx(
        hProcess, NULL, szLibFileLen, MEM_COMMIT, PAGE_READWRITE);
    if(RemoteProcessMemory == NULL){
        MessageBox(NULL, _TEXT("VirtualAllocEx"), _TEXT("ERROR"), MB_OK);
        return -1;
    }

	// そこに、DLLのフルネームを書き込む
    if(WriteProcessMemory(hProcess, RemoteProcessMemory,
        (PVOID)szLibFile, szLibFileLen, NULL) == 0){
            MessageBox(NULL, _TEXT("WriteProcessMemory"), _TEXT("ERROR"), MB_OK);
            return -1;
    }

	// GetProcAddressはKernel32.dllからLoadLibraryW（UNICODE用）を探してきてそのアドレスを返しています。
	// これはUNICODE用のソースコードですのでLoadLibraryWですが、ASCIIならLoadLibraryAを利用してください。
	// Windows2000/XPはLoadLibraryAを使っても内部的にUNICODEに変換されて結局LoadLibraryWを呼んでいるようなので、
	// UNICODEかASCIIかで特に気にする必要はないでしょう。
    PTHREAD_START_ROUTINE pfnThreadRtn;
    pfnThreadRtn = (PTHREAD_START_ROUTINE)GetProcAddress(
        GetModuleHandle(_TEXT("Kernel32")), "LoadLibraryW");
    if (pfnThreadRtn == NULL){
        MessageBox(NULL, _TEXT("GetProcAddress"), _TEXT("ERROR"), MB_OK);
        return -1;
    }


	// ついにCreateRemoteThreadが呼ばれました。
	// 第一引数はOpenProcessの戻り値であるプロセスのハンドルです。
	// 第四引数はLoadLibraryWのアドレス。第五引数はDLL絶対パスのアドレス。
	// これでCreateRemoteThreadを呼び出せばターゲットのプロセスでLoadLibraryWスレッドが生成され、
	// 見事、任意のDLLがターゲットプロセスにマッピングされることになります。
	// そして、このプログラム（in_dll.exe）は終了します。このプログラムにDLLがマッピングされているならば、
	// 終了と同時にDLL自体も終了します。しかし別のプロセスにマッピングさせたならば自分自身が終了してもDLLの処理は生き続けます。IEXPLORE.EXEならばIEが終了するまで生き続けますし、explorer.exeといったWindowsの動作に不可欠なプロセスなどにマッピングさせたならばWindowsの終了まで生き続けることになります。
    HANDLE hThread;
    hThread = CreateRemoteThread(hProcess, NULL, 0, 
        pfnThreadRtn, RemoteProcessMemory, 0, NULL);
    if (hThread == NULL){
        MessageBox(NULL, _TEXT("CreateRemoteThread"), _TEXT("ERROR"), MB_OK);
        return -1;
    }

    Sleep(5000);
    return 0;
}
