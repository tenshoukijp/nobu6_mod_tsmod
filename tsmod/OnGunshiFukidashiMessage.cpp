#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "MessageN6PPatch.h"
#include "ScenarioMod.h"

/*
�R�t�̐����o�����b�Z�[�W�̒��O
004A102C  |. FF75 08        PUSH    DWORD PTR SS:[EBP+8]
004A102F  |. 53             PUSH    EBX
004A1030  |. E8 B8C10000    CALL    TENSHOU.004AD1ED
004A1035  |. 83C4 08        ADD     ESP, 8
*/

int isOnCurGunshiFukidashi = false;
extern bool isCurScenarioModReWrite;
void OnTenshouExeGunshiFukidashiMessageExecute() {
	isOnCurGunshiFukidashi = true;

	// �����Ŗ��O�����o���B
	char name[256] = "";

	if ( nb6fukidashirelatepersons->i1stPersonID <= 532 ) {
		strcat( name, nb6bushouname[nb6fukidashirelatepersons->i1stPersonID-1].familyname);
		strcat( name, nb6bushouname[nb6fukidashirelatepersons->i1stPersonID-1].fastname);
	}

	// �O�����b�Z�[�W�u��������ScenarioMod�ɂčs��
	if ( p_snOnRewriteFukidashiMessage ) {

		char szModMsgBuf[512] = "";

		// ���݂̃������̍ŐV�̏�Ԃ���荞��
		char szCurMsgBuf[512] = "�@�@�@�@"; // �n�����������k���Ȃǂ̔���ɍ��i���Ȃ��悤�ɂ���Ȃ��
		// ����������ǂݍ���
		ReadProcessMemory( CommonGlobalHandle::curProcess, (LPCVOID)(CPM.getMemoryAddress(LOC3)), szCurMsgBuf, sizeof(szCurMsgBuf)-1, NULL);
		szCurMsgBuf[511] = NULL; // ���S�u���b�N

		((PFNSNONREWRITEFUKIDASHIMESSAGE) p_snOnRewriteFukidashiMessage)(szModMsgBuf, name, szCurMsgBuf, nb6fukidashirelatepersons);
		if ( strlen(szModMsgBuf) > 1 ) {
			// ��������getCurVarPtr �Ƃ����悤�ɌÂ���Ԃł��n�j
			strcpy((char *)CPM.getMemoryAddress(LOC3), szModMsgBuf);
			isCurScenarioModReWrite = true;
		}
	}
}


/*
004A102C  |. FF75 08        PUSH    DWORD PTR SS:[EBP+8]
004A102F  |. 53             PUSH    EBX
004A1030  |. E8 B8C10000    CALL    TENSHOU.004AD1ED
				�������� JMP TSMod.OnTSExeGunshiFukidashiMessage �Ə��������Ă����ۂ̏���
004A1035  |. 83C4 08        ADD     ESP, 8
 */
int pTenshouExeJumpFromToOnTenshouExeGunshiFukidashiMessage    =0x4A1030; // �֐��͂��̃A�h���X����AOnTenshouExeGunshiFukidashiMessage�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeGunshiFukidashiMessage=0x4AD1ED; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeGunshiFukidashiMessage =0x4A1035; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeGunshiFukidashiMessage() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeGunshiFukidashiMessage

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
	OnTenshouExeGunshiFukidashiMessageExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeGunshiFukidashiMessage
	}
}

char cmdOnTenshouExeJumpFromGunshiFukidashiMessage[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeGunshiFukidashiMessage() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeGunshiFukidashiMessage;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeGunshiFukidashiMessage + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGunshiFukidashiMessage  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromGunshiFukidashiMessage+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeGunshiFukidashiMessage), cmdOnTenshouExeJumpFromGunshiFukidashiMessage, 5, NULL); //5�o�C�g�̂ݏ�������
}

