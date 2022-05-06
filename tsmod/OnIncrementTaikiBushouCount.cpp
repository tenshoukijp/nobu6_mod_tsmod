#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "SetBushouAvgParam.h"
#include "ScenarioMod.h"


extern void EXSuperService_IfAbilityIsUnderdevelopedOnAlwaysMainGame(int iBushouID);
extern void EXSuperService_IfFixExperienceMaxOnAlwaysMainGame(int iBushouID);
extern void GeneralBushouBornExecute(); // �ėp�������a�������B

// �P�̃_���Ȋ����������
extern void ModifyWrongFaceDamegao(int iBushouID);


/*
00443DF2   56               |PUSH    ESI
00443DF3   E8 45000000      |CALL    TENSHOU.00443E3D   �� �ҋ@�����̒a���Ǝv����
00443DF8   83C4 04          |ADD     ESP, 4
00443DFB   85C0             |TEST    EAX, EAX
00443DFD  ^74 BA            |JE      SHORT TENSHOU.00443DB9
00443DFF   66:FF05 37FC4C00 |INC     WORD PTR DS:[4CFC37]  �� �ҋ@�����̓o�ꐔ�������鎞
00443E06   56               |PUSH    ESI
00443E07   E8 06CA0200      CALL    TENSHOU.00470812�@�@�� �኱�Ȃ��B���b�Z�[�W�Ƃ��o���̂��ȁH
00443E0C   83C4 04          |ADD     ESP, 4
*/


// �ҋ@�������a�����钼�O
bool isRequestSkipOfIncrementTaikiBushouCount = false;
int ESI_OF_IncrementTaikiBushouCount = -1;

// �X�L�b�v���邱�Ƃ��o���Ă����B
vector<int> registerSkippedOfIncrementTaikiBushouCount;


void OnTenshouExeIncrementTaikiBushouCountExecute() {
	// �X�L�b�v�̓f�t�H���g�ł͂����A�����Ƃ��̂܂ܒa��
	isRequestSkipOfIncrementTaikiBushouCount = false;

	int iBushouID = (ESI_OF_IncrementTaikiBushouCount & 0xFFFF) -1; // ESI�̂����ASI�̕�������

	if (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		/*
		NB6TAIKIBUSHOUNAME* p�ҋ@Array = GetTaikiBushouNameArrayPointer();
		OutputDebugStream("%d", (int)p�ҋ@Array);

		int b = getTouzyouBushouNum();
		OutputDebugStream("��");
		OutputDebugStream(p�ҋ@Array[b].familyname);
		OutputDebugStream(p�ҋ@Array[b].fastname);
		*/

		// ScenarioMod�ւƗ��j�����̒a����ʒm
		if (p_snOnSkippedRekishiBushou) {
			int isMustRegist = p_snOnSkippedRekishiBushou(iBushouID, getTouzyouBushouNum(), getTaikiBushouDataNum());

			if (isMustRegist == 0) {
				registerSkippedOfIncrementTaikiBushouCount.push_back(iBushouID);

				// �Y���X���b�g�͗��p�����A�����̓X�L�b�v���ׂ�
				isRequestSkipOfIncrementTaikiBushouCount = true;
			}
		}

	}

}


/*
00443DF2   56               |PUSH    ESI
00443DF3   E8 45000000      |CALL    TENSHOU.00443E3D   �� �ҋ@�����̒a���Ǝv����
�������� JMP TSMod.OnTSExeIncrementTaikiBushouCount �Ə��������Ă����ۂ̏���
00443DF8   83C4 04          |ADD     ESP, 4
00443DFB   85C0             |TEST    EAX, EAX

*/
int pTenshouExeJumpFromToOnTenshouExeIncrementTaikiBushouCount = 0x443DF3; // �֐��͂��̃A�h���X����AOnTenshouExeIncrementTaikiBushouCount�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeIncrementTaikiBushouCount = 0x443E3D; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeIncrementTaikiBushouCount = 0x443DF8; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
int pTenshouExeReturn2LblFromOnTenshouExeIncrementTaikiBushouCount = 0x443DFB; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec(naked) void WINAPI OnTenshouExeIncrementTaikiBushouCount() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov ESI_OF_IncrementTaikiBushouCount, ESI

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnTenshouExeIncrementTaikiBushouCountExecute();

	// �X�L�b�v����ׂ��ł���΁c
	if (isRequestSkipOfIncrementTaikiBushouCount) {

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

			// call�͂����Ɂc

			// ��ӂ₵����B���Ȃ킿�A���̑ҋ@�����̓o��̓X���[���ꂽ
			INC     WORD PTR DS : [GAMEDATASTRUCT_TOUZYOU_BUSHOU_NUM_ADDRESS] //  �� �ҋ@�����̓o�ꐔ�𑝂₷�B

			mov eax, 0

			jmp pTenshouExeReturn2LblFromOnTenshouExeIncrementTaikiBushouCount // �ʂ̏ꏊ(Return2)�ւƃW�����v
		}

	}
	else {

		// �ʏ�ʂ� �X�^�b�N�Ɉ����o��
		__asm {
			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			call pTenshouExeJumpCallFromToOnTenshouExeIncrementTaikiBushouCount

			jmp pTenshouExeReturnLblFromOnTenshouExeIncrementTaikiBushouCount
		}
	}
}





