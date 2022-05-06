#define _CRT_SECURE_NO_WARNINGS

#include "GameDataStruct.h"
#include "FuncSelectGundanDialog.h"
#include "CommonGlobalHandle.h"
/*
0045036E  |> 6A 00          /PUSH 0                                  ; /Arg5 = 00000000
00450370  |. 6A 00          |PUSH 0                                  ; |Arg4 = 00000000
00450372  |. 56             |PUSH ESI                                ; |Arg3
00450373  |. 68 7FCD0000    |PUSH 0CD7F                              ; |Arg2 = 0000CD7F (�R�c�I��p�Œ�B�\�����ڂ�I�ׂ�)
00450378  |. 68 D8934D00    |PUSH TENSHOU.004D93D8                   ; |Arg1 = 004D93D8
0045037D  |. E8 D7B30000    |CALL TENSHOU.0045B759                   ; \TENSHOU.0045B759
00450382  |. 66:8BF8        |MOV DI,AX
00450385  |. 83C4 14        |ADD ESP,14
*/

/*
0045B763  |. FF75 10        PUSH    DWORD PTR SS:[EBP+10]            ; |Arg4
0045B766  |. FF75 0C        PUSH    DWORD PTR SS:[EBP+C]             ; |Arg3
0045B769     6A 01          PUSH    1										���̏ꏊ�̐l�����A�I���\�l�� 1�Œ�ŏ����Ă��邪�A0�ɐ؂�ւ��\�Ƃ���B
0045B76B  |. FF75 08        PUSH    DWORD PTR SS:[EBP+8]             ; |Arg1
0045B76E  |. E8 EEFEFFFF    CALL    TENSHOU.0045B661                 ; \TENSHOU.0045B661
*/


//---------------------�I���R�c�_�C�A���O-------------------------
int pCallFromFuncSelectGundanDialog = 0x45B759; // ���XTENSHOU.EXE���ɂ�����CALL��
int iReturnOfSelectGundanDialog = 0; // �Ԃ�l�BAX�̕����̂ݍ̗p����B
WORD iArrayOfGundanList[GAMEDATASTRUCT_GUNDAN_NUM+1] = {}; // �\������ΏۂƂȂ�R�c���X�g�ԍ��B�Ō��0xFFFF�ŏI���Ƃ݂Ȃ��B
int iFilterOptionOfSelectGundanDialog = Filter::Gundan::�f�t�H���g;
static int iMultiGundanNumOfSelectGundanDialog = 1; // �I���\�ȌR�c�̍ő吔�B

void FuncSelectGundanDialogAsm() {

	__asm {
		push 0
		push 0
		push 1 
		push iFilterOptionOfSelectGundanDialog // �t�B���^�[�l
		push 0x4D93D8 // WORD�ŌR�c�̔ԍ��̔z����i�[���Ă�ꏊ�B�Ō�0xFFFF

		call pCallFromFuncSelectGundanDialog

		mov iReturnOfSelectGundanDialog, eax

		add  esp, 0x14
	}
}

// �ΏۂƂȂ�R�c���X�g�B
int FuncSelectGundanDialog(vector<int> list, int iFilterOption ) {

	// ���X�g�������Ȃ�Ȃɂ����Ȃ��B
	if ( list.size() == 0 ) {
		return 0xFFFF;
	}

	// �n���ꂽvector�`���̃��X�g��WORD�`���̔z��ւ̃R�s�[���Ă����B
	WORD *pCopy = iArrayOfGundanList;
	// �R�c�̃��X�g��z��`���ɂ���B
	for ( vector<int>::iterator it=list.begin(); it != list.end(); it++ ) {

		int iGundanID = *it;
		// �͈͊O�͖���
		if ( ! (0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM) ) {
			continue;
		}
		// �R�c�������Ȃ��ԍ��͖���
		if ( nb6gundan[iGundanID].leader == 0xFFFF ) {
			continue;
		}
		
		*pCopy = (WORD)iGundanID+1; // �z��p���ԍ��ɂ��Ĕz��Ɋi�[
		pCopy++;
	}
	
	// �R�s�[�σ|�C���^���P���i��ł��Ȃ��B==�P���L���ȌR�c�����Ȃ�����
	if ( pCopy == iArrayOfGundanList ) {
		return 0xFFFF;
	}

	*pCopy = 0xFFFF; // �Ō�̕�����0xFFFF�𑫂��B

	// 0x4D93D8 // WORD�ŌR�c�̔ԍ��̔z����i�[���Ă�ꏊ�B
	// �R�c���X�g�̓��e��tenshou.exe�������ւƕ����W�J�B
	memcpy( (void *)0x4D93D8, iArrayOfGundanList, sizeof(iArrayOfGundanList) );

	// �t�B���^�[�I�v�V�������㏑��
	iFilterOptionOfSelectGundanDialog = iFilterOption;

	__asm {
		push eax
		push ebx
		push ecx
		push edx
		push esp
		push esi
		push edi
	}

	// TENSHOU.EXE���̊֐��Ăяo��
	FuncSelectGundanDialogAsm();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

	iReturnOfSelectGundanDialog = iReturnOfSelectGundanDialog & 0xFFFF; // EAX�̂����AAX�̕����̂݁B
	if ( iReturnOfSelectGundanDialog == 0xFF || iReturnOfSelectGundanDialog == 0xFFFF ) { // �I�����Ȃ�����
		return 0xFFFF;
	} else {
		// ���Ԗڂ�I���������H �ł͂Ȃ��āA�Ώۂ̌R�c�ԍ��y�z��p�z�𒼐ڕԂ��B
		return iReturnOfSelectGundanDialog-1;
	}
}