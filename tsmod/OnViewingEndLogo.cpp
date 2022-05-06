#include "WinTarget.h"

#include <sys/types.h>
#include <sys/stat.h>

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "FuncPlayMovie.h"
#include "TSModCommand.h"
#include "GameDataStruct.h"

/*
  �Ōネ�S��\�����鏊
 */

/*
00496AC2  |. 50             PUSH EAX                                 ; /Arg5
00496AC3  |. 6A 70          PUSH 70                                  ; |Arg4 = 00000070
00496AC5  |. 68 D0000000    PUSH 0D0                                 ; |Arg3 = 000000D0
00496ACA  |. A1 44A94D00    MOV EAX,DWORD PTR DS:[4DA944]            ; |
00496ACF  |. 83C0 60        ADD EAX,60                               ; |
00496AD2  |. 8BCE           MOV ECX,ESI                              ; |
00496AD4  |. 50             PUSH EAX                                 ; |Arg2
00496AD5  |. A1 40A94D00    MOV EAX,DWORD PTR DS:[4DA940]            ; |
00496ADA  |. 05 D8000000    ADD EAX,0D8                              ; |
00496ADF  |. 50             PUSH EAX                                 ; |Arg1
00496AE0     E8 04760100    CALL TENSHOU.004AE0E9							�� LOGO �̏㕔
00496AE5  |. 6A 04          PUSH 4
00496AE7  |. 8BCF           MOV ECX,EDI
00496AE9  |. 53             PUSH EBX
00496AEA  |. 68 F6130000    PUSH 13F6
00496AEF  |. E8 11470100    CALL TENSHOU.004AB205
00496AF4  |. 50             PUSH EAX                                 ; /Arg5
00496AF5  |. 6A 68          PUSH 68                                  ; |Arg4 = 00000068
00496AF7  |. 68 10010000    PUSH 110                                 ; |Arg3 = 00000110
00496AFC  |. A1 44A94D00    MOV EAX,DWORD PTR DS:[4DA944]            ; |
00496B01  |. 05 E0000000    ADD EAX,0E0                              ; |
00496B06  |. 8BCE           MOV ECX,ESI                              ; |
00496B08  |. 50             PUSH EAX                                 ; |Arg2
00496B09  |. A1 40A94D00    MOV EAX,DWORD PTR DS:[4DA940]            ; |
00496B0E  |. 05 B8000000    ADD EAX,0B8                              ; |
00496B13  |. 50             PUSH EAX                                 ; |Arg1
00496B14     E8 D0750100    CALL TENSHOU.004AE0E9							�� LOGO �̉���
00496B19  |. 68 70304D00    PUSH TENSHOU.004D3070
00496B1E     E8 90C7F8FF    CALL TENSHOU.004232B3
00496B23  |. 83C4 04        ADD ESP,4
00496B26  |. 6A 08          PUSH 8										   �� LOGO�̑҂�����
00496B28     E8 C7C5F8FF    CALL TENSHOU.004230F4
00496B2D  |. 83C4 04        ADD ESP,4
00496B30  |. E8 B9C7F8FF    CALL TENSHOU.004232EE
00496B35  |> 5F             POP EDI
00496B36  |. 5E             POP ESI
00496B37  |. 5B             POP EBX
00496B38  |. 8BE5           MOV ESP,EBP
00496B3A  |. 5D             POP EBP
00496B3B  \. C3             RETN
*/



void WritePatchUnviewingEndingLogo5Byte( int iAddress, char *szAsm ) {
	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(iAddress), szAsm, 5, NULL); //call 1����5�o�C�g��������
}

void WritePatchUnviewingEndingLogo1Byte( int iAddress, char Value ) {
	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(iAddress), &Value, 1, NULL);
}


// ���S�̏㉺�\���Q�ƁA�҂����Ԃ��W���O��
void SetUnviewingEndingLogo() {

	struct stat st;
	if (stat("Movie\\ENDLOGO.avi", &st) == 0) {
		char szNop[] = "\x90\x90\x90\x90\x90";
		WritePatchUnviewingEndingLogo5Byte( 0x496AE0, szNop ); // 00496AE0     E8 04760100    CALL TENSHOU.004AE0E9 �� LOGO �̏㕔
		WritePatchUnviewingEndingLogo5Byte( 0x496B14, szNop ); // 00496B14     E8 D0750100    CALL TENSHOU.004AE0E9 �� LOGO �̉���

		int iWaitTime = 0; // 8��0
		WritePatchUnviewingEndingLogo1Byte( 0x496B27, iWaitTime ); // 00496B26  |. 6A 08          PUSH 8   �� LOGO�̑҂�����
	}
}



