#include "WinTarget.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"
#include "EXBushou_All.h"
#include "ScenarioMod.h"


/*
 �喼�̌�p�ҕ����h�c���K�v�Ƃ��ꂽ��
 */


/*

0040BAB8  |. E8 9A080500    CALL    TENSHOU.0045C357

0045C357  /$ 56             PUSH    ESI
0045C358  |. 57             PUSH    EDI
0045C359  |. 66:8B7424 0C   MOV     SI, WORD PTR SS:[ESP+C]
0045C35E  |. 56             PUSH    ESI
0045C35F  |. E8 768C0400    CALL    TENSHOU.004A4FDA
0045C364  |. 66:A3 907C4D00 MOV     WORD PTR DS:[4D7C90], AX
0045C36A  |. 83C4 04        ADD     ESP, 4
0045C36D  |. 56             PUSH    ESI
0045C36E  |. E8 FFF4FFFF    CALL    TENSHOU.0045B872
0045C373  |. 83C4 04        ADD     ESP, 4
0045C376  |. 56             PUSH    ESI
0045C377  |. E8 7EFBFFFF    CALL    TENSHOU.0045BEFA	�� ��p�҂t�h�B���̒����A��q�� 0045BF30  |. E8 6DFCFFFF    CALL    TENSHOU.0045BBA2 �ւƑ���
0045C37C  |. 66:8BF8        MOV     DI, AX				�� �`�w�Ɍ�p�ҕ����ԍ� �e�e�e�e���ƖŖS�B��p�҂̏����t�h���o����ɂ����ɂ���B
0045C37F  |. 83C4 04        ADD     ESP, 4
0045C382  |. 57             PUSH    EDI
0045C383  |. E8 6AEBFAFF    CALL    TENSHOU.0040AEF2
0045C388  |. 83C4 04        ADD     ESP, 4
0045C38B  |. 85C0           TEST    EAX, EAX
0045C38D  |. 74 09          JE      SHORT TENSHOU.0045C398
0045C38F  |. 56             PUSH    ESI                              ; /Arg1
0045C390  |. E8 39FEFFFF    CALL    TENSHOU.0045C1CE                 ; \TENSHOU.0045C1CE
0045C395  |. 83C4 04        ADD     ESP, 4
0045C398  |> 66:8BC7        MOV     AX, DI
0045C39B  |. 5F             POP     EDI
0045C39C  |. 5E             POP     ESI
0045C39D  \. C3             RETN


�E�d�c�h�̂����A�c�h�����݂̑喼�̕����ԍ��i�{�P�̂ق��j
�E�d�a�o���A�O���P�X�i�喼�ԍ��́{�P�̕��j

�E�d�a�w���Q�R
�v���C���̏ꍇ�́A
0045BF19  |> 55             PUSH    EBP
0045BF30  |. E8 6DFCFFFF    CALL    TENSHOU.0045BBA2
0045BF35  |. 66:8BD8        MOV     BX, AX
�`�w�ɐV�����喼�̕����ԍ�������B
�ŁA�V�喼�����h�c�̌��肪�s����B

�m�o�b�̏ꍇ�����l

*/


/*
 �喼�̌�p�ҕ����h�c���K�v�Ƃ��ꂽ��
 */





WORD iOrgBushouIDOfOnDaimyoKoukeishaBushouID = 0xFFFF;	// ���񂾑喼�����h�c
WORD iNewBushouIDOfOnDaimyoKoukeishaBushouID = 0xFFFF;	// �ύX����Ȃ炱���ɐV���ȕ����h�c������
int iOrgDaimyoIDOfOnDaimyoKoukeishaBushouID = 0xFFFF;	// �Y�������̑喼���͂̂h�c

void OnTenshouExeGetDaimyoKoukeishaBushouIDExecute() {

	int iBushouID = iOrgBushouIDOfOnDaimyoKoukeishaBushouID-1; // AX���������������ԍ�

	// �����łȂ�������݁B���Ȃ�������0xFFFF�ɂ��ǂ��B
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ScenarioMod�̃J�X�^�������ŌR�t��ݒ肷��
		if ( p_snOnCustomCondition ) {															                                  //���喼�ԍ�                              �����S�喼�����ԍ�                     ����p�җ\��ҕ����ԍ�
			int isNewDaimyoKoukeishaBushouID = ((PFNSNONCUSTOMCONDITION) p_snOnCustomCondition)("AS����::���C��::�喼��p�ҕ����ԍ�", iOrgDaimyoIDOfOnDaimyoKoukeishaBushouID, iOrgBushouIDOfOnDaimyoKoukeishaBushouID, iNewBushouIDOfOnDaimyoKoukeishaBushouID, "", "", 0.0, 0.0);
			// �R�t���ύX�ƂȂ��Ă���B
			if ( isNewDaimyoKoukeishaBushouID != CUSTOM_CONDITION_NOMODIFY ) {
				iNewBushouIDOfOnDaimyoKoukeishaBushouID = isNewDaimyoKoukeishaBushouID;
			}
		}

	}
}


/*
0045BF19  |> 55             PUSH    EBP
0045BF30  |. E8 6DFCFFFF    CALL    TENSHOU.0045BBA2
				�������� JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID �Ə��������Ă����ۂ̏���
0045BF35  |. 66:8BD8        MOV     BX, AX
*/
int pTenshouExeJumpFromToOnTenshouExeGetDaimyoKoukeishaBushouID	     =0x45BF30; // �֐��͂��̃A�h���X����AOnTenshouExeGetDaimyoKoukeishaBushouID�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeGetDaimyoKoukeishaBushouID	 =0x45BBA2; // �֐��͂��̃A�h���X����AOnTenshouExeGetDaimyoKoukeishaBushouID�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeGetDaimyoKoukeishaBushouID   =0x45BF35; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeGetDaimyoKoukeishaBushouID() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iOrgBushouIDOfOnDaimyoKoukeishaBushouID, AX // ���񂾕����̂h�c��ۑ�

		mov iOrgDaimyoIDOfOnDaimyoKoukeishaBushouID, EBP // �喼���͂h�c

		// ���Xtenshou.exe�ɂ�����������������
		call pTenshouExeJumpCallFromToOnTenshouExeGetDaimyoKoukeishaBushouID

		mov iNewBushouIDOfOnDaimyoKoukeishaBushouID, AX // ��p�ҕ����̂h�c��ۑ�

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnTenshouExeGetDaimyoKoukeishaBushouIDExecute();

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

		// �㏑�����ׂ��Ȃ�㏑������B
		MOV     AX, iNewBushouIDOfOnDaimyoKoukeishaBushouID

		jmp pTenshouExeReturnLblFromOnTenshouExeGetDaimyoKoukeishaBushouID
	}
}



char cmdOnTenshouExeJumpFromGetDaimyoKoukeishaBushouID[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeGetDaimyoKoukeishaBushouID() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeGetDaimyoKoukeishaBushouID;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeGetDaimyoKoukeishaBushouID + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromGetDaimyoKoukeishaBushouID+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeGetDaimyoKoukeishaBushouID), cmdOnTenshouExeJumpFromGetDaimyoKoukeishaBushouID, 5, NULL); //5�o�C�g�̂ݏ�������
}

