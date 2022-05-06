#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "FuncPlayResourceWav.h"
#include "ScenarioMod.h"
#include "WaitTimeProcedure.h"



int iTeppouAttackBushouIDInTeppouDeath = -1; // ��������
int iTeppouDefendBushouIDInTeppouDeath = -1; // �����ꂽ��


int iTeppouResultDeathOnJudge = -1;

void OnTenshouExeJudgeTeppouDeathInBattleExecute() {

	int iDefendID = iTeppouDefendBushouIDInTeppouDeath-1;
	int iAttackID = iTeppouAttackBushouIDInTeppouDeath-1;

	if ( 0 <= iAttackID && iAttackID < GAMEDATASTRUCT_BUSHOU_NUM && 0 <= iDefendID && iDefendID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// �����ꂽ���������X���b�g�E�^���^���X�ł���B
		// �����Ď��Ȃ�
		if ( isLancelotTartare(iDefendID) ) {
			// �������������y�������́A�R�n�Ȃ�΁A�|��������Ƃ������ƂȂ̂ŁA�^���^���X�͎��ȂȂ�
			if ( nb6bushouref[iAttackID].form == 0 || nb6bushouref[iAttackID].form == 1 ) {
				iTeppouResultDeathOnJudge = 0; // ���ʂ͏��false
			}
		}

		// �����ꂽ������ɂł���B
		// �����Ď��Ȃ�
		if (isShuiHidechiyo(iDefendID)) {
			// �������������y�������́A�R�n�Ȃ�΁A�|��������Ƃ������ƂȂ̂ŁA��ɂ͎��ȂȂ�
			if (nb6bushouref[iAttackID].form == 0 || nb6bushouref[iAttackID].form == 1) {
				iTeppouResultDeathOnJudge = 0; // ���ʂ͏��false
			}
		}

		// �|������Ă�
		if ( nb6bushouref[iAttackID].form == 0 || nb6bushouref[iAttackID].form == 1 ) {
			// �֐���DLL�Œ�`����Ă�����ACONDITION�Ƃ��ČĂяo���A���̌��ʂ�TRUE�ł���΁A���S�����AFALSE�ł���΁A���S���Ȃ��B
			// ScenarioMod�œ��ɉ������Ȃ��Ƃ������Ƃ��\�Ƃ��邽�߁A
			if ( p_snOnCustomCondition ) {																						          // ����������						 ���ł��ꂽ��					�� �\�肳��Ă鎀�S����
				int isScenarioModDeathResult = ((PFNSNONCUSTOMCONDITION) p_snOnCustomCondition)("AS����::�푈::�|�󓮎Ҏ��S��", iTeppouAttackBushouIDInTeppouDeath, iTeppouDefendBushouIDInTeppouDeath, iTeppouResultDeathOnJudge, "", "", 0.0, 0.0);
				// ��C���v�j�łȂ���΁c
				if ( isScenarioModDeathResult != CUSTOM_CONDITION_NOMODIFY ) {
					iTeppouResultDeathOnJudge = isScenarioModDeathResult;
				}
			}
		// �S�C������Ă�
		} else if ( nb6bushouref[iAttackID].form == 2 || nb6bushouref[iAttackID].form == 3 ) {
			// �֐���DLL�Œ�`����Ă�����ACONDITION�Ƃ��ČĂяo���A���̌��ʂ�TRUE�ł���΁A���S�����AFALSE�ł���΁A���S���Ȃ��B
			// ScenarioMod�œ��ɉ������Ȃ��Ƃ������Ƃ��\�Ƃ��邽�߁A
			if ( p_snOnCustomCondition ) {																						          // ����������						 ���ł��ꂽ��					�� �\�肳��Ă鎀�S����
				int isScenarioModDeathResult = ((PFNSNONCUSTOMCONDITION) p_snOnCustomCondition)("AS����::�푈::�S�C�󓮎Ҏ��S��", iTeppouAttackBushouIDInTeppouDeath, iTeppouDefendBushouIDInTeppouDeath, iTeppouResultDeathOnJudge, "", "", 0.0, 0.0);
				// ��C���v�j�łȂ���΁c
				if ( isScenarioModDeathResult != CUSTOM_CONDITION_NOMODIFY ) {
					iTeppouResultDeathOnJudge = isScenarioModDeathResult;
				}
			}
		}
	}

}


