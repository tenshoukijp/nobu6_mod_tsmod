#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

#include "TSModCommand.h"



/*
00413D9F   > 813D 7C2A4D00 21000000           CMP     DWORD PTR DS:[4D2A7C], 21
00413DA9   . 72 10                            JB      SHORT tenshou.00413DBB
00413DAB   . 6A 01                            PUSH    1
00413DAD   . FF35 C0344D00                    PUSH    DWORD PTR DS:[4D34C0]
*/

/*
0046B561   . 8D4D F8        LEA     ECX, DWORD PTR SS:[EBP-8]
0046B564   . 57             PUSH    EDI
0046B565   . E8 21B2F9FF    CALL    TENSHOU.0040678B
0046B56A   >^E9 3088FAFF    JMP     TENSHOU.00413D9F			�� 46B56A
0046B56F     90             NOP
0046B570   > 68 FFFF0000    PUSH    0FFFF
0046B575   .-E9 B6A73600    JMP     TSMOD.JmpOnTSExeNextTurnIsFieldOrCastle
0046B57A   . 83C4 0C        ADD     ESP, 0C
0046B57D   . 85C0           TEST    EAX, EAX

00413D9F   > 813D 7C2A4D00 >CMP     DWORD PTR DS:[4D2A7C], 21	�� �����֗���
00413DA9   . 72 10          JB      SHORT TENSHOU.00413DBB		�����ɍ��i���� 413DBB��
00413DAB   . 6A 01          PUSH    1
00413DAD   . FF35 C0344D00  PUSH    DWORD PTR DS:[4D34C0]
00413DB3   . E8 31960700    CALL    TENSHOU.0048D3E9
00413DB8   . 83C4 08        ADD     ESP, 8
00413DBB   > E9 2A4A0300    JMP     TENSHOU.004487EA			��



// ������CMP���C������΂悢
00413D9F     833D 7C2A4D00 >CMP     DWORD PTR DS:[4D2A7C], 21		�������̐��l��21�Ƒ傫���̂ŋȔԍ������i���Ȃ��B(���Ԃ񂾂��A�_�p�b�`��34�ȍ~���푈�̋ȂȂ̂ŁA33�ȉ����Ƃ͂����Ă�Ǝv���B)
00413DA9   . 72 10          JB      SHORT TENSHOU.00413DBB			JB �́u�����v
00413DAB   . 6A 01          PUSH    1
00413DAD   . FF35 C0344D00  PUSH    DWORD PTR DS:[4D34C0]
00413DB3   . E8 31960700    CALL    TENSHOU.0048D3E9
00413DB8   . 83C4 08        ADD     ESP, 8
00413DBB   > E9 2A4A0300    JMP     TENSHOU.004487EA
*/


// �푈���ɐ��炷�a�f�l���A����̔ԍ��ȏォ�ȉ����Ńt�B���^�����O���Ă���A���S���Y��


int iCheckValueOfWarBGMCheckExecute = FALSE;
int iPreviousBGMCheckExecute = -1;

void OnTenshouExeWarBGMCheckExecute() {
	WORD *pBGMID = (WORD *)GAMEDATASTRUCT_PLAYING_BGM_ID_ADDRESS; // ������Đ�����ׂ��Ȃ́H

	int iBGMID = *pBGMID;

	iCheckValueOfWarBGMCheckExecute = FALSE;

	// BGM���[�h��13�Ȃ̕���
	if (TSModCommand::Environment::cmdBGMMode == 0 || TSModCommand::Environment::cmdBGMMode == 1 ||  TSModCommand::Environment::cmdBGMMode == 3 ) {

		if ( iBGMID < 11 ) { // 0�I���W���ł���11�Ԗ��������؂�ւ���B�u�푈�ٔ��̎��́A���^�[������Ă��邪�A�����͐؂�ւ��Ȃ��B�푈�ĂƂ����ʂł��邪�A���Ȃ��悤�ł���B�v

			// �푈������
			if ( Is_FieldWar() || Is_CastleWar() ) {
				iCheckValueOfWarBGMCheckExecute = TRUE;
			}
		}

	// BGM���[�h��38�Ȃ̕����B�������͍��{�I�ɉ������Ȃ��Ă��ǂ��B
	} else {

		// 
		if ( iBGMID >= 0x21 ) {
			iCheckValueOfWarBGMCheckExecute = TRUE;
		} else {
			iCheckValueOfWarBGMCheckExecute = FALSE;
		}
	
	}
}


/*
00413D9F   > 813D 7C2A4D00 21000000           CMP     DWORD PTR DS:[4D2A7C], 21
00413DA9   . 72 10                            JB      SHORT tenshou.00413DBB		JVB
				�������� JMP TSMod.OnTSExeWarBGMCheck �Ə��������Ă����ۂ̏���
00413DAB   . 6A 01                            PUSH    1
00413DAD   . FF35 C0344D00                    PUSH    DWORD PTR DS:[4D34C0]
 */
int pTenshouExeJumpFromToOnTenshouExeWarBGMCheck    =0x413D9F; // �֐��͂��̃A�h���X����AOnTenshouExeWarBGMCheck�ւƃW�����v���Ă���B
int pTenshouExeReturnLbl1FromOnTenshouExeWarBGMCheck =0x413DAB; // (38�Ȃ̎���)0x21�ȏ�̎��ɂ����A�h���X (13�Ȃ̎��́A�푈�������̋Ȃ������Ȃ�F�߂�)
int pTenshouExeReturnLbl2FromOnTenshouExeWarBGMCheck =0x413DBB; // 21�����̎��ɂ����A�h���X
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeWarBGMCheck() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// TENSHOU.EXE�Ɍ��X������������������

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
	OnTenshouExeWarBGMCheckExecute();

	// �������Ȃ��Ă��ǂ��Ȃ�Υ��
	if ( iCheckValueOfWarBGMCheckExecute ) { // �푈�̂��߂̋ȂȂ�Υ��

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


			jmp pTenshouExeReturnLbl1FromOnTenshouExeWarBGMCheck // �V����BGM���Đ�����BLbl1�̕��ɃW�����v
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


			jmp pTenshouExeReturnLbl2FromOnTenshouExeWarBGMCheck // BGM�Đ����Ȃ��BLbl2�̕��ɃW�����v
		}
	}
}

/*
00444769  |. 50             PUSH    EAX
0044476A  |. E8 3A2C0200    CALL    TENSHOU.004673A9
				�������� JMP TSMod.OnTSExeWarBGMCheck �Ə��������Ă����ۂ̏���
0044476F  |. 83C4 04        ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromWarBGMCheck[13]="\xE9\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeWarBGMCheck() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeWarBGMCheck;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeWarBGMCheck + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeWarBGMCheck  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromWarBGMCheck+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeWarBGMCheck), cmdOnTenshouExeJumpFromWarBGMCheck, 12, NULL); //12�o�C�g�̂ݏ�������
}




