

#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"












/*
 * �Z�[�u�⃍�[�h���ɁA�X���b�g�ԍ���I��������
 */



int iSelectSaveOrLoadNo = -1;

// �Z�[�u���[�h�_�C�A���O�őI�����Ă��鎞�����g����B
int getSelectSaveOrLoadNo() {
	return iSelectSaveOrLoadNo;
}


void OnTenshouExeSelectSaveOrLoadNoExecute() {
//	OutputDebugStream << iSelectSaveOrLoadNo << endl;
}


/*
004798D9  |. 43             INC     EBX
004798DA  |. 53             PUSH    EBX									�� ����EBX���X���b�g�̔ԍ�(�ォ��1�ŉ���8)
004798DB  |. BE 789B4D00    MOV     ESI, TENSHOU.004D9B78            ;  ASCII "1:1560�N �t �C�D�E�@�A�B �����Ԃ̍��� ����   1��N��"
004798E0  |. 68 B4F04C00    PUSH    TENSHOU.004CF0B4                 ;  ASCII "%d�Ԗ�%s�܂�"
004798E5  |. 56             PUSH    ESI
004798E6  |. E8 3D420300    CALL    TENSHOU.004ADB28
				�������� JMP TSMod.OnTSExeSelectSaveOrLoadNo �Ə��������Ă����ۂ̏���
004798EB  |. 83C4 10        ADD     ESP, 10

*/
int pTenshouExeJumpFromToOnTenshouExeSelectSaveOrLoadNo		=0x4798E6; // �֐��͂��̃A�h���X����AOnTenshouExeSelectSaveOrLoadNo�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeSelectSaveOrLoadNo  =0x4ADB28; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeSelectSaveOrLoadNo	=0x4798EB; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeSelectSaveOrLoadNo() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		mov iSelectSaveOrLoadNo, ebx

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
	OnTenshouExeSelectSaveOrLoadNoExecute();

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
		call pTenshouExeJumpCallFromToOnTenshouExeSelectSaveOrLoadNo

		jmp pTenshouExeReturnLblFromOnTenshouExeSelectSaveOrLoadNo
	}
}

/*
00446A7B  |. 57             PUSH    EDI
00446A7C  |. E8 3159FCFF    CALL    TENSHOU.0040C3B2
				�������� JMP TSMod.OnTSExeSelectSaveOrLoadNo �Ə��������Ă����ۂ̏���
00446A81  |. 83C4 04        ADD     ESP, 4
00446A84  |. 85C0           TEST    EAX, EAX
00446A86  |. 74 45          JE      SHORT TENSHOU.00446ACD
*/
char cmdOnTenshouExeJumpFromSelectSaveOrLoadNo[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeSelectSaveOrLoadNo() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeSelectSaveOrLoadNo;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSelectSaveOrLoadNo + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeSelectSaveOrLoadNo  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromSelectSaveOrLoadNo+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSelectSaveOrLoadNo), cmdOnTenshouExeJumpFromSelectSaveOrLoadNo, 5, NULL); //5�o�C�g�̂ݏ�������
}




