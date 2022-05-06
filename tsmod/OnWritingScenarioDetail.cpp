#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "PluginMod.h"
#include "ScenarioMod.h"

#include <map>

using namespace std;



/*
00422D96  |. 47             INC     EDI
00422D97  |. 53             PUSH    EBX                                          ; /Arg8
00422D98  |. 53             PUSH    EBX                                          ; |Arg7
00422D99  |. 53             PUSH    EBX                                          ; |Arg6
00422D9A  |. 53             PUSH    EBX                                          ; |Arg5
00422D9B  |. 53             PUSH    EBX                                          ; |Arg4
00422D9C  |. 53             PUSH    EBX                                          ; |Arg3
00422D9D  |. 53             PUSH    EBX                                          ; |Arg2
00422D9E  |. 50             PUSH    EAX                                          ; |Arg1
00422D9F  |. E8 BC920400    CALL    TENSHOU.0046C060                             ; \TENSHOU.0046C060  �㕔
00422DA4  |. 83C4 20        ADD     ESP, 20
00422DA7  |. 8BCE           MOV     ECX, ESI
00422DA9  |. 50             PUSH    EAX
00422DAA  |. E8 9FB70800    CALL    TENSHOU.004AE54E
00422DAF  |. 68 90000000    PUSH    90
00422DB4  |. 8BCE           MOV     ECX, ESI
00422DB6  |. 6A 10          PUSH    10
00422DB8  |. E8 CBE6FDFF    CALL    TENSHOU.00401488
00422DBD  |. 55             PUSH    EBP
00422DBE  |. 53             PUSH    EBX                                          ; /Arg8
00422DBF  |. 53             PUSH    EBX                                          ; |Arg7
00422DC0  |. 53             PUSH    EBX                                          ; |Arg6
00422DC1  |. 53             PUSH    EBX                                          ; |Arg5
00422DC2  |. 53             PUSH    EBX                                          ; |Arg4
00422DC3  |. 53             PUSH    EBX                                          ; |Arg3
00422DC4  |. 53             PUSH    EBX                                          ; |Arg2
00422DC5  |. 57             PUSH    EDI                                          ; |Arg1
00422DC6  |. E8 95920400    CALL    TENSHOU.0046C060                             ; \TENSHOU.0046C060�@����
00422DCB  |. 83C4 20        ADD     ESP, 20
00422DCE  |. 8BCE           MOV     ECX, ESI
00422DD0  |. 50             PUSH    EAX
00422DD1  |. E8 78B70800    CALL    TENSHOU.004AE54E
00422DD6  |. 68 F4000000    PUSH    0F4
00422DDB  |. 8BCE           MOV     ECX, ESI
00422DDD  |. 6A 10          PUSH    10
00422DDF  |. E8 A4E6FDFF    CALL    TENSHOU.00401488
00422DE4  |. 55             PUSH    EBP
00422DE5  |. 53             PUSH    EBX                                          ; /Arg8
00422DE6  |. 53             PUSH    EBX                                          ; |Arg7
00422DE7  |. 47             INC     EDI                                          ; |
00422DE8  |. 53             PUSH    EBX                                          ; |Arg6
00422DE9  |. 53             PUSH    EBX                                          ; |Arg5
00422DEA  |. 53             PUSH    EBX                                          ; |Arg4
00422DEB  |. 53             PUSH    EBX                                          ; |Arg3
00422DEC  |. 53             PUSH    EBX                                          ; |Arg2
00422DED  |. 57             PUSH    EDI                                          ; |Arg1
00422DEE  |. E8 6D920400    CALL    TENSHOU.0046C060                             ; \TENSHOU.0046C060�@����
00422DF3  |. 83C4 20        ADD     ESP, 20
*/


//---------------------------�@
/*
 * �V�i���I�ڍו��́u�㕔�v��������鎞
 */

/*
00422D96  |. 47             INC     EDI
00422D97  |. 53             PUSH    EBX                                          ; /Arg8
00422D98  |. 53             PUSH    EBX                                          ; |Arg7
00422D99  |. 53             PUSH    EBX                                          ; |Arg6
00422D9A  |. 53             PUSH    EBX                                          ; |Arg5
00422D9B  |. 53             PUSH    EBX                                          ; |Arg4
00422D9C  |. 53             PUSH    EBX                                          ; |Arg3
00422D9D  |. 53             PUSH    EBX                                          ; |Arg2
00422D9E  |. 50             PUSH    EAX                                          ; |Arg1
00422D9F  |. E8 BC920400    CALL    TENSHOU.0046C060                             ; \TENSHOU.0046C060
00422DA4  |. 83C4 20        ADD     ESP, 20
*/

