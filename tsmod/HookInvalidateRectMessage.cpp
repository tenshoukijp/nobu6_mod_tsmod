#include "WinTarget.h"


BOOL Hook_InvalidateRectMessage(
  HWND hWnd,           // �E�B���h�E�̃n���h��
  CONST RECT *lpRect,  // �����`�̍��W
  BOOL bErase          // �������邩�ǂ����̏��
)
{
	return TRUE;
}
