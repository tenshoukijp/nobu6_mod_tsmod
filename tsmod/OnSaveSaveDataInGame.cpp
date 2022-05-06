
#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "SetBushouAvgParam.h"

/*
���C���Q�[�����ŃZ�[�u����
*/
/*
00479E61  |. E8 ACFDFFFF    CALL    TENSHOU.00479C12 �� ���̊֐��̌���EAX���P��������Z�[�u�����B
00479E66  |. 83C4 04        ADD     ESP, 4
00479E69  |. 33C0           XOR     EAX, EAX			�� ����EAX���P��������Z�[�u����
00479E6B  \. C3             RETN
*/

//-------------------------------------------------------------------------------------
// �Z�[�u���[�h�_�C�A���O�őI�����Ă��鎞�����g����B
extern int getSelectSaveOrLoadNo();

int isSaveExecuteOfSaveSaveDataInGame = 0;
void OnTenshouExeSaveSaveDataInGameExecute() {
	if ( isSaveExecuteOfSaveSaveDataInGame ) {
		// ���������ɁA�Z�[�u�������̊g���f�[�^����
		{
	
			int iSelectID = getSelectSaveOrLoadNo();

			// �G���[�ł͂Ȃ��B
			if ( iSelectID != -1 ) {
				// OutputDebugStream << iSelectID << psznb6savedatafilename << endl;
				SaveSaveDataEx(iSelectID );
			}
		}
	}
}


/*
00479E61  |. E8 ACFDFFFF    CALL    TENSHOU.00479C12 �� ���̊֐��̌���EAX���P��������Z�[�u�����B
				�������� JMP TSMod.OnTSExeSaveSaveDataInGame �Ə��������Ă����ۂ̏���
00479E66  |. 83C4 04        ADD     ESP, 4
00479E69  |. 33C0           XOR     EAX, EAX			�� ����EAX���P��������Z�[�u����
00479E6B  \. C3             RETN
*/
int pTenshouExeJumpFromToOnTenshouExeSaveSaveDataInGame		=0x479E61; // �֐��͂��̃A�h���X����AOnTenshouExeSaveSaveDataInGame�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeSaveSaveDataInGame =0x479C12; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeSaveSaveDataInGame	=0x479E66; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeSaveSaveDataInGame() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeSaveSaveDataInGame

		mov isSaveExecuteOfSaveSaveDataInGame, EAX // �����ƃZ�[�u�����Ȃ�1���A�L�����Z�����ŃZ�[�u���ĂȂ��Ȃ�0������B

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
	OnTenshouExeSaveSaveDataInGameExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeSaveSaveDataInGame
	}
}

char cmdOnTenshouExeJumpFromSaveSaveDataInGame[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeSaveSaveDataInGame() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeSaveSaveDataInGame;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSaveSaveDataInGame + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeSaveSaveDataInGame  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromSaveSaveDataInGame+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSaveSaveDataInGame), cmdOnTenshouExeJumpFromSaveSaveDataInGame, 5, NULL); //5�o�C�g�̂ݏ�������
}




