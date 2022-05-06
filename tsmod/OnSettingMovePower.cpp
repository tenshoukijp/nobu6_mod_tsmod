#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"

#include "ScenarioMod.h"

extern int isCurBushouHasAUma(int iBushouID);

// iPrev���݂̒l�AiNext�ύX��̒l
int OnTenshouExeChangeMovePower(int iBushouID, int iPrev, int iNext) {

	int iModifiedNext = iNext;

	// �񕜎�
	if (iNext > iPrev) {
		// �^���^���X�ł���΁c
		if ( isLancelotTartare( iBushouID ) ) {
			if ( iNext < 12 ) {
				iModifiedNext = 15; // �Œ�ł�15�͖񑩂����B
			}
			if ( iNext < 28 ) {
				iModifiedNext = iNext + 3; // �ړ��c�ʂ�3���₳���B
			}
		}
		// ��ɂł���΁c
		else if ( isShuiHidechiyo(iBushouID) ) {
			if (iNext < 12) {
				iModifiedNext = 12; // �Œ�ł�12�͖񑩂����B
			}
			if (iNext < 28) {
				iModifiedNext = iNext + 3; // �ړ��c�ʂ�3���₳���B
			}
		}
		// �����A�y�сA�E�҂ł���΁c
		else if ( nb6bushouref[iBushouID].job == 0xC0 || nb6bushouref[iBushouID].job ==0x10 ) {
			if ( iNext < 25 ) {
				iModifiedNext = iNext + 3; // �ړ��c�ʂ�3���₳���B
			}
		}

		else {
			// �n�������Ă���
			int iHasUmaRank = isCurBushouHasAUma(iBushouID);
			if ( iHasUmaRank > 0 ) {
				if ( iNext < (0xF + iHasUmaRank) ) { // �ʏ�l��15+�n�����N�ȉ��ł���΁c
					iModifiedNext = iNext + (1 + int(iHasUmaRank/5));  // �ړ��c�ʂ�1�`3���₳���B
				}
			}
		}
	} // �񕜎��I���

	/*
	OutputDebugStream << nb6bushouname[iBushouID].fastname << ":";
	OutputDebugStream << "���݂̈ړ���:" << iPrev << ",";
	OutputDebugStream << "���̈ړ���:" << iNext << endl;
	*/

	// ScenarioMod�ɒ�`����Ă�΁A�㏑��
	if (p_snOnChangeUnitMovePowerInBattle) {
		iModifiedNext = p_snOnChangeUnitMovePowerInBattle(iBushouID, iPrev, iModifiedNext);
	}

	if (iNext != iModifiedNext) {
		return iModifiedNext;
	} else {
		return -1;
	}
}


// �ړ��͎c�ʂ��ύX���ꂽ���B�u����̃^�C�~���O�v�Ɓu�񕜂̃^�C�~���O�v������B
static int iEAXOfSettingMovePowerBushouUnitNo = NULL;
static int pNextPowerOfSettingMovePower = 0;
void OnTenshouExeSettingMovePowerInBattleExecute() {

	int iNo = (iEAXOfSettingMovePowerBushouUnitNo & 0xFFFF) -1;
	int iBushouID = nb6bushou_hex_unit_info[iNo].bushou - 1;
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int iPrev = (int)(nb6bushou_hex_unit_info[iNo].movepower);
		int iNext = *(int *)(pNextPowerOfSettingMovePower);
		int iValueOfOverWrite = OnTenshouExeChangeMovePower(iBushouID, iPrev, iNext);

		// �ύX����Ƃ����ӎv���������ꂽ�Ȃ�A�㏑������B�X�^�b�N������������
		if (iValueOfOverWrite != -1) {
			*(int *)(pNextPowerOfSettingMovePower) = iValueOfOverWrite;
		}
	}
}


/*
0040C20D  |. 74 0D          JE      SHORT TENSHOU.0040C21C
0040C20F  |. FF7424 08      PUSH    DWORD PTR SS:[ESP+8]		�V���Ȉړ��͎c�ʂ̃v�b�V��
0040C213  |. 50             PUSH    EAX							���ʂ̃r�b�g���푈���̔z��̔ԍ�(NB6BUSHOU_HEX_UNIT_INFO�̔z��ԍ�+1)���w���͗l
0040C214  |. E8 E9190200    CALL    TENSHOU.0042DC02
0040C219  |. 83C4 08        ADD     ESP, 8
0040C21C  \> C3             RETN

*/
int pTenshouExeJumpFromToOnTenshouExeSettingMovePowerInBattle		=0x40C214; // �֐��͂��̃A�h���X����AOnTenshouExeSettingMovePowerInBattle�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeSettingMovePowerInBattle   =0x42DC02; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeSettingMovePowerInBattle	=0x40C219; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeSettingMovePowerInBattle() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iEAXOfSettingMovePowerBushouUnitNo, EAX  // ���j�b�g�̔ԍ�ix

		// ��UEAX�ɃR�s�[
		mov eax, ESP
		add eax, 0x4

		mov pNextPowerOfSettingMovePower, EAX  // ���ɐݒ肷�ׂ��ړ��c��

		mov eax, iEAXOfSettingMovePowerBushouUnitNo // eax�����ɖ߂�

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
	OnTenshouExeSettingMovePowerInBattleExecute();

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

		// ���X�̎c�ʕύX������������
		call pTenshouExeJumpCallFromToOnTenshouExeSettingMovePowerInBattle

		jmp pTenshouExeReturnLblFromOnTenshouExeSettingMovePowerInBattle
	}
}

char cmdOnTenshouExeJumpFromSettingMovePowerInBattle[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeSettingMovePowerInBattle() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeSettingMovePowerInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSettingMovePowerInBattle + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeSettingMovePowerInBattle  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromSettingMovePowerInBattle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSettingMovePowerInBattle), cmdOnTenshouExeJumpFromSettingMovePowerInBattle, 5, NULL); //5�o�C�g�̂ݏ�������
}




