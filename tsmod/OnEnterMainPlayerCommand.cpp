#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "ScenarioMod.h"

/*

0041050B  |> 6A 00          /PUSH    0
0041050D  |. E8 C4780700    |CALL    TENSHOU.00487DD6
00410512  |. 83C4 04        |ADD     ESP, 4
00410515  |. E8 BDB30600    |CALL    TENSHOU.0047B8D7				�� �v���C�������C����ʂœ��ɉ������͂��Ă��炸�A�T���Ă����ԁi���C���R�}���h���T�����O�����!!)
0041051A  |. 8BD8           |MOV     EBX, EAX
0041051C  |. 85DB           |TEST    EBX, EBX
0041051E  |. 74 2E          |JE      SHORT TENSHOU.0041054E


004105EB  |. 897D F8        |MOV     DWORD PTR SS:[EBP-8], EDI
004105EE  |. E8 196D0600    |CALL    TENSHOU.0047730C
004105F3  |. E8 A6000000    |CALL    TENSHOU.0041069E				�� �u�Êρv�R�}���h�B�����͓���
004105F8  |. 85C0           |TEST    EAX, EAX
004105FA  |. 74 63          |JE      SHORT TENSHOU.0041065F
004105FC  |. E8 28040000    |CALL    TENSHOU.00410A29
00410601  |. EB 7D          |JMP     SHORT TENSHOU.00410680
00410603  |> B9 64000000    |MOV     ECX, 64
00410608  |. 8BC3           |MOV     EAX, EBX
0041060A  |. 2BD2           |SUB     EDX, EDX
0041060C  |. F7F1           |DIV     ECX
0041060E  |. 83F8 03        |CMP     EAX, 3
00410611  |. 75 1A          |JNZ     SHORT TENSHOU.0041062D
00410613  |. 8D45 F4        |LEA     EAX, DWORD PTR SS:[EBP-C]
00410616  |. 8D4D F8        |LEA     ECX, DWORD PTR SS:[EBP-8]
00410619  |. 50             |PUSH    EAX                             ; /Arg3
0041061A  |. 51             |PUSH    ECX                             ; |Arg2
0041061B  |. 53             |PUSH    EBX                             ; |Arg1
0041061C  |. E8 D3010000    |CALL    TENSHOU.004107F4                ; \TENSHOU.004107F4	���R�}���h�Q�B���C���J�e�S��
00410621  |. 83C4 0C        |ADD     ESP, 0C
00410624  |. 8BD8           |MOV     EBX, EAX
00410626  |. 83FB 01        |CMP     EBX, 1
00410629  |. 77 61          |JA      SHORT TENSHOU.0041068C
0041062B  |. EB 1D          |JMP     SHORT TENSHOU.0041064A
0041062D  |> 83F8 04        |CMP     EAX, 4
00410630  |. 75 2D          |JNZ     SHORT TENSHOU.0041065F
00410632  |. 8D45 F4        |LEA     EAX, DWORD PTR SS:[EBP-C]
00410635  |. 8D4D F8        |LEA     ECX, DWORD PTR SS:[EBP-8]
00410638  |. 50             |PUSH    EAX
00410639  |. 51             |PUSH    ECX
0041063A  |. 53             |PUSH    EBX
0041063B  |. E8 56010000    |CALL    TENSHOU.00410796										�� �R�}���h�S�B�T�u�J�e�S��
00410640  |. 83C4 0C        |ADD     ESP, 0C

*/

#include "OnEnterMainPlayerCommand.h"

void OnEnterMainPlayerCommand(int iSelectedCommandID, int iSubCommandID) {
	if ( iSelectedCommandID == 0 ) {
		return;
	}

	// ScenarioMod�̌ďo��
	if ( p_snOnEnterMainPlayerCommand ) {
		p_snOnEnterMainPlayerCommand( iSelectedCommandID, iSubCommandID );
	}
	// �������̃R�}���h�ɓ������ۂɂ͂����Ŕ���\
	// OutputDebugStream << "Enter" << iSelectedCommandID << "," << iSubCommand << endl;
}



