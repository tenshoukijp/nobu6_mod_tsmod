#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "FuncPlayMovie.h"

void OnTenshouExeMovieBlackBack1Execute() {
}


/*
0043CE65  |. 56             PUSH    ESI
0043CE66     E8 07010000    CALL    TENSHOU.0043CF72
				�������� JMP TSMod.OnTSExeMovieBlackBack1 �Ə��������Ă����ۂ̏���
0043CE6B  |. 83C4 04        ADD     ESP, 4
*/
int pTenshouExeJumpFromToOnTenshouExeMovieBlackBack1	 =0x43CE66; // �֐��͂��̃A�h���X����AOnTenshouExeMovieBlackBack1�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeMovieBlackBack1 =0x43CF72; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeMovieBlackBack1	 =0x43CE6B; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeMovieBlackBack1() {
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
	OnTenshouExeMovieBlackBack1Execute();

	// �u���b�N�o�b�N����Ȃ炱�����B
	if ( isMovieWithBlackBack ) {
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
			call pTenshouExeJumpCallFromToOnTenshouExeMovieBlackBack1

			jmp pTenshouExeReturnLblFromOnTenshouExeMovieBlackBack1
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

			jmp pTenshouExeReturnLblFromOnTenshouExeMovieBlackBack1
		}
	}
}

char cmdOnTenshouExeJumpFromMovieBlackBack1[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeMovieBlackBack1() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeMovieBlackBack1;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeMovieBlackBack1 + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeMovieBlackBack1  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromMovieBlackBack1+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeMovieBlackBack1), cmdOnTenshouExeJumpFromMovieBlackBack1, 5, NULL); //5�o�C�g�̂ݏ�������
}




//------------------------------------------------------------------------------------


void OnTenshouExeMovieBlackBack2Execute() {
}


/*
0044AB0D  |. FF15 34764E00  CALL    DWORD PTR DS:[<&USER32.EnableMen>; \EnableMenuItem
0044AB13  |. E8 B022FDFF    CALL    TENSHOU.0041CDC8
				�������� JMP TSMod.OnTSExeMovieBlackBack2 �Ə��������Ă����ۂ̏���
0044AB18  |. 6A 01          PUSH    1                                ; /Erase = TRUE
*/
int pTenshouExeJumpFromToOnTenshouExeMovieBlackBack2		=0x44AB13; // �֐��͂��̃A�h���X����AOnTenshouExeMovieBlackBack2�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeMovieBlackBack2    =0x41CDC8; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeMovieBlackBack2	    =0x44AB18; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeMovieBlackBack2() {
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
	OnTenshouExeMovieBlackBack2Execute();

	// �u���b�N�o�b�N����Ȃ炱�����B
	if ( isMovieWithBlackBack ) {
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
			call pTenshouExeJumpCallFromToOnTenshouExeMovieBlackBack2

			jmp pTenshouExeReturnLblFromOnTenshouExeMovieBlackBack2
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

			jmp pTenshouExeReturnLblFromOnTenshouExeMovieBlackBack2
		}
	}
}

char cmdOnTenshouExeJumpFromMovieBlackBack2[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeMovieBlackBack2() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeMovieBlackBack2;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeMovieBlackBack2 + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeMovieBlackBack2  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromMovieBlackBack2+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeMovieBlackBack2), cmdOnTenshouExeJumpFromMovieBlackBack2, 5, NULL); //5�o�C�g�̂ݏ�������
}



