#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"

/*
 * �}�E�X�ɑ���������̂��ڑ�����Ă��邩�ǂ����B
 * �m�[�g�p�\�R���Ȃǂ̃p�b�h�Ȃǂ��}�E�X�Ƃ݂Ȃ����B
 * �����̊��S�^�u���b�g�ɔ����āA���S�Ƀ}�E�X�������Ƃ����삷��悤�ɂ���B
 */


int pTenshouExeJumpFromToOnTenshouExeResultOfMouseDriverInstalledInPC = 0x417533; 


/*
00417529  |. 6A 2B          PUSH    2B                               ; /Index = SM_CMOUSEBUTTONS
0041752B  |. FF15 60764E00  CALL    DWORD PTR DS:[<&USER32.GetSystem>; \GetSystemMetrics
00417531  |. 85C0           TEST    EAX, EAX
00417533  |. 75 07          JNZ     SHORT TENSHOU.0041753C
				�������� JMP SHORT TENSHOU.0041753C �ƁuJNZ�v���uJMP�v�Ə���������B�� �����������Ԃɍ���Ȃ��̂�tenshou.exe ���̂�����������!!
00417535  |. BE 90C44B00    MOV     ESI, TENSHOU.004BC490            ;  ASCII "�}�E�X���C���X�g�[������Ă��܂���B"
0041753A  |. EB 6B          JMP     SHORT TENSHOU.004175A7

 */

// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeResultOfMouseDriverInstalledInPC() {

	char sz[] = "\xEB"; // jmp�Ƃ����A�Z���u��

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeResultOfMouseDriverInstalledInPC), sz, 1, NULL); //1�o�C�g�̂ݏ�������
}

