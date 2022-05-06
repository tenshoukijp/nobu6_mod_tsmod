#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "ScenarioMod.h"



int iKahouIDOfKahouMeibutsuJiten = -1;

#define RETSUDEN_BUF_ADDRESS		0x4D97C0

static char szBufMeibutsuJitenTitle [50*1] = ""; // ���ۂɂ͑S�p�P�W���܂ő��߂ɂƂ��Ă����B
static char szBufMeibutsuJitenDetail[50*4] = ""; // ���ۂɂ͑S�p�P�W�����~�S�s�܂ŁB���߂ɂƂ��Ă����B

void OverWriteKahouMeibutsuJitenTitle(int iKahouID) {
	// �����N���A���Ă���c
	strcpy(szBufMeibutsuJitenTitle, "");
	strcpy(szBufMeibutsuJitenDetail, "");

	if ( p_snOnViewingMeibutsuJiten ) {
		p_snOnViewingMeibutsuJiten(iKahouID, szBufMeibutsuJitenTitle, szBufMeibutsuJitenDetail);

		// �L���ȕ����������Ă�����A�R�s�[
		if (strlen(szBufMeibutsuJitenTitle) ) {
			char *szTitle = (char *)RETSUDEN_BUF_ADDRESS;
			strcpy(szTitle, szBufMeibutsuJitenTitle);

			// �R�s�[���I������̂ŃN���A���Ă���
			strcpy(szBufMeibutsuJitenTitle, "");
		}
	}
}
void OnTenshouExeKahouMeibutsuJitenTitleExecute() {
	int iKahouID = iKahouIDOfKahouMeibutsuJiten;
	if ( 0 <= iKahouID && iKahouID < GAMEDATASTRUCT_KAHOU_NUM ) {
		OverWriteKahouMeibutsuJitenTitle(iKahouID);
	}
}

/*
00494A4B  |. 6A 00          PUSH    0                                                                                       ; /Arg8 = 00000000
00494A4D  |. 6A 00          PUSH    0                                                                                       ; |Arg7 = 00000000
00494A4F  |. 6A 00          PUSH    0                                                                                       ; |Arg6 = 00000000
00494A51  |. 6A 00          PUSH    0                                                                                       ; |Arg5 = 00000000
00494A53  |. 6A 00          PUSH    0                                                                                       ; |Arg4 = 00000000
00494A55  |. 6A 00          PUSH    0                                                                                       ; |Arg3 = 00000000
00494A57  |. 6A 00          PUSH    0                                                                                       ; |Arg2 = 00000000
00494A59  |. 57             PUSH    EDI                                                                                     ; |Arg1
00494A5A  |. E8 0176FDFF    CALL    TENSHOU.0046C060                                                                        ; \TENSHOU.0046C060
				�������� JMP TSMod.OnTSExeKahouMeibutsuJitenTitle �Ə��������Ă����ۂ̏���
00494A5F  |. 83C4 20        ADD     ESP, 20
*/
int pTenshouExeJumpFromToOnTenshouExeKahouMeibutsuJitenTitle	  =0x494A5A; // �֐��͂��̃A�h���X����AOnTenshouExeKahouMeibutsuJitenTitle�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeKahouMeibutsuJitenTitle =0x46C060; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeKahouMeibutsuJitenTitle  =0x494A5F; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeKahouMeibutsuJitenTitle() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iKahouIDOfKahouMeibutsuJiten, EBP // �ƕ�ԍ��z��p(-1�̕�)�������Ă���B

		// ���X�̏�����������
		call pTenshouExeJumpCallFromToOnTenshouExeKahouMeibutsuJitenTitle

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
	OnTenshouExeKahouMeibutsuJitenTitleExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeKahouMeibutsuJitenTitle
	}
}

char cmdOnTenshouExeJumpFromKahouMeibutsuJitenTitle[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeKahouMeibutsuJitenTitle() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeKahouMeibutsuJitenTitle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeKahouMeibutsuJitenTitle + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeKahouMeibutsuJitenTitle  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromKahouMeibutsuJitenTitle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeKahouMeibutsuJitenTitle), cmdOnTenshouExeJumpFromKahouMeibutsuJitenTitle, 5, NULL); //5�o�C�g�̂ݏ�������
}











void OnTenshouExeKahouMeibutsuJitenDetailExecute() {

	// �L���ȕ����������Ă�����A�R�s�[
	if (strlen(szBufMeibutsuJitenDetail) ) {
		char *szDetail = (char *)RETSUDEN_BUF_ADDRESS;
		strcpy(szDetail, szBufMeibutsuJitenDetail);
		
		// �R�s�[���I������̂ŃN���A���Ă���
		strcpy(szBufMeibutsuJitenDetail, "");
	}
}



/*
00494A93  |. 50             PUSH    EAX                                                                                     ; /Arg8 => 00000000
00494A94  |. 50             PUSH    EAX                                                                                     ; |Arg7 => 00000000
00494A95  |. 50             PUSH    EAX                                                                                     ; |Arg6 => 00000000
00494A96  |. 47             INC     EDI                                                                                     ; |
00494A97  |. 50             PUSH    EAX                                                                                     ; |Arg5 => 00000000
00494A98  |. 50             PUSH    EAX                                                                                     ; |Arg4 => 00000000
00494A99  |. 50             PUSH    EAX                                                                                     ; |Arg3 => 00000000
00494A9A  |. 50             PUSH    EAX                                                                                     ; |Arg2 => 00000000
00494A9B  |. 57             PUSH    EDI                                                                                     ; |Arg1
00494A9C  |. E8 BF75FDFF    CALL    TENSHOU.0046C060                                                                        ; \TENSHOU.0046C060
				�������� JMP TSMod.OnTSExeKahouMeibutsuJitenDetail �Ə��������Ă����ۂ̏���
00494AA1  |. 83C4 20        ADD     ESP, 20
*/
int pTenshouExeJumpFromToOnTenshouExeKahouMeibutsuJitenDetail	   = 0x494A9C; // �֐��͂��̃A�h���X����AOnTenshouExeKahouMeibutsuJitenDetail�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeKahouMeibutsuJitenDetail = 0x46C060; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeKahouMeibutsuJitenDetail  = 0x494AA1; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeKahouMeibutsuJitenDetail() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// ���X�̏�����������
		call pTenshouExeJumpCallFromToOnTenshouExeKahouMeibutsuJitenDetail

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
	OnTenshouExeKahouMeibutsuJitenDetailExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeKahouMeibutsuJitenDetail
	}
}

char cmdOnTenshouExeJumpFromKahouMeibutsuJitenDetail[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeKahouMeibutsuJitenDetail() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeKahouMeibutsuJitenDetail;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeKahouMeibutsuJitenDetail + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeKahouMeibutsuJitenDetail  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromKahouMeibutsuJitenDetail+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeKahouMeibutsuJitenDetail), cmdOnTenshouExeJumpFromKahouMeibutsuJitenDetail, 5, NULL); //5�o�C�g�̂ݏ�������
}




