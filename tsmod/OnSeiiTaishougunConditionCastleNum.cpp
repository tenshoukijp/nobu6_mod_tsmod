#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "TSModCommand.h"

/*
00413B5C   . 3D 82000000    CMP     EAX, 82								130�̍�
00413B61   . 73 07          JNB     SHORT TENSHOU.00413B6A								
00413B63   . 33C0           XOR     EAX, EAX								
00413B65   . E9 37030000    JMP     TENSHOU.00413EA1								
00413B6A   > 6A 04          PUSH    4								
00413B6C   . 56             PUSH    ESI								
00413B6D   . E8 0E190900    CALL    TENSHOU.004A5480								
00413B72   . 83C4 08        ADD     ESP, 8								
00413B75   . 85C0           TEST    EAX, EAX								
00413B77   . 74 07          JE      SHORT TENSHOU.00413B80								
00413B79   . 33C0           XOR     EAX, EAX								
00413B7B   . E9 21030000    JMP     TENSHOU.00413EA1								
00413B80   > 56             PUSH    ESI								
00413B81   . E8 251B0900    CALL    TENSHOU.004A56AB								
00413B86   . 66:8BD8        MOV     BX, AX								
00413B89   . 83C4 04        ADD     ESP, 4								
00413B8C   . 56             PUSH    ESI								
00413B8D   . E8 48140900    CALL    TENSHOU.004A4FDA								
00413B92   . 66:A3 907C4D00 MOV     WORD PTR DS:[4D7C90], AX								
00413B98   . 83C4 04        ADD     ESP, 4								
00413B9B   . 66:8BF8        MOV     DI, AX								
00413B9E   . 66:81FB 8000   CMP     BX, 80								128�i�������䏊�������)�̎x�z��
00413BA3   . 74 32          JE      SHORT TENSHOU.00413BD7								
00413BA5   . 66:81FB B500   CMP     BX, 0B5								�]�ˏ�(=181)
00413BAA   . 75 0E          JNZ     SHORT TENSHOU.00413BBA								
00413BAC   . 57             PUSH    EDI								
00413BAD   . E8 F65EFFFF    CALL    TENSHOU.00409AA8								
00413BB2   . 83C4 04        ADD     ESP, 4								
*/

/*
 * ���Α叫�R�ɂȂ�����̂����A�Œ�鐔�����肳�ꂽ��
 */
// �����Ƃ��Ă킽���Ă�������Ώۂ̌R�cID
int iSeiiTaishoConditionCastleNumDaimyoID = -1;
// ��Ԏx�z���ɂ����߂Ă���Ƃ��낪�������͂̎��ۂ̌��݂̏鐔
int iSeiiTaishoConditionCastleNum = -1;
int iSeiiTaishoConditionCastleNeedNum = 130;

// �鐔�����Α叫�R�Ƃ��č��i���Ă��邩�H
int iSeiiTaishoConditionCastleNumOver = 0;


void OnTenshouExeSeiiTaishoConditionCastleNumExecute() {

	// �u�鐔�����Α叫�R�Ƃ��č��i���Ă��邩�H�v�͖��x���Z�b�g
	iSeiiTaishoConditionCastleNumOver = 0;

	// TSMod.txt�ɂāA�Œ�鐔���w�肳��Ă���΁A�����K�v���̃f�t�H���g���Ƃ���Ƃ���
	if ( TSModCommand::AllDaimyo::cmdSeiiTaishogunNeedCastleNum > 0 ) {
		iSeiiTaishoConditionCastleNeedNum = TSModCommand::AllDaimyo::cmdSeiiTaishogunNeedCastleNum;
	}
	// �K�v���𖞂����Ă���΂n�j�D
	if ( iSeiiTaishoConditionCastleNum >= iSeiiTaishoConditionCastleNeedNum ) {
		iSeiiTaishoConditionCastleNumOver = 1;
	}

}


/*
00413B5C   . 3D 82000000    CMP     EAX, 82								0x82��130�̍�������	
				�������� JMP TSMod.OnTSExeSeiiTaishoConditionCastleNum �Ə��������Ă����ۂ̏���
00413B61   . 73 07          JNB     SHORT TENSHOU.00413B6A				(��菬�����Ȃ����==130�ȏ�ł���� TENSHOU.00413B6A��	
00413B63   . 33C0           XOR     EAX, EAX									
00413B65   . E9 37030000    JMP     TENSHOU.00413EA1									
 */

int pTenshouExeJumpFromToOnTenshouExeSeiiTaishoConditionCastleNum    =0x413B5C; // �֐��͂��̃A�h���X����AOnTenshouExeSeiiTaishoConditionCastleNum�ւƃW�����v���Ă���B
int pTenshouExeJumpFromToOnTenshouExeSeiiTaishoConditionCastleNumOver=0x413B6A; // 130��ȏゾ�����ꍇ�̃W�����v��̃A�h���X
int pTenshouExeReturnLblFromOnTenshouExeSeiiTaishoConditionCastleNum =0x413B63; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w

// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeSeiiTaishoConditionCastleNum() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���݂̔���̑ΏۂƂȂ�喼ID���T���Ă���
		mov iSeiiTaishoConditionCastleNumDaimyoID, esi;

		// ��ԑ������͂̏鐔
		mov iSeiiTaishoConditionCastleNum, eax;

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
	OnTenshouExeSeiiTaishoConditionCastleNumExecute();

	// �������Α叫�R�Ƃ��ĕK�v�Ƃ����鐔�ȏ�ł���΁c
	if ( iSeiiTaishoConditionCastleNumOver ) {
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

			jmp pTenshouExeJumpFromToOnTenshouExeSeiiTaishoConditionCastleNumOver
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

			jmp pTenshouExeReturnLblFromOnTenshouExeSeiiTaishoConditionCastleNum
		}
	}
}

/*
00444769  |. 50             PUSH    EAX
0044476A  |. E8 3A2C0200    CALL    TENSHOU.004673A9
				�������� JMP TSMod.OnTSExeSeiiTaishoConditionCastleNum �Ə��������Ă����ۂ̏���
0044476F  |. 83C4 04        ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromSeiiTaishoConditionCastleNum[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeSeiiTaishoConditionCastleNum() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeSeiiTaishoConditionCastleNum;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSeiiTaishoConditionCastleNum + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeSeiiTaishoConditionCastleNum  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromSeiiTaishoConditionCastleNum+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSeiiTaishoConditionCastleNum), cmdOnTenshouExeJumpFromSeiiTaishoConditionCastleNum, 5, NULL); //5�o�C�g�̂ݏ�������
}













