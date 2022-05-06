#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "SetBushouAvgParam.h"

/*
�I�[�v�j���O�Ń��[�h���肵���B
*/
/*
0049D84E  |. E8 BFC3FDFF    |CALL    TENSHOU.00479C12�� ���̊֐��̌���EAX���P��������I�[�v�j���O�Ń��[�h�����B
0049D853  |. 83C4 04        |ADD     ESP, 4
0049D856  |. 85C0           |TEST    EAX, EAX			�� ����EAX���P�������烍�[�h����
0049D858  |.^0F84 40FFFFFF  |JE      TENSHOU.0049D79E
0049D85E  |. 68 00020000    |PUSH    200
0049D863  |. E8 4B97FCFF    |CALL    TENSHOU.00466FB3
0049D863  |. E8 4B97FCFF    |CALL    TENSHOU.00466FB3
0049D868  |. 83C4 04        |ADD     ESP, 4
*/

//-------------------------------------------------------------------------------------

// �N�E�G�߂��ǂ����Ő錾����Ă���B
extern int nb6progressseason;

// �P�O�̋G�߂̏��
extern char pre_check_season;

extern int getSelectSaveOrLoadNo();

void OnTenshouExeLoadSaveDataInOpeningExecute() {
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
0049D84E  |. E8 BFC3FDFF    |CALL    TENSHOU.00479C12�� ���̊֐��̌���EAX���P��������I�[�v�j���O�Ń��[�h�����B
0049D853  |. 83C4 04        |ADD     ESP, 4
0049D856  |. 85C0           |TEST    EAX, EAX			�� ����EAX���P�������烍�[�h����
0049D858  |.^0F84 40FFFFFF  |JE      TENSHOU.0049D79E
0049D85E  |. 68 00020000    |PUSH    200
0049D863  |. E8 4B97FCFF    |CALL    TENSHOU.00466FB3
				�������� JMP TSMod.OnTSExeLoadSaveDataInOpening �Ə��������Ă����ۂ̏���
0049D868  |. 83C4 04        |ADD     ESP, 4
*/
int pTenshouExeJumpFromToOnTenshouExeLoadSaveDataInOpening		=0x49D863; // �֐��͂��̃A�h���X����AOnTenshouExeLoadSaveDataInOpening�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeLoadSaveDataInOpening = 0x466FB3; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeLoadSaveDataInOpening	=0x49D868; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeLoadSaveDataInOpening() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeLoadSaveDataInOpening

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
	OnTenshouExeLoadSaveDataInOpeningExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeLoadSaveDataInOpening
	}
}

char cmdOnTenshouExeJumpFromLoadSaveDataInOpening[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeLoadSaveDataInOpening() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeLoadSaveDataInOpening;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeLoadSaveDataInOpening + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeLoadSaveDataInOpening  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromLoadSaveDataInOpening+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeLoadSaveDataInOpening), cmdOnTenshouExeJumpFromLoadSaveDataInOpening, 5, NULL); //5�o�C�g�̂ݏ�������
}




