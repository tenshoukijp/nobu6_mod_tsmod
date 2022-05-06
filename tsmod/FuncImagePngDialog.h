#pragma once

#pragma pack(1)

namespace ImageDialogCloseTime {
	enum { ProgramClose = -0x03, AutoClose = -0x02, ManualClose = -0x01 };
}

// �r�b�g�}�b�v�\���_�C�A���O�B���ʂ�win32�Ŏ����B�t���J���[�B
// ���W���w�肵�Ȃ��ꍇ�ɂ́A�^�񒆂ɕ\������B
// AutoClose�͎��Ԃŏ���Ƀ_�C�A���O����邩�ǂ����BFALSE��ݒ肷��ƕ��Ȃ��Ȃ�B
int OpenPngDialog(char *szBitmapName, int posX=-1, int posY=-1, int iTime=ImageDialogCloseTime::AutoClose);
int OpenPngResourceDialog(char *szResourceName, HMODULE hModule, int posX=-1, int posY=-1, int iTime=ImageDialogCloseTime::AutoClose);

BOOL FreeImageDialog();