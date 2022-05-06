#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"

/*
���X�g�I������ۂɁA4DBD40�ɕ����̂h�c��W�J���Ă���ۂ��B�ȉ�01EF�͂����炭�o�[���[�t�H��
�ǂ��ɑ喼���g�𑫂��Ă��ǂ��B���̌�A�\�͏��Ń\�[�g����邩��B
004DBD40  [EF 01] [F4 01] F0 01 F3 01 F2 01 8B 00 F1 01 BB 00 					
004DBD50  [FF FF]=�I���t���O 8B 00 D3 00 FF FF C8 00 FF FF 
004DBD60  74 01 76 01 7B 01 7C 01 73 01 50 01 7E 01 FF FF  
004DBD70  C2 00 BE 00 D1 00 D0 00 D2 00 D3 00 D4 00 C7 00  
004DBD80  C9 00 FF FF 00 00 00 00 00 00 00 00 00 00 00 00  
004DBD90  00

���̓�����͔�I�̎��ɂ�������Ă��Ȃ�
00419DE1   . 68 40BD4D00    PUSH    TENSHOU.004DBD40
00419DE6   . 6A 26          PUSH    26
00419DE8   . E8 83D20400    CALL    TENSHOU.00467070
00419DED   . 50             PUSH    EAX                              ; |Arg1
00419DEE   . E8 B843FFFF    CALL    TENSHOU.0040E1AB                 ; \TENSHOU.0040E1AB  ������Call�Ŕ�I�őI�𕐏����X�g�ꗗ�ɂȂ肦�镐�����X�g���A4DBD40�ɍ���Ă���

00419DF3   . 83C4 18        ADD     ESP, 18
00419DF6   . 83F8 07        CMP     EAX, 7
00419DF9   . 73 23          JNB     SHORT TENSHOU.00419E1E
00419DFB   . 57             PUSH    EDI
00419DFC   . 57             PUSH    EDI

*/

/*
 * ��I���ɑ喼���g��I��ΏۂƏo�؂�悤��
 */
void OnTenshouExeHirouSelectingBushouListExecute() {

	// �I�𕐏��̕����h�c�̃��X�g���������ɓW�J����Ă���ꏊ
	WORD *pBushouListAddress = NULL;
	pBushouListAddress = (WORD *)GAMEDATASTRUCT_SELECTING_BUSHOU_LIST;

	// ���݂̃^�[�����́A���݂̑喼�i�^�[���喼�j�̔ԍ��𓾂�B
	WORD iCurDaimyo = nb6turn.daimyo;
	int iCurDaimyoBushouID = nb6daimyo[iCurDaimyo-1].attach;

	// �����ɂ����Ȃ��悤�Ɉꉞ�����̍ő�l���Ń|�C���^�����ւ̈��S���u
	for ( int ix=0; ix < GAMEDATASTRUCT_BUSHOU_NUM; ix++ ) {

		// ���X�g�̈�ԍŌ�ł���΁A�喼��ǉ�����
		if (*(pBushouListAddress+ix) == 0xFFFF) {
			// ���X�g�̈�ԍŌ�Ȃ�A�喼�ɒu��������B
			*(pBushouListAddress+ix) = iCurDaimyoBushouID;
			// ���̎����Ō�Ƃ���B
			*(pBushouListAddress+ix+1) = 0xFFFF;
			break;

		// �喼���g���������ꍇ�͉������Ȃ��B
		} else if ( *(pBushouListAddress+ix) == iCurDaimyoBushouID ) {
			break;
		}	
	}
}


/*
00419DED   . 50             PUSH    EAX                              ; |Arg1
00419DEE   . E8 B843FFFF    CALL    TENSHOU.0040E1AB                 ; \TENSHOU.0040E1AB  ������Call�Ŕ�I�őI�𕐏����X�g�ꗗ�ɂȂ肦�镐�����X�g���A4DBD40�ɍ���Ă���
				�������� JMP TSMod.OnTSExeHirouSelectingBushouList �Ə��������Ă����ۂ̏���
00419DF3   . 83C4 18        ADD     ESP, 18
*/
int pTenshouExeJumpFromToOnTenshouExeHirouSelectingBushouList		=0x419DEE; // �֐��͂��̃A�h���X����AOnTenshouExeHirouSelectingBushouList�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeHirouSelectingBushouList   =0x40E1AB; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeHirouSelectingBushouList	=0x419DF3; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeHirouSelectingBushouList() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeHirouSelectingBushouList

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
	OnTenshouExeHirouSelectingBushouListExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeHirouSelectingBushouList
	}
}

/*
00419DED   . 50             PUSH    EAX                              ; |Arg1
00419DEE   . E8 B843FFFF    CALL    TENSHOU.0040E1AB                 ; \TENSHOU.0040E1AB  ������Call�Ŕ�I�őI�𕐏����X�g�ꗗ�ɂȂ肦�镐�����X�g���A4DBD40�ɍ���Ă���
				�������� JMP TSMod.OnTSExeHirouSelectingBushouList �Ə��������Ă����ۂ̏���
00419DF3   . 83C4 18        ADD     ESP, 18
*/
char cmdOnTenshouExeJumpFromHirouSelectingBushouList[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeHirouSelectingBushouList() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeHirouSelectingBushouList;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeHirouSelectingBushouList + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeHirouSelectingBushouList  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromHirouSelectingBushouList+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeHirouSelectingBushouList), cmdOnTenshouExeJumpFromHirouSelectingBushouList, 5, NULL); //5�o�C�g�̂ݏ�������
}




