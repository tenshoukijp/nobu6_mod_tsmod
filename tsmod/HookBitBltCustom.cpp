#include "WinTarget.h"
#include <fstream>

#include "DialogKahou.h"

using namespace std;

extern DialogKahou DK;

// 画像系のハック
BOOL Hook_BitBltCustom(
  HDC hdcDest, // コピー先デバイスコンテキストのハンドル
  int nXDest,  // コピー先長方形の左上隅の x 座標
  int nYDest,  // コピー先長方形の左上隅の y 座標
  int nWidth,  // コピー先長方形の幅
  int nHeight, // コピー先長方形の高さ
  HDC hdcSrc,  // コピー元デバイスコンテキストのハンドル
  int nXSrc,   // コピー元長方形の左上隅の x 座標
  int nYSrc,   // コピー元長方形の左上隅の y 座標
  DWORD dwRop  // ラスタオペレーションコード
  ) {

	return TRUE;
}
