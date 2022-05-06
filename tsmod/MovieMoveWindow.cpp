#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <vfw.h>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"

// 簡易版のムービーヘッダ構造
struct MY_AVIHEADER {
	int dummy[16];
	int dwWidth;
	int dwHeight;
};

BOOL GetAviHeader(LPTSTR lpszFileName, MainAVIHeader *lpAviHeader, AVIStreamHeader *lpVideoStreamHeader, LPBITMAPINFO lpbm, AVIStreamHeader *lpAudioStreamHeader, LPWAVEFORMAT lpwf);
BOOL GetStreamHeader(HMMIO hmmio, AVIStreamHeader *lpStreamHeader, LPVOID lpFormat);

BOOL GetAviHeader(LPTSTR lpszFileName, MainAVIHeader *lpAviHeader, AVIStreamHeader *lpVideoStreamHeader, LPBITMAPINFO lpbmInfo, AVIStreamHeader *lpAudioStreamHeader, LPWAVEFORMAT lpwf)
{
	HMMIO    hmmio;
	MMCKINFO mmckRiff;
	MMCKINFO mmckList;
	MMCKINFO mmckAvih;
	
	hmmio = mmioOpen(lpszFileName, NULL, MMIO_READ);
	if (hmmio == NULL) {
		// MessageBox(NULL, TEXT("ファイルのオープンに失敗しました。"), NULL, MB_ICONWARNING);
		return FALSE;
	}
	
	mmckRiff.fccType = formtypeAVI;
	if (mmioDescend(hmmio, &mmckRiff, NULL, MMIO_FINDRIFF) != MMSYSERR_NOERROR) {
		// MessageBox(NULL, TEXT("AVIファイルではありません。"), NULL, MB_ICONWARNING);
		mmioClose(hmmio, 0);
		return FALSE;
	}

	mmckList.fccType = listtypeAVIHEADER;
	if (mmioDescend(hmmio, &mmckList, NULL, MMIO_FINDLIST) != MMSYSERR_NOERROR) {
		mmioClose(hmmio, 0);
		return FALSE;
	}

	if (lpAviHeader != NULL) {
		mmckAvih.ckid = ckidAVIMAINHDR;
		if (mmioDescend(hmmio, &mmckAvih, NULL, MMIO_FINDCHUNK) != MMSYSERR_NOERROR) {
			mmioClose(hmmio, 0);
			return FALSE;
		}

		mmioRead(hmmio, (HPSTR)lpAviHeader, mmckAvih.cksize);
		mmioAscend(hmmio, &mmckAvih, 0);
	}

	if (!GetStreamHeader(hmmio, lpVideoStreamHeader, lpbmInfo)) {
		mmioClose(hmmio, 0);
		return FALSE;
	}
	
	if (!GetStreamHeader(hmmio, lpAudioStreamHeader, lpwf)) {
		mmioClose(hmmio, 0);
		return FALSE;
	}
	
	mmioAscend(hmmio, &mmckList, 0);
	mmioAscend(hmmio, &mmckRiff, 0);

	return TRUE;
}

BOOL GetStreamHeader(HMMIO hmmio, AVIStreamHeader *lpStreamHeader, LPVOID lpFormat)
{
	MMCKINFO mmckList;
	MMCKINFO mmckStrh;
	MMCKINFO mmckStrf;

	mmckList.fccType = listtypeSTREAMHEADER;
	if (mmioDescend(hmmio, &mmckList, NULL, MMIO_FINDLIST) != MMSYSERR_NOERROR)
		return FALSE;

	if (lpStreamHeader != NULL) {
		mmckStrh.ckid = ckidSTREAMHEADER;
		if (mmioDescend(hmmio, &mmckStrh, NULL, MMIO_FINDCHUNK) != MMSYSERR_NOERROR) {
			mmioAscend(hmmio, &mmckList, 0);
			return FALSE;
		}

		mmioRead(hmmio, (HPSTR)lpStreamHeader, mmckStrh.cksize);
		mmioAscend(hmmio, &mmckStrh, 0);
	}

	if (lpFormat != NULL) {
		mmckStrf.ckid = ckidSTREAMFORMAT;
		if (mmioDescend(hmmio, &mmckStrf, NULL, MMIO_FINDCHUNK) != MMSYSERR_NOERROR) {
			mmioAscend(hmmio, &mmckList, 0);
			return FALSE;
		}

		mmioRead(hmmio, (HPSTR)lpFormat, mmckStrf.cksize);
		mmioAscend(hmmio, &mmckStrf, 0);
	}

	mmioAscend(hmmio, &mmckList, 0);

	return TRUE;
}




int iMovieMoveWindow_X = 160;		// 一応最初デフォルトで
int iMovieMoveWindow_Y = 120;		// 一応最初デフォルトで
int iMovieMoveWindow_Width  = 320;  // 一応最初デフォルトで
int iMovieMoveWindow_Height = 240;  // 一応最初デフォルトで
HWND *hWndMovieMove = NULL;

int iEBP_OF_MovieMoveWindowExecute; // EBPの値。ここに、描画すべきムービー用ウィンドウのＸ,Ｙ,Ｗ,Ｈの情報が詰まっている。

