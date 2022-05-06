#include "WinTarget.h"
#include "DialogKahou.h"
#include "SoundServer.h"

extern DialogKahou DK;
extern void DestroyKahouMovieMCI();
extern void OnTenshouExeViewingCastleRetsudenFuncExecutePost();

// �摜�n�̃n�b�N
int Hook_MessageBoxACustom(
  HWND hWnd,          // �I�[�i�[�E�B���h�E�̃n���h��
  PCTSTR lpText,     // ���b�Z�[�W�{�b�N�X���̃e�L�X�g
  PCTSTR lpCaption,  // ���b�Z�[�W�{�b�N�X�̃^�C�g��
  UINT uType,          // ���b�Z�[�W�{�b�N�X�̃X�^�C��
  int OrigMessageBoxAResult  // �I���W�i���̃{�b�N�X�̕ԓ�
  )
{
	// �E�B���h�E�̉E�ォ�炢���Ȃ�I���ƁA�ƕ󂪂��炿��c��̂ŁA�����Ŋ��S�ɏ�������
	if (strstr(lpText, "���I�����܂��B") && OrigMessageBoxAResult==IDOK) {
		// ���`���̕`��͒�~
		OnTenshouExeViewingCastleRetsudenFuncExecutePost();

		DK.eModeKahou = KAHOU_MODE_GAMEEND;
		DestroyKahouMovieMCI(); // ������̓��[�r�[�ł̕�

		// �S�Ẳ����~�߂�
		StopAllSound();
	}


	return OrigMessageBoxAResult;
}
