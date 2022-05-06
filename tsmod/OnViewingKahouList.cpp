#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"

#include "KahouType.h"

/*
 * �ƕ󃊃X�g��������
 */

// �ƕ�ԍ�
int iKahouIDOnViewingKahouList = -1;

// �ƕ�̎�ޖ��������Ă��镶����̃|�C���^
int iKahouTypePointerOnViewingKahouList = 0;

void OnTenshouExeViewingKahouListExecute() {

	iKahouIDOnViewingKahouList = iKahouIDOnViewingKahouList - 1;

	iKahouTypePointerOnViewingKahouList = NULL; // ���xNULL�ŏ�����

	// �ƕ�͈̔͂��ǂ������`�F�b�N���āc
	if ( 0 <= iKahouIDOnViewingKahouList && iKahouIDOnViewingKahouList < GAMEDATASTRUCT_KAHOU_NUM ) {

		char *pszCustomKahouType = GetOverWriteKahouTypeMeiPointer(iKahouIDOnViewingKahouList);

		// �ƕ��ޖ��̓���ȏ����������Ԃ��Đ؂ꂢ��΁c
		if ( pszCustomKahouType ) {
			// �z��ɉƕ󖼂�����B
			strcpy( nb6kahou_custom_type_string[iKahouIDOnViewingKahouList].szKahouTypeString, pszCustomKahouType);

			// �z��ɓ��ꂽ�ꏊ���A�\�����ׂ��|�C���^�Ƃ��ēn��
			iKahouTypePointerOnViewingKahouList = (int)nb6kahou_custom_type_string[iKahouIDOnViewingKahouList].szKahouTypeString;
		}

		// ScenarioMod�œƓ��̉ƕ��ޖ��ω������邩�ǂ����B
		if ( p_snOnViewingKahouTypeName ) {
			// ���S�����đ傫�ڂ�
			char szTempName[20] = "";
			bool isReWrite = ((PFNSNONVIEWINGKAHOUTYPENAME)p_snOnViewingKahouTypeName)( szTempName, iKahouIDOnViewingKahouList+1 );
			if ( isReWrite ) {
				szTempName[10] = NULL; // �S�p�T�����܂� 

				// �z��ɉƕ󖼂�����B
				strcpy( nb6kahou_custom_type_string[iKahouIDOnViewingKahouList].szKahouTypeString, szTempName);

				// �z��ɓ��ꂽ�ꏊ���A�\�����ׂ��|�C���^�Ƃ��ēn��
				iKahouTypePointerOnViewingKahouList = (int)nb6kahou_custom_type_string[iKahouIDOnViewingKahouList].szKahouTypeString;
			}
		}

	}
}


/*
004839EF   . FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
004839F3   . E8 79C0FCFF    CALL    TENSHOU.0044FA71
				�������� JMP TSMod.OnTSExeViewingKahouList �Ə��������Ă����ۂ̏���
004839F8   . 83C4 04        ADD     ESP, 4
004839FB   . 8B0485 20144D0>MOV     EAX, DWORD PTR DS:[EAX*4+4D1420] ;  TENSHOU.004CFE64
00483A02   . C3             RETN
*/
int pTenshouExeJumpFromToOnTenshouExeViewingKahouList		=0x4839F3; // �֐��͂��̃A�h���X����AOnTenshouExeViewingKahouList�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeViewingKahouList   =0x44FA71; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeViewingKahouList	=0x483A02; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
int pTenshouExeReturnLblFromOnTenshouExeViewingKahouListNoCustomCase	=0x4839F8; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeViewingKahouList() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// ���悤�Ƃ��Ă���ƕ�ԍ���ECX�ɓ����Ă���B
		mov iKahouIDOnViewingKahouList, ecx		

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeViewingKahouList

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
	OnTenshouExeViewingKahouListExecute();

	// �J�X�^�������񂪂������ꍇ
	if ( iKahouTypePointerOnViewingKahouList ) {

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

			// ���X�̏�����������
			add esp, 4

			// 
			MOV EAX, DWORD PTR DS:[iKahouTypePointerOnViewingKahouList]

			jmp pTenshouExeReturnLblFromOnTenshouExeViewingKahouList
		}

	// �J�X�^�������񂪖����ꍇ
	} else {
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

			jmp pTenshouExeReturnLblFromOnTenshouExeViewingKahouListNoCustomCase
		}
	}
}


char cmdOnTenshouExeJumpFromViewingKahouList[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeViewingKahouList() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeViewingKahouList;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingKahouList + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingKahouList  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromViewingKahouList+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingKahouList), cmdOnTenshouExeJumpFromViewingKahouList, 5, NULL); //5�o�C�g�̂ݏ�������
}




