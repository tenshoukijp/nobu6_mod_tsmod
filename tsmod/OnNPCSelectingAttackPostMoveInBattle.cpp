#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"

extern void OnSelectingAttackEventHandlerInBattle(int iBushouID, int iTargetID, int iSelectActionID, BOOL isMoveBefore);

#include "MakeIsseikougekiMembers.h"

// �퓬��NPC�����ۂ̍s�����Ƃ�ۂ�ID�����肳�ꂽ���B���j�b�g��������ɍs��������̂̏ꍇ�B

/*
00489DBA  |. 83F8 01        CMP     EAX, 1							��������EAX�Ȃ�ړ���œˌ��B����ȊO(����0)�Ȃ�U���BEBX������̕����ԍ��AESI�������̕����ԍ�
00489DBD  |. 75 11          JNZ     SHORT TENSHOU.00489DD0
00489DBF  |. 6A 01          PUSH    1                                ; /Arg5 = 00000001


���ۂɎ��s���钼�O�̃W�����v��(NPC)
��004486D1  |. FF14BD C8AE4C00             CALL    DWORD PTR DS:[EDI*4+4CAEC8]
*/


int iNPCSelectingAttackPostMoveID = -1;		// 0:�U�� 1:�ˌ�
int iBushouIdOfNPCSelectingAttackPostMove = -1;
int iTargetIdOfNPCSelectingAttackPostMove = -1;

// 0:�U�� 1:�R�n 2:�S�C 3:��C 4:��čU��
void OnTenshouExeNPCSelectingAttackPostMoveInBattleExecute() {
	// OutputDebugStream("%d", iSelectingAttackMenuID );
	int iBushouID = (iBushouIdOfNPCSelectingAttackPostMove & 0xFFFF)-1;
	int iTargetID = (iTargetIdOfNPCSelectingAttackPostMove & 0xFFFF)-1;

	OnSelectingAttackEventHandlerInBattle(iBushouID, iTargetID, iNPCSelectingAttackPostMoveID, FALSE);

}

/*
00489DBA  |. 83F8 01        CMP     EAX, 1							��������EAX�Ȃ�ړ���œˌ��B����ȊO(����0)�Ȃ�U���BEBX������̕����ԍ��AESI�������̕����ԍ�
00489DBD  |. 75 11          JNZ     SHORT TENSHOU.00489DD0
				������2�s�� JMP TSMod.OnTSExeNPCSelectingAttackPostMoveInBattle �Ə��������Ă����ۂ̏���
00489DBF  |. 6A 01          PUSH    1                                ; /Arg5 = 00000001
*/
int pTenshouExeJumpFromToOnTenshouExeNPCSelectingAttackPostMoveInBattle		=0x489DBA; // �֐��͂��̃A�h���X����AOnTenshouExeNPCSelectingAttackPostMoveInBattle�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeNPCSelectingAttackPostMoveInBattle =0x489DBD; 
int pTenshouExeReturnLblFromOnTenshouExeNPCSelectingAttackPostMoveInBattle  =0x489DBF; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
int pTenshouExeReturnLblFromOnTenshouExeNPCSelectingAttackPostMoveInBattleNormal  =0x489DD0; // ���ʂ̍U���̍ۂ̃W�����v��  JNZ     SHORT TENSHOU.00489DD0
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeNPCSelectingAttackPostMoveInBattle() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iNPCSelectingAttackPostMoveID, eax			// �I���s��(0=�U��, 1=�ˌ�)
		mov iTargetIdOfNPCSelectingAttackPostMove, ebx	// ����̕����ԍ�

		// eax�͕ۑ������̂ŁAeax���ێ����ϐ��ɗ��p����B
		mov eax, DWORD PTR SS:[ebp+8]
		mov iBushouIdOfNPCSelectingAttackPostMove, eax // �����̕�����ۑ�

		mov eax, iNPCSelectingAttackPostMoveID  // ebx�����ւƖ߂�

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
	OnTenshouExeNPCSelectingAttackPostMoveInBattleExecute();

	if ( iNPCSelectingAttackPostMoveID == 1 ) {

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

			jmp pTenshouExeReturnLblFromOnTenshouExeNPCSelectingAttackPostMoveInBattle
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

			jmp pTenshouExeReturnLblFromOnTenshouExeNPCSelectingAttackPostMoveInBattleNormal
		}
	}
}

char cmdOnTenshouExeJumpFromNPCSelectingAttackPostMoveInBattle[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeNPCSelectingAttackPostMoveInBattle() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeNPCSelectingAttackPostMoveInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeNPCSelectingAttackPostMoveInBattle + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeNPCSelectingAttackPostMoveInBattle  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromNPCSelectingAttackPostMoveInBattle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeNPCSelectingAttackPostMoveInBattle), cmdOnTenshouExeJumpFromNPCSelectingAttackPostMoveInBattle, 5, NULL); //5�o�C�g�̂ݏ�������
}