/*
00432900  |. E8 10EDFFFF    CALL    TENSHOU.00431615                 ; \TENSHOU.00431615
							�������� JMP TSMod.OnTSExeeJudgeTeppouDeathInBattle �Ə��������Ă����ۂ̏���
00432905  |. 83C4 0C        ADD     ESP, 0C
00432908  |. 85C0           TEST    EAX, EAX						�� ����EAX�����񂾂��ǂ����̔���
*/
int pTenshouExeJumpFromToOnTenshouExeJudgeTeppouDeathInBattle    =0x432900; // �֐��͂��̃A�h���X����AOnTenshouExeJudgeTeppouDeathInBattle�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeJudgeTeppouDeathInBattle=0x431615; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeJudgeTeppouDeathInBattle =0x432905; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeJudgeTeppouDeathInBattle() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iTeppouAttackBushouIDInTeppouDeath, esi		// �������킪�̂h�c���R�s�[
		mov iTeppouDefendBushouIDInTeppouDeath, edi		// �����ꂽ����h�c���R�s�[

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeJudgeTeppouDeathInBattle;

		// ���S�̌��ʂ�ۑ�
		mov iTeppouResultDeathOnJudge, eax

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
	OnTenshouExeJudgeTeppouDeathInBattleExecute();

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

		mov eax, iTeppouResultDeathOnJudge; // ���S���ʂ��㏑��

		jmp pTenshouExeReturnLblFromOnTenshouExeJudgeTeppouDeathInBattle
	}
}

/*
00410467  |. 66:8BF0        MOV     SI, AX
0041046A  |. BF FFFF0000    MOV     EDI, 0FFFF
				�������� JMP TSMod.OnTSExeJudgeTeppouDeathInBattle �Ə��������Ă����ۂ̏���
0041046F  |. E8 71CA0900    CALL    TENSHOU.004ACEE5
*/
char cmdOnTenshouExeJumpFromJudgeTeppouDeathInBattle[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeJudgeTeppouDeathInBattle() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeJudgeTeppouDeathInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeJudgeTeppouDeathInBattle + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeJudgeTeppouDeathInBattle  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromJudgeTeppouDeathInBattle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeJudgeTeppouDeathInBattle), cmdOnTenshouExeJumpFromJudgeTeppouDeathInBattle, 5, NULL); //5�o�C�g�̂ݏ�������
}








//--------------------------------------�S�C�Ԃ��̏���


int iTeppouHanekaesiInTeppouDeath = 0L;

void OnTenshouExeTeppouDeathInBattleExecute() {

	// ������Ď��ɗ\��̑��������X���b�g�E�^���^���X�ł���B
	if (isLancelotTartare(iTeppouDefendBushouIDInTeppouDeath - 1)) {

		// ���������̂h�c
		int iAttackID = iTeppouAttackBushouIDInTeppouDeath - 1;

		// �����������S�C���������͋R�n�S�C���ł���΁c
		// �S�C�ʂ�e���Ԃ��Č������������Ȃ��鏈��
		if (nb6bushouref[iAttackID].form == 2 || nb6bushouref[iAttackID].form == 3) {

			int iBushouID = iTeppouDefendBushouIDInTeppouDeath - 1;

			// �S�C���˕Ԃ���
			FuncPlaySoundWav(721);

			WaitTimeProcedure(250);

			iTeppouHanekaesiInTeppouDeath = 1;

			// ���������ƌ����ꂽ���̌���
			int tmp = iTeppouDefendBushouIDInTeppouDeath;
			iTeppouDefendBushouIDInTeppouDeath = iTeppouAttackBushouIDInTeppouDeath;
			iTeppouAttackBushouIDInTeppouDeath = tmp;

			// �������̂��S�C�ł͂Ȃ��A�|��ł������ꍇ
		}
		else if (nb6bushouref[iAttackID].form == 0 || nb6bushouref[iAttackID].form == 1) {
			iTeppouHanekaesiInTeppouDeath = 0;

			// �����ɂ͗���Ă��Ȃ�
		}

	// ������Ď��ɗ\��̑�����ɂł���B
	} else if (isShuiHidechiyo(iTeppouDefendBushouIDInTeppouDeath - 1)) {

		// ���������̂h�c
		int iAttackID = iTeppouAttackBushouIDInTeppouDeath - 1;

		// �����������S�C���������͋R�n�S�C���ł���΁c
		// �S�C�ʂ�e���Ԃ��Č������������Ȃ��鏈��
		if (nb6bushouref[iAttackID].form == 2 || nb6bushouref[iAttackID].form == 3) {

			int iBushouID = iTeppouDefendBushouIDInTeppouDeath - 1;

			// �S�C���˕Ԃ���
			FuncPlaySoundWav(721);

			WaitTimeProcedure(250);

			iTeppouHanekaesiInTeppouDeath = 1;

			// ���������ƌ����ꂽ���̌���
			int tmp = iTeppouDefendBushouIDInTeppouDeath;
			iTeppouDefendBushouIDInTeppouDeath = iTeppouAttackBushouIDInTeppouDeath;
			iTeppouAttackBushouIDInTeppouDeath = tmp;

			// �������̂��S�C�ł͂Ȃ��A�|��ł������ꍇ
		}
		else if (nb6bushouref[iAttackID].form == 0 || nb6bushouref[iAttackID].form == 1) {
			iTeppouHanekaesiInTeppouDeath = 0;

			// �����ɂ͗���Ă��Ȃ�
		}

	// ����ȊO�̐l���Ԃɋ��܂����ꍇ�́A�O�ɖ߂�
	} else {
		iTeppouHanekaesiInTeppouDeath = 0;
	}
}