// �֐���char **�̃Z�b�g���o���Ă���
map<FARPROC, char **> psnGetProcAdressAndPointerMap;
void ReadFromTaretGeneralModDll(int nSubNo) {

	int nSN = getCurScenarioLocation();

	// PluginMod�n�̂O�ȏ�i�����\�j�Ȃc�k�k�̃��C���Q�[���J�n���̃��\�b�h���Ăяo���B
	// ���s�ΏۂƂȂ��Ă���DLL�����ɑ{�����āc
	for ( vector<HMODULE>::iterator it = listOfTargetDLL.begin(); it != listOfTargetDLL.end(); it++ ) {

		// �Ώۂ�DLL��MainGameStart�֐������݂��邩���`�F�b�N�B
		PFNTARGETMODWRITINGSCENARIODETAIL p_OnWritingScenarioDetail = (PFNTARGETMODWRITINGSCENARIODETAIL)GetProcAddress(*it, "OnWritingScenarioDetail");

		if ( !p_OnWritingScenarioDetail ) {
			continue;
		}

		if ( 0 <= nSN && nSN < SCENARIODATASTRUCT_NUM ) {
	
			char **psn = NULL;

			// �f�[�^�x�[�X�ɖ��o�^�ł���΁c
			if ( psnGetProcAdressAndPointerMap.find((FARPROC)p_OnWritingScenarioDetail) == psnGetProcAdressAndPointerMap.end() ) {
				// �󂯎��p�ɌĂяo����
				psn = p_OnWritingScenarioDetail( nSN+1 ); // 1�`6�ɒ����Ă����Ă�����B
				// �f�[�^�x�[�X�ɓo�^
				psnGetProcAdressAndPointerMap[(FARPROC)p_OnWritingScenarioDetail] = psn;

			// �f�[�^�x�[�X�ɓo�^����Ă���΁A���p
			} else {
				psn = psnGetProcAdressAndPointerMap[(FARPROC)p_OnWritingScenarioDetail];
			}

			// Target��DLL�������l��n���Ă����B
			if ( psn ) {

				if ( strlen(psn[nSubNo]) > 2 ) {
					// ��������
					WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(CPM.getMemoryAddress(LOC4)), psn[nSubNo], strlen(psn[nSubNo])+1, NULL);

				}
			}
		}
	}

	// ScenarioMod�̕�
	if ( p_snOnWritingScenarioDetail ) {

		if ( 0 <= nSN && nSN < SCENARIODATASTRUCT_NUM ) {

			char **psn = NULL;

			// �f�[�^�x�[�X�ɖ��o�^�ł���΁c
			if ( psnGetProcAdressAndPointerMap.find((FARPROC)p_snOnWritingScenarioDetail) == psnGetProcAdressAndPointerMap.end() ) {
				// �󂯎��p�ɌĂяo����
				psn = p_snOnWritingScenarioDetail( nSN+1 ); // 1�`6�ɒ����Ă����Ă�����B
				// �f�[�^�x�[�X�ɓo�^
				psnGetProcAdressAndPointerMap[(FARPROC)p_snOnWritingScenarioDetail] = psn;

			// �f�[�^�x�[�X�ɓo�^����Ă���΁A���p
			} else {
				psn = psnGetProcAdressAndPointerMap[(FARPROC)p_snOnWritingScenarioDetail];
			}


			// Target��DLL�������l��n���Ă����B
			if ( psn ) {

				if ( strlen(psn[nSubNo]) > 2 ) {
					// ��������
					WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(CPM.getMemoryAddress(LOC4)), psn[nSubNo], strlen(psn[nSubNo])+1, NULL);

				}
			}
		}
	}

}



void OnTenshouExeWritingScenarioDetail1stExecute() {
	ReadFromTaretGeneralModDll(0); // �܂���PluginMod�n�̈��Mod����ǂށB
}


/*
00422D9E  |. 50             PUSH    EAX                                          ; |Arg1
00422D9F  |. E8 BC920400    CALL    TENSHOU.0046C060                             ; \TENSHOU.0046C060
				�������� JMP TSMod.OnTSExeWritingScenarioDetail1st �Ə��������Ă����ۂ̏���
00422DA4  |. 83C4 20        ADD     ESP, 20
 */
int pTenshouExeJumpFromToOnTenshouExeWritingScenarioDetail1st    =0x422D9F; // �֐��͂��̃A�h���X����AOnTenshouExeWritingScenarioDetail1st�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeWritingScenarioDetail1st=0x46C060; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeWritingScenarioDetail1st =0x422DA4; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeWritingScenarioDetail1st() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeWritingScenarioDetail1st

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// �Q�[���f�[�^���v���C���[�^�[�����ς�����i�K�œǂݍ��ݍX�V���Ă���
	OnTenshouExeWritingScenarioDetail1stExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeWritingScenarioDetail1st
	}
}

char cmdOnTenshouExeJumpFromWritingScenarioDetail1st[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeWritingScenarioDetail1st() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeWritingScenarioDetail1st;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeWritingScenarioDetail1st + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeWritingScenarioDetail1st  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromWritingScenarioDetail1st+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeWritingScenarioDetail1st), cmdOnTenshouExeJumpFromWritingScenarioDetail1st, 5, NULL); //5�o�C�g�̂ݏ�������
}






//---------------------------�A
/*
 * �V�i���I�ڍו��́u�����v��������鎞
 */

