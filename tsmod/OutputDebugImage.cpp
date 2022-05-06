#include "WinTarget.h"
#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"

#include <gdiplus.h>
using namespace Gdiplus;


HBITMAP CreateWindowBitmap(HWND hWnd)
{
    HBITMAP hbmNew;
    HDC hDC, hMemDC;
    HGDIOBJ hbmOld;
    RECT rc;
    SIZE siz;
    DWORD dwError = 0;
 
    GetWindowRect(hWnd, &rc);
    siz.cx = rc.right - rc.left;
    siz.cy = rc.bottom - rc.top;
 
    hbmNew = NULL;
    hDC = GetWindowDC(hWnd);
    if (hDC != NULL)
    {
        hMemDC = CreateCompatibleDC(hDC);
        if (hMemDC != NULL)
        {
            hbmNew = CreateCompatibleBitmap(hDC, siz.cx, siz.cy);
 
            if (hbmNew != NULL)
            {
                hbmOld = SelectObject(hMemDC, hbmNew);
                BitBlt(hMemDC, 0, 0, siz.cx, siz.cy,
                       hDC, 0, 0, SRCCOPY);
                SelectObject(hMemDC, hbmOld);
            }
            else
                dwError = GetLastError();
            DeleteDC(hMemDC);
        }
        else
            dwError = GetLastError();
        ReleaseDC(hWnd, hDC);
        SetLastError(dwError);
    }
 
    return hbmNew;
}


int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    UINT  num = 0, size = 0;

    GetImageEncodersSize(&num, &size);
    if (size == 0)
        return -1;  // Failure

    ImageCodecInfo* pImageCodecInfo = (ImageCodecInfo*)malloc(size);
    if (pImageCodecInfo == NULL)
        return -1;  // Failure

    GetImageEncoders(num, size, pImageCodecInfo);

    for(UINT j = 0; j < num; ++j)
    {
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
        {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;  // Success
        }
    }

    free(pImageCodecInfo);
    return -1;  // Failure
}

BOOL SaveBitmapAsPngFile(LPCSTR pszFileName, HBITMAP hbm)
{
    WCHAR wszFileName[MAX_PATH];
    MultiByteToWideChar(CP_ACP, 0, pszFileName, -1, wszFileName, MAX_PATH);

    CLSID clsid;
    Bitmap b(hbm, (HPALETTE)GetStockObject(DEFAULT_PALETTE));
    if (GetEncoderClsid(L"image/png", &clsid) >= 0)
    {
        b.Save(wszFileName, &clsid, NULL);
        return TRUE;
    }
    return FALSE;
}

BOOL SaveBitmapAsJpegFile(LPCSTR pszFileName, HBITMAP hbm)
{
    WCHAR wszFileName[MAX_PATH];
    MultiByteToWideChar(CP_ACP, 0, pszFileName, -1, wszFileName, MAX_PATH);

    CLSID clsid;
    Bitmap b(hbm, (HPALETTE)GetStockObject(DEFAULT_PALETTE));
    if (GetEncoderClsid(L"image/jpeg", &clsid) >= 0)
    {
        b.Save(wszFileName, &clsid, NULL);
        return TRUE;
    }
    return FALSE;
}

BOOL SaveBitmapAsBmpFile(LPCSTR pszFileName, HBITMAP hbm)
{
    WCHAR wszFileName[MAX_PATH];
    MultiByteToWideChar(CP_ACP, 0, pszFileName, -1, wszFileName, MAX_PATH);

    CLSID clsid;
    Bitmap b(hbm, (HPALETTE)GetStockObject(DEFAULT_PALETTE));
    if (GetEncoderClsid(L"image/bmp", &clsid) >= 0)
    {
        b.Save(wszFileName, &clsid, NULL);
        return TRUE;
    }
    return FALSE;
}
//------------------------------------------------





// クライアント矩形を画像へと保存
void OutputDebugImage(char *szFileName) {

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
    
	// GDI+の初期化。
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    
	HBITMAP hbm = CreateWindowBitmap(CommonGlobalHandle::tenshouWND);
	if ( OnigMatch( szFileName, "\\.png" ) ) {
		SaveBitmapAsPngFile(szFileName, hbm);
	}
	else if ( OnigMatch( szFileName, "\\.jpeg$|\\.jpg$" ) ) {
		SaveBitmapAsJpegFile(szFileName, hbm);
	}
	else {
		SaveBitmapAsBmpFile(szFileName, hbm);
	}
	DeleteObject(hbm);
    
	// GDI+の後処理。
	GdiplusShutdown(gdiplusToken);
}