/*
00432900  |. E8 10EDFFFF    CALL    TENSHOU.00431615                 ; \TENSHOU.00431615
00432905  |. 83C4 0C        ADD     ESP, 0C
00432908  |. 85C0           TEST    EAX, EAX
0043290A  |. 74 0C          JE      SHORT TENSHOU.00432918
0043290C  |. 56             PUSH    ESI                              ; /Arg2	�� �U���������̕����ԍ�
0043290D  |. 57             PUSH    EDI                              ; |Arg1	�� �U�����ꂽ���̕����ԍ�
0043290E  |. E8 A5EDFFFF    CALL    TENSHOU.004316B8                 ; \TENSHOU.004316B8
							�������� JMP TSMod.OnTSExeeTeppouDeathInBattle �Ə��������Ă����ۂ̏���
00432913  |. 83C4 08        ADD     ESP, 8
*/
int pTenshouExeJumpFromToOnTenshouExeTeppouDeathInBattle    =0x43290E; // �֐��͂��̃A�h���X����AOnTenshouExeTeppouDeathInBattle�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeTeppouDeathInBattle=0x4316B8; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeTeppouDeathInBattle =0x432913; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeTeppouDeathInBattle() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		mov iTeppouAttackBushouIDInTeppouDeath, esi		// �������킪�̂h�c���R�s�[
		mov iTeppouDefendBushouIDInTeppouDeath, edi		// �����ꂽ����h�c���R�s�[

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
	OnTenshouExeTeppouDeathInBattleExecute();

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

		/*
		0043290C  |. 56             PUSH    ESI                              ; /Arg2	�� �U���������̕����ԍ�
		0043290D  |. 57             PUSH    EDI                              ; |Arg1	�� �U�����ꂽ���̕����ԍ�
		*/
		// ��̂Q���܂��́A�������ƂƂ���B
		pop edi
		pop esi

		// �l�����肩����(��������Ȃ���)�l�ŁA���X�̏�����������
		push iTeppouAttackBushouIDInTeppouDeath
		push iTeppouDefendBushouIDInTeppouDeath

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeTeppouDeathInBattle;

		mov iTeppouHanekaesiInTeppouDeath, 0

		jmp pTenshouExeReturnLblFromOnTenshouExeTeppouDeathInBattle
	}
}

/*
00410467  |. 66:8BF0        MOV     SI, AX
0041046A  |. BF FFFF0000    MOV     EDI, 0FFFF
				�������� JMP TSMod.OnTSExeTeppouDeathInBattle �Ə��������Ă����ۂ̏���
0041046F  |. E8 71CA0900    CALL    TENSHOU.004ACEE5
*/
char cmdOnTenshouExeJumpFromTeppouDeathInBattle[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeTeppouDeathInBattle() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeTeppouDeathInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeTeppouDeathInBattle + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeTeppouDeathInBattle  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromTeppouDeathInBattle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeTeppouDeathInBattle), cmdOnTenshouExeJumpFromTeppouDeathInBattle, 5, NULL); //5�o�C�g�̂ݏ�������
}




