#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "FuncPlayMovie.h"


void OnTenshouExeMovieBGMNonStopExecute() {
}


/*
0043CE55  |. 0F84 D7000000  JE      TENSHOU.0043CF32
0043CE5B     E8 23060500    CALL    TENSHOU.0048D483
				�������� JMP TSMod.OnTSExeMovieBGMNonStop �Ə��������Ă����ۂ̏���
0043CE60  |. E8 F8030500    CALL    TENSHOU.0048D25D
*/
int pTenshouExeJumpFromToOnTenshouExeMovieBGMNonStop	 =0x43CE5B; // �֐��͂��̃A�h���X����AOnTenshouExeMovieBGMNonStop�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeMovieBGMNonStop =0x48D483; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeMovieBGMNonStop	 =0x43CE60; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeMovieBGMNonStop() {
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
	
	// �Q�[���f�[�^���v���C���[�^�[�����ς�����i�K�œǂݍ��ݍX�V���Ă���
	OnTenshouExeMovieBGMNonStopExecute();

	// �u���b�N�o�b�N����Ȃ炱�����B
	if ( isMovieWithBGMSTOP ) {
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
			call pTenshouExeJumpCallFromToOnTenshouExeMovieBGMNonStop

			jmp pTenshouExeReturnLblFromOnTenshouExeMovieBGMNonStop
		}
	
	// �u���b�N�o�b�N���Ȃ��Ȃ炱����
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

			jmp pTenshouExeReturnLblFromOnTenshouExeMovieBGMNonStop
		}
	}
}

char cmdOnTenshouExeJumpFromMovieBGMNonStop[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeMovieBGMNonStop() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeMovieBGMNonStop;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeMovieBGMNonStop + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeMovieBGMNonStop  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromMovieBGMNonStop+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeMovieBGMNonStop), cmdOnTenshouExeJumpFromMovieBGMNonStop, 5, NULL); //5�o�C�g�̂ݏ�������
}



