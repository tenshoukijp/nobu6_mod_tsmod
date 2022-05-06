#include "WinTarget.h"

#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXBushou_All.h"
#include "ScenarioMod.h"

/*
 * �푈���Ƀ��j�b�g����U�����悤�Ƃ���Ƃ��ɁA�u�S�C���v���A�������́u�S�C�R�n���v�ł��邩�𔻒肷��B
 * �ǂ��炩�ł���΃����[�g�U�����\�����炾�B
 */


/*
 * 
 */
//
extern int isBushouIDJudgeRemoteAttack;
int isJudgeCanRemoteAttackValue = -1;   // ���ʂƂ��ă����[�g�U�����̂����ǂ����B����ŃW�����v���U�蕪����

int iTargetHeikaOfJudgeTeppouOrTeppouTaiExecute = -1;   // ���ʂƂ��ă����[�g�U�����̂����ǂ����B����ŃW�����v���U�蕪����

void OnTenshouExeJudgeTeppouOrTeppouTaiExecute() {

	isJudgeCanRemoteAttackValue = iTargetHeikaOfJudgeTeppouOrTeppouTaiExecute==2 ? 1 : 0;		  // �����[�g�A�^�b�N�͍ŏ��̒l�̂܂܁B

	int iBushouID = (isBushouIDJudgeRemoteAttack & 0xFFFF) -1;

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {
		// �S�C���Ɍ���AScenarioMod�ŁA�S�C�U���s�\�Ƃ����㏑�����\
		if ( nb6bushouref[iBushouID].form == 2 ) {

			isJudgeCanRemoteAttackValue = 1; // �����[�g�U���\�B

			// ScenarioMod�œV��₻�̂ق��Ɋւ�炸�A�S�C�����Ă邩�ۂ��̏�����ύX���Ă��܂��B
			if ( p_snOnCustomCondition ) {
				// ���C����ʂł̓����̐���																			�����s��    
				int isCanTeppouResult = ((PFNSNONCUSTOMCONDITION) p_snOnCustomCondition)("OR����::�푈::�S�C�s��", iBushouID+1, 0,   0, "", "", 0.0, 0.0);
				if ( isCanTeppouResult == TRUE ) {
					isJudgeCanRemoteAttackValue = 0; // �S�C��s�Ƃ���
				}
			}
		}
	}
}


/*
0042B431  |. 85C0           TEST    EAX, EAX
0042B433  |. 74 14          JE      SHORT TENSHOU.0042B449
0042B435  |. 3B5D F0        CMP     EBX, DWORD PTR SS:[EBP-10] ��EDI�̕����̓S�C�̓K����EBX�ɃR�s�[
0042B438  |. 72 0F          JB      SHORT TENSHOU.0042B449
0042B43A  |. 837D EC 02     CMP     DWORD PTR SS:[EBP-14], 2 ���S�C�����ǂ����̔���
							�������� JMP TSMod.OnTSExeJudgeTeppouOrTeppouTai �Ə��������Ă����ۂ̏���
0042B43E  |. 74 06          JE      SHORT TENSHOU.0042B446
0042B440  |. 837D EC 03     CMP     DWORD PTR SS:[EBP-14], 3 ���R�n�S�C�����ǂ����̔���
0042B444  |. 75 03          JNZ     SHORT TENSHOU.0042B449  ���R�n�S�C���ł��Ȃ��Ȃ�΁c
0042B446  |> FE45 FA        INC     BYTE PTR SS:[EBP-6]
*/
int pTenshouExeJumpFromToOnTenshouExeJudgeTeppouOrTeppouTai	=0x42B43A; // �֐��͂��̃A�h���X����AOnTenshouExeJudgeTeppouOrTeppouTai�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeJudgeTeppouOrTeppouTai	= 0x42B440; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w

int pTenshouExeReturnRemoteJmpFromOnTenshouExeJudgeTeppouOrTeppouTai	=0x42B446; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B

