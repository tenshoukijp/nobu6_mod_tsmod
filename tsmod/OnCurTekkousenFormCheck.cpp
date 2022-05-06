#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


/*
 * ���ݓS�b�D�̃t�H�[����Ԃ��ǂ����B���ۂɂ͐��ɂ����āA���݃��j�b�g���ǂ̂悤�Ȍ`�Ԃ����`�F�b�N���ۑ�����
 */
extern int iCurTekkousenForm[];

// �Ώە����̃t�H�[��
int iCurTekkousenFormValue = -1;

// �t�H�[�����`�F�b�N���镐��ID
int iCurTekkousenFormBushouID = -1;

void OnTenshouExeCurTekkousenFormCheckExecute() {

	// �����͈̔̓`�F�b�N�����āc
	if ( 0 < iCurTekkousenFormBushouID-1 && iCurTekkousenFormBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM) {

		// �Ώە����̃t�H�[����ۑ����Ă���
		iCurTekkousenForm[iCurTekkousenFormBushouID-1] = iCurTekkousenFormValue;
	}
}


/*
00499F1B  |. E8 C10FF9FF    CALL    TENSHOU.0042AEE1                 ; \TENSHOU.0042AEE1     �����œS�b�D��Ԃ���EAX�ɑ��
				�������� JMP TSMod.OnTSExeCurTekkousenFormCheck �Ə��������Ă����ۂ̏���
00499F20  |. 83C4 04        ADD     ESP, 4
00499F23  |. 8BE8           MOV     EBP, EAX
00499F25  |. 83FD 04        CMP     EBP, 4													�S�b�D��ԂȂ�` �Ƃ�������
00499F28  |. 75 0E          JNZ     SHORT TENSHOU.00499F38

*/
int pTenshouExeJumpFromToOnTenshouExeCurTekkousenFormCheck	     =0x499F1B; // �֐��͂��̃A�h���X����AOnTenshouExeCurTekkousenFormCheck�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeCurTekkousenFormCheck   =0x42AEE1; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeCurTekkousenFormCheck    =0x499F20; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w

// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeCurTekkousenFormCheck() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ŌĂяo���Ă����������Ăяo��
		call pTenshouExeJumpCallFromToOnTenshouExeCurTekkousenFormCheck

		// ����ID���R�s�[
		mov iCurTekkousenFormBushouID, esi

		// ���݂̃��j�b�g�̃t�H�[���̏�Ԃ��R�s�[(4=�S�b�D�A0�`3, ���y,�R�n,�S�C,�R�n�S�C,�Ȃǂ�����)
		mov iCurTekkousenFormValue, eax;

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
	OnTenshouExeCurTekkousenFormCheckExecute();

	__asm {

		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		jmp pTenshouExeReturnLblFromOnTenshouExeCurTekkousenFormCheck
	}
}

/*
0040A185     E8 44320A00    CALL    TENSHOU.004AD3CE
				�������� JMP TSMod.OnTSExeCurTekkousenFormCheck �Ə��������Ă����ۂ̏���
0040A18A  |. 0FB7C0         MOVZX   EAX, AX
*/
char cmdOnTenshouExeJumpFromCurTekkousenFormCheck[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeCurTekkousenFormCheck() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeCurTekkousenFormCheck;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeCurTekkousenFormCheck + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeCurTekkousenFormCheck  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromCurTekkousenFormCheck+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeCurTekkousenFormCheck), cmdOnTenshouExeJumpFromCurTekkousenFormCheck, 5, NULL); //5�o�C�g�̂ݏ�������
}




