#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"



int iAddressLastestWord = 0;

char szMessageBuildLastestWordName[13] = "";


void OnTenshouExeMessageBuildLastestWordExecute() {
#pragma warning(push)
#pragma warning(disable:4996)
	// ���O���R�s�[
	strncpy( szMessageBuildLastestWordName, (char *)iAddressLastestWord, 12);
#pragma warning(pop)
	// �ň��̎��̂��߂̃K�[�h
	szMessageBuildLastestWordName[12]='\0';
}


/*
/*
004AD7E1  |. 50             PUSH    EAX
004AD7E2  |. FF7424 18      PUSH    DWORD PTR SS:[ESP+18]
004AD7E6  |. 55             PUSH    EBP
004AD7E7  |. E8 9BFDFFFF    CALL    TENSHOU.004AD587
				�������� JMP TSMod.OnTSExeMessageBuildLastestWord �Ə��������Ă����ۂ̏���
004AD7EC  |. 83C4 0C        ADD     ESP, 0C

 */

int pTenshouExeJumpFromToOnTenshouExeMessageBuildLastestWord    =0x4AD7E7; // �֐��͂��̃A�h���X����AOnTenshouExeMessageBuildLastestWord�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeMessageBuildLastestWord=0x4AD587; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeMessageBuildLastestWord =0x4AD7EC; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeMessageBuildLastestWord() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
	
		// �Ō�̒P�ꂪ�i�[����Ă���A�h���X
		mov iAddressLastestWord, esi;

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeMessageBuildLastestWord

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
	OnTenshouExeMessageBuildLastestWordExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeMessageBuildLastestWord
	}
}

/*
00444769  |. 50             PUSH    EAX
0044476A  |. E8 3A2C0200    CALL    TENSHOU.004673A9
				�������� JMP TSMod.OnTSExeMessageBuildLastestWord �Ə��������Ă����ۂ̏���
0044476F  |. 83C4 04        ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromMessageBuildLastestWord[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeMessageBuildLastestWord() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeMessageBuildLastestWord;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeMessageBuildLastestWord + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeMessageBuildLastestWord  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromMessageBuildLastestWord+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeMessageBuildLastestWord), cmdOnTenshouExeJumpFromMessageBuildLastestWord, 5, NULL); //5�o�C�g�̂ݏ�������
}




