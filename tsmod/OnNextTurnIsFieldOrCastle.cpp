#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"


int iNextTurnIsFieldOrCastle = -1;		// ���Ȃ�0�A�����Ȃ�1
extern int iNextTurnCastleID;	// ����̃^�[������̏ꍇ�̏�ԍ��B��łȂ��ꍇ�ɂ́A0xFFFF������B
void OnTenshouExeNextTurnIsFieldOrCastleExecute() {
	// ���͖��ł���B
	if ( iNextTurnIsFieldOrCastle == 0 ) {
		iNextTurnCastleID = 0xFFFF;

		// ��킾�����̂ŁA���̂܂�ScenarioMod�ɓ`�B
		if ( p_snSetCurrentCastleIDInBattle ) {
			((PFNSNSETCURRENTCASTLEIDINBATTLE) p_snSetCurrentCastleIDInBattle)(iNextTurnCastleID);
		}
	}

	
}


/*
0046B575 . E8 A502FEFF CALL TENSHOU.0044B81F			���Ȃ��Ƃ�EAX�ɁA��킩����킩�̏�񂪓��� 
				�������� JMP TSMod.OnTSExeNextTurnIsFieldOrCastle �Ə��������Ă����ۂ̏���
0046B57A . 83C4 0C ADD ESP, 0C 
0046B57D . 85C0 TEST EAX, EAX							EAX��0�Ȃ���A1�Ȃ����� 
 */
int pTenshouExeJumpFromToOnTenshouExeNextTurnIsFieldOrCastle    =0x46B575; // �֐��͂��̃A�h���X����AOnTenshouExeNextTurnIsFieldOrCastle�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeNextTurnIsFieldOrCastle=0x44B81F; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeNextTurnIsFieldOrCastle =0x46B57A; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeNextTurnIsFieldOrCastle() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeNextTurnIsFieldOrCastle

		mov iNextTurnIsFieldOrCastle, eax

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
	OnTenshouExeNextTurnIsFieldOrCastleExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeNextTurnIsFieldOrCastle
	}
}

/*
0046B575 . E8 A502FEFF CALL TENSHOU.0044B81F			���Ȃ��Ƃ�EAX�ɁA��킩����킩�̏�񂪓��� 
				�������� JMP TSMod.OnTSExeNextTurnIsFieldOrCastle �Ə��������Ă����ۂ̏���
0046B57A . 83C4 0C ADD ESP, 0C 
0046B57D . 85C0 TEST EAX, EAX							EAX��1�Ȃ���A0�Ȃ����� 
*/
char cmdOnTenshouExeJumpFromNextTurnIsFieldOrCastle[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeNextTurnIsFieldOrCastle() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeNextTurnIsFieldOrCastle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeNextTurnIsFieldOrCastle + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeNextTurnIsFieldOrCastle  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromNextTurnIsFieldOrCastle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeNextTurnIsFieldOrCastle), cmdOnTenshouExeJumpFromNextTurnIsFieldOrCastle, 5, NULL); //5�o�C�g�̂ݏ�������
}








//---------------------------------------------------------------------------------------------


int iNextTurnCastleID = 0xFFFF;	// ����̃^�[������̏ꍇ�̏�ԍ��B��łȂ��ꍇ�ɂ́A0xFFFF������B
void OnTenshouExeGetNextTurnCastleIDExecute() {
	iNextTurnCastleID = iNextTurnCastleID & 0xFFFF; // EAX���AAX��������ԍ�

	if ( 0 <= iNextTurnCastleID-1 && iNextTurnCastleID-1 < GAMEDATASTRUCT_CASTLE_NUM ) {

		// ��킾�����̂ŁA������ScenarioMod�ɓ`�B
		if ( p_snSetCurrentCastleIDInBattle ) {
			((PFNSNSETCURRENTCASTLEIDINBATTLE) p_snSetCurrentCastleIDInBattle)(iNextTurnCastleID);
		}
	}
}

/*
0046B589 . E8 B0FEFDFF CALL TENSHOU.0044B43E		���OnTenshouExeNextTurnIsFieldOrCastle�ł�EAX���肪����̂ŁA����̎��̂ݗ���B����̃f�[�^���������⃌�W�X�^�ɃZ�b�g�������B 
0046B58E . 83C4 04 ADD ESP, 4						������̃��^�[���i�K�ŁAEAX�̂����AAX����ԍ� 
0046B591 . 56 PUSH ESI
*/
int pTenshouExeJumpFromToOnTenshouExeGetNextTurnCastleID    =0x46B589; // �֐��͂��̃A�h���X����AOnTenshouExeGetNextTurnCastleID�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeGetNextTurnCastleID=0x44B43E; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeGetNextTurnCastleID =0x46B58E; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeGetNextTurnCastleID() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeGetNextTurnCastleID

		mov iNextTurnCastleID, eax		// ����EAX��AX��(���̒����)���̏�ԍ�

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
	OnTenshouExeGetNextTurnCastleIDExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeGetNextTurnCastleID
	}
}

/*
0046B589 . E8 B0FEFDFF CALL TENSHOU.0044B43E		���OnTenshouExeNextTurnIsFieldOrCastle�ł�EAX���肪����̂ŁA����̎��̂ݗ���B����̃f�[�^���������⃌�W�X�^�ɃZ�b�g�������B 
0046B58E . 83C4 04 ADD ESP, 4						������̃��^�[���i�K�ŁAEAX�̂����AAX����ԍ� 
0046B591 . 56 PUSH ESI
*/
char cmdOnTenshouExeJumpFromGetNextTurnCastleID[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeGetNextTurnCastleID() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeGetNextTurnCastleID;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeGetNextTurnCastleID + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGetNextTurnCastleID  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromGetNextTurnCastleID+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeGetNextTurnCastleID), cmdOnTenshouExeJumpFromGetNextTurnCastleID, 5, NULL); //5�o�C�g�̂ݏ�������
}




//---------------------------------------------------------------------------------------------

