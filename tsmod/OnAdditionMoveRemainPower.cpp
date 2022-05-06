#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


// �푈�ɂ����āA�ړ��͂̎c�ʂɉ��Z���������鎞�B
// (���^��==0F �������炱�̏����܂ŗ��Ȃ��̂Œ��ӁB���Ȃ����R�͕�����s�����������邽�߂Ƀ��^�[�����Ă��܂��Ă��邩�炾�B�����͂���͂��Ȃ̂��B)

/*
0042DC02  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0042DC06  |. E8 CEFEFFFF    CALL    TENSHOU.0042DAD9
0042DC0B  |. 8A4C24 0C      MOV     CL, BYTE PTR SS:[ESP+C]			// ���̏u�Ԃ�ESP+C �̃������A�h���X�ɁA�ړ��͂������Ă���B
0042DC0F  |. 83C4 04        ADD     ESP, 4
0042DC12  |. 8848 0C        MOV     BYTE PTR DS:[EAX+C], CL
0042DC15  \. C3             RETN
*/

// return �n�̏㏸�l(1�`10)
int isCurBushouHasAUma(int iBushouID) {
	int iUmaMaxRank = 0;

	// �ƕ����ʂ蒲�ׂ�
	for (int iKahouID=0; iKahouID<GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {

		// ���L�҂̕���ID����v����ꍇ�̂ݔ���̑Ώ�
		if (nb6kahou[iKahouID].attach-1 == iBushouID) {

			// �Ώۂ̉ƕ󂪁u�n�v�ł���B
			if ( nb6kahou[iKahouID].type == 8 ) { // �������Ă��邩������Ȃ��̂őΉ�����B

				// ���Ɏ����Ă����������������N��������΁A���ꂪ�{�l��������������ƕ�㏸�l
				if ( iUmaMaxRank < nb6kahou[iKahouID].param ) {
					iUmaMaxRank = nb6kahou[iKahouID].param;
				}
			}
		}
	}
	// �����N��Ԃ��B�ŏ�0(�������Ă��Ȃ�)�A�ő�10
	return iUmaMaxRank;
}










char iCurMoveRemainPower = 0;  // 
void OnTenshouExeMoveRemainPowerInBattleExecute() {
}

/*
0042DC02  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0042DC06  |. E8 CEFEFFFF    CALL    TENSHOU.0042DAD9
0042DC0B  |. 8A4C24 0C      MOV     CL, BYTE PTR SS:[ESP+C]			// ���̏u�Ԃ�ESP+C �̃������A�h���X�ɁA�ړ��͂������Ă���BCL�ɃR�s�[�����B
0042DC0F  |. 83C4 04        ADD     ESP, 4
0042DC12  |. 8848 0C        MOV     BYTE PTR DS:[EAX+C], CL
0042DC15  \. C3             RETN
*/
int pTenshouExeJumpFromToOnTenshouExeMoveRemainPowerInBattle    =0x42DC06; // �֐��͂��̃A�h���X����AOnTenshouExeMoveRemainPowerInBattle�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeMoveRemainPowerInBattle=0x42DAD9; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeMoveRemainPowerInBattle =0x42DC15; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeMoveRemainPowerInBattle() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeMoveRemainPowerInBattle

		// ���XTENSHOU.EXE���ɂ��������̂�������
		mov     cl, byte ptr ss:[esp+0xC]
		add     esp, 4

		// ����v�Z���ʂɂ��ړ��͎c��ʂ��R�s�[
		mov iCurMoveRemainPower, cl

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
	OnTenshouExeMoveRemainPowerInBattleExecute();

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
		mov     byte ptr ds:[eax+0xC], cl

		jmp pTenshouExeReturnLblFromOnTenshouExeMoveRemainPowerInBattle
	}
}

/*
00410467  |. 66:8BF0        MOV     SI, AX
0041046A  |. BF FFFF0000    MOV     EDI, 0FFFF
				�������� JMP TSMod.OnTSExeMoveRemainPowerInBattle �Ə��������Ă����ۂ̏���
0041046F  |. E8 71CA0900    CALL    TENSHOU.004ACEE5
*/
char cmdOnTenshouExeJumpFromMoveRemainPowerInBattle[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeMoveRemainPowerInBattle() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeMoveRemainPowerInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeMoveRemainPowerInBattle + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeMoveRemainPowerInBattle  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromMoveRemainPowerInBattle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeMoveRemainPowerInBattle), cmdOnTenshouExeJumpFromMoveRemainPowerInBattle, 5, NULL); //5�o�C�g�̂ݏ�������
}




