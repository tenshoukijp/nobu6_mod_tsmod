/*
0041B37D  |> E8 55050600    CALL    TENSHOU.0047B8D7 �� �]��J�n
0041B382  |. E8 677F0000    CALL    TENSHOU.004232EE �� �]��I��
*/


#include "WinTarget.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"


// �R�c�����ǂ����̔���\�B�R�c�J�n�`�I���܂ŁA�isModeGungiDoing�v��1�ƂȂ�B
int isModeGungiDoing = 0L;

/*
0041B37D  |> E8 55050600    CALL    TENSHOU.0047B8D7 �� �]��J�n
				�������� JMP TSMod.OnTSExeGundiDoing �Ə��������Ă����ۂ̏���
0041B382  |. E8 677F0000    CALL    TENSHOU.004232EE �� �]��I��
*/
int pTenshouExeJumpFromToOnTenshouExeGundiDoing	 =0x41B37D; // �֐��͂��̃A�h���X����AOnTenshouExeGundiDoing�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeGundiDoing =0x47B8D7; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeGundiDoing  =0x41B382; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeGundiDoing() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov isModeGungiDoing, 1  // �R�c���[�h��TRUE�ɂ���B

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeGundiDoing

		mov isModeGungiDoing, 0  // �R�c���[�h��FALSE�ɂ���B

		jmp pTenshouExeReturnLblFromOnTenshouExeGundiDoing
	}
}



char cmdOnTenshouExeJumpFromGundiDoing[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeGundiDoing() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeGundiDoing;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeGundiDoing + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGundiDoing  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromGundiDoing+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeGundiDoing), cmdOnTenshouExeJumpFromGundiDoing, 5, NULL); //5�o�C�g�̂ݏ�������
}


