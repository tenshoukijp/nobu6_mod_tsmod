#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

#include "FuncImagePngDialog.h"

//---- GDI+ヘッダ関連
#include <gdiplus.h>
#pragma comment (lib, "gdiplus.lib")        // リンク時に必要なライブラリ

using namespace Gdiplus;

static ULONG_PTR gdiplusToken;
static HDC hDC;
static int iPngLeft=-1, iPngTop=-1;

BOOL isModePngDialog = FALSE;

int StartPngDialog(char *szPngName, int posX, int posY) {

    // 文字コードをワイド文字列に変換
    Gdiplus::GdiplusStartupInput    gdiplusStartupInput;
 
    //---- GDI+の初期設定
    if( Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL) !=  Gdiplus::Ok ) {
        return FALSE;
    }

 	// ウィンドウのデバイスコンテキストハンドルを取得する
	hDC = GetDC( CommonGlobalHandle::tenshouWND );

	Graphics pngGraphics(hDC);

	WCHAR wc_szPngName[256];
	mbstowcs(wc_szPngName, szPngName, strlen(szPngName)+1);

	Image pngImage(wc_szPngName);
	if ( pngImage.GetLastStatus() != Ok ) {
		return FALSE;
	}

	// 真ん中を求めるため。
	RECT rc; // クライアント領域のサイズ
	::GetClientRect(CommonGlobalHandle::tenshouWND, &rc);

	int width = rc.right-rc.left;
	int height = rc.bottom-rc.top;

	iPngLeft = width/2 - pngImage.GetWidth()/2;
	iPngTop = height/2 - pngImage.GetHeight()/2;

	// 座標が指定されていれば上書き
	if ( posX != -1 ) {
		iPngLeft = posX;
	}
	if ( posY != -1 ) {
		iPngTop = posY;
	}

	pngGraphics.DrawImage(&pngImage, iPngLeft, iPngTop, pngImage.GetWidth(), pngImage.GetHeight());

	return TRUE;
}

void EndPngDialog() {
	//---- GDI+の解放
    Gdiplus::GdiplusShutdown(gdiplusToken);

	ReleaseDC( CommonGlobalHandle::tenshouWND, hDC );   // ウィンドウのデバイスコンテキストハンドルを解放する
}

