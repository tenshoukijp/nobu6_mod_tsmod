#pragma once

#include "WinTarget.h"
#include <map>
#include <string>

using namespace std;


#define RESOURCE_MENU_ID_SPECIAL_EFFECTS_OF_SCREEN_DRAWING		111  // �V�ċL�Ɍ����炠��u��ʕ`��̓�����ʂ��g�p�v�̃��j���[�ԍ�

#define RESOURCE_MENU_ID_CENTER_POS_IN_SYSTEM_MENU	174				// ����̃A�C�R�����E�N���b�N������o�Ă���V�X�e�����j���[�ɑ�������

#define RESOURCE_MENU_ID_SAVEDATA_EXECUTE		175
#define RESOURCE_MENU_ID_MODDEBUGGER_EXECUTE	176
#define RESOURCE_MENU_ID_SAKURA_EXECUTE			180
#define RESOURCE_MENU_ID_MEMORY_EDITOR_MOD		181		// MemoryEditorMod�p�̃��j���[�ԍ��B�Œ�TSMod�Ɠ����B

#define MENU_ITEM_MODIFY_BGN_ID			RESOURCE_MENU_ID_SAVEDATA_EXECUTE		 // �������j���[�̃X�^�[�g�́u�Z�[�u�f�[�^�G�f�B�^�v�̔ԍ�
#define MENU_ITEM_MODULE_END_ID			49999		 // tenshou.exe�ɒǉ��������\�[�X������B


// ���j���[�A�C�e����ǉ��ς��ǂ���
extern bool isAddMenuItems;

// �ǂݍ��߂Ă���Mod��***.DLL�̃��X�g�BScenarioMod.dll + PluginMod�n �̍��v�̃}�b�v�B���͕�����B
extern map<HMODULE, string> mapListEspecialModule;

// ���ʃ��W���[����o�^����B
void AddModuleItem( HMODULE hModule, string modulename );

// ���ۂ̃��j���[�ɒǉ�����B
void AddMenuItem(string menuname, int position, int wID = -1); // wID = ���j���[�ɂh�c��t����ꍇ�Ɏw��

// ���C�����j���[�̒ǉ�
void AddMenu(string menuname, int position);

// �Ώۂ̈ʒu�ɂ��郁�j���[����^�O�f�[�^(�D���ɖ��ߍ��߂�^�O�f�[�^)���E��
LPTSTR GetItemData(HMENU hmenu, int nParam, BOOL bByPosition);

// �V�X�e�����j���[�Ɂu�����z�u�v�Ƃ������ڂ�����
void addCenterItemInApplicationSystemMenu(void);

// �E�B���h�E�𒆉��ւƔz�u���Ȃ���
void SetCenterWindow(HWND hwnd);



// �g�b�v�̐ݒ胁�j���[�ɂԂ牺����|�b�v���j���[�̃��j���[�n���h���𓾂�
HMENU GetTenshoukiSetMenu();

// �g�b�v�̉������j���[�ɂԂ牺����|�b�v���j���[�̃��j���[�n���h���𓾂�
HMENU GetTenshoukiModMenu();

// �g�b�v�̃w���v���j���[�ɂԂ牺����|�b�v���j���[�̃��j���[�n���h���𓾂�
HMENU GetTenshoukiHelpMenu();


// �������̃��j���[���r�b�g�}�b�v�^�C�v�ɂ���B
void SetApplicationMenuBitmapType();


// �V�ċL�̃^�C�g���o�[�̈ʒu���
TITLEBARINFO GetTenshouTitleBarInfo();

// ���j���[���B���B
VOID CALLBACK TenshouMenuHide( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);