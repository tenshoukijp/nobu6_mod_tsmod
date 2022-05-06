#define _CRT_SECURE_NO_WARNINGS

#include "GameDataStruct.h"
#include "FuncSelectCastleDialog.h"
#include "CommonGlobalHandle.h"
/*
��̈���


0018FE14   004D8F50  |Arg1 = 004D8F50			�����̊i�[�ꏊ
0018FE18   00000001  |Arg2 = 00000001
0018FE1C   0033D57B  |Arg3 = 0033D57B			�J�����I�v�V����
0018FE20   00000000  |Arg4 = 00000000
0018FE24   0000FFFF  |Arg5 = 0000FFFF
0018FE28   0000FFFF  |Arg6 = 0000FFFF
0018FE2C   00000000  |Arg7 = 00000000
0018FE30   00000000  \Arg8 = 00000000

��̒P���p
0045040D  |> 6A 00          /PUSH 0                                  ; /Arg7 = 00000000
0045040F  |. 6A 00          |PUSH 0                                  ; |Arg6 = 00000000
00450411  |. 53             |PUSH EBX                                ; |Arg5
00450412  |. 53             |PUSH EBX                                ; |Arg4
00450413  |. 57             |PUSH EDI                                ; |Arg3
00450414  |. 68 7BD53300    |PUSH 33D57B                             ; |Arg2 = 0033D57B		�J�����I�v�V����
00450419  |. 68 508F4D00    |PUSH TENSHOU.004D8F50                   ; |Arg1 = 004D8F50		�����Œ�B���X�g�̕����̊i�[�ꏊ
0045041E  |. E8 B8E10200    |CALL TENSHOU.0047E5DB                   ; \TENSHOU.0047E5DB
00450423  |. 66:8BF0        |MOV SI,AX
00450426  |. 83C4 1C        |ADD     ESP, 1C


��̒P���p
�ďo����B��I���͂P�����Ȃ��͗l�B
0047E5DB  /$ 55             PUSH EBP
0047E5DC  |. 8BEC           MOV EBP,ESP
0047E5DE  |. FF7424 20      PUSH DWORD PTR SS:[ESP+20]               ; /Arg8
0047E5E2  |. FF75 1C        PUSH DWORD PTR SS:[EBP+1C]               ; |Arg7
0047E5E5  |. FF75 18        PUSH DWORD PTR SS:[EBP+18]               ; |Arg6
0047E5E8  |. FF75 14        PUSH DWORD PTR SS:[EBP+14]               ; |Arg5
0047E5EB  |. FF75 10        PUSH DWORD PTR SS:[EBP+10]               ; |Arg4
0047E5EE  |. FF75 0C        PUSH DWORD PTR SS:[EBP+C]                ; |Arg3
0047E5F1  |. 6A 01          PUSH 1                                   ; |Arg2 = 00000001
0047E5F3  |. FF75 08        PUSH DWORD PTR SS:[EBP+8]                ; |Arg1
0047E5F6  |. E8 6BFEFFFF    CALL TENSHOU.0047E466                    ; \TENSHOU.0047E466
0047E5FB  |. 83C4 20        ADD ESP,20
0047E5FE  |. 83F8 01        CMP EAX,1
0047E601  |. 66:B8 FFFF     MOV AX,0FFFF
0047E605  |. 75 0A          JNZ SHORT TENSHOU.0047E611
0047E607  |. 6A 00          PUSH 0
0047E609  |. FF75 08        PUSH DWORD PTR SS:[EBP+8]			
0047E60C  |. E8 6408F9FF    CALL TENSHOU.0040EE75
0047E611  |> 8BE5           MOV ESP,EBP
0047E613  |. 5D             POP EBP
0047E614  \. C3             RETN
*/


//---------------------�I����_�C�A���O-------------------------
int pCallFromFuncSelectCastleDialog = 0x47E5DB; // ���XTENSHOU.EXE���ɂ�����CALL��
int iReturnOfSelectCastleDialog = 0; // �Ԃ�l�BAX�̕����̂ݍ̗p����B
WORD iArrayOfCastleList[GAMEDATASTRUCT_CASTLE_NUM+1] = {}; // �\������ΏۂƂȂ�郊�X�g�ԍ��B�Ō��0xFFFF�ŏI���Ƃ݂Ȃ��B
int iFilterOptionOfSelectCastleDialog = Filter::Castle::�f�t�H���g;
static int iMultiCastleNumOfSelectCastleDialog = 1; // �I���\�ȏ�̍ő吔�B

void FuncSelectCastleDialogAsm() {

	__asm {
		push 0 
		push 0 
		push 0xFFFF
		push 0xFFFF
		push 0 
		push iFilterOptionOfSelectCastleDialog // �t�B���^�[�l
		push 0x4D8F50 // WORD�ŏ�̔ԍ��̔z����i�[���Ă�ꏊ�B�Ō�0xFFFF

		call pCallFromFuncSelectCastleDialog

		mov iReturnOfSelectCastleDialog, eax

		add  esp, 0x1C
	}
}

// �ΏۂƂȂ�郊�X�g�B
int FuncSelectCastleDialog(vector<int> list, int iFilterOption ) {

	// ���X�g�������Ȃ�Ȃɂ����Ȃ��B
	if ( list.size() == 0 ) {
		return 0xFFFF;
	}

	// �n���ꂽvector�`���̃��X�g��WORD�`���̔z��ւ̃R�s�[���Ă����B
	WORD *pCopy = iArrayOfCastleList;
	// ��̃��X�g��z��`���ɂ���B
	for ( vector<int>::iterator it=list.begin(); it != list.end(); it++ ) {

		int iCastleID = *it;
		// �͈͊O�͖���
		if ( ! (0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM) ) {
			continue;
		}
		
		*pCopy = (WORD)iCastleID+1; // �z��p���ԍ��ɂ��Ĕz��Ɋi�[
		pCopy++;
	}
	
	// �R�s�[�σ|�C���^���P���i��ł��Ȃ��B==�P���L���ȏ邪���Ȃ�����
	if ( pCopy == iArrayOfCastleList ) {
		return 0xFFFF;
	}

	*pCopy = 0xFFFF; // �Ō�̕�����0xFFFF�𑫂��B

	// 0x4D8F50 // WORD�ŏ�̔ԍ��̔z����i�[���Ă�ꏊ�B
	// �郊�X�g�̓��e��tenshou.exe�������ւƕ����W�J�B
	memcpy( (void *)0x4D8F50, iArrayOfCastleList, sizeof(iArrayOfCastleList) );

	// �t�B���^�[�I�v�V�������㏑��
	iFilterOptionOfSelectCastleDialog = iFilterOption;

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
	FuncSelectCastleDialogAsm();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

	iReturnOfSelectCastleDialog = iReturnOfSelectCastleDialog & 0xFFFF; // EAX�̂����AAX�̕����̂݁B
	if ( iReturnOfSelectCastleDialog == 0xFF || iReturnOfSelectCastleDialog == 0xFFFF ) { // �I�����Ȃ�����
		return 0xFFFF;
	} else {
		// ���Ԗڂ�I���������H �ł͂Ȃ��āA�Ώۂ̏�ԍ��y�z��p�z�𒼐ڕԂ��B
		return iReturnOfSelectCastleDialog-1;
	}
}