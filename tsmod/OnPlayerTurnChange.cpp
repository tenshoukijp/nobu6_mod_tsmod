#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "FuncSelectBushouDialog.h"

// �N�E�G�߂��ǂ����Ő錾����Ă���B

extern void writeUpdateGameDataStructSettingsEx();

char pre_check_season = 100;	// �P�O�̎��Ƀ`�F�b�N�����V�[�Y���̒l���m�ۂ��Ă����B�V�[�Y���̐؂�ւ��ڂ����ނ��߁B �G�߂�0�`3�Ȃ̂ŁA����ȊO�̐��̒l�ŏ�����

extern int iEnterOfClickMainScreenMainCommand;

void OnTenshouExePlayerTurnChangeExecute() {

	// ���ɂ���̕K�v�͂Ȃ��͂������A�ꉞ�O�̂��߁B
	CommonGlobalHandle::isBattleMode = 0;

	// �v���C���[�^�[�����ɍs���T�[�r�X
	EXSuperService_OnPlayerTurnChange();

	// �����v���C���[�^�[���`�F���W���ɋG�߂�����Ă����Ȃ�΁c
	if ( pre_check_season != nb6year.season ) {
		// �Y���̃V�[�Y����ۑ����Ă����āc
		pre_check_season = nb6year.season;

		// �^�[���؂�ւ�� ���� �V�[�Y�����؂�ւ��̃^�C�~���O�̏������s��
		EXSuperService_OnSeasonChangeAndPlayerTurnChange();

		// ���[�h�����̋G�ߌo�ߐ���1���Z
		nb6progressseason++;

		iEnterOfClickMainScreenMainCommand = 0;
	}
}


/*
00410467  |. 66:8BF0        MOV     SI, AX
0041046A  |. BF FFFF0000    MOV     EDI, 0FFFF
				�������� JMP TSMod.OnTSExePlayerTurnChange �Ə��������Ă����ۂ̏���
0041046F  |. E8 71CA0900    CALL    TENSHOU.004ACEE5
*/
int pTenshouExeJumpFromToOnTenshouExePlayerTurnChange    =0x41046A; // �֐��͂��̃A�h���X����AOnTenshouExePlayerTurnChange�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExePlayerTurnChange =0x41046F; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExePlayerTurnChange() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		mov edi, 0xFFFF

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
	OnTenshouExePlayerTurnChangeExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExePlayerTurnChange
	}
}

/*
00410467  |. 66:8BF0        MOV     SI, AX
0041046A  |. BF FFFF0000    MOV     EDI, 0FFFF
				�������� JMP TSMod.OnTSExePlayerTurnChange �Ə��������Ă����ۂ̏���
0041046F  |. E8 71CA0900    CALL    TENSHOU.004ACEE5
*/
char cmdOnTenshouExeJumpFromPlayerTurnChange[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExePlayerTurnChange() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExePlayerTurnChange;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExePlayerTurnChange + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExePlayerTurnChange  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromPlayerTurnChange+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExePlayerTurnChange), cmdOnTenshouExeJumpFromPlayerTurnChange, 5, NULL); //5�o�C�g�̂ݏ�������
}




