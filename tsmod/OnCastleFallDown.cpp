#include "WinTarget.h"
#include <time.h>


#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "FuncPlayResourceWav.h"

void OnTenshouExeCastleFallDownExecute() {

	// �R��ɂP�񂮂炢�����炷
	if ( rand() % 3 == 0 ) {

		// ����炷�B	714	����騁i�{�ې苒�j 
		FuncPlaySoundWav(714);
	}
}


/*
004487DB   . 57             PUSH    EDI
004487DC   . 57             PUSH    EDI
004487DD   . 57             PUSH    EDI
004487DE   . 6A 0A          PUSH    0A
004487E0   > E8 044C0400    CALL    TENSHOU.0048D3E9
				�������� JMP TSMod.OnTSExeCastleFallDown �Ə��������Ă����ۂ̏���
004487E5   . 83C4 10        ADD     ESP, 10
004487E8   . EB 0B          JMP     SHORT TENSHOU.004487F5
*/
int pTenshouExeJumpFromToOnTenshouExeCastleFallDown		=0x4487E0; // �֐��͂��̃A�h���X����AOnTenshouExeCastleFallDown�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeCastleFallDown =0x48D3E9; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeCastleFallDown	=0x4487E5; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeCastleFallDown() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeCastleFallDown

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
	OnTenshouExeCastleFallDownExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeCastleFallDown
	}
}

/*
004487DB   . 57             PUSH    EDI
004487DC   . 57             PUSH    EDI
004487DD   . 57             PUSH    EDI
004487DE   . 6A 0A          PUSH    0A
004487E0   > E8 044C0400    CALL    TENSHOU.0048D3E9
				�������� JMP TSMod.OnTSExeCastleFallDown �Ə��������Ă����ۂ̏���
004487E5   . 83C4 10        ADD     ESP, 10
004487E8   . EB 0B          JMP     SHORT TENSHOU.004487F5
*/
char cmdOnTenshouExeJumpFromCastleFallDown[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeCastleFallDown() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeCastleFallDown;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeCastleFallDown + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeCastleFallDown  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromCastleFallDown+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeCastleFallDown), cmdOnTenshouExeJumpFromCastleFallDown, 5, NULL); //5�o�C�g�̂ݏ�������
}




