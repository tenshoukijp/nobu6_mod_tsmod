#include "WinTarget.h"

#include <shlwapi.h>
#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSMod.h"
#include "ScenarioMod.h"

#pragma comment(lib, "shlwapi.lib")

/*
79A32533   6A 40            PUSH 40
79A32535   68 E029A979      PUSH DDraw.79A929E0                      ; ASCII "20"
79A3253A   56               PUSH ESI
79A3253B   E8 301F0000      CALL DDraw.79A34470
79A32540   68 E029A979      PUSH DDraw.79A929E0                      ; ASCII "20"
79A32545   68 00100000      PUSH 1000
79A3254A   68 6042037A      PUSH DDraw.7A034260                      ; ASCII "kao_24bit\0020.bmp"
79A3254F   E8 AE1F0000      CALL DDraw.79A34502
79A32554   68 2CF3A379      PUSH DDraw.79A3F32C                      ; ASCII ".bmp"
79A32559   68 00100000      PUSH 1000
79A3255E   68 6042037A      PUSH DDraw.7A034260                      ; ASCII "kao_24bit\0020.bmp"
79A32563   E8 9A1F0000      CALL DDraw.79A34502
79A32568   83C4 28          ADD ESP,28
79A3256B   6A 00            PUSH 0
79A3256D   68 80000000      PUSH 80
79A32572   6A 03            PUSH 3
79A32574   6A 00            PUSH 0
79A32576   6A 00            PUSH 0
79A32578   68 00000080      PUSH 80000000                                  ←この処理の後、メモリ上を書き換えてしまえばよい。
79A3257D   68 6042037A      PUSH DDraw.7A034260                      ; ASCII "kao_24bit\0020.bmp"
79A32582   FF15 88E0A379    CALL NEAR DWORD PTR DS:[<&KERNEL32.Creat>; KERNEL32.CreateFileA

*/


#define DDRAW_DLL_ENTRYPOINT_TO_PUSH_80000000						 0x2578					// DDrawの先頭から、上記の「79A32578   68 00000080      PUSH 80000000」の先頭相当するアドレス
#define DDRAW_DLL_FILENAME_ADDRESS_RELATIVE									(0x7A034260-0x79A30000)   // ファイル名が格納されているアドレス

// DDrawオリジナルハンドル
extern HINSTANCE hDDrawMod;

void OnDrawKaoFullColorFileNameExecute() {
	// OutputDebugString("顔グラ決定時");
	int nFileNameAddress = (int)hDDrawMod + DDRAW_DLL_FILENAME_ADDRESS_RELATIVE;
	char *szFileName = (char *)nFileNameAddress;
	char overrideFileNameBuffer[MAX_PATH] = "";
	if (p_snOnRequestFullcolorFaceFileName) {
		char szLowerFileName[MAX_PATH] = "";
		strcpy(szLowerFileName, szFileName);
		_strlwr(szLowerFileName);

		p_snOnRequestFullcolorFaceFileName(szLowerFileName, overrideFileNameBuffer);

		// 有効な文字が入っていたら、コピー
		if (strlen(overrideFileNameBuffer)) {
			if (PathFileExists(overrideFileNameBuffer)) {
				strncpy(szFileName, overrideFileNameBuffer, 98);
				szFileName[99] = '\0';
			}
		}
	}

	// OutputDebugStream("%s", (int)hDDrawMod + DDRAW_DLL_FILENAME_ADDRESS_RELATIVE);
}


int pDDrawJumpFromToOnDrawKaoFullColorFileName = NULL;
int pDDrawReturnLblFromOnDrawKaoFullColorFileName = NULL; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w

__declspec(naked) void WINAPI OnDrawKaoFullColorFileName() {
	// スタックにためておく
	__asm {

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnDrawKaoFullColorFileNameExecute();

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

		push 0x80000000 // 元の処理をここにそのまま

		jmp pDDrawReturnLblFromOnDrawKaoFullColorFileName
	}
}


char cmdOnDDrawJumpFromwKaoFullColorFileName[6] = "\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteDDrawDrawKaoFullColorFileName() {
	pDDrawJumpFromToOnDrawKaoFullColorFileName = (int)hDDrawMod + DDRAW_DLL_ENTRYPOINT_TO_PUSH_80000000; // DDrawの先頭から、上記の「79A32578   68 00000080      PUSH 80000000」の先頭相当するアドレス
	pDDrawReturnLblFromOnDrawKaoFullColorFileName = pDDrawJumpFromToOnDrawKaoFullColorFileName + 5; // １行分の命令の次の場所が返る場所

	// まずアドレスを数字として扱う
	int iAddress = (int)OnDrawKaoFullColorFileName;
	int SubAddress = iAddress - (pDDrawJumpFromToOnDrawKaoFullColorFileName + 5);
	// ５というのは、
	memcpy(cmdOnDDrawJumpFromwKaoFullColorFileName + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory(CommonGlobalHandle::curProcess, (LPVOID)(pDDrawJumpFromToOnDrawKaoFullColorFileName), cmdOnDDrawJumpFromwKaoFullColorFileName, 5, NULL); //5バイトのみ書き込む
}
