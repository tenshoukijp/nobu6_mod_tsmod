#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"

// �푈���̒����R�}���h��I�񂾍ۂɁA�Ȃ������m��100�Ƀt�B���^�[����Ă��܂��֐�������B
// ���ꂪ�Ă΂ꂽ���̏���




// ������̗\��̕���
int iSoldierNewDraftedValue = 0;

// �����O�̕���
int iSoldierOrgDraftedValue = 0;

// ���������Ώۂ̕���ID(�z���ł͂Ȃ��A�����̒l�ł���(�z����+1������))
int iDraftedBushouID = 0;



void OnTenshouExeSoldierDraftFilterInBattleExecute() {
	/*
		iSoldierOrgDraftedValue,iSoldierNewDraftedValue,(iDraftedBushouID-1)��
		�������肵����H �m�C����ʂɑ��₵����H
	 */
}


/*
00433F33  |. 56             PUSH    ESI
00433F34  |. E8 A25CFDFF    CALL    TENSHOU.00409BDB
				�������� JMP TSMod.OnTSExeSoldierDraftFilterInBattle �Ə��������Ă����ۂ̏���
00433F39  |. 83C4 08        ADD     ESP, 8
*/
int pTenshouExeJumpFromToOnTenshouExeSoldierDraftFilterInBattle		=0x433F34; // �֐��͂��̃A�h���X����AOnTenshouExeSoldierDraftFilterInBattle�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeSoldierDraftFilterInBattle =0x409BDB; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeSoldierDraftFilterInBattle	=0x433F39; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeSoldierDraftFilterInBattle() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ����ecx�̒l���ړI�̒l�Ȃ̂ŕۑ�
		mov iSoldierNewDraftedValue, ecx

		// ����ebx�̒l���ړI�̒l�Ȃ̂ŕۑ�
		mov iSoldierOrgDraftedValue, ebx

		// �Ώۂ̕���ID
		mov iDraftedBushouID, esi

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// �������ɉ�������Ȃ�΁c
	OnTenshouExeSoldierDraftFilterInBattleExecute();

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

		// ������̒l�̂݉��ς̉\��������̂ŃR�s�[
		mov ecx, iSoldierNewDraftedValue

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeSoldierDraftFilterInBattle

		jmp pTenshouExeReturnLblFromOnTenshouExeSoldierDraftFilterInBattle
	}
}

/*
00433F33  |. 56             PUSH    ESI
00433F34  |. E8 A25CFDFF    CALL    TENSHOU.00409BDB
				�������� JMP TSMod.OnTSExeSoldierDraftFilterInBattle �Ə��������Ă����ۂ̏���
00433F39  |. 83C4 08        ADD     ESP, 8
00433F3C  |. FF75 F8        PUSH    DWORD PTR SS:[EBP-8]
*/
char cmdOnTenshouExeJumpFromSoldierDraftFilterInBattle[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeSoldierDraftFilterInBattle() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeSoldierDraftFilterInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSoldierDraftFilterInBattle + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeSoldierDraftFilterInBattle  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromSoldierDraftFilterInBattle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSoldierDraftFilterInBattle), cmdOnTenshouExeJumpFromSoldierDraftFilterInBattle, 5, NULL); //5�o�C�g�̂ݏ�������
}




