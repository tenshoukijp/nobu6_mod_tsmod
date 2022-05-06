#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "MessageN6PPatch.h"
#include "ScenarioMod.h"

/*
004A0F94   8946 70          MOV     DWORD PTR DS:[ESI+70], EAX
004A0F97   C603 00          MOV     BYTE PTR DS:[EBX], 0
004A0F9A   74 19            JE      SHORT TENSHOU.004A0FB5
*/

int EBXOfRareMessageWriteMemory = -1;

void OnTenshouExeRareMessageWriteMemoryExecute() {
}


/*
004A0F94   8946 70          MOV     DWORD PTR DS:[ESI+70], EAX
004A0F97   C603 00          MOV     BYTE PTR DS:[EBX], 0
				�������� JMP TSMod.OnTSExeRareMessageWriteMemory �Ə��������Ă����ۂ̏���
004A0F9A   74 19            JE      SHORT TENSHOU.004A0FB5
00499038  |. 5E             POP     ESI
*/
int pTenshouExeJumpFromToOnTenshouExeRareMessageWriteMemory		=0x4A0F94; // �֐��͂��̃A�h���X����AOnTenshouExeRareMessageWriteMemory�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeRareMessageWriteMemory	=0x4A0F9A; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeRareMessageWriteMemory() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		MOV     DWORD PTR DS:[ESI+0x70], EAX
		MOV     BYTE PTR DS:[EBX], 0		// �� ����EBX��	0x4D91B4 �̎��A�R�t�Ȃǂ�����ׂ��Ă���̂ő��Ƃ̓��b�Z�[�W�̈ʒu���قȂ�LOC3�ƂȂ�B ASCII "���悤�Ȏ�y�Y�ł͉Ƃ͗�����
		mov EBXOfRareMessageWriteMemory, EBX  // EBX��ۑ�
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
	OnTenshouExeRareMessageWriteMemoryExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeRareMessageWriteMemory
	}
}

/*
0049902C  |. 8BCE           MOV     ECX, ESI
0049902E  |. 68 B8334D00    PUSH    TENSHOU.004D33B8                 ;  ASCII "��ʃT�C�Y��I�����Ă�������"
00499033  |. E8 16550100    CALL    TENSHOU.004AE54E
				�������� JMP TSMod.OnTSExeRareMessageWriteMemory �Ə��������Ă����ۂ̏���
00499038  |. 5E             POP     ESI
*/
char cmdOnTenshouExeJumpFromRareMessageWriteMemory[7]="\xE9\x90\x90\x90\x90\x90";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeRareMessageWriteMemory() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeRareMessageWriteMemory;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeRareMessageWriteMemory + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeRareMessageWriteMemory  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromRareMessageWriteMemory+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeRareMessageWriteMemory), cmdOnTenshouExeJumpFromRareMessageWriteMemory, 6, NULL); //6�o�C�g�̂ݏ�������
}




