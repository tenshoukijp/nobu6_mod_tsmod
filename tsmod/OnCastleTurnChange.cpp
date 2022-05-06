#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"

/*
 ���C����ʒ��̏�^�[�����؂�ւ�������B
 �P�R�c�^�[�����ɓ����邪���񂩗��邱�Ƃ�����
 */


/*
004670FB  |. 77 08          JA      SHORT TENSHOU.00467105
004670FD  |. 66:A3 59934C00 MOV     WORD PTR DS:[4C9359], AX	�� ������nb6turn.castle �ɒl���������B
00467103  |. EB 11          JMP     SHORT TENSHOU.00467116
*/

int iCastleValueOfCastleTurnChange = -1;
void OnTenshouExeCastleTurnChangeExecute() {
	iCastleValueOfCastleTurnChange = (iCastleValueOfCastleTurnChange & 0xFFFF) - 1; // AX���l�Ȃ̂ŁA���ʃr�b�g�������p����B

	if ( 0 <= iCastleValueOfCastleTurnChange && iCastleValueOfCastleTurnChange < GAMEDATASTRUCT_CASTLE_NUM ) {
		// ��̐؂�ւ��ڂ̏���
		// ���������������鎞�̓R�R
	}

}


/*
004670FB  |. 77 08          JA      SHORT TENSHOU.00467105
004670FD  |. 66:A3 59934C00 MOV     WORD PTR DS:[4C9359], AX	�� ������nb6turn.castle �ɒl���������B
				�������� JMP TSMod.OnTSExeCastleTurnChange �Ə��������Ă����ۂ̏���
00467103  |. EB 11          JMP     SHORT TENSHOU.00467116
*/
int pTenshouExeJumpFromToOnTenshouExeCastleTurnChange    =0x4670FD; // �֐��͂��̃A�h���X����AOnTenshouExeCastleTurnChange�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeCastleTurnChange =0x467103; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeCastleTurnChange() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		MOV     WORD PTR DS:[0x4C9359], AX

		// �R�c�ԍ���ۑ����Ă����B
		mov iCastleValueOfCastleTurnChange, eax

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
	OnTenshouExeCastleTurnChangeExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeCastleTurnChange
	}
}



char cmdOnTenshouExeJumpFromCastleTurnChange[7]="\xE9\x90\x90\x90\x90\x90";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�U�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeCastleTurnChange() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeCastleTurnChange;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeCastleTurnChange + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeCastleTurnChange  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromCastleTurnChange+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeCastleTurnChange), cmdOnTenshouExeJumpFromCastleTurnChange, 6, NULL); //6�o�C�g�̂ݏ�������
}




