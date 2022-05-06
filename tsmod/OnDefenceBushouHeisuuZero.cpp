#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"


// �ދp�������͕ߔ��t�߂�ύX����

static int iDefenceBushouIDOnHeisuZero = -1; // �������O�ɂȂ����ދp���邱�ƂɂȂ���������iBushouID
static int iAttackBushouIDOnHeisuZero = -1; // ��̎��ɍU�������l��iBushouID
static int iIsOverrideDeathFuncOnHeisuZero = false;


namespace  �U�����@ {
	enum { �U�� = 0, �ˌ� = 1, �S�C = 2, ��C = 3, ��čU�� = 4, �| = 7 };
}
void OnTenshouExeDefenceBushouHeisuZeroExecute(int i�U�����@�ԍ�) {
	iIsOverrideDeathFuncOnHeisuZero = false;

	int iAttackID = iAttackBushouIDOnHeisuZero & 0xFFFF - 1;
	int iDefendID = iDefenceBushouIDOnHeisuZero & 0xFFFF - 1;
	if (0 <= iAttackID && iAttackID < GAMEDATASTRUCT_BUSHOU_NUM &&
		0 <= iDefendID && iDefendID < GAMEDATASTRUCT_BUSHOU_NUM) {
		// ���łɑދp�����S
		if (nb6bushouname[iDefendID].battleRetreat || nb6bushouname[iDefendID].battleDeath) {
			return;
		}

		if (nb6settingsex.deathfreq == TRUE) {
			// OutputDebugStream << "�U��" << nb6bushouname[iAttackID].familyname << endl;
			// OutputDebugStream << "�h��" << nb6bushouname[iDefendID].familyname << endl;

			// �U�������̂̓����X���b�g�^���^���X�ł���B
			if (isLancelotTartare(iAttackID)) {

				// �R���g���[�����������U���B����͎��ȂȂ�
				iIsOverrideDeathFuncOnHeisuZero = false;


			// �U�������͎̂�ɂł���B
			} else if (isShuiHidechiyo(iAttackID)) {

				// �R���g���[�����������U���B����͎��ȂȂ�
				iIsOverrideDeathFuncOnHeisuZero = false;

			}

			// �U�����̃W���u���㋉�A�Ȃ�A�U�����󂯂����̎��S������������
			else if (nb6bushouref[iAttackID].job >= 0xA0) {
				// ���葤�̃W���u���㋉����
				if (nb6bushouref[iDefendID].job < 0xA0) {
					// 75�`-75���炢
					int coef = getBushouBat(iAttackID) - getBushouBat(iDefendID);
					coef = coef / 20; // 3�`-3���炢
					coef = 5 - coef;  // 2�`8���炢
					if (coef && rand() % coef == 0) {
						iIsOverrideDeathFuncOnHeisuZero = true;
					}
				}
			}
			// ���A��ʂȂ�A�㋉�̔����̊m��
			else {
				// ���葤�̃W���u���㋉�ȊO
				if (nb6bushouref[iDefendID].job < 0xA0) {
					// 75�`-75���炢
					int coef = getBushouBat(iAttackID) - getBushouBat(iDefendID);
					coef = coef / 20; // 3�`-3���炢
					coef = 5 - coef;  // 2�`8���炢
					// OutputDebugStream << coef*2 << endl;
					if (coef && rand() % coef == 0) {
						if (rand() % 2 == 0) { // 1/4�`1/16���炢
							iIsOverrideDeathFuncOnHeisuZero = true;
						}
					}
				}
			}

			// �h�䑤�������X���b�g�^���^���X�Ȃ玀�S���Ȃ��B
			if (isLancelotTartare(iDefendID)) {
				iIsOverrideDeathFuncOnHeisuZero = false;
			}

			// �h�䑤����ɂȂ玀�S���Ȃ��B
			if (isShuiHidechiyo(iDefendID)) {
				iIsOverrideDeathFuncOnHeisuZero = false;
			}

		}

		// �֐���DLL�Œ�`����Ă�����ACONDITION�Ƃ��ČĂяo���A���̌��ʂ�TRUE�ł���΁A���S�����AFALSE�ł���΁A���S���Ȃ��B
		// ScenarioMod�œ��ɉ������Ȃ��Ƃ������Ƃ��\�Ƃ��邽�߁A
		if (p_snOnCustomCondition) {																						      // ���U��������  ���U�����ꂽ��    �� �\�肳��Ă鎀�S����
			int isScenarioModDeathResult = ((PFNSNONCUSTOMCONDITION)p_snOnCustomCondition)("AS����::�푈::��펀�U���󓮎ҕ��m���������S��", iAttackID + 1, iDefendID, iIsOverrideDeathFuncOnHeisuZero, "", "", (float)i�U�����@�ԍ�, 0.0);
			// ��C���v�j�łȂ���΁c
			if (isScenarioModDeathResult != CUSTOM_CONDITION_NOMODIFY) {
				iIsOverrideDeathFuncOnHeisuZero = isScenarioModDeathResult;
			}
		}
	}
}

