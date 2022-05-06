#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "TSModCommand.h"
#include "ScenarioMod.h"

/*
 * �퓬���ɓ����R�����g�̌��ʂ��o����
 * (���̌��ʂ𕢂������ꍇ�p)
 */

/*
// �����ɗ���Ɠ����͐������Ă�ƍl���Ă悢
0043345F  |> E8 84330600    CALL    TENSHOU.004967E8
00433464  |. 85C0           TEST    EAX, EAX
00433466  |. 75 19          JNZ     SHORT TENSHOU.00433481
00433468  |. 55             PUSH    EBP                              ; /Arg3			
00433469  |. 57             PUSH    EDI                              ; |Arg2			�� ����4�o�C�g����������鑤�̕����ԍ�  ��ʂɂ���0x18�����邱�Ƃ�����B
0043346A  |. 68 C30F0000    PUSH    0FC3                             ; |Arg1 = 00000FC3
0043346F  |. E8 0B900300    CALL    TENSHOU.0046C47F                 ; \TENSHOU.0046C47F
00433474  |. 83C4 0C        ADD     ESP, 0C



// ����4334DE�ɂ��鎞�́A�����͎��s���Ă���B
004334DE  |> E8 05330600    CALL    TENSHOU.004967E8
004334E3  |. 85C0           TEST    EAX, EAX
004334E5  |. 75 19          JNZ     SHORT TENSHOU.00433500
004334E7  |. 55             PUSH    EBP                              ; /Arg3			
004334E8  |. 57             PUSH    EDI                              ; |Arg2			�� ����4�o�C�g����������鑤�̕����ԍ�  ��ʂɂ���0x18�����邱�Ƃ�����B
004334E9  |. 68 C40F0000    PUSH    0FC4                             ; |Arg1 = 00000FC4
004334EE  |. E8 8C8F0300    CALL    TENSHOU.0046C47F                 ; \TENSHOU.0046C47F
004334F3  |. 83C4 0C        ADD     ESP, 0C
*/

int iGlobalResultOfNaiouInBattle = FALSE;
// �����̌��ʂ��㏑������B
BOOL OverrideResultOfNaiouInBattle(BOOL iCurrentResult, int iBushouID, int iTargetID) {

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= iTargetID && iTargetID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// �����X���b�g�^���^���X���d�|���鑤�ł���΁A�L�������킹�������͐���
		if ( isLancelotTartare(iBushouID) ) {
			iCurrentResult = TRUE;
		// �����X���b�g�^���^���X���d�|�����鑤�ł���΁A�L�������킹�������͐���
		} else if ( isLancelotTartare(iTargetID) ) {
			iCurrentResult = FALSE;
		// �f�l�u���d�|���鑤�ł���΁A�L�������킹�������͐���
		} else if ( strcmp( nb6bushouname[iBushouID].familyname, "�j�k" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "�l�m" )==0 ) {
			iCurrentResult = TRUE;
		}

		// SceanrioMod�Ō��ʂ��㏑������Ȃ炷�邪�ǂ��B
		if ( p_snOnCustomCondition ) {																				  // ������������	���������ꂽ��	�� �\�肳��Ă��������
			int isScenarioModNaiouResult = ((PFNSNONCUSTOMCONDITION) p_snOnCustomCondition)("AS����::�푈::�������ʉ�", iBushouID+1,      iTargetID+1,    iCurrentResult, "", "", 0.0, 0.0);
			// ��C���v�j�łȂ���΁c
			if ( isScenarioModNaiouResult != CUSTOM_CONDITION_NOMODIFY ) {
				iCurrentResult = isScenarioModNaiouResult;
			}
		}
	} 

	// ���݂̌��ʂ����̂܂܃O���[�o���ɑ��
	iGlobalResultOfNaiouInBattle = iCurrentResult; // ���ʂ��㏑���B

	// �ꉞ���^�[��
	return iGlobalResultOfNaiouInBattle;
}



int iBushouIDOfResultOfNaiouInBattlle = -1; // �����̍s����(�d�|������)�̕����ԍ�
int iTargetIDOfResultOfNaiouInBattlle = -1; // �����̎蓮��(�d�|����ꂽ��)�̕����ԍ�



void OnTenshouExeResultOfNaiouSuccessInBattleExecute() {

	int iBushouID = (iBushouIDOfResultOfNaiouInBattlle & 0xFFFF)-1; // ����������
	int iTargetID = (iTargetIDOfResultOfNaiouInBattlle & 0xFFFF)-1; // ����������

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= iTargetID && iTargetID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		OverrideResultOfNaiouInBattle(TRUE, iBushouID, iTargetID); // TRUE = �����ɂ��Ă��鎞�_�ŁA�����̌��ʂ͐������Ă��邪�A���̊֐��ɂ���Č��ʂ�ς�������������Ȃ��B
	}
}


/*
0043345F  |> E8 84330600    CALL    TENSHOU.004967E8
00433464  |. 85C0           TEST    EAX, EAX
				�������� JMP TSMod.OnTSExeResultOfNaiouSuccessInBattle �Ə��������Ă����ۂ̏���
 */
