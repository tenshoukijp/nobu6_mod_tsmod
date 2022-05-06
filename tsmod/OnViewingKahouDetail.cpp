#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


// �ƕ��ID��EAX�ł킽���Ă���B�ƕ�ԍ��B����Ĕz��Ŏg�����߂ɂ́A-1���邱�ƁB
int iKahouIDOnViewingKahouDetail = -1;
int iKahouIDOnViewingKahouID = 0;
void OnTenshouExeViewingKahouDetailExecute() {
	// �ƕ�͈̔͂��ǂ������`�F�b�N���āc
	if ( 0 <= iKahouIDOnViewingKahouDetail-1 && iKahouIDOnViewingKahouDetail-1 < GAMEDATASTRUCT_KAHOU_NUM ) {
		// �ԍ����z��p��ID�ɂ��Ă����B
		iKahouIDOnViewingKahouID = iKahouIDOnViewingKahouDetail -1;
	}
}


/*
00494ABE  /$ 8B4424 04      MOV     EAX, DWORD PTR SS:[ESP+4]
00494AC2  |. 8B5424 08      MOV     EDX, DWORD PTR SS:[ESP+8]
00494AC6  |. 8941 70        MOV     DWORD PTR DS:[ECX+70], EAX
00494AC9  |. 8951 74        MOV     DWORD PTR DS:[ECX+74], EDX
00494ACC  |. E8 1F15F7FF    CALL    TENSHOU.00405FF0
				�������� JMP TSMod.OnTSExeViewingKahouDetail �Ə��������Ă����ۂ̏���
00494AD1  \. C2 0800        RETN    8
*/
int pTenshouExeJumpFromToOnTenshouExeViewingKahouDetail		=0x494ACC; // �֐��͂��̃A�h���X����AOnTenshouExeViewingKahouDetail�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeViewingKahouDetail =0x405FF0; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeViewingKahouDetail	=0x494AD1; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeViewingKahouDetail() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// ���悤�Ƃ��Ă���ƕ�ԍ���EAX�ɓ����Ă���B
		mov iKahouIDOnViewingKahouDetail, eax		

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeViewingKahouDetail

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
	OnTenshouExeViewingKahouDetailExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingKahouDetail
	}
}

/*
00494ABE  /$ 8B4424 04      MOV     EAX, DWORD PTR SS:[ESP+4]
00494AC2  |. 8B5424 08      MOV     EDX, DWORD PTR SS:[ESP+8]
00494AC6  |. 8941 70        MOV     DWORD PTR DS:[ECX+70], EAX
00494AC9  |. 8951 74        MOV     DWORD PTR DS:[ECX+74], EDX
00494ACC  |. E8 1F15F7FF    CALL    TENSHOU.00405FF0
				�������� JMP TSMod.OnTSExeViewingKahouDetail �Ə��������Ă����ۂ̏���
00494AD1  \. C2 0800        RETN    8
*/
char cmdOnTenshouExeJumpFromViewingKahouDetail[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeViewingKahouDetail() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeViewingKahouDetail;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingKahouDetail + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingKahouDetail  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromViewingKahouDetail+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingKahouDetail), cmdOnTenshouExeJumpFromViewingKahouDetail, 5, NULL); //5�o�C�g�̂ݏ�������
}




