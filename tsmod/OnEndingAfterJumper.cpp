#include "WinTarget.h"

#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

/*
 �G���f�B���O��ɁA�����I���ɂ��邩�����ݒ�ɖ߂邩�̐ؑցB
 �������A�P���ɃR�[�����Ȃ��Ƃ��������Ƃ����ƁA�f�����[�h���烆�[�U�[�I�����Ă�1��ږ�������Ă��܂��̂ŁA������l������B
 */

/*
004103DF  |. 74 28          |JE      SHORT TENSHOU.00410409
004103E1  |> E8 4FE80600    |CALL    TENSHOU.0047EC35
004103E6  |. 85C0           |TEST    EAX, EAX
*/

void OnTenshouExeEndingAfterJumperExecute() {
}


/*
004103DF  |. 74 28          |JE      SHORT TENSHOU.00410409
004103E1  |> E8 4FE80600    |CALL    TENSHOU.0047EC35
�������� JMP TSMod.OnTSExeEndingAfterJumper �Ə��������Ă����ۂ̏���
004103E6  |. 85C0           |TEST    EAX, EAX
*/
int pTenshouExeJumpFromToOnTenshouExeEndingAfterJumper		=0x4103E1; // �֐��͂��̃A�h���X����AOnTenshouExeEndingAfterJumper�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeEndingAfterJumper  =0x47EC35; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeEndingAfterJumper	=0x4103E6; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeEndingAfterJumper() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeEndingAfterJumper

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// �G���f�B���O����
	OnTenshouExeEndingAfterJumperExecute();

	if ( CommonGlobalHandle::isEndingMode ) {
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

			mov eax, 0 // �G���f�B���O����1�ɐݒ肳��āA1���Ƌ����I���悤���������Ȃ�

			jmp pTenshouExeReturnLblFromOnTenshouExeEndingAfterJumper
		}
	// �G���f�B���O���[�h�ȊO�́A�����ύX�����A���̂܂ܖ߂�
	} else {
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

			jmp pTenshouExeReturnLblFromOnTenshouExeEndingAfterJumper
		}
	}
}

/*
*/
char cmdOnTenshouExeJumpFromEndingAfterJumper[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeEndingAfterJumper() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeEndingAfterJumper;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeEndingAfterJumper + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeEndingAfterJumper  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromEndingAfterJumper+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeEndingAfterJumper), cmdOnTenshouExeJumpFromEndingAfterJumper, 5, NULL); //5�o�C�g�̂ݏ�������
}




