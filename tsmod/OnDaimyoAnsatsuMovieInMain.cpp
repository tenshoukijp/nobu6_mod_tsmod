#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "FuncPlayBGM.h"

// �喼�ÎE�������̃��[�r�[�����ꂽ��


/*
0045B934  |> 6A 01          PUSH    1                                ; /Arg2 = 00000001
0045B936  |. 6A 1B          PUSH    1B                               ; |Arg1 = 0000001B
0045B938  |. E8 2714FEFF    CALL    TENSHOU.0043CD64                 ; \TENSHOU.0043CD64
0045B93D  |. 83C4 08        ADD     ESP, 8
*/

extern int iAnsatuBGMOfPatchInAnsatsuBushouInMain;
void OnTenshouExeDaimyoAnsatsuMovieInMainExecute() {
	// ����̓p�b�`�B�喼�ÎE���Ƀ��[�r�[�������ƁABGM���Đ�����Ȃ��Ȃ��Ă��܂��̂ŁA
	// ����܂ōĐ�����Ă���BGM�������I�ɍĐ�����B
	if (iAnsatuBGMOfPatchInAnsatsuBushouInMain) {
		FuncPlayBGM(iAnsatuBGMOfPatchInAnsatsuBushouInMain);
	}
}


int pTenshouExeJumpFromToOnTenshouExeDaimyoAnsatsuMovieInMain    =0x45B938; // �֐��͂��̃A�h���X����AOnTenshouExeDaimyoAnsatsuMovieInMain�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeDaimyoAnsatsuMovieInMain=0x43CD64; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeDaimyoAnsatsuMovieInMain =0x45B93D; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeDaimyoAnsatsuMovieInMain() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		call pTenshouExeJumpCallFromToOnTenshouExeDaimyoAnsatsuMovieInMain

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
	OnTenshouExeDaimyoAnsatsuMovieInMainExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeDaimyoAnsatsuMovieInMain
	}
}


char cmdOnTenshouExeJumpFromDaimyoAnsatsuMovieInMain[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeDaimyoAnsatsuMovieInMain() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeDaimyoAnsatsuMovieInMain;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeDaimyoAnsatsuMovieInMain + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeDaimyoAnsatsuMovieInMain  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromDaimyoAnsatsuMovieInMain+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeDaimyoAnsatsuMovieInMain), cmdOnTenshouExeJumpFromDaimyoAnsatsuMovieInMain, 5, NULL); //5�o�C�g�̂ݏ�������
}




