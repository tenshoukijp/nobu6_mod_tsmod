#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "MessageN6PPatch.h"


/*
 * �t�H�[�J�X�̕��o�����؂�ւ�����^�C�~���O
 */

void OnTenshouExeFukidashiTypeBFocusChangeExecute() {
	// �V�K���b�Z�[�W�͎g���Ă��Ȃ����ƂƂȂ�B
}


/*
0042312A   $ E8 F13E0400    CALL    TENSHOU.00467020
				�������� JMP TSMod.OnTSExeFukidashiTypeBFocusChange �Ə��������Ă����ۂ̏���
0042312F   . 83F8 04        CMP     EAX, 4
00423132   . 73 13          JNB     SHORT TENSHOU.00423147
00423134   . 0FAF4424 04    IMUL    EAX, DWORD PTR SS:[ESP+4]
 */
int pTenshouExeJumpFromToOnTenshouExeFukidashiTypeBFocusChange    =0x42312A; // �֐��͂��̃A�h���X����AOnTenshouExeFukidashiTypeBFocusChange�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeFukidashiTypeBFocusChange=0x467020; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeFukidashiTypeBFocusChange =0x42312F; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeFukidashiTypeBFocusChange() {
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
	
	// 
	OnTenshouExeFukidashiTypeBFocusChangeExecute();

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
		call pTenshouExeJumpCallFromToOnTenshouExeFukidashiTypeBFocusChange

		jmp pTenshouExeReturnLblFromOnTenshouExeFukidashiTypeBFocusChange
	}
}

/*
0042312A   $ E8 F13E0400    CALL    TENSHOU.00467020
				�������� JMP TSMod.OnTSExeFukidashiTypeBFocusChange �Ə��������Ă����ۂ̏���
0042312F   . 83F8 04        CMP     EAX, 4
00423132   . 73 13          JNB     SHORT TENSHOU.00423147
00423134   . 0FAF4424 04    IMUL    EAX, DWORD PTR SS:[ESP+4]
*/
char cmdOnTenshouExeJumpFromFukidashiTypeBFocusChange[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeFukidashiTypeBFocusChange() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeFukidashiTypeBFocusChange;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeFukidashiTypeBFocusChange + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeFukidashiTypeBFocusChange  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromFukidashiTypeBFocusChange+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeFukidashiTypeBFocusChange), cmdOnTenshouExeJumpFromFukidashiTypeBFocusChange, 5, NULL); //5�o�C�g�̂ݏ�������
}


//--------------------------------------------------------------------------------------------------------------------




/*
 * �t�H�[�J�X�̕��o���̃t�H�[�J�X���؂�ւ��A���̏����O�̃^�C�~���O
 * ���ꂩ�畬�o���̃t�H�[�J�X���ς�낤�Ƃ��Ă���A�܂��������}�b�v��̃��b�Z�[�W�͎��̃��b�Z�[�W�ւƐ؂�ւ���Ă��Ȃ��B
 */

void OnTenshouExeFukidashiTypeBMessageChangePrevExecute() {
	MessageN6PPatch::isOnTenshouExeMessageCopyUsed = false;
}


/*
0046C1CB  /$ C705 5CD74C00 >MOV     DWORD PTR DS:[4CD75C], TENSHOU.004D97C0          ;  ASCII "�����킩����
����͂�����
�����i���悱��"
0046C1D5  |. 33C0           XOR     EAX, EAX
0046C1D7  |. A3 68D74C00    MOV     DWORD PTR DS:[4CD768], EAX
0046C1DC  |. A3 60D74C00    MOV     DWORD PTR DS:[4CD760], EAX
0046C1E1  |. A3 64D74C00    MOV     DWORD PTR DS:[4CD764], EAX
0046C1E6  |. A3 FCD74C00    MOV     DWORD PTR DS:[4CD7FC], EAX
				�������� JMP TSMod.OnTSExeFukidashiTypeBMessageChange �Ə��������Ă����ۂ̏���
0046C1EB  \. C3             RETN
 */
int pTenshouExeJumpFromToOnTenshouExeFukidashiTypeBMessageChange    =0x46C1E6; // �֐��͂��̃A�h���X����AOnTenshouExeFukidashiTypeBMessageChange�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeFukidashiTypeBMessageChange =0x46C1EB; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeFukidashiTypeBMessageChange() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// ���XTENSHOU.EXE���ɂ��������̂�������
		MOV     DWORD PTR DS:[0x4CD7FC], EAX

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeFukidashiTypeBMessageChangePrevExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeFukidashiTypeBMessageChange
	}
}

