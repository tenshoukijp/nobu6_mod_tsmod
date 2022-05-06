#include "WinTarget.h"

#include "CommonGlobalHandle.h"

/*
 * �f�B�X�N�̋󂫗e�ʂ��`�F�b�N����鎞

/*
00479CCC  |. E8 D7000000    CALL    TENSHOU.00479DA8
00479CD1     3D 00A00A00    CMP     EAX, 0AA000
00479CD6     7D 28          JGE     SHORT TENSHOU.00479D00
*/

/*
BOOL GetDiskFreeSpaceEx(
  LPCTSTR lpDirectoryName,                 // �f�B���N�g����
  PULARGE_INTEGER lpFreeBytesAvailable,    // �Ăяo���������p�ł���o�C�g��
  PULARGE_INTEGER lpTotalNumberOfBytes,    // �f�B�X�N�S�̂̃o�C�g��
  PULARGE_INTEGER lpTotalNumberOfFreeBytes // �f�B�X�N�S�̂̋󂫃o�C�g��
);
*/
int iEAXOfCheckDiskSpaceExecute = 0; // �f�B�X�N�󂫗e�ʒl������B�A���A�ő�ł�32bit��signed int max�� 0x7FFFFFFF �𒴂��Ȃ��B

void OnTenshouExeCheckDiskSpaceExecute() {
    ULARGE_INTEGER i64UseBytes;   // �Ăяo���������p�ł���o�C�g��
    ULARGE_INTEGER i64TotalBytes; // �f�B�X�N�S�̂̃o�C�g��
    ULARGE_INTEGER i64FreeBytes;  // �f�B�X�N�S�̂̋󂫃o�C�g��
	GetDiskFreeSpaceEx( NULL, &i64UseBytes, &i64TotalBytes, &i64FreeBytes);

	ULONGLONG ifreebyte = i64TotalBytes.QuadPart; //ULARGE_INTEGER�͓���ȋ��p�̂ŁA�S�̂�ULONGLONG�^��QuadPart�B

	// 32bit�̕����tint���I�[�o�[���Ă�����A�����tINT��MAX�Ƃ���B
	if (ifreebyte > 0x7FFFFFFF ) {
		ifreebyte = 0x7FFFFFFF;
	}

	iEAXOfCheckDiskSpaceExecute = (int)ifreebyte;
	
}

/*
00479CCC  |. E8 D7000000    CALL    TENSHOU.00479DA8
00479CD1     3D 00A00A00    CMP     EAX, 0AA000
00479CD6     7D 28          JGE     SHORT TENSHOU.00479D00
*/
int pTenshouExeJumpFromToOnTenshouExeCheckDiskSpace		=0x479CCC; // �֐��͂��̃A�h���X����AOnTenshouExeCheckDiskSpace�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeCheckDiskSpace =0x479DA8; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeCheckDiskSpace	=0x479CD1; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeCheckDiskSpace() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// ���X�̏���
		CALL pTenshouExeJumpCallFromToOnTenshouExeCheckDiskSpace

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
	OnTenshouExeCheckDiskSpaceExecute();

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

		mov eax, iEAXOfCheckDiskSpaceExecute // EAX�̒l�͋󂫗e�ʁB����̊֐��ŋ󂫗e�ʂ��v�Z���čđ���B�ő�l��32bit signed int��0x7FFFFFFF�܂łɗ}���Ă���B

		jmp pTenshouExeReturnLblFromOnTenshouExeCheckDiskSpace
	}
}


char cmdOnTenshouExeJumpFromCheckDiskSpace[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeCheckDiskSpace() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeCheckDiskSpace;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeCheckDiskSpace + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeCheckDiskSpace  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromCheckDiskSpace+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeCheckDiskSpace), cmdOnTenshouExeJumpFromCheckDiskSpace, 5, NULL); //5�o�C�g�̂ݏ�������
}