char cmdOnTenshouExeJumpFromIncrementTaikiBushouCount[6] = "\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeIncrementTaikiBushouCount() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeIncrementTaikiBushouCount;
	int SubAddress = iAddress - (pTenshouExeJumpFromToOnTenshouExeIncrementTaikiBushouCount + 5);
	// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeIncrementTaikiBushouCount  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromIncrementTaikiBushouCount + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory(CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeIncrementTaikiBushouCount), cmdOnTenshouExeJumpFromIncrementTaikiBushouCount, 5, NULL); //5�o�C�g�̂ݏ�������
}






















/*
00443DF2   56               |PUSH    ESI
00443DF3   E8 45000000      |CALL    TENSHOU.00443E3D   �� �ҋ@�����̒a���Ǝv����
00443DF8   83C4 04          |ADD     ESP, 4
00443DFB   85C0             |TEST    EAX, EAX
00443DFD  ^74 BA            |JE      SHORT TENSHOU.00443DB9
00443DFF   66:FF05 37FC4C00 |INC     WORD PTR DS:[4CFC37]  �� �ҋ@�����̓o�ꐔ�������鎞
00443E06   56               |PUSH    ESI
00443E07   E8 06CA0200      CALL    TENSHOU.00470812�@�@�� �኱�Ȃ��B���b�Z�[�W�Ƃ��o���̂��ȁH
00443E0C   83C4 04          |ADD     ESP, 4
*/

// �o�꒼��̑����i�K�ŏ������������Ƃ��o���Ă����B��\�����Ōォ��㏑�����Ă��܂�Ȃ��悤�ɁB
vector<int> registerModifiedOfIncrementTaikiBushouCount;

int ESI_OF_IncrementTaikiBushouCountAfter = -1;
void OnTenshouExeIncrementTaikiBushouCountAfterExecute() {

	int iBushouID = (ESI_OF_IncrementTaikiBushouCountAfter & 0xFFFF) - 1; // ESI�̂����ASI�̕�������

	if (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		// �P�̃_���狸��������B
		ModifyWrongFaceDamegao(iBushouID);

		// �Z�\�����J���ȏꍇ�̏���
		EXSuperService_IfAbilityIsUnderdevelopedOnAlwaysMainGame(iBushouID);

		// �����̏n���x���l�`�w�Œ�ł������ꍇ�̏���
		EXSuperService_IfFixExperienceMaxOnAlwaysMainGame(iBushouID);

		// ���V���[�Y�̕����̔\�͂𔽉f����
		SetBushouAvgParamAppearanceBushou(iBushouID);

		// ���X�g�ɒǉ�
		registerModifiedOfIncrementTaikiBushouCount.push_back(iBushouID);

		// ScenarioMod�ւƗ��j�����̒a����ʒm
		if (p_snOnBornRekishiBushou) {
			p_snOnBornRekishiBushou(iBushouID);
		}
	}
}


/*
00443DFF   66:FF05 37FC4C00 |INC     WORD PTR DS:[4CFC37]  �� �ҋ@�����̓o�ꐔ�������鎞
00443E06   56               |PUSH    ESI
00443E07   E8 06CA0200      CALL    TENSHOU.00470812�@�@�� �኱�Ȃ��B���b�Z�[�W�Ƃ��o���̂��ȁH
�������� JMP TSMod.OnTSExeIncrementTaikiBushouCountAfter �Ə��������Ă����ۂ̏���
00443E0C   83C4 04          |ADD     ESP, 4

*/
int pTenshouExeJumpFromToOnTenshouExeIncrementTaikiBushouCountAfter = 0x443E07; // �֐��͂��̃A�h���X����AOnTenshouExeIncrementTaikiBushouCountAfter�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeIncrementTaikiBushouCountAfter = 0x470812; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeIncrementTaikiBushouCountAfter = 0x443E0C; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec(naked) void WINAPI OnTenshouExeIncrementTaikiBushouCountAfter() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov ESI_OF_IncrementTaikiBushouCountAfter, ESI
			
		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnTenshouExeIncrementTaikiBushouCountAfterExecute();

	// �X�L�b�v����ׂ��ł���΁c
	// �ʏ�ʂ� �X�^�b�N�Ɉ����o��
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		call pTenshouExeJumpCallFromToOnTenshouExeIncrementTaikiBushouCountAfter

		jmp pTenshouExeReturnLblFromOnTenshouExeIncrementTaikiBushouCountAfter
	}
}





char cmdOnTenshouExeJumpFromIncrementTaikiBushouCountAfter[6] = "\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeIncrementTaikiBushouCountAfter() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeIncrementTaikiBushouCountAfter;
	int SubAddress = iAddress - (pTenshouExeJumpFromToOnTenshouExeIncrementTaikiBushouCountAfter + 5);
	// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeIncrementTaikiBushouCountAfter  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromIncrementTaikiBushouCountAfter + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory(CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeIncrementTaikiBushouCountAfter), cmdOnTenshouExeJumpFromIncrementTaikiBushouCountAfter, 5, NULL); //5�o�C�g�̂ݏ�������
}




