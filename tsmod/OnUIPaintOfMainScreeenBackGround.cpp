#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

/*
0046568F     BE 80010000    MOV     ESI, 180
00465694   . 50             PUSH    EAX                              ; /Arg3
00465695   . 33FF           XOR     EDI, EDI                         ; |
00465697   . 6A 30          PUSH    30                               ; |/Arg4 = 00000030
00465699   . 8D4D 9C        LEA     ECX, DWORD PTR SS:[EBP-64]       ; ||
0046569C   . 56             PUSH    ESI                              ; ||Arg3 => 00000180
0046569D   . 57             PUSH    EDI                              ; ||Arg2 => 00000000
0046569E   . 68 80020000    PUSH    280                              ; ||Arg1 = 00000280
004656A3   . E8 52610400    CALL    TENSHOU.004AB7FA                 ; |\TENSHOU.004AB7FA
004656A8   . 50             PUSH    EAX                              ; |Arg2
004656A9   . 8D4D 8C        LEA     ECX, DWORD PTR SS:[EBP-74]       ; |
004656AC   . 51             PUSH    ECX                              ; |Arg1
004656AD   . E8 A5650400    CALL    TENSHOU.004ABC57                 ; \TENSHOU.004ABC57

*/

/*
004231AD  /$ 55             PUSH    EBP

0046568F   . BE 80010000    MOV     ESI, 180

0046568F   . BE 80010000    MOV     ESI, 180
00465694   . 50             PUSH    EAX                              ; /Arg3
00465695   . 33FF           XOR     EDI, EDI                         ; |
00465697   . 6A 30          PUSH    30                               ; |/Arg4 = 00000030

0046575C     68 C0000000    PUSH    0C0


*/

/*
���̂Q�̕����̍��W��ύX���邱�ƂŁA�ʒu��ς��邱�Ƃ��o����悤���B
0046569E     68 80030000    PUSH    380
00465736     68 80030000    PUSH    380
*/

/*
004655BB     B8 24584600    MOV     EAX, TENSHOU.00465824		�֐��̂͂��߁B�����ȍ~�`RETURN�܂ŉ��ɂ܂��������������Ȃ��Ă�UI�̏���̔w�i���`�悳��Ȃ������ł���B

00465823   . C3             RETN		�����܂ł��֐�

*/

// ���C���X�N���[���̏㕔�̍��Ƃ������ɂ��炷Offset
int iOffsetOfUIPaintOfMainScreenBackGround = 0;

void OnTenshouExeUIPaintOfMainScreenBackGroundExecute() {
	if ( iOffsetOfUIPaintOfMainScreenBackGround == 0 ) {
		int iOffset = 0x280;
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x46569F), &iOffset, 2, NULL); //2�o�C�g�̂ݏ�������
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x465737), &iOffset, 2, NULL); //2�o�C�g�̂ݏ�������

	} else if ( iOffsetOfUIPaintOfMainScreenBackGround >= 1 ) {
		// ���C����ʏ㕔��OFFSET�l��ݒ肵�Ȃ����B���ʂ�0x280���̂܂�
		int iOffset1 = 0x280 +iOffsetOfUIPaintOfMainScreenBackGround+20;
		int iOffset2 = 0x280 +iOffsetOfUIPaintOfMainScreenBackGround;
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x46569F), &iOffset1, 2, NULL); //2�o�C�g�̂ݏ�������
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x465737), &iOffset2, 2, NULL); //2�o�C�g�̂ݏ�������
	}
}


/*
004655BB     B8 24584600    MOV     EAX, TENSHOU.00465824
				�������� JMP TSMod.OnUIPaintOfMainScreenBackGround �Ə��������Ă����ۂ̏���
004655C0   . E8 B7AE0400    CALL    TENSHOU.004B047C
*/

int pTenshouExeJumpFromToOnTenshouExeUIPaintOfMainScreenBackGround		=0x4655BB; // �֐��͂��̃A�h���X����AOnTenshouExeUIPaintOfMainScreenBackGround�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeUIPaintOfMainScreenBackGround	=0x4655C0; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeUIPaintOfMainScreenBackGround() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// �V�ċL�̌��X�̏���
		MOV     EAX, 0x465824

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	OnTenshouExeUIPaintOfMainScreenBackGroundExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeUIPaintOfMainScreenBackGround
	}
}


char cmdOnTenshouExeJumpFromUIPaintOfMainScreenBackGround[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeUIPaintOfMainScreenBackGround() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeUIPaintOfMainScreenBackGround;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeUIPaintOfMainScreenBackGround + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeUIPaintOfMainScreenBackGround  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromUIPaintOfMainScreenBackGround+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeUIPaintOfMainScreenBackGround), cmdOnTenshouExeJumpFromUIPaintOfMainScreenBackGround, 5, NULL); //5�o�C�g�̂ݏ�������
}




