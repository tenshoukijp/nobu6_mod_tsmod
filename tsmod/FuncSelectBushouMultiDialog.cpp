#define _CRT_SECURE_NO_WARNINGS

#include "GameDataStruct.h"
#include "FuncSelectBushouDialog.h"
/*
0040199E  |. 6A 00          PUSH 0                                   ; /Arg9 = 00000000
004019A0  |. 6A 00          PUSH 0                                   ; |Arg8 = 00000000
004019A2  |. 57             PUSH EDI                                 ; |Arg7 = 0xFFFF
004019A3  |. 6A 0A          PUSH 0A                                  ; |Arg6 = 0000000A
004019A5  |. 68 80100000    PUSH 1080                                ; |Arg5 = 00001080
004019AA  |. 6A 00          PUSH 0                                   ; |Arg4 = 00000000
004019AC  |. 68 103C0900    PUSH 93C10                               ; |Arg3 = 00093C10 (�\���I���I�v�V�����B3FC93F9D���ƑS��.python��bin�Œ������B)
004019B1  |. FF75 FC        PUSH DWORD PTR SS:[EBP-4]                ; |Arg2 = 5 ( �I���\�l�� )
004019B4  |. FF76 70        PUSH DWORD PTR DS:[ESI+70]               ; |Arg1 = 0x4DBD40(�Œ�)
004019B7  |. E8 797C0300    CALL TENSHOU.00439635                    ; \TENSHOU.00439635
004019BC  |. 83C4 24        ADD ESP,24
004019BF  |. 8BD8           MOV EBX,EAX


0042F405  |> 6A 00          PUSH    0                                ; /Arg9 = 00000000
0042F407  |. 6A 00          PUSH    0                                ; |Arg8 = 00000000
0042F409  |. 50             PUSH    EAX                              ; |Arg7 = 0xFFFF
0042F40A  |. 55             PUSH    EBP                              ; |Arg6 = 0000000D
0042F40B  |. 52             PUSH    EDX                              ; |Arg5 = 00000080
0042F40C  |. 51             PUSH    ECX                              ; |Arg4 = 00000000
0042F40D  |. FFB7 C0000000  PUSH    DWORD PTR DS:[EDI+C0]            ; |Arg3 = 00093C10 (�\���I���I�v�V�����B)
0042F413  |. FFB7 AC000000  PUSH    DWORD PTR DS:[EDI+AC]            ; |Arg2 = 5 ( �I���\�l�� )
0042F419  |. FF33           PUSH    DWORD PTR DS:[EBX]               ; |Arg1
0042F41B  |. E8 15A20000    CALL    TENSHOU.00439635                 ; \TENSHOU.00439635
0042F420  |. 83C4 24        ADD     ESP, 24
0042F423  |. 3D FFFF0000    CMP     EAX, 0FFFF
0042F428  |. 74 12          JE      SHORT TENSHOU.0042F43C
0042F42A  |. FFB7 A8000000  PUSH    DWORD PTR DS:[EDI+A8]		�u0x4DDF58�ŌŒ�v�B�����ɑI�����ꂽ�����ԍ���WORD�z��ŃR�s�[�����B
0042F430  |. FF33           PUSH    DWORD PTR DS:[EBX]
0042F432  |. E8 12000000    CALL    TENSHOU.0042F449
0042F437  |. 83C4 08        ADD     ESP, 8
0042F43A  |. 8906           MOV     DWORD PTR DS:[ESI], EAX
0042F43C  |> B8 01000000    MOV     EAX, 1
0042F441  |. 5D             POP     EBP
0042F442  |. 5F             POP     EDI
0042F443  |. 5E             POP     ESI
0042F444  |. 5B             POP     EBX
0042F445  |. 83C4 08        ADD     ESP, 8


0018FE0C   004DBD40  |Arg1 = 004DBD40
0018FE10   00000001  |Arg2 = 00000005
0018FE14   3FC93F9D  |Arg3 = 3FC93F9D
0018FE18   00000000  |Arg4 = 00000000
0018FE1C   00000000  |Arg5 = 00000000
0018FE20   0000FFFF  |Arg6 = 0000FFFF
0018FE24   0000FFFF  |Arg7 = 0000FFFF
0018FE28   00000000  |Arg8 = 00000000
0018FE2C   00000000  \Arg9 = 00000000
*/



