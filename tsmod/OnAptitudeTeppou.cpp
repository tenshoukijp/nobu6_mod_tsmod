#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


/*
 * �S�C�K���̎擾��
 */

int iApptitudeTeppouBushouID = -1;

int iAddtionApptitudeTeppouValue = 0;

void OnTenshouExeAptitudeTeppouExecute() {

	// �]���ɑ����S�C�K���͖��x0
	iAddtionApptitudeTeppouValue = 0;

	int iBushouID = iApptitudeTeppouBushouID-1;
}


/*
0040A1C6     E8 44320A00    CALL    TENSHOU.004AD3CE
				�������� JMP TSMod.OnTSExeAptitudeTeppou �Ə��������Ă����ۂ̏���
0040A1CB  |. 0FB7C0         MOVZX   EAX, AX
0040A1CE  |. 83C4 08        ADD     ESP, 8

*/
int pTenshouExeJumpFromToOnTenshouExeAptitudeTeppou	      =0x40A1C6; // �֐��͂��̃A�h���X����AOnTenshouExeAptitudeTeppou�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeAptitudeTeppou   =0x4AD3CE; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeAptitudeTeppou    =0x40A1CE; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w

// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeAptitudeTeppou() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ����ID���R�s�[
		mov iApptitudeTeppouBushouID, edi

		// ���XTENSHOU.EXE���ɂ���������
		call pTenshouExeJumpCallFromToOnTenshouExeAptitudeTeppou

		// TENSHOU.EXE���̌��̏�����������
		movzx   eax, ax

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
	OnTenshouExeAptitudeTeppouExecute();

	__asm {

		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax


		// �S�C�K���𑫂�����(����eax�͉�������ex�������S�C�K���Ȃ̂Œ���!!)
		add eax, iAddtionApptitudeTeppouValue

		jmp pTenshouExeReturnLblFromOnTenshouExeAptitudeTeppou
	}
}

/*
0040A185     E8 44320A00    CALL    TENSHOU.004AD3CE
				�������� JMP TSMod.OnTSExeAptitudeTeppou �Ə��������Ă����ۂ̏���
0040A18A  |. 0FB7C0         MOVZX   EAX, AX
*/
char cmdOnTenshouExeJumpFromAptitudeTeppou[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeAptitudeTeppou() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeAptitudeTeppou;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeAptitudeTeppou + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeAptitudeTeppou  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromAptitudeTeppou+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeAptitudeTeppou), cmdOnTenshouExeJumpFromAptitudeTeppou, 5, NULL); //5�o�C�g�̂ݏ�������
}




