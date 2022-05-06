#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"

int iSelectingScenarioLocation = -1;
int iSelectingScenarioID = -1;

/*

00422D72   MOVZX   ECX, WORD PTR DS:[EAX*2+4D3718]
00427B6A   MOVZX   EAX, WORD PTR DS:[EDI*2+4D3718]
00436F98   MOVZX   EAX, WORD PTR DS:[EBP*2+4D3718]
00478A21   MOVZX   EAX, WORD PTR DS:[EAX*2+4D3718]

004D3718  00 00 02 00 03 00 04 00 01 00 05 00              .......�@�� �V�i���I��Location�ƃV�i���IID�̑Ή��\
*/


// �U�̃V�i���I�̂ǂ��I�������̂��B
/*
1534 0  0
1560 2  1
1571 3  2
1582 4  3
1546 1  4
1599 5  5
*/
void OnTenshouExeSelectingScenarioExecute() {

	// �V�i���I�I���̃��[�h�ƂȂ�����A���̃t���O�𗧂Ă�
	CommonGlobalHandle::isModeScenario = true;

	// �V�i���I�͂����ƑI�����ꂽ��
	if ( 0 <= iSelectingScenarioID && iSelectingScenarioID <= 5 ) {
		// �I���V�i���I���P�[�V������ݒ�
		if ( iSelectingScenarioID == 0 ) {
			iSelectingScenarioLocation = 0;
		} else if ( iSelectingScenarioID == 1 ) {
			iSelectingScenarioLocation = 2;
		} else if ( iSelectingScenarioID == 2 ) {
			iSelectingScenarioLocation = 3;
		} else if ( iSelectingScenarioID == 3 ) {
			iSelectingScenarioLocation = 4;
		} else if ( iSelectingScenarioID == 4 ) {
			iSelectingScenarioLocation = 1;
		} else if ( iSelectingScenarioID == 5 ) {
			iSelectingScenarioLocation = 5;
		} else {
			iSelectingScenarioLocation = -1;
		}
	}
}


/*
0049D7CE  |> E8 75F8FFFF    |CALL    TENSHOU.0049D048
				�������� JMP TSMod.OnTSExeSelectingScenario �Ə��������Ă����ۂ̏���
0049D7D3  |. 3BC6           |CMP     EAX, ESI
 */
int pTenshouExeJumpFromToOnTenshouExeSelectingScenario    =0x49D7CE; // �֐��͂��̃A�h���X����AOnTenshouExeSelectingScenario�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeSelectingScenario=0x49D048; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeSelectingScenario =0x49D7D3; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeSelectingScenario() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeSelectingScenario
		mov iSelectingScenarioID, eax // �I�������V�i���I�ԍ��B���Ԃ��o���o���B�I�����ĂȂ����ɂ�0xFFFF

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
	OnTenshouExeSelectingScenarioExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeSelectingScenario
	}
}

/*
0049D7CE  |> E8 75F8FFFF    |CALL    TENSHOU.0049D048
				�������� JMP TSMod.OnTSExeSelectingScenario �Ə��������Ă����ۂ̏���
0049D7D3  |. 3BC6           |CMP     EAX, ESI
*/
char cmdOnTenshouExeJumpFromSelectingScenario[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeSelectingScenario() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeSelectingScenario;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSelectingScenario + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeSelectingScenario  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromSelectingScenario+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSelectingScenario), cmdOnTenshouExeJumpFromSelectingScenario, 5, NULL); //5�o�C�g�̂ݏ�������
}




