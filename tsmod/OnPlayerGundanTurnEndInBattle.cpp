#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


/*
 * �퓬���Ƀv���C���[�̍U���S���R�c�̃^�[�����I�������
 */


void OnTenshouExePlayerGundanTurnEndInBattleExecute() {
}


/*
0046B237  |> 5F             POP     EDI								
				�������� JMP TSMod.OnTSExePlayerGundanTurnEndInBattle �Ə��������Ă����ۂ̏���

0046B238  |. 5E             POP     ESI								
0046B239  |. 8BE5           MOV     ESP, EBP					���͂̃^�[�����؂�ւ��Ō�ŁA�����ɗ���Ă���B
0046B23B  |. 5D             POP     EBP						
0046B23C  |. C3             RETN
*/
int pTenshouExeJumpFromToOnTenshouExePlayerGundanTurnEndInBattle	  =0x46B237; // �֐��͂��̃A�h���X����AOnTenshouExePlayerGundanTurnEndInBattle�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExePlayerGundanTurnEndInBattle  =0x46B23C; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w

// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExePlayerGundanTurnEndInBattle() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// �ȉ��S�s�́A���XTENSHOU.EXE���ɂ������L�q���̂܂�
		pop edi
		pop esi
		mov esp, ebp
		pop ebp


		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExePlayerGundanTurnEndInBattleExecute();

	__asm {

		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		jmp pTenshouExeReturnLblFromOnTenshouExePlayerGundanTurnEndInBattle
	}
}

/*
0046B237  |> 5F             POP     EDI								
				�������� JMP TSMod.OnTSExePlayerGundanTurnEndInBattle �Ə��������Ă����ۂ̏���

0046B238  |. 5E             POP     ESI								
0046B239  |. 8BE5           MOV     ESP, EBP					���͂̃^�[�����؂�ւ��Ō�ŁA�����ɗ���Ă���B
0046B23B  |. 5D             POP     EBP						
0046B23C  |. C3             RETN
*/
char cmdOnTenshouExeJumpFromPlayerGundanTurnEndInBattle[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExePlayerGundanTurnEndInBattle() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExePlayerGundanTurnEndInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExePlayerGundanTurnEndInBattle + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExePlayerGundanTurnEndInBattle  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromPlayerGundanTurnEndInBattle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExePlayerGundanTurnEndInBattle), cmdOnTenshouExeJumpFromPlayerGundanTurnEndInBattle, 5, NULL); //5�o�C�g�̂ݏ�������
}




