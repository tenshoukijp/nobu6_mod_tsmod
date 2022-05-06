
#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"
#include "SoundServer.h"
#include "FuncPlayBGM.h"


int iNextPlayTrackingNo = -1;

void OnTenshouExeN6PAudioPlayTrackingExecute() {

	// ScenarioMod�̃I�[�f�B�I�g���b�N�̂���ւ��֐������݂���B
	if ( p_snOnN6PAudioPlayTrack ) { 
		int iNewPlayTrackingNo = ((PFNSNONN6PAUDIOPLAYTRACK) p_snOnN6PAudioPlayTrack)( iNextPlayTrackingNo ); // �_�p�b�`�Ɠ����l�B
		// �A��l��-1�ȊO�̎��ɍ����ւ���
		if ( iNewPlayTrackingNo != -1 ) {
			iNextPlayTrackingNo = iNewPlayTrackingNo; // �L���Ȓl���Ԃ��ė��Ă��������B
		}
	}
}


/*
0048D448  |. FF7424 0C      PUSH    DWORD PTR SS:[ESP+C]

				�������� JMP TSMod.OnTSExeN6PAudioPlayTracking �Ə��������Ă����ۂ̏���
0048D44C  |. 8D4E 01        LEA     ECX, DWORD PTR DS:[ESI+1]
0048D44F  |. 51             PUSH    ECX
 */
int pTenshouExeJumpFromToOnTenshouExeN6PAudioPlayTracking    =0x48D448; // �֐��͂��̃A�h���X����AOnTenshouExeN6PAudioPlayTracking�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeN6PAudioPlayTracking =0x48D44F; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeN6PAudioPlayTracking() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// TENSHOU.EXE�Ɍ��X������������������
		PUSH    DWORD PTR SS:[ESP+0xC]
		LEA     ECX, DWORD PTR DS:[ESI+1]

		mov iNextPlayTrackingNo, ecx

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
	OnTenshouExeN6PAudioPlayTrackingExecute();

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

		mov ecx, iNextPlayTrackingNo

		jmp pTenshouExeReturnLblFromOnTenshouExeN6PAudioPlayTracking
	}
}

/*
00444769  |. 50             PUSH    EAX
0044476A  |. E8 3A2C0200    CALL    TENSHOU.004673A9
				�������� JMP TSMod.OnTSExeN6PAudioPlayTracking �Ə��������Ă����ۂ̏���
0044476F  |. 83C4 04        ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromN6PAudioPlayTracking[8]="\xE9\x90\x90\x90\x90\x90\x90";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeN6PAudioPlayTracking() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeN6PAudioPlayTracking;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeN6PAudioPlayTracking + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeN6PAudioPlayTracking  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromN6PAudioPlayTracking+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeN6PAudioPlayTracking), cmdOnTenshouExeJumpFromN6PAudioPlayTracking, 7, NULL); //7�o�C�g�̂ݏ�������
}




