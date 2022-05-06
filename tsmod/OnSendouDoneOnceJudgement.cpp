#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


/*
 * ���������łɍs�������߁A�ēx�s���邩�ǂ������肷�鏈���B
 */

// ���������łɍs�������ǂ���
int isSendouDoneOnceJudgement = 0;

// �������s�������s�����h�c
int iSendouDoneOnceBushouID = -1;

void OnTenshouExeSendouOneOnceJudgementExecute() {

	// ������ȏ�ł���΁A�����͉��x�ł��s����B
	// ����ID�����͈̔�(����ȊO�̒l�������Ă��邱�Ƃ����邽�߃`�F�b�N)
	if (0 <= iSendouDoneOnceBushouID-1 && iSendouDoneOnceBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM) {
		if (getBushouInt(iSendouDoneOnceBushouID-1) >= 50 &&   // �q�d��50(=100)�ȏ�
			nb6bushouref[iSendouDoneOnceBushouID-1].maxint >= 70 &&	 // �q�˂�70(=140)�ȏ�
			nb6bushouref[iSendouDoneOnceBushouID-1].ambition >= 50) {	// ��]��50�ȏ�

			// �����͂܂��s���Ă��Ȃ����ƂƂ���B
			isSendouDoneOnceJudgement = 0;
		}
	}
}


/*
0041B892   . E8 5A270100    CALL    TENSHOU.0042DFF1
				�������� JMP TSMod.OnTSExeSendouOneOnceJudgement �Ə��������Ă����ۂ̏���
0041B897   . 83C4 04        ADD     ESP, 4
0041B89A     85C0           TEST    EAX, EAX
0041B89C     74 24          JE      SHORT TENSHOU.0041B8C2
*/
int pTenshouExeJumpFromToOnTenshouExeSendouOneOnceJudgement	 =0x41B892; // �֐��͂��̃A�h���X����AOnTenshouExeSendouOneOnceJudgement�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeSendouOneOnceJudgement =0x42DFF1; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeSendouOneOnceJudgement  =0x41B897; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeSendouOneOnceJudgement() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeSendouOneOnceJudgement

		// ��������������ǂ�����EAX�ɓ����Ă���BEAX����0�ł���ΐ����͍s���ς�
		mov isSendouDoneOnceJudgement, eax
		
		// �Ώۂ̕���ID
		mov iSendouDoneOnceBushouID, esi

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
	OnTenshouExeSendouOneOnceJudgementExecute();

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

		// �󋵂ɉ����āA��������������ǂ�����EAX�ɏ㏑��
		mov eax, isSendouDoneOnceJudgement

		jmp pTenshouExeReturnLblFromOnTenshouExeSendouOneOnceJudgement
	}
}

/*
0041B892   . E8 5A270100    CALL    TENSHOU.0042DFF1
				�������� JMP TSMod.OnTSExeSendouOneOnceJudgement �Ə��������Ă����ۂ̏���
0041B897   . 83C4 04        ADD     ESP, 4
0041B89A     85C0           TEST    EAX, EAX
0041B89C     74 24          JE      SHORT TENSHOU.0041B8C2
*/
char cmdOnTenshouExeJumpFromSendouOneOnceJudgement[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeSendouOneOnceJudgement() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeSendouOneOnceJudgement;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSendouOneOnceJudgement + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeSendouOneOnceJudgement  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromSendouOneOnceJudgement+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSendouOneOnceJudgement), cmdOnTenshouExeJumpFromSendouOneOnceJudgement, 5, NULL); //5�o�C�g�̂ݏ�������
}




