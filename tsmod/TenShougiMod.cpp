
#include <sys/types.h>
#include <sys/stat.h>


#include "GameDataStruct.h"
#include "CommonGlobalHandle.h"
#include "TenShougiMod.h"



// 天将棋Mod.dllのモジュールハンドル
HMODULE hTenShougiModDll = NULL;


// 完全起動(TSModからはやらない、TenShougiModServerが行う)
PFNTENSHOUGIMODBGN p_sgTenShougiModBgn = NULL;

// 完全終了(TSModからはやらない、TenShougiModServerが行う)
PFNTENSHOUGIMODEND p_sgTenShougiModEnd = NULL;


// フォント名を転送する。
PFNSETTENSHOUGIFONTNAME p_sgSetTenShougiFontName = NULL;

// 武将の棋力を転送
PFNSETTENSHOUGIBUSHOUPOWER p_sgSetTenShougiBushouPower = NULL;

// 天将棋ウィンドウが表示されているかどうかを天将棋→TSModを得る。
PFNISTENSHOUGIFORMSHOWN p_sgIsTenShougiFormShown = NULL;


// 天将棋にコマンドを送る。
PFNSENDCOMMAND p_sgSendCommand = NULL;


bool IsTenShougiModFilesExists() {

	// さらに
	struct stat st;
	// 
	if (stat("TenShougiMod.dll", &st) != 0) {
		return false;
	}

	if (stat("TenShougiModServer.exe", &st) != 0) {
		return false;
	}
	if (stat("TenShougiMod序盤棋譜.bin", &st) != 0) {
		return false;
	}
	if (stat("TenShougiMod評価関数.bin", &st) != 0) {
		return false;
	}

	return true;
}


// 天翔記内部で使用されているフォント名を、天将棋へ渡す
void SetTenshougiFontName() {

	// フォントを指している場所
	char *szTenShoukiFontName = (char *)GAMEDATASTRUCT_FONTNAME_ADDRESS;

	p_sgSetTenShougiFontName = (PFNSETTENSHOUGIFONTNAME)GetProcAddress(hTenShougiModDll, "sgSetTenShougiFontName");
	if ( p_sgSetTenShougiFontName ) {
		// 天翔記のフォント名を、天将棋のSharedMemory用領域に伝達
		p_sgSetTenShougiFontName( szTenShoukiFontName );
	}
}


// ２人の棋力を転送する。
void SetShougiBushouPower(int iBBushouID, int iWBushouID) {

	p_sgSetTenShougiBushouPower = (PFNSETTENSHOUGIBUSHOUPOWER)GetProcAddress(hTenShougiModDll, "sgSetTenShougiBushouPower");

	if ( p_sgSetTenShougiBushouPower ) {

		// 棋力構造体、先手用、後手用
		TBUshouDaniPowerInfo BPower = {NULL};
		TBUshouDaniPowerInfo WPower = {NULL};

		// 先手用
		BPower.iPower = GetShogiPower(iBBushouID);
		strcpy( BPower.szDaniName, getSpecialDaniStringOfBushou(iBBushouID) );
		BPower.iBushouID = iBBushouID;

		// 後手用
		WPower.iPower = GetShogiPower(iWBushouID);
		strcpy( WPower.szDaniName, getSpecialDaniStringOfBushou(iWBushouID) );
		WPower.iBushouID = iWBushouID;

		// 天将棋へと渡す
		p_sgSetTenShougiBushouPower( (void *)&BPower, (void *)&WPower );
	}
}


// 天将棋のメモリ情報を更新する。
void Update_TenShougiMemoryInfo(int iBBushouID, int iWBushouID) {
	// 天将棋入ってない。ダメ。
	if (!hTenShougiModDll ) { return; }

	// 現在天将棋のウィンドウが開く関数をセットしたことがなければ、ダメ。更新しない。
	if (!p_sgIsTenShougiFormShown ) { return; }

	if ( 0 <= iBBushouID && iBBushouID < GAMEDATASTRUCT_BUSHOU_NUM && 
	     0 <= iWBushouID && iWBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// フォント名を伝達
		SetTenshougiFontName();

		// ２人の棋力を転送する。
		SetShougiBushouPower(iBBushouID, iWBushouID);
	}
}


// 天将棋開始
void TenShougiModBgn(int iBBushouID, int iWBushouID) {

	// 現在天将棋のウィンドウが開いている状態かどうかの関数
	if (!p_sgIsTenShougiFormShown) {
		p_sgIsTenShougiFormShown = (PFNISTENSHOUGIFORMSHOWN)GetProcAddress(hTenShougiModDll, "sgIsTenShougiFormShown");
	}
	if (!p_sgIsTenShougiFormShown ) { return; }

	if (!p_sgSendCommand) {
		p_sgSendCommand = (PFNSENDCOMMAND)GetProcAddress(hTenShougiModDll, "sgSendCommand");
	}
	if (!p_sgSendCommand) { return; }


	// まだ天将棋を起動したことがない。ボナンザPipeが構築されていない。
	if (p_sgTenShougiModBgn==NULL)  {

		p_sgTenShougiModBgn = (PFNTENSHOUGIMODBGN)GetProcAddress(hTenShougiModDll, "sgTenShougiModBgn");
		if ( p_sgTenShougiModBgn) {
			// 何もなし。開始は、TenShougiServerが行う。
		}

		// 武将等のメモリ情報を更新。
		Update_TenShougiMemoryInfo(iBBushouID, iWBushouID);

		// 天将棋ランチャーを起動
		ShellExecute(NULL, NULL, "TenShougiModServer.exe", "launch", NULL, SW_HIDE);
	}

	// 起動したことはある。今天将棋のウィンドウが開いていない。
	else if ( ! p_sgIsTenShougiFormShown() ) {

		// 武将等のメモリ情報を更新。
		Update_TenShougiMemoryInfo(iBBushouID, iWBushouID);

		p_sgSendCommand( "open_window_and_new_game" );
		// ★ここでリニューゲームするだけ。ウィンドウは開かない。
	} else {
	}

}

void TenShougiModHide() {
	// 天将棋入ってない。ダメ。
	if (!hTenShougiModDll ) { return; }

	// ウィンドウが表示されてるかどうかの関数が存在する
	if ( p_sgIsTenShougiFormShown ) {
		// ウィンドウが表示されているならば
		if ( p_sgIsTenShougiFormShown() ) {
			// 隠す
			p_sgSendCommand( "hide_window" );
		}
	}
}

// 天将棋終了
void TenShougiModEnd() {

	// 天将棋入ってない。ダメ。
	if (!hTenShougiModDll ) { return; }

	p_sgTenShougiModEnd = (PFNTENSHOUGIMODBGN)GetProcAddress(hTenShougiModDll, "sgTenShougiModEnd");
	if ( p_sgTenShougiModEnd ) {
		// 何もなし。解放は、TenShougiServerが行う。
	}

	// DLLの解放
	FreeLibrary( hTenShougiModDll );
}


