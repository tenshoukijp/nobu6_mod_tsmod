
#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "TSModCommand.h"


/*
 �������Q�l�ɂȂ������̌M����d���N���̑���
*/
/*
0040B421  |. 56             PUSH    ESI
0040B422  |. 57             PUSH    EDI
0040B423  |. E8 7DE5FFFF    CALL    TENSHOU.004099A5		EDI���Ώۂ̕���
0040B428  |. 83C4 08        ADD     ESP, 8
*/


int iCurBushouIDInUpdateToRoninKunkouParam = -1;
int iCurBushouKunkouValueInUpdateToRoninKunkouParam = -1;

void OnTenshouExeUpdateToRoninKunkouParamExecuteBefore() { // �O

	int iBushouID = iCurBushouIDInUpdateToRoninKunkouParam-1;

	// �M�����Z�b�g���Ȃ�
	if ( TSModCommand::AllBushou::cmdNonMeritoriousReset ) {

		// ���������͈̔͂ł��邱��
		if (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {
			if ( nb6bushouname[iBushouID].State == 4 ) { // �Q�l�ɂ��ꂽ��
				iCurBushouKunkouValueInUpdateToRoninKunkouParam = nb6bushouref[iBushouID].merits; // �M���l����[�ۑ�
			}
		}
	}
}

void OnTenshouExeUpdateToRoninKunkouParamExecuteAfter() { // ��

	int iBushouID = iCurBushouIDInUpdateToRoninKunkouParam-1;

	// �M�����Z�b�g���Ȃ�
	if ( TSModCommand::AllBushou::cmdNonMeritoriousReset ) {

		// ���������͈̔͂ł��邱��
		if (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {
			if ( nb6bushouname[iBushouID].State == 4 ) { // �Q�l�ɂ��ꂽ��
				nb6bushouref[iBushouID].merits = (WORD)(iCurBushouKunkouValueInUpdateToRoninKunkouParam * 0.7); // �M���l�𕜌��B0.7�{
			}
		}
	}
}

/*
0040B421  |. 56             PUSH    ESI
0040B422  |. 57             PUSH    EDI
0040B423  |. E8 7DE5FFFF    CALL    TENSHOU.004099A5
				�������� JMP TSMod.OnTSExeUpdateToRoninKunkouParam �Ə��������Ă����ۂ̏���
0040B428  |. 83C4 08        ADD     ESP, 8
*/
int pTenshouExeJumpFromToOnTenshouExeUpdateToRoninKunkouParam	   =0x40B423; // �֐��͂��̃A�h���X����AOnTenshouExeUpdateToRoninKunkouParam�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeUpdateToRoninKunkouParam  =0x4099A5; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeUpdateToRoninKunkouParam   =0x40B428; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeUpdateToRoninKunkouParam() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ����ID��ۑ�
		mov iCurBushouIDInUpdateToRoninKunkouParam, edi

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
	OnTenshouExeUpdateToRoninKunkouParamExecuteBefore();

	__asm {
		// �X�^�b�N�����ɖ߂��Ď��s
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeUpdateToRoninKunkouParam

		// �ĂуX�^�b�N�ɂ��߂�
		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnTenshouExeUpdateToRoninKunkouParamExecuteAfter();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeUpdateToRoninKunkouParam
	}
}

/*
*/
char cmdOnTenshouExeJumpFromUpdateToRoninKunkouParam[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeUpdateToRoninKunkouParam() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeUpdateToRoninKunkouParam;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeUpdateToRoninKunkouParam + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeUpdateToRoninKunkouParam  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromUpdateToRoninKunkouParam+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeUpdateToRoninKunkouParam), cmdOnTenshouExeJumpFromUpdateToRoninKunkouParam, 5, NULL); //5�o�C�g�̂ݏ�������
}




