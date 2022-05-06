#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "ScenarioMod.h"






int iBushouIDOfBushouRetsudenTitlePrev = -1;

void OnTenshouExeBushouRetsudenTitlePrevExecute() {
}

/*
00493E82  |. 6A 01          PUSH    1
00493E84  |. E8 9486FDFF    CALL    TENSHOU.0046C51D
				�������� JMP TSMod.OnTSExeBushouRetsudenTitlePrev �Ə��������Ă����ۂ̏���
00493E89  |. 83C4 04        ADD     ESP, 4
*/
int pTenshouExeJumpFromToOnTenshouExeBushouRetsudenTitlePrev	 =0x493E84; // �֐��͂��̃A�h���X����AOnTenshouExeBushouRetsudenTitlePrev�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeBushouRetsudenTitlePrev =0x46C51D; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeBushouRetsudenTitlePrev	 =0x493E89; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeBushouRetsudenTitlePrev() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// �����ԍ����T���Ă���
		mov iBushouIDOfBushouRetsudenTitlePrev, EAX

		// ���X�������������������
		call pTenshouExeJumpCallFromToOnTenshouExeBushouRetsudenTitlePrev

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
	OnTenshouExeBushouRetsudenTitlePrevExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeBushouRetsudenTitlePrev
	}
}

char cmdOnTenshouExeJumpFromBushouRetsudenTitlePrev[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeBushouRetsudenTitlePrev() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeBushouRetsudenTitlePrev;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeBushouRetsudenTitlePrev + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeBushouRetsudenTitlePrev  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromBushouRetsudenTitlePrev+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeBushouRetsudenTitlePrev), cmdOnTenshouExeJumpFromBushouRetsudenTitlePrev, 5, NULL); //5�o�C�g�̂ݏ�������
}














int iFaceIDOfBushouRetsudenTitle = -1; // �v�����ꂽ��ԍ�

#define RETSUDEN_BUF_ADDRESS		0x4D97C0

static char szBufRestudenTitle [50*1] = ""; // ���ۂɂ͑S�p�P�W���܂ő��߂ɂƂ��Ă����B
static char szBufRestudenDetail[50*4] = ""; // ���ۂɂ͑S�p�P�W�����~�S�s�܂ŁB���߂ɂƂ��Ă����B

void OverWriteBushouRetsudenTitle(int iBushouID, int iFaceID) {
	// �����N���A���Ă���c
	strcpy(szBufRestudenTitle, "");
	strcpy(szBufRestudenDetail, "");

	if ( p_snOnViewingRetsuden ) {

		p_snOnViewingRetsuden(iBushouID, iFaceID, szBufRestudenTitle, szBufRestudenDetail);

		// �L���ȕ����������Ă�����A�R�s�[
		if (strlen(szBufRestudenTitle) ) {
			char *szTitle = (char *)RETSUDEN_BUF_ADDRESS;
			strcpy(szTitle, szBufRestudenTitle);

			// �R�s�[���I������̂ŃN���A���Ă���
			strcpy(szBufRestudenTitle, "");
		}
	}
}
void OnTenshouExeBushouRetsudenTitleExecute() {
	int iBushouID = iBushouIDOfBushouRetsudenTitlePrev-1;
	int iFaceID = iFaceIDOfBushouRetsudenTitle;
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		OverWriteBushouRetsudenTitle(iBushouID, iFaceID);
	}
}

