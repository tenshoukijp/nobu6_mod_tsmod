
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXBushou_All.h"
#include "ScenarioMod.h"
#include "TSModCommand.h"

/*
004C8ED0  94 4E 97 EE 00 00 00 00 88 EA 96 E5 00 00 00 00  �N��....���....
004C8EE0  8C 52 92 63 92 B7 00 00 90 67 95 AA 00 00 00 00  �R�c��..�g��....
004C8EF0  91 E5 96 BC 00 00 00 00 95 90 8F AB 96 BC 00 00  �喼....������..

004D13D0  [F0 8E 4C] 00 [E0 8E 4C] 00 [BC FE 4C] 00 [A0 FF 4C] 00  �喼 �R�c�� ��� �B��
004D13E0  [B4 FE 4C] 00 [B0 B7 4B] 00 [AC FE 4C] 00 [A4 FE 4C] 00  �Q�l �P �ҋ@ ���S


// ��Ԃ�v������֐�
// �ȉ��̂R�̏ꏊ�ŏ�Ԃ́u������|�C���^�̔z��ւ̃|�C���^�v�����p�����B
�Q�ƃf�[�^ �F TENSHOU:.text -> 004D13D0..004D13D0
���ڽ      �ި�������                                ����
�P�J���� 00438F3B   MOV     EAX, DWORD PTR DS:[EAX*4+4D13D0]  �ʏ�̕������X�g�̎��ɕ������Ƃɂ�����ʉ߂���B(��{�I�ɘQ�l�o�p�̎�)
�Q�J���� 00475A62   PUSH    DWORD PTR DS:[EAX*4+4D13D0]     (��Ȃǂ��N���b�N������
�R�ӏ��� 00495290   PUSH    DWORD PTR DS:[EAX*4+4D13D0]     (�n�}�ɂ��喼���̂Ƃ���)
*/





static int GetOverRewriteJotaiAddress(int iBushouID, int iJotai=-1 ) {

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// �f�t�H���g�͌��݂̃|�W�V����������
		if ( iJotai==-1) {
			iJotai = nb6bushouname[iBushouID].State;
		}

		// ScenarioMod�̌Ăяo���B( return ���Ă���̂�ScenarioMod���ɓ����)
		if ( p_snOnViewingBushouStateName ) {
			int iPointerAddress = 0;
			p_snOnViewingBushouStateName( &iPointerAddress, iBushouID+1, iJotai );
			if ( iPointerAddress ) {
				return int(iPointerAddress);
			}
		}

		/*
		if ( TSModCommand::UltraBushou::cmdExists ) { // �ȉ��A������Əd�����̂Ńt�B���^�[
			if ( iJotai == 0 ) {
				if ( isLancelotTartare(iBushouID) ) { // �ݽۯĂȂ�
					static char szJotai[] = "������"; // ۽۰ر�
					return int(szJotai);
				}
			}
		}
		*/

		if ( iJotai == 0 ) { // 0�喼,1�R��,2����,3�B��,4�Q�l,5�P,6,�ҋ@,7���S
			int iDaimyoID = nb6bushouref[iBushouID].attach - 1;
			if ( 0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM ) {
				int iSimbol = nb6daimyo[iDaimyoID].symbol;
				if ( iSimbol == 41 ) { // �{�莛�Ɩ�
					static char szJotai[] = "�@��";
					return int(szJotai);
				}
			}

		// �Q�l�����Ǒm���Ȃ�_���i�s�r�m�j�B
		} else if ( iJotai == 4 && nb6bushouref[iBushouID].job == 0x40 ) {
			static char szJotai[] = "�Q�l";
			return int(szJotai);
		}

	}


	return NULL;
}





//---------------------------------------------------------------------------------------------------------------------------



/*
 * �g����\������Ƃ��ɁA�����ԍ����E���Ă���
 * EAX==004D00BD�݂����ɉ������̃r�b�g�������ԍ�
 */
int iBushouBangouOfShowingJotai1 = -1;
int iJotaiNumberOfShowingJotai1 = -1;
int iEAXPointerOfShowingJotai1 = NULL;
void OnTenshouExeShowingJotai1Execute() {

	int iBushouID = (iBushouBangouOfShowingJotai1 & 0xFFFF) - 1; // �������������ԍ�

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int ipointer = GetOverRewriteJotaiAddress(iBushouID);

		if ( ipointer ) {
			iEAXPointerOfShowingJotai1 = ipointer;
		}
	}
}



