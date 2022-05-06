#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


// �퓬����U��풆�ɁA�R�c�^�[�����؂�ւ�鎞
/*
0046A6B7   . E8 C59F0300    CALL    TENSHOU.004A4681
0046A6BC   . 83C4 04        ADD     ESP, 4
0046A6BF   . 85C0           TEST    EAX, EAX
0046A6C1   . 75 04          JNZ     SHORT TENSHOU.0046A6C7
0046A6C3   . 33C0           XOR     EAX, EAX
0046A6C5   . EB 52          JMP     SHORT TENSHOU.0046A719
0046A6C7   > 66:8B0D 545D4D>MOV     CX, WORD PTR DS:[4D5D54]�@�� ���ꍡ�̌R�c�^�[��
0046A6CE   . 8D4424 0C      LEA     EAX, DWORD PTR SS:[ESP+C]
0046A6D2   . 50             PUSH    EAX
0046A6D3   . 6A 08          PUSH    8
0046A6D5   . 51             PUSH    ECX
0046A6D6   . E8 CC14FEFF    CALL    TENSHOU.0044BBA7
0046A6DB   . 83C4 0C        ADD     ESP, 0C
*/





extern int arrRemainMoveCountAllBushou[GAMEDATASTRUCT_BUSHOU_NUM];


// �퓬����U��풆�ɁA�R�c�^�[�����؂�ւ�鎞
void OnTenshouExeChangeGundanTurnInBattleExecute() {
	// �ړ��͂��኱��
	for ( int i=0; i < GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_NUM; i++ ) {
		// ���݃^�[�����I�����悤�Ƃ��Ă���R�c�ɑ����镐���ɂ��āc
		if ( nb6bushou_hex_unit_info[i].junban == nb6gundan_hex_info.junban ) {
			// ���O�������̂Łc
			int iBushouID = nb6bushou_hex_unit_info[i].bushou - 1;
			if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
				// �J�X�^���Ȏc��s�������O�Ƃ���B
				arrRemainMoveCountAllBushou[iBushouID] = 0;
			}
		}
	}
}


/*
0046A6D5   . 51             PUSH    ECX
0046A6D6   . E8 CC14FEFF    CALL    TENSHOU.0044BBA7
				�������� JMP TSMod.OnTSExeChangeGundanTurnInBattle �Ə��������Ă����ۂ̏���
0046A6DB   . 83C4 0C        ADD     ESP, 0C
*/
int pTenshouExeJumpFromToOnTenshouExeChangeGundanTurnInBattle	  =0x46A6D6; // �֐��͂��̃A�h���X����AOnTenshouExeChangeGundanTurnInBattle�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeChangeGundanTurnInBattle =0x44BBA7;
int pTenshouExeReturnLblFromOnTenshouExeChangeGundanTurnInBattle  =0x46A6DB; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w

// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeChangeGundanTurnInBattle() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeChangeGundanTurnInBattleExecute();

	__asm {

		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	
		call pTenshouExeJumpCallFromToOnTenshouExeChangeGundanTurnInBattle // TENSHOU.EXE�Ɍ��X����������

		jmp pTenshouExeReturnLblFromOnTenshouExeChangeGundanTurnInBattle
	}
}

/*
0046A6D5   . 51             PUSH    ECX
0046A6D6   . E8 CC14FEFF    CALL    TENSHOU.0044BBA7
				�������� JMP TSMod.OnTSExeChangeGundanTurnInBattle �Ə��������Ă����ۂ̏���
0046A6DB   . 83C4 0C        ADD     ESP, 0C
*/
char cmdOnTenshouExeJumpFromChangeGundanTurnInBattle[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeChangeGundanTurnInBattle() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeChangeGundanTurnInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeChangeGundanTurnInBattle + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeChangeGundanTurnInBattle  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromChangeGundanTurnInBattle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeChangeGundanTurnInBattle), cmdOnTenshouExeJumpFromChangeGundanTurnInBattle, 5, NULL); //5�o�C�g�̂ݏ�������
}




