#include "WinTarget.h"

#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "SetNeedKoudouryoku.h"

// �����ȂǂT�l�I�����邽�߂̓��̓_�C�A���O���o����


/*
0042F21C  |. FFB1 B0000000  PUSH    DWORD PTR DS:[ECX+B0]
0042F222     FFB6 BC000000  PUSH    DWORD PTR DS:[ESI+BC]
0042F228  |. E8 0E420500    CALL    TENSHOU.0048343B
*/

void OnTenshouExeViewingNaiseiExecutantsEtc5NingumiDialogExecute() {
}
static int iTmpEAXOfViewingNaiseiExecutantsEtc5NingumiDialog = 0; // EAX�̒l�̈ꎟ�ۑ��p

int pTenshouExeJumpFromToOnTenshouExeViewingNaiseiExecutantsEtc5NingumiDialog    =CMD_NEED_KOUDOUVALUE_5NINGUMI_ADDRESS_06; // �֐��͂��̃A�h���X����AOnTenshouExeViewingNaiseiExecutantsEtc5NingumiDialog�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeViewingNaiseiExecutantsEtc5NingumiDialog =0x42F228; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeViewingNaiseiExecutantsEtc5NingumiDialog() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iTmpEAXOfViewingNaiseiExecutantsEtc5NingumiDialog, eax // EAX�ꎟ�g�p����̂ŁA��������ێ�
		mov eax, TSModCommand::AllGundan::cmdNeedKoudouValue5Ningumi // �T�l�g�̍s���͂�������EAX�ɃR�s�[
		mov DWORD PTR DS:[ESI+0xBC], EAX // EAX�̒l���㏑��

		PUSH    DWORD PTR DS:[ESI+0xBC] // ���X�V�ċL�ɂ�����������������

		mov eax, iTmpEAXOfViewingNaiseiExecutantsEtc5NingumiDialog // EAX�̒l�����ւƖ߂�

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
	OnTenshouExeViewingNaiseiExecutantsEtc5NingumiDialogExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingNaiseiExecutantsEtc5NingumiDialog
	}
}


char cmdOnTenshouExeJumpFromViewingNaiseiExecutantsEtc5NingumiDialog[7]="\xE9\x90\x90\x90\x90\x90";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeViewingNaiseiExecutantsEtc5NingumiDialog() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeViewingNaiseiExecutantsEtc5NingumiDialog;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingNaiseiExecutantsEtc5NingumiDialog + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingNaiseiExecutantsEtc5NingumiDialog  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromViewingNaiseiExecutantsEtc5NingumiDialog+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingNaiseiExecutantsEtc5NingumiDialog), cmdOnTenshouExeJumpFromViewingNaiseiExecutantsEtc5NingumiDialog, 6, NULL); //6�o�C�g�̂ݏ�������
}




