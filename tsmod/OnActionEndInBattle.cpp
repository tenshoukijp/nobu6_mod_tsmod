#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"

/*
�@�ȉ��̍s���ς݂Ƃ���ۂɒʉ߂��Ă���֐��B
  �����͂o�b/�m�o�b���ʂł���Ă���B

  �����j�b�g��z�u���������A�s���ς݂ƌ��Ȃ���邪�A���̏ꍇ�Aedi��esi�̒l�́ue�v�̕����ɃV�O�i��������悤�Ȃ̂ŁA���p�ł��邩���B
*/

/*
0042DC89  |. E8 4BFEFFFF    CALL    TENSHOU.0042DAD9
				�������� JMP TSMod.OnTSExeOnActionEndInBattle �Ə��������Ă����ۂ̏���
0042DC8E  |. 8B4C24 0C      MOV     ECX, DWORD PTR SS:[ESP+C]
0042DC92  |. 83C4 04        ADD     ESP, 4
0042DC95     0948 18        OR      DWORD PTR DS:[EAX+18], ECX
0042DC98  \. C3             RETN
*/

// �s�����C�������̃|�C���^
int iCurMainBushouPointerOnActionEndInBattle = NULL;

// �֌W����
int iCurBushouAIDOnOnActionEndInBattle = -1; // �����`
int iCurBushouBIDOnOnActionEndInBattle = -1; // �����a

// �e�X�̕����́A����̃^�[���ł̎c��s����
extern int arrRemainMoveCountAllBushou[GAMEDATASTRUCT_BUSHOU_NUM];

bool arrIsFirstHaichiAction[GAMEDATASTRUCT_BUSHOU_NUM];

extern int OnTenshouExeChangeMovePower(int iBushouID, int iPrev, int iNext);

void DoTenshouExeOnActonEndInBattleExecute(int iBushouID) {
	/*
	 *  �����Ɋe���j�b�g���s�������������̋�̓I�ȋL�q�������B
	 */
	// ������卬���������ԂłȂ���΁c
	if ( GetKonranStatus(iBushouID) != KONRAN_STATUS::���� && GetKonranStatus(iBushouID) != KONRAN_STATUS::�卬�� && GetKonranStatus(iBushouID) != KONRAN_STATUS::�������� ) {

		// ������ID���㋉�E�ł���΁c �������� �����X���b�g�E�^���^���X�ł���΁c ���邢�͎�ɂȂ�΁c
		if ( nb6bushouref[iBushouID].job >= 0xA0 || isLancelotTartare(iBushouID) || isShuiHidechiyo(iBushouID) ) {
			if ( arrRemainMoveCountAllBushou[iBushouID] > 1 ) { // �܂����s���J�E���g���c���Ă����
				setIsActionEndInBattle(iBushouID, 0);

				// �ړ��͂��኱��
				for ( int i=0; i < GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_NUM; i++ ) {
					if ( nb6bushou_hex_unit_info[i].bushou == iBushouID+1 ) {
						if ( nb6bushou_hex_unit_info[i].movepower < 6 ) {
							int iNewValue = OnTenshouExeChangeMovePower(iBushouID, nb6bushou_hex_unit_info[i].movepower, 6 );
							if (iNewValue != -1 ) {
								nb6bushou_hex_unit_info[i].movepower = iNewValue;  // �s���p���[�񕜁B
							}
							break;
						}
					}
				}
			}
		}
		arrRemainMoveCountAllBushou[iBushouID]--; // �J�E���g���P���炷
		if ( arrRemainMoveCountAllBushou[iBushouID] < 0 ) {
			arrRemainMoveCountAllBushou[iBushouID] = 0;
		}
	}
}


void OnTenshouExeOnActionEndInBattleExecute() {

	WORD *pBushouID = (WORD *)iCurMainBushouPointerOnActionEndInBattle;
	WORD iBushouID = (*pBushouID) -1;

	if ( getRemainTurnInBattle() <= 30 ) {
		if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			// ���z�u�ς݈ȍ~�ł���΁A�f�o�b�O�o��
			// ���z�u�̎��́A�z�u�����Łu�s�������v�ƌ��Ȃ���Ă��܂����߁A���̑Ώ��B
			if ( arrIsFirstHaichiAction[iBushouID] ) {
				// �e�X�̃��j�b�g�����ۂɍs�������ۂ̏���
				DoTenshouExeOnActonEndInBattleExecute(iBushouID);
			}

			// ���z�u�����m�ہB
			// ���z�u�̎��́A�z�u�����Łu�s�������v�ƌ��Ȃ���Ă��܂����߁A���̑Ώ��B
			arrIsFirstHaichiAction[iBushouID] = true;
		}
	}
}


/*
0042DC89  |. E8 4BFEFFFF    CALL    TENSHOU.0042DAD9
				�������� JMP TSMod.OnTSExeOnActionEndInBattle �Ə��������Ă����ۂ̏���
0042DC8E  |. 8B4C24 0C      MOV     ECX, DWORD PTR SS:[ESP+C]
0042DC92  |. 83C4 04        ADD     ESP, 4
0042DC95     0948 18        OR      DWORD PTR DS:[EAX+18], ECX
0042DC98  \. C3             RETN
*/
int pTenshouExeJumpFromToOnTenshouExeOnActionEndInBattle	 =0x42DC89; // �֐��͂��̃A�h���X����AOnTenshouExeOnActionEndInBattle�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeOnActionEndInBattle =0x42DAD9; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeOnActionEndInBattle	 =0x42DC98; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeOnActionEndInBattle() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// ���XTENSHOU.EXE���ɂ��������̂������ŁB���̒��ŁA�s����(=0x20)�ւƐݒ肳���
		call pTenshouExeJumpCallFromToOnTenshouExeOnActionEndInBattle

		MOV     ECX, DWORD PTR SS:[ESP+0xC]
		ADD     ESP, 4
		OR      DWORD PTR DS:[EAX+0x18], ECX


		// ������ID��ۑ� SI�̕����݂̂������ԍ�
		mov iCurBushouAIDOnOnActionEndInBattle, esi
		mov iCurBushouBIDOnOnActionEndInBattle, edi

		mov iCurMainBushouPointerOnActionEndInBattle, eax // 0x4D5D58�ȂǁA��̐푈���̊e�������ւ̃|�C���^�������Ă���B

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
	OnTenshouExeOnActionEndInBattleExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeOnActionEndInBattle
	}
}

/*
0049D7AA  |. 6A 03          |PUSH    3                               ; |Arg3 = 00000003
0049D7AC  |. 56             |PUSH    ESI                             ; |Arg2
0049D7AD  |. 56             |PUSH    ESI                             ; |Arg1
0049D7AE  |. E8 D3540000    |CALL    TENSHOU.004A2C86                ; \TENSHOU.004A2C86
				�������� JMP TSMod.OnTSExeOnActionEndInBattle �Ə��������Ă����ۂ̏���
0049D7B3  |. 83C4 1C        |ADD     ESP, 1C
0049D7B6  |. 85C0           |TEST    EAX, EAX
*/
char cmdOnTenshouExeJumpFromOnActionEndInBattle[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeOnActionEndInBattle() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeOnActionEndInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeOnActionEndInBattle + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeOnActionEndInBattle  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromOnActionEndInBattle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeOnActionEndInBattle), cmdOnTenshouExeJumpFromOnActionEndInBattle, 5, NULL); //5�o�C�g�̂ݏ�������
}




