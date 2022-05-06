#include <windows.h>
#include <windowsx.h>

#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"


LRESULT OnKeyEventHandler( HWND hWnd,      // �E�B���h�E�̃n���h��
						   UINT Msg,       // ���b�Z�[�W�̎��ʎq
						   WPARAM wParam,  // ���b�Z�[�W�̍ŏ��̃p�����[�^
						   LPARAM lParam   // ���b�Z�[�W�� 2 �Ԗڂ̃p�����[�^
)
{
	// WM_KEYDOWN��DDraw�ɂ���āA�D���Ă���̂ŁAWM_CHAR(�����ʏ�̕����L�[�{�[�h�������ꂽ)�ŉ䖝�BDDraw�̕��ɏ����Ă��ǂ�
	// ��ʃT�C�Y�̑I���t�F�C�Y�͏I�����Ă��邱�ƁB(�����܂�)
	if ( Msg == WM_CHAR ) {

		if ( GetKeyState('M')<0 && GetKeyState(VK_CONTROL) < 0 ) {

			// ���j���[�o�[�ƃ^�C�g���o�[���B�����[�h�ŁA���A�X�N���[���̃T�C�Y�I�����I���Ă���(���j���[�́u�I���v��Valild�ɂȂ�����
			if ( TSModCommand::Environment::cmdNoTitleBarAndMenuBar && CommonGlobalHandle::IsAfterScreenSizeSelected ) {

				// ���݂̃��j���[�́H
				HMENU curMenu = GetMenu(hWnd);

				// ���j���[������
				if ( !curMenu ) {
					SetMenu( CommonGlobalHandle::tenshouWND, CommonGlobalHandle::hMenu);
					// ���j���[�\���ێ��^�C�}�[���㏑������B�T�b�ێ�
					SetTimer( CommonGlobalHandle::tenshouWND, TIMER_EVENT_ID_MENU_HIDE, 3000, TenshouMenuHide );

				} else {
					SetMenu( CommonGlobalHandle::tenshouWND, NULL );
				}
			}

		}
	}
	return 0;
}


