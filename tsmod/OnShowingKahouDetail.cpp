#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"

/*
 * �ƕ�\�����[�h�̂n�m�Ƃn�e�e�������ǂ�B�ƕ�ڍ׏���\�����Ă���Ԃ́AiKahouShowingMode ��ON�ɂ��邽�߂��B
 */


// �ƕ��Shouwing���[�h�̃X�^�[�g�B
int iKahouShowingMode = 0L;

void OnTenshouExeShowingKahouDetailExecute() {
	// �A�Z���u���̒��Ńt���O�̑�����������Ă���B
}


/*
004538DA  |. FF75 08        PUSH    DWORD PTR SS:[EBP+8]
004538DD  |. E8 DC110400    CALL    TENSHOU.00494ABE
004538E2  |. E8 F07F0200    CALL    TENSHOU.0047B8D7
				�������� JMP TSMod.OnTSExeShowingKahouDetail �Ə��������Ă����ۂ̏���
004538E7  |. 8D8D 7CFFFFFF  LEA     ECX, DWORD PTR SS:[EBP-84]
004538ED  |. 8935 80154D00  MOV     DWORD PTR DS:[4D1580], ESI
*/
int pTenshouExeJumpFromToOnTenshouExeShowingKahouDetail		=0x4538E2; // �֐��͂��̃A�h���X����AOnTenshouExeShowingKahouDetail�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeShowingKahouDetail =0x47B8D7; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeShowingKahouDetail	=0x4538E7; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeShowingKahouDetail() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// �ƕ�Shouwing���[�h��1������B
		mov iKahouShowingMode, 1

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeShowingKahouDetail

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
	OnTenshouExeShowingKahouDetailExecute();

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

		// �ƕ�Shouwing���[�h��0������B
		mov iKahouShowingMode, 0

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingKahouDetail
	}
}

/*
004538DA  |. FF75 08        PUSH    DWORD PTR SS:[EBP+8]
004538DD  |. E8 DC110400    CALL    TENSHOU.00494ABE
004538E2  |. E8 F07F0200    CALL    TENSHOU.0047B8D7
				�������� JMP TSMod.OnTSExeShowingKahouDetail �Ə��������Ă����ۂ̏���
004538E7  |. 8D8D 7CFFFFFF  LEA     ECX, DWORD PTR SS:[EBP-84]
004538ED  |. 8935 80154D00  MOV     DWORD PTR DS:[4D1580], ESI
*/
char cmdOnTenshouExeJumpFromShowingKahouDetail[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeShowingKahouDetail() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeShowingKahouDetail;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingKahouDetail + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingKahouDetail  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromShowingKahouDetail+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingKahouDetail), cmdOnTenshouExeJumpFromShowingKahouDetail, 5, NULL); //5�o�C�g�̂ݏ�������
}




