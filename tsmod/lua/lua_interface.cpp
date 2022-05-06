
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

	szBufDebug[length - 1] = '\0'; // �ԕ�

    OutputDebugString( szBufDebug );
}


void LoadLuaInterpreter() {
	// Lua���J��
	L = luaL_newstate();

	// Lua�W���֐����g��
	luaL_openlibs(L);
}




void FreeLuaInterpreter() {
	// Lua�����
	lua_close(L);
}




// �w��̖��O�̋�e�[�u����Lua�Ƀv�b�V��
int PushEmptyTable(lua_State *L, char *szTableName) {
	lua_newtable(L);

	lua_setglobal(L, szTableName);

	return 1;
}

// lua_getglobal�̐��l�^�C�v�̃��b�p�[
double lua_getglobal_numtype(char *var_name) {
	// �O���[�o���ϐ���ǂ�
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

// lua_getglobal�̕�����^�C�v�̃��b�p�[
string lua_getglobal_strtype(char *var_name) {
	// �O���[�o���ϐ���ǂ�
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


// lua_getfield�̐��l�^�C�v�̃��b�p�[
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

// lua_getfield�̕�����^�C�v�̃��b�p�[
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



// printf�`���Ɠ��l�̌`���ŁAlua�̋L�q�����s���邱�Ƃ��ł���B
int lua_DoStringFormat( const char *format, ... ) {
    char szBufCommand[512] = "";

    va_list arg;

    va_start(arg, format);
    vsprintf_s(szBufCommand, format, arg);
    va_end(arg);

	int ret =luaL_dostring(L, szBufCommand);
	// �G���[������΁A�\��
	if (ret) {
		string errmsg = lua_tostring(L, -1);
		errmsg += "\n";
		OutputDebugString(errmsg.c_str());
	}
	return ret;
}


// �G���[�\�����āAluaState����Ă��܂�
void lua_error_and_exit(lua_State *L, const char *fmt, ...) {
	va_list argp;
	va_start(argp, fmt);
	vfprintf(stderr, fmt, argp);
	lua_OutputDebugStream(fmt, argp);
	va_end(argp);
	// lua_close(L);
	// exit(EXIT_FAILURE);
}



// �X�^�b�N�ɉ������邩�\��
void lua_DumpStack(lua_State* L)
{
	int i;
	//�X�^�b�N�ɐς܂�Ă��鐔���擾����
	int stackSize = lua_gettop(L);
	for( i = stackSize; i >= 1; i-- ) {
		int type = lua_type(L, i);
		lua_OutputDebugStream("Stack[%2d-%10s] : ", i, lua_typename(L,type) );

		switch( type ) {
		case LUA_TNUMBER:
			//number�^
			lua_OutputDebugStream("%f", lua_tonumber(L, i) );
			break;
		case LUA_TBOOLEAN:
			//boolean�^
			if( lua_toboolean(L, i) ) {
				lua_OutputDebugStream("true");
			}else{
				lua_OutputDebugStream("false");
			}
			break;
		case LUA_TSTRING:
			//string�^
			lua_OutputDebugStream("%s", lua_tostring(L, i) );
			break;
		case LUA_TNIL:
			//nil
			break;
		default:
			//���̑��̌^
			lua_OutputDebugStream("%s", lua_typename(L, type));
			break;
		}
		lua_OutputDebugStream("\n");
	}
	lua_OutputDebugStream("\n");
}