/*
00438F33   > E8 6214FDFF    CALL    TENSHOU.0040A39A
				�������� JMP TSMod.OnTSExeShowingMibunBefore �Ə��������Ă����ۂ̏���
00438F38   . 83C4 04        ADD     ESP, 4
00438F3B   . 8B0485 D0134D0>MOV     EAX, DWORD PTR DS:[EAX*4+4D13D0]
00438F42   > 5E             POP     ESI                              ;  TENSHOU.004E66E8
*/
int pTenshouExeJumpFromToOnTenshouExeShowingJotai1		=0x438F33; // �֐��͂��̃A�h���X����AOnTenshouExeShowingJotai1�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeShowingJotai1  =0x40A39A; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeShowingJotai1	=0x438F42; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeShowingJotai1() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// �����ԍ��ۑ�
		mov iBushouBangouOfShowingJotai1, ecx

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeShowingJotai1

		// ���XTENSHOU.EXE���ɂ��������̂�������
		ADD     ESP, 4

		// ��Ԕԍ��̕ۑ�
		mov iJotaiNumberOfShowingJotai1, eax

		// (�E�Ӓl��EAX�Ƃ͐g���̒l�A�uEAX*4+0x4D13B0�v���e�g���̕�����ւ̃|�C���^�������Ă���A�h���X�ł��ˁA���Ӓl��EAX�Ŏ󂯎���Ă���)
		MOV     EAX, DWORD PTR DS:[EAX*4+0x4D13D0]

		// ���̕�����ւ̃|�C���^�Ƃ��ĕۑ�
		mov iEAXPointerOfShowingJotai1, EAX

		// eax�����ւƖ߂�
		mov eax, iJotaiNumberOfShowingJotai1

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
	OnTenshouExeShowingJotai1Execute();

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

		// ������̃|�C���^�̏㏑��
		mov EAX, iEAXPointerOfShowingJotai1

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingJotai1
	}
}

char cmdOnTenshouExeJumpFromShowingJotai1[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�W�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeShowingJotai1() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeShowingJotai1;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingJotai1 + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromShowingJotai1+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingJotai1), cmdOnTenshouExeJumpFromShowingJotai1, 5, NULL); //5�o�C�g�̂ݏ�������
}



//---------------------------------------------------------------------------------------------------------------------------



/*
 * �g����\������Ƃ��ɁA�����ԍ����E���Ă���
 * EAX==004D00BD�݂����ɉ������̃r�b�g�������ԍ�
 */
int iBushouBangouOfShowingJotai2 = -1;
int iJotaiNumberOfShowingJotai2 = -1;
int iEAXPointerOfShowingJotai2 = NULL;
void OnTenshouExeShowingJotai2Execute() {

	int iBushouID = (iBushouBangouOfShowingJotai2 & 0xFFFF) - 1; // �������������ԍ�

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int ipointer = GetOverRewriteJotaiAddress(iBushouID);

		if ( ipointer ) {
			iEAXPointerOfShowingJotai2 = ipointer;
		}
	}
}



/*
00475A50  |. FF75 FE                  PUSH    DWORD PTR SS:[EBP-2]
00475A53  |. E8 874CF9FF              CALL    TENSHOU.0040A6DF
				�������� JMP TSMod.OnTSExeShowingMibunBefore �Ə��������Ă����ۂ̏���
00475A58  |. 83C4 04                  ADD     ESP, 4
00475A5B  |. 83F8 01                  CMP     EAX, 1
00475A5E  |. 1BC0                     SBB     EAX, EAX
00475A60  |. F7D8                     NEG     EAX
00475A62  |. FF3485 D0134D00          PUSH    DWORD PTR DS:[EAX*4+4D13D0]
00475A69  |. 68 7CAD4B00              PUSH    TENSHOU.004BAD7C                 ;  ASCII "%s %s"
*/
int pTenshouExeJumpFromToOnTenshouExeShowingJotai2		=0x475A53; // �֐��͂��̃A�h���X����AOnTenshouExeShowingJotai2�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeShowingJotai2  =0x40A6DF; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeShowingJotai2	=0x475A69; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeShowingJotai2() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// EAX���Ԏ؂肷��̂ŁA��U�ۑ�
		mov iJotaiNumberOfShowingJotai2, eax

		// �����̔ԍ�
		mov eax,  DWORD PTR SS:[EBP-0x2]

		// �����̔ԍ��̕ۑ�
		mov iBushouBangouOfShowingJotai2, eax

		// eax�����ւƖ߂�
		mov eax, iJotaiNumberOfShowingJotai2

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeShowingJotai2

		// ���XTENSHOU.EXE���ɂ��������̂�������
		ADD     ESP, 4
		CMP     EAX, 1
		SBB     EAX, EAX
		NEG     EAX

		// ��Ԕԍ��̕ۑ�
		mov iJotaiNumberOfShowingJotai2, eax

		// EAX���ĂъԎ؂�
		// (�E�Ӓl��EAX�Ƃ͐g���̒l�A�uEAX*4+0x4D13B0�v���e�g���̕�����ւ̃|�C���^�������Ă���A�h���X�ł��ˁA���Ӓl��EAX�Ŏ󂯎���Ă���)
		MOV     EAX, DWORD PTR DS:[EAX*4+0x4D13D0]

		mov iEAXPointerOfShowingJotai2, EAX

		// eax�����ւƖ߂�
		mov eax, iJotaiNumberOfShowingJotai2

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
	OnTenshouExeShowingJotai2Execute();

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

		// ������̃|�C���^
		push iEAXPointerOfShowingJotai2

		// EAX�����̏�Ԕԍ��ւƖ߂�
		mov EAX, iJotaiNumberOfShowingJotai2

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingJotai2
	}
}

