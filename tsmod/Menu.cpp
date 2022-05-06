// ���j���[���̂��̂��Ō�ɒǉ�����B

#include "GameDataStruct.h"
#include "TSMod.h"
#include "Menu.h"
#include "Resource.h"
#include "GetOSVersion.h"
#include "WinTarget.h"
#include "TSModCommand.h"

#include <shellapi.h>

map<HMODULE, string> mapListEspecialModule;


// ���j���[�A�C�e����ǉ��ς��ǂ���
bool isAddMenuItems = false;

// ���ꃂ�W���[���Ƃ��Ẵ��X�g�ɓo�^����B
void AddModuleItem( HMODULE hModule, string modulename ) {

	// ���W���[���Ƃ��ēo�^����B
	mapListEspecialModule.insert( map<HMODULE, string>::value_type(hModule, modulename) );
}

// ���j���[�A�C�e���̒ǉ�
// menuname ���j���[�A�C�e���̕�����B"---"���ƃZ�p���[�^�ƂȂ�B
void AddMenuItem(string menuname, int position, int wID) {

	MENUITEMINFO info;

	// �쐬�������j���[���ڂ�ǉ�����
	ZeroMemory( &info, sizeof(info) );
	info.cbSize = sizeof( info );

	// ID�ԍ��̎w�肪�����ꍇ
	if ( wID == -1 ) {
		info.fMask = MIIM_TYPE;
	// ID�ԍ��̎w�肪����ꍇ
	} else {
		info.fMask = MIIM_TYPE | MIIM_ID;
		info.wID = wID;
	}
	if ( menuname == "---" ) {
		info.fType = MFT_SEPARATOR;
	} else {
		info.fType = MFT_STRING;
	}
	info.dwTypeData =(LPSTR)menuname.c_str();
	InsertMenuItem( CommonGlobalHandle::hMenu, position, FALSE, &info );
	DrawMenuBar( CommonGlobalHandle::tenshouWND );
}

// ��L�̊O���Q�Ɨp
void WINAPI Extern_AddMenuItem(char *menuname, int position, int wID) {
	position = MENU_ITEM_MODIFY_BGN_ID; // �������j���[�̐擪��ID�B�������ɉ����Ă���

	AddMenuItem(menuname, position, wID);
}

// ���C�����j���[�̒ǉ�
void AddMenu(string menuname, int position) {

	MENUITEMINFO info;

	// �쐬�������j���[���ڂ�ǉ�����
	ZeroMemory( &info, sizeof(info) );
	info.cbSize = sizeof( info );
	info.fMask = MIIM_TYPE;
	info.fType = MFT_STRING;
	info.dwTypeData = (LPSTR)menuname.c_str();
	InsertMenuItem( CommonGlobalHandle::hMenu, 0xFFFF, TRUE, &info );
	DrawMenuBar( CommonGlobalHandle::tenshouWND );
}

// ���j���[�̏�Ԃ̕ύX�B�g����
// ChangeMenuItem(hmenuPopup, ID_MENU, MFS_CHECKED);
// ChangeMenuItem(hmenuPopup, ID_MENU, MFS_UNCHECKED);
// ChangeMenuItem(hmenuPopup, ID_EXIT, MFS_GRAYED);
// ChangeMenuItem(hmenuPopup, ID_EXIT, MFS_ENABLED);
void ChangeMenuItem(HMENU hmenu, int nId, ULONG uState)
{
	MENUITEMINFO mii;
	
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask  = MIIM_ID | MIIM_STATE;
	mii.fState = uState;
	mii.wID    = nId;

	SetMenuItemInfo(hmenu, nId, FALSE, &mii);
}



/*
    GetItemData�͓�����GetMenuItemInfo���Ăяo������֐��ŁA
	��2�����Ƒ�3�����́AGetMenuItemInfo�̂���ƑΉ����Ă��܂��B
	LOWORD(wParam)�́A���j���[���|�b�v�A�b�v���j���[�������ǂ����ł��̒l�̈Ӗ����قȂ�܂����A
	��3������HIWORD(wParam) & MF_POPUP�Ƃ���΁A �|�b�v�A�b�v���j���[�����ꍇ�͒l��TURE�ƂȂ�A
	�����łȂ��ꍇ��FALSE�ƂȂ邽�߁A
	��2�����Ƃ̒l�̈Ӗ��ɖ����͂Ȃ��Ȃ�܂��B 
*/
LPTSTR GetItemData(HMENU hmenu, int nParam, BOOL bByPosition)
{
	MENUITEMINFO mii;

	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask  = MIIM_DATA;

	GetMenuItemInfo(hmenu, nParam, bByPosition, &mii);

	return (LPTSTR)mii.dwItemData;
}



