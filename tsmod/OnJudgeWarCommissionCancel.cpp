#include "WinTarget.h"

#include "CommonGlobalHandle.h"


/*
004103B5  |. E8 F82B0100    |CALL    TENSHOU.00422FB2
004103BA  |. 85C0           |TEST    EAX, EAX
004103BC  |. 74 23          |JE      SHORT TENSHOU.004103E1
*/

/*
 ���C�����̃f���L�����Z��
 */


int isJudgeMainCommissionCancel = 0;

void OnTenshouExeJudgeMainCommissionCancelExecute() {
	// �V�ċL���t�H�A�O���E���h�ł���AESC�L�[����������
	if ( GetForegroundWindow()==CommonGlobalHandle::tenshouWND && GetKeyState(VK_ESCAPE) < 0) {
		// �ϔC����
		isJudgeMainCommissionCancel = 1;
	}
} 
/* 
004103B5  |. E8 F82B0100    |CALL    TENSHOU.00422FB2
004103BA  |. 85C0           |TEST    EAX, EAX
004103BC  |. 74 23          |JE      SHORT TENSHOU.004103E1
*/ 
int pTenshouExeJumpFromToOnTenshouExeJudgeMainCommissionCancel    =0x4103B5; 
int pTenshouExeJumpCallFromToOnTenshouExeJudgeMainCommissionCancel=0x422FB2; 
int pTenshouExeReturnLblFromOnTenshouExeJudgeMainCommissionCancel =0x4103BA; 
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeJudgeMainCommissionCancel() { 
	__asm { 

		// TENSHOU.EXE����call
		call pTenshouExeJumpCallFromToOnTenshouExeJudgeMainCommissionCancel

		mov isJudgeMainCommissionCancel, eax	// ���ʂ�ۑ�

		push eax 
		push ebx 
		push ecx 
		push edx 
		push esp 
		push ebp 
		push esi 
		push edi 
	} 

	OnTenshouExeJudgeMainCommissionCancelExecute(); 

	__asm { 
		pop edi 
		pop esi 
		pop ebp 
		pop esp 
		pop edx 
		pop ecx 
		pop ebx 
		pop eax 

		mov eax, isJudgeMainCommissionCancel	// ���ʂ��㏑��

		jmp pTenshouExeReturnLblFromOnTenshouExeJudgeMainCommissionCancel 
	} 
} 
char cmdOnTenshouExeJumpFromJudgeMainCommissionCancel[6]="\xE9"; 
void WriteAsmJumperOnTenshouExeJudgeMainCommissionCancel() { 
	int iAddress = (int)OnTenshouExeJudgeMainCommissionCancel; 
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeJudgeMainCommissionCancel + 5 ) ; 
	memcpy(cmdOnTenshouExeJumpFromJudgeMainCommissionCancel+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B 
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeJudgeMainCommissionCancel), cmdOnTenshouExeJumpFromJudgeMainCommissionCancel, 5, NULL); // �ꉞ���S�̂��� 
} 


