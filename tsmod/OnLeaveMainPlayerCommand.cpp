#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "ScenarioMod.h"

/*
00410693  |.^EB F0          JMP     SHORT TENSHOU.00410685
00410695  |> E8 8F030000    CALL    TENSHOU.00410A29
0041069A  |. 8BC3           MOV     EAX, EBX
*/

void OnLeaveMainPlayerCommand() {

	// �������̃R�}���h����o���ۂɂ͂����Ŕ���\
	// OutputDebugStream << "Leave" << endl;

	// ScenarioMod�Ōďo��
	if ( p_snOnLeaveMainPlayerCommand ) {
		p_snOnLeaveMainPlayerCommand();
	}
}





//----------------------------------------------------------------------�v���C���^�[������E�p���Ă��܂��B(���[�h������A�Êς�����A�Q�[���I��������)

void OnTenshouExeLeavePlayerGameExecute() {

	OnLeaveMainPlayerCommand();
}


/*
00410693  |.^EB F0          JMP     SHORT TENSHOU.00410685
00410695  |> E8 8F030000    CALL    TENSHOU.00410A29
				�������� JMP TSMod.OnTSExeLeavePlayerGame �Ə��������Ă����ۂ̏���
0041069A  |. 8BC3           MOV     EAX, EBX
*/
int pTenshouExeJumpFromToOnTenshouExeLeavePlayerGame		=0x410695; // �֐��͂��̃A�h���X����AOnTenshouExeLeavePlayerGame�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeLeavePlayerGame    =0x410A29; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeLeavePlayerGame	    =0x41069A; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeLeavePlayerGame() {
	// �X�^�b�N�ɂ��߂Ă���
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
	
	// �Q�[���f�[�^���v���C���[�^�[�����ς�����i�K�œǂݍ��ݍX�V���Ă���
	OnTenshouExeLeavePlayerGameExecute();

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

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeLeavePlayerGame

		jmp pTenshouExeReturnLblFromOnTenshouExeLeavePlayerGame
	}
}

char cmdOnTenshouExeJumpFromLeavePlayerGame[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeLeavePlayerGame() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeLeavePlayerGame;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeLeavePlayerGame + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeLeavePlayerGame  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromLeavePlayerGame+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeLeavePlayerGame), cmdOnTenshouExeJumpFromLeavePlayerGame, 5, NULL); //5�o�C�g�̂ݏ�������
}


