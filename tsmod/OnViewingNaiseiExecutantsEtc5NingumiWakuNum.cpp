#include "WinTarget.h"

#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "SetNeedKoudouryoku.h"

// 
// �����ȂǂT�l�I�����邽�߂̓��̓_�C�A���O���o�����́u�g�̐��v�����߂�


/*
0042F1AC  /$ 53             PUSH    EBX
0042F1AD  |. 56             PUSH    ESI
0042F1AE  |. 57             PUSH    EDI
0042F1AF  |. 8BD9           MOV     EBX, ECX
0042F1B1  |. 33F6           XOR     ESI, ESI
0042F1B3  |. 8DBB AC000000  LEA     EDI, DWORD PTR DS:[EBX+AC]		�� ����EBX+AC�̒l���A�_�C�A���O����I���\�Ȑl���ƂȂ�B
0042F1B9  |. 3937           CMP     DWORD PTR DS:[EDI], ESI
0042F1BB  |. 76 0D          JBE     SHORT TENSHOU.0042F1CA
0042F1BD  |> 56             /PUSH    ESI
0042F1BE  |. 8BCB           |MOV     ECX, EBX
0042F1C0  |. 46             |INC     ESI
0042F1C1  |. E8 95FFFFFF    |CALL    TENSHOU.0042F15B
0042F1C6  |. 3937           |CMP     DWORD PTR DS:[EDI], ESI
0042F1C8  |.^77 F3          \JA      SHORT TENSHOU.0042F1BD
0042F1CA  |> 5F             POP     EDI
0042F1CB  |. 5E             POP     ESI
0042F1CC  |. 5B             POP     EBX
0042F1CD  \. C3             RETN

*/
#include "OnEnterMainPlayerCommand.h"

extern int iEnterOfClickMainScreenMainCommand; // �v���C���̍ۂɎ��s���Ă���R�}���h


