#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"
#include "VariableNameRegistAndJudge.h"




// �f�t�H���g�őg�ݍ��܂�Ă���u�C���E���ƁE���l�E�����v�̃f�[�^��T���A�����ŃW���uID��ς���B�f�[�^���烍�[�h���āA�����ւƐݒ肷��B
extern byte nb6_all_bushou_job_ex[GAMEDATASTRUCT_RETSUDEN_NUM];

// �ΏۃW���u�̑Ώۂ̕���ID(�z���ł͂Ȃ��A�����̒l�ł���(�z����+1������))
int iViewingJobNoInBushouID = 0;
int iViewingJobNoInJobID = 0;


void setJobEXOfCurABushou(int iBushouID) {

	// ���S���Ă���Α���ɂ��Ȃ�
	if (nb6bushouname[iBushouID].State == 7) {
		return;
	}

	// ���łɃW���u���ݒ肳��Ă���΁A�������Ȃ�����
	if (nb6bushouref[iBushouID].job) {
		return;
	}

	if ( !IsOriginalRegistedBushou(iBushouID) ) {
		return;
	}

	// ����ID�����`ID�������o��
	int iRetsudenID = nb6bushouname[iBushouID].number;

	// ��`ID���I�[�o�[���Ă���(=����EX�Ƃ�1507�V�i���I)�̂̓X���[
	if (iRetsudenID >= GAMEDATASTRUCT_RETSUDEN_NUM ) {
		return;
	}

	// ��`ID����(�����I�ɐݒ肷��)�g��JOB�̒l���o��
	int iJobID = nb6_all_bushou_job_ex[iRetsudenID];

	// �܂��ݒ肳��ĂȂ��L���Ȋg��Job������Ȃ�΁A���������GAMEDATASTRUCT�̗�����ݒ肷��B
	if (iJobID) {
		nb6bushouref[iBushouID].job = iJobID;
	}
}

