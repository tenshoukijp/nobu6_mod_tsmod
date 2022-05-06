
#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "TSModCommand.h"


/*
 �������Q�l�ɂȂ������̌M����d���N���̑���B�푈����
*/
/*
00429438  |. 53             PUSH    EBX
00429439  |. 57             PUSH    EDI
0042943A  |. E8 6605FEFF    CALL    TENSHOU.004099A5		EDI���Ώۂ̕���
0042943F  |. 83C4 08        ADD     ESP, 8
*/


int iCurBushouIDInUpdateToRoninOfWarKunkouParam = -1;
int iCurBushouKunkouValueInUpdateToRoninOfWarKunkouParam = -1;

void OnTenshouExeUpdateToRoninOfWarKunkouParamExecuteBefore() { // �O

	int iBushouID = iCurBushouIDInUpdateToRoninOfWarKunkouParam-1;

	// �M�����Z�b�g���Ȃ�
	if ( TSModCommand::AllBushou::cmdNonMeritoriousReset ) {
		// ���������͈̔͂ł��邱��
		if (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {
			if ( nb6bushouname[iBushouID].State == 4 ) { // �Q�l�ɂ��ꂽ��
				iCurBushouKunkouValueInUpdateToRoninOfWarKunkouParam = nb6bushouref[iBushouID].merits; // �M���l����[�ۑ�
			}
		}
	}
}

void OnTenshouExeUpdateToRoninOfWarKunkouParamExecuteAfter() { // ��

	int iBushouID = iCurBushouIDInUpdateToRoninOfWarKunkouParam-1;

	// �M�����Z�b�g���Ȃ�
	if ( TSModCommand::AllBushou::cmdNonMeritoriousReset ) {
		// ���������͈̔͂ł��邱��
		if (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {
			if ( nb6bushouname[iBushouID].State == 4 ) { // �Q�l�ɂ��ꂽ��
				nb6bushouref[iBushouID].merits = (WORD)(iCurBushouKunkouValueInUpdateToRoninOfWarKunkouParam * 0.7); // �M���l�𕜌��B0.65�{
			}
		}
	}
}

/*
00429438  |. 53             PUSH    EBX
00429439  |. 57             PUSH    EDI
0042943A  |. E8 6605FEFF    CALL    TENSHOU.004099A5		EDI���Ώۂ̕���
				�������� JMP TSMod.OnTSExeUpdateToRoninOfWarKunkouParam �Ə��������Ă����ۂ̏���
0042943F  |. 83C4 08        ADD     ESP, 8
*/
int pTenshouExeJumpFromToOnTenshouExeUpdateToRoninOfWarKunkouParam	    =0x42943A; // �֐��͂��̃A�h���X����AOnTenshouExeUpdateToRoninOfWarKunkouParam�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeUpdateToRoninOfWarKunkouParam  =0x4099A5; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeUpdateToRoninOfWarKunkouParam   =0x42943F; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeUpdateToRoninOfWarKunkouParam() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ����ID��ۑ�
		mov iCurBushouIDInUpdateToRoninOfWarKunkouParam, edi

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
	OnTenshouExeUpdateToRoninOfWarKunkouParamExecuteBefore();

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
		call pTenshouExeJumpCallFromToOnTenshouExeUpdateToRoninOfWarKunkouParam

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

	OnTenshouExeUpdateToRoninOfWarKunkouParamExecuteAfter();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeUpdateToRoninOfWarKunkouParam
	}
}

/*
*/
char cmdOnTenshouExeJumpFromUpdateToRoninOfWarKunkouParam[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeUpdateToRoninOfWarKunkouParam() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeUpdateToRoninOfWarKunkouParam;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeUpdateToRoninOfWarKunkouParam + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeUpdateToRoninOfWarKunkouParam  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromUpdateToRoninOfWarKunkouParam+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeUpdateToRoninOfWarKunkouParam), cmdOnTenshouExeJumpFromUpdateToRoninOfWarKunkouParam, 5, NULL); //5�o�C�g�̂ݏ�������
}




