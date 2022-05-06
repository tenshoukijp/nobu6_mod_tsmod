#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"


/*
0047D24A   . 56             PUSH    ESI
0047D24B   . 57             PUSH    EDI
0047D24C   . 8B39           MOV     EDI, DWORD PTR DS:[ECX]
0047D24E   . 8BF1           MOV     ESI, ECX
0047D250   . FF57 30        CALL    DWORD PTR DS:[EDI+30]		�� �֐��|�C���^�W�����v
0047D253   . 8BCE           MOV     ECX, ESI
0047D255   . FF57 34        CALL    DWORD PTR DS:[EDI+34]		�� �֐��|�C���^�W�����v
0047D258   . 5F             POP     EDI
0047D259   . 5E             POP     ESI
0047D25A   . C3             RETN
*/

extern int iOffsetOfUIPaintOfMainScreenBackGround; // �㕔UI�̍��Ƃ��̉E�ւ�Offset�l
extern int iConstWideResolutionSize;

int IsOnUIPaintOfMainScreeenBackGround = FALSE;

int iEDIOfTenshouExeFunctionPointerTableJump = 0; // EDI�̒l�B���̒l���uEDI==0x4B5608�v�̎��A���C����ʂ̏㕔�̂t�h��\�����鎞�̔ԍ�

void OnTenshouExeFunctionPointerTableJumpExecute() {

	IsOnUIPaintOfMainScreeenBackGround = FALSE;

	// �܂����C����ʂ�UI�̏㕔�̊֐��|�C���^�ւƔ��ł��Ă��邱��
	// ������1600�ł��邱�ƁB
	if ( iEDIOfTenshouExeFunctionPointerTableJump == 0x4B5608 && GetResolutionWidth()==iConstWideResolutionSize ) {
		IsOnUIPaintOfMainScreeenBackGround = TRUE; // �y�C���g���J��Ԃ����[�h
	}
}


/*
0047D253   . 8BCE           MOV     ECX, ESI
0047D255   . FF57 34        CALL    DWORD PTR DS:[EDI+34]		�� �֐��|�C���^�W�����v
				�������� JMP TSMod.OnFunctionPointerTableJump �Ə��������Ă����ۂ̏���
0047D258   . 5F             POP     EDI
*/

int pTenshouExeJumpFromToOnTenshouExeFunctionPointerTableJump		=0x47D253; // �֐��͂��̃A�h���X����AOnTenshouExeFunctionPointerTableJump�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeFunctionPointerTableJump	=0x47D258; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeFunctionPointerTableJump() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iOffsetOfUIPaintOfMainScreenBackGround, 0 // ��vUI�w�i��Offset�l

		// �V�ċL�̌��X�̏���
		MOV     ECX, ESI
		CALL    DWORD PTR DS:[EDI+0x34]	// 	�� �֐��|�C���^�W�����v

		mov iEDIOfTenshouExeFunctionPointerTableJump, EDI // EDI�Ŋ֐��|�C���^�ւƃW�����v����̂ŁA���̒l���S��

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	OnTenshouExeFunctionPointerTableJumpExecute();

	if ( IsOnUIPaintOfMainScreeenBackGround ) {

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

			mov iOffsetOfUIPaintOfMainScreenBackGround, 256;
			// �V�ċL�̌��X�̏���
			MOV     ECX, ESI
			CALL    DWORD PTR DS:[EDI+0x34]	// 	�� �֐��|�C���^�W�����v


			mov iOffsetOfUIPaintOfMainScreenBackGround, 512;
			// �V�ċL�̌��X�̏���
			MOV     ECX, ESI
			CALL    DWORD PTR DS:[EDI+0x34]	// 	�� �֐��|�C���^�W�����v

			push eax
			push ebx
			push ecx
			push edx
			push esp
			push ebp
			push esi
			push edi
		}
	}

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

		jmp pTenshouExeReturnLblFromOnTenshouExeFunctionPointerTableJump
	}
}


char cmdOnTenshouExeJumpFromFunctionPointerTableJump[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeFunctionPointerTableJump() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeFunctionPointerTableJump;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeFunctionPointerTableJump + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeFunctionPointerTableJump  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromFunctionPointerTableJump+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeFunctionPointerTableJump), cmdOnTenshouExeJumpFromFunctionPointerTableJump, 5, NULL); //5�o�C�g�̂ݏ�������
}

