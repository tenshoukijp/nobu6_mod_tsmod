#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


/*
 * �퓬���ɋZ�\�̑I����
 */

// 0 = �ꊅ, 1 = ����, 2 = ����, 3 = �ÎE, 4 = ����, 5 = ����
int iSelectingAlibityMenuID = -1;  // �I�����ꂽ�Z�\ID

void OnTenshouExeSelectingAbilityMenuInBattleExecute() {
	; // OutputDebugStream("%d", iSelectingAlibityMenuID );
}


/*
0041BDCD  |. EB 02               JMP     SHORT TENSHOU.0041BDD1
0041BDCF  |> 8BD1                MOV     EDX, ECX
0041BDD1  |> FF75 08             PUSH    DWORD PTR SS:[EBP+8]
				������2�s�� JMP TSMod.OnTSExeSelectingAbilityMenuInBattle �Ə��������Ă����ۂ̏���
0041BDD4  |. FF1495 68C84B00     CALL    DWORD PTR DS:[EDX*4+4BC868]
0041BDDB  |. 83C4 04             ADD     ESP, 4
*/
int pTenshouExeJumpFromToOnTenshouExeSelectingAbilityMenuInBattle	  =0x41BDCF; // �֐��͂��̃A�h���X����AOnTenshouExeSelectingAbilityMenuInBattle�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeSelectingAbilityMenuInBattle  =0x41BDD4; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeSelectingAbilityMenuInBattle() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		mov edx, ecx
		push dword ptr ss:[ebp+8]

		// �I�������R�}���hID���R�s�[���Ă���
		mov iSelectingAlibityMenuID, ecx

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
	OnTenshouExeSelectingAbilityMenuInBattleExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeSelectingAbilityMenuInBattle
	}
}

/*
0041BDCD  |. EB 02               JMP     SHORT TENSHOU.0041BDD1
0041BDCF  |> 8BD1                MOV     EDX, ECX
0041BDD1  |> FF75 08             PUSH    DWORD PTR SS:[EBP+8]
				������2�s�� JMP TSMod.OnTSExeSelectingAbilityMenuInBattle �Ə��������Ă����ۂ̏���
0041BDD4  |. FF1495 68C84B00     CALL    DWORD PTR DS:[EDX*4+4BC868]
0041BDDB  |. 83C4 04             ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromSelectingAbilityMenuInBattle[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeSelectingAbilityMenuInBattle() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeSelectingAbilityMenuInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSelectingAbilityMenuInBattle + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeSelectingAbilityMenuInBattle  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromSelectingAbilityMenuInBattle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSelectingAbilityMenuInBattle), cmdOnTenshouExeJumpFromSelectingAbilityMenuInBattle, 5, NULL); //5�o�C�g�̂ݏ�������
}




