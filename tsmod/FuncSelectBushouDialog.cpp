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


004397B3  /$ 55             PUSH EBP
004397B4  |. 8BEC           MOV EBP,ESP
004397B6  |. FF7424 24      PUSH DWORD PTR SS:[ESP+24]               ; /Arg9
004397BA  |. FF75 20        PUSH DWORD PTR SS:[EBP+20]               ; |Arg8
004397BD  |. FF75 1C        PUSH DWORD PTR SS:[EBP+1C]               ; |Arg7
004397C0  |. FF75 18        PUSH DWORD PTR SS:[EBP+18]               ; |Arg6
004397C3  |. FF75 14        PUSH DWORD PTR SS:[EBP+14]               ; |Arg5
004397C6  |. FF75 10        PUSH DWORD PTR SS:[EBP+10]               ; |Arg4
004397C9  |. FF75 0C        PUSH DWORD PTR SS:[EBP+C]                ; |Arg3	
004397CC  |. 6A 01          PUSH 1                                   ; |Arg2 = 00000001
004397CE  |. FF75 08        PUSH DWORD PTR SS:[EBP+8]                ; |Arg1
004397D1  |. E8 5FFEFFFF    CALL TENSHOU.00439635                    ; \TENSHOU.00439635
004397D6  |. 83C4 24        ADD ESP,24
004397D9  |. 83F8 01        CMP EAX,1
004397DC  |. 66:B8 FFFF     MOV AX,0FFFF
004397E0  |. 75 0A          JNZ SHORT TENSHOU.004397EC
004397E2  |. 6A 00          PUSH 0
004397E4  |. FF75 08        PUSH DWORD PTR SS:[EBP+8]				0x4DBD40(�Œ�)
004397E7  |. E8 724EFDFF    CALL TENSHOU.0040E65E					���̊֐��̌��ʂ͑I�𕐏���ID�ƂȂ�B
004397EC  |> 8BE5           MOV ESP,EBP
004397EE  |. 5D             POP EBP
004397EF  \. C3             RETN

0018FE0C   004DBD40  |Arg1 = 004DBD40
0018FE10   00000001  |Arg2 = 00000001
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
static int pCallFromFuncSelectBushouDialog2 = 0x40E65E; // ���XTENSHOU.EXE���ɂ�����CALL��
static int iReturnOfSelectBushouDialog = 0; // �Ԃ�l�BAX�̕����̂ݍ̗p����B
static WORD iArrayOfBushouList[GAMEDATASTRUCT_BUSHOU_NUM+1] = {}; // �\������ΏۂƂȂ镐�����X�g�ԍ��B�Ō��0xFFFF�ŏI���Ƃ݂Ȃ��B
static int iFilterOptionOfSelectBushouDialog = Filter::Bushou::�f�t�H���g;
static int iFilterExOptionOfSelectBushouDialog = 0; // �������33bit�ڈȍ~�̃I�v�V�����B

void FuncSelectBushouDialogAsm() {

	__asm {
		push 0
		push 0
		push 0xFFFF
		push 0xFFFF
		push 0
		push iFilterExOptionOfSelectBushouDialog // 33bit�ڈȍ~�̊g���I�v�V����
		push iFilterOptionOfSelectBushouDialog // �t�B���^�[�l
		push 1 // �I���\��(�A�������𕡐��I������v���͂Ȃ��͂�)
		push 0x4DBD40 // WORD�ŕ����̔ԍ��̔z����i�[���Ă�ꏊ�B�Ō�0xFFFF

		call pCallFromFuncSelectBushouDialog
		add  esp, 0x24
		cmp eax, 1	// ���̒i�K�ł�eax�͂����ƑI�񂾂��ǂ����̂݁B
		mov ax, 0xFFFF
//		jnz labelEndOfFuncSelectBushouDialogAsm

		push 0
		push 0x4DBD40
		call pCallFromFuncSelectBushouDialog2
		mov iReturnOfSelectBushouDialog, eax // ���ꂪ���ۂ̑I�𕐏��ԍ�
		add  esp, 0x8

//labelEndOfFuncSelectBushouDialogAsm:
//		;
	}
}



// �ΏۂƂȂ镐�����X�g�B
int FuncSelectBushouDialog(vector<int> list, __int64 iFilterOption) {

	// ���X�g�������Ȃ�Ȃɂ����Ȃ��B
	if ( list.size() == 0 ) {
		return 0xFFFF;
	}

	// �Ԃ�l�̓f�t�H���g�́u�����I�����Ȃ������v
	iReturnOfSelectBushouDialog = 0xFFFF;

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
		return 0xFFFF;
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
	FuncSelectBushouDialogAsm();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

	iReturnOfSelectBushouDialog = iReturnOfSelectBushouDialog & 0xFFFF; // EAX�̂����AAX�̕����̂݁B
	if ( iReturnOfSelectBushouDialog == 0xFFFF ) { // �I�����Ȃ�����
		return 0xFFFF;
	} else {
		// ���Ԗڂ�I���������H �ł͂Ȃ��āA�Ώۂ̕����ԍ��y�z��p�z�𒼐ڕԂ��B
		return int(iReturnOfSelectBushouDialog)-1;
	}
}