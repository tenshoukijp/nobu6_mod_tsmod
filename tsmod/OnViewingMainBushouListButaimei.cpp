#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

#include "SetOverWriteButaiMei.h"


/*
 * ���C����ʂ̕������X�g���J�����Ƃ�����
 */ 


/*
0043919C   . E8 E80CFDFF    CALL    TENSHOU.00409E89
004391A1   . 83C4 04        ADD     ESP, 4
004391A4   . 8B0485 80134D0>MOV     EAX, DWORD PTR DS:[EAX*4+4D1380] ;  TENSHOU.004BBC2C
004391AB   . EB 0A          JMP     SHORT TENSHOU.004391B7
*/

// �\�����悤�Ƃ��Ă��镐���ԍ�
int iBushouIDOnTenshouExeViewingMainBushouListButaimeiExecute = -1;
extern int pOrgButaiMeiArrayOnExecute; // ���XTENSHOU.EXE���ɂ�����CALL��

extern int pNewButaiMeiArrayOnExecute;
extern int pOrgButaiFormNo;

void OnTenshouExeViewingMainBushouListButaimeiExecute() {
	// ESI�̂����A����SI�̕��������������ԍ��ɑ�������
	iBushouIDOnTenshouExeViewingMainBushouListButaimeiExecute = iBushouIDOnTenshouExeViewingMainBushouListButaimeiExecute & 0xFFFF;

	int iBushouID = iBushouIDOnTenshouExeViewingMainBushouListButaimeiExecute - 1;

	pOrgButaiMeiArrayOnExecute = 0x4D1380;

	// �������z��|�C���^���㏑������B
	int p = GetOverWriteButaiMeiArrayPointer(iBushouID);
	if ( p ) {
		pNewButaiMeiArrayOnExecute = p + pOrgButaiFormNo*4;

	} else {
		// ���������x���͕K���I���W�i���ŏ㏑�������B
		pNewButaiMeiArrayOnExecute = 0x4D1380 + pOrgButaiFormNo*4;
	}
}



/*
// ���̒i�K��ESI�ɕ����ԍ��������Ă��邪�A���ۂ̕����ԍ���SI����(���ʃr�b�g)
0043919C   . E8 E80CFDFF    CALL    TENSHOU.00409E89
				�������� JMP TSMod.OnTSExeViewingMainBushouListButaimei �Ə��������Ă����ۂ̏���
004391A1   . 83C4 04        ADD     ESP, 4
004391A4   . 8B0485 80134D0>MOV     EAX, DWORD PTR DS:[EAX*4+4D1380] ;  TENSHOU.004BBC2C
004391AB   . EB 0A          JMP     SHORT TENSHOU.004391B7
*/
int pTenshouExeJumpFromToOnTenshouExeViewingMainBushouListButaimei	   = 0x43919C; // �֐��͂��̃A�h���X����AOnTenshouExeViewingMainBushouListButaimei�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeViewingMainBushouListButaimei = 0x409E89; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeViewingMainBushouListButaimei  = 0x4391AB; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeViewingMainBushouListButaimei() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���̏����������ɋL��
		call pTenshouExeJumpCallFromToOnTenshouExeViewingMainBushouListButaimei

		// ���̏���
		ADD     ESP, 4

		// nb6bushou[iBushouID].form ������ۑ�
		mov pOrgButaiFormNo, eax

		// �����ԍ��̕ۑ�
		mov iBushouIDOnTenshouExeViewingMainBushouListButaimeiExecute, esi

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
	OnTenshouExeViewingMainBushouListButaimeiExecute();

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

		// pNewButaiMeiArrayOnExecute�͏��Ȃ��Ƃ��I���W�i�����A�㏑�����ꂽ�Achar *[4]�̔z��B
		mov eax, pNewButaiMeiArrayOnExecute
		mov eax, [eax]

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingMainBushouListButaimei
	}
}




char cmdOnTenshouExeJumpFromViewingMainBushouListButaimei[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v5�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeViewingMainBushouListButaimei() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeViewingMainBushouListButaimei;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingMainBushouListButaimei + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingMainBushouListButaimei  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromViewingMainBushouListButaimei+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingMainBushouListButaimei), cmdOnTenshouExeJumpFromViewingMainBushouListButaimei, 5, NULL); //5�o�C�g�̂ݏ�������
}




