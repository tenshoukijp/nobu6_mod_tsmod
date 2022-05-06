#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"

#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>

#include "CommonGlobalHandle.h"
#include "TSModCommand.h"
#include "TSModPatchProcessMemory.h"
#include "OutputDebugStream.h"

#include "lua/lua_interface.h"

using namespace std;


/*----------------------------------------------------------------*
 IOProcessMemory系宣言
 *----------------------------------------------------------------*/

//----------------------------------------------------------------

/*----------------------------------------------------------------*
文字列処理系宣言。
※この関数はもう使われていない。Lua系へと変更された!!
 *----------------------------------------------------------------*/
void WriteProcessMemoryPatchData(char *szFileName, int iMode) { 

	ifstream ifs;
	ifs.open(szFileName, ios::in);

	if ( !ifs ) { return; }

	char szCurLine[255];
	int iTargetMemoryAddress;
	char szPatchTokenBuff[255];

	while( !ifs.eof() ){
		ifs.getline(szCurLine, sizeof(szCurLine));

		// コメントなら次
		if (szCurLine[0] == '#' || szCurLine[0] == '/') {
			continue;
		}

		/* \tを区切りに文字列を抽出 */
		int iTokenIX = 0;
		iTargetMemoryAddress = 0;
		char *tp = NULL; // 現在指してる文字列の場所
		bool isModeAsm = false;
		bool isModeCmd = false;

		for ( tp = strtok( szCurLine, ":" ), iTokenIX=0; tp != NULL; tp = strtok( NULL, "\n" ), iTokenIX++ ) {
			if (iTokenIX==0) {
				// cout << tp << endl;
				sscanf(tp, "%x", &iTargetMemoryAddress);

			// 2番目がasmになってるか調べる
			} else if (iTokenIX==1 && !strncmp(tp, "asm", 3)) {
				isModeAsm = true;
				strcpy(szPatchTokenBuff, tp+strlen("asm:")); // ｢asm:｣ を除いてコピーするので…
			} else if (iTokenIX==1 && !strncmp(tp, "cmd", 3)) {
				isModeCmd = true;
				strcpy(szPatchTokenBuff, tp+strlen("cmd:")); // ｢asm:｣ を除いてコピーするので…
			} else if (iTokenIX==1 && !strncmp(tp, "txt", 3)) {
				strcpy(szPatchTokenBuff, tp+strlen("txt:")); // ｢txt:｣ を除いてコピーするので…
			} else {
				strcpy(szPatchTokenBuff, tp);
				//cout << tp << num_buf << endl;
			}
			
		}
		if (iTargetMemoryAddress) {
			// アセンブラモード
			if (isModeAsm ) {
				// アセンブラコードを分解
				char *p_asm = NULL; // 現在指してる文字列の場所
				int iAsmCodeIX = 0;
				int byteCurAsm = 0; // 現在のasm文字列(2文字)をchar型に変換した際の格納場所
				// asm コードを ｢:｣で分解してゆき、1回1回書いてゆく。
				for ( p_asm = strtok( szPatchTokenBuff, ":" ); p_asm != NULL; p_asm = strtok( NULL, ":" ) ) {
					sscanf_s(p_asm, "%x", &byteCurAsm); // 16進数文字→数値
					WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(iTargetMemoryAddress+iAsmCodeIX), &byteCurAsm, 1, NULL);
					iAsmCodeIX++;
				}

			// コマンドモード
			} else if (isModeCmd ) {
				
				// コマンドコードを分解
				char *p_cmd = NULL; // 現在指してる文字列の場所
				int iCmdCodeIX = 0;
				// cmd コードを ｢:｣で分解する、原則、「cmd:コマンド名:コマンドレベル」となっている。
				char szCurCmdTokenBuf[256] = "";
				int byteCurCmdLv = 0;
				for ( p_cmd = strtok( szPatchTokenBuff, ":" ); p_cmd != NULL; p_cmd = strtok( NULL, ":" ) ) {
					// １番目は、具体的なコマンド文字列
					if (iCmdCodeIX==0) {
						// コマンド文字列をコピー
						strcpy( szCurCmdTokenBuf, p_cmd );

					// ２番目は、コマンドのレベル
					} else if (iCmdCodeIX==1) {
						// コマンド文字列を数値にしてコピー
						sscanf_s(p_cmd, "%d", &byteCurCmdLv);
					}
					iCmdCodeIX++;
				}

				TSModCommand::setCommandSwitch(szCurCmdTokenBuf, byteCurCmdLv);
				
				// アセンブラではない普通の文字列タイプ
			} else {
				WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(iTargetMemoryAddress), szPatchTokenBuff, strlen(szPatchTokenBuff)+1, NULL);
			}
		}

	}

}

