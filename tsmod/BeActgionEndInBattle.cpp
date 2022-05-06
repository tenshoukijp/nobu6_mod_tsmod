#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"

/*
�@�ȉ��̍s���ς݂Ƃ���ۂɒʉ߂��Ă���֐��B
  ����OnActionEndInBattle���D�G�Ȃ̂Ŗ����p
*/

/*
0042DF77  /$ FF7424 08      PUSH    DWORD PTR SS:[ESP+8]             ; /Arg3
0042DF7B  |. 6A 20          PUSH    20                               ; |Arg2 = 00000020
0042DF7D  |. FF7424 0C      PUSH    DWORD PTR SS:[ESP+C]             ; |Arg1
0042DF81  |. E8 04000000    CALL    TENSHOU.0042DF8A                 ; \TENSHOU.0042DF8A
0042DF86  |. 83C4 0C        ADD     ESP, 0C
0042DF89  \. C3             RETN
*/

int iCurBushouIDOnBeActionEndInBattle = -1; // TENSHOU.EXE����n���Ă���Ώۂ̕���

// �e�X�̕����́A����̃^�[���ł̎c��s����
extern int arrRemainMoveCountAllBushou[GAMEDATASTRUCT_BUSHOU_NUM];

void OnTenshouExeBeActionEndInBattleExecute() {
	int iBushouID = iCurBushouIDOnBeActionEndInBattle-1;
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
	}
}


/*
/*
0042DF77  /$ FF7424 08      PUSH    DWORD PTR SS:[ESP+8]             ; /Arg3
0042DF7B  |. 6A 20          PUSH    20                               ; |Arg2 = 00000020
0042DF7D  |. FF7424 0C      PUSH    DWORD PTR SS:[ESP+C]             ; |Arg1
0042DF81  |. E8 04000000    CALL    TENSHOU.0042DF8A                 ; \TENSHOU.0042DF8A
				�������� JMP TSMod.OnTSExeBeActionEndInBattle �Ə��������Ă����ۂ̏���
0042DF86  |. 83C4 0C        ADD     ESP, 0C
0042DF89  \. C3             RETN
*/
int pTenshouExeJumpFromToOnTenshouExeBeActionEndInBattle	 =0x42DF81; // �֐��͂��̃A�h���X����AOnTenshouExeBeActionEndInBattle�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeBeActionEndInBattle =0x42DF8A; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeBeActionEndInBattle	 =0x42DF86; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeBeActionEndInBattle() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ������ID��ۑ�
		mov iCurBushouIDOnBeActionEndInBattle, esi

		// ���XTENSHOU.EXE���ɂ��������̂������ŁB���̒��ŁA�s����(=0x20)�ւƐݒ肳���
		call pTenshouExeJumpCallFromToOnTenshouExeBeActionEndInBattle

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
	OnTenshouExeBeActionEndInBattleExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeBeActionEndInBattle
	}
}

/*
0049D7AA  |. 6A 03          |PUSH    3                               ; |Arg3 = 00000003
0049D7AC  |. 56             |PUSH    ESI                             ; |Arg2
0049D7AD  |. 56             |PUSH    ESI                             ; |Arg1
0049D7AE  |. E8 D3540000    |CALL    TENSHOU.004A2C86                ; \TENSHOU.004A2C86
				�������� JMP TSMod.OnTSExeBeActionEndInBattle �Ə��������Ă����ۂ̏���
0049D7B3  |. 83C4 1C        |ADD     ESP, 1C
0049D7B6  |. 85C0           |TEST    EAX, EAX
*/
char cmdOnTenshouExeJumpFromBeActionEndInBattle[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeBeActionEndInBattle() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeBeActionEndInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeBeActionEndInBattle + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeBeActionEndInBattle  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromBeActionEndInBattle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeBeActionEndInBattle), cmdOnTenshouExeJumpFromBeActionEndInBattle, 5, NULL); //5�o�C�g�̂ݏ�������
}




