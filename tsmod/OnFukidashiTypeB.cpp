#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "MessageN6PPatch.h"


// �]�����ꂽ���b�Z�[�W�͗��p���Ă��Ȃ��Ƃ����t���O���Ă�

/*
 * ���o��TypeB�^(�O���[�̘g���t���Ă����)�̍��オ�\������钼�O
 */

void OnTenshouExeFukidashiTypeB_LeftExecute() {
	// �]�����ꂽ���b�Z�[�W�͗��p���Ă��Ȃ��Ƃ����t���O���Ă�
}


/*
004A1FA9  |. B9 F0994D00    MOV     ECX, TENSHOU.004D99F0            ; |ASCII "???"
004A1FAE  |. 68 6CBA4B00    PUSH    TENSHOU.004BBA6C                 ; |Arg1 = 004BBA6C
004A1FB3  |. E8 41FEFFFF    CALL    TENSHOU.004A1DF9                 ; \TENSHOU.004A1DF9
				�������� JMP TSMod.OnTSExeFukidashiTypeB_Left �Ə��������Ă����ۂ̏���
004A1FB8  |. 6A 00          PUSH    0                                ; /Arg6 = 00000000
 */
int pTenshouExeJumpFromToOnTenshouExeFukidashiTypeB_Left    =0x4A1FB3; // �֐��͂��̃A�h���X����AOnTenshouExeFukidashiTypeB_Left�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeFukidashiTypeB_Left=0x4A1DF9; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeFukidashiTypeB_Left =0x4A1FB8; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeFukidashiTypeB_Left() {
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
	
	// 
	OnTenshouExeFukidashiTypeB_LeftExecute();

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

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeFukidashiTypeB_Left

		jmp pTenshouExeReturnLblFromOnTenshouExeFukidashiTypeB_Left
	}
}

/*
00444769  |. 50             PUSH    EAX
0044476A  |. E8 3A2C0200    CALL    TENSHOU.004673A9
				�������� JMP TSMod.OnTSExeFukidashiTypeB_Left �Ə��������Ă����ۂ̏���
0044476F  |. 83C4 04        ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromFukidashiTypeB_Left[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeFukidashiTypeB_Left() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeFukidashiTypeB_Left;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeFukidashiTypeB_Left + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeFukidashiTypeB_Left  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromFukidashiTypeB_Left+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeFukidashiTypeB_Left), cmdOnTenshouExeJumpFromFukidashiTypeB_Left, 5, NULL); //5�o�C�g�̂ݏ�������
}


///-----------------------------------------------------------------------





/*
 * ���o��TypeB�^(�O���[�̘g���t���Ă����)�̉E�����\������钼�O
 */

void OnTenshouExeFukidashiTypeB_RightExecute() {
	// �]�����ꂽ���b�Z�[�W�͗��p���Ă��Ȃ��Ƃ����t���O���Ă�
}


/*
004A1FFF  |. B9 A89A4D00    MOV     ECX, TENSHOU.004D9AA8            ; |ASCII "�VK"
004A2004  |. 68 6CBA4B00    PUSH    TENSHOU.004BBA6C                 ; |Arg1 = 004BBA6C
004A2009  |. E8 EBFDFFFF    CALL    TENSHOU.004A1DF9                 ; \TENSHOU.004A1DF9
				�������� JMP TSMod.OnTSExeFukidashiTypeB_Right �Ə��������Ă����ۂ̏���
004A200E  |. 6A 00          PUSH    0                                ; /Arg6 = 00000000
 */
int pTenshouExeJumpFromToOnTenshouExeFukidashiTypeB_Right    =0x4A2009; // �֐��͂��̃A�h���X����AOnTenshouExeFukidashiTypeB_Right�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeFukidashiTypeB_Right=0x4A1DF9; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeFukidashiTypeB_Right =0x4A200E; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeFukidashiTypeB_Right() {
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
	
	// 
	OnTenshouExeFukidashiTypeB_RightExecute();

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

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeFukidashiTypeB_Right

		jmp pTenshouExeReturnLblFromOnTenshouExeFukidashiTypeB_Right
	}
}

/*
00444769  |. 50             PUSH    EAX
0044476A  |. E8 3A2C0200    CALL    TENSHOU.004673A9
				�������� JMP TSMod.OnTSExeFukidashiTypeB_Right �Ə��������Ă����ۂ̏���
0044476F  |. 83C4 04        ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromFukidashiTypeB_Right[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeFukidashiTypeB_Right() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeFukidashiTypeB_Right;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeFukidashiTypeB_Right + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeFukidashiTypeB_Right  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromFukidashiTypeB_Right+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeFukidashiTypeB_Right), cmdOnTenshouExeJumpFromFukidashiTypeB_Right, 5, NULL); //5�o�C�g�̂ݏ�������
}




