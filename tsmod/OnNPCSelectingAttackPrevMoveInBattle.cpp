#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


#include "MakeIsseikougekiMembers.h"

// �퓬��NPC�����ۂ̍s�����Ƃ�ۂ�ID�����肳�ꂽ���B���j�b�g�������O�ɍs��������̂̏ꍇ�B
// �ړ����Ă��U���̏ꍇ�̓p�X�X���[���Ă��܂��̂Œ���(�������A���̑O��𕪐͂���΂�������͖���)

/*
00489D77  |. E8 A3E6FBFF              CALL    TENSHOU.0044841F
00489D7C  |. 83C4 04                  ADD     ESP, 4
00489D7F  |. 8945 F8                  MOV     DWORD PTR SS:[EBP-8], EAX�@�@�� �����̂ڂ��āA�����ōU���̎�ނ͌��܂�B(EBX�����蕐���A[EBP-2]�������̕����ԍ�)
00489D82  |> 33C0                     XOR     EAX, EAX



00489DFD  |. FF75 FE        PUSH    DWORD PTR SS:[EBP-2]
00489E00  |. FF75 08        PUSH    DWORD PTR SS:[EBP+8]
00489E03  |. FF149D 401F4D00          CALL    DWORD PTR DS:[EBX*4+4D1F40]	�� PC�Ƃ�������BNPC�̐퓬�s����ނ�����B
00489E0A  |. 83C4 08        ADD     ESP, 8

00489D2E  |. 66:8BD8                  MOV     BX, AX



*/
extern void OnSelectingAttackEventHandlerInBattle(int iBushouID, int iTargetID, int iSelectActionID, BOOL isMoveBefore);

int iNPCSelectingAttackPrevMoveID = -1;		// 0:�U�� 1:�R�n 2:�S�C 3:��C 4:��čU��
int iBushouIdOfNPCSelectingAttackPrevMove = -1;
int iTargetIdOfNPCSelectingAttackPrevMove = -1;

// 0:�U�� 1:�R�n 2:�S�C 3:��C 4:��čU��
void OnTenshouExeNPCSelectingAttackPrevMoveInBattleExecute() {
	// OutputDebugStream("%d", iSelectingAttackMenuID );
	int iBushouID = (iBushouIdOfNPCSelectingAttackPrevMove & 0xFFFF)-1;
	int iTargetID = (iTargetIdOfNPCSelectingAttackPrevMove & 0xFFFF)-1;

	OnSelectingAttackEventHandlerInBattle(iBushouID, iTargetID, iNPCSelectingAttackPrevMoveID, TRUE);

	// ��čU����������
	if ( iNPCSelectingAttackPrevMoveID == 4 ) {
		// �����Y�������o���\�z
		MakeIsseikougekiMembers(iBushouID, iTargetID);

		/*
		for each(int i in vListIsseiKougekiMembers) {
			OutputDebugStream << nb6bushouname[i].fastname << endl;
		}
		OutputDebugStream << "------------" << endl;
		*/
	}
}

/*
00489D77  |. E8 A3E6FBFF              CALL    TENSHOU.0044841F
				������2�s�� JMP TSMod.OnTSExeNPCSelectingAttackPrevMoveInBattle �Ə��������Ă����ۂ̏���
00489D7C  |. 83C4 04                  ADD     ESP, 4
00489D7F  |. 8945 F8                  MOV     DWORD PTR SS:[EBP-8], EAX�@�@�� �����̂ڂ��āA�����ōU���̎�ނ͌��܂�B(EBX�����蕐���A[EBP-2]�������̕����ԍ�)
00489D82  |> 33C0                     XOR     EAX, EAX
*/
int pTenshouExeJumpFromToOnTenshouExeNPCSelectingAttackPrevMoveInBattle	 =0x489D77; // �֐��͂��̃A�h���X����AOnTenshouExeNPCSelectingAttackPrevMoveInBattle�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeNPCSelectingAttackPrevMoveInBattle=0x44841F; 
int pTenshouExeReturnLblFromOnTenshouExeNPCSelectingAttackPrevMoveInBattle  =0x489D7C; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeNPCSelectingAttackPrevMoveInBattle() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeNPCSelectingAttackPrevMoveInBattle

		mov iNPCSelectingAttackPrevMoveID, eax  // �s���I��ԍ�

		// eax�͕ۑ������̂ŁAeax���ێ����ϐ��ɗ��p����B
		mov eax, DWORD PTR SS:[ebp+8]
		mov iBushouIdOfNPCSelectingAttackPrevMove, eax // �����̕�����ۑ�
		
		mov eax, DWORD PTR SS:[ebp-0x2]
		mov iTargetIdOfNPCSelectingAttackPrevMove, eax	// ����̕����ԍ�

		mov eax, iNPCSelectingAttackPrevMoveID  // ebx�����ւƖ߂�

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
	OnTenshouExeNPCSelectingAttackPrevMoveInBattleExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeNPCSelectingAttackPrevMoveInBattle
	}
}

char cmdOnTenshouExeJumpFromNPCSelectingAttackPrevMoveInBattle[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeNPCSelectingAttackPrevMoveInBattle() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeNPCSelectingAttackPrevMoveInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeNPCSelectingAttackPrevMoveInBattle + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeNPCSelectingAttackPrevMoveInBattle  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromNPCSelectingAttackPrevMoveInBattle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeNPCSelectingAttackPrevMoveInBattle), cmdOnTenshouExeJumpFromNPCSelectingAttackPrevMoveInBattle, 5, NULL); //5�o�C�g�̂ݏ�������
}
