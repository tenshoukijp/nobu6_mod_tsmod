#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"



/*
 * �����Ґ���ʂɓ�������
 */ 


/*
0047A022  |. E8 B0180000    CALL    TENSHOU.0047B8D7
0047A027  |. 8D8D 58FFFFFF  LEA     ECX, DWORD PTR SS:[EBP-A8]

*/


int isOnEditingButaiMode = 0L;

void OnTenshouExeEditingButaiExecute() {
}



/*
0047A022  |. E8 B0180000    CALL    TENSHOU.0047B8D7				�������Ґ���ʂɓ������B���̊֐����Ă΂�Ă���Ԃ������Ґ���ʊJ���Ă�ԁB
0047A027  |. 8D8D 58FFFFFF  LEA     ECX, DWORD PTR SS:[EBP-A8]
*/
int pTenshouExeJumpFromToOnTenshouExeEditingButai	  = 0x47A022; // �֐��͂��̃A�h���X����AOnTenshouExeEditingButai�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeEditingButai = 0x47B8D7; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeEditingButai  = 0x47A027; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeEditingButai() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov isOnEditingButaiMode, 1	// �����Ґ����[�h�n�m

			// ���̏����������ɋL��
		call pTenshouExeJumpCallFromToOnTenshouExeEditingButai

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
	OnTenshouExeEditingButaiExecute();

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

		mov isOnEditingButaiMode, 0	// �����Ґ����[�h�n�e�e

		jmp pTenshouExeReturnLblFromOnTenshouExeEditingButai
	}
}




char cmdOnTenshouExeJumpFromEditingButai[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v5�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeEditingButai() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeEditingButai;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeEditingButai + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeEditingButai  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromEditingButai+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeEditingButai), cmdOnTenshouExeJumpFromEditingButai, 5, NULL); //5�o�C�g�̂ݏ�������
}




