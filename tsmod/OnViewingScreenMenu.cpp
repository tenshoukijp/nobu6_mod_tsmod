#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


void OnTenshouExeViewingScreenMenuExecute() {
	// �Q�[���͂͂��܂���(���낢��ȃ��������������Ăn�j���ǂ����̃t���O�ƂȂ�)
	CommonGlobalHandle::isNobunaganoYabouGameStart = true;
}


/*
0049902C  |. 8BCE           MOV     ECX, ESI
0049902E  |. 68 B8334D00    PUSH    TENSHOU.004D33B8                 ;  ASCII "��ʃT�C�Y��I�����Ă�������"
00499033  |. E8 16550100    CALL    TENSHOU.004AE54E
				�������� JMP TSMod.OnTSExeViewingScreenMenu �Ə��������Ă����ۂ̏���
00499038  |. 5E             POP     ESI
*/
int pTenshouExeJumpFromToOnTenshouExeViewingScreenMenu		=0x499033; // �֐��͂��̃A�h���X����AOnTenshouExeViewingScreenMenu�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeViewingScreenMenu  =0x4AE54E; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeViewingScreenMenu	=0x499038; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeViewingScreenMenu() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeViewingScreenMenu

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
	OnTenshouExeViewingScreenMenuExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingScreenMenu
	}
}

/*
0049902C  |. 8BCE           MOV     ECX, ESI
0049902E  |. 68 B8334D00    PUSH    TENSHOU.004D33B8                 ;  ASCII "��ʃT�C�Y��I�����Ă�������"
00499033  |. E8 16550100    CALL    TENSHOU.004AE54E
				�������� JMP TSMod.OnTSExeViewingScreenMenu �Ə��������Ă����ۂ̏���
00499038  |. 5E             POP     ESI
*/
char cmdOnTenshouExeJumpFromViewingScreenMenu[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeViewingScreenMenu() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeViewingScreenMenu;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingScreenMenu + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingScreenMenu  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromViewingScreenMenu+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingScreenMenu), cmdOnTenshouExeJumpFromViewingScreenMenu, 5, NULL); //5�o�C�g�̂ݏ�������
}




