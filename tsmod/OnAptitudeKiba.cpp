#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


/*
 * �R�n�K���̎擾��
 */
int iApptitudeKibaBushouID = -1;

void OnTenshouExeAptitudeKibaExecute() {
}


/*
0040A185     E8 44320A00    CALL    TENSHOU.004AD3CE
				�������� JMP TSMod.OnTSExeAptitudeKiba �Ə��������Ă����ۂ̏���
0040A18A  |. 0FB7C0         MOVZX   EAX, AX
0040A18D  |. 83C4 08        ADD     ESP, 8

*/
int pTenshouExeJumpFromToOnTenshouExeAptitudeKiba	    =0x40A185; // �֐��͂��̃A�h���X����AOnTenshouExeAptitudeKiba�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeAptitudeKiba   =0x4AD3CE; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeAptitudeKiba    =0x40A18D; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w

// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeAptitudeKiba() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ����ID���R�s�[
		mov iApptitudeKibaBushouID, edi

		// ���XTENSHOU.EXE���ɂ���������
		call pTenshouExeJumpCallFromToOnTenshouExeAptitudeKiba

		movzx eax, ax

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
	OnTenshouExeAptitudeKibaExecute();

	__asm {

		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		jmp pTenshouExeReturnLblFromOnTenshouExeAptitudeKiba
	}
}

/*
0040A185     E8 44320A00    CALL    TENSHOU.004AD3CE
				�������� JMP TSMod.OnTSExeAptitudeKiba �Ə��������Ă����ۂ̏���
0040A18A  |. 0FB7C0         MOVZX   EAX, AX
*/
char cmdOnTenshouExeJumpFromAptitudeKiba[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeAptitudeKiba() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeAptitudeKiba;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeAptitudeKiba + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeAptitudeKiba  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromAptitudeKiba+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeAptitudeKiba), cmdOnTenshouExeJumpFromAptitudeKiba, 5, NULL); //5�o�C�g�̂ݏ�������
}