/*
00493EE8  |. 50             PUSH    EAX                              ; /Arg8 => 00000000
00493EE9  |. 50             PUSH    EAX                              ; |Arg7 => 00000000
00493EEA  |. 50             PUSH    EAX                              ; |Arg6 => 00000000
00493EEB  |. 50             PUSH    EAX                              ; |Arg5 => 00000000
00493EEC  |. 50             PUSH    EAX                              ; |Arg4 => 00000000
00493EED  |. 50             PUSH    EAX                              ; |Arg3 => 00000000
00493EEE  |. 50             PUSH    EAX                              ; |Arg2 => 00000000
00493EEF  |. 57             PUSH    EDI                              ; |Arg1
00493EF0  |. E8 6B81FDFF    CALL    TENSHOU.0046C060                 ; \TENSHOU.0046C060
				�������� JMP TSMod.OnTSExeBushouRetsudenTitle �Ə��������Ă����ۂ̏���
00493EF5  |. 83C4 20        ADD     ESP, 20
*/
int pTenshouExeJumpFromToOnTenshouExeBushouRetsudenTitle	 =0x493EF0; // �֐��͂��̃A�h���X����AOnTenshouExeBushouRetsudenTitle�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeBushouRetsudenTitle =0x46C060; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeBushouRetsudenTitle	 =0x493EF5; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeBushouRetsudenTitle() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iFaceIDOfBushouRetsudenTitle, ESI // ��ԍ�

		// ���X�̏�����������
		call pTenshouExeJumpCallFromToOnTenshouExeBushouRetsudenTitle

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
	OnTenshouExeBushouRetsudenTitleExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeBushouRetsudenTitle
	}
}

char cmdOnTenshouExeJumpFromBushouRetsudenTitle[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeBushouRetsudenTitle() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeBushouRetsudenTitle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeBushouRetsudenTitle + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeBushouRetsudenTitle  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromBushouRetsudenTitle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeBushouRetsudenTitle), cmdOnTenshouExeJumpFromBushouRetsudenTitle, 5, NULL); //5�o�C�g�̂ݏ�������
}











void OnTenshouExeBushouRetsudenDetailExecute() {

	// �L���ȕ����������Ă�����A�R�s�[
	if (strlen(szBufRestudenDetail) ) {
		char *szDetail = (char *)RETSUDEN_BUF_ADDRESS;
		strcpy(szDetail, szBufRestudenDetail);
		
		// �R�s�[���I������̂ŃN���A���Ă���
		strcpy(szBufRestudenDetail, "");
	}
}



/*
00493F3D  |. 55             PUSH    EBP                              ; |Arg7
00493F3E  |. 55             PUSH    EBP                              ; |Arg6
00493F3F  |. 47             INC     EDI                              ; |
00493F40  |. 55             PUSH    EBP                              ; |Arg5
00493F41  |. 55             PUSH    EBP                              ; |Arg4
00493F42  |. 55             PUSH    EBP                              ; |Arg3
00493F43  |. 55             PUSH    EBP                              ; |Arg2
00493F44  |. 57             PUSH    EDI                              ; |Arg1
00493F45  |. E8 1681FDFF    CALL    TENSHOU.0046C060                 ; \TENSHOU.0046C060
				�������� JMP TSMod.OnTSExeBushouRetsudenDetail �Ə��������Ă����ۂ̏���
00493F4A  |. 83C4 20        ADD     ESP, 20
*/
int pTenshouExeJumpFromToOnTenshouExeBushouRetsudenDetail	  = 0x493F45; // �֐��͂��̃A�h���X����AOnTenshouExeBushouRetsudenDetail�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeBushouRetsudenDetail = 0x46C060; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeBushouRetsudenDetail  = 0x493F4A; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeBushouRetsudenDetail() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// ���X�̏�����������
		call pTenshouExeJumpCallFromToOnTenshouExeBushouRetsudenDetail

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
	OnTenshouExeBushouRetsudenDetailExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeBushouRetsudenDetail
	}
}

char cmdOnTenshouExeJumpFromBushouRetsudenDetail[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeBushouRetsudenDetail() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeBushouRetsudenDetail;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeBushouRetsudenDetail + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeBushouRetsudenDetail  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromBushouRetsudenDetail+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeBushouRetsudenDetail), cmdOnTenshouExeJumpFromBushouRetsudenDetail, 5, NULL); //5�o�C�g�̂ݏ�������
}