int iEnterOfClickMainScreenMainCommand = 0; // �R�}���h�ɐN���������ǂ����B���C���R�}���h���ƃT�u�R�}���h���ŐN���̃^�C�~���O�𕪂���B
int iCurrentCommandOfClickMainScreenMainCommand = -1; // �I�����ꂽ�R�}���h�B
void OnTenshouExeClickMainScreenMainCommandExecute() {

	switch (iCurrentCommandOfClickMainScreenMainCommand ) {
		case ���C���R�}���h��::�{��:
			{
				iEnterOfClickMainScreenMainCommand = ���C���R�}���h��::�{��;
				break;
			}
		case ���C���R�}���h��::�x��:
			{
				iEnterOfClickMainScreenMainCommand = ���C���R�}���h��::�x��;
				break;
			}
		case ���C���R�}���h��::��:
			{
				iEnterOfClickMainScreenMainCommand = ���C���R�}���h��::��;
				break;
			}
		case ���C���R�}���h��::�n�}:
			{
				iEnterOfClickMainScreenMainCommand = ���C���R�}���h��::�n�};
				break;
			}
		case ���C���R�}���h��::�푈:
			{
				iEnterOfClickMainScreenMainCommand = ���C���R�}���h��::�푈;
				break;
			}
		case ���C���R�}���h��::�]��:
			{
				iEnterOfClickMainScreenMainCommand = ���C���R�}���h��::�]��;
				break;
			}
		case ���C���R�}���h��::�:
			{
				iEnterOfClickMainScreenMainCommand = ���C���R�}���h��::�;
				break;
			}
		default:
			{
				iEnterOfClickMainScreenMainCommand = 0; // �܂��N�����Ă��Ȃ��B�T�u�R�}���h����I�ԕK�v�����邽�߁B
			}
	}

	OnEnterMainPlayerCommand(iEnterOfClickMainScreenMainCommand, -1);
}


/*
00410613  |. 8D45 F4        |LEA     EAX, DWORD PTR SS:[EBP-C]
00410616  |. 8D4D F8        |LEA     ECX, DWORD PTR SS:[EBP-8]
00410619  |. 50             |PUSH    EAX                             ; /Arg3
0041061A  |. 51             |PUSH    ECX                             ; |Arg2
0041061B  |. 53             |PUSH    EBX                             ; |Arg1	�� ���ꂪ�I���̃R�}���h�Benum�l�ƈ�v
0041061C  |. E8 D3010000    |CALL    TENSHOU.004107F4                ; \TENSHOU.004107F4	���R�}���h�Q�B���C���J�e�S��
				�������� JMP TSMod.OnTSExeClickMainScreenMainCommand �Ə��������Ă����ۂ̏���
00410621  |. 83C4 0C        |ADD     ESP, 0C
*/
int pTenshouExeJumpFromToOnTenshouExeClickMainScreenMainCommand		 =0x41061C; // �֐��͂��̃A�h���X����AOnTenshouExeClickMainScreenMainCommand�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeClickMainScreenMainCommand  =0x4107F4; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeClickMainScreenMainCommand	 =0x410621; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeClickMainScreenMainCommand() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iCurrentCommandOfClickMainScreenMainCommand, ebx // �I��(�N���b�N)�R�}���h�̒l��ۑ�

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
	OnTenshouExeClickMainScreenMainCommandExecute();

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
		call pTenshouExeJumpCallFromToOnTenshouExeClickMainScreenMainCommand

		jmp pTenshouExeReturnLblFromOnTenshouExeClickMainScreenMainCommand
	}
}

char cmdOnTenshouExeJumpFromClickMainScreenMainCommand[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeClickMainScreenMainCommand() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeClickMainScreenMainCommand;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeClickMainScreenMainCommand + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeClickMainScreenMainCommand  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromClickMainScreenMainCommand+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeClickMainScreenMainCommand), cmdOnTenshouExeJumpFromClickMainScreenMainCommand, 5, NULL); //5�o�C�g�̂ݏ�������
}











//----------------------------------------------------------------------�T�u�R�}���h��



int iCurrentCommandOfClickMainScreenSubCommand = -1; // �I�����ꂽ�R�}���h�B
int iPointerOfMainCommandValueOfClickMainScreenSubCommand = NULL; // �I�������T�u�R�}���h���̃��C���R�}���h����ID(�������A�����Ȓl�ł͂Ȃ��A�����Z����Ă���)
void OnTenshouExeClickMainScreenSubCommandExecute() {

	int *iMainCommandValud =  (int *)iPointerOfMainCommandValueOfClickMainScreenSubCommand;
	int iMainCommandValue = ���C���R�}���h��::�{�� + (*iMainCommandValud); // �A�h���X�̒��g���e�̃R�}���h�ԍ��l�B
	iEnterOfClickMainScreenMainCommand = iMainCommandValue;

	// �T�u�R�}���h�̈�ԍ�����������0x190�A�Q�ԖڂȂ�0x191�Ȃ̂ŁA����������āA����Ƀ��C���R�}���h��1000�{�𑫂��B
	// ����ł��ꂢ�Ȉ�ӂ̔ԍ��Ƃ���B
	OnEnterMainPlayerCommand(iMainCommandValue, iMainCommandValue*1000 + iCurrentCommandOfClickMainScreenSubCommand-0x190);
}


