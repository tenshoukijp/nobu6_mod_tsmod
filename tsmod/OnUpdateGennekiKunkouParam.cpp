
#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "TSModCommand.h"


/*
 �������Q�l���猻���ɂȂ�����A��㏈�����ŁA�ߗ��o�ꎞ�ɁA�M�����̑���
*/
/*
0040BC4A  |. 50             PUSH    EAX
0040BC4B  |> 57             PUSH    EDI
0040BC4C  |. E8 54DDFFFF    CALL    TENSHOU.004099A5	EDI���Ώۂ̕���
0040BC51  |. 83C4 08        ADD     ESP, 8
*/


int iCurBushouIDInUpdateToGennekiKunkouParam = -1;
int iCurBushouKunkouValueInUpdateToGennekiKunkouParam = -1;

void OnTenshouExeUpdateToGennekiKunkouParamExecuteBefore() { // �O

	int iBushouID = iCurBushouIDInUpdateToGennekiKunkouParam-1;

	// �M�����Z�b�g���Ȃ�
	if ( TSModCommand::AllBushou::cmdNonMeritoriousReset ) {
		// ���������͈̔͂ł��邱��
		if (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {
			if ( nb6bushouname[iBushouID].State == 4 ) { // �Q�l����̓o�p�̎�
				iCurBushouKunkouValueInUpdateToGennekiKunkouParam = nb6bushouref[iBushouID].merits; // �M���l����[�ۑ�
			}
		}
	}
}

void OnTenshouExeUpdateToGennekiKunkouParamExecuteAfter() { // ��

	int iBushouID = iCurBushouIDInUpdateToGennekiKunkouParam-1;

	// �M�����Z�b�g���Ȃ�
	if ( TSModCommand::AllBushou::cmdNonMeritoriousReset ) {
		// ���������͈̔͂ł��邱��
		if (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {
			if ( nb6bushouname[iBushouID].State == 4 ) { // �Q�l����̓o�p�̎�
				nb6bushouref[iBushouID].merits = iCurBushouKunkouValueInUpdateToGennekiKunkouParam; // �M���l�𕜌��B

				// �����y�Ȃ�΁c �M���ɉ����āA���i������B
				if ( nb6bushouname[iBushouID].position == 6 ) { // �����y�Ȃ�A���i�����Ă����v

					/*
					#define GAMEDATASTRUCT_REQUIREDMERITS_NUM		5
					// [�h�V�E�ƘV�E�����E���叫�E���y��]�̂T��ނ���`����Ă���B
					nb6requiredmerits[i] 
					//�g��		(�B��=0, �喼=1, �h�V=2, �ƘV=3, ����=4, ���叫=5, ���y��=6)
					*/
					if ( nb6bushouref[iBushouID].merits >= nb6requiredmerits[0].merits ) { // �h�V���M�������B
						nb6bushouname[iBushouID].position = 2; // �h�V��
					} else if ( nb6bushouref[iBushouID].merits >= nb6requiredmerits[1].merits ) { // �ƘV���M�������B
						nb6bushouname[iBushouID].position = 3; // �ƘV��
					} else if ( nb6bushouref[iBushouID].merits >= nb6requiredmerits[2].merits ) { // �������M�������B
						nb6bushouname[iBushouID].position = 4; // ������
					} else if ( nb6bushouref[iBushouID].merits >= nb6requiredmerits[3].merits ) { // ���叫���M�������B
						nb6bushouname[iBushouID].position = 5; // ���叫��
					}
				}
			}
		}
	}
}

/*
0040BC4A  |. 50             PUSH    EAX
0040BC4B  |> 57             PUSH    EDI
0040BC4C  |. E8 54DDFFFF    CALL    TENSHOU.004099A5	EDI���Ώۂ̕���
				�������� JMP TSMod.OnTSExeUpdateToGennekiKunkouParam �Ə��������Ă����ۂ̏���
0040BC51  |. 83C4 08        ADD     ESP, 8
*/
int pTenshouExeJumpFromToOnTenshouExeUpdateToGennekiKunkouParam	     =0x40BC4C; // �֐��͂��̃A�h���X����AOnTenshouExeUpdateToGennekiKunkouParam�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeUpdateToGennekiKunkouParam  =0x4099A5; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeUpdateToGennekiKunkouParam   =0x40BC51; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeUpdateToGennekiKunkouParam() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ����ID��ۑ�
		mov iCurBushouIDInUpdateToGennekiKunkouParam, edi

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
	OnTenshouExeUpdateToGennekiKunkouParamExecuteBefore();

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
		call pTenshouExeJumpCallFromToOnTenshouExeUpdateToGennekiKunkouParam

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

	OnTenshouExeUpdateToGennekiKunkouParamExecuteAfter();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeUpdateToGennekiKunkouParam
	}
}

/*
*/
char cmdOnTenshouExeJumpFromUpdateToGennekiKunkouParam[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeUpdateToGennekiKunkouParam() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeUpdateToGennekiKunkouParam;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeUpdateToGennekiKunkouParam + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeUpdateToGennekiKunkouParam  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromUpdateToGennekiKunkouParam+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeUpdateToGennekiKunkouParam), cmdOnTenshouExeJumpFromUpdateToGennekiKunkouParam, 5, NULL); //5�o�C�g�̂ݏ�������
}