/*
SetMenuItemFromIcon�Ƃ�������֐��́A�A�C�R���̃n���h������r�b�g�}�b�v���擾���A
 ��������j���[���ڂɐݒ肷��֐��ł��B 
 ��1�����͍��ڂ��܂ރ��j���[�n���h���ł���A
 ��2�����̓r�b�g�}�b�v��ݒ肵�������ڂ�ID�ł��B
 ��3�����̓A�C�R���̃n���h���ł���A�����SHGetStockIconInfo�Ŏ擾���Ă��܂��B
 
 ���̊֐��̓V�X�e���ŗp�ӂ���Ă���A�C�R����Ԃ��֐��ł���A
 ��1������SIID_SHIELD���w�肵���ꍇ�̓V�[���h�A�C�R�����擾�ł��܂��B

	SHSTOCKICONINFO sii;
	sii.cbSize = sizeof(SHSTOCKICONINFO);
	SHGetStockIconInfo(SIID_SHIELD, SHGFI_ICON | SHGFI_SMALLICON, &sii);

����̓A�C�R�����n���h���Ƃ��Ď擾�������̂ŁA��2�����ɂ�SHGFI_ICON���w�肵�A����ɏ������A�C�R���Ƃ��Ď擾���邽�߂�SHGFI_SMALLICON���w�肵�Ă��܂��B 
 */
static HBITMAP CreateBitmapARGB(int nWidth, int nHeight)
{
	LPVOID           lpBits;
	BITMAPINFO       bmi;
	BITMAPINFOHEADER bmiHeader;

	ZeroMemory(&bmiHeader, sizeof(BITMAPINFOHEADER));
	bmiHeader.biSize      = sizeof(BITMAPINFOHEADER);
	bmiHeader.biWidth     = nWidth;
	bmiHeader.biHeight    = nHeight;
	bmiHeader.biPlanes    = 1;
	bmiHeader.biBitCount  = 32;

	bmi.bmiHeader = bmiHeader;
	
	return CreateDIBSection(NULL, (LPBITMAPINFO)&bmi, DIB_RGB_COLORS, &lpBits, NULL, 0);
}
/*
GetSystemMetrics�ŏ������A�C�R���̃T�C�Y���擾���A��������CreateBitmapARGB�Ƃ�������֐����Ăяo���Ă��܂��B
 ���̊֐���32�r�b�g�̃r�b�g�}�b�v��DIB�Z�N�V�����Ƃ��č쐬���A���̃n���h���Ԃ��܂��B
 �������A�r�b�g�}�b�v�̃n���h�������ł͂��̃r�b�g�}�b�v�ɃA�C�R���̃s�N�Z�����i�[�ł��Ȃ����߁A
 SelectObject�Ń������f�o�C�X�R���e�L�X�g�ɑI������悤�ɂ��Ă��܂��B
 
 ���̃������f�o�C�X�R���e�L�X�g�ɑ΂���DrawIconEx���Ăяo���΁A
 �I������Ă���r�b�g�}�b�v�ɃA�C�R�����`�悳��邱�ƂɂȂ�A
 �r�b�g�}�b�v�ɂ̓A�C�R���̃s�N�Z�����i�[���ꂽ���ƂɂȂ�܂��B
 
 ��͂����MENUITEMINFO�\���̂�hbmpItem�Ɏw�肵��SetMenuItemInfo���Ăяo���΁A
 ���j���[���ڂɃr�b�g�}�b�v���ݒ肳��邱�ƂɂȂ�܂��B 
*/
void SetMenuItemFromIcon(HMENU hmenu, int nId, HICON hicon)
{
	UINT         uWidth, uHeight;
	HDC          hdcMem;
	HBITMAP      hbmp, hbmpPrev;
	MENUITEMINFO mii;

	uWidth = GetSystemMetrics(SM_CXSMICON);
	uHeight = GetSystemMetrics(SM_CYSMICON);
	hbmp = CreateBitmapARGB(uWidth, uHeight);
	
	hdcMem = CreateCompatibleDC(NULL);
	hbmpPrev = (HBITMAP)SelectObject(hdcMem, hbmp);
	DrawIconEx(hdcMem, 0, 0, hicon, uWidth, uHeight, 0, NULL, DI_NORMAL);
	SelectObject(hdcMem, hbmpPrev);
	DeleteDC(hdcMem);

	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_BITMAP;
	mii.hbmpItem = hbmp;
	SetMenuItemInfo(hmenu, nId, FALSE, &mii);	
}



