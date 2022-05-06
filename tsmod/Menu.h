#pragma once

#include "WinTarget.h"
#include <map>
#include <string>

using namespace std;


#define RESOURCE_MENU_ID_SPECIAL_EFFECTS_OF_SCREEN_DRAWING		111  // 天翔記に元からある「画面描画の特殊効果を使用」のメニュー番号

#define RESOURCE_MENU_ID_CENTER_POS_IN_SYSTEM_MENU	174				// 左上のアイコンを右クリックしたら出てくるシステムメニューに足す項目

#define RESOURCE_MENU_ID_SAVEDATA_EXECUTE		175
#define RESOURCE_MENU_ID_MODDEBUGGER_EXECUTE	176
#define RESOURCE_MENU_ID_SAKURA_EXECUTE			180
#define RESOURCE_MENU_ID_MEMORY_EDITOR_MOD		181		// MemoryEditorMod用のメニュー番号。固定TSModと同じ。

#define MENU_ITEM_MODIFY_BGN_ID			RESOURCE_MENU_ID_SAVEDATA_EXECUTE		 // 改造メニューのスタートは「セーブデータエディタ」の番号
#define MENU_ITEM_MODULE_END_ID			49999		 // tenshou.exeに追加したリソースを見よ。


// メニューアイテムを追加済かどうか
extern bool isAddMenuItems;

// 読み込めているModの***.DLLのリスト。ScenarioMod.dll + PluginMod系 の合計のマップ。中は文字列。
extern map<HMODULE, string> mapListEspecialModule;

// 特別モジュールを登録する。
void AddModuleItem( HMODULE hModule, string modulename );

// 実際のメニューに追加する。
void AddMenuItem(string menuname, int position, int wID = -1); // wID = メニューにＩＤを付ける場合に指定

// メインメニューの追加
void AddMenu(string menuname, int position);

// 対象の位置にあるメニューからタグデータ(好きに埋め込めるタグデータ)を拾う
LPTSTR GetItemData(HMENU hmenu, int nParam, BOOL bByPosition);

// システムメニューに「中央配置」という項目を入れる
void addCenterItemInApplicationSystemMenu(void);

// ウィンドウを中央へと配置しなおす
void SetCenterWindow(HWND hwnd);



// トップの設定メニューにぶら下がるポップメニューのメニューハンドルを得る
HMENU GetTenshoukiSetMenu();

// トップの改造メニューにぶら下がるポップメニューのメニューハンドルを得る
HMENU GetTenshoukiModMenu();

// トップのヘルプメニューにぶら下がるポップメニューのメニューハンドルを得る
HMENU GetTenshoukiHelpMenu();


// いくつかのメニューをビットマップタイプにする。
void SetApplicationMenuBitmapType();


// 天翔記のタイトルバーの位置情報
TITLEBARINFO GetTenshouTitleBarInfo();

// メニューを隠す。
VOID CALLBACK TenshouMenuHide( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);