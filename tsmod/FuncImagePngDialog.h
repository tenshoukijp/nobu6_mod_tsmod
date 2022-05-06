#pragma once

#pragma pack(1)

namespace ImageDialogCloseTime {
	enum { ProgramClose = -0x03, AutoClose = -0x02, ManualClose = -0x01 };
}

// ビットマップ表示ダイアログ。普通のwin32で実現。フルカラー。
// 座標を指定しない場合には、真ん中に表示する。
// AutoCloseは時間で勝手にダイアログを閉じるかどうか。FALSEを設定すると閉じなくなる。
int OpenPngDialog(char *szBitmapName, int posX=-1, int posY=-1, int iTime=ImageDialogCloseTime::AutoClose);
int OpenPngResourceDialog(char *szResourceName, HMODULE hModule, int posX=-1, int posY=-1, int iTime=ImageDialogCloseTime::AutoClose);

BOOL FreeImageDialog();