// �g�b�v�̐ݒ胁�j���[�ɂԂ牺����|�b�v���j���[�̃��j���[�n���h���𓾂�
HMENU GetTenshoukiSetMenu() {
	return ::GetSubMenu(CommonGlobalHandle::hMenu, 1); // nPos��0�x�[�X��1�̃��j���[
}


// �g�b�v�̉������j���[�ɂԂ牺����|�b�v���j���[�̃��j���[�n���h���𓾂�
HMENU GetTenshoukiModMenu() {
	return ::GetSubMenu(CommonGlobalHandle::hMenu, 2); // nPos��0�x�[�X��2�̃��j���[
}

// �g�b�v�̃w���v���j���[�ɂԂ牺����|�b�v���j���[�̃��j���[�n���h���𓾂�
HMENU GetTenshoukiHelpMenu() {
	return ::GetSubMenu(CommonGlobalHandle::hMenu, 3); // nPos��0�x�[�X��3�̃��j���[
}


// �w���v���j���[���r�b�g�}�b�v�^�C�v�ɂ���B
static void SetBitmapTypeHelpMenu() {

	// Vista�����͂����ŏ����I���B���j���[�̃A�C�R���Ƃ��͕ύX���Ȃ��B
	if ( getOSVersion() < 6.0 ) { // Vista ����
		return;
	}
/*
	// �w���v���j���[�̃��j���[�n���h���𓾂�
	HMENU hmenuPopup = GetTenshoukiModMenu();

	SHSTOCKICONINFO sii;
	sii.cbSize = sizeof(SHSTOCKICONINFO);
	SHGetStockIconInfo(SIID_SHIELD, SHGFI_ICON | SHGFI_SMALLICON, &sii);
	SetMenuItemFromIcon(hmenuPopup, 121, sii.hIcon); // 121==����K�C�h

	// ����̃��j���[�ł́A���ڂ̍����Ƀr�b�g�}�b�v�ƃ`�F�b�N�}�[�N��2�̃X�y�[�X���p�ӂ���܂����A
	// ����̂悤�Ƀr�b�g�}�b�v������\���������̏ꍇ�́A1�̃X�y�[�X�����ŏ\���Ƃ����܂��B
	// ���̂悤�ȏꍇ�́A���j���[���ڂł͂Ȃ����j���[���̂̏���ݒ肷��SetMenuInfo���Ăяo���A
	// ���j���[�̃X�^�C����ύX���邱�ƂɂȂ�܂��B 
	MENUINFO menuInfo;
	menuInfo.cbSize = sizeof(MENUINFO);
	menuInfo.fMask = MIM_STYLE; 
	GetMenuInfo(hmenuPopup, &menuInfo);
	menuInfo.dwStyle = (menuInfo.dwStyle & ~MNS_NOCHECK) | MNS_CHECKORBMP;
	SetMenuInfo(hmenuPopup, &menuInfo);
*/
}

// �w���v���j���[���r�b�g�}�b�v�^�C�v�ɂ���B
void SetApplicationMenuBitmapType() {
	SetBitmapTypeHelpMenu();
}


// �^�C�g���o�[�̋�`�̈���擾
TITLEBARINFO GetTenshouTitleBarInfo() {
	static TITLEBARINFO info;

	info.cbSize = sizeof(TITLEBARINFO);
	GetTitleBarInfo( CommonGlobalHandle::tenshouWND, &info );
	return info;
}

// �^�C�g���ƃ��j���[�Ȃ�
void setNoCaptionAndMenuBarBefore() {

}

// �^�C�g���ƃ��j���[�Ȃ�
void setNoCaptionAndMenuBarAfter() {
	// �^�C�g���o�[�ƃ��j���[�����t���O��ON�Ȃ��
	if ( TSModCommand::Environment::cmdNoTitleBarAndMenuBar ) {
		// 	���j���[�Ȃ�
		SetMenu( CommonGlobalHandle::tenshouWND, NULL );

		// �^�C�g���o�[�Ȃ�
		LONG lStyle;
		lStyle  = GetWindowLong( CommonGlobalHandle::tenshouWND, GWL_STYLE );
		lStyle = WS_OVERLAPPED | WS_CLIPSIBLINGS | WS_VISIBLE | WS_THICKFRAME | WS_DLGFRAME ;
		SetWindowLong( CommonGlobalHandle::tenshouWND, GWL_STYLE, lStyle );
	}
}

// ���j���[���^�C�}�[�ŉB��
VOID CALLBACK TenshouMenuHide( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
	SetMenu( hwnd, NULL );
}