__declspec( naked ) void WINAPI OnTenshouExeJudgeTeppouOrTeppouTai() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov isBushouIDJudgeRemoteAttack, EDI // EDI�Ɍ��ݑΏۂƂȂ镐���ԍ�������B���ʃr�b�g�ɍi���Ă�������

		// ���ł�EDI���W�X�^���Ԏ؂肵�āA���Ȓl��ۑ����Ă���
		mov EDI, DWORD PTR SS:[EBP-0x14]

		mov iTargetHeikaOfJudgeTeppouOrTeppouTaiExecute, EDI

		// EDI�����ւƖ߂��Ă���
		mov EDI, isBushouIDJudgeRemoteAttack

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	OnTenshouExeJudgeTeppouOrTeppouTaiExecute();

	// �����[�g�U�����\�Ȃ̂ł���΁c
	if ( isJudgeCanRemoteAttackValue ) {
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

			// ���̔���������ɓ����
			CMP     DWORD PTR SS:[EBP-0x14], 2 //  ���S�C�����ǂ����̔���

			jmp pTenshouExeReturnRemoteJmpFromOnTenshouExeJudgeTeppouOrTeppouTai;
		}

	// �����[�g�U�����s�\�Ȓʏ��ԂȂ̂ł���΁c
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

			// ���̔���������ɓ����
			CMP     DWORD PTR SS:[EBP-0x14], 2 //  ���S�C�����ǂ����̔���

			jmp pTenshouExeReturnLblFromOnTenshouExeJudgeTeppouOrTeppouTai // �����[�g�U�����s�\�Ȃ�Ώ������΂��B�JNZ SHORT TENSHOU.0042B449�  ����
		}
	}
}





char cmdOnTenshouExeJumpFromJudgeTeppouOrTeppouTai[7]="\xE9\x90\x90\x90\x90\x90";
// ���̖��߂�6�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�U�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeJudgeTeppouOrTeppouTai() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeJudgeTeppouOrTeppouTai;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeJudgeTeppouOrTeppouTai + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeJudgeTeppouOrTeppouTai  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromJudgeTeppouOrTeppouTai+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeJudgeTeppouOrTeppouTai), cmdOnTenshouExeJumpFromJudgeTeppouOrTeppouTai, 6, NULL); //6�o�C�g�̂ݏ�������
}







void OnTenshouExeJudgeTeppouOrTeppouKibaExecute() {

	isJudgeCanRemoteAttackValue = iTargetHeikaOfJudgeTeppouOrTeppouTaiExecute==3 ? 1 : 0;		  // �����[�g�A�^�b�N�͍ŏ��̒l�̂܂܁B

	int iBushouID = (isBushouIDJudgeRemoteAttack & 0xFFFF)-1;
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		//����(0=���y,1=�R�n,2=�S�C,3=�R�n�S�C,18�܂�)

		if ( nb6bushouref[iBushouID].form == 3 ) {
			isJudgeCanRemoteAttackValue = 1; // �����[�g�U���\�B

		// ���y�ŋ|���a�ȏ�ł���΁c
		} else if ( nb6bushouref[iBushouID].form == 0 && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3) { // ���y�ŋ|���a�ȏ�ł���΁c
			isJudgeCanRemoteAttackValue = 1;


		// �R�n�ŋ|���a�ȏ�ł���΁c
		// �R�n�K�����a�ȏ�ŁA�|�K�����a�ȏ�Ȃ�΁A�����[�g�U���\
		} else if ( nb6bushouref[iBushouID].form == 1 && nb6bushouref[iBushouID].aptitudekib >= 3 && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3) {
			isJudgeCanRemoteAttackValue = 1;
		}

		// �R�n�S�C���Ɍ���AScenarioMod�ŁA�S�C�U���s�\�Ƃ����㏑�����\
		if ( nb6bushouref[iBushouID].form == 3 ) {

			// ScenarioMod�œV��₻�̂ق��Ɋւ�炸�A�S�C�����Ă邩�ۂ��̏�����ύX���Ă��܂��B
			if ( p_snOnCustomCondition ) {
				// ���C����ʂł̓����̐���																			�����s��    
				int isCanTeppouResult = ((PFNSNONCUSTOMCONDITION) p_snOnCustomCondition)("OR����::�푈::�S�C�s��", iBushouID+1, 0,   0, "", "", 0.0, 0.0);
				if ( isCanTeppouResult == TRUE ) {
					isJudgeCanRemoteAttackValue = 0; // �S�C��s�Ƃ���
				}
			}
		}
	}
}






