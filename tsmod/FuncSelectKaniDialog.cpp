#define _CRT_SECURE_NO_WARNINGS

#include "GameDataStruct.h"
#include "FuncSelectKaniDIalog.h"

/*
00401B0A  |> 53             /PUSH EBX                                ; /Arg5 = 0
00401B0B  |. 53             |PUSH EBX                                ; |Arg4 = 0
00401B0C  |. 53             |PUSH EBX                                ; |Arg3 = 0
00401B0D  |. 6A 0F          |PUSH 0F                                 ; |Arg2 = 0000000F �� �I���I�v�V����
00401B0F  |. 68 00B14D00    |PUSH TENSHOU.004DB100                   ; |Arg1 = 004DB100 �� �z��i�[�̏ꏊ
00401B14  |. E8 09750000    |CALL TENSHOU.00409022                   ; \TENSHOU.00409022
00401B19  |. 83C4 14        |ADD ESP,14
00401B1C  |. 8BF8           |MOV EDI,EAX
*/

//---------------------�I�����ʃ_�C�A���O-------------------------
int pCallFromFuncSelectKaniDialog = 0x409022; // ���XTENSHOU.EXE���ɂ�����CALL��
int iReturnOfSelectKaniDialog = 0; // �Ԃ�l�BAX�̕����̂ݍ̗p����B
int iArrayOfKaniList[GAMEDATASTRUCT_KANI_NUM+1] = {}; // �\������ΏۂƂȂ銯�ʃ��X�g�ԍ��B�Ō��0xFFFF�ŏI���Ƃ݂Ȃ��B
int iFilterOptionOfSelectKaniDialog = Filter::Kani::�f�t�H���g;
void FuncSelectKaniDialogAsm() {

	__asm {
		push 0
		push 0
		push 0 // �I���\��(�A�����ʂ𕡐��I������v���͂Ȃ��͂�)
		push iFilterOptionOfSelectKaniDialog   // �\�����ڃI�v�V�����B0x3�Ƃ������̂�����B
		push 0x4DB100 // WORD�Ŋ��ʂ̔ԍ��̔z����i�[���Ă�ꏊ�B�Ō�0xFFFF

		call pCallFromFuncSelectKaniDialog

		mov iReturnOfSelectKaniDialog, eax

		add  esp, 0x14
	}
}

// �ΏۂƂȂ銯�ʃ��X�g�B
int FuncSelectKaniDialog(vector<int> list, int iFilterOption) {

	// ���X�g�������Ȃ�Ȃɂ����Ȃ��B
	if ( list.size() == 0 ) {
		return 0xFFFF;
	}

	// �n���ꂽvector�`���̃��X�g��WORD�`���̔z��ւ̃R�s�[���Ă����B
	int *pCopy = iArrayOfKaniList;
	// ���ʂ̃��X�g��z��`���ɂ���B
	for ( vector<int>::iterator it=list.begin(); it != list.end(); it++ ) {

		int iKaniID = *it;
		// �͈͊O�͖���
		if ( ! (0 <= iKaniID && iKaniID < GAMEDATASTRUCT_KANI_NUM) ) {
			continue;
		}
		
		*pCopy = iKaniID+1; // �z��p���ԍ��ɂ��Ĕz��Ɋi�[
		pCopy++;
	}
	
	// �R�s�[�σ|�C���^���P���i��ł��Ȃ��B==�P���L���Ȋ��ʂ��P���Ȃ�����
	if ( pCopy == iArrayOfKaniList ) {
		return 0xFFFF;
	}

	*pCopy = 0xFFFF; // �Ō�̕�����0xFFFF�𑫂��B

	// 0x4DB100 // int�Ŋ��ʂ̔ԍ��̔z����i�[���Ă�ꏊ�B
	// ���ʃ��X�g�̓��e��tenshou.exe�������ւƕ����W�J�B
	memcpy( (void *)0x4DB100, iArrayOfKaniList, sizeof(iArrayOfKaniList) );

	// �t�B���^�[�I�v�V�������㏑��
	iFilterOptionOfSelectKaniDialog = iFilterOption;

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
	FuncSelectKaniDialogAsm();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

	iReturnOfSelectKaniDialog = iReturnOfSelectKaniDialog & 0xFFFF; // EAX�̂����AAX�̕����̂݁B
	if ( iReturnOfSelectKaniDialog == 0xFF ||iReturnOfSelectKaniDialog == 0xFFFF ) { // �I�����Ȃ�����
		return 0xFFFF;
	} else {
		// ���Ԗڂ�I���������H �ł͂Ȃ��āA�Ώۂ̊��ʔԍ��y�z��p�z�𒼐ڕԂ��B
		return iReturnOfSelectKaniDialog-1;
	}
}