#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"


/*
 ���C����ʒ��̌R�c�^�[�����؂�ւ�������B
 ��G�߂P�񂸂݂̂̂悤���B
 �Z�[�u����̃��[�h���ɂ͎��s����Ȃ��B
 ���̗��R�̍l�����Ƃ��āA����ύX���\�b�h�Ɠ������Ǝv���΂悢���낤�B
 */

/*
004670C0  |. 77 08                   JA      SHORT TENSHOU.004670CA
004670C2  |. 66:A3 57934C00          MOV     WORD PTR DS:[4C9357], AX�@�� ������nb6turn.gundan �ɒl���������B
004670C8  |. EB 11                   JMP     SHORT TENSHOU.004670DB
*/

int iGundanValueOfGundanTurnChange = -1;
void OnTenshouExeGundanTurnChangeExecute() {
	int iGundanID = (iGundanValueOfGundanTurnChange & 0xFFFF) - 1; // AX���l�Ȃ̂ŁA���ʃr�b�g�������p����B

	if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
		int iBushouID = nb6gundan[iGundanID].leader - 1;
		if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

			// ScenarioMod�̌R�c�^�[���؂�ւ��ڂ̊֐��̌Ăяo��
			if ( p_snOnGundanTurnInMainScreen ) {
				p_snOnGundanTurnInMainScreen( iGundanID + 1 ); // �R�c�ԍ��Ƃ��ēn���̂łP������B
			}
		}
	}

}


/*
004670C0  |. 77 08                   JA      SHORT TENSHOU.004670CA
004670C2  |. 66:A3 57934C00          MOV     WORD PTR DS:[4C9357], AX�@�� ������nb6turn.gundan �ɒl���������B
				�������� JMP TSMod.OnTSExeGundanTurnChange �Ə��������Ă����ۂ̏���
004670C8  |. EB 11                   JMP     SHORT TENSHOU.004670DB
*/
int pTenshouExeJumpFromToOnTenshouExeGundanTurnChange    =0x4670C2; // �֐��͂��̃A�h���X����AOnTenshouExeGundanTurnChange�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeGundanTurnChange =0x4670C8; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeGundanTurnChange() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		MOV     WORD PTR DS:[0x4C9357], AX

		// �R�c�ԍ���ۑ����Ă����B
		mov iGundanValueOfGundanTurnChange, eax

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
	OnTenshouExeGundanTurnChangeExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeGundanTurnChange
	}
}



char cmdOnTenshouExeJumpFromGundanTurnChange[7]="\xE9\x90\x90\x90\x90\x90";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�U�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeGundanTurnChange() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeGundanTurnChange;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeGundanTurnChange + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGundanTurnChange  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromGundanTurnChange+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeGundanTurnChange), cmdOnTenshouExeJumpFromGundanTurnChange, 6, NULL); //6�o�C�g�̂ݏ�������
}



//----------------------------------------------------------- �R�c�^�[�����N���A�����ꏊ
void OnTenshouExeGundanTurnChangeAfterExecute() {
	// �喼A �R�c1 �R�c2 0xFFFF �喼B �R�c1 0xFFFF �喼C �R�c1 �R�c2 �R�c3 0xFFFF �݂����ȏ����ƂȂ�B
	// int iTmp = (iGundanValueOfGundanTurnChange & 0xFFFF) - 1; // AX���l�Ȃ̂ŁA���ʃr�b�g�������p����B
}

/*
004670DC  /$ 66:C705 57934C00 FFFF              MOV     WORD PTR DS:[4C9357], 0FFFF
*/
/*
004670DC  /$ 66:C705 57934C00 FFFF              MOV     WORD PTR DS:[4C9357], 0FFFF
				�������� JMP TSMod.OnTSExeGundanTurnChangeAfter �Ə��������Ă����ۂ̏���
004670E5  \. C3                                 RETN
*/
int pTenshouExeJumpFromToOnTenshouExeGundanTurnChangeAfter    =0x4670DC; // �֐��͂��̃A�h���X����AOnTenshouExeGundanTurnChangeAfter�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeGundanTurnChangeAfter =0x4670E5; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeGundanTurnChangeAfter() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// ���X�̏�����������
		MOV     WORD PTR DS:[0x4C9357], 0xFFFF   // �R�c�^�[�����N���A

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
	OnTenshouExeGundanTurnChangeAfterExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeGundanTurnChangeAfter
	}
}



char cmdOnTenshouExeJumpFromGundanTurnChangeAfter[10]="\xE9\x90\x90\x90\x90\x90\x90\x90\x90";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�U�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeGundanTurnChangeAfter() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeGundanTurnChangeAfter;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeGundanTurnChangeAfter + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGundanTurnChangeAfter  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromGundanTurnChangeAfter+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeGundanTurnChangeAfter), cmdOnTenshouExeJumpFromGundanTurnChangeAfter, 9, NULL); //9�o�C�g�̂ݏ�������
}
