#include <windows.h>
#include "Menu.h"

#include "CommonGlobalHandle.h"

/*
�V�X�e�����j���[�Ƃ́A�E�C���h�E�^�C�g���̍����ɂ���A�C�R���� �N���b�N�����Ƃ��ɕ\������郁�j���[�̂��ƂŁA
�E�C���h�E���j���[�� �R���g���[�����j���[�ƌĂ΂�邱�Ƃ�����܂��B

���̃��j���[�ɂ͓Ǝ��̍��ڂ�ǉ����邱�Ƃ��\�ŁA
�����̍D���Ȃ悤�ɃJ�X�^�}�C�Y���Ă���肠��܂���B
�V�X�e�����j���[�̃n���h���́AGetSystemMenu�Ŏ擾���܂��B 
*/
/*
win32api

HMENU GetSystemMenu(
    HWND hWnd,
    BOOL bRevert
);
*/


static void InitializeMenuItem(HMENU hmenu, LPTSTR lpszItemName, int nId, int nPos) {
	MENUITEMINFO mii;
	
	mii.cbSize     = sizeof(MENUITEMINFO);
	mii.fMask      = MIIM_ID | MIIM_TYPE;
	mii.fType      = MFT_STRING;
	mii.wID        = nId;
	mii.dwTypeData = lpszItemName;

	InsertMenuItem(hmenu, nPos, TRUE, &mii);
}

// �V�X�e�����j���[�Ɂu�����z�u�v�Ƃ������ڂ�����
void addCenterItemInApplicationSystemMenu(void) {
	HMENU hmenu = GetSystemMenu(CommonGlobalHandle::tenshouWND, FALSE);
	InitializeMenuItem(hmenu, "�����̈ʒu�ɖ߂�(&P)", RESOURCE_MENU_ID_CENTER_POS_IN_SYSTEM_MENU, 1);
}

// �E�B���h�E�𒆉��ւƔz�u���Ȃ���
void SetCenterWindow(HWND hwnd) {
	/*
	int  x, y;
	int  nScreenWidth, nScreenHeight;
	RECT rc;
	GetWindowRect(hwnd, &rc);
	
	nScreenWidth  = GetSystemMetrics(SM_CXFULLSCREEN);
	nScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	x = (nScreenWidth - (rc.right - rc.left)) / 2;
	y = (nScreenHeight - (rc.bottom - rc.top)) / 2;
	*/

	int x = CommonGlobalHandle::latestCenterWindowRect.left;
	int y = CommonGlobalHandle::latestCenterWindowRect.top;

	SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOSIZE);
}