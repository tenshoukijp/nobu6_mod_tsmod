#pragma once


#include "WinTarget.h"
#include <map>
#include <string>

using namespace std;

#define HANDLESTRUCT_HDC_ADDRESS	0x4D1A00	// HDCのハンドルが存在するアドレス。


class CommonGlobalHandle {
public:
		// tenshou.exeの方のもじゅーあるハンドル
		static HANDLE hTenshouExe;
		// tenshou.exeのフルネーム
		static char szTenshouExeFullPath[MAX_PATH];


		// 本体のモジュールハンドル
		static HANDLE hDLLModule;
		// 天翔記の本体ウィンドウハンドル
		static HWND tenshouWND;
		// 天翔記のスレッドID
		static DWORD tenshouThreadId;
		// 天翔記のプロセス
		static HANDLE curProcess;

		// 天翔記のデバイスコンテキスト
		static HDC hDC;

		// 解像度を選んだところからゲームスタート。
		// ムービーとか流している間に、メモリをせっせこせっせこ読み込まないようにする。
		static BOOL isNobunaganoYabouGameStart;

		// メインゲームがスタートしている
		static BOOL isMainGameStart;

		// シナリオ選択～メインゲーム実行の間のモード
		static BOOL isModeScenario;

		static HMODULE hResourceWavMod;
		static HMODULE hResourceWavMod2;
		static HMODULE hResourceWavMod3;

		// 
		static BOOL isNobunaganoYabouGameEnd;

		// エンディングに入った
		static BOOL isEndingMode;

		// エンディングのパラメータクリア後
		static BOOL isEndingModeAfter;

		// 戦争モードに突入している。(実際のどこか同士の戦場マップと、メインマップ上でのどこかでの戦争中の両方を含む）
		static int isBattleMode;

		static int isPlayerMainScreenInputWait; // メイン画面でプレイヤが城ウィンドウと軍団ウィンドウを出して、コマンド入力可能な状態。

		// グローバル。メニューのハンドル。
		static HMENU hMenu;

		// シナリオモッド用のPopMenu
		static HMENU hScenarioModPopMenu;

		// シフトキーを押しながら起動した。
		static BOOL isShiftKey;

		// ディスクが必要ないモード
		static BOOL isNoDiskMode;

		// ディスクを入れていない
		static BOOL isExistDisk;

		// 現在のデバイスコンテキストのハンドル。patbltから頂く。
		static HDC curHDC;

		// 起動時のオプション構造体
		static RECT latestCenterWindowRect;

		// スクリーンのサイズが切り替わった後だけ操作可能。そのタイミングまでは「終了」のメニューがアクティブにならないから。
		static BOOL IsAfterScreenSizeSelected;

		// タスクバーのウィンドウハンドル
		static HWND hWndTaskBar;
};