//-----------------------------------------------------------------------------------------


/*
 �Ō�̃��S�̕\�����I���̂�҂��Ă��鎞
 */

/*
00496B26  |. 6A 08          PUSH 8										   �� LOGO�̑҂�����
00496B28     E8 C7C5F8FF    CALL TENSHOU.004230F4
00496B2D  |. 83C4 04        ADD ESP,4
*/


void OnTenshouExeEndLogoWaitingExecute() {

	// ���j���[��^�C�g�����B����I��ł���ꍇ�́A���j���[�͑����ɉB��
	// ���j���[�o�[�ƃ^�C�g���o�[���B�����[�h�ŁA���A�X�N���[���̃T�C�Y�I�����I���Ă���(���j���[�́u�I���v��Valild�ɂȂ�����
	if ( TSModCommand::Environment::cmdNoTitleBarAndMenuBar && CommonGlobalHandle::IsAfterScreenSizeSelected ) {
		// ���j���[�͂����B��
		SetMenu( CommonGlobalHandle::tenshouWND, NULL );
	}

	// �����ɗ��邽�тɃG���f�B���O���[�h�I��
	CommonGlobalHandle::isEndingMode = false;

	// �v���C���S���R�cID�����ׂăN���A���邱�ƂŃf����ԂƓ��l�Ƃ���
	WORD iGundanID = 0;
	for (int iPG = 0; iPG < GAMEDATASTRUCT_PLAYERS8_NUM; iPG++) {
		// �v���C���[���S�����Ă���R�c�̂h�c���N���A
		nb6players8[iPG].gundan = 0xFFFF;

	}

	// ���ɂ������ɕ��l�܂�ɂȂ��Ă��邱�Ƃ�����B�ȉ��̃A�h���X��0xFF01�����Ȃ��Ƃ��߂Ȗ͗l
	// �G���f�B���O��A���C�����j���[�ɖ߂�Ȃ����Ƃɂ�鋭���ȃp�b�`
	// WORD* nextModeForceAddressAfterEndingMode = (WORD*)0x4D1584;
	// *nextModeForceAddressAfterEndingMode = 0xFF01;


	// ���̃��[�r�[�̓n�[�h�f�B�X�N�ɑ��݂��邩
	struct stat st;
	if (stat("Movie\\ENDLOGO.avi", &st) == 0) {
		FuncPlayMovie( "ENDLOGO", TRUE, TRUE );
	}

}


/*
00496B26  |. 6A 08          PUSH 8										   �� LOGO�̑҂�����
00496B28     E8 C7C5F8FF    CALL TENSHOU.004230F4
				�������� JMP TSMod.OnTSExeEndLogoWaiting �Ə��������Ă����ۂ̏���
00496B2D  |. 83C4 04        ADD ESP,4
*/
int pTenshouExeJumpFromToOnTenshouExeEndLogoWaiting		=0x496B28; // �֐��͂��̃A�h���X����AOnTenshouExeEndLogoWaiting�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeEndLogoWaiting =0x4230F4; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeEndLogoWaiting	=0x496B2D; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeEndLogoWaiting() {
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
	OnTenshouExeEndLogoWaitingExecute();

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
		call pTenshouExeJumpCallFromToOnTenshouExeEndLogoWaiting

		jmp pTenshouExeReturnLblFromOnTenshouExeEndLogoWaiting
	}
}

/*
*/
char cmdOnTenshouExeJumpFromEndLogoWaiting[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeEndLogoWaiting() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeEndLogoWaiting;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeEndLogoWaiting + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeEndLogoWaiting  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromEndLogoWaiting+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeEndLogoWaiting), cmdOnTenshouExeJumpFromEndLogoWaiting, 5, NULL); //5�o�C�g�̂ݏ�������
}




