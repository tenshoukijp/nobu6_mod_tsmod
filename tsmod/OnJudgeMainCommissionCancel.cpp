#include "WinTarget.h"

#include "CommonGlobalHandle.h"


/*
0046B4A0  |. E8 0D7BFBFF    |CALL    TENSHOU.00422FB2
0046B4A5  |. 85C0           |TEST    EAX, EAX
0046B4A7  |. 74 3C          |JE      SHORT TENSHOU.0046B4E5
*/

/*
 �푈���̈ϔC�L�����Z��
 */
/*
GetKeyState�̖߂�l�͂ǂ����1�񉟂���-127�ŗ�����0�ɖ߂�2��ځi�A�łł��Ȃ��H�j������-128�����ė�����1�ɖ߂��Ă���悤�ȋC�������ł����A
�����JaritenCat����̌����鐔��ff80�Ȃǂ��֌W���Ă���̂ł��傤���H�ł�ff80��-127�͓����Ȃ�ł��傤���H�H

 ���ŉ��ʃr�b�g�͉����ė������т�0��1������ւ��܁����B�i�g�O������Ƃ����܂��j
�Ƃ������Ƃ͂��ꂪ1��ډ�����0��2��ډ�����1��3��ډ�����0�Ƃ����悤�ɂȂ��Ă��錴���ł��傤���B
*/


int isJudgeWarCommissionCancel = 0;

void OnTenshouExeJudgeWarCommissionCancelExecute() {
	// �V�ċL���t�H�A�O���E���h�ł���AESC�L�[����������
	if ( GetForegroundWindow()==CommonGlobalHandle::tenshouWND && GetKeyState(VK_ESCAPE) < 0) {
		// �ϔC����
		isJudgeWarCommissionCancel = 1;
	}
} 
/* 
0046B4A0  |. E8 0D7BFBFF    |CALL    TENSHOU.00422FB2
0046B4A5  |. 85C0           |TEST    EAX, EAX						EAX��1�Ȃ�ϔC�L�����Z��
0046B4A7  |. 74 3C          |JE      SHORT TENSHOU.0046B4E5
*/ 
int pTenshouExeJumpFromToOnTenshouExeJudgeWarCommissionCancel    =0x46B4A0; 
int pTenshouExeJumpCallFromToOnTenshouExeJudgeWarCommissionCancel=0x422FB2; 
int pTenshouExeReturnLblFromOnTenshouExeJudgeWarCommissionCancel =0x46B4A5; 
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeJudgeWarCommissionCancel() { 
	__asm { 

		// TENSHOU.EXE����call
		call pTenshouExeJumpCallFromToOnTenshouExeJudgeWarCommissionCancel

		mov isJudgeWarCommissionCancel, eax	// ���ʂ�ۑ�

		push eax 
		push ebx 
		push ecx 
		push edx 
		push esp 
		push ebp 
		push esi 
		push edi 
	} 

	OnTenshouExeJudgeWarCommissionCancelExecute(); 

	__asm { 
		pop edi 
		pop esi 
		pop ebp 
		pop esp 
		pop edx 
		pop ecx 
		pop ebx 
		pop eax 

		mov eax, isJudgeWarCommissionCancel	// ���ʂ��㏑��

		jmp pTenshouExeReturnLblFromOnTenshouExeJudgeWarCommissionCancel 
	} 
} 
char cmdOnTenshouExeJumpFromJudgeWarCommissionCancel[6]="\xE9"; 
void WriteAsmJumperOnTenshouExeJudgeWarCommissionCancel() { 
	int iAddress = (int)OnTenshouExeJudgeWarCommissionCancel; 
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeJudgeWarCommissionCancel + 5 ) ; 
	memcpy(cmdOnTenshouExeJumpFromJudgeWarCommissionCancel+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B 
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeJudgeWarCommissionCancel), cmdOnTenshouExeJumpFromJudgeWarCommissionCancel, 5, NULL); // �ꉞ���S�̂��� 
} 


