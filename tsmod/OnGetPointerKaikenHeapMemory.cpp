#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

/*
 �e�����̉�n�̃f�[�^���W�J����Ă���q�[�v�������̃A�h���X�𓾂�
 */

/*
00470292  |. 56             PUSH    ESI
00470293  |. E8 B6E8FFFF    CALL    TENSHOU.0046EB4E
00470298  |. 0FB678 03      MOVZX   EDI, BYTE PTR DS:[EAX+3]	�� ����EAX�̈ʒu���Ώۂ̃q�[�v��
0047029C  |. 83C4 04        ADD     ESP, 4
*/

/*
 * ��n�̃f�[�^���W�J����Ă���q�[�v�������̃A�h���X�𓾂�
 */
int iAddressOfGetPointerKaikenHeapMemoryExecute = NULL;
void OnTenshouExeGetPointerKaikenHeapMemoryExecute() {
}


/*
00470292  |. 56             PUSH    ESI
00470293  |. E8 B6E8FFFF    CALL    TENSHOU.0046EB4E
				�������� JMP TSMod.OnTSExeGetPointerKaikenHeapMemory �Ə��������Ă����ۂ̏���
00470298  |. 0FB678 03      MOVZX   EDI, BYTE PTR DS:[EAX+3]	�� ����EAX�̈ʒu���Ώۂ̃q�[�v��
0047029C  |. 83C4 04        ADD     ESP, 4
*/
int pTenshouExeJumpFromToOnTenshouExeGetPointerKaikenHeapMemory		=0x470293; // �֐��͂��̃A�h���X����AOnTenshouExeGetPointerKaikenHeapMemory�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeGetPointerKaikenHeapMemory =0x46EB4E; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeGetPointerKaikenHeapMemory	=0x470298; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeGetPointerKaikenHeapMemory() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeGetPointerKaikenHeapMemory

		mov iAddressOfGetPointerKaikenHeapMemoryExecute, eax; // �擾������̃q�[�v�������̈ʒu�𓾂�

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeGetPointerKaikenHeapMemoryExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeGetPointerKaikenHeapMemory
	}
}

/*
00419DED   . 50             PUSH    EAX                              ; |Arg1
00419DEE   . E8 B843FFFF    CALL    TENSHOU.0040E1AB                 ; \TENSHOU.0040E1AB  ������Call�Ŕ�I�őI�𕐏����X�g�ꗗ�ɂȂ肦�镐�����X�g���A4DBD40�ɍ���Ă���
				�������� JMP TSMod.OnTSExeGetPointerKaikenHeapMemory �Ə��������Ă����ۂ̏���
00419DF3   . 83C4 18        ADD     ESP, 18
*/
char cmdOnTenshouExeJumpFromGetPointerKaikenHeapMemory[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeGetPointerKaikenHeapMemory() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeGetPointerKaikenHeapMemory;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeGetPointerKaikenHeapMemory + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGetPointerKaikenHeapMemory  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromGetPointerKaikenHeapMemory+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeGetPointerKaikenHeapMemory), cmdOnTenshouExeJumpFromGetPointerKaikenHeapMemory, 5, NULL); //5�o�C�g�̂ݏ�������
}




