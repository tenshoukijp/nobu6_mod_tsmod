
#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "TSModCommand.h"


/*
 ���g���n�̎��A�M�����̑���
*/
/*
00442911  |> 50             PUSH    EAX
00442912  |. 56             PUSH    ESI
00442913  |. E8 8D70FCFF    CALL    TENSHOU.004099A5		ESI���Ώە���
00442918  |. 83C4 08        ADD     ESP, 8
*/


int iCurBushouIDInUpdateToHataageKunkouParam = -1;
int iCurBushouKunkouValueInUpdateToHataageKunkouParam = -1;

void OnTenshouExeUpdateToHataageKunkouParamExecuteBefore() { // �O

	int iBushouID = iCurBushouIDInUpdateToHataageKunkouParam-1;

	// �M�����Z�b�g���Ȃ�
	if ( TSModCommand::AllBushou::cmdNonMeritoriousReset ) {
		// ���ɂȂ�
	}
}

void OnTenshouExeUpdateToHataageKunkouParamExecuteAfter() { // ��

	int iBushouID = iCurBushouIDInUpdateToHataageKunkouParam-1;

	// �M�����Z�b�g���Ȃ�
	if ( TSModCommand::AllBushou::cmdNonMeritoriousReset ) {
		// ���ɂȂ�
	}
}

/*
00442911  |> 50             PUSH    EAX
00442912  |. 56             PUSH    ESI
00442913  |. E8 8D70FCFF    CALL    TENSHOU.004099A5		ESI���Ώە���
				�������� JMP TSMod.OnTSExeUpdateToHataageKunkouParam �Ə��������Ă����ۂ̏���
00442918  |. 83C4 08        ADD     ESP, 8
*/
int pTenshouExeJumpFromToOnTenshouExeUpdateToHataageKunkouParam	     =0x442913; // �֐��͂��̃A�h���X����AOnTenshouExeUpdateToHataageKunkouParam�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeUpdateToHataageKunkouParam  =0x4099A5; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeUpdateToHataageKunkouParam   =0x442918; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeUpdateToHataageKunkouParam() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ����ID��ۑ�
		mov iCurBushouIDInUpdateToHataageKunkouParam, esi

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
	OnTenshouExeUpdateToHataageKunkouParamExecuteBefore();

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
		call pTenshouExeJumpCallFromToOnTenshouExeUpdateToHataageKunkouParam

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

	OnTenshouExeUpdateToHataageKunkouParamExecuteAfter();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeUpdateToHataageKunkouParam
	}
}

/*
*/
char cmdOnTenshouExeJumpFromUpdateToHataageKunkouParam[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeUpdateToHataageKunkouParam() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeUpdateToHataageKunkouParam;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeUpdateToHataageKunkouParam + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeUpdateToHataageKunkouParam  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromUpdateToHataageKunkouParam+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeUpdateToHataageKunkouParam), cmdOnTenshouExeJumpFromUpdateToHataageKunkouParam, 5, NULL); //5�o�C�g�̂ݏ�������
}




