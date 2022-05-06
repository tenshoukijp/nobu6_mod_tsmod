#include "WinTarget.h"
#include "DialogKahou.h"
#include "SoundServer.h"

extern DialogKahou DK;
extern void DestroyKahouMovieMCI();
extern void OnTenshouExeViewingCastleRetsudenFuncExecutePost();

// 画像系のハック
int Hook_MessageBoxACustom(
  HWND hWnd,          // オーナーウィンドウのハンドル
  PCTSTR lpText,     // メッセージボックス内のテキスト
  PCTSTR lpCaption,  // メッセージボックスのタイトル
  UINT uType,          // メッセージボックスのスタイル
  int OrigMessageBoxAResult  // オリジナルのボックスの返答
  )
{
	// ウィンドウの右上からいきなり終わると、家宝がちらちら残るので、ここで完全に処理する
	if (strstr(lpText, "を終了します。") && OrigMessageBoxAResult==IDOK) {
		// 城列伝内の描画は停止
		OnTenshouExeViewingCastleRetsudenFuncExecutePost();

		DK.eModeKahou = KAHOU_MODE_GAMEEND;
		DestroyKahouMovieMCI(); // こちらはムービー版の分

		// 全ての音を止める
		StopAllSound();
	}


	return OrigMessageBoxAResult;
}
