#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

#include "ScenarioMod.h"


// �ߗ������O�Ɏ��s�����֐�
void OnTenshouExeOnPostBattleTreatment() {

	// �����ʂ��I��������A�ߗ��������n�܂�O
	if (p_snOnBattleScreenEnd) {
		p_snOnBattleScreenEnd();
	}
}

/*
0042A29B   |. E8 26F2FFFF    CALL    TENSHOU.004294C6
0042A2A0   |. E8 79F3FFFF    CALL    TENSHOU.0042961E
0042A2A5   |. E8 79FDFFFF    CALL    TENSHOU.0042A023	�� ���̊֐����ߗ������̃^�C�~���O
0042A2AA   |. E8 D8FDFFFF    CALL    TENSHOU.0042A087
0042A2AF   |. E8 4CFEFFFF    CALL    TENSHOU.0042A100
0042A2B4   |. E8 E1FEFFFF    CALL    TENSHOU.0042A19A
0042A2B9   |. E8 ACFFFFFF    CALL    TENSHOU.0042A26A
*/


BOOL PostBattleTreatmentPeriod = FALSE; // �ߗ������̃^�C�~���O

/*
0042A2A0   |. E8 79F3FFFF    CALL    TENSHOU.0042961E
0042A2A5   |. E8 79FDFFFF    CALL    TENSHOU.0042A023	�� ���̊֐����ߗ������̃^�C�~���O
				�������� JMP TSMod.OnTSExePostBattleTreatment �Ə��������Ă����ۂ̏���
0042A2AA   |. E8 D8FDFFFF    CALL    TENSHOU.0042A087
*/
int pTenshouExeJumpFromToOnTenshouExePostBattleTreatment	 =0x42A2A5; // �֐��͂��̃A�h���X����AOnTenshouExePostBattleTreatment�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExePostBattleTreatment =0x42A023; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExePostBattleTreatment  =0x42A2AA; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExePostBattleTreatment() {
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

	OnTenshouExeOnPostBattleTreatment();

	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		mov PostBattleTreatmentPeriod, 1	// �ߗ��������[�h�X�^�[�g
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExePostBattleTreatment

		mov PostBattleTreatmentPeriod, 0	// �ߗ��������[�h�I��

		jmp pTenshouExeReturnLblFromOnTenshouExePostBattleTreatment
	}
}

char cmdOnTenshouExeJumpFromPostBattleTreatment[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExePostBattleTreatment() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExePostBattleTreatment;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExePostBattleTreatment + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExePostBattleTreatment  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromPostBattleTreatment+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExePostBattleTreatment), cmdOnTenshouExeJumpFromPostBattleTreatment, 5, NULL); //5�o�C�g�̂ݏ�������
}




