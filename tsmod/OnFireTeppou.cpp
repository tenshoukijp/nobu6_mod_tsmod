#include "WinTarget.h"

#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

/*
 * �S�C������������A�|���˂���
 * ��������s���������̔ԍ����m�ۂ��邽�߁B
 */
/*
004328B2  |. 57             PUSH    EDI                              ; /Arg2
004328B3  |. 56             PUSH    ESI                              ; |Arg1
004328B4  |. E8 F78F0600    CALL    TENSHOU.0049B8B0                 ; \TENSHOU.0049B8B0
004328B9  |. 83C4 08        ADD     ESP, 8
*/

int iFireTeppouAttackBushouID = -1; // �S�C������������
int iFireTeppouTargetBushouID = -1; // �S�C�������ꂽ����

void OnTenshouExeFireTeppouExecute() {
	// �ʏ펞�́A0xFFFF�͂���Ȃ��̂����A�卬����(�Y�����j�b�g�̈ӎv�œ����Ȃ��R�}���h���s�̍ۂɂ�
	// 0x[0018][0BID]�Ƃ����悤�Ȍ`�ɂȂ�悤���B
	iFireTeppouAttackBushouID = iFireTeppouAttackBushouID & 0xFFFF;
	iFireTeppouTargetBushouID = iFireTeppouTargetBushouID & 0xFFFF;
	; // ���ɂȂ�
}



/*
004328B2  |. 57             PUSH    EDI                              ; /Arg2�@ ���S�C�������ꂽ����
004328B3  |. 56             PUSH    ESI                              ; |Arg1   �� �S�C��������
004328B4  |. E8 F78F0600    CALL    TENSHOU.0049B8B0                 ; \TENSHOU.0049B8B0 �� �S�C���������ۂɌĂ΂��֐�
				�������� JMP TSMod.OnTSExeFireTeppou �Ə��������Ă����ۂ̏���
004328B9  |. 83C4 08        ADD     ESP, 8
*/
int pTenshouExeJumpFromToOnTenshouExeFireTeppou		=0x4328B4; // �֐��͂��̃A�h���X����AOnTenshouExeFireTeppou�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeFireTeppou =0x49B8B0; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeFireTeppou	=0x4328B9; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeFireTeppou() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ������ID��ۑ�
		mov iFireTeppouAttackBushouID, esi // �� �S�C��������
		mov iFireTeppouTargetBushouID, edi // ���S�C�������ꂽ����

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
	OnTenshouExeFireTeppouExecute();

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
		call pTenshouExeJumpCallFromToOnTenshouExeFireTeppou

		jmp pTenshouExeReturnLblFromOnTenshouExeFireTeppou
	}
}




char cmdOnTenshouExeJumpFromFireTeppou[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeFireTeppou() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeFireTeppou;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeFireTeppou + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeFireTeppou  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromFireTeppou+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeFireTeppou), cmdOnTenshouExeJumpFromFireTeppou, 5, NULL); //5�o�C�g�̂ݏ�������
}




