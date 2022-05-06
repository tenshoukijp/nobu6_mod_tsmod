#pragma once

#include <string>
#include "lua.hpp"

using namespace std;

extern lua_State* L;
extern int lua_State_Err;

// lua_State�̎擾
__declspec(dllexport) lua_State* luaL_getstate(void);

// lua_State���A�ŏ��̃t�@�C�����s���ɉ��炩�̃G���[��ԂɂȂ������ǂ����̃t���O�BTSMod�ŁA�G���[�ɂȂ��Ă��Ȃ����Ƃ�O��Ƃ��ď�����i�߂邽�߁B
__declspec(dllexport) int luaL_getstate_err(void);


void lua_OutputDebugStream( const char *format, ... );

// lua�C���^�v���^���[�h
void LoadLuaInterpreter();
// lua�C���^�v���^���
void FreeLuaInterpreter();

// lua�̃X�^�b�N�̃_���v
void lua_DumpStack(lua_State* L);

// error�\�����ăC���^�v���^��exit����B
void lua_error_and_exit(lua_State *L, const char *fmt, ...);


// �w��̖��O�̋�e�[�u����Lua�Ƀv�b�V��
int PushEmptyTable(lua_State *L, char *szTableName);

// lua_getglobal�̐��l�^�C�v�̃��b�p�[
double lua_getglobal_numtype(char *var_name);

// lua_getglobal�̕�����^�C�v�̃��b�p�[
string lua_getglobal_strtype(char *var_name);

// lua_getfield�̐��l�^�C�v�̃��b�p�[
int lua_getfield_numtype(lua_State *L, const char *key);

// lua_getfield�̕�����^�C�v�̃��b�p�[
string lua_getfield_strtype(lua_State *L, const char *key);

// printf�`���Ɠ��l�̌`���ŁAlua�̋L�q�����s���邱�Ƃ��ł���B
int lua_DoStringFormat( const char *format, ... );