BOOL isMustOverrideOfViewingNaiseiExecutantsEtc5WakuNum = FALSE;
int iWakuCntOfViewingNaiseiExecutantsEtc5WakuNum = 5;
void OnTenshouExeViewingNaiseiExecutantsEtc5WakuNumExecute() {
	isMustOverrideOfViewingNaiseiExecutantsEtc5WakuNum = FALSE;
	iWakuCntOfViewingNaiseiExecutantsEtc5WakuNum = 5;

	// �T�l�g�ŏ����s���l���f�t�H���g�ł͂Ȃ�
	if (TSModCommand::AllGundan::cmdNeedKoudouValue5Ningumi != CMD_NEED_KOUDOUVALUE_5NINGUMI_DEFAULT ) {
		// ����͏���������K�v������
		isMustOverrideOfViewingNaiseiExecutantsEtc5WakuNum = TRUE;

		// ���݂̃^�[���̏�ԍ�
		int iCastleID = nb6turn.castle - 1;
		if ( 0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM) {
			int iValidSelectableBushouCnt = 0;

			// ���̏�̏����R�c
			int iGundanID = nb6castle[iCastleID].attach - 1;
			if ( 0 <= iGundanID &&  iGundanID < GAMEDATASTRUCT_GUNDAN_NUM) {
				int iKoudouPowerRest = nb6gundan[iGundanID].act;

				// �I���\�ȃJ�E���g�� 
				int iCanSelectCnt = 0;
				int iCurLostPower = TSModCommand::AllGundan::cmdNeedKoudouValue5Ningumi; // �ŏ��͏���l���̂��̂���̃X�^�[�g

				// ���̏�ɏ������镐��
				for (int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {
					// �s���l�̎c�肪�A����l��菭�Ȃ��Ȃ�����_��
					if ( iKoudouPowerRest < iCurLostPower ) {
						break;
					}

					// �T�l���l�`�w(UI�I�ɂ�)
					if ( iCanSelectCnt >= 5 ) {
						break;
					}
/*					// 0����̓_��
					if ( iCurLostPower == 0 ) {
						break;
					}
*/
					// ���̏�ɋ��Ȃ��B���̐l
					if (nb6bushouref[iBushouID].castle-1 != iCastleID) {
						continue;
					}


					// �喼���R�c���������̂����ꂩ�ł���
					if (nb6bushouname[iBushouID].State == 0 || nb6bushouname[iBushouID].State == 1 || nb6bushouname[iBushouID].State == 2) {

						// ���S�Ƃ��Ȃ��Ă��Ȃ�
						if(nb6bushouname[iBushouID].battleDeath == 0) {
							// (�v�Z���)�c��s���l������
							iKoudouPowerRest -= iCurLostPower;

							// ��������s���l������
							iCurLostPower--;
							if (iCurLostPower < 0 ) {
								iCurLostPower = 0;
							}

							// �P�l�I�����
							iCanSelectCnt++;
						}
					}
				}

				iWakuCntOfViewingNaiseiExecutantsEtc5WakuNum = iCanSelectCnt;


				// �g�̑��������ƁA�I������ƁA�ȑO�̎c�[���c�邱�Ƃ�����B
				// ����Ęg�̑|��
				WORD *pWakuClean = (WORD *)GAMEDATASTRUCT_VARIABLE_VALUE_GROUP2_DIALOG;
				BOOL isMustBeAfterClean = false;
				for (int i=0; i<5; i++) {
					if (isMustBeAfterClean) {
						pWakuClean[i] = 0xFFFF;
					}
					if (pWakuClean[i]==0xFFFF) {
						isMustBeAfterClean = true;
					}
				}
				
			}
		}
	}
}

static int iTmpEAXOfViewingNaiseiExecutantsEtc5WakuNum = 0; // EAX�̒l�̈ꎟ�ۑ��p

/*
0042F1B1  |. 33F6           XOR     ESI, ESI
0042F1B3  |. 8DBB AC000000  LEA     EDI, DWORD PTR DS:[EBX+AC]		�� ����EBX+AC�̒l���A�_�C�A���O����I���\�Ȑl���ƂȂ�B
0042F1B9  |. 3937           CMP     DWORD PTR DS:[EDI], ESI
*/
int pTenshouExeJumpFromToOnTenshouExeViewingNaiseiExecutantsEtc5WakuNum    =0x42F1B3; // �֐��͂��̃A�h���X����AOnTenshouExeViewingNaiseiExecutantsEtc5WakuNum�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeViewingNaiseiExecutantsEtc5WakuNum =0x42F1B9; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeViewingNaiseiExecutantsEtc5WakuNum() {
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
	OnTenshouExeViewingNaiseiExecutantsEtc5WakuNumExecute();

	// ���������Ȃ���΂Ȃ�Ȃ�
	if (isMustOverrideOfViewingNaiseiExecutantsEtc5WakuNum) {
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

			mov iTmpEAXOfViewingNaiseiExecutantsEtc5WakuNum, eax // eax����������ۑ�

			mov eax, iWakuCntOfViewingNaiseiExecutantsEtc5WakuNum // �g�̐���EAX��
			mov DWORD PTR DS:[EBX+0xAC], eax					// ��������ɏ㏑��
			LEA     EDI, DWORD PTR DS:[EBX+0xAC]	// �V�ċL�ɂ��������X�̏���

			mov eax, iTmpEAXOfViewingNaiseiExecutantsEtc5WakuNum // eax��߂�

			jmp pTenshouExeReturnLblFromOnTenshouExeViewingNaiseiExecutantsEtc5WakuNum
		}
	} else {
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

			LEA     EDI, DWORD PTR DS:[EBX+0xAC]	// �V�ċL�ɂ��������X�̏���

			jmp pTenshouExeReturnLblFromOnTenshouExeViewingNaiseiExecutantsEtc5WakuNum
		}
	}
}


char cmdOnTenshouExeJumpFromViewingNaiseiExecutantsEtc5WakuNum[7]="\xE9\x90\x90\x90\x90\x90";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeViewingNaiseiExecutantsEtc5WakuNum() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeViewingNaiseiExecutantsEtc5WakuNum;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingNaiseiExecutantsEtc5WakuNum + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingNaiseiExecutantsEtc5WakuNum  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromViewingNaiseiExecutantsEtc5WakuNum+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingNaiseiExecutantsEtc5WakuNum), cmdOnTenshouExeJumpFromViewingNaiseiExecutantsEtc5WakuNum, 6, NULL); //6�o�C�g�̂ݏ�������
}