/*
0042B431  |. 85C0           TEST    EAX, EAX
0042B433  |. 74 14          JE      SHORT TENSHOU.0042B449
0042B435  |. 3B5D F0        CMP     EBX, DWORD PTR SS:[EBP-10] ��EDI�̕����̓S�C�̓K����EBX�ɃR�s�[
0042B438  |. 72 0F          JB      SHORT TENSHOU.0042B449
0042B43A  |. 837D EC 02     CMP     DWORD PTR SS:[EBP-14], 2 ���S�C�����ǂ����̔���
0042B43E  |. 74 06          JE      SHORT TENSHOU.0042B446
0042B440  |. 837D EC 03     CMP     DWORD PTR SS:[EBP-14], 3 ���R�n�S�C�����ǂ����̔���
							�������� JMP TSMod.OnTSExeJudgeTeppouOrTeppouKiba �Ə��������Ă����ۂ̏���
0042B444  |. 75 03          JNZ     SHORT TENSHOU.0042B449  ���R�n�S�C�����ł��Ȃ��Ȃ�΁c
0042B446  |> FE45 FA        INC     BYTE PTR SS:[EBP-6]
*/
int pTenshouExeJumpFromToOnTenshouExeJudgeTeppouOrTeppouKiba	=0x42B440; // �֐��͂��̃A�h���X����AOnTenshouExeJudgeTeppouOrTeppouKiba�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeJudgeTeppouOrTeppouKiba	=0x42B446; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w

int pTenshouExeReturnRemoteJmpFromOnTenshouExeJudgeTeppouOrTeppouKiba	=0x42B449; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeJudgeTeppouOrTeppouKiba() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov isBushouIDJudgeRemoteAttack, EDI // EDI�Ɍ��ݑΏۂƂȂ镐���ԍ�������B���ʃr�b�g�ɍi���Ă�������

		// ���ł�EDI���W�X�^���Ԏ؂肵�āA���Ȓl��ۑ����Ă���
		mov EDI, DWORD PTR SS:[EBP-0x14]

		mov iTargetHeikaOfJudgeTeppouOrTeppouTaiExecute, EDI

		// EDI�����ւƖ߂��Ă���
		mov EDI, isBushouIDJudgeRemoteAttack

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	OnTenshouExeJudgeTeppouOrTeppouKibaExecute();

	// �����[�g�U�����\�Ȃ̂ł���΁c
	if ( isJudgeCanRemoteAttackValue ) {
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

			// ���̔���������ɓ����
			CMP     DWORD PTR SS:[EBP-0x14], 3 //  ���R�n�S�C�����ǂ����̔���

			jmp pTenshouExeReturnLblFromOnTenshouExeJudgeTeppouOrTeppouKiba;
		}

	// �����[�g�U�����s�\�Ȓʏ��ԂȂ̂ł���΁c
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

			// ���̔���������ɓ����
			CMP     DWORD PTR SS:[EBP-0x14], 3 //  ���R�n�S�C�����ǂ����̔���

			jmp pTenshouExeReturnRemoteJmpFromOnTenshouExeJudgeTeppouOrTeppouKiba // �����[�g�U�����s�\�Ȃ�Ώ������΂��B�JNZ SHORT TENSHOU.0042B449�  ����
		}
	}
}





char cmdOnTenshouExeJumpFromJudgeTeppouOrTeppouKiba[7]="\xE9\x90\x90\x90\x90\x90";
// ���̖��߂�6�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�U�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeJudgeTeppouOrTeppouKiba() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeJudgeTeppouOrTeppouKiba;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeJudgeTeppouOrTeppouKiba + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeJudgeTeppouOrTeppouKiba  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromJudgeTeppouOrTeppouKiba+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeJudgeTeppouOrTeppouKiba), cmdOnTenshouExeJumpFromJudgeTeppouOrTeppouKiba, 6, NULL); //6�o�C�g�̂ݏ�������
}


