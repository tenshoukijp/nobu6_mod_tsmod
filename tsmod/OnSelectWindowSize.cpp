#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "TSModCommand.h"

#include "ScenarioMod.h"
#include "PluginMod.h"

/*
 * �E�B���h�E�T�C�Y���ǂ���ɂ��邩���肵����
 */ 


// ���j���[�o�[�ƃ^�C�g���o�[�𖳂���
extern void setNoCaptionAndMenuBarAfter();

void OnTenshouExeSelectWindowSizeExecute() {

	CommonGlobalHandle::IsAfterScreenSizeSelected = TRUE;

	// ���j���[�o�[�ƃ^�C�g���o�[�𖳂���
	setNoCaptionAndMenuBarAfter();
}

/*
0049945C  |. 50             PUSH    EAX
0049945D  |. 74 07          JE      SHORT TENSHOU.00499466
0049945F  |. E8 B3FCFFFF    CALL    TENSHOU.00499117
				�������� JMP TSMod.OnTSExeSelectWindowSize �Ə��������Ă����ۂ̏���
00499464  |. EB 05          JMP     SHORT TENSHOU.0049946B

*/
int pTenshouExeJumpFromToOnTenshouExeSelectWindowSize		=0x49945F; // �֐��͂��̃A�h���X����AOnTenshouExeSelectWindowSize�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeSelectWindowSize   =0x499117; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeSelectWindowSize	=0x499464; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeSelectWindowSize() {

	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeSelectWindowSize

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
	OnTenshouExeSelectWindowSizeExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeSelectWindowSize
	}
}

char cmdOnTenshouExeJumpFromSelectWindowSize[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeSelectWindowSize() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeSelectWindowSize;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSelectWindowSize + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeSelectWindowSize  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromSelectWindowSize+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSelectWindowSize), cmdOnTenshouExeJumpFromSelectWindowSize, 5, NULL); //5�o�C�g�̂ݏ�������
}


