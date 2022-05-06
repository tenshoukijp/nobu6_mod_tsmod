
#include <windows.h>

#include "lua.hpp"
#include "lua_interface.h"
#include "../TSModCommand.h"

lua_State* L = NULL; // lua_state

const int LUA_VARIABLE_GET_EXCEPTION = 1;

typedef const int E_LUA_EXCEPTION;


void lua_OutputDebugStream( const char *format, ... ) {
    char szBufDebug[4096] = "";
    int length = sizeof(szBufDebug);

    va_list arg;

    va_start(arg, format);
    _vsnprintf_s(szBufDebug, length, _TRUNCATE, format, arg);
    va_end(arg);

	szBufDebug[length - 1] = '\0'; // 番兵

    OutputDebugString( szBufDebug );
}


void LoadLuaInterpreter() {
	// Luaを開く
	L = luaL_newstate();

	// Lua標準関数を使う
	luaL_openlibs(L);
}




void FreeLuaInterpreter() {
	// Luaを閉じる
	lua_close(L);
}




// 指定の名前の空テーブルをLuaにプッシュ
int PushEmptyTable(lua_State *L, char *szTableName) {
	lua_newtable(L);

	lua_setglobal(L, szTableName);

	return 1;
}

// lua_getglobalの数値タイプのラッパー
double lua_getglobal_numtype(char *var_name) {
	// グローバル変数を読む
	lua_getglobal(L, var_name);
	if(lua_isnumber(L, -1)) {
		double ret = lua_tonumber(L, -1);
		lua_pop(L, 1);
		return ret;
	} else {
		lua_pop(L, 1);
		throw LUA_VARIABLE_GET_EXCEPTION;
	}
}

// lua_getglobalの文字列タイプのラッパー
string lua_getglobal_strtype(char *var_name) {
	// グローバル変数を読む
	lua_getglobal(L, var_name);
	if(lua_isstring(L, -1)) {
		string ret = lua_tostring(L, -1);
		lua_pop(L, 1);
		return ret;
	} else {
		lua_pop(L, 1);
		throw LUA_VARIABLE_GET_EXCEPTION;
	}
}


// lua_getfieldの数値タイプのラッパー
int lua_getfield_numtype(lua_State *L, const char *key) {
	int result;
	lua_getfield(L, -1, key);
	if ( lua_isnumber(L, -1) ) {
		result = (int)lua_tonumber(L, -1);
		lua_pop(L, 1);
		return result;
	} else {
		lua_pop(L, 1);
		throw LUA_VARIABLE_GET_EXCEPTION;
	}
}

// lua_getfieldの文字列タイプのラッパー
string lua_getfield_strtype(lua_State *L, const char *key) {
	string result = "";
	lua_getfield(L, -1, key);
	if ( lua_isstring(L, -1) ) {
		result = lua_tostring(L, -1);
		lua_pop(L, 1);
		return result;
	} else {
		lua_pop(L, 1);
		throw LUA_VARIABLE_GET_EXCEPTION;
	}
}



// printf形式と同様の形式で、luaの記述を実行することができる。
int lua_DoStringFormat( const char *format, ... ) {
    char szBufCommand[512] = "";

    va_list arg;

    va_start(arg, format);
    vsprintf_s(szBufCommand, format, arg);
    va_end(arg);

	int ret =luaL_dostring(L, szBufCommand);
	// エラーがあれば、表示
	if (ret) {
		string errmsg = lua_tostring(L, -1);
		errmsg += "\n";
		OutputDebugString(errmsg.c_str());
	}
	return ret;
}


// エラー表示して、luaStateを閉じてしまう
void lua_error_and_exit(lua_State *L, const char *fmt, ...) {
	va_list argp;
	va_start(argp, fmt);
	vfprintf(stderr, fmt, argp);
	lua_OutputDebugStream(fmt, argp);
	va_end(argp);
	// lua_close(L);
	// exit(EXIT_FAILURE);
}



// スタックに何があるか表示
void lua_DumpStack(lua_State* L)
{
	int i;
	//スタックに積まれている数を取得する
	int stackSize = lua_gettop(L);
	for( i = stackSize; i >= 1; i-- ) {
		int type = lua_type(L, i);
		lua_OutputDebugStream("Stack[%2d-%10s] : ", i, lua_typename(L,type) );

		switch( type ) {
		case LUA_TNUMBER:
			//number型
			lua_OutputDebugStream("%f", lua_tonumber(L, i) );
			break;
		case LUA_TBOOLEAN:
			//boolean型
			if( lua_toboolean(L, i) ) {
				lua_OutputDebugStream("true");
			}else{
				lua_OutputDebugStream("false");
			}
			break;
		case LUA_TSTRING:
			//string型
			lua_OutputDebugStream("%s", lua_tostring(L, i) );
			break;
		case LUA_TNIL:
			//nil
			break;
		default:
			//その他の型
			lua_OutputDebugStream("%s", lua_typename(L, type));
			break;
		}
		lua_OutputDebugStream("\n");
	}
	lua_OutputDebugStream("\n");
}

