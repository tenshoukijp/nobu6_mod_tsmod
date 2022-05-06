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
 IOProcessMemory�n�錾
 *----------------------------------------------------------------*/

//----------------------------------------------------------------

/*----------------------------------------------------------------*
�����񏈗��n�錾�B
�����̊֐��͂����g���Ă��Ȃ��BLua�n�ւƕύX���ꂽ!!
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

		// �R�����g�Ȃ玟
		if (szCurLine[0] == '#' || szCurLine[0] == '/') {
			continue;
		}

		/* \t����؂�ɕ�����𒊏o */
		int iTokenIX = 0;
		iTargetMemoryAddress = 0;
		char *tp = NULL; // ���ݎw���Ă镶����̏ꏊ
		bool isModeAsm = false;
		bool isModeCmd = false;

		for ( tp = strtok( szCurLine, ":" ), iTokenIX=0; tp != NULL; tp = strtok( NULL, "\n" ), iTokenIX++ ) {
			if (iTokenIX==0) {
				// cout << tp << endl;
				sscanf(tp, "%x", &iTargetMemoryAddress);

			// 2�Ԗڂ�asm�ɂȂ��Ă邩���ׂ�
			} else if (iTokenIX==1 && !strncmp(tp, "asm", 3)) {
				isModeAsm = true;
				strcpy(szPatchTokenBuff, tp+strlen("asm:")); // �asm:� �������ăR�s�[����̂Łc
			} else if (iTokenIX==1 && !strncmp(tp, "cmd", 3)) {
				isModeCmd = true;
				strcpy(szPatchTokenBuff, tp+strlen("cmd:")); // �asm:� �������ăR�s�[����̂Łc
			} else if (iTokenIX==1 && !strncmp(tp, "txt", 3)) {
				strcpy(szPatchTokenBuff, tp+strlen("txt:")); // �txt:� �������ăR�s�[����̂Łc
			} else {
				strcpy(szPatchTokenBuff, tp);
				//cout << tp << num_buf << endl;
			}
			
		}
		if (iTargetMemoryAddress) {
			// �A�Z���u�����[�h
			if (isModeAsm ) {
				// �A�Z���u���R�[�h�𕪉�
				char *p_asm = NULL; // ���ݎw���Ă镶����̏ꏊ
				int iAsmCodeIX = 0;
				int byteCurAsm = 0; // ���݂�asm������(2����)��char�^�ɕϊ������ۂ̊i�[�ꏊ
				// asm �R�[�h�� �:��ŕ������Ă䂫�A1��1�񏑂��Ă䂭�B
				for ( p_asm = strtok( szPatchTokenBuff, ":" ); p_asm != NULL; p_asm = strtok( NULL, ":" ) ) {
					sscanf_s(p_asm, "%x", &byteCurAsm); // 16�i�����������l
					WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(iTargetMemoryAddress+iAsmCodeIX), &byteCurAsm, 1, NULL);
					iAsmCodeIX++;
				}

			// �R�}���h���[�h
			} else if (isModeCmd ) {
				
				// �R�}���h�R�[�h�𕪉�
				char *p_cmd = NULL; // ���ݎw���Ă镶����̏ꏊ
				int iCmdCodeIX = 0;
				// cmd �R�[�h�� �:��ŕ�������A�����A�ucmd:�R�}���h��:�R�}���h���x���v�ƂȂ��Ă���B
				char szCurCmdTokenBuf[256] = "";
				int byteCurCmdLv = 0;
				for ( p_cmd = strtok( szPatchTokenBuff, ":" ); p_cmd != NULL; p_cmd = strtok( NULL, ":" ) ) {
					// �P�Ԗڂ́A��̓I�ȃR�}���h������
					if (iCmdCodeIX==0) {
						// �R�}���h��������R�s�[
						strcpy( szCurCmdTokenBuf, p_cmd );

					// �Q�Ԗڂ́A�R�}���h�̃��x��
					} else if (iCmdCodeIX==1) {
						// �R�}���h������𐔒l�ɂ��ăR�s�[
						sscanf_s(p_cmd, "%d", &byteCurCmdLv);
					}
					iCmdCodeIX++;
				}

				TSModCommand::setCommandSwitch(szCurCmdTokenBuf, byteCurCmdLv);
				
				// �A�Z���u���ł͂Ȃ����ʂ̕�����^�C�v
			} else {
				WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(iTargetMemoryAddress), szPatchTokenBuff, strlen(szPatchTokenBuff)+1, NULL);
			}
		}

	}

}

