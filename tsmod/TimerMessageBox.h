#pragma once

#pragma pack(1)

#include "WinTarget.h"

// ���ԂŎ����I�ɕ��郁�b�Z�[�W�{�b�N�X�B�{�^���������Ă��������邵�A�����ł�����B
int TimedMessageBox(HWND hwndOwner,
					LPCTSTR pszMessage,
					LPCTSTR pszTitle,
					UINT flags,
					DWORD dwTimeout);