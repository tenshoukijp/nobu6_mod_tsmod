#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

// ���C���E�B���h�E�́u�G�v�u��v�u���v�́u�G�v�̃{�^������������

BOOL isMainWindowCastleButtonMode = 0;
void OnTenshouExeMainWindowCastleButtonExecute() {

}

/*
00476B59   . E8 D5CBFDFF    CALL    TENSHOU.00453733
00476B5E   . 83C4 08        ADD     ESP, 8
*/

int pTenshouExeJumpFromToOnTenshouExeMainWindowCastleButton		=0x476B59; // �֐��͂��̃A�h���X����AOnTenshouExeMainWindowCastleButton�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeMainWindowCastleButton =0x453733; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeMainWindowCastleButton	=0x476B5E; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeMainWindowCastleButton() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov isMainWindowCastleButtonMode, 1

		call pTenshouExeJumpCallFromToOnTenshouExeMainWindowCastleButton

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// ���������ύX����������Ȃ�΁c
	OnTenshouExeMainWindowCastleButtonExecute();

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

		mov isMainWindowCastleButtonMode, 0


		jmp pTenshouExeReturnLblFromOnTenshouExeMainWindowCastleButton
	}
}

char cmdOnTenshouExeJumpFromMainWindowCastleButton[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeMainWindowCastleButton() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeMainWindowCastleButton;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeMainWindowCastleButton + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeMainWindowCastleButton  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromMainWindowCastleButton+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeMainWindowCastleButton), cmdOnTenshouExeJumpFromMainWindowCastleButton, 5, NULL); //5�o�C�g�̂ݏ�������
}