void OnTenshouExeDefenceBushouHeisuZeroInTujouExecute() {
	OnTenshouExeDefenceBushouHeisuZeroExecute(�U�����@::�U��);
}

/*
// �ʏ�U���̌��ʁA�u�U�����󂯂������������O�ƂȂ�A�ߔ��������͑ދp�v�����ƂȂ�����
00432020  |> 56             PUSH    ESI
00432021  |. 57             PUSH    EDI
00432022  |> E8 71F5FFFF    CALL    TENSHOU.00431598 �@// �ދp�������͕ߔ�����������֐�
00432027  |. 83C4 08        ADD     ESP, 8
*/
int pTenshouExeJumpFromToOnTenshouExeDefenceBushouHeisuZeroTujou = 0x432022;
int pTenshouExeJumpCallTaikyakuFuncFromToOnTenshouExeDefenceBushouHeisuZeroTujou = 0x431598; // ���X�̑ދp�p�̊֐�
int pTenshouExeJumpCallDeathFuncFromToOnTenshouExeDefenceBushouHeisuZeroTujou = 0x4316B8; // �����^�C�~���O�ł�������ĂԂƎ��S����
int pTenshouExeReturnLblFromOnTenshouExeDefenceBushouHeisuZeroTujou = 0x432027;
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec(naked) void WINAPI OnTenshouExeDefenceBushouHeisuZeroTujou() {
	__asm {
		mov iDefenceBushouIDOnHeisuZero, EDI			// �������
		mov iAttackBushouIDOnHeisuZero, ESI			// �U������
		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnTenshouExeDefenceBushouHeisuZeroInTujouExecute();

	// ���Ɏw�肪�Ȃ��܂܂ł������B
	if (iIsOverrideDeathFuncOnHeisuZero == false) {
		__asm {

			// �X�^�b�N����
			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			// ���̊֐����Ă�
			call pTenshouExeJumpCallTaikyakuFuncFromToOnTenshouExeDefenceBushouHeisuZeroTujou

			jmp pTenshouExeReturnLblFromOnTenshouExeDefenceBushouHeisuZeroTujou
		}
	}
	else {
		__asm {
			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			// ���S�̊֐����Ă�
			call pTenshouExeJumpCallDeathFuncFromToOnTenshouExeDefenceBushouHeisuZeroTujou

			jmp pTenshouExeReturnLblFromOnTenshouExeDefenceBushouHeisuZeroTujou
		}
	}

}
char cmdOnTenshouExeJumpFromDefenceBushouHeisuZeroTujou[6] = "\xE9";
void WriteAsmJumperOnTenshouExeDefenceBushouHeisuZeroTujou() {
	int iAddress = (int)OnTenshouExeDefenceBushouHeisuZeroTujou;
	int SubAddress = iAddress - (pTenshouExeJumpFromToOnTenshouExeDefenceBushouHeisuZeroTujou + 5);
	memcpy(cmdOnTenshouExeJumpFromDefenceBushouHeisuZeroTujou + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B 
	WriteProcessMemory(CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeDefenceBushouHeisuZeroTujou), cmdOnTenshouExeJumpFromDefenceBushouHeisuZeroTujou, 5, NULL); // �ꉞ���S�̂��� 
}

















/*
// ��čU���̌��ʁA�u�U�����󂯂������������O�ƂȂ�A�ߔ��������͑ދp�v�����ƂȂ�����
004321D0 | .FF75 08        PUSH    DWORD PTR SS : [EBP + 8]
004321D3 | .FF75 0C        PUSH    DWORD PTR SS : [EBP + C]
004321D6 | .E8 BDF3FFFF    CALL    TENSHOU.00431598
004321DB | . 83C4 08        ADD     ESP, 8
*/
void OnTenshouExeDefenceBushouHeisuZeroInIsseiExecute() {
	OnTenshouExeDefenceBushouHeisuZeroExecute(�U�����@::��čU��);
}

int EAXOfDefenceBushouHeisuZeroInIssei = 0; // �ꎞ�ۑ��p

int pTenshouExeJumpFromToOnTenshouExeDefenceBushouHeisuZeroInIssei = 0x4321D6;
int pTenshouExeJumpCallTaikyakuFuncFromToOnTenshouExeDefenceBushouHeisuZeroInIssei = 0x431598; // ���X�̑ދp�p�̊֐�
int pTenshouExeJumpCallDeathFuncFromToOnTenshouExeDefenceBushouHeisuZeroInIssei = 0x4316B8; // �����^�C�~���O�ł�������ĂԂƎ��S����
int pTenshouExeReturnLblFromOnTenshouExeDefenceBushouHeisuZeroInIssei = 0x4321DB;
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec(naked) void WINAPI OnTenshouExeDefenceBushouHeisuZeroInIssei() {
	__asm {
		mov EAXOfDefenceBushouHeisuZeroInIssei, EAX // ��U�ޔ�

		mov EAX, [EBP + 0xC]
		mov iDefenceBushouIDOnHeisuZero, EAX			// �������
		mov EAX, [EBP + 0x8]
		mov iAttackBushouIDOnHeisuZero, EAX			// �U������

		mov EAX, EAXOfDefenceBushouHeisuZeroInIssei // ��ւƖ߂�

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnTenshouExeDefenceBushouHeisuZeroInIsseiExecute();

	// ���Ɏw�肪�Ȃ��܂܂ł������B
	if (iIsOverrideDeathFuncOnHeisuZero == false) {
		__asm {

			// �X�^�b�N����
			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			// ���̊֐����Ă�
			call pTenshouExeJumpCallTaikyakuFuncFromToOnTenshouExeDefenceBushouHeisuZeroInIssei

			jmp pTenshouExeReturnLblFromOnTenshouExeDefenceBushouHeisuZeroInIssei
		}
	}
	else {
		__asm {
			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			// ���S�̊֐����Ă�
			call pTenshouExeJumpCallDeathFuncFromToOnTenshouExeDefenceBushouHeisuZeroInIssei

			jmp pTenshouExeReturnLblFromOnTenshouExeDefenceBushouHeisuZeroInIssei
		}
	}

}
char cmdOnTenshouExeJumpFromDefenceBushouHeisuZeroInIssei[6] = "\xE9";
void WriteAsmJumperOnTenshouExeDefenceBushouHeisuZeroInIssei() {
	int iAddress = (int)OnTenshouExeDefenceBushouHeisuZeroInIssei;
	int SubAddress = iAddress - (pTenshouExeJumpFromToOnTenshouExeDefenceBushouHeisuZeroInIssei + 5);
	memcpy(cmdOnTenshouExeJumpFromDefenceBushouHeisuZeroInIssei + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B 
	WriteProcessMemory(CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeDefenceBushouHeisuZeroInIssei), cmdOnTenshouExeJumpFromDefenceBushouHeisuZeroInIssei, 5, NULL); // �ꉞ���S�̂��� 
}








/*
// ��čU���̌��ʁA�u�U�����󂯂������������O�ƂȂ�A�ߔ��������͑ދp�v�����ƂȂ������A���[�v��
00432236 | .FF75 0C | PUSH    DWORD PTR SS : [EBP + C]
00432239 | .FF75 F6 | PUSH    DWORD PTR SS : [EBP - A]
0043223C | .E8 57F3FFFF | CALL    TENSHOU.00431598
00432241 | . 83C4 08 | ADD     ESP, 8
*/
int EAXOfDefenceBushouHeisuZeroInIsseiLoop = 0; // �ꎞ�ۑ��p
int pTenshouExeJumpFromToOnTenshouExeDefenceBushouHeisuZeroInIsseiLoop = 0x43223C;
int pTenshouExeJumpCallTaikyakuFuncFromToOnTenshouExeDefenceBushouHeisuZeroInIsseiLoop = 0x431598; // ���X�̑ދp�p�̊֐�
int pTenshouExeJumpCallDeathFuncFromToOnTenshouExeDefenceBushouHeisuZeroInIsseiLoop = 0x4316B8; // �����^�C�~���O�ł�������ĂԂƎ��S����
int pTenshouExeReturnLblFromOnTenshouExeDefenceBushouHeisuZeroInIsseiLoop = 0x432241;
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec(naked) void WINAPI OnTenshouExeDefenceBushouHeisuZeroInIsseiLoop() {
	__asm {
		mov EAXOfDefenceBushouHeisuZeroInIsseiLoop, EAX // ��U�ޔ�

		mov EAX, [EBP - 0xA]
		mov iDefenceBushouIDOnHeisuZero, EAX			// �������
		mov EAX, [EBP + 0xC]
		mov iAttackBushouIDOnHeisuZero, EAX			// �U������

		mov EAX, EAXOfDefenceBushouHeisuZeroInIsseiLoop // ��ւƖ߂�

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnTenshouExeDefenceBushouHeisuZeroInIsseiExecute();

	// ���Ɏw�肪�Ȃ��܂܂ł������B
	if (iIsOverrideDeathFuncOnHeisuZero == false) {
		__asm {

			// �X�^�b�N����
			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			// ���̊֐����Ă�
			call pTenshouExeJumpCallTaikyakuFuncFromToOnTenshouExeDefenceBushouHeisuZeroInIsseiLoop

			jmp pTenshouExeReturnLblFromOnTenshouExeDefenceBushouHeisuZeroInIsseiLoop
		}
	}
	else {
		__asm {
			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			// ���S�̊֐����Ă�
			call pTenshouExeJumpCallDeathFuncFromToOnTenshouExeDefenceBushouHeisuZeroInIsseiLoop

			jmp pTenshouExeReturnLblFromOnTenshouExeDefenceBushouHeisuZeroInIsseiLoop
		}
	}

}
char cmdOnTenshouExeJumpFromDefenceBushouHeisuZeroInIsseiLoop[6] = "\xE9";
void WriteAsmJumperOnTenshouExeDefenceBushouHeisuZeroInIsseiLoop() {
	int iAddress = (int)OnTenshouExeDefenceBushouHeisuZeroInIsseiLoop;
	int SubAddress = iAddress - (pTenshouExeJumpFromToOnTenshouExeDefenceBushouHeisuZeroInIsseiLoop + 5);
	memcpy(cmdOnTenshouExeJumpFromDefenceBushouHeisuZeroInIsseiLoop + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B 
	WriteProcessMemory(CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeDefenceBushouHeisuZeroInIsseiLoop), cmdOnTenshouExeJumpFromDefenceBushouHeisuZeroInIsseiLoop, 5, NULL); // �ꉞ���S�̂��� 
}







/*
// �S�C�U���̌��ʁA�u�U�����󂯂������������O�ƂȂ�A�ߔ��������͑ދp�v�����ƂȂ������A
00432925 | . 56             PUSH    ESI
00432926 | . 57             PUSH    EDI
00432927 | .E8 6CECFFFF    CALL    TENSHOU.00431598
0043292C | . 83C4 08        ADD     ESP, 8

OnTenshouExeJudgeTeppouDeathInBattle�ŏ�������̂ł����ł͉������Ȃ�
*/
void OnTenshouExeDefenceBushouHeisuZeroInTeppouExecute() {
	iIsOverrideDeathFuncOnHeisuZero = false;
}

int pTenshouExeJumpFromToOnTenshouExeDefenceBushouHeisuZeroInTeppou = 0x432927;
int pTenshouExeJumpCallTaikyakuFuncFromToOnTenshouExeDefenceBushouHeisuZeroInTeppou = 0x431598; // ���X�̑ދp�p�̊֐�
int pTenshouExeJumpCallDeathFuncFromToOnTenshouExeDefenceBushouHeisuZeroInTeppou = 0x4316B8; // �����^�C�~���O�ł�������ĂԂƎ��S����
int pTenshouExeReturnLblFromOnTenshouExeDefenceBushouHeisuZeroInTeppou = 0x43292C;
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec(naked) void WINAPI OnTenshouExeDefenceBushouHeisuZeroInTeppou() {
	__asm {
		mov iDefenceBushouIDOnHeisuZero, EDI			// �������
			mov iAttackBushouIDOnHeisuZero, ESI			// �U������
			push eax
			push ebx
			push ecx
			push edx
			push esp
			push ebp
			push esi
			push edi
	}

	OnTenshouExeDefenceBushouHeisuZeroInTeppouExecute();

	// ���Ɏw�肪�Ȃ��܂܂ł������B
	if (iIsOverrideDeathFuncOnHeisuZero == false) {
		__asm {

			// �X�^�b�N����
			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			// ���̊֐����Ă�
			call pTenshouExeJumpCallTaikyakuFuncFromToOnTenshouExeDefenceBushouHeisuZeroInTeppou

			jmp pTenshouExeReturnLblFromOnTenshouExeDefenceBushouHeisuZeroInTeppou
		}
	}
	else {
		__asm {
			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			// ���S�̊֐����Ă�
			call pTenshouExeJumpCallDeathFuncFromToOnTenshouExeDefenceBushouHeisuZeroInTeppou

			jmp pTenshouExeReturnLblFromOnTenshouExeDefenceBushouHeisuZeroInTeppou
		}
	}

}
char cmdOnTenshouExeJumpFromDefenceBushouHeisuZeroInTeppou[6] = "\xE9";
void WriteAsmJumperOnTenshouExeDefenceBushouHeisuZeroInTeppou() {
	int iAddress = (int)OnTenshouExeDefenceBushouHeisuZeroInTeppou;
	int SubAddress = iAddress - (pTenshouExeJumpFromToOnTenshouExeDefenceBushouHeisuZeroInTeppou + 5);
	memcpy(cmdOnTenshouExeJumpFromDefenceBushouHeisuZeroInTeppou + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B 
	WriteProcessMemory(CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeDefenceBushouHeisuZeroInTeppou), cmdOnTenshouExeJumpFromDefenceBushouHeisuZeroInTeppou, 5, NULL); // �ꉞ���S�̂��� 
}





/* ��C�U���̌��ʁA�u�U�����󂯂������������O�ƂȂ�A�ߔ��������͑ދp�v�����ƂȂ������A���[�v��
00432BE8 | . 56 || PUSH    ESI
00432BE9 | . 53 || PUSH    EBX
00432BEA | .E8 A9E9FFFF || CALL    TENSHOU.00431598
00432BEF | .E9 85000000 || JMP     TENSHOU.00432C79
*/
void OnTenshouExeDefenceBushouHeisuZeroInTaihouExecute() {
	iIsOverrideDeathFuncOnHeisuZero = false;
}

int pTenshouExeJumpFromToOnTenshouExeDefenceBushouHeisuZeroInTaihou = 0x432BEA;
int pTenshouExeJumpCallTaikyakuFuncFromToOnTenshouExeDefenceBushouHeisuZeroInTaihou = 0x431598; // ���X�̑ދp�p�̊֐�
int pTenshouExeJumpCallDeathFuncFromToOnTenshouExeDefenceBushouHeisuZeroInTaihou = 0x4316B8; // �����^�C�~���O�ł�������ĂԂƎ��S����
int pTenshouExeReturnLblFromOnTenshouExeDefenceBushouHeisuZeroInTaihou = 0x432BEF;
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec(naked) void WINAPI OnTenshouExeDefenceBushouHeisuZeroInTaihou() {
	__asm {
		mov iDefenceBushouIDOnHeisuZero, EBX			// �������
		mov iAttackBushouIDOnHeisuZero, ESI			// �U������
		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnTenshouExeDefenceBushouHeisuZeroInTaihouExecute();

	// ���Ɏw�肪�Ȃ��܂܂ł������B
	if (iIsOverrideDeathFuncOnHeisuZero == false) {
		__asm {

			// �X�^�b�N����
			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			// ���̊֐����Ă�
			call pTenshouExeJumpCallTaikyakuFuncFromToOnTenshouExeDefenceBushouHeisuZeroInTaihou

			jmp pTenshouExeReturnLblFromOnTenshouExeDefenceBushouHeisuZeroInTaihou
		}
	}
	else {
		__asm {
			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			// ���S�̊֐����Ă�
			call pTenshouExeJumpCallDeathFuncFromToOnTenshouExeDefenceBushouHeisuZeroInTaihou

			jmp pTenshouExeReturnLblFromOnTenshouExeDefenceBushouHeisuZeroInTaihou
		}
	}

}
char cmdOnTenshouExeJumpFromDefenceBushouHeisuZeroInTaihou[6] = "\xE9";
void WriteAsmJumperOnTenshouExeDefenceBushouHeisuZeroInTaihou() {
	int iAddress = (int)OnTenshouExeDefenceBushouHeisuZeroInTaihou;
	int SubAddress = iAddress - (pTenshouExeJumpFromToOnTenshouExeDefenceBushouHeisuZeroInTaihou + 5);
	memcpy(cmdOnTenshouExeJumpFromDefenceBushouHeisuZeroInTaihou + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B 
	WriteProcessMemory(CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeDefenceBushouHeisuZeroInTaihou), cmdOnTenshouExeJumpFromDefenceBushouHeisuZeroInTaihou, 5, NULL); // �ꉞ���S�̂��� 
}



/* 
�ˌ��̌��ʁA�u�U�����󂯂������������O�ƂȂ�A�ߔ��������͑ދp�v�����ƂȂ�����
00432694  |. 83C4 04        ADD     ESP, 4
00432697  |. 837D F0 01     CMP     DWORD PTR SS:[EBP-10], 1 // ���S��񂪓����Ă�ꏊ
0043269B  |. 57             PUSH    EDI
0043269C  |. 53             PUSH    EBX
0043269D  |. 75 07          JNZ     SHORT TENSHOU.004326A6   // ����łȂ���Εߔ��������͑ދp��
0043269F  |. E8 14F0FFFF    CALL    TENSHOU.004316B8         // ���S�֐�
004326A4  |. EB 05          JMP     SHORT TENSHOU.004326AB   // �ߔ��������͑ދp�̓p�X
004326A6  |> E8 EDEEFFFF    CALL    TENSHOU.00431598         // �ߔ��������͑ދp�֐�
004326AB  |> 83C4 08        ADD     ESP, 8

OnTenshouExeTotsugekiResultTest�@�ő��삷��̂ł����ł͉������Ȃ�
*/



/*
�푈���݂Ă��Ȃ����ɁA�u�U�����󂯂������������O�ƂȂ�A�ߔ��������͑ދp�v�����ƂȂ�����
004351CC  /$ FF7424 08      PUSH    DWORD PTR SS:[ESP+8]
004351D0  |. FF7424 08      PUSH    DWORD PTR SS:[ESP+8]
004351D4  |. E8 BFC3FFFF    CALL    TENSHOU.00431598
004351D9  |. 83C4 08        ADD     ESP, 8

�푈���݂Ă��Ȃ����ɁA�u�U�����󂯂����������펀�v�����ƂȂ�����
004351DD  /$ FF7424 08      PUSH    DWORD PTR SS:[ESP+8]             ; /Arg2
004351E1  |. FF7424 08      PUSH    DWORD PTR SS:[ESP+8]             ; |Arg1
004351E5  |. E8 CEC4FFFF    CALL    TENSHOU.004316B8                 ; \TENSHOU.004316B8
004351EA  |. 83C4 08        ADD     ESP, 8
004351ED  \. C3             RETN
*/