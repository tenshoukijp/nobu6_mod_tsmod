#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "MessageN6PPatch.h"


/*
00419D61  |. E8 4FFEFFFF    CALL    TENSHOU.00419BB5�@�@�@�@�� ���ꂪ�u��I�v�̒��g�̊֐�

�W�l�����������H
00457A39  /$ 56             PUSH    ESI
00457A3A  |. 8BF1           MOV     ESI, ECX
00457A3C  |. E8 C3F4FFFF    CALL    TENSHOU.00456F04
00457A41  |. 8BCE           MOV     ECX, ESI
00457A43  |. E8 D7F5FFFF    CALL    TENSHOU.0045701F
00457A48  |. 8BCE           MOV     ECX, ESI
00457A4A  |. E8 4BF6FFFF    CALL    TENSHOU.0045709A
00457A4F  |. 8BCE           MOV     ECX, ESI
00457A51  |. E8 B8F6FFFF    CALL    TENSHOU.0045710E
00457A56  |. 8BCE           MOV     ECX, ESI
00457A58  |. E8 00FCFFFF    CALL    TENSHOU.0045765D
00457A5D  |. 8BCE           MOV     ECX, ESI
00457A5F  |. E8 11FCFFFF    CALL    TENSHOU.00457675
00457A64  |. 8BCE           MOV     ECX, ESI
00457A66  |. E8 49FFFFFF    CALL    TENSHOU.004579B4
00457A6B  |. 8BCE           MOV     ECX, ESI
00457A6D  |. E8 EFFDFFFF    CALL    TENSHOU.00457861
00457A72  |. 8BCE           MOV     ECX, ESI
00457A74  |. E8 4CFEFFFF    CALL    TENSHOU.004578C5
00457A79  |. 5E             POP     ESI
00457A7A  \. C3             RETN


*/

/*
00456EB1  /$ 8B4424 08      MOV     EAX, DWORD PTR SS:[ESP+8]
00456EB5  |. 85C0           TEST    EAX, EAX
00456EB7  |. 74 36          JE      SHORT TENSHOU.00456EEF
	��
00456EB1�@					JMP		OnTenshouExeHirouFukidashi �Ƃ���B
00456EB6					NOP(=90)
00456EB7  |. 74 36          JE      SHORT TENSHOU.00456EEF
*/
/*
 * ���I��̐����o����
 */

void OnTenshouExeHirouFukidashiExecute() {
	MessageN6PPatch::isOnTenshouExeMessageCopyUsed = false;

	// ��������CurProcessMemory��CPM�l���Z�b�g
	CPM.setVariableFromMemory();

	// �������A���b�Z�[�W�A�t�����
	ReWritePatchMessageN6P( CPM.getCurVarPtr(LOC1), CPM.getCurVarPtr(LOC4), NULL );
}


int pTenshouExeJumpFromToOnTenshouExeHirouFukidashi    =0x456EB1; // �֐��͂��̃A�h���X����AOnTenshouExeHirouFukidashi�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeHirouFukidashi =0x456EB7; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeHirouFukidashi() {
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
	OnTenshouExeHirouFukidashiExecute();

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
		mov     eax, dword ptr ss:[esp+8]
		test    eax, eax

// 		call pTenshouExeJumpCallFromToOnTenshouExeHirouFukidashi

		jmp pTenshouExeReturnLblFromOnTenshouExeHirouFukidashi
	}
}


char cmdOnTenshouExeJumpFromHirouFukidashi[7]="\xE9\x90\x90\x90\x90\x90";
// ���̖��߂�5�o�C�g�A�Ō��NOP���P�����āA�Ȍ�̊֐��Ő��܂�閽�߂����v�U�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeHirouFukidashi() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeHirouFukidashi;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeHirouFukidashi + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeHirouFukidashi  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromHirouFukidashi+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeHirouFukidashi), cmdOnTenshouExeJumpFromHirouFukidashi, 6, NULL); //6�o�C�g�̂ݏ�������
}

