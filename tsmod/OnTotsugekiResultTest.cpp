#include "WinTarget.h"
#include <time.h>

#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"

/*
 * �푈�ɂ����āA�ˌ��n��������(NPC�܂�)�A�ˌ����ꂽ���̎��S����̑���
 */

int iTotsugekiAttackTestBushouID = -1; // �ˌ���������ID
int iTotsugekiDefendTestBushouID = -1; // �ˌ����ꂽ����ID
int iIsTotsugekiResultDeath = -1;	   // �ˌ��������ʁA���S�����̂����Ă��Ȃ��̂��B

void OnTenshouExeTotsugekiResultTestExecute() { 
	int iBushouID = iTotsugekiAttackTestBushouID - 1;
	int iTargetID = iTotsugekiDefendTestBushouID - 1;
	// �͈̓`�F�b�N
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM && 0 <= iTargetID && iTargetID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// �ˌ������̂̓����X���b�g�^���^���X�ł���B
		if (isLancelotTartare(iBushouID)) {

			// n/50�̊m��(n=TSMod.txt)�ő���͎��S�B�P�ˌ�������Ȃ̂ŁAn=3�Ȃ�A�����1/10�قǂ̊m���Ŏ��ʁB
			if (rand() % 30 == 0) {
				iIsTotsugekiResultDeath = 1;
			}

		// �ˌ������͎̂�ɂł���B
		} else if (isShuiHidechiyo(iBushouID)) {

			// n/50�̊m��(n=TSMod.txt)�ő���͎��S�B�P�ˌ�������Ȃ̂ŁAn=3�Ȃ�A�����1/10�قǂ̊m���Ŏ��ʁB
			if (rand() % 30 == 0) {
				iIsTotsugekiResultDeath = 1;
			}

		// �ˌ����̃W���u���R�m�ł���A�R�n���R�n�S�C�ł���
		} else if ( nb6bushouref[iBushouID].job == 0xA0 && (nb6bushouref[iBushouID].form == 1 || nb6bushouref[iBushouID].form == 3 ) ) {
			// ���葤�̃W���u���㋉����
			if (nb6bushouref[iTargetID].job < 0xA0) {
				if ( rand() % 30 == 0 ) {
					iIsTotsugekiResultDeath = 1;
				}
			}
		}

		// ���肪�����X���b�g�^���^���X�Ȃ�ˌ��ɂ�鎀�S���Ȃ��B
		if ( isLancelotTartare( iTargetID ) ) {
			iIsTotsugekiResultDeath = 0;
		} 

		// ���肪��ɂȂ�ˌ��ɂ�鎀�S���Ȃ��B
		else if (isShuiHidechiyo(iTargetID)) {
			iIsTotsugekiResultDeath = 0;
		}

		// �֐���DLL�Œ�`����Ă�����ACONDITION�Ƃ��ČĂяo���A���̌��ʂ�TRUE�ł���΁A���S�����AFALSE�ł���΁A���S���Ȃ��B
		// ScenarioMod�œ��ɉ������Ȃ��Ƃ������Ƃ��\�Ƃ��邽�߁A
		if ( p_snOnCustomCondition ) {																						          // ���ˌ�������               ���ˌ����ꂽ��                 �� �\�肳��Ă鎀�S����
			int isScenarioModDeathResult = ((PFNSNONCUSTOMCONDITION) p_snOnCustomCondition)("AS����::�푈::�ˌ��󓮎Ҏ��S��", iTotsugekiAttackTestBushouID, iTotsugekiDefendTestBushouID, iIsTotsugekiResultDeath, "", "", 0.0, 0.0);
			// ��C���v�j�łȂ���΁c
			if ( isScenarioModDeathResult != CUSTOM_CONDITION_NOMODIFY ) {
				iIsTotsugekiResultDeath = isScenarioModDeathResult;
			}
		}
	}
}
/* 
004324A6  |. E8 6AF1FFFF    |CALL    TENSHOU.00431615                ; \TENSHOU.00431615�@���ˌ��n
004324AB  |. 83C4 0C        |ADD     ESP, 0C
004324AE  |. 85C0           |TEST    EAX, EAX							EAX��1�Ȃ瑊�肪���S�����B�d�a�w������̕����ԍ�

*/ 
int pTenshouExeJumpFromToOnTenshouExeTotsugekiResultTest =0x4324A6; 
int pTenshouExeJumpCallFromToOnTenshouExeTotsugekiResultTest=0x431615; 
int pTenshouExeReturnLblFromOnTenshouExeTotsugekiResultTest =0x4324AB; 
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeTotsugekiResultTest() { 
	__asm { 
		// TENSHOU.EXE����call
		call pTenshouExeJumpCallFromToOnTenshouExeTotsugekiResultTest 

		// 
		mov iIsTotsugekiResultDeath, eax				// EAX��1�������Ă���΁A���蕐���͎��S����
		mov iTotsugekiAttackTestBushouID, edi			// �ˌ��������̕���ID 
		mov iTotsugekiDefendTestBushouID, ebx			// �ˌ����ꂽ���̕���ID 
		push eax 
		push ebx 
		push ecx 
		push edx 
		push esp 
		push ebp 
		push esi 
		push edi 
	} 

	OnTenshouExeTotsugekiResultTestExecute(); 

	__asm { 
		pop edi 
		pop esi 
		pop ebp 
		pop esp 
		pop edx 
		pop ecx 
		pop ebx 
		pop eax 

		// ���삳�ꂽ�ˌ����ʂɂ�鎀�S���茋�ʂ�eax�ɖ߂��B
		mov eax, iIsTotsugekiResultDeath 

		jmp pTenshouExeReturnLblFromOnTenshouExeTotsugekiResultTest 
	} 
} 
char cmdOnTenshouExeJumpFromTotsugekiResultTest[6]="\xE9"; 
void WriteAsmJumperOnTenshouExeTotsugekiResultTest() { 
	int iAddress = (int)OnTenshouExeTotsugekiResultTest; 
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeTotsugekiResultTest + 5 ) ; 
	memcpy(cmdOnTenshouExeJumpFromTotsugekiResultTest+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B 
	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeTotsugekiResultTest), cmdOnTenshouExeJumpFromTotsugekiResultTest, 5, NULL); // 5�o�C�g��������
} 