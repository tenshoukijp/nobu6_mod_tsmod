#include <windows.h>
#include <windowsx.h>

#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"

LRESULT OnMouseEventHandler( HWND hWnd,      // �E�B���h�E�̃n���h��
							 UINT Msg,       // ���b�Z�[�W�̎��ʎq
							 WPARAM wParam,  // ���b�Z�[�W�̍ŏ��̃p�����[�^
							 LPARAM lParam   // ���b�Z�[�W�� 2 �Ԗڂ̃p�����[�^
)
{
	// �}�E�X�����͂���
	// OutputDebugStream("%d, %d, %ld", Msg, wParam, lParam);
	// int x = GET_X_LPARAM(lParam);
	// int y = GET_Y_LPARAM(lParam);

	// �}�E�X�C�x���g�̗�
	switch (Msg) {
		case WM_LBUTTONDBLCLK : {
			/*
			 �����̓Q�[�������ł͗���Ă��Ă���B
			 */
			return 0;
								}
		case WM_RBUTTONDOWN: {
			/*
			// �N���C�A���g�N���b�N���W�����[���h���W�ւƕϊ�����B
			POINT pt;

			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);

			ClientToScreen(hWnd, &pt);

			// OutputDebugStream("%d,%d", pt.x, pt.y);
			*/
			return 0;

		}
		case WM_LBUTTONDOWN: {
			// �����͗��Ȃ�
			return 0;
		}
	}

	return 0;
}
