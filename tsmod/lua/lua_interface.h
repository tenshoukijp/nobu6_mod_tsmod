#pragma once

#include <string>
#include "lua.hpp"

using namespace std;

extern lua_State* L;
extern int lua_State_Err;

// lua_Stateの取得
__declspec(dllexport) lua_State* luaL_getstate(void);

// lua_Stateが、最初のファイル実行時に何らかのエラー状態になったかどうかのフラグ。TSModで、エラーになっていないことを前提として処理を進めるため。
__declspec(dllexport) int luaL_getstate_err(void);


void lua_OutputDebugStream( const char *format, ... );

// luaインタプリタロード
void LoadLuaInterpreter();
// luaインタプリタ解放
void FreeLuaInterpreter();

// luaのスタックのダンプ
void lua_DumpStack(lua_State* L);

// error表示してインタプリタをexitする。
void lua_error_and_exit(lua_State *L, const char *fmt, ...);


// 指定の名前の空テーブルをLuaにプッシュ
int PushEmptyTable(lua_State *L, char *szTableName);

// lua_getglobalの数値タイプのラッパー
double lua_getglobal_numtype(char *var_name);

// lua_getglobalの文字列タイプのラッパー
string lua_getglobal_strtype(char *var_name);

// lua_getfieldの数値タイプのラッパー
int lua_getfield_numtype(lua_State *L, const char *key);

// lua_getfieldの文字列タイプのラッパー
string lua_getfield_strtype(lua_State *L, const char *key);

// printf形式と同様の形式で、luaの記述を実行することができる。
int lua_DoStringFormat( const char *format, ... );

