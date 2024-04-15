#include "WinTarget.h"

#include <shlwapi.h>
#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSMod.h"
#include "ScenarioMod.h"

#pragma comment(lib, "shlwapi.lib")

/*
79A32533   6A 40            PUSH 40
79A32535   68 E029A979      PUSH DDraw.79A929E0                      ; ASCII "20"
79A3253A   56               PUSH ESI
79A3253B   E8 301F0000      CALL DDraw.79A34470
79A32540   68 E029A979      PUSH DDraw.79A929E0                      ; ASCII "20"
79A32545   68 00100000      PUSH 1000
79A3254A   68 6042037A      PUSH DDraw.7A034260                      ; ASCII "kao_24bit\0020.bmp"
79A3254F   E8 AE1F0000      CALL DDraw.79A34502
79A32554   68 2CF3A379      PUSH DDraw.79A3F32C                      ; ASCII ".bmp"
79A32559   68 00100000      PUSH 1000
79A3255E   68 6042037A      PUSH DDraw.7A034260                      ; ASCII "kao_24bit\0020.bmp"
79A32563   E8 9A1F0000      CALL DDraw.79A34502
79A32568   83C4 28          ADD ESP,28
79A3256B   6A 00            PUSH 0
79A3256D   68 80000000      PUSH 80
79A32572   6A 03            PUSH 3
79A32574   6A 00            PUSH 0
79A32576   6A 00            PUSH 0
79A32578   68 00000080      PUSH 80000000                                  �����̏����̌�A������������������Ă��܂��΂悢�B
79A3257D   68 6042037A      PUSH DDraw.7A034260                      ; ASCII "kao_24bit\0020.bmp"
79A32582   FF15 88E0A379    CALL NEAR DWORD PTR DS:[<&KERNEL32.Creat>; KERNEL32.CreateFileA

*/


#define DDRAW_DLL_ENTRYPOINT_TO_PUSH_80000000						 0x2578					// DDraw�̐擪����A��L�́u79A32578   68 00000080      PUSH 80000000�v�̐擪��������A�h���X
#define DDRAW_DLL_FILENAME_ADDRESS_RELATIVE									(0x7A034260-0x79A30000)   // �t�@�C�������i�[����Ă���A�h���X

// DDraw�I���W�i���n���h��
extern HINSTANCE hDDrawMod;

void OnDrawKaoFullColorFileNameExecute() {
	// OutputDebugString("��O�����莞");
	int nFileNameAddress = (int)hDDrawMod + DDRAW_DLL_FILENAME_ADDRESS_RELATIVE;
	char *szFileName = (char *)nFileNameAddress;
	char overrideFileNameBuffer[MAX_PATH] = "";
	if (p_snOnRequestFullcolorFaceFileName) {
		char szLowerFileName[MAX_PATH] = "";
		strcpy(szLowerFileName, szFileName);
		_strlwr(szLowerFileName);

		p_snOnRequestFullcolorFaceFileName(szLowerFileName, overrideFileNameBuffer);

		// �L���ȕ����������Ă�����A�R�s�[
		if (strlen(overrideFileNameBuffer)) {
			if (PathFileExists(overrideFileNameBuffer)) {
				strncpy(szFileName, overrideFileNameBuffer, 98);
				szFileName[99] = '\0';
			}
		}
	}

	// OutputDebugStream("%s", (int)hDDrawMod + DDRAW_DLL_FILENAME_ADDRESS_RELATIVE);
}


int pDDrawJumpFromToOnDrawKaoFullColorFileName = NULL;
int pDDrawReturnLblFromOnDrawKaoFullColorFileName = NULL; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w

__declspec(naked) void WINAPI OnDrawKaoFullColorFileName() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnDrawKaoFullColorFileNameExecute();

	// �X�^�b�N�Ɉ����o��
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		push 0x80000000 // ���̏����������ɂ��̂܂�

		jmp pDDrawReturnLblFromOnDrawKaoFullColorFileName
	}
}


char cmdOnDDrawJumpFromwKaoFullColorFileName[6] = "\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteDDrawDrawKaoFullColorFileName() {
	pDDrawJumpFromToOnDrawKaoFullColorFileName = (int)hDDrawMod + DDRAW_DLL_ENTRYPOINT_TO_PUSH_80000000; // DDraw�̐擪����A��L�́u79A32578   68 00000080      PUSH 80000000�v�̐擪��������A�h���X
	pDDrawReturnLblFromOnDrawKaoFullColorFileName = pDDrawJumpFromToOnDrawKaoFullColorFileName + 5; // �P�s���̖��߂̎��̏ꏊ���Ԃ�ꏊ

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnDrawKaoFullColorFileName;
	int SubAddress = iAddress - (pDDrawJumpFromToOnDrawKaoFullColorFileName + 5);
	// �T�Ƃ����̂́A
	memcpy(cmdOnDDrawJumpFromwKaoFullColorFileName + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory(CommonGlobalHandle::curProcess, (LPVOID)(pDDrawJumpFromToOnDrawKaoFullColorFileName), cmdOnDDrawJumpFromwKaoFullColorFileName, 5, NULL); //5�o�C�g�̂ݏ�������
}
