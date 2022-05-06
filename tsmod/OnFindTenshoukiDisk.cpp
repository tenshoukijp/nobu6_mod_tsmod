#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "TSModCommand.h"

/*
0043CDC7  |> BB 80954C00    MOV     EBX, TENSHOU.004C9580			�� �������V�ċL�̂b�c�f�B�X�N�̃A�h���X 
0043CDCC  |> 6A 01          /PUSH    1                               ; /ErrorMode = SEM_FAILCRITICALERRORS
0043CDCE  |. FF15 18754E00  |CALL    DWORD PTR DS:[<&KERNEL32.SetErr>; \SetErrorMode
0043CDD4  |. 53             |PUSH    EBX                             ; /Arg1
0043CDD5  |. 8945 FC        |MOV     DWORD PTR SS:[EBP-4], EAX       ; |
0043CDD8  |. E8 1CCD0200    |CALL    TENSHOU.00469AF9                ; \TENSHOU.00469AF9
0043CDDD  |. 83C4 04        |ADD     ESP, 4
0043CDE0  |. 85C0           |TEST    EAX, EAX
*/
int iTenshouExeFindTenshoukiDisk = TRUE;
void OnTenshouExeFindTenshoukiDiskExecute() {

	// �f�B�X�N�����ۂɓ���Ă���̂����Ȃ��̂��ɂ��āA�����Ɍ��ʂ��i�[���Ă����B
	CommonGlobalHandle::isExistDisk = iTenshouExeFindTenshoukiDisk;

	// �m�[�f�B�X�N���[�h�ł���΁A��Ɍ��������Ƃ���B
	if ( CommonGlobalHandle::isNoDiskMode ) {
		// �������ĂȂ��Ă����������Ƃ������Ƃɂ��Ă����B
		iTenshouExeFindTenshoukiDisk = TRUE;

		// �h���C�u���̃|�C���^�B���ɍ��͂Ȃɂ����Ȃ��B
		char *szDiskName = (char *)GAMEDATASTRUCT_TENSHOUKI_DRIVENAME_ADDRESS;
	}

}


/*
0043CDC7  |> BB 80954C00    MOV     EBX, TENSHOU.004C9580


0043CDD4  |. 53             |PUSH    EBX                             ; /Arg1
0043CDD5  |. 8945 FC        |MOV     DWORD PTR SS:[EBP-4], EAX       ; |
0043CDD8  |. E8 1CCD0200    |CALL    TENSHOU.00469AF9                ; \TENSHOU.00469AF9
				�������� JMP TSMod.OnTSExeFindTenshoukiDisk �Ə��������Ă����ۂ̏���
0043CDDD  |. 83C4 04        |ADD     ESP, 4
0043CDE0  |. 85C0           |TEST    EAX, EAX
 */

int pTenshouExeJumpFromToOnTenshouExeFindTenshoukiDisk    =0x43CDD8; // �֐��͂��̃A�h���X����AOnTenshouExeFindTenshoukiDisk�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeFindTenshoukiDisk=0x469AF9; // 130��ȏゾ�����ꍇ�̃W�����v��̃A�h���X
int pTenshouExeReturnLblFromOnTenshouExeFindTenshoukiDisk =0x43CDDD; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w

// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeFindTenshoukiDisk() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE�ɂ�����������������
		call pTenshouExeJumpCallFromToOnTenshouExeFindTenshoukiDisk

		// �f�B�X�N�������������H �̌��ʂ���������ł���
		mov iTenshouExeFindTenshoukiDisk, eax

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
	OnTenshouExeFindTenshoukiDiskExecute();

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

		// ���ʂ��㏑������B
		mov eax, iTenshouExeFindTenshoukiDisk

		jmp pTenshouExeReturnLblFromOnTenshouExeFindTenshoukiDisk
	}
}

/*
00444769  |. 50             PUSH    EAX
0044476A  |. E8 3A2C0200    CALL    TENSHOU.004673A9
				�������� JMP TSMod.OnTSExeFindTenshoukiDisk �Ə��������Ă����ۂ̏���
0044476F  |. 83C4 04        ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromFindTenshoukiDisk[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeFindTenshoukiDisk() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeFindTenshoukiDisk;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeFindTenshoukiDisk + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeFindTenshoukiDisk  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromFindTenshoukiDisk+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeFindTenshoukiDisk), cmdOnTenshouExeJumpFromFindTenshoukiDisk, 5, NULL); //5�o�C�g�̂ݏ�������
}













