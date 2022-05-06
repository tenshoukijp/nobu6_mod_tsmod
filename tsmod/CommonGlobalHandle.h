#pragma once


#include "WinTarget.h"
#include <map>
#include <string>

using namespace std;

#define HANDLESTRUCT_HDC_ADDRESS	0x4D1A00	// HDC�̃n���h�������݂���A�h���X�B


class CommonGlobalHandle {
public:
		// tenshou.exe�̕��̂�����[����n���h��
		static HANDLE hTenshouExe;
		// tenshou.exe�̃t���l�[��
		static char szTenshouExeFullPath[MAX_PATH];


		// �{�̂̃��W���[���n���h��
		static HANDLE hDLLModule;
		// �V�ċL�̖{�̃E�B���h�E�n���h��
		static HWND tenshouWND;
		// �V�ċL�̃X���b�hID
		static DWORD tenshouThreadId;
		// �V�ċL�̃v���Z�X
		static HANDLE curProcess;

		// �V�ċL�̃f�o�C�X�R���e�L�X�g
		static HDC hDC;

		// �𑜓x��I�񂾂Ƃ��납��Q�[���X�^�[�g�B
		// ���[�r�[�Ƃ������Ă���ԂɁA�������������������������ǂݍ��܂Ȃ��悤�ɂ���B
		static BOOL isNobunaganoYabouGameStart;

		// ���C���Q�[�����X�^�[�g���Ă���
		static BOOL isMainGameStart;

		// �V�i���I�I���`���C���Q�[�����s�̊Ԃ̃��[�h
		static BOOL isModeScenario;

		static HMODULE hResourceWavMod;
		static HMODULE hResourceWavMod2;
		static HMODULE hResourceWavMod3;

		// 
		static BOOL isNobunaganoYabouGameEnd;

		// �G���f�B���O�ɓ�����
		static BOOL isEndingMode;

		// �G���f�B���O�̃p�����[�^�N���A��
		static BOOL isEndingModeAfter;

		// �푈���[�h�ɓ˓����Ă���B(���ۂ̂ǂ������m�̐��}�b�v�ƁA���C���}�b�v��ł̂ǂ����ł̐푈���̗������܂ށj
		static int isBattleMode;

		static int isPlayerMainScreenInputWait; // ���C����ʂŃv���C������E�B���h�E�ƌR�c�E�B���h�E���o���āA�R�}���h���͉\�ȏ�ԁB

		// �O���[�o���B���j���[�̃n���h���B
		static HMENU hMenu;

		// �V�i���I���b�h�p��PopMenu
		static HMENU hScenarioModPopMenu;

		// �V�t�g�L�[�������Ȃ���N�������B
		static BOOL isShiftKey;

		// �f�B�X�N���K�v�Ȃ����[�h
		static BOOL isNoDiskMode;

		// �f�B�X�N�����Ă��Ȃ�
		static BOOL isExistDisk;

		// ���݂̃f�o�C�X�R���e�L�X�g�̃n���h���Bpatblt���璸���B
		static HDC curHDC;

		// �N�����̃I�v�V�����\����
		static RECT latestCenterWindowRect;

		// �X�N���[���̃T�C�Y���؂�ւ�����ゾ������\�B���̃^�C�~���O�܂ł́u�I���v�̃��j���[���A�N�e�B�u�ɂȂ�Ȃ�����B
		static BOOL IsAfterScreenSizeSelected;

		// �^�X�N�o�[�̃E�B���h�E�n���h��
		static HWND hWndTaskBar;
};