/*
00422DBD  |. 55             PUSH    EBP
00422DBE  |. 53             PUSH    EBX                                          ; /Arg8
00422DBF  |. 53             PUSH    EBX                                          ; |Arg7
00422DC0  |. 53             PUSH    EBX                                          ; |Arg6
00422DC1  |. 53             PUSH    EBX                                          ; |Arg5
00422DC2  |. 53             PUSH    EBX                                          ; |Arg4
00422DC3  |. 53             PUSH    EBX                                          ; |Arg3
00422DC4  |. 53             PUSH    EBX                                          ; |Arg2
00422DC5  |. 57             PUSH    EDI                                          ; |Arg1
00422DC6  |. E8 95920400    CALL    TENSHOU.0046C060                             ; \TENSHOU.0046C060�@����
00422DCB  |. 83C4 20        ADD     ESP, 20
*/

void OnTenshouExeWritingScenarioDetail2ndExecute() {
	ReadFromTaretGeneralModDll(1); // �܂���PluginMod�n�̈��Mod����ǂށB
}


/*
00422DC5  |. 57             PUSH    EDI                                          ; |Arg1
00422DC6  |. E8 95920400    CALL    TENSHOU.0046C060                             ; \TENSHOU.0046C060�@����
				�������� JMP TSMod.OnTSExeWritingScenarioDetail2nd �Ə��������Ă����ۂ̏���
00422DCB  |. 83C4 20        ADD     ESP, 20
 */
int pTenshouExeJumpFromToOnTenshouExeWritingScenarioDetail2nd    =0x422DC6; // �֐��͂��̃A�h���X����AOnTenshouExeWritingScenarioDetail2nd�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeWritingScenarioDetail2nd=0x46C060; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeWritingScenarioDetail2nd =0x422DCB; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeWritingScenarioDetail2nd() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeWritingScenarioDetail2nd

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// �Q�[���f�[�^���v���C���[�^�[�����ς�����i�K�œǂݍ��ݍX�V���Ă���
	OnTenshouExeWritingScenarioDetail2ndExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeWritingScenarioDetail2nd
	}
}

char cmdOnTenshouExeJumpFromWritingScenarioDetail2nd[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeWritingScenarioDetail2nd() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeWritingScenarioDetail2nd;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeWritingScenarioDetail2nd + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeWritingScenarioDetail2nd  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromWritingScenarioDetail2nd+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeWritingScenarioDetail2nd), cmdOnTenshouExeJumpFromWritingScenarioDetail2nd, 5, NULL); //5�o�C�g�̂ݏ�������
}






//---------------------------�B
/*
 * �V�i���I�ڍו��́u�����v��������鎞
 */

/*
00422DE5  |. 53             PUSH    EBX                                          ; /Arg8
00422DE6  |. 53             PUSH    EBX                                          ; |Arg7
00422DE7  |. 47             INC     EDI                                          ; |
00422DE8  |. 53             PUSH    EBX                                          ; |Arg6
00422DE9  |. 53             PUSH    EBX                                          ; |Arg5
00422DEA  |. 53             PUSH    EBX                                          ; |Arg4
00422DEB  |. 53             PUSH    EBX                                          ; |Arg3
00422DEC  |. 53             PUSH    EBX                                          ; |Arg2
00422DED  |. 57             PUSH    EDI                                          ; |Arg1
00422DEE  |. E8 6D920400    CALL    TENSHOU.0046C060                             ; \TENSHOU.0046C060�@����
00422DF3  |. 83C4 20        ADD     ESP, 20
*/

void OnTenshouExeWritingScenarioDetail3rdExecute() {
	ReadFromTaretGeneralModDll(2); // �܂���PluginMod�n�̈��Mod����ǂށB
}


/*
00422DED  |. 57             PUSH    EDI                                          ; |Arg1
00422DEE  |. E8 6D920400    CALL    TENSHOU.0046C060                             ; \TENSHOU.0046C060�@����
				�������� JMP TSMod.OnTSExeWritingScenarioDetail3rd �Ə��������Ă����ۂ̏���
00422DF3  |. 83C4 20        ADD     ESP, 20
 */
int pTenshouExeJumpFromToOnTenshouExeWritingScenarioDetail3rd    =0x422DEE; // �֐��͂��̃A�h���X����AOnTenshouExeWritingScenarioDetail3rd�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeWritingScenarioDetail3rd=0x46C060; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeWritingScenarioDetail3rd =0x422DF3; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeWritingScenarioDetail3rd() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeWritingScenarioDetail3rd

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// �Q�[���f�[�^���v���C���[�^�[�����ς�����i�K�œǂݍ��ݍX�V���Ă���
	OnTenshouExeWritingScenarioDetail3rdExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeWritingScenarioDetail3rd
	}
}

char cmdOnTenshouExeJumpFromWritingScenarioDetail3rd[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeWritingScenarioDetail3rd() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeWritingScenarioDetail3rd;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeWritingScenarioDetail3rd + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeWritingScenarioDetail3rd  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromWritingScenarioDetail3rd+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeWritingScenarioDetail3rd), cmdOnTenshouExeJumpFromWritingScenarioDetail3rd, 5, NULL); //5�o�C�g�̂ݏ�������
}

