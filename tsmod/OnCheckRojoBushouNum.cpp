#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"

/*
 �ď鎞�A������ɉ��l����̂�(�o�w���Ă��Ȃ��̂��܂߂�)�B
 �ď鎞�̉�ʂɏo�颕�������̐l���𓾂Ă������߁B
 */

int iCheckRojoBushouNum = -1;
void OnTenshouExeCheckRojoBushouNumExecute() {
}

/*
004620A2  |. 56             PUSH    ESI                              ; |Arg3
004620A3  |. 6A 27          PUSH    27                               ; |Arg2 = 00000027
004620A5  |. 57             PUSH    EDI                              ; |Arg1
004620A6  |. E8 62BFFAFF    CALL    TENSHOU.0040E00D                 ; \TENSHOU.0040E00D  �� ���̊֐���ESI��(�ď鎞�̉�ʂɏo�颕�������̐l�����o��)
004620AB  |. 83C4 18        ADD     ESP, 18
*/ 
int pTenshouExeJumpFromToOnTenshouExeCheckRojoBushouNum    =0x4620A6; 
int pTenshouExeJumpCallFromToOnTenshouExeCheckRojoBushouNum=0x40E00D; 
int pTenshouExeReturnLblFromOnTenshouExeCheckRojoBushouNum =0x4620AB; 
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeCheckRojoBushouNum() { 
	__asm { 

		// TENSHOU.EXE����call
		call pTenshouExeJumpCallFromToOnTenshouExeCheckRojoBushouNum
		
		// ���ʂ��ď�l���𓾂Ă����B
		mov iCheckRojoBushouNum, esi;

		push eax 
		push ebx 
		push ecx 
		push edx 
		push esp 
		push ebp 
		push esi 
		push edi 
	} 

	OnTenshouExeCheckRojoBushouNumExecute(); 

	__asm { 
		pop edi 
		pop esi 
		pop ebp 
		pop esp 
		pop edx 
		pop ecx 
		pop ebx 
		pop eax 

		jmp pTenshouExeReturnLblFromOnTenshouExeCheckRojoBushouNum  // �g�킸�W�����v
	} 
} 
char cmdOnTenshouExeJumpFromCheckRojoBushouNum[6]="\xE9"; 
void WriteAsmJumperOnTenshouExeCheckRojoBushouNum() { 
	int iAddress = (int)OnTenshouExeCheckRojoBushouNum; 
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeCheckRojoBushouNum + 5 ) ; 
	memcpy(cmdOnTenshouExeJumpFromCheckRojoBushouNum+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B 
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeCheckRojoBushouNum), cmdOnTenshouExeJumpFromCheckRojoBushouNum, 5, NULL); // �ꉞ���S�̂��� 
} 


