#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"


/*
�Z�[�u�f�[�^���Z�[�u����ۂɁA���O(savedat.n6p)�����܂鎞�B

0049D57E  |. 68 708D4C00    PUSH    TENSHOU.004C8D70                 ;  ASCII "A:SAVEDAT.N6P"
0049D583  |. 8D45 C0        LEA     EAX, DWORD PTR SS:[EBP-40]
*/


void OnTenshouExePushSaveFileNameOfUnknownExecute() {
	// �Z�[�u�t�@�C���̑I���B�������s���ȃ^�C�~���O
}



/*
0049D57E  |. 68 708D4C00    PUSH    TENSHOU.004C8D70                 ;  ASCII "A:SAVEDAT.N6P"
				�������� JMP TSMod.OnTenshouExePushSaveFileNameOfUnknown �Ə��������Ă����ۂ̏���
0049D583  |. 8D45 C0        LEA     EAX, DWORD PTR SS:[EBP-40]
*/
int pTenshouExeJumpFromToOnTenshouExePushSaveFileNameOfUnknown	  = 0x49D57E; // �֐��͂��̃A�h���X����AOnTenshouExePushSaveFileNameOfUnknown�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExePushSaveFileNameOfUnknown  = 0x49D583; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExePushSaveFileNameOfUnknown() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	OnTenshouExePushSaveFileNameOfUnknownExecute();

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

		// ���Xtenshou.exe �ɂ����������������ɁB
		PUSH    0x4C8D70

		jmp pTenshouExeReturnLblFromOnTenshouExePushSaveFileNameOfUnknown
	}
}




char cmdOnTenshouExeJumpFromPushSaveFileNameOfUnknown[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v5�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExePushSaveFileNameOfUnknown() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExePushSaveFileNameOfUnknown;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExePushSaveFileNameOfUnknown + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExePushSaveFileNameOfUnknown  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromPushSaveFileNameOfUnknown+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExePushSaveFileNameOfUnknown), cmdOnTenshouExeJumpFromPushSaveFileNameOfUnknown, 5, NULL); //5�o�C�g�̂ݏ�������
}

