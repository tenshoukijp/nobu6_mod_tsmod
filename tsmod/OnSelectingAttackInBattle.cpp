#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"

#include "MakeIsseikougekiMembers.h"

/*
0046AA14  |. 50             PUSH    EAX
0046AA15  |. FF14BD 20D54C0>CALL    DWORD PTR DS:[EDI*4+4CD520]
0046AA1C  |. 83C4 08        ADD     ESP, 8
0046AA1F  |> 5F             POP     EDI
*/



/*
 * �퓬���ɋZ�\�̑I����
 */
extern void OnSelectingAttackEventHandlerInBattle(int iBushouID, int iTargetID, int iSelectActionID, BOOL IsMoveBefore);

int iSelectingAttackMenuID = -1;  // �I�����ꂽ�Z�\ID 0:�U�� 1:�ˌ� 2:�S�C 3:��C 4:��čU��
int iBushouIdOfSelectingAttack = -1;
int iTargetIdOfSelectingAttack = -1;

// 0:�U�� 1:�R�n 2:�S�C 3:��C 4:��čU��
void OnTenshouExeSelectingAttackMenuInBattleExecute() {
	; // OutputDebugStream("%d", iSelectingAttackMenuID );
	int iBushouID = (iBushouIdOfSelectingAttack & 0xFFFF)-1;
	int iTargetID = (iTargetIdOfSelectingAttack & 0xFFFF)-1;

	OnSelectingAttackEventHandlerInBattle(iBushouID, iTargetID, iSelectingAttackMenuID, TRUE);
	// ��čU����������
	if ( iSelectingAttackMenuID == 4 ) {
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
0046AA14  |. 50             PUSH    EAX
0046AA15  |. FF14BD 20D54C00       CALL    DWORD PTR DS:[EDI*4+4CD520]
				�����̍s�� JMP TSMod.OnTSExeSelectingAttackMenuInBattle �Ə��������Ă����ۂ̏���
0046AA1C  |. 83C4 08        ADD     ESP, 8
0046AA1F  |> 5F             POP     EDI
*/
int pTenshouExeJumpFromToOnTenshouExeSelectingAttackMenuInBattle	 =0x46AA15; // �֐��͂��̃A�h���X����AOnTenshouExeSelectingAttackMenuInBattle�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeSelectingAttackMenuInBattle  =0x46AA1C; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeSelectingAttackMenuInBattle() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		mov iSelectingAttackMenuID, edi // �I�����ꂽ�h�c��ۑ����Ă���

		mov iBushouIdOfSelectingAttack, eax	// ���s��������
		mov iTargetIdOfSelectingAttack, esi // ��p�Ώۂ̕���

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
	OnTenshouExeSelectingAttackMenuInBattleExecute();

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

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call DWORD PTR DS:[EDI*4+0x4CD520]

		jmp pTenshouExeReturnLblFromOnTenshouExeSelectingAttackMenuInBattle
	}
}

char cmdOnTenshouExeJumpFromSelectingAttackMenuInBattle[8]="\xE9\x90\x90\x90\x90\x90\x90";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeSelectingAttackMenuInBattle() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeSelectingAttackMenuInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSelectingAttackMenuInBattle + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeSelectingAttackMenuInBattle  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromSelectingAttackMenuInBattle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSelectingAttackMenuInBattle), cmdOnTenshouExeJumpFromSelectingAttackMenuInBattle, 7, NULL); //7�o�C�g�̂ݏ�������
}




