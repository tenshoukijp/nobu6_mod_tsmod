
#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"

/*
00446B72   E8 14640600      CALL    TENSHOU.004ACF8B	�� ��������������̂��A���΂��Ȃ̂��A���l���{��̂����߂�֐��B���̂Ƃ��AEDI�́A���ɍs���������ԍ�
00446B77   83C4 04          ADD     ESP, 4
00446B7A   85C0             TEST    EAX, EAX
*/


/*
* ���l�Ȃ�΁A���ʂ̐����͖񑩂���Ă���B
*/




int iCurBushouIDInCommerceSuccessJudge = -1;
int nSuccessJudgeInCommerceSuccessJudge = 0;
void OnTenshouExeCommerceSuccessJudgeExecute() {

	// ���������͈̔͂ł��邱��
	if (0 <= iCurBushouIDInCommerceSuccessJudge - 1 && iCurBushouIDInCommerceSuccessJudge - 1 < GAMEDATASTRUCT_BUSHOU_NUM) {

		// ���l�ł���΁c ��΂̐������񑩂���Ă���B
		if (nb6bushouref[iCurBushouIDInCommerceSuccessJudge - 1].job == 0x70) {
			nSuccessJudgeInCommerceSuccessJudge = 2; // �听��

			// OutputDebugString("���������ł̑听��\n");
		}
	}
}


/*
00446B72   E8 14640600      CALL    TENSHOU.004ACF8B	�� ��������������̂��A���΂��Ȃ̂��A���l���{��̂����߂�֐��B���̂Ƃ��AEDI�́A���ɍs���������ԍ�
�������� JMP TSMod.OnTSExeCommerceSuccessJudge �Ə��������Ă����ۂ̏���
00446B77   83C4 04          ADD     ESP, 4
00446B7A   85C0             TEST    EAX, EAX
*/
int pTenshouExeJumpFromToOnTenshouExeCommerceSuccessJudge = 0x446B72; // �֐��͂��̃A�h���X����AOnTenshouExeCommerceSuccessJudge�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeCommerceSuccessJudge = 0x4ACF8B; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeCommerceSuccessJudge = 0x446B77; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec(naked) void WINAPI OnTenshouExeCommerceSuccessJudge() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iCurBushouIDInCommerceSuccessJudge, edi

		call pTenshouExeJumpCallFromToOnTenshouExeCommerceSuccessJudge

		mov nSuccessJudgeInCommerceSuccessJudge, eax // 0:�����ĂȂ� 1:�����΂� 2:������ 4:�����s

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
	OnTenshouExeCommerceSuccessJudgeExecute();

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

		// ���l���ۂ��̌��ʂ������āA0�`4��2(=����)�ɏ㏑��
		mov eax, nSuccessJudgeInCommerceSuccessJudge

		jmp pTenshouExeReturnLblFromOnTenshouExeCommerceSuccessJudge
	}
}



char cmdOnTenshouExeJumpFromCommerceSuccessJudge[6] = "\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeCommerceSuccessJudge() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeCommerceSuccessJudge;
	int SubAddress = iAddress - (pTenshouExeJumpFromToOnTenshouExeCommerceSuccessJudge + 5);
	// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeCommerceSuccessJudge  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromCommerceSuccessJudge + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory(CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeCommerceSuccessJudge), cmdOnTenshouExeJumpFromCommerceSuccessJudge, 5, NULL); //5�o�C�g�̂ݏ�������
}


























/*
 * ���Ǝ��ɏ��l�p�̃��b�Z�[�W�B
 */


int iCurBushouIDInCommerceMessageJudge = -1;
bool isUseSyoninMsgInCommerceSuccessJudge = false;
void OnTenshouExeCommerceMessageJudgeExecute() {

	// �����l�p�̃��b�Z�[�W�ւƕύX���ׂ����ǂ����͂܂�false;
	isUseSyoninMsgInCommerceSuccessJudge = false;

	// ���������͈̔͂ł��邱��
	if (0 <= iCurBushouIDInCommerceMessageJudge - 1 && iCurBushouIDInCommerceMessageJudge - 1 < GAMEDATASTRUCT_BUSHOU_NUM) {

		// ���l�ł���΁c ��΂̐������񑩂���Ă���B
		if (nb6bushouref[iCurBushouIDInCommerceMessageJudge - 1].job == 0x70) {
			isUseSyoninMsgInCommerceSuccessJudge = true;
		}
	}
}


/*
00446F75  |. 66:A3 907C4D00 MOV     WORD PTR DS:[4D7C90], AX
00446F7B  |. B8 B21F0000    MOV     EAX, 1FB2
�������� JMP TSMod.OnTSExeCommerceMessageJudge �Ə��������Ă����ۂ̏���
00446F80  |. 75 0E          JNZ     SHORT TENSHOU.00446F90
00446F82  |. 83E9 04        SUB     ECX, 4
*/
int pTenshouExeJumpFromToOnTenshouExeCommerceMessageJudge = 0x446F7B; // �֐��͂��̃A�h���X����AOnTenshouExeCommerceMessageJudge�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeCommerceMessageJudge = 0x446F80; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec(naked) void WINAPI OnTenshouExeCommerceMessageJudge() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		mov eax, 0x1FB2

		// ����ID��ۑ�
		mov iCurBushouIDInCommerceMessageJudge, edx

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
	OnTenshouExeCommerceMessageJudgeExecute();

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

		test esi, esi

		jmp pTenshouExeReturnLblFromOnTenshouExeCommerceMessageJudge
	}
}




