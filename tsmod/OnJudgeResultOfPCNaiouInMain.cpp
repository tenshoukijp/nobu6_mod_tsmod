#include "WinTarget.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"
#include "EXBushou_All.h"
#include "ScenarioMod.h"

/*
�����̑O�ɖ�O�������ǂ����B
00402897  |. 85C0           TEST    EAX, EAX


0040288F  |. E8 8D1B0500    CALL    TENSHOU.00454421�@�� ��������s������AESI�Ɏd�|�������AEBX�Ɏd�|����ꂽ���̕����ԍ��B
													  �@ ����́APC/NPC�ŕς��Ȃ��B
00402894  |. 83C4 04        ADD     ESP, 4


004028BF  |. 53             PUSH    EBX
004028C0  |. 56             PUSH    ESI
004028C1  |. E8 62FDFFFF    CALL    TENSHOU.00402628 PC�̏ꍇ   ���̊֐����Ăяo����EDI�Ɍ��ʂ̒l������
004028C6  |. 83C4 08        ADD     ESP, 8

00402901  |. E8 612D0A00    CALL    TENSHOU.004A5667 NPC�̏ꍇ�@���̊֐����Ăяo����EDI�Ɍ��ʂ̒l������B
00402906  |. 83C4 04        ADD     ESP, 4


����̌��ʁAEDI�̒l�Ő��ۂ̃����N�����܂�B
EDI=0 ���̏�ő����ɏo�z
EDI=1 �����̎��t���ɐ���
EDI=2 ���s�����B����ɔے肳�ꂽ�B
EDI=3 �t�ɓ�������Ă��܂��A�o�z���Ă������B(�喼��R�c�����o�z����Ə�񂪋����̂Œ���)
EDI=4 ���s�����B�t�Ɏ�ɕs���͂Ȃ��̂��H �ƌ���ꂽ�B
*/

int GetJudgeResultOfNaiouInMain(int iActiveBushou, int iPassiveBushou, int iResult) {
	if ( 0 <= iActiveBushou  && iActiveBushou  < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= iPassiveBushou && iPassiveBushou < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ScenarioMod�̃J�X�^�������ŌR�t��ݒ肷��
		if ( p_snOnCustomCondition ) {
			// ���C����ʂł̓����̐���																							�����s��	     ���󓮎�	    �����ݗ\�肳��Ă��錋�� 
			int isScenarioModNaiouResult = ((PFNSNONCUSTOMCONDITION) p_snOnCustomCondition)("AS����::���C��::�������ʉ�", iActiveBushou+1, iPassiveBushou+1, iResult, "", "", 0.0, 0.0);
			if ( isScenarioModNaiouResult != CUSTOM_CONDITION_NOMODIFY ) {

				// �����̐��ۂ��㏑������B
				return isScenarioModNaiouResult;
			}
		}

		// �������󂯂鑤�̗��ꂪ��ʂ̏h�V�`���y��
		if ( nb6bushouname[iPassiveBushou].position >= 2  &&   // �h�V�`���y
			 nb6bushouname[iPassiveBushou].State == 2 ) {      // ���ꂪ�u��ʁv�ł���B

			// �������鑤�������X���b�g�E�^���^���X�Ȃ�1/3�̊m���ł������ܑ�����o�z������B
			if ( isLancelotTartare(iActiveBushou) ) {
				if ( rand() % 3 == 0 ) {
					return 0; // 1/3�̊m���Ō��ʂ�听���ɏ㏑���B
				}
			}
			// �������鑤���o�[���[�t�H���͂���ɍI�݂��B1/2�̊m���ł������ܑ�����o�z������B
			else if ( strcmp( nb6bushouname[iActiveBushou].familyname, "�I�J�K") == 0 && strcmp( nb6bushouname[iActiveBushou].fastname, "�L�M�N") == 0 ) {
				if ( rand() % 2 == 0 ) {
					return 0; // 1/2�̊m���Ō��ʂ�听���ɏ㏑���B
				}
			}
		}

	}

	return -1; // ���̒l�̂܂�
}




static int iActiveBushouOfJudgeResultOfPCNaiouInMain = -1;
static int iPassiveBushouOfJudgeResultOfPCNaiouInMain = -1;
static int iResultOfJudgeResultOfPCNaiouInMain = 1; // ����

void OnTenshouExeJudgeResultOfPCNaiouInMainExecute() {
	int iActiveBushou  = iActiveBushouOfJudgeResultOfPCNaiouInMain - 1;   // �d�|������
	int iPassiveBushou = iPassiveBushouOfJudgeResultOfPCNaiouInMain - 1;  // �d�|����ꂽ��

	int iOverRide = GetJudgeResultOfNaiouInMain(iActiveBushou, iPassiveBushou, iResultOfJudgeResultOfPCNaiouInMain);
	if (iOverRide != -1) {
		iResultOfJudgeResultOfPCNaiouInMain = iOverRide;
	}
}


/*
004028BF  |. 53             PUSH    EBX
004028C0  |. 56             PUSH    ESI
004028C1  |. E8 62FDFFFF    CALL    TENSHOU.00402628 PC�̏ꍇ   ���̊֐����Ăяo����EDI�Ɍ��ʂ̒l������
				�������� JMP TSMod.OnTSExeJudgeResultOfPCNaiouInMain �Ə��������Ă����ۂ̏���
004028C6  |. 83C4 08        ADD     ESP, 8
*/
int pTenshouExeJumpFromToOnTenshouExeJudgeResultOfPCNaiouInMain	    =0x4028C1; // �֐��͂��̃A�h���X����AOnTenshouExeJudgeResultOfPCNaiouInMain�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeJudgeResultOfPCNaiouInMain =0x402628; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeJudgeResultOfPCNaiouInMain  =0x4028C6; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeJudgeResultOfPCNaiouInMain() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iActiveBushouOfJudgeResultOfPCNaiouInMain, ESI // �d�|���鑤
		mov iPassiveBushouOfJudgeResultOfPCNaiouInMain, EBX // �d�|�����鑤

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeJudgeResultOfPCNaiouInMain

		mov iResultOfJudgeResultOfPCNaiouInMain, EDI
		/*
		EDI=0 ���̏�ő����ɏo�z
		EDI=1 �����̎��t���ɐ���
		EDI=2 ���s�����B����ɔے肳�ꂽ�B
		EDI=3 �t�ɓ�������Ă��܂��A�o�z���Ă������B(�喼��R�c�����o�z����Ə�񂪋����̂Œ���)
		EDI=4 ���s�����B�t�Ɏ�ɕs���͂Ȃ��̂��H �ƌ���ꂽ�B
		*/

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnTenshouExeJudgeResultOfPCNaiouInMainExecute();

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

		mov EDI, iResultOfJudgeResultOfPCNaiouInMain // ���ʂ̏㏑��

		jmp pTenshouExeReturnLblFromOnTenshouExeJudgeResultOfPCNaiouInMain
	}
}



char cmdOnTenshouExeJumpFromJudgeResultOfPCNaiouInMain[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeJudgeResultOfPCNaiouInMain() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeJudgeResultOfPCNaiouInMain;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeJudgeResultOfPCNaiouInMain + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeJudgeResultOfPCNaiouInMain  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromJudgeResultOfPCNaiouInMain+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeJudgeResultOfPCNaiouInMain), cmdOnTenshouExeJumpFromJudgeResultOfPCNaiouInMain, 5, NULL); //5�o�C�g�̂ݏ�������
}


