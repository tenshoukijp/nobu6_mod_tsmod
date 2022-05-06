#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "TSModCommand.h"

#include "ScenarioMod.h"
#include "PluginMod.h"

/*
 * �I���A�������́A�����I���̌��ʁA�E�C���h�E�T�C�Y���ύX����悤�Ƃ��Ă��鎞(���ꂽ����)�B
 */ 



/*
00499625  |. E8 7AEFFFFF    CALL    TENSHOU.004985A4 �� ���̃��\�b�h�ŕύX�����B
0049962A  |. E8 A790FBFF    CALL    TENSHOU.004526D6
0049962F  |. E8 EC9CF8FF    CALL    TENSHOU.00423320
*/

void OnTenshouExeAfterChangeWindowSizeExecute() {

	// PluginMod�n�̂O�ȏ�i�����\�j�Ȃc�k�k�̃��C���Q�[���J�n���̃��\�b�h���Ăяo���B
	// ���s�ΏۂƂȂ��Ă���DLL�����ɑ{�����āc
	for ( vector<HMODULE>::iterator it = listOfTargetDLL.begin(); it != listOfTargetDLL.end(); it++ ) {

		// �Ώۂ�DLL��MainGameStart�֐������݂��邩���`�F�b�N�B
		PFNTARGETMODAFTERCHANGEWINDOWSIZE p_AfterChangeWindowSize = (PFNTARGETMODAFTERCHANGEWINDOWSIZE)GetProcAddress(*it, "OnAfterChangeWindowSize");

		if ( !p_AfterChangeWindowSize ) {
			continue;
		}

		((PFNTARGETMODAFTERCHANGEWINDOWSIZE) p_AfterChangeWindowSize)();
	}

	// ScenarioMod�̓������\�b�h
	if ( p_snOnAfterChangeWindowSize ) {
		p_snOnAfterChangeWindowSize();
	}

	// �Ō�̂��ύX��`���X�V���Ă����B
	GetWindowRect( CommonGlobalHandle::tenshouWND, &CommonGlobalHandle::latestCenterWindowRect );
}

/*
00499625  |. E8 7AEFFFFF    CALL    TENSHOU.004985A4 �� ���̃��\�b�h�ŕύX�����B
				�������� JMP TSMod.OnTSExeAfterChangeWindowSize �Ə��������Ă����ۂ̏���
0049962A  |. E8 A790FBFF    CALL    TENSHOU.004526D6
*/
int pTenshouExeJumpFromToOnTenshouExeAfterChangeWindowSize		=0x499625; // �֐��͂��̃A�h���X����AOnTenshouExeAfterChangeWindowSize�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeAfterChangeWindowSize  =0x4985A4; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeAfterChangeWindowSize	=0x49962A; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeAfterChangeWindowSize() {

	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeAfterChangeWindowSize

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// �Q�[������ʂŃ��[�h���ɉ�������Ȃ�΁c
	OnTenshouExeAfterChangeWindowSizeExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeAfterChangeWindowSize
	}
}

char cmdOnTenshouExeJumpFromAfterChangeWindowSize[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeAfterChangeWindowSize() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeAfterChangeWindowSize;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeAfterChangeWindowSize + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeAfterChangeWindowSize  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromAfterChangeWindowSize+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeAfterChangeWindowSize), cmdOnTenshouExeJumpFromAfterChangeWindowSize, 5, NULL); //5�o�C�g�̂ݏ�������
}


