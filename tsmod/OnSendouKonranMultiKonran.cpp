#include "WinTarget.h"
#include <time.h>

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


/*
 * ���������ő��l���������ɁA�N�����������邩�ǂ�����
 */

// �����������s�������s�����h�c
int iSendouKonranMultiKonranBushouID = -1;

// ���������̔��f�ΏۂƂȂ�^�[�Q�b�g�����h�c
int iSendouKonranMultiKonranTargetID = -1;

// ����̃^�[�Q�b�g��������������邩�ǂ���
int iSendouKonranCurTargetBushouMustKonran = -1;

// ���l���������̂��H �ł��̌�̃��b�Z�[�W��ύX����K�v������B
int iSendouKonranTargetConranCnt = 0;

int iSendouKonranMultiKonranSeed = 0;

void OnTenshouExeSendouKonranMultiKonranExecute() {

	iSendouKonranCurTargetBushouMustKonran = 1;

	iSendouKonranMultiKonranSeed++;

	// �S���̎��A�����Ώۂ͎��Q�͊܂܂�Ȃ��B
	// ����ID�����͈̔�(����ȊO�̒l�������Ă��邱�Ƃ����邽�߃`�F�b�N)
	if (0 <= iSendouKonranMultiKonranBushouID-1 && iSendouKonranMultiKonranBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM) {

		if (0 <= iSendouKonranMultiKonranTargetID-1 && iSendouKonranMultiKonranTargetID-1 < GAMEDATASTRUCT_BUSHOU_NUM) {

			/*
			// �Y���̕����̐��ɂ����闧���Ԃ�(�s��=0, ���ڍU�߂鑤=1, ���ڎ�鑤=2, �U�߂鑤�̉��R=3, ��鑤�̉��R=4, �����R=5)
			int GetWarStandPointTheBushou(int iBushouID);
			*/
			int i1stStdPnt = GetWarStandPointTheBushou(iSendouKonranMultiKonranBushouID-1);
			int i2ndStdPnt = GetWarStandPointTheBushou(iSendouKonranMultiKonranTargetID-1);

			// �������s�҂̏����喼�ƃ^�[�Q�b�g�̏����喼�������ł���΁A���������Ȃ�
			if ( nb6bushouref[iSendouKonranMultiKonranBushouID-1].attach == nb6bushouref[iSendouKonranMultiKonranTargetID-1].attach ) {
				iSendouKonranCurTargetBushouMustKonran = 0;

			// �d�|���l���A�u���ڍU�����v�ŁA�d�|�����鑤���u���̉��R�v�ł���΁A�������Ȃ��B
			} else if ( i1stStdPnt == 1 && i2ndStdPnt == 3 ) {
				iSendouKonranCurTargetBushouMustKonran = 0;

			// �d�|���l���A�u�U�����̉��R�v�ŁA�d�|�����鑤���u���ڍU�����v�ł���΁A�������Ȃ��B
			} else if ( i1stStdPnt == 3 && i2ndStdPnt == 1 ) {
				iSendouKonranCurTargetBushouMustKonran = 0;

			// �d�|���l���A�u���ڂ̎���w�v�ŁA�d�|�����鑤���u���̉��R�v�ł���΁A�������Ȃ��B
			} else if ( i1stStdPnt == 2 && i2ndStdPnt == 4 ) {
				iSendouKonranCurTargetBushouMustKonran = 0;

			// �d�|���l���A�u������̉��R�v�ŁA�d�|�����鑤���u���ڂ̎���R�v�ł���΁A�������Ȃ��B
			} else if ( i1stStdPnt == 4 && i2ndStdPnt == 2 ) {
				iSendouKonranCurTargetBushouMustKonran = 0;

			// �����Ґ���������قǁA��������m�������炷
			} else if ((iSendouKonranTargetConranCnt >= 1) && (rand() % 2 == 0) ) {
				iSendouKonranCurTargetBushouMustKonran = 0;

			} else {
				// ���������l���𑝂₷
				iSendouKonranTargetConranCnt++;
			}
		}
	}
}


/*
00433AC4  |. 6A 02          |PUSH    2
00433AC6  |. 50             |PUSH    EAX
00433AC7  |. E8 27A3FFFF    |CALL    TENSHOU.0042DDF3
				�������� JMP TSMod.OnTSExeSendouKonranMultiKonran �Ə��������Ă����ۂ̏���
00433ACC  |. 66:8B06        |MOV     AX, WORD PTR DS:[ESI]
00433ACF  |. 83C4 08        |ADD     ESP, 8

*/
int pTenshouExeJumpFromToOnTenshouExeSendouKonranMultiKonran	  =0x433AC7; // �֐��͂��̃A�h���X����AOnTenshouExeSendouKonranMultiKonran�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeSendouKonranMultiKonran =0x42DDF3; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeSendouKonranMultiKonran  =0x433ACC; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeSendouKonranMultiKonran() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// ���s������ID
		mov iSendouKonranMultiKonranBushouID, edi

		// �����ΏۂƂȂ�^�[�Q�b�g�̕�����ID
		mov iSendouKonranMultiKonranTargetID, eax

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
	OnTenshouExeSendouKonranMultiKonranExecute();

	if ( iSendouKonranCurTargetBushouMustKonran ) {
		__asm {

			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			// ����������ׂ��Ȃ�֐��Ăяo��
			call pTenshouExeJumpCallFromToOnTenshouExeSendouKonranMultiKonran
			jmp pTenshouExeReturnLblFromOnTenshouExeSendouKonranMultiKonran
		}

	} else {

		__asm {

			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			jmp pTenshouExeReturnLblFromOnTenshouExeSendouKonranMultiKonran
		}
	}
}

/*
00433AC4  |. 6A 02          |PUSH    2
00433AC6  |. 50             |PUSH    EAX
00433AC7  |. E8 27A3FFFF    |CALL    TENSHOU.0042DDF3
				�������� JMP TSMod.OnTSExeSendouKonranMultiKonran �Ə��������Ă����ۂ̏���
00433ACC  |. 66:8B06        |MOV     AX, WORD PTR DS:[ESI]
00433ACF  |. 83C4 08        |ADD     ESP, 8
*/
char cmdOnTenshouExeJumpFromSendouKonranMultiKonran[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeSendouKonranMultiKonran() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeSendouKonranMultiKonran;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSendouKonranMultiKonran + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeSendouKonranMultiKonran  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromSendouKonranMultiKonran+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSendouKonranMultiKonran), cmdOnTenshouExeJumpFromSendouKonranMultiKonran, 5, NULL); //5�o�C�g�̂ݏ�������
}