char cmdOnTenshouExeJumpFromCommerceMessageJudge[6] = "\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeCommerceMessageJudge() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeCommerceMessageJudge;
	int SubAddress = iAddress - (pTenshouExeJumpFromToOnTenshouExeCommerceMessageJudge + 5);
	// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeCommerceMessageJudge  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromCommerceMessageJudge + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory(CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeCommerceMessageJudge), cmdOnTenshouExeJumpFromCommerceMessageJudge, 5, NULL); //5�o�C�g�̂ݏ�������
}












//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------











/*
* ���Ǝ��ɏ��l�Ȃ�΁A���ʂ̐����͖񑩂���Ă���B
*/

/*
// ����ID��ۑ�
int iCurBushouIDInCommerceAngryJudge = -1;
bool isAngryMsgForceControlCommerceAngryJudge = false; // ���l�̓{����ΐ���

void OnTenshouExeCommerceAngryJudgeExecuteBefore() {

	// �{��̒��ڂ̃R���g���[��
	isAngryMsgForceControlCommerceAngryJudge = false;

	if (0 <= iCurBushouIDInCommerceAngryJudge - 1 && iCurBushouIDInCommerceAngryJudge - 1 < GAMEDATASTRUCT_BUSHOU_NUM) {

		// ���������l
		if (nb6bushouref[iCurBushouIDInCommerceAngryJudge - 1].job == 0x70) {
			// �{��R���g���[�����[�h��ON��
			isAngryMsgForceControlCommerceAngryJudge = true;
		}

	}
}

void OnTenshouExeCommerceAngryJudgeExecuteAfter() {

	// �{�胂�[�h��ON�̎������A���O�̒l�Ƃ̔�r�ȂǁA����������Ȃ�
	if (isAngryMsgForceControlCommerceAngryJudge) {
		// ���������l
		if (nb6bushouref[iCurBushouIDInCommerceAngryJudge - 1].job == 0x70) {

			isAngryMsgForceControlCommerceAngryJudge = true; // ����͕s�v

			// ���l�Ƃ̗F�D���傫���Ȃ��Ă��Ȃ��Ȃ�A���̒l���P�傫������
			int iDaimyoID = nb6bushouref[iCurBushouIDInCommerceAngryJudge - 1].attach - 1;

			// �܂��A���l���{���ĕs�݂ȂǂƂȂ��Ă����Ƃ�����A����͋����C��
			nb6daimyo[iDaimyoID].bMerchantAngry = 0;
		}
	}
}
*/

/*
004471F6   51               PUSH    ECX
004471F7   FF55 0C          CALL    DWORD PTR SS:[EBP+C]
�������� JMP TSMod.OnTSExeCommerceAngryJudge �Ə��������Ă����ۂ̏���
004471FA   83C4 10          ADD     ESP, 10
004471FD   85C0             TEST    EAX, EAX		EAX��0�Ȃ�{��
*/

/*

int pTenshouExeJumpFromToOnTenshouExeCommerceAngryJudge = 0x4471F7; // �֐��͂��̃A�h���X����AOnTenshouExeCommerceAngryJudge�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeCommerceAngryJudge = 0x4471FD; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec(naked) void WINAPI OnTenshouExeCommerceAngryJudge() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// ����ID��ۑ�
		mov iCurBushouIDInCommerceAngryJudge, ecx
			
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
	OnTenshouExeCommerceAngryJudgeExecuteBefore();

	// ���l�̓{����ΐ���
	if (isAngryMsgForceControlCommerceAngryJudge) {
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

			// ���X�̎��s���Ăяo��
			CALL    DWORD PTR SS : [EBP + 0xC]

			ADD     ESP, 0x10

			push eax
			push ebx
			push ecx
			push edx
			push esp
			push ebp
			push esi
			push edi
		}

		OnTenshouExeCommerceAngryJudgeExecuteAfter();
		
		__asm {

			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			// ���l�͌����ē{��Ȃ�
			mov eax, 1

			// ���̃t���O�͍~�낵�Ă���
			mov isAngryMsgForceControlCommerceAngryJudge, 0

			jmp pTenshouExeReturnLblFromOnTenshouExeCommerceAngryJudge
		}

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

			// ���X�̎��s���Ăяo��
			CALL    DWORD PTR SS : [EBP + 0xC]

			ADD     ESP, 0x10

			// ���̃t���O�͍~�낵�Ă���
			mov isAngryMsgForceControlCommerceAngryJudge, 0

			jmp pTenshouExeReturnLblFromOnTenshouExeCommerceAngryJudge
		}
	}
}

char cmdOnTenshouExeJumpFromCommerceAngryJudge[7] = "\xE9\x90\x90\x90\x90\x90";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeCommerceAngryJudge() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeCommerceAngryJudge;
	int SubAddress = iAddress - (pTenshouExeJumpFromToOnTenshouExeCommerceAngryJudge + 5);
	// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeCommerceAngryJudge  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromCommerceAngryJudge + 1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory(CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeCommerceAngryJudge), cmdOnTenshouExeJumpFromCommerceAngryJudge, 6, NULL); //6�o�C�g�̂ݏ�������
}
*/




