#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"



// �퓬��NPC�����ۂ̍s�����Ƃ�ۂ�ID�����肳��āA���ۂ̊֐��ւƃW�����v���鏊�B

/*

004486D0  |. 56                              PUSH    ESI
004486D1  |. FF14BD C8AE4C00                 CALL    DWORD PTR DS:[EDI*4+4CAEC8]
004486D8  |> 83C4 08                         ADD     ESP, 8
004486DB  |. B8 01000000                     MOV     EAX, 1

*/


int iNPCSelectingAttackPostMoveAfterID = -1;		// 0:�U�� 1:�ˌ�
int iBushouIdOfNPCSelectingAttackPostMoveAfter = -1;
int iTargetIdOfNPCSelectingAttackPostMoveAfter = -1;

// 0:�U�� 1:�R�n 2:�S�C 3:��C 4:��čU��
void OnTenshouExeNPCSelectingAttackPostMoveAfterInBattleExecute() {
	// OutputDebugStream("%d", iSelectingAttackMenuID );
	int iBushouID = (iBushouIdOfNPCSelectingAttackPostMoveAfter & 0xFFFF)-1;
	int iTargetID = (iTargetIdOfNPCSelectingAttackPostMoveAfter & 0xFFFF)-1;

	// �����̓C�x���g�n���h���\���Ă΂Ȃ��ėǂ��B(�Ȃ��Ȃ炱���͍s�����̃��b�Z�[�W�_�C�A���O���o���ゾ����B
	// �����Ƒ����^�C�~���O�ŕʂ̏ꏊ�ŌĂяo���Ă���B

}

/*
004486D0  |. 56                              PUSH    ESI
004486D1  |. FF14BD C8AE4C00                 CALL    DWORD PTR DS:[EDI*4+4CAEC8]
				�����̍s�� JMP TSMod.OnTSExeNPCSelectingAttackPostMoveAfterInBattle �Ə��������Ă����ۂ̏���
004486D8  |> 83C4 08                         ADD     ESP, 8
004486DB  |. B8 01000000                     MOV     EAX, 1

*/
int pTenshouExeJumpFromToOnTenshouExeNPCSelectingAttackPostMoveAfterInBattle	 =0x4486D1; // �֐��͂��̃A�h���X����AOnTenshouExeNPCSelectingAttackPostMoveAfterInBattle�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeNPCSelectingAttackPostMoveAfterInBattle  =0x4486D8; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeNPCSelectingAttackPostMoveAfterInBattle() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iNPCSelectingAttackPostMoveAfterID, edi
		mov iBushouIdOfNPCSelectingAttackPostMoveAfter, esi // �����̕����ԍ�
		mov iTargetIdOfNPCSelectingAttackPostMoveAfter, ebx // ����̕����ԍ�

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// �Z�\�I�����ɍs������
	OnTenshouExeNPCSelectingAttackPostMoveAfterInBattleExecute();

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

		// ���X�V�ċL�ɂ������W�����v�ꏊ
		CALL    DWORD PTR DS:[EDI*4+0x4CAEC8]

		jmp pTenshouExeReturnLblFromOnTenshouExeNPCSelectingAttackPostMoveAfterInBattle
	}
}

char cmdOnTenshouExeJumpFromNPCSelectingAttackPostMoveAfterInBattle[8]="\xE9\x90\x90\x90\x90\x90\x90";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeNPCSelectingAttackPostMoveAfterInBattle() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeNPCSelectingAttackPostMoveAfterInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeNPCSelectingAttackPostMoveAfterInBattle + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeNPCSelectingAttackPostMoveAfterInBattle  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromNPCSelectingAttackPostMoveAfterInBattle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeNPCSelectingAttackPostMoveAfterInBattle), cmdOnTenshouExeJumpFromNPCSelectingAttackPostMoveAfterInBattle, 7, NULL); //7�o�C�g�̂ݏ�������
}
