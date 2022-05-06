#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "ScenarioMod.h"


short iCastleIDOfCastleMeijoMonogatari = -1;

#define RETSUDEN_BUF_ADDRESS		0x4D97C0

static char szBufMeijoMonogatariTitle [50*1] = ""; // ���ۂɂ͑S�p�P�W���܂ő��߂ɂƂ��Ă����B
static char szBufMeijoMonogatariDetail[50*4] = ""; // ���ۂɂ͑S�p�P�W�����~�S�s�܂ŁB���߂ɂƂ��Ă����B

void OverWriteCastleMeijoMonogatariTitle(int iCastleID) {
	// �����N���A���Ă���c
	strcpy(szBufMeijoMonogatariTitle, "");
	strcpy(szBufMeijoMonogatariDetail, "");
	if ( p_snOnViewingMeijoMonogatari ) {

		p_snOnViewingMeijoMonogatari(iCastleID, szBufMeijoMonogatariTitle, szBufMeijoMonogatariDetail);

		// �L���ȕ����������Ă�����A�R�s�[
		if (strlen(szBufMeijoMonogatariTitle) ) {
			char *szTitle = (char *)RETSUDEN_BUF_ADDRESS;
			strcpy(szTitle, szBufMeijoMonogatariTitle);

			// �R�s�[���I������̂ŃN���A���Ă���
			strcpy(szBufMeijoMonogatariTitle, "");
		}
	}
}
void OnTenshouExeCastleMeijoMonogatariTitleExecute() {
	int iCastleID = iCastleIDOfCastleMeijoMonogatari-1;
	if ( 0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM ) {
		OverWriteCastleMeijoMonogatariTitle(iCastleID);
	}
}

/*
00476F06  |> 66:8B86 C00100>MOV     AX, WORD PTR DS:[ESI+1C0]


00476F69  |. 50             PUSH    EAX                              ; /Arg8 => 00000000
00476F6A  |. 50             PUSH    EAX                              ; |Arg7 => 00000000
00476F6B  |. 50             PUSH    EAX                              ; |Arg6 => 00000000
00476F6C  |. 50             PUSH    EAX                              ; |Arg5 => 00000000
00476F6D  |. 50             PUSH    EAX                              ; |Arg4 => 00000000
00476F6E  |. 50             PUSH    EAX                              ; |Arg3 => 00000000
00476F6F  |. 50             PUSH    EAX                              ; |Arg2 => 00000000
00476F70  |. 53             PUSH    EBX                              ; |Arg1
00476F71  |. E8 EA50FFFF    CALL    TENSHOU.0046C060                 ; \TENSHOU.0046C060
				�������� JMP TSMod.OnTSExeCastleMeijoMonogatariTitle �Ə��������Ă����ۂ̏���
00476F76  |. 83C4 20        ADD     ESP, 20
*/
int pTenshouExeJumpFromToOnTenshouExeCastleMeijoMonogatariTitle	  =0x476F71; // �֐��͂��̃A�h���X����AOnTenshouExeCastleMeijoMonogatariTitle�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeCastleMeijoMonogatariTitle =0x46C060; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeCastleMeijoMonogatariTitle  =0x476F76; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w

int iEAXOfCastleMeijoMonogatariTitle = -1; // �ꎞ�I��EAX�ۑ���

// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeCastleMeijoMonogatariTitle() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		mov iEAXOfCastleMeijoMonogatariTitle, eax

		// ��̔ԍ��������Ă���
		mov AX, WORD PTR DS:[ESI+0x1C0] // ��̔ԍ�

		mov iCastleIDOfCastleMeijoMonogatari, AX // �ۑ����Ă���

		mov eax, iEAXOfCastleMeijoMonogatariTitle // EAX�̒l��߂�

		// ���X�̏�����������
		call pTenshouExeJumpCallFromToOnTenshouExeCastleMeijoMonogatariTitle

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
	OnTenshouExeCastleMeijoMonogatariTitleExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeCastleMeijoMonogatariTitle
	}
}

char cmdOnTenshouExeJumpFromCastleMeijoMonogatariTitle[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeCastleMeijoMonogatariTitle() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeCastleMeijoMonogatariTitle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeCastleMeijoMonogatariTitle + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeCastleMeijoMonogatariTitle  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromCastleMeijoMonogatariTitle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeCastleMeijoMonogatariTitle), cmdOnTenshouExeJumpFromCastleMeijoMonogatariTitle, 5, NULL); //5�o�C�g�̂ݏ�������
}











void OnTenshouExeCastleMeijoMonogatariDetailExecute() {

	// �L���ȕ����������Ă�����A�R�s�[
	if (strlen(szBufMeijoMonogatariDetail) ) {
		char *szDetail = (char *)RETSUDEN_BUF_ADDRESS;
		strcpy(szDetail, szBufMeijoMonogatariDetail);
		
		// �R�s�[���I������̂ŃN���A���Ă���
		strcpy(szBufMeijoMonogatariDetail, "");
	}
}



/*
00476FC1  |. 50             PUSH    EAX                              ; /Arg8 => 00000000
00476FC2  |. 50             PUSH    EAX                              ; |Arg7 => 00000000
00476FC3  |. 50             PUSH    EAX                              ; |Arg6 => 00000000
00476FC4  |. 43             INC     EBX                              ; |
00476FC5  |. 50             PUSH    EAX                              ; |Arg5 => 00000000
00476FC6  |. 50             PUSH    EAX                              ; |Arg4 => 00000000
00476FC7  |. 50             PUSH    EAX                              ; |Arg3 => 00000000
00476FC8  |. 50             PUSH    EAX                              ; |Arg2 => 00000000
00476FC9  |. 53             PUSH    EBX                              ; |Arg1
00476FCA  |. E8 9150FFFF    CALL    TENSHOU.0046C060                 ; \TENSHOU.0046C060
				�������� JMP TSMod.OnTSExeCastleMeijoMonogatariDetail �Ə��������Ă����ۂ̏���
00476FCF  |. 83C4 20        ADD     ESP, 20
*/
int pTenshouExeJumpFromToOnTenshouExeCastleMeijoMonogatariDetail	   = 0x476FCA; // �֐��͂��̃A�h���X����AOnTenshouExeCastleMeijoMonogatariDetail�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeCastleMeijoMonogatariDetail = 0x46C060; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeCastleMeijoMonogatariDetail  = 0x476FCF; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeCastleMeijoMonogatariDetail() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// ���X�̏�����������
		call pTenshouExeJumpCallFromToOnTenshouExeCastleMeijoMonogatariDetail

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
	OnTenshouExeCastleMeijoMonogatariDetailExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeCastleMeijoMonogatariDetail
	}
}

char cmdOnTenshouExeJumpFromCastleMeijoMonogatariDetail[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeCastleMeijoMonogatariDetail() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeCastleMeijoMonogatariDetail;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeCastleMeijoMonogatariDetail + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeCastleMeijoMonogatariDetail  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromCastleMeijoMonogatariDetail+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeCastleMeijoMonogatariDetail), cmdOnTenshouExeJumpFromCastleMeijoMonogatariDetail, 5, NULL); //5�o�C�g�̂ݏ�������
}