void _WaitPngDialog(int iTime) {
	// マウスクリック待機
	DWORD preTimeGetTime = timeGetTime();
	// メッセージ速度によって、イメージダイアログが自動的に消える秒数が異なる。
	int RefleshTimeArray[] = { 300, 800, 2000, 4000, INT_MAX };
	int RefleshTime = RefleshTimeArray[ nb6settings.messagespd ]; // メッセージ速度０～４ ４＝∞

	// ユーザー指定されていれば、その値で上書き
	if ( iTime > 0 ) { 
		RefleshTime = iTime;
	}

	// ここでマウス入力待ち
	MSG msg;
	while(1) {
		BOOL ret = GetMessage( &msg, NULL, 0, 0 );  // メッセージを取得する

		// ゲーム終わり。
		if ( CommonGlobalHandle::isNobunaganoYabouGameEnd ) {
			break;
		}

		// マウスを押した。抜ける
		if ( msg.message == WM_LBUTTONDOWN || msg.message == WM_RBUTTONDOWN ) {
			break; // 抜ける
		}

		if ( iTime >= ImageDialogCloseTime::AutoClose ) { // AutoCloseもしくは、時間指定
			DWORD curTimeGetTime = timeGetTime();
			// 規定の間隔時間が過ぎた
			if ( int(curTimeGetTime - preTimeGetTime) > int(RefleshTime) ) {
				break;
			} 
		}

		// メッセージを処理する
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
}

int OpenPngDialog(char *szPngName, int posX, int posY, int iTime) {

	// まずは表示
	BOOL ret = StartPngDialog(szPngName, posX, posY);
	if ( ret == FALSE ) { return FALSE; }

	_WaitPngDialog(iTime);

	// 解放
	EndPngDialog();

	return TRUE;
}


// 外部参照用
int WINAPI Extern_OpenImageDialog(char *szPictureName, int posX, int posY, int iTime ) {
	return OpenPngDialog( szPictureName, posX, posY, iTime);
}
int WINAPI Extern_StartImageDialog(char *szPictureName, int posX, int posY ) {
	return StartPngDialog( szPictureName, posX, posY);
}
void WINAPI Extern_EndImageDialog() {
	EndPngDialog();
}
//---------------------------------------------------------------------------------------------------------------

/*
 リソースから読み込むタイプ
*/ 
 // リソースから画像を読み込む関数です
Gdiplus::Bitmap *LoadImageFromResource(
    HMODULE hinst,  // 読み込み元のインスタンスのハンドルです
    LPCTSTR pszName,  // 読み込むリソースの名前です
    LPCTSTR pszType   // 読み込むリソースの種類です
)
{
    // 指定されたリソースを探します
    HRSRC hRes = FindResource(hinst, pszName, pszType);
	if (hRes == NULL) {
        return NULL;
	}
    // リソースのサイズを取得します
    DWORD Size = SizeofResource(hinst, hRes);
	if (Size == 0) {
        return NULL;
	}

    // リソースを読み込みます
    HGLOBAL hData = LoadResource(hinst, hRes);
	if (hData == NULL) {
        return NULL;
	}
    const void *pData = LockResource(hData);
	if (pData == NULL) {
        return NULL;
	}

    // リソースのデータをコピーします
    HGLOBAL hBuffer = GlobalAlloc(GMEM_MOVEABLE, Size);
	if (hBuffer == NULL) {
        return NULL;
	}
    void *pBuffer = GlobalLock(hBuffer);
    if (pBuffer == NULL) {
        GlobalFree(hBuffer);
        return NULL;
    }
    CopyMemory(pBuffer, pData, Size);
    GlobalUnlock(hBuffer);

    // 画像を読み込みます
    IStream *pStream;
    if (CreateStreamOnHGlobal(hBuffer, TRUE, &pStream) != S_OK) {
        GlobalFree(hBuffer);
        return NULL;
    }
    Gdiplus::Bitmap *pPngImage = Gdiplus::Bitmap::FromStream(pStream);
    pStream->Release();

    return pPngImage;
}


static Gdiplus::Bitmap *pPngImage = NULL;
int StartPngResourceDialog(char *szResourceName, HMODULE hModule, int posX, int posY) {
    // 文字コードをワイド文字列に変換
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
 
    //---- GDI+の初期設定
    if( Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL) !=  Gdiplus::Ok ) {
        return FALSE;
    }

 	// ウィンドウのデバイスコンテキストハンドルを取得する
	hDC = GetDC( CommonGlobalHandle::tenshouWND );

	Graphics pngGraphics(hDC);

	Gdiplus::Bitmap *pPngImage = NULL;

    // 画像を読み込みます
    pPngImage = LoadImageFromResource(hModule, szResourceName, "IMAGE");
    if (pPngImage == NULL)
        return -1;  // 画像の読み込み失敗

	// 真ん中を求めるため。
	RECT rc; // クライアント領域のサイズ
	::GetClientRect(CommonGlobalHandle::tenshouWND, &rc);

	int width = rc.right-rc.left;
	int height = rc.bottom-rc.top;

	iPngLeft = width/2 - pPngImage->GetWidth()/2;
	iPngTop = height/2 - pPngImage->GetHeight()/2;

	// 座標が指定されていれば上書き
	if ( posX != -1 ) {
		iPngLeft = posX;
	}
	if ( posY != -1 ) {
		iPngTop = posY;
	}

	pngGraphics.DrawImage(pPngImage, iPngLeft, iPngTop, pPngImage->GetWidth(), pPngImage->GetHeight());

	return TRUE;
}

void EndPngResourceDialog() {
	if ( pPngImage ) {
		delete pPngImage;
	}
	//---- GDI+の解放
    Gdiplus::GdiplusShutdown(gdiplusToken);

	ReleaseDC( CommonGlobalHandle::tenshouWND, hDC );   // ウィンドウのデバイスコンテキストハンドルを解放する
}

int OpenPngResourceDialog(char *szResourceName, HMODULE hModule, int posX, int posY, int iTime) {

	// まずは表示
	BOOL ret = StartPngResourceDialog(szResourceName, hModule, posX, posY);
	if ( ret == FALSE ) { return FALSE; }

	_WaitPngDialog(iTime);

	// 解放
	EndPngResourceDialog();

	return TRUE;
}

// 外部参照用
int WINAPI Extern_OpenResourceImageDialog(char *szPictureName, HMODULE hModule, int posX, int posY, int iTime ) {
	return OpenPngResourceDialog( szPictureName, hModule, posX, posY, iTime);
}
int WINAPI Extern_StartResourceImageDialog(char *szPictureName, HMODULE hModule, int posX, int posY ) {
	return StartPngResourceDialog( szPictureName, hModule, posX, posY );
}
void WINAPI Extern_EndResourceImageDialog() {
	EndPngResourceDialog();
}

// 途中でゲーム等、終了した場合でも解放
BOOL FreeImageDialog() {
	EndPngDialog();
	EndPngResourceDialog();

	return TRUE;
}