// 文字列の分割
extern vector<string> string_split(const string &str, const string &delim);

// asmのテーブルの値を取得
int ImportAsmTable(lua_State *L) {

	lua_getglobal(L, "asm");

	lua_pushnil(L);
	while (lua_next(L, -2) != 0) {
		lua_pushvalue(L, -2);

		string strMemoryAddress = lua_tostring(L, -1);
		const char *pszValue = lua_tostring(L, -2);

		// キーで16進数の文字列を数値に変更
		int iMemoryAddress = 0;
		sscanf(strMemoryAddress.c_str(), "%x", &iMemoryAddress);

		// アセンブラコードを分解
		char *p_asm = NULL; // 現在指してる文字列の場所
		int iAsmCodeIX = 0; // アドレスの進行。１つのbyteCurAsmを書いたら、１バイト書いたということなので、charアドレスが１つ進行
		int byteCurAsm = 0; // 現在のasm文字列(2文字)をchar型に変換した際の格納場所

		vector<string> string_list = string_split(pszValue, ":");

		for ( vector<string>::iterator it = string_list.begin(); it != string_list.end(); it++) {

			sscanf_s(it->c_str(), "%x", &byteCurAsm); // 16進数文字→数値
			WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(iMemoryAddress+iAsmCodeIX), &byteCurAsm, 1, NULL);
			iAsmCodeIX++;
		}

		lua_pop(L, 2);
	}

	lua_pop(L, 1);

	return true;
}

// txtのテーブルの値を取得
int ImportTxtTable(lua_State *L) {

	lua_getglobal(L, "txt");

	lua_pushnil(L);
	while (lua_next(L, -2) != 0) {
		lua_pushvalue(L, -2);

		string strMemoryAddress = lua_tostring(L, -1);
		string value = lua_tostring(L, -2);

		// キーで16進数の文字列を数値に変更
		int iMemoryAddress = 0;
		sscanf(strMemoryAddress.c_str(), "%x", &iMemoryAddress);

		// キーのアドレスに、値の文字列を書き込む
		WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(iMemoryAddress), value.c_str(), strlen( value.c_str() )+1, NULL);

		lua_pop(L, 2);
	}

	lua_pop(L, 1);

	return false;
}



// cmdのテーブルの値を取得
int ImportCmdTable(lua_State* L) {
	lua_getglobal(L, "cmd");

	lua_pushnil(L);
	while (lua_next(L, -2) != 0) {
		lua_pushvalue(L, -2);

		string key = lua_tostring(L, -1);
		int value = lua_tointeger(L, -2);

		// 各種変数に設定する。
		TSModCommand::setCommandSwitch((char *)key.c_str(), value);

		lua_pop(L, 2);
	}

	// スタックを削除
	lua_pop(L, 1);

	return 1;
}


/*----------------------------------------------------------------*
 LuaからTSMod.iniを読み込む
 *----------------------------------------------------------------*/
void ImportTSModIni(char *szFileName) {

	// BGMmmの段階で、lua_Stateがエラーになっていなければ…
	if (! luaL_getstate_err() ) {

		// まずはcmdテーブルのインポート
		ImportCmdTable(L);
		// 次にtxtテーブルのインポート
		ImportTxtTable(L);
		// 最後にasmテーブルのインポート
		ImportAsmTable(L);
	}
}



/*----------------------------------------------------------------*
 IOProcessMemory系処理
 *----------------------------------------------------------------*/
bool isDonePatchProcessMemory = false;

void TDModPatchProcessMemory(int iMode) { 
	
	if ( isDonePatchProcessMemory == false ) {
		isDonePatchProcessMemory = true;
	}

	struct stat  st;
	// TSMod.iniがあれば…
	if (stat("TSMod.ini", &st) == 0) {
		try {
			ImportTSModIni("TSMod.ini");
		} catch (...) { }
	}
	if (stat("TSModEx.ini", &st) == 0) {
		try {
			ImportTSModIni("TSModEx.ini");
		} catch (...) { }
	}

}


