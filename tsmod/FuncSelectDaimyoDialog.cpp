#define _CRT_SECURE_NO_WARNINGS

#include "GameDataStruct.h"
#include "FuncSelectDaimyoDialog.h"
/*
0042534D  /$ FF7424 08      PUSH    DWORD PTR SS:[ESP+8]
00425351  |. 8B4C24 08      MOV     ECX, DWORD PTR SS:[ESP+8]
00425355  |. E8 63010500    CALL    TENSHOU.004754BD
0042535A  |. 33C0           XOR     EAX, EAX
0042535C  |. 50             PUSH    EAX
0042535D  |. 50             PUSH    EAX
0042535E  |. 6A 08          PUSH    8
00425360  |. 6A 02          PUSH    2
00425362  |. 50             PUSH    EAX
00425363  |. 68 9F071103    PUSH    311079F
00425368  |. E8 031D0400    CALL    TENSHOU.00467070
0042536D  |. 50             PUSH    EAX                              ; |Arg2
0042536E  |. 68 D8A54D00    PUSH    TENSHOU.004DA5D8                 ; |Arg1 = 004DA5D8
00425373  |. E8 F7D10200    CALL    TENSHOU.0045256F                 ; \TENSHOU.0045256F
00425378  |. 66:A3 FCE54B00 MOV     WORD PTR DS:[4BE5FC], AX
0042537E  |. 83C4 20        ADD     ESP, 20
00425381  \. C3             RETN
*/

/*
00415B93  |> 33C0           XOR     EAX, EAX
00415B95  |. B9 FFFF0000    MOV     ECX, 0FFFF
00415B9A  |. 50             PUSH    EAX
00415B9B  |. 50             PUSH    EAX
00415B9C  |. 51             PUSH    ECX
00415B9D  |. 51             PUSH    ECX
00415B9E  |. 50             PUSH    EAX
00415B9F  |. 68 9F071103    PUSH    311079F
00415BA4  |. E8 C7140500    CALL    TENSHOU.00467070
00415BA9  |. 50             PUSH    EAX                              ; |Arg2
00415BAA  |. 68 D8A54D00    PUSH    TENSHOU.004DA5D8                 ; |Arg1 = 004DA5D8
00415BAF  |. E8 BBC90300    CALL    TENSHOU.0045256F                 ; \TENSHOU.0045256F
00415BB4  |. 66:8BD8        MOV     BX, AX   �� AX���I��喼�ԍ�

*/

//---------------------�I��喼�_�C�A���O-------------------------
int pCallFromFuncSelectDaimyoDialog = 0x45256F; // ���XTENSHOU.EXE���ɂ�����CALL��
int pCallFromFuncSelectDaimyoDialogSub = 0x467070; // ���XTENSHOU.EXE���ɂ�����CALL��
int iDaimyoIDOfSelectDaimyoDialog = -1; // ��ςƂȂ�iDaimyoID�B�F�D�l�Ȃǂ�����̂ŁA��ϑ喼���K�v�B
int iReturnOfSelectDaimyoDialog = 0; // �Ԃ�l�Beax�̕����̂ݍ̗p����B
int iFilterOptionOfSelectDaimyoDialog = Filter::Daimyo::�f�t�H���g; // �t�B���^�[�I�v�V����
WORD iArrayOfDaimyoList[GAMEDATASTRUCT_DAIMYO_NUM+1] = {}; // �\������ΏۂƂȂ�喼���X�g�ԍ��B�Ō��0xFFFF�ŏI���Ƃ݂Ȃ��B

void FuncSelectDaimyoDialogAsm() {

	__asm {
		xor eax, eax
		mov ecx, 0xFFFF
	
		push eax      // �ʂ̂��� 0 
		push eax	  //            0 
		push ecx	  //            8 
		push ecx	  //            2 
		push 0x4001	  //            0 
		push iFilterOptionOfSelectDaimyoDialog

		// call pCallFromFuncSelectDaimyoDialogSub // �����̑喼���͂̔ԍ���eax�ɃZ�b�g
		mov eax, iDaimyoIDOfSelectDaimyoDialog
		push eax
		push 0x4DA5D8 // WORD�ő喼�̔ԍ��̔z����i�[���Ă�ꏊ�B�Ō�0xFFFF

		call pCallFromFuncSelectDaimyoDialog

		mov iReturnOfSelectDaimyoDialog, eax

		add  esp, 0x20
	}
}

// �ΏۂƂȂ�喼���X�g�B
int FuncSelectDaimyoDialog(vector<int> list, int iFilterOption, int iMainDaimyoID ) {

	// ���X�g�������Ȃ�Ȃɂ����Ȃ��B
	if ( list.size() == 0 ) {
		return 0xFFFF;
	}

	if ( iMainDaimyoID == -1 ) {
		iMainDaimyoID = nb6turn.daimyo-1;
	}
	if ( !(0 <= iMainDaimyoID && iMainDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM ) ) {
		return 0xFFFF;
	}
	iDaimyoIDOfSelectDaimyoDialog = iMainDaimyoID+1;

	// �n���ꂽvector�`���̃��X�g��WORD�`���̔z��ւ̃R�s�[���Ă����B
	WORD *pCopy = iArrayOfDaimyoList;
	// �喼�̃��X�g��z��`���ɂ���B
	for ( vector<int>::iterator it=list.begin(); it != list.end(); it++ ) {

		int iDaimyoID = *it;
		// �͈͊O�͖���
		if ( ! (0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM) ) {
			continue;
		}
		// �喼�����Ȃ��ԍ��͖���
		if ( nb6daimyo[iDaimyoID].attach == 0xFFFF ) {
			continue;
		}
		
		*pCopy = (WORD)iDaimyoID+1; // �z��p���ԍ��ɂ��Ĕz��Ɋi�[
		pCopy++;
	}
	
	// �R�s�[�σ|�C���^���P���i��ł��Ȃ��B==�P���L���ȑ喼�����Ȃ�����
	if ( pCopy == iArrayOfDaimyoList ) {
		return 0xFFFF;
	}

	*pCopy = 0xFFFF; // �Ō�̕�����0xFFFF�𑫂��B

	// 0x4DA5D8 // WORD�ő喼�̔ԍ��̔z����i�[���Ă�ꏊ�B
	// �喼���X�g�̓��e��tenshou.exe�������ւƕ����W�J�B
	memcpy( (void *)0x4DA5D8, iArrayOfDaimyoList, sizeof(iArrayOfDaimyoList) );

	// �t�B���^�[�I�v�V�������㏑��
	// ���R�����g�A�E�g!! tenshou.exe ���ł��̃t�B���^�[�I�v�V��������������Ă��Ȃ�!!
	iFilterOptionOfSelectDaimyoDialog = iFilterOption;

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
	FuncSelectDaimyoDialogAsm();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

	iReturnOfSelectDaimyoDialog = iReturnOfSelectDaimyoDialog & 0xFFFF; // EAX�̂����AAX�̕����̂݁B
	if ( iReturnOfSelectDaimyoDialog == 0xFF || iReturnOfSelectDaimyoDialog == 0xFFFF ) { // �I�����Ȃ�����
		return 0xFFFF;
	} else {
		// ���Ԗڂ�I���������H �ł͂Ȃ��āA�Ώۂ̑喼�ԍ��y�z��p�z�𒼐ڕԂ��B
		return iReturnOfSelectDaimyoDialog-1;
	}
}