void setJobEXOfCurAllBushou() {

	for ( int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {
		setJobEXOfCurABushou(iBushouID);
	}
}


void OnTenshouExeViewingJobNoInBushouDetailExecute() {

	// ���ʂS�o�C�g������ID�Ȃ̂ŁA�}�X�N
	iViewingJobNoInBushouID = iViewingJobNoInBushouID & 0xFFFF;

	// �܂� ���X�L���ȐE��(�o��)���A���ł�TENSHOU.EXE(���������)�ݒ肳��Ă�����A����͏㏑�����Ȃ��̂ŉ������Ȃ�
	if ( iViewingJobNoInJobID ) { return; }

	// �Ώۂ̕����́A������512����
	if ( 0 <= iViewingJobNoInBushouID && iViewingJobNoInBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ����ID�����`ID�������o��
		int iRetsudenID = (nb6bushouname[iViewingJobNoInBushouID-1].number);

		// ��`ID���I�[�o�[���Ă��Ȃ���΁c
		if (iRetsudenID < GAMEDATASTRUCT_RETSUDEN_NUM ) {

			// ��`ID����(�����I�ɐݒ肷��)�g��JOB�̒l���o��
			iViewingJobNoInJobID = nb6_all_bushou_job_ex[iRetsudenID] / 16;
		}

	}
}

/*
00493D5D  |> 56             PUSH    ESI
00493D5E  |. E8 7F86F7FF    CALL    TENSHOU.0040C3E2
				�������� JMP TSMod.OnTSExeViewingJobNoInBushouDetail �Ə��������Ă����ۂ̏���
00493D63  |. 83C4 04        ADD     ESP, 4
00493D66  |. 8BF8           MOV     EDI, EAX
00493D68  |. 85FF           TEST    EDI, EDI						�W���u��0��������
00493D6A     74 2B          JE      SHORT TENSHOU.00493D97			���O��\�����鏈�����΂�
00493D6C  |. 68 A0000000    PUSH    0A0
*/
int pTenshouExeJumpFromToOnTenshouExeViewingJobNoInBushouDetail		=0x493D5E; // �֐��͂��̃A�h���X����AOnTenshouExeViewingJobNoInBushouDetail�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeViewingJobNoInBushouDetail =0x40C3E2; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeViewingJobNoInBushouDetail	=0x493D6C; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeViewingJobNoInBushouDetail() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iViewingJobNoInBushouID, esi	// ����ID���R�s�[�������Aesi�̉���4�o�C�g������ID�Ȃ̂ŁA�}�X�N����K�v����B

		call pTenshouExeJumpCallFromToOnTenshouExeViewingJobNoInBushouDetail

		mov iViewingJobNoInJobID, eax    // �W���uID��ۑ�

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// ���̊֐����ň��S�Ƀf�[�^�����H����B
	OnTenshouExeViewingJobNoInBushouDetailExecute();

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
		
		// �T�蓖�Ă��W���uID��eax�ɃR�s�[����B
		mov eax, iViewingJobNoInJobID


		// ���Xtenshou.exe�ɂ�����������������
        add     esp, 4
        mov     edi, eax
        test    edi, edi			// �W���u��0��������
        // JE      SHORT TENSHOU.00493D97 // ���O��\�����Ȃ��悤�ɔ�΂��A�̏������΂��Ȃ��悤�ɂ���B

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingJobNoInBushouDetail
	}
}

/*
00493D5D  |> 56             PUSH    ESI
00493D5E  |. E8 7F86F7FF    CALL    TENSHOU.0040C3E2
				�������� JMP TSMod.OnTSExeViewingJobNoInBushouDetail �Ə��������Ă����ۂ̏���
00493D63  |. 83C4 04        ADD     ESP, 4
00493D66  |. 8BF8           MOV     EDI, EAX
00493D68  |. 85FF           TEST    EDI, EDI
*/
char cmdOnTenshouExeJumpFromJobNoInBushouDetail[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�



// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeViewingJobNoInBushouDetail() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeViewingJobNoInBushouDetail;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingJobNoInBushouDetail + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingJobNoInBushouDetail  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromJobNoInBushouDetail+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingJobNoInBushouDetail), cmdOnTenshouExeJumpFromJobNoInBushouDetail, 5, NULL); //5�o�C�g�̂ݏ�������
}





// �����\���f�[�^�̊g�������̂����A�u�E��(�o��)�v�̃f�[�^�B(�C��,����,���l,����,�؎x�O)
byte nb6_all_bushou_job_ex[GAMEDATASTRUCT_RETSUDEN_NUM] = {
0x00,
0x00,
0x00,
0x00,
0x00,
0x80,
0x80,
0x70,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x00,
0x00,
0x60,
0x90,
0x00,
0x70,
0x00,
0x00,
0x00,
0x90,
0x00,
0x00,
0x60,
0x00,
0x90,
0x00,
0x00,
0x00,
0x60,
0x60,
0x80,
0x80,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x90,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x00,
0x60,
0x00,
0x00,
0x60,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x50,
0x80,
0x00,
0x00,
0x70,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x00,
0x00,
0x90,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x90,
0x90,
0x00,
0x00,
0x00,
0x90,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x00,
0x00,
0x00,
0x80,
0x50,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x50,
0x60,
0x00,
0x00,
0x00,
0x60,
0x60,
0x00,
0x90,
0x90,
0x00,
0x60,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x00,
0x00,
0x60,
0x00,
0x50,
0x00,
0x00,
0x00,
0x00,
0x50,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x00,
0x00,
0x00,
0x00,
0x60,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x00,
0x60,
0x00,
0x60,
0x00,
0x60,
0x00,
0x60,
0x00,
0x00,
0x60,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x00,
0x60,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x90,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x60,
0x60,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x00,
0x60,
0x60,
0x60,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x70,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x60,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x50,
0x50,
0x60,
0x00,
0x00,
0x50,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x60,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x80,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x00,
0x60,
0x60,
0x60,
0x50,
0x50,
0x50,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x60,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x60,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x60,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x70,
0x90,
0x00,
0x00,
0x00,
0x90,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x00,
0x00,
0x00,
0x00,
0x60,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x90,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x50,
0x00,
0x00,
0x00,
0x90,
0x00,
0x00,
0x50,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x00,
0x00,
0x50,
0x50,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x00,
0x60,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x90,
0x90,
0x90,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x00,
0x00,
0x00,
0x50,
0x50,
0x00,
0x90,
0x90,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x50,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x70,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x50,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x90,
0x00,
0x90,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x00,
0x00,
0x00,
0x00,
0x80,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x80,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x90,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x60,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x90,
0x00,
0x00,
0x00,
0x00,
0x60,
0x00,
0x00,
0x00,
0x50,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x90,
0x00,
0x70,
0x90,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
};