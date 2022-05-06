#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "SetBushouAvgParam.h"

/*
���C���Q�[�����Ń��[�h���肵��
*/
/*
00479E97   . E8 76FDFFFF    CALL    TENSHOU.00479C12�� ���̊֐��̌���EAX���P��������Q�[�����Ń��[�h�����B
00479E9C   . 83C4 04        ADD     ESP, 4
00479E9F   . B9 18C24D00    MOV     ECX, TENSHOU.004DC218
00479EA4   . 85C0           TEST    EAX, EAX		�� ����EAX���P�������烍�[�h����
00479EA6   . 74 2C          JE      SHORT TENSHOU.00479ED4
00479EA8   . E8 3CC3FEFF    CALL    TENSHOU.004661E9 �� ���[�h����΂��Ȃ炸�ŏ��ɂ��̊֐����s
*/

//-------------------------------------------------------------------------------------

// �N�E�G�߂��ǂ����Ő錾����Ă���B
extern int nb6progressseason;

// �P�O�̋G�߂̏��
extern char pre_check_season;

// �Z�[�u���[�h�_�C�A���O�őI�����Ă��鎞�����g����B
extern int getSelectSaveOrLoadNo();

void OnTenshouExeLoadSaveDataInGameExecute() {
	nb6progressseason = 1L;

	// �P�O�̋G�߂����蓾�Ȃ����l�ɂ��Ă���
	pre_check_season = 100;

	// ���������ɁA���[�h�������̊g���f�[�^����
	{
		int iSelectID = getSelectSaveOrLoadNo();

		// �G���[�ł͂Ȃ��B
		if ( iSelectID != -1 ) {
			// OutputDebugStream << iSelectID << psznb6savedatafilename << endl;
			LoadSaveDataEx(iSelectID);
		}
	}
}


/*
00479E97   . E8 76FDFFFF    CALL    TENSHOU.00479C12�� ���̊֐��̌���EAX���P��������I�[�v�j���O�Ń��[�h�����B
00479E9C   . 83C4 04        ADD     ESP, 4
00479E9F   . B9 18C24D00    MOV     ECX, TENSHOU.004DC218
00479EA4   . 85C0           TEST    EAX, EAX		�� ����EAX���P�������烍�[�h����
00479EA6   . 74 2C          JE      SHORT TENSHOU.00479ED4
00479EA8   . E8 3CC3FEFF    CALL    TENSHOU.004661E9
				�������� JMP TSMod.OnTSExeLoadSaveDataInGame �Ə��������Ă����ۂ̏���
00479EAD   . 68 00020000    PUSH    200
*/
int pTenshouExeJumpFromToOnTenshouExeLoadSaveDataInGame		=0x479EA8; // �֐��͂��̃A�h���X����AOnTenshouExeLoadSaveDataInGame�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeLoadSaveDataInGame =0x4661E9; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeLoadSaveDataInGame	=0x479EAD; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeLoadSaveDataInGame() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeLoadSaveDataInGame

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// �Q�[������ʂŃ��[�h���ɉ�������Ȃ�΁c
	OnTenshouExeLoadSaveDataInGameExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeLoadSaveDataInGame
	}
}

char cmdOnTenshouExeJumpFromLoadSaveDataInGame[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeLoadSaveDataInGame() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeLoadSaveDataInGame;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeLoadSaveDataInGame + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeLoadSaveDataInGame  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromLoadSaveDataInGame+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeLoadSaveDataInGame), cmdOnTenshouExeJumpFromLoadSaveDataInGame, 5, NULL); //5�o�C�g�̂ݏ�������
}




