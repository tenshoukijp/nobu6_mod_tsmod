#include "CommonGlobalHandle.h"
#include "DialogKahouMovie.h"
#include "GameDataStruct.h"

/*
�ȉ���
�uJNZ     SHORT TENSHOU.00494AE6�v��NOP,NOP�ł���Ԃ��Ă����ƁA�ƕ�_�C�A���O���傫�����������ɂ��A�K���`�悵�Ȃ����B

00494ADC   . 833E 04        CMP     DWORD PTR DS:[ESI], 4
00494ADF   . 75 05          JNZ     SHORT TENSHOU.00494AE6
*/

char cmdOnTenshouExeJumpFromJudgeKahouDialogBigMove[3]="\x90\x90";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v5�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�

// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeJudgeKahouDialogBigMove() {

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x494ADF), cmdOnTenshouExeJumpFromJudgeKahouDialogBigMove, 2, NULL); //2�o�C�g�̂ݏ�������
}
