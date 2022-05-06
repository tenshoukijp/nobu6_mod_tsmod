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
 * �ƕ�ڍ׉�ʂ��J���ۂɁA�u�ƕ��ނ̒l�v���Ăяo����鎞�B
 */


// �ƕ��ID��EAX�ł킽���Ă���B�ƕ�ԍ��B����Ĕz��Ŏg�����߂ɂ́A-1���邱�ƁB
int iKahouIDOnViewingKahouDetailKahouType = -1;

// �ƕ�̎�ޖ��������Ă��镶����̃|�C���^
int iKahouTypePointerOnViewingKahouDetailKahouType = 0;

// ���̊֐��͈�u�łQ��Ăяo�����B
void OnTenshouExeViewingKahouDetailKahouTypeExecute() {
	// esi�̒l����A�ƕ󂪓����Ă���ꏊ��˂��~�߁A���̒l��iKahouIDOnViewingKahouDetailKahouType�ɉ��߂ē����B
	// MOV     EBP, DWORD PTR DS:[ESI+70] �Ɠ������Ƃ���낤�Ƃ����̂��B
	char *p = (char *)iKahouIDOnViewingKahouDetailKahouType + 0x70;
	iKahouIDOnViewingKahouDetailKahouType = *p;
	iKahouIDOnViewingKahouDetailKahouType--; // �ԍ����h�c��

	if ( 0 <= iKahouIDOnViewingKahouDetailKahouType && iKahouIDOnViewingKahouDetailKahouType < GAMEDATASTRUCT_KAHOU_NUM ) {

		char *pszCustomKahouType = GetOverWriteKahouTypeMeiPointer(iKahouIDOnViewingKahouDetailKahouType);

		// �ƕ��ޖ��̓���ȏ����������Ԃ��Đ؂ꂢ��΁c
		if ( pszCustomKahouType ) {
			// �z��ɉƕ󖼂�����B
			strcpy( nb6kahou_custom_type_string[iKahouIDOnViewingKahouDetailKahouType].szKahouTypeString, pszCustomKahouType);

			// �z��ɓ��ꂽ�ꏊ���A�\�����ׂ��|�C���^�Ƃ��ēn��
			iKahouTypePointerOnViewingKahouDetailKahouType = (int)nb6kahou_custom_type_string[iKahouIDOnViewingKahouDetailKahouType].szKahouTypeString;
		}

		// ScenarioMod�œƓ��̉ƕ��ޖ��ω������邩�ǂ����B
		if ( p_snOnViewingKahouTypeName ) {
			// ���S�����đ傫�ڂ�
			char szTempName[20] = "";
			bool isReWrite = ((PFNSNONVIEWINGKAHOUTYPENAME)p_snOnViewingKahouTypeName)( szTempName, iKahouIDOnViewingKahouDetailKahouType+1 );
			if ( isReWrite ) {
				szTempName[10] = NULL; // �S�p�T�����܂� 

				// �z��ɉƕ󖼂�����B
				strcpy( nb6kahou_custom_type_string[iKahouIDOnViewingKahouDetailKahouType].szKahouTypeString, szTempName);

				// �z��ɓ��ꂽ�ꏊ���A�\�����ׂ��|�C���^�Ƃ��ēn��
				iKahouTypePointerOnViewingKahouDetailKahouType = (int)nb6kahou_custom_type_string[iKahouIDOnViewingKahouDetailKahouType].szKahouTypeString;
			}
		}
	}
}


/*
0049493D  |. FF76 70        PUSH    DWORD PTR DS:[ESI+70]
00494940  |. E8 3DB1FBFF    CALL    TENSHOU.0044FA82
				�������� JMP TSMod.OnTSExeViewingKahouDetailKahouType �Ə��������Ă����ۂ̏���
00494945  |. 83C4 04        ADD     ESP, 4
*/
int pTenshouExeJumpFromToOnTenshouExeViewingKahouDetailKahouType	 =0x494940; // �֐��͂��̃A�h���X����AOnTenshouExeViewingKahouDetailKahouType�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeViewingKahouDetailKahouType =0x44FA82; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeViewingKahouDetailKahouType	 =0x494945; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeViewingKahouDetailKahouType() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// �B
		mov iKahouIDOnViewingKahouDetailKahouType, esi // �ƕ�h�c�֘A�A�h���X���R�s�[

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeViewingKahouDetailKahouType

		// ���̌��ʓ�����EAX���A�ƕ�̃^�C�v�̕�����|�C���^���̂��́B
		mov iKahouTypePointerOnViewingKahouDetailKahouType, eax;

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
	OnTenshouExeViewingKahouDetailKahouTypeExecute();

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

		// �|�C���^���㏑������
		mov eax, iKahouTypePointerOnViewingKahouDetailKahouType

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingKahouDetailKahouType
	}
}


char cmdOnTenshouExeJumpFromViewingKahouDetailKahouType[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeViewingKahouDetailKahouType() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeViewingKahouDetailKahouType;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingKahouDetailKahouType + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingKahouDetailKahouType  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromViewingKahouDetailKahouType+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingKahouDetailKahouType), cmdOnTenshouExeJumpFromViewingKahouDetailKahouType, 5, NULL); //5�o�C�g�̂ݏ�������
}




