#define _CRT_SECURE_NO_WARNINGS

#include "GameDataStruct.h"
#include "FuncSelectKahouDIalog.h"

/*
00446673  |> 6A 00          /PUSH    0                               ; /Arg5 = 00000000
00446675  |. 6A 00          |PUSH    0                               ; |Arg4 = 00000000
00446677  |. 57             |PUSH    EDI                             ; |Arg3
00446678  |. 6A 1F          |PUSH    1F                              ; |Arg2 = 0000001F
0044667A  |. 68 50C84D00    |PUSH    TENSHOU.004DC850                ; |Arg1 = 004DC850
0044667F  |. E8 0FD70300    |CALL    TENSHOU.00483D93                ; \TENSHOU.00483D93
00446684  |. 83C4 14        |ADD     ESP, 14
00446687  |. 8BF0           |MOV     ESI, EAX

*/

//---------------------�I���ƕ�_�C�A���O-------------------------
int pCallFromFuncSelectKahouDialog = 0x483D93; // ���XTENSHOU.EXE���ɂ�����CALL��
int iReturnOfSelectKahouDialog = 0; // �Ԃ�l�BAX�̕����̂ݍ̗p����B
int iArrayOfKahouList[GAMEDATASTRUCT_KAHOU_NUM+1] = {}; // �\������ΏۂƂȂ�ƕ󃊃X�g�ԍ��B�Ō��0xFFFF�ŏI���Ƃ݂Ȃ��B
int iFilterOptionOfSelectKahouDialog = Filter::Kahou::�f�t�H���g;
void FuncSelectKahouDialogAsm() {

	__asm {
		push 0
		push 0
		push 1 // �I���\��(�A���ƕ�𕡐��I������v���͂Ȃ��͂�)
		push iFilterOptionOfSelectKahouDialog   // �\�����ڃI�v�V�����B0x3�Ƃ������̂�����B
		push 0x4DC850 // WORD�ŉƕ�̔ԍ��̔z����i�[���Ă�ꏊ�B�Ō�0xFFFF

		call pCallFromFuncSelectKahouDialog

		mov iReturnOfSelectKahouDialog, eax

		add  esp, 0x14
	}
}

// �ΏۂƂȂ�ƕ󃊃X�g�B
int FuncSelectKahouDialog(vector<int> list, int iFilterOption) {

	// ���X�g�������Ȃ�Ȃɂ����Ȃ��B
	if ( list.size() == 0 ) {
		return 0xFFFF;
	}

	// �n���ꂽvector�`���̃��X�g��WORD�`���̔z��ւ̃R�s�[���Ă����B
	int *pCopy = iArrayOfKahouList;
	// �ƕ�̃��X�g��z��`���ɂ���B
	for ( vector<int>::iterator it=list.begin(); it != list.end(); it++ ) {

		int iKahouID = *it;
		// �͈͊O�͖���
		if ( ! (0 <= iKahouID && iKahouID < GAMEDATASTRUCT_KAHOU_NUM) ) {
			continue;
		}
		
		*pCopy = iKahouID+1; // �z��p���ԍ��ɂ��Ĕz��Ɋi�[
		pCopy++;
	}
	
	// �R�s�[�σ|�C���^���P���i��ł��Ȃ��B==�P���L���ȉƕ󂪂P���Ȃ�����
	if ( pCopy == iArrayOfKahouList ) {
		return 0xFFFF;
	}

	*pCopy = 0xFFFF; // �Ō�̕�����0xFFFF�𑫂��B

	// 0x4DC850 // int�ŉƕ�̔ԍ��̔z����i�[���Ă�ꏊ�B
	// �ƕ󃊃X�g�̓��e��tenshou.exe�������ւƕ����W�J�B
	memcpy( (void *)0x4DC850, iArrayOfKahouList, sizeof(iArrayOfKahouList) );

	// �t�B���^�[�I�v�V�������㏑��
	iFilterOptionOfSelectKahouDialog = iFilterOption;

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
	FuncSelectKahouDialogAsm();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

	iReturnOfSelectKahouDialog = iReturnOfSelectKahouDialog & 0xFFFF; // EAX�̂����AAX�̕����̂݁B
	if ( iReturnOfSelectKahouDialog == 0xFF ||iReturnOfSelectKahouDialog == 0xFFFF ) { // �I�����Ȃ�����
		return 0xFFFF;
	} else {
		// ���Ԗڂ�I���������H �ł͂Ȃ��āA�Ώۂ̉ƕ�ԍ��y�z��p�z�𒼐ڕԂ��B
		return iReturnOfSelectKahouDialog-1;
	}
}