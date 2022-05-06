#include "WinTarget.h"


BOOL Hook_InvalidateRectMessage(
  HWND hWnd,           // ウィンドウのハンドル
  CONST RECT *lpRect,  // 長方形の座標
  BOOL bErase          // 消去するかどうかの状態
)
{
	return TRUE;
}
