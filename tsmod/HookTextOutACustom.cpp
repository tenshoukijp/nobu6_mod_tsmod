#include "WinTarget.h"
#include "CommonGlobalHandle.h"

BOOL Hook_TextOutACustom(
	HDC hdc,           // �f�o�C�X�R���e�L�X�g�̃n���h��
	int nXStart,       // �J�n�ʒu�i��_�j�� x ���W
	int nYStart,       // �J�n�ʒu�i��_�j�� y ���W
	LPCTSTR lpString,  // ������
	int cbString       // ������
) {
	// �V�ċL�{�̂ŗ��p���Ă���f�o�C�X�R���e�L�X�g���R�s�[���Ă����B
	CommonGlobalHandle::hDC = hdc;

	return TRUE;
}
