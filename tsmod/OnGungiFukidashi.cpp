#define  _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "MessageN6PPatch.h"
#include "ScenarioMod.h"


// �}�E�X�őI�������R�c���̌R�c�ԍ�
int iOnGungiSelectingGudanNoNo = -1;

/*
004161E4  |. 50             PUSH    EAX                     �� ���̎��A�����R�c�ԍ�    
004161E5  |. E8 34E80300    CALL    TENSHOU.00454A1E
				�������� JMP TSMod.OnTSExeGungiGudanNo �Ə��������Ă����ۂ̏���
004161EA  |. 83C4 04        ADD     ESP, 4
004161ED  |. 48             DEC     EAX
004161EE  |. 8BD8           MOV     EBX, EAX
004161F0  |. 8B4D FC        MOV     ECX, DWORD PTR SS:[EBP-4]
*/
int pTenshouExeJumpFromToOnTenshouExeGungiGudanNo	 =0x4161E5; // �֐��͂��̃A�h���X����AOnTenshouExeGungiGudanNo�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeGungiGudanNo =0x454A1E; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeGungiGudanNo  =0x4161EA; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeGungiGudanNo() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// �I�������R�c���̌R�c�ԍ����R�s�[�����B
		mov iOnGungiSelectingGudanNoNo, eax

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	
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
		call pTenshouExeJumpCallFromToOnTenshouExeGungiGudanNo

		jmp pTenshouExeReturnLblFromOnTenshouExeGungiGudanNo
	}
}



char cmdOnTenshouExeJumpFromGungiGudanNo[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeGungiGudanNo() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeGungiGudanNo;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeGungiGudanNo + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGungiGudanNo  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromGungiGudanNo+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeGungiGudanNo), cmdOnTenshouExeJumpFromGungiGudanNo, 5, NULL); //5�o�C�g�̂ݏ�������
}




//--------------------------------------------------------------------------------------------------------




/*
 * �R�c�̎��́A�������A�h���XLOC4�ɂقƂ�ǃZ���t��������Ă��邪�A
 * �喼�̃Z���t�́A�������A�h���XLOC1�ɏ�����Ă��܂��悤���B���̂��߁A�喼�̐��������܂��n��Ȃ��Ƃ�����肪����B
 *
 */
// ���݂̐����o�����A��]�裂̐����o���ł��邱�Ƃ̃t���O
int	isOnCurGungiFukidashi = FALSE;

void OnTenshouExeGungiFukidashiExecute() {
	// MessageN6PPatch::isOnTenshouExeMessageCopyUsed = false;

	// �͈̓`�F�b�N
	int iBushouID = -1;
	// �R�c���̕���ID(�z��p)�𓾂�B
	if ( 0 < iOnGungiSelectingGudanNoNo && iOnGungiSelectingGudanNoNo <= GAMEDATASTRUCT_GUNDAN_NUM ) {
		iBushouID = nb6gundan[iOnGungiSelectingGudanNoNo-1].leader-1;
	}

	// �����������͈̔̓`�F�b�N
	if ( ! (0 <= iBushouID && iBushouID <= GAMEDATASTRUCT_BUSHOU_NUM) ) {
		return;
	}

	// �^�[�Q�b�g�ƂȂ郁�b�Z�[�W�̃|�C���^�́A�喼(No1)���A�R�c��(No2�ȍ~)���ňقȂ�B
	char *pGungiTargetMessage = NULL;
	// �I�������Y���̕������喼�̏ꍇ ( State:0 == �喼 )
	if ( nb6bushouname[iBushouID].State == 0 ) {
		// �Ώۂ̃������A�h���X�́A�A�h���X1
		pGungiTargetMessage = (char *)CPM.getMemoryAddress(LOC1);
	} else {
		// �Ώۂ̃������A�h���X�́A�A�h���X4
		pGungiTargetMessage = (char *)CPM.getMemoryAddress(LOC4);
	}

	/*
		TSMod���ŁA�R�c�֘A�ŕ������u�����������ꍇ�́A���̏ꏊ��
		pGungiTargetMessage �̕����𒲂ׂāApGungiTargetMessage���̂�����������΂悢�B
	*/


	// ������, ���݂̏��������ΏۂƂȂ��Ă��郁�b�Z�[�W, �֘A�l�����
	// �O�����b�Z�[�W�u��������ScenarioMod�ɂčs��
	if ( p_snOnRewriteFukidashiMessage ) {

		// �����Ŗ��O�����o���B
		char name[256] = "";

		strcat( name, nb6bushouname[iBushouID].familyname);
		strcat( name, nb6bushouname[iBushouID].fastname);

		// ScenarioMod�Œu��������ꍇ�ɑΉ�����B
		NB6FUKIDASHIRELATEPERSONS gungifukidashirelatepaeresons;
		gungifukidashirelatepaeresons.i1stPersonID = iBushouID + 1;

		int iDaimyoID = nb6bushouref[iBushouID].attach - 1;
		int iDaimyoBushouID = nb6daimyo[iDaimyoID].attach - 1;
		gungifukidashirelatepaeresons.i2ndPersonID = iDaimyoBushouID + 1;

		char szModMsgBuf[512] = "";

		((PFNSNONREWRITEFUKIDASHIMESSAGE) p_snOnRewriteFukidashiMessage)(szModMsgBuf, name, pGungiTargetMessage, &gungifukidashirelatepaeresons);
		if ( strlen(szModMsgBuf) > 1 ) {
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pGungiTargetMessage), szModMsgBuf, strlen(szModMsgBuf)+1, NULL);
		}
	}

	isOnCurGungiFukidashi = TRUE;
	// �]��̐����o���t���O�𗧂Ă�BMessageN6PPatch.cpp �� ReWritePatchMessageN6P �ŏ������Ă������Ԃɍ���Ȃ����߁A
	// ������ł̏�����}�����邽�߂ɂ��̃t���O�𗧂Ă�B
}


/*
00416223  |. FF75 EC        PUSH    DWORD PTR SS:[EBP-14]            ; |Arg2
00416226  |. FF75 F0        PUSH    DWORD PTR SS:[EBP-10]            ; |Arg1
00416229  |. E8 28BF0800    CALL    TENSHOU.004A2156                 ; \TENSHOU.004A2156
				�������� JMP TSMod.OnTSExeGungiFukidashi �Ə��������Ă����ۂ̏���
0041622E  |. 6A 08          PUSH    8
*/
int pTenshouExeJumpFromToOnTenshouExeGungiFukidashi	 =0x416229; // �֐��͂��̃A�h���X����AOnTenshouExeGungiFukidashi�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeGungiFukidashi =0x4A2156; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeGungiFukidashi  =0x41622E; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeGungiFukidashi() {
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
	OnTenshouExeGungiFukidashiExecute();

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
		call pTenshouExeJumpCallFromToOnTenshouExeGungiFukidashi

		jmp pTenshouExeReturnLblFromOnTenshouExeGungiFukidashi
	}
}



char cmdOnTenshouExeJumpFromGungiFukidashi[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeGungiFukidashi() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeGungiFukidashi;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeGungiFukidashi + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGungiFukidashi  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromGungiFukidashi+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeGungiFukidashi), cmdOnTenshouExeJumpFromGungiFukidashi, 5, NULL); //5�o�C�g�̂ݏ�������
}




