#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"

// �푈���̒����R�}���h��I�񂾍ۂɁA�Ȃ������m��100�Ƀt�B���^�[����Ă��܂��֐�������B
// ���ꂪ�Ă΂ꂽ���̏���


// �����̒l�B���̒l���O�̎��i���y�̎��j�������\�ƂȂ�B���̒l�������ɂ���ĕς���̂��ړI�B
int iConditionedForm = -1;

// ���������Ώۂ̕���ID(�z���ł͂Ȃ��A�����̒l�ł���(�z����+1������))
int iConditionedBushouID = 0;

void OnTenshouExeSoldierDraftConditionInBattleExecute() {

	// ���������͈̔͂ł��邱��
	if (0 <= iConditionedBushouID-1 && iConditionedBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM) {
		
		// �����X���b�g�E�^���^���X�ł���B
		if ( isLancelotTartare(iConditionedBushouID-1) ) {
			// ���Ԃ�0(=���y)�Ƃ��邱�Ƃŋ����I�ɒ����\�ƂȂ�B
			iConditionedForm = 0;
		}

		// ��ɂł���B
		if (isShuiHidechiyo(iConditionedBushouID - 1)) {
			// ���Ԃ�0(=���y)�Ƃ��邱�Ƃŋ����I�ɒ����\�ƂȂ�B
			iConditionedForm = 0;
		}

	}
}


/*
0041BBE9   . 83C4 04        ADD     ESP, 4										
0041BBEC   . 85C0           TEST    EAX, EAX				����EAX�e�X�g��0���ƁA������MAX�ł��邽�߁A���₷�K�v���Ȃ��Ƃł�BEAX���P�Ƃ���Ηǂ�
0041BBEE   . 57             PUSH    EDI                              ; /Arg2										
0041BBEF   . 75 1E          JNZ     SHORT TENSHOU.0041BC0F           ; |										
0041BBF1   . 68 DC0F0000    PUSH    0FDC                             ; |Arg1 = 00000FDC										
0041BBF6   . E8 5D080500    CALL    TENSHOU.0046C458                 ; \TENSHOU.0046C458										
0041BBFB   . 83C4 08        ADD     ESP, 8										
0041BBFE   . 50             PUSH    EAX										
0041BBFF   . 57             PUSH    EDI										
0041BC00   . E8 42EA0700    CALL    TENSHOU.0049A647										
0041BC05   . 83C4 08        ADD     ESP, 8										
0041BC08   . 33C0           XOR     EAX, EAX										
0041BC0A   . E9 A2000000    JMP     TENSHOU.0041BCB1										
0041BC0F   > E8 75E2FEFF    CALL    TENSHOU.00409E89										
				�������� JMP TSMod.OnTSExeSoldierDraftConditionInBattle �Ə��������Ă����ۂ̏���
0041BC14   . 83C4 04        ADD     ESP, 4										
0041BC17   . 85C0           TEST    EAX, EAX				����EAX�e�X�g���u0�ȊO�v���ƁA����ނ����y�łȂ��ƒ����ł��Ȃ��B�������Ȃ����ƁA����Ɋւ�炸�A�������\�ƂȂ�B
0041BC19   . 74 24          JE      SHORT TENSHOU.0041BC3F										
0041BC1B   . 33C0           XOR     EAX, EAX										
*/
int pTenshouExeJumpFromToOnTenshouExeSoldierDraftConditionInBattle		=0x41BC0F; // �֐��͂��̃A�h���X����AOnTenshouExeSoldierDraftConditionInBattle�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeSoldierDraftConditionInBattle  =0x409E89; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeSoldierDraftConditionInBattle	=0x41BC17; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeSoldierDraftConditionInBattle() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeSoldierDraftConditionInBattle

		// ���XTENSHOU.EXE���ɂ��������̂�������
		add esp, 4

		// ���Ԃ̒l���Ԃ��Ă���B0�Ȃ�Α��y���Ƃ݂Ȃ����̂ŁA�����\�ƂȂ�B
		mov iConditionedForm, eax

		// ����ID���R�s�[
		mov iConditionedBushouID, edi

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// ���������ύX����������Ȃ�΁c
	OnTenshouExeSoldierDraftConditionInBattleExecute();

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

		// �u���������l��eax���㏑��
		mov eax, iConditionedForm

		jmp pTenshouExeReturnLblFromOnTenshouExeSoldierDraftConditionInBattle
	}
}

/*
0041BC0A   . E9 A2000000    JMP     TENSHOU.0041BCB1										
0041BC0F   > E8 75E2FEFF    CALL    TENSHOU.00409E89										
				�������� JMP TSMod.OnTSExeSoldierDraftConditionInBattle �Ə��������Ă����ۂ̏���
0041BC14   . 83C4 04        ADD     ESP, 4										
*/
char cmdOnTenshouExeJumpFromSoldierDraftConditionInBattle[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeSoldierDraftConditionInBattle() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeSoldierDraftConditionInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSoldierDraftConditionInBattle + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeSoldierDraftConditionInBattle  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromSoldierDraftConditionInBattle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSoldierDraftConditionInBattle), cmdOnTenshouExeJumpFromSoldierDraftConditionInBattle, 5, NULL); //5�o�C�g�̂ݏ�������
}