void OnTenshouExeMovieMoveWindowExecute() {

	// EBP をベースに、ムービーの縦サイズと横サイズや、Ｘ，Ｙを得る
	int *pEBP = (int *)iEBP_OF_MovieMoveWindowExecute;
	iMovieMoveWindow_Height = *(pEBP-1);
	iMovieMoveWindow_Width  = *(pEBP-2);
	iMovieMoveWindow_Y  = *(pEBP-3);
	iMovieMoveWindow_X  = *(pEBP-4);

	// その値を計算すると、クライアントの幅と高さが安全に得られる
	int iClientWidth  = iMovieMoveWindow_Width  + iMovieMoveWindow_X * 2;
	int iClientHeight = iMovieMoveWindow_Height + iMovieMoveWindow_Y * 2;

	// ムービーのサイズがデフォルトのサイズであれば、処理を実行する
	if ( iMovieMoveWindow_Width == 320 && iMovieMoveWindow_Height == 240 ) {

		// ムービーウィンドウのウィンドウハンドルを得る。
		hWndMovieMove = (HWND *)0x4DFD90;

		// ムービーウィンドウのタイトルテキストを得る
		char szTitleName[256] = "";
		GetWindowText( *hWndMovieMove, szTitleName, sizeof(szTitleName) );

		char szFullMovieName[256] = "movie\\";
		strcat_s( szFullMovieName, szTitleName );

		struct stat st;
		// そのムービーはハードディスクに存在するか
		if (stat(szFullMovieName, &st) == 0) {

			FILE *fp = fopen(szFullMovieName, "r");

			if ( fp ) {

				MainAVIHeader   aviHeader;
				AVIStreamHeader videoStreamHeader;
				AVIStreamHeader audioStreamHeader;
				BITMAPINFO      bmInfo;
				WAVEFORMAT      wf;


				// ムービーがバグっていることがあるのでエラー処理
				if (GetAviHeader(szFullMovieName, &aviHeader, &videoStreamHeader, &bmInfo, &audioStreamHeader, &wf) ) {

					// ムービーのヘッダー情報から、幅と高さを得る
					int iMovieHeight = aviHeader.dwHeight;
					int iMovieWidgh  = aviHeader.dwWidth;

					// XとYの位置を計算する。
					int iX = (iClientWidth - iMovieWidgh)/2;
					int iY = (iClientHeight- iMovieHeight)/2;

					// それを元々のEBPの場所へと反映し、今回のムービーサイズ、再生位置とする。
					*(pEBP-1) = iMovieHeight;
					*(pEBP-2) = iMovieWidgh;
					*(pEBP-3) = iY;
					*(pEBP-4) = iX;
				} else {
					// エラーの場合は、天翔記既定のサイズとする
					int iMovieHeight = 240;
					int iMovieWidgh  = 320;

					// XとYの位置を計算する。
					int iX = (iClientWidth - iMovieWidgh)/2;
					int iY = (iClientHeight- iMovieHeight)/2;

					// それを元々のEBPの場所へと反映し、今回のムービーサイズ、再生位置とする。
					*(pEBP-1) = iMovieHeight;
					*(pEBP-2) = iMovieWidgh;
					*(pEBP-3) = iY;
					*(pEBP-4) = iX;
				}

			}

		}
	}
}


/*
00変格  |> 6A 01          PUSH    1                                        ; /Repaint = TRUE
0045320A  |. FF75 FC        PUSH    DWORD PTR SS:[EBP-4]                     ; |Height
0045320D  |. FF75 F8        PUSH    DWORD PTR SS:[EBP-8]                     ; |Width
00453210  |. FF75 F4        PUSH    DWORD PTR SS:[EBP-C]                     ; |Y
00453213  |. FF75 F0        PUSH    DWORD PTR SS:[EBP-10]                    ; |X
00453216  |. FF35 90FD4D00  PUSH    DWORD PTR DS:[4DFD90]                    ; |hWnd = 000420E6 ('SN4_OPENING.avi',class='AVIWnd32',parent=000620DA)
				└ここを JMP TSMod.OnTSExeMovieMoveWindow と書き換えてやる実際の処理
0045321C  |. FF15 A8754E00  CALL    DWORD PTR DS:[<&USER32.MoveWindow>]      ; \MoveWindow
00453222  |. 6A 05          PUSH    5                                        ; /ShowState = SW_SHOW
*/
int pTenshouExeJumpFromToOnTenshouExeMovieMoveWindow	  =0x453208; // 関数はこのアドレスから、OnTenshouExeMovieMoveWindowへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeMovieMoveWindow	  =0x45321C; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeMovieMoveWindow() {
	// スタックにためておく
	__asm {

		// ムービーの元の再生の位置や大きさをコピー。通常は320,240の値、ムービー２倍サイズ再生時は640,480の値となる。
		mov iEBP_OF_MovieMoveWindowExecute, EBP;

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	OnTenshouExeMovieMoveWindowExecute();

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

		
		push    1                                          //; /Repaint = TRUE
		push    DWORD PTR SS:[EBP-0x4]                     //; |Height
		push    DWORD PTR SS:[EBP-0x8]                     //; |Width
		push    DWORD PTR SS:[EBP-0xC]                     //; |Y
		push    DWORD PTR SS:[EBP-0x10]                    //; |X
		push    DWORD PTR DS:[0x4DFD90]                    //; |hWnd = 000420E6 ('********.avi',class='AVIWnd32',parent=000620DA)

		jmp pTenshouExeReturnLblFromOnTenshouExeMovieMoveWindow
	}
}


char cmdOnTenshouExeJumpFromMovieMoveWindow[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeMovieMoveWindow() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeMovieMoveWindow;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeMovieMoveWindow + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeMovieMoveWindow  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromMovieMoveWindow+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeMovieMoveWindow), cmdOnTenshouExeJumpFromMovieMoveWindow, 5, NULL); //5バイトのみ書き込む
}