/*
00410632  |. 8D45 F4        |LEA     EAX, DWORD PTR SS:[EBP-C]
00410635  |. 8D4D F8        |LEA     ECX, DWORD PTR SS:[EBP-8]
00410638  |. 50             |PUSH    EAX													
00410639  |. 51             |PUSH    ECX													�� ���̎���ECX�A�h���X�ɂ���l�����C���R�}���h���̒l�Ɗ֌W���Ă���B
0041063A  |. 53             |PUSH    EBX
0041063B  |. E8 56010000    |CALL    TENSHOU.00410796										�� �R�}���h�S�B�T�u�J�e�S��
				�������� JMP TSMod.OnTSExeClickMainScreenSubCommand �Ə��������Ă����ۂ̏���
00410640  |. 83C4 0C        |ADD     ESP, 0C
*/
int pTenshouExeJumpFromToOnTenshouExeClickMainScreenSubCommand		 =0x41063B; // �֐��͂��̃A�h���X����AOnTenshouExeClickMainScreenSubCommand�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeClickMainScreenSubCommand   =0x410796; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeClickMainScreenSubCommand	 =0x410640; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeClickMainScreenSubCommand() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iCurrentCommandOfClickMainScreenSubCommand, ebx // �I��(�N���b�N)�R�}���h�̒l��ۑ�

		mov iPointerOfMainCommandValueOfClickMainScreenSubCommand, ecx // ���C���R�}���h���̒l�������Ă���A�h���X��ۑ�

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
	OnTenshouExeClickMainScreenSubCommandExecute();

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
		call pTenshouExeJumpCallFromToOnTenshouExeClickMainScreenSubCommand

		jmp pTenshouExeReturnLblFromOnTenshouExeClickMainScreenSubCommand
	}
}

char cmdOnTenshouExeJumpFromClickMainScreenSubCommand[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeClickMainScreenSubCommand() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeClickMainScreenSubCommand;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeClickMainScreenSubCommand + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeClickMainScreenSubCommand  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromClickMainScreenSubCommand+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeClickMainScreenSubCommand), cmdOnTenshouExeJumpFromClickMainScreenSubCommand, 5, NULL); //5�o�C�g�̂ݏ�������
}





//----------------------------------------------------------------------�ÊσR�}���h

extern void OnLeaveMainPlayerCommand();

void OnTenshouExeClickMainScreenSeikanCommandExecute() {

	// �Êςɓ�����
	iEnterOfClickMainScreenMainCommand = ���C���R�}���h��::�Ê�;

	OnEnterMainPlayerCommand(iEnterOfClickMainScreenMainCommand, -1);
}

int isZikkouOfMainScreenSeikanCommand = -1; // �ÊσR�}���h����������Ɏ��s����
void OnTenshouExeClickMainScreenSeikanCommandExecute2() {

	// �ÊσR�}���h�����s����Ă��܂��ƁAOnLeave�����s����Ȃ��Ȃ�̂ŁA�����Ŗ����I�ɌĂяo��
	if ( isZikkouOfMainScreenSeikanCommand ) {
		OnLeaveMainPlayerCommand();
	}
}

/*
004105EE  |. E8 196D0600    |CALL    TENSHOU.0047730C
004105F3  |. E8 A6000000    |CALL    TENSHOU.0041069E				�� �u�Êρv�R�}���h�B�����͓���
				�������� JMP TSMod.OnTSExeClickMainScreenSeikanCommand �Ə��������Ă����ۂ̏���
004105F8  |. 85C0           |TEST    EAX, EAX
*/
int pTenshouExeJumpFromToOnTenshouExeClickMainScreenSeikanCommand		 =0x4105F3; // �֐��͂��̃A�h���X����AOnTenshouExeClickMainScreenSeikanCommand�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeClickMainScreenSeikanCommand    =0x41069E; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeClickMainScreenSeikanCommand	 =0x4105F8; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeClickMainScreenSeikanCommand() {
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
	
	// �Q�[���f�[�^���v���C���[�^�[�����ς�����i�K�œǂݍ��ݍX�V���Ă���
	OnTenshouExeClickMainScreenSeikanCommandExecute();

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
		call pTenshouExeJumpCallFromToOnTenshouExeClickMainScreenSeikanCommand

		mov isZikkouOfMainScreenSeikanCommand, eax // �ÊσR�}���h�́u���s�v�u����v�̌��ʕۑ�
	}

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

	// �Q�[���f�[�^���v���C���[�^�[�����ς�����i�K�œǂݍ��ݍX�V���Ă���
	OnTenshouExeClickMainScreenSeikanCommandExecute2();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeClickMainScreenSeikanCommand
	}
}

char cmdOnTenshouExeJumpFromClickMainScreenSeikanCommand[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeClickMainScreenSeikanCommand() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeClickMainScreenSeikanCommand;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeClickMainScreenSeikanCommand + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeClickMainScreenSeikanCommand  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromClickMainScreenSeikanCommand+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeClickMainScreenSeikanCommand), cmdOnTenshouExeJumpFromClickMainScreenSeikanCommand, 5, NULL); //5�o�C�g�̂ݏ�������
}


