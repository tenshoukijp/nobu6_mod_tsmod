#pragma once

#pragma pack(1)

#include "WinTarget.h"

// 時間で自動的に閉じるメッセージボックス。ボタンを押しても反応するし、自動でも閉じる。
int TimedMessageBox(HWND hwndOwner,
					LPCTSTR pszMessage,
					LPCTSTR pszTitle,
					UINT flags,
					DWORD dwTimeout);