#include "WinTarget.h"
#include <fstream>

#include "DialogKahou.h"

using namespace std;

extern DialogKahou DK;

// �摜�n�̃n�b�N
BOOL Hook_BitBltCustom(
  HDC hdcDest, // �R�s�[��f�o�C�X�R���e�L�X�g�̃n���h��
  int nXDest,  // �R�s�[�撷���`�̍������ x ���W
  int nYDest,  // �R�s�[�撷���`�̍������ y ���W
  int nWidth,  // �R�s�[�撷���`�̕�
  int nHeight, // �R�s�[�撷���`�̍���
  HDC hdcSrc,  // �R�s�[���f�o�C�X�R���e�L�X�g�̃n���h��
  int nXSrc,   // �R�s�[�������`�̍������ x ���W
  int nYSrc,   // �R�s�[�������`�̍������ y ���W
  DWORD dwRop  // ���X�^�I�y���[�V�����R�[�h
  ) {

	return TRUE;
}
