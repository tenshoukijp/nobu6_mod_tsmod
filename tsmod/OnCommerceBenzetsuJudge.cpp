
#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


/*
 * ���Ǝ��ɕِ�������ǂ������W���b�W���A���l�Ȃ�΁A�ِ�������Ȃ��Ă��A�ِ�������ƂƓ����Ƃ���B
 */



int iCurBushouIDInCommerceBenzetsuJudge = -1;
int bBenzetsuJudgeInCommerceBenzetsuJudge = 0;
void OnTenshouExeCommerceBenzetsuJudgeExecute() {

	// ���������͈̔͂ł��邱��
	if (0 <= iCurBushouIDInCommerceBenzetsuJudge-1 && iCurBushouIDInCommerceBenzetsuJudge-1 < GAMEDATASTRUCT_BUSHOU_NUM) {

		// ���l�ł���΁c �ِ�������Ă��Ȃ��Ă������Ă��邱�ƂƂȂ�B
		if ( nb6bushouref[iCurBushouIDInCommerceBenzetsuJudge-1].job == 0x70 ) {
			bBenzetsuJudgeInCommerceBenzetsuJudge = 1;
		}
	}
}


/*
00446A7B  |. 57             PUSH    EDI
00446A7C  |. E8 3159FCFF    CALL    TENSHOU.0040C3B2
				�������� JMP TSMod.OnTSExeCommerceBenzetsuJudge �Ə��������Ă����ۂ̏���
00446A81  |. 83C4 04        ADD     ESP, 4
00446A84  |. 85C0           TEST    EAX, EAX
00446A86  |. 74 45          JE      SHORT TENSHOU.00446ACD
*/
int pTenshouExeJumpFromToOnTenshouExeCommerceBenzetsuJudge		=0x446A7C; // �֐��͂��̃A�h���X����AOnTenshouExeCommerceBenzetsuJudge�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeCommerceBenzetsuJudge  =0x40C3B2; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeCommerceBenzetsuJudge	=0x446A81; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeCommerceBenzetsuJudge() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeCommerceBenzetsuJudge

		// ����ID��ۑ�
		mov iCurBushouIDInCommerceBenzetsuJudge, edi

		// �ِ�������Ă��邩�ǂ�����ۑ�
		mov bBenzetsuJudgeInCommerceBenzetsuJudge, eax

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
	OnTenshouExeCommerceBenzetsuJudgeExecute();

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

		// ���l���ۂ��̌��ʂ������āA�ِ�����㏑��
		mov eax, bBenzetsuJudgeInCommerceBenzetsuJudge 

		jmp pTenshouExeReturnLblFromOnTenshouExeCommerceBenzetsuJudge
	}
}

/*
00446A7B  |. 57             PUSH    EDI
00446A7C  |. E8 3159FCFF    CALL    TENSHOU.0040C3B2
				�������� JMP TSMod.OnTSExeCommerceBenzetsuJudge �Ə��������Ă����ۂ̏���
00446A81  |. 83C4 04        ADD     ESP, 4
00446A84  |. 85C0           TEST    EAX, EAX
00446A86  |. 74 45          JE      SHORT TENSHOU.00446ACD
*/
char cmdOnTenshouExeJumpFromCommerceBenzetsuJudge[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeCommerceBenzetsuJudge() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeCommerceBenzetsuJudge;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeCommerceBenzetsuJudge + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeCommerceBenzetsuJudge  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromCommerceBenzetsuJudge+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeCommerceBenzetsuJudge), cmdOnTenshouExeJumpFromCommerceBenzetsuJudge, 5, NULL); //5�o�C�g�̂ݏ�������
}




