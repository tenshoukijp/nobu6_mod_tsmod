#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


/*
 * �V�i���I�喼�I�����̏���
 */
void OnTenshouExeSelectingScenarioDaimyoExecute() {
	// �V�i���I�喼�I�����̓��ꏈ�����s���B
	EXSuperService_OnSelectingScenarioDaimyo();
}


/*
0049D289  |. E8 87A7F8FF    CALL    TENSHOU.00427A15                 ; \TENSHOU.00427A15
0049D28E  |. E8 4160F8FF    CALL    TENSHOU.004232D4
				�������� JMP TSMod.OnTSExeSelectingScenarioDaimyo �Ə��������Ă����ۂ̏���
0049D293  |. E8 3FE6FDFF    CALL    TENSHOU.0047B8D7
*/
int pTenshouExeJumpFromToOnTenshouExeSelectingScenarioDaimyo	 =0x49D28E; // �֐��͂��̃A�h���X����AOnTenshouExeSelectingScenarioDaimyo�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeSelectingScenarioDaimyo =0x4232D4; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeSelectingScenarioDaimyo  =0x49D293; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeSelectingScenarioDaimyo() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeSelectingScenarioDaimyo

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
	OnTenshouExeSelectingScenarioDaimyoExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeSelectingScenarioDaimyo
	}
}

/*
0049D289  |. E8 87A7F8FF    CALL    TENSHOU.00427A15                 ; \TENSHOU.00427A15
0049D28E  |. E8 4160F8FF    CALL    TENSHOU.004232D4
				�������� JMP TSMod.OnTSExeSelectingScenarioDaimyo �Ə��������Ă����ۂ̏���
0049D293  |. E8 3FE6FDFF    CALL    TENSHOU.0047B8D7
*/
char cmdOnTenshouExeJumpFromSelectingScenarioDaimyo[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeSelectingScenarioDaimyo() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeSelectingScenarioDaimyo;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSelectingScenarioDaimyo + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeSelectingScenarioDaimyo  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromSelectingScenarioDaimyo+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSelectingScenarioDaimyo), cmdOnTenshouExeJumpFromSelectingScenarioDaimyo, 5, NULL); //5�o�C�g�̂ݏ�������
}




