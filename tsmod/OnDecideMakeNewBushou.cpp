#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"
#include "ScenarioMod.h"

// �V�K�����쐬�Łu����v������

/*
00497983  |. E8 4F3FFEFF    CALL    TENSHOU.0047B8D7  �� �����̕Ԃ�l��0�Ȃ�u����v�A1�Ȃ�u���s�v EBX�������ԍ���
00497988  |. 8D8D 08FFFFFF  LEA     ECX, DWORD PTR SS:[EBP-F8]
0049798E  |. 8945 F0        MOV     DWORD PTR SS:[EBP-10], EAX
00497991  |. C705 84154D00 >MOV     DWORD PTR DS:[4D1584], 0FFFF
*/

extern int aptitude_coef();

extern int iBudhouNodOfPathDDrawHimekaoApeearBug;

int iNoOfNewBushouExecute = -1;
int iIsDecideOfNewBushouExecute = 0; // �u���s�v:1 �u����v:0

void OnTenshouExeDecideMakeNewBushouExecute() {

	if (iIsDecideOfNewBushouExecute) {

		int iBushouID = iNoOfNewBushouExecute - 1;

		if (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {
			// OutputDebugString(nb6bushouname[iBushouID].fastname);

			int yumi = aptitude_coef() - 1;
			int yari = aptitude_coef() - 1;

			// �|��TSMod.txt�̐ݒ�ɂ͊֌W�Ȃ��ݒ肷��B
			// �|�̓K�����R�s�[
			// �|�������Ƃ����t���O��ON�Ȃ̂ł���΁A0�ŏ㏑������B
			if (TSModCommand::World::cmdTheYumiNoExists) {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = 0;
			}
			else {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = yumi;
			}
			// ����TSMod.txt�̐ݒ�ɂ͊֌W�Ȃ��ݒ肷��B
			// ���̓K�����R�s�[
			nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari = yari;

			// ���ꑮ���́A��ʕ����͎������킹�Ȃ��̂ŁA�S�ăN���A
			nb6savedata_ex.bushou_ex[iBushouID].aptitudeono = 0;
			nb6savedata_ex.bushou_ex[iBushouID].aptitudemuchi = 0;
			nb6savedata_ex.bushou_ex[iBushouID].aptitudetuchi = 0;
			nb6savedata_ex.bushou_ex[iBushouID].aptitudetue = 0;
			nb6savedata_ex.bushou_ex[iBushouID].aptitudeken = 0;

			// ScenarioMod�ւ̉��z�����a���ʒm
			if (p_snOnBornGeneralBushou) {
				p_snOnBornGeneralBushou(iBushouID);
			}

		}
	}
}


/*
00497983  |. E8 4F3FFEFF    CALL    TENSHOU.0047B8D7  �� �����̕Ԃ�l��0�Ȃ�u����v�A1�Ȃ�u���s�v EBX�������ԍ�
�������� JMP TSMod.OnTSExeDecideMakeNewBushou �Ə��������Ă����ۂ̏���
00497988  |. 8D8D 08FFFFFF  LEA     ECX, DWORD PTR SS:[EBP-F8]
0049798E  |. 8945 F0        MOV     DWORD PTR SS:[EBP-10], EAX
00497991  |. C705 84154D00 >MOV     DWORD PTR DS:[4D1584], 0FFFF
*/

int pTenshouExeJumpFromToOnTenshouExeDecideMakeNewBushou = 0x497983; // �֐��͂��̃A�h���X����AOnTenshouExeDecideMakeNewBushou�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeDecideMakeNewBushou = 0x47B8D7; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeDecideMakeNewBushou = 0x497988; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec(naked) void WINAPI OnTenshouExeDecideMakeNewBushou() {

	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iBudhouNodOfPathDDrawHimekaoApeearBug, EBX

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeDecideMakeNewBushou

		mov iBudhouNodOfPathDDrawHimekaoApeearBug, -1

		// EAX�́u���s�v�Ȃ�1�A�u����v�Ȃ�0
		mov iIsDecideOfNewBushouExecute, EAX

		// �����Ă��������ԍ���ۑ�
		mov iNoOfNewBushouExecute, EBX

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
	OnTenshouExeDecideMakeNewBushouExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeDecideMakeNewBushou
	}
}

/*
0049D7AA  |. 6A 03          |PUSH    3                               ; |Arg3 = 00000003
0049D7AC  |. 56             |PUSH    ESI                             ; |Arg2
0049D7AD  |. 56             |PUSH    ESI                             ; |Arg1
0049D7AE  |. E8 D3540000    |CALL    TENSHOU.004A2C86                ; \TENSHOU.004A2C86
�������� JMP TSMod.OnTSExeDecideMakeNewBushou �Ə��������Ă����ۂ̏���
0049D7B3  |. 83C4 1C        |ADD     ESP, 1C
0049D7B6  |. 85C0           |TEST    EAX, EAX
*/
char cmdOnTenshouExeJumpFromDecideMakeNewBushou[6] = "\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeDecideMakeNewBushou() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeDecideMakeNewBushou;
	int SubAddress = iAddress - (pTenshouExeJumpFromToOnTenshouExeDecideMakeNewBushou + 5);
	// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeDecideMakeNewBushou  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromDecideMakeNewBushou + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory(CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeDecideMakeNewBushou), cmdOnTenshouExeJumpFromDecideMakeNewBushou, 5, NULL); //5�o�C�g�̂ݏ�������
}




