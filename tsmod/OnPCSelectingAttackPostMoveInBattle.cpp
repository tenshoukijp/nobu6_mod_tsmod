#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"

extern void OnSelectingAttackEventHandlerInBattle(int iBushouID, int iTargetID, int iSelectActionID, BOOL isMoveBefore);

#include "MakeIsseikougekiMembers.h"

// �퓬��PC�����ۂ̍s�����Ƃ�ۂ�ID�����肳�ꂽ���B���j�b�g��������ɍs��������̂̏ꍇ�B

/*
0044864A  |. 57             PUSH    EDI
0044864B  |. 56             PUSH    ESI
0044864C  |. FF149D C8AE4C0>CALL    DWORD PTR DS:[EBX*4+4CAEC8]                                        ;  TENSHOU.00431ECD
00448653  |. E9 80000000    JMP     TENSHOU.004486D8

���ۂɎ��s���钼�O�̃W�����v��(NPC)
0044864C  |. FF149D C8AE4C0>CALL    DWORD PTR DS:[EBX*4+4CAEC8]                                        ;  TENSHOU.00431ECD
*/


int iPCSelectingAttackPostMoveID = -1;		// 0:�U�� 1:�ˌ�
int iBushouIdOfPCSelectingAttackPostMove = -1;
int iTargetIdOfPCSelectingAttackPostMove = -1;

// 0:�U�� 1:�R�n 2:�S�C 3:��C 4:��čU��
void OnTenshouExePCSelectingAttackPostMoveInBattleExecute() {
	; // OutputDebugStream("%d", iSelectingAttackMenuID );
	int iBushouID = (iBushouIdOfPCSelectingAttackPostMove & 0xFFFF)-1;
	int iTargetID = (iTargetIdOfPCSelectingAttackPostMove & 0xFFFF)-1;

	OnSelectingAttackEventHandlerInBattle(iBushouID, iTargetID, iPCSelectingAttackPostMoveID, FALSE);

}

/*
0044864A  |. 57             PUSH    EDI
0044864B  |. 56             PUSH    ESI
0044864C  |. FF149D C8AE4C00                  CALL    DWORD PTR DS:[EBX*4+4CAEC8]                                        ;  TENSHOU.00431ECD
				�����̍s�� JMP TSMod.OnTSExePCSelectingAttackPostMoveInBattle �Ə��������Ă����ۂ̏���
00448653  |. E9 80000000    JMP     TENSHOU.004486D8
*/
int pTenshouExeJumpFromToOnTenshouExePCSelectingAttackPostMoveInBattle		=0x44864C; // �֐��͂��̃A�h���X����AOnTenshouExePCSelectingAttackPostMoveInBattle�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExePCSelectingAttackPostMoveInBattle  =0x448653; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExePCSelectingAttackPostMoveInBattle() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iPCSelectingAttackPostMoveID, ebx
		mov iBushouIdOfPCSelectingAttackPostMove, esi // �����̕����ԍ�
		mov iTargetIdOfPCSelectingAttackPostMove, edi // ����̕����ԍ�

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
	OnTenshouExePCSelectingAttackPostMoveInBattleExecute();

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
		CALL    DWORD PTR DS:[EBX*4+0x4CAEC8]

		jmp pTenshouExeReturnLblFromOnTenshouExePCSelectingAttackPostMoveInBattle
	}
}

char cmdOnTenshouExeJumpFromPCSelectingAttackPostMoveInBattle[8]="\xE9\x90\x90\x90\x90\x90\x90";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExePCSelectingAttackPostMoveInBattle() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExePCSelectingAttackPostMoveInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExePCSelectingAttackPostMoveInBattle + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExePCSelectingAttackPostMoveInBattle  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromPCSelectingAttackPostMoveInBattle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExePCSelectingAttackPostMoveInBattle), cmdOnTenshouExeJumpFromPCSelectingAttackPostMoveInBattle, 7, NULL); //7�o�C�g�̂ݏ�������
}