// ������̕���
extern vector<string> string_split(const string &str, const string &delim);

// asm�̃e�[�u���̒l���擾
int ImportAsmTable(lua_State *L) {

	lua_getglobal(L, "asm");

	lua_pushnil(L);
	while (lua_next(L, -2) != 0) {
		lua_pushvalue(L, -2);

		string strMemoryAddress = lua_tostring(L, -1);
		const char *pszValue = lua_tostring(L, -2);

		// �L�[��16�i���̕�����𐔒l�ɕύX
		int iMemoryAddress = 0;
		sscanf(strMemoryAddress.c_str(), "%x", &iMemoryAddress);

		// �A�Z���u���R�[�h�𕪉�
		char *p_asm = NULL; // ���ݎw���Ă镶����̏ꏊ
		int iAsmCodeIX = 0; // �A�h���X�̐i�s�B�P��byteCurAsm����������A�P�o�C�g�������Ƃ������ƂȂ̂ŁAchar�A�h���X���P�i�s
		int byteCurAsm = 0; // ���݂�asm������(2����)��char�^�ɕϊ������ۂ̊i�[�ꏊ

		vector<string> string_list = string_split(pszValue, ":");

		for ( vector<string>::iterator it = string_list.begin(); it != string_list.end(); it++) {

			sscanf_s(it->c_str(), "%x", &byteCurAsm); // 16�i�����������l
			WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(iMemoryAddress+iAsmCodeIX), &byteCurAsm, 1, NULL);
			iAsmCodeIX++;
		}

		lua_pop(L, 2);
	}

	lua_pop(L, 1);

	return true;
}

// txt�̃e�[�u���̒l���擾
int ImportTxtTable(lua_State *L) {

	lua_getglobal(L, "txt");

	lua_pushnil(L);
	while (lua_next(L, -2) != 0) {
		lua_pushvalue(L, -2);

		string strMemoryAddress = lua_tostring(L, -1);
		string value = lua_tostring(L, -2);

		// �L�[��16�i���̕�����𐔒l�ɕύX
		int iMemoryAddress = 0;
		sscanf(strMemoryAddress.c_str(), "%x", &iMemoryAddress);

		// �L�[�̃A�h���X�ɁA�l�̕��������������
		WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(iMemoryAddress), value.c_str(), strlen( value.c_str() )+1, NULL);

		lua_pop(L, 2);
	}

	lua_pop(L, 1);

	return false;
}



// cmd�̃e�[�u���̒l���擾
int ImportCmdTable(lua_State* L) {
	lua_getglobal(L, "cmd");

	lua_pushnil(L);
	while (lua_next(L, -2) != 0) {
		lua_pushvalue(L, -2);

		string key = lua_tostring(L, -1);
		int value = lua_tointeger(L, -2);

		// �e��ϐ��ɐݒ肷��B
		TSModCommand::setCommandSwitch((char *)key.c_str(), value);

		lua_pop(L, 2);
	}

	// �X�^�b�N���폜
	lua_pop(L, 1);

	return 1;
}


/*----------------------------------------------------------------*
 Lua����TSMod.ini��ǂݍ���
 *----------------------------------------------------------------*/
void ImportTSModIni(char *szFileName) {

	// BGMmm�̒i�K�ŁAlua_State���G���[�ɂȂ��Ă��Ȃ���΁c
	if (! luaL_getstate_err() ) {

		// �܂���cmd�e�[�u���̃C���|�[�g
		ImportCmdTable(L);
		// ����txt�e�[�u���̃C���|�[�g
		ImportTxtTable(L);
		// �Ō��asm�e�[�u���̃C���|�[�g
		ImportAsmTable(L);
	}
}



/*----------------------------------------------------------------*
 IOProcessMemory�n����
 *----------------------------------------------------------------*/
bool isDonePatchProcessMemory = false;

void TDModPatchProcessMemory(int iMode) { 
	
	if ( isDonePatchProcessMemory == false ) {
		isDonePatchProcessMemory = true;
	}

	struct stat  st;
	// TSMod.ini������΁c
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