int pTenshouExeJumpFromToOnTenshouExeResultOfNaiouSuccessInBattle    =0x43345F; // �֐��͂��̃A�h���X����AOnTenshouExeResultOfNaiouSuccessInBattle�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeResultOfNaiouSuccessInBattle=0x4967E8; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeResultOfNaiouSuccessInBattle =0x433464; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
extern int pTenshouExeReturnLblFromOnTenshouExeResultOfNaiouFailInBattle;
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeResultOfNaiouSuccessInBattle() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeResultOfNaiouSuccessInBattle

		// �������鑤�̕����ԍ���ۑ�
		mov iBushouIDOfResultOfNaiouInBattlle,  ESI

		// �������ꂽ���̕����ԍ���ۑ�
		mov iTargetIDOfResultOfNaiouInBattlle,  EDI

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
	OnTenshouExeResultOfNaiouSuccessInBattleExecute();

	// �������ʂ������ł������Ȃ�A�ʏ�ʂ�̏ꏊ�Ƀ��^�[������B
	if ( iGlobalResultOfNaiouInBattle == TRUE ) {
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

			jmp pTenshouExeReturnLblFromOnTenshouExeResultOfNaiouSuccessInBattle
		}

	// �����������ʂ�FALSE�ւƏ㏑�����ꂽ�̂ł���΁A���s�����ꍇ�̏ꏊ�ւƃ��^�[������B
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

			jmp pTenshouExeReturnLblFromOnTenshouExeResultOfNaiouFailInBattle
		}
	}
}

char cmdOnTenshouExeJumpFromResultOfNaiouSuccessInBattle[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeResultOfNaiouSuccessInBattle() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeResultOfNaiouSuccessInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeResultOfNaiouSuccessInBattle + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeResultOfNaiouSuccessInBattle  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromResultOfNaiouSuccessInBattle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeResultOfNaiouSuccessInBattle), cmdOnTenshouExeJumpFromResultOfNaiouSuccessInBattle, 5, NULL); //5�o�C�g�̂ݏ�������
}













void OnTenshouExeResultOfNaiouFailInBattleExecute() {

	int iBushouID = (iBushouIDOfResultOfNaiouInBattlle & 0xFFFF)-1; // ����������
	int iTargetID = (iTargetIDOfResultOfNaiouInBattlle & 0xFFFF)-1; // ����������

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= iTargetID && iTargetID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		OverrideResultOfNaiouInBattle(FALSE, iBushouID, iTargetID); // FALSE = �����ɂ��Ă��鎞�_�ŁA�����̌��ʂ͎��s���Ă��邪�A���̊֐��ɂ���Č��ʂ�ς�������������Ȃ��B
	}
}


/*
004334DE  |> E8 05330600    CALL    TENSHOU.004967E8
004334E3  |. 85C0           TEST    EAX, EAX
				�������� JMP TSMod.OnTSExeResultOfNaiouFailInBattle �Ə��������Ă����ۂ̏���
 */
int pTenshouExeJumpFromToOnTenshouExeResultOfNaiouFailInBattle    =0x4334DE; // �֐��͂��̃A�h���X����AOnTenshouExeResultOfNaiouFailInBattle�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeResultOfNaiouFailInBattle=0x4967E8; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeResultOfNaiouFailInBattle =0x4334E3; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
extern int pTenshouExeReturnLblFromOnTenshouExeResultOfNaiouSuccessInBattle;
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeResultOfNaiouFailInBattle() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeResultOfNaiouFailInBattle

		// �������鑤�̕����ԍ���ۑ�
		mov iBushouIDOfResultOfNaiouInBattlle,  ESI

		// �������ꂽ���̕����ԍ���ۑ�
		mov iTargetIDOfResultOfNaiouInBattlle,  EDI

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
	OnTenshouExeResultOfNaiouFailInBattleExecute();

	// �������ʂ����s�ł������Ȃ�A�ʏ�ʂ�̏ꏊ�Ƀ��^�[������B
	if ( iGlobalResultOfNaiouInBattle == FALSE ) {

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

			jmp pTenshouExeReturnLblFromOnTenshouExeResultOfNaiouFailInBattle
		}

	// �����������ʂ�TRUE�ւƏ㏑�����ꂽ�̂ł���΁A���������ꍇ�̏ꏊ�ւƃ��^�[������B
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

			jmp pTenshouExeReturnLblFromOnTenshouExeResultOfNaiouSuccessInBattle
		}
	}
}

char cmdOnTenshouExeJumpFromResultOfNaiouFailInBattle[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeResultOfNaiouFailInBattle() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeResultOfNaiouFailInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeResultOfNaiouFailInBattle + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeResultOfNaiouFailInBattle  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromResultOfNaiouFailInBattle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeResultOfNaiouFailInBattle), cmdOnTenshouExeJumpFromResultOfNaiouFailInBattle, 5, NULL); //5�o�C�g�̂ݏ�������
}