//---------------------�I�𕐏��_�C�A���O-------------------------
static int pCallFromFuncSelectBushouDialog = 0x439635; // ���XTENSHOU.EXE���ɂ�����CALL��
static int pCallFromFuncSelectBushouDialog2 = 0x42F449; // ���XTENSHOU.EXE���ɂ�����CALL�� �B�����l���̏ꍇ�ɁA�w��̃|�C���^�ɒl�𕡐�����B
static int iReturnOfSelectBushouDialog = 0; // �Ԃ�l�BAX�̕����̂ݍ̗p����B
static WORD iArrayOfBushouList[GAMEDATASTRUCT_BUSHOU_NUM+1] = {}; // �\������ΏۂƂȂ镐�����X�g�ԍ��B�Ō��0xFFFF�ŏI���Ƃ݂Ȃ��B
static int iFilterOptionOfSelectBushouDialog = Filter::Bushou::�f�t�H���g;
static int iFilterExOptionOfSelectBushouDialog = 0; // �������33bit�ڈȍ~�̃I�v�V�����B
static int iMultiBushouNumOfSelectBushouDialog = 5; // �I���\�ȕ����̍ő�l���B
void FuncSelectBushouMultiDialogAsm() {

	__asm {
		push 0
		push 0
		push 0xFFFF
		push 0xFFFF
		push 0
		push iFilterExOptionOfSelectBushouDialog // 33bit�ڈȍ~�̊g���I�v�V����
		push iFilterOptionOfSelectBushouDialog // �t�B���^�[�l
		push iMultiBushouNumOfSelectBushouDialog // �I���\��(�A�������𕡐��I������v���͂Ȃ��͂�)
		push 0x4DBD40 // WORD�ŕ����̔ԍ��̔z����i�[���Ă�ꏊ�B�Ō�0xFFFF

		call pCallFromFuncSelectBushouDialog
		add  esp, 0x24
		cmp eax, 0xFFFF	// ���̒i�K�ł�eax�͂����ƑI�񂾂��ǂ����̂݁B
		je labelEndOfFuncSelectBushouDialogAsm

		push 0x4DDF58	// ���̏ꏊ�ɑI�𕐏����ʂ�WORD�z��ŃR�s�[�����B�����ɂ��Ō�0xFFFF������B
		push 0x4DBD40   // ���������X�g�̏ꏊ
		call pCallFromFuncSelectBushouDialog2
		add  esp, 0x8

labelEndOfFuncSelectBushouDialogAsm:
		mov eax, 1
		;
	}
}



// �ΏۂƂȂ镐�����X�g�B
vector<int> FuncSelectBushouMultiDialog(vector<int> list, __int64 iFilterOption, int iSelectableMaxBushouNum ) {

	char *szFill0xFF = "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
	memcpy((char *)0x4DDF58, szFill0xFF, 2*17); // �����I�����ʂ̕����ԍ��r�񂪊i�[�����ꏊ�B�����ɂ��Ō�0xFFFF������BWORD��17��FF�Ŗ��߂Ă����B


	// �I���\�ő�l�����I�[�o�[���Ă��璼���B
	if ( iSelectableMaxBushouNum > 16 ) { 
		iSelectableMaxBushouNum = 16;
	}
	iMultiBushouNumOfSelectBushouDialog = iSelectableMaxBushouNum;

	vector<int> vResultList;
	// ���X�g�������Ȃ�Ȃɂ����Ȃ��B
	if ( list.size() == 0 ) {
		return vResultList;
	}

	// �n���ꂽvector�`���̃��X�g��WORD�`���̔z��ւ̃R�s�[���Ă����B
	WORD *pCopy = iArrayOfBushouList;
	// �����̃��X�g��z��`���ɂ���B
	for ( vector<int>::iterator it=list.begin(); it != list.end(); it++ ) {

		int iBushouID = *it;
		// �͈͊O�͖���
		if ( ! (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) ) {
			continue;
		}
		
		*pCopy = (WORD)iBushouID+1; // �z��p���ԍ��ɂ��Ĕz��Ɋi�[
		pCopy++;
	}
	
	// �R�s�[�σ|�C���^���P���i��ł��Ȃ��B==�P���L���ȕ��������Ȃ�����
	if ( pCopy == iArrayOfBushouList ) {
		return vResultList;
	}

	*pCopy = 0xFFFF; // �Ō�̕�����0xFFFF�𑫂��B

	// 0x4DBD40 // WORD�ŕ����̔ԍ��̔z����i�[���Ă�ꏊ�B
	// �������X�g�̓��e��tenshou.exe�������ւƕ����W�J�B
	memcpy( (void *)0x4DBD40, iArrayOfBushouList, sizeof(iArrayOfBushouList) );

	// �t�B���^�[�I�v�V�������㏑��
	iFilterOptionOfSelectBushouDialog = iFilterOption & 0xFFFFFFFF;
	iFilterOption = iFilterOption >> 32; // ���32bit�����ʂɎ����Ă���B(�g���I�v�V���������ʂ�)
	iFilterExOptionOfSelectBushouDialog = iFilterOption & 0xFFFFFFFF;

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
	FuncSelectBushouMultiDialogAsm();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

	WORD *pResult = (WORD *)0x4DDF58; // �����I�����ʂ̕����ԍ��r�񂪊i�[�����ꏊ�B�����ɂ��Ō�0xFFFF������B

	// �ő�Ŏw�肵�����܂ŃR�s�[
	for ( int i=0; i < iSelectableMaxBushouNum; i++ ) {
		if ( *pResult == 0xFFFF ) {
			break;
		}
		int iBushouID = (*pResult)-1; // �����ԍ��������ԍ��z��p�ɂ��Ċi�[
		if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			vResultList.push_back(iBushouID);
		}
		pResult++;
	}

	return vResultList;
}


template<int iSelectableMaxBushouNum>
vector<int> FuncSelectBushouDialog(vector<int> list, __int64 iFilterOption ) {
    return FuncSelectBushouMultiDialog(list, iFilterOption, int iSelectableMaxBushouNum);
}