char cmdOnTenshouExeJumpFromShowingJotai2[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�W�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeShowingJotai2() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeShowingJotai2;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingJotai2 + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromShowingJotai2+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingJotai2), cmdOnTenshouExeJumpFromShowingJotai2, 5, NULL); //5�o�C�g�̂ݏ�������
}






/*
 * �g����\������Ƃ��ɁA�����ԍ����E���Ă���
 * EAX==004D00BD�݂����ɉ������̃r�b�g�������ԍ�
 */
int iBushouBangouOfShowingJotai3 = -1;
int iJotaiNumberOfShowingJotai3 = -1;
int iEAXPointerOfShowingJotai3 = NULL;
void OnTenshouExeShowingJotai3Execute() {

	int iBushouID = (iBushouBangouOfShowingJotai3 & 0xFFFF) - 1; // �������������ԍ�

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int ipointer = GetOverRewriteJotaiAddress(iBushouID);

		if ( ipointer ) {
			iEAXPointerOfShowingJotai3 = ipointer;
		}
	}
}



/*
00495281  |. FF7424 16      PUSH    DWORD PTR SS:[ESP+16]
00495285  |. E8 1051F7FF    CALL    TENSHOU.0040A39A
				�������� JMP TSMod.OnTSExeShowingMibunBefore �Ə��������Ă����ۂ̏���
0049528A  |. 66:8B0F        MOV     CX, WORD PTR DS:[EDI]
0049528D  |. 83C4 04        ADD     ESP, 4
00495290  |. FF3485 D0134D0>PUSH    DWORD PTR DS:[EAX*4+4D13D0]
00495297  |. 51             PUSH    ECX                              ;  TENSHOU.004D0019

*/
int pTenshouExeJumpFromToOnTenshouExeShowingJotai3		=0x495285; // �֐��͂��̃A�h���X����AOnTenshouExeShowingJotai3�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeShowingJotai3  =0x40A39A; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeShowingJotai3	=0x495297; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeShowingJotai3() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// EAX���Ԏ؂肷��̂ŁA��U�ۑ�
		mov iJotaiNumberOfShowingJotai3, eax

		// �����̔ԍ�
		mov eax,  DWORD PTR SS:[ESP]

		// �����̔ԍ��̕ۑ�
		mov iBushouBangouOfShowingJotai3, eax

		// eax�����ւƖ߂�
		mov eax, iJotaiNumberOfShowingJotai3

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeShowingJotai3

		// ���XTENSHOU.EXE���ɂ��������̂�������
		MOV     CX, WORD PTR DS:[EDI]
		ADD     ESP, 4

		// ��Ԕԍ��̕ۑ�
		mov iJotaiNumberOfShowingJotai3, eax

		// EAX���ĂъԎ؂�
		// (�E�Ӓl��EAX�Ƃ͐g���̒l�A�uEAX*4+0x4D13B0�v���e�g���̕�����ւ̃|�C���^�������Ă���A�h���X�ł��ˁA���Ӓl��EAX�Ŏ󂯎���Ă���)
		MOV     EAX, DWORD PTR DS:[EAX*4+0x4D13D0]

		mov iEAXPointerOfShowingJotai3, EAX

		// eax�����ւƖ߂�
		mov eax, iJotaiNumberOfShowingJotai3

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
	OnTenshouExeShowingJotai3Execute();

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

		// ������̃|�C���^
		push iEAXPointerOfShowingJotai3

		// EAX�����̏�Ԕԍ��ւƖ߂�
		mov EAX, iJotaiNumberOfShowingJotai3

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingJotai3
	}
}

char cmdOnTenshouExeJumpFromShowingJotai3[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�W�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeShowingJotai3() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeShowingJotai3;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingJotai3 + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromShowingJotai3+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingJotai3), cmdOnTenshouExeJumpFromShowingJotai3, 5, NULL); //5�o�C�g�̂ݏ�������
}

