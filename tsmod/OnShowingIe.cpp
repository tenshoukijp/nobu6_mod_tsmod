
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXBushou_All.h"
#include "ScenarioMod.h"
#include "TSModCommand.h"


static int iEAXOfShowingIe1 = -1;
static int iBushouBangouOfShowingIe1 = -1;
void OnTenshouExeShowingIe1Execute() {

	int iBushouID = (iBushouBangouOfShowingIe1 & 0xFFFF) - 1; // �������������ԍ�

	if (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		if (p_snOnViewingIe) {
			bool isUpdate = p_snOnViewingIe(iBushouID+1, (char *)iEAXOfShowingIe1);
			// OutputDebugStream << nb6bushouname[iBushouID].fastname << endl;
			// OutputDebugStream << (int)isUpdate << endl;
		}

	}
}

/*
0040D226  |. 68 ACB84B00    PUSH    TENSHOU.004BB8AC                 ;  ASCII "��"
0040D22B  |. 56             PUSH    ESI
0040D22C  |. E8 DCFF0900    CALL    TENSHOU.004AD20D					// EDI�̉��ʃr�b�g��EDI 004D00BD TENSHOU.004D00BD
0040D231  |. 83C4 08        ADD     ESP, 8
*/

int pTenshouExeJumpFromToOnTenshouExeShowingIe1     = 0x40D22C; // �֐��͂��̃A�h���X����AOnTenshouExeShowingIe1�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeShowingIe1 = 0x4AD20D; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeShowingIe1  = 0x40D231; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec(naked) void WINAPI OnTenshouExeShowingIe1() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ������������̏���������
		mov iEAXOfShowingIe1, EAX;

		// EDI�̉��ʂɕ����̔ԍ��B
		mov iBushouBangouOfShowingIe1, EDI;

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeShowingIe1

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
	OnTenshouExeShowingIe1Execute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingIe1
	}
}

char cmdOnTenshouExeJumpFromShowingIe1[6] = "\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�W�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeShowingIe1() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeShowingIe1;
	int SubAddress = iAddress - (pTenshouExeJumpFromToOnTenshouExeShowingIe1 + 5);
	// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromShowingIe1 + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory(CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingIe1), cmdOnTenshouExeJumpFromShowingIe1, 5, NULL); //5�o�C�g�̂ݏ�������
}













static int iEAXOfShowingIe2 = -1;
static int iBushouBangouOfShowingIe2 = -1;
void OnTenshouExeShowingIe2Execute() {

	int iBushouID = (iBushouBangouOfShowingIe2 & 0xFFFF) - 1; // �������������ԍ�

	if (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {
		if (p_snOnViewingIe) {
			bool isUpdate = p_snOnViewingIe(iBushouID+1, (char *)iEAXOfShowingIe2);
			// OutputDebugStream << nb6bushouname[iBushouID].fastname << endl;
			// OutputDebugStream << (int)isUpdate << endl;
		}
	}
}

/*
0040D2A6  |. 68 ACB84B00    PUSH    TENSHOU.004BB8AC                 ;  ASCII "��"
0040D2AB  |. 57             PUSH    EDI
0040D2AC  |. E8 5CFF0900    CALL    TENSHOU.004AD20D                    // EBP�̉��ʃr�b�g��EBP 001900BD TENSHOU.001900BD
0040D2B1  |. 83C4 08        ADD     ESP, 8
*/

int pTenshouExeJumpFromToOnTenshouExeShowingIe2     = 0x40D2AC; // �֐��͂��̃A�h���X����AOnTenshouExeShowingIe2�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeShowingIe2 = 0x4AD20D; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeShowingIe2  = 0x40D2B1; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec(naked) void WINAPI OnTenshouExeShowingIe2() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// ������������̏���������
		mov iEAXOfShowingIe2, EAX;

		// EDI�̉��ʂɕ����̔ԍ��B
		mov iBushouBangouOfShowingIe2, EBP;

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeShowingIe2

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
	OnTenshouExeShowingIe2Execute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingIe2
	}
}

char cmdOnTenshouExeJumpFromShowingIe2[6] = "\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�W�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeShowingIe2() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeShowingIe2;
	int SubAddress = iAddress - (pTenshouExeJumpFromToOnTenshouExeShowingIe2 + 5);
	// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromShowingIe2 + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory(CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingIe2), cmdOnTenshouExeJumpFromShowingIe2, 5, NULL); //5�o�C�g�̂ݏ�������
}




