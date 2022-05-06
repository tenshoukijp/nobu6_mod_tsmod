#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


/*
 * ���������őS�����������Ă��܂����|�̃��b�Z�[�W����a��������̂ŏ���������
 */

// ���ۂɉ��l���������̂��H
extern int iSendouKonranTargetConranCnt;

void OnTenshouExeSendouKonranAllMessageExecute() {

	// ���b�Z�[�W���]�񂳂�Ă���A�h���X�𓾂�
	int iSzAddress = CPM.getMemoryAddress(LOC4);
	// �A�h���X����char *�ϐ��ւƕύX
	char *szMsgSendouKonranAll = (char *)iSzAddress;

	if ( iSendouKonranTargetConranCnt > 6 ) {

		// �S��������
		char *szZenin = strstr(szMsgSendouKonranAll, "�S��");
		if (szZenin) {
			memcpy (szZenin, "����", 4);
		}

		// �����Ă��܂�����������
		szZenin = strstr(szMsgSendouKonranAll, "�����Ă��܂���");
		if (szZenin) {
			memcpy (szZenin, "������\0", 7);
		}
	}
	else if ( iSendouKonranTargetConranCnt >= 1 ) {

		// �S��������
		char *szZenin = strstr(szMsgSendouKonranAll, "���͂̎҂��������X�Ɗ�������");
		if (szZenin) {
			memcpy (szZenin, "���͂̐������̑������������", 28);
		}

		// �S����卬���ւƓ����Ă��܂�����������
		szZenin = strstr(szMsgSendouKonranAll, "�S����卬��");
		if ( szZenin ) {
			memcpy (szZenin, "�卬���ւƓ�����\0", 17);
		}
	}
	else {
		// �S��������
		char *szZenin = strstr(szMsgSendouKonranAll, "���͂̎҂�����");
		if ( szZenin ) {
			memcpy (szZenin, "���͂̒N�����������Ȃ�����\0", 27);
		}
	}
}


/*
00433AF1  |. 68 18100000    PUSH    1018                             ; |Arg1 = 00001018
00433AF6  |. E8 5D890300    CALL    TENSHOU.0046C458                 ; \TENSHOU.0046C458
				�������� JMP TSMod.OnTSExeSendouKonranAllMessage �Ə��������Ă����ۂ̏���
00433AFB  |. 83C4 08        ADD     ESP, 8
*/
int pTenshouExeJumpFromToOnTenshouExeSendouKonranAllMessage	    =0x433AF6; // �֐��͂��̃A�h���X����AOnTenshouExeSendouKonranAllMessage�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeSendouKonranAllMessage =0x46C458; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeSendouKonranAllMessage  =0x433AFB; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w

// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeSendouKonranAllMessage() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// TENSHOU.EXE�Ɍ��X����������
		call pTenshouExeJumpCallFromToOnTenshouExeSendouKonranAllMessage

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
	OnTenshouExeSendouKonranAllMessageExecute();

	__asm {

		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		jmp pTenshouExeReturnLblFromOnTenshouExeSendouKonranAllMessage
	}
}

/*
00433AF1  |. 68 18100000    PUSH    1018                             ; |Arg1 = 00001018
00433AF6  |. E8 5D890300    CALL    TENSHOU.0046C458                 ; \TENSHOU.0046C458
				�������� JMP TSMod.OnTSExeSendouKonranAllMessage �Ə��������Ă����ۂ̏���
00433AFB  |. 83C4 08        ADD     ESP, 8
*/
char cmdOnTenshouExeJumpFromSendouKonranAllMessage[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeSendouKonranAllMessage() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeSendouKonranAllMessage;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSendouKonranAllMessage + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeSendouKonranAllMessage  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromSendouKonranAllMessage+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSendouKonranAllMessage), cmdOnTenshouExeJumpFromSendouKonranAllMessage, 5, NULL); //5�o�C�g�̂ݏ�������
}