/*
0046C1CB  /$ C705 5CD74C00 >MOV     DWORD PTR DS:[4CD75C], TENSHOU.004D97C0          ;  ASCII "�����킩����
����͂�����
�����i���悱��"
0046C1D5  |. 33C0           XOR     EAX, EAX
0046C1D7  |. A3 68D74C00    MOV     DWORD PTR DS:[4CD768], EAX
0046C1DC  |. A3 60D74C00    MOV     DWORD PTR DS:[4CD760], EAX
0046C1E1  |. A3 64D74C00    MOV     DWORD PTR DS:[4CD764], EAX
0046C1E6  |. A3 FCD74C00    MOV     DWORD PTR DS:[4CD7FC], EAX
				�������� JMP TSMod.OnTSExeFukidashiTypeBMessageChange �Ə��������Ă����ۂ̏���
0046C1EB  \. C3             RETN
*/
char cmdOnTenshouExeJumpFromFukidashiTypeBMessageChange[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeFukidashiTypeBMessageChange() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeFukidashiTypeBMessageChange;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeFukidashiTypeBMessageChange + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeFukidashiTypeBMessageChange  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromFukidashiTypeBMessageChange+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeFukidashiTypeBMessageChange), cmdOnTenshouExeJumpFromFukidashiTypeBMessageChange, 5, NULL); //5�o�C�g�̂ݏ�������
}








/*
 * �t�H�[�J�X�̕��o���̂P���ځA���̏����O�̃^�C�~���O
 * ���ꂩ�畬�o���̃t�H�[�J�X���ς�낤�Ƃ��Ă���A�܂��������}�b�v��̃��b�Z�[�W�͎��̃��b�Z�[�W�ւƐ؂�ւ���Ă��Ȃ��B
 */

void OnTenshouExeFukidashiTypeBMessageStartPrevExecute() {
	MessageN6PPatch::isOnTenshouExeMessageCopyUsed = false;
}



/*
0046C1A8  |. FF05 5CD74C00  INC     DWORD PTR DS:[4CD75C]                         ;  TENSHOU.004D97C0
0046C1AE  |. FF05 68D74C00  INC     DWORD PTR DS:[4CD768]
0046C1B4  |. 813D 68D74C00 >CMP     DWORD PTR DS:[4CD768], 1FE
0046C1BE  |. 72 0A          JB      SHORT TENSHOU.0046C1CA
0046C1C0  |. C705 FCD74C00 >MOV     DWORD PTR DS:[4CD7FC], 1
				�������� JMP TSMod.OnTSExeFukidashiTypeBMessageStart �Ə��������Ă����ۂ̏���
0046C1CA  \> C3             RETN
 */
int pTenshouExeJumpFromToOnTenshouExeFukidashiTypeBMessageStart    =0x46C1C0; // �֐��͂��̃A�h���X����AOnTenshouExeFukidashiTypeBMessageStart�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeFukidashiTypeBMessageStart =0x46C1CA; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeFukidashiTypeBMessageStart() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// ���XTENSHOU.EXE���ɂ��������̂�������
		MOV     DWORD PTR DS:[0x4CD7FC], 1

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeFukidashiTypeBMessageStartPrevExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeFukidashiTypeBMessageStart
	}
}

/*
0046C1CB  /$ C705 5CD74C00 >MOV     DWORD PTR DS:[4CD75C], TENSHOU.004D97C0          ;  ASCII "�����킩����
����͂�����
�����i���悱��"
0046C1D5  |. 33C0           XOR     EAX, EAX
0046C1D7  |. A3 68D74C00    MOV     DWORD PTR DS:[4CD768], EAX
0046C1DC  |. A3 60D74C00    MOV     DWORD PTR DS:[4CD760], EAX
0046C1E1  |. A3 64D74C00    MOV     DWORD PTR DS:[4CD764], EAX
0046C1E6  |. A3 FCD74C00    MOV     DWORD PTR DS:[4CD7FC], EAX
				�������� JMP TSMod.OnTSExeFukidashiTypeBMessageStart �Ə��������Ă����ۂ̏���
0046C1EB  \. C3             RETN
*/
char cmdOnTenshouExeJumpFromFukidashiTypeBMessageStart[7]="\xE9\x90\x90\x90\x90\x90";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�U�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeFukidashiTypeBMessageStart() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeFukidashiTypeBMessageStart;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeFukidashiTypeBMessageStart + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeFukidashiTypeBMessageStart  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromFukidashiTypeBMessageStart+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeFukidashiTypeBMessageStart), cmdOnTenshouExeJumpFromFukidashiTypeBMessageStart, 6, NULL); //6�o�C�g�̂ݏ�������
}

