
#include "WinTarget.h"

#include <sys/types.h>
#include <sys/stat.h>

#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "FuncPlayResourceWav.h"

/*
0049BA16  |. FF75 F0        PUSH    DWORD PTR SS:[EBP-10]
0049BA19  |. 6A 0A          PUSH    0A           // ����10�Ԃ̉��Ƃ����̂��A�S�C�̔��ˉ�
0049BA1B  |. 33F6           XOR     ESI, ESI
0049BA1D  |. E8 8B78FEFF    CALL    TENSHOU.004832AD	//�@�����Ŏ��ۂɍĐ����邱�ƂƂȂ�B
0049BA22  |. 83C4 08        ADD     ESP, 8
*/


/*
 * �S�C�����������̉�
 */

extern int iFireTeppouAttackBushouID; // �S�C������������
extern int iFireTeppouTargetBushouID; // �S�C�������ꂽ����

int isMustPlaySubstituteSE = 1; // ��ւ��̉����Đ�����K�v�����邩�ǂ����B

int isShouldBePlayTeppouSE = 1;
void OnTenshouExeFireTeppouSEExecute() {
	isShouldBePlayTeppouSE = 1; // ���ʂ͓S�C���Ȃ炷�ׂ�
	isMustPlaySubstituteSE = 1; // �{����ւ��̉��̍Đ��̕K�v������

	int iBushouID = iFireTeppouAttackBushouID-1;

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���y���������́A�R�n���Ȃ̂ɁA�S�C���������Ƃ������Ƃ́c ����͋|���������͋|�R�n���ł���
		if ( (nb6bushouref[iBushouID].form == 0 || nb6bushouref[iBushouID].form == 1) && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 ) {

			isShouldBePlayTeppouSE = 0;

			struct stat  st;
			// �t�����ʉ��L�b�g�𓱓����Ă���΁c
			if (stat("n6pwav1.dll", &st) == 0) {
				isMustPlaySubstituteSE = 0; // ��ւ��͕K�v�Ȃ��B
				FuncPlaySoundWav( 716 );
			} else {
			}
		}
	}
}



/*
0049BA16  |. FF75 F0        PUSH    DWORD PTR SS:[EBP-10]
				�������� JMP TSMod.OnTSExeFireTeppouSE �Ə��������Ă����ۂ̏���
0049BA19  |. 6A 0A          PUSH    0A           // ����10�Ԃ̉��Ƃ����̂��A�S�C�̔��ˉ�
0049BA1B  |. 33F6           XOR     ESI, ESI
0049BA1D  |. E8 8B78FEFF    CALL    TENSHOU.004832AD	//�@�����Ŏ��ۂɍĐ����邱�ƂƂȂ�B
0049BA22  |. 83C4 08        ADD     ESP, 8
*/
int pTenshouExeJumpFromToOnTenshouExeFireTeppouSE		=0x49BA16; // �֐��͂��̃A�h���X����AOnTenshouExeFireTeppouSE�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeFireTeppouSE   =0x4832AD; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeFireTeppouSE	=0x49BA22; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeFireTeppouSE() {
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
	OnTenshouExeFireTeppouSEExecute();

	// ���ʂɓS�C���������͓S�C�R�n���ł���΁c �������̂܂܂Ȃ炷
	if ( isShouldBePlayTeppouSE ) {
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

			PUSH    DWORD PTR SS:[EBP-0x10]

			PUSH    0xA           // ����10�Ԃ̉��Ƃ����̂��A�S�C�̔��ˉ�
			XOR     ESI, ESI

			call pTenshouExeJumpCallFromToOnTenshouExeFireTeppouSE // �������ŉ����Đ������

			jmp pTenshouExeReturnLblFromOnTenshouExeFireTeppouSE
		}

	// �|�����A��ւ��̉����K�v�Ȃ�΁c14�Ԃ�炷
	} else if ( isMustPlaySubstituteSE ) {
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

			PUSH    DWORD PTR SS:[EBP-0x10]

			PUSH    0xE           // ����14�Ԃ̉��Ƃ����̂��A�Ǔo��(������)�̔��ˉ�
			XOR     ESI, ESI

			call pTenshouExeJumpCallFromToOnTenshouExeFireTeppouSE // �������ŉ����Đ������

			jmp pTenshouExeReturnLblFromOnTenshouExeFireTeppouSE
		}

	// �t�����ʉ������Ƃ������Ă���A�|�Ȃ�΁A�ʘg�Ŗ炵�Ă���̂ŁA�֐����Đ����Ȃ��B
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

			PUSH    DWORD PTR SS:[EBP-0x10]

			PUSH    0xA           // ����10�Ԃ̉��Ƃ����̂��A�S�C�̔��ˉ�
			XOR     ESI, ESI

			// call pTenshouExeJumpCallFromToOnTenshouExeFireTeppouSE // �������ŉ����Đ������̂��R�����g�A�E�g���邱�ƂŁA���̌��ʉ��͂Ȃ�Ȃ�

			jmp pTenshouExeReturnLblFromOnTenshouExeFireTeppouSE
		}
	}
}




char cmdOnTenshouExeJumpFromFireTeppouSE[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeFireTeppouSE() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeFireTeppouSE;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeFireTeppouSE + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeFireTeppouSE  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromFireTeppouSE+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeFireTeppouSE), cmdOnTenshouExeJumpFromFireTeppouSE, 5, NULL); //5�o�C�g�̂ݏ�������
}








///--------------------------------------------------------------------------------------------




/*
 * �S�C�����������̉�
 */

int iCurrentKonranAttackSENo = -1; // ����̍U���ɂ��r�d�i���o�[�̕ۑ��ꏊ�B0xA���ƓS�C��
int iKonranAttackSENoEAX = -1;
int isShouldBePlayOriginalSE = 0;

void OnTenshouExeFireKonranTeppouSEExecute() {
	isShouldBePlayOriginalSE = 1; // ���ʂ͓S�C���Ȃ炷�ׂ�
	isMustPlaySubstituteSE = 1; // �{����ւ��̉��̍Đ��̕K�v������

	// �����S�C�Ȃ̂ł���΁c
	if ( iCurrentKonranAttackSENo == 0xA ) {

		int iBushouID = iFireTeppouAttackBushouID-1;

		if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			// ���y���������́A�R�n���Ȃ̂ɁA�S�C���������Ƃ������Ƃ́c ����͋|���������͋|�R�n���ł���
			if ( (nb6bushouref[iBushouID].form == 0 || nb6bushouref[iBushouID].form == 1) && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 ) {
				isShouldBePlayOriginalSE = 0;

				struct stat  st;
				// �t�����ʉ��L�b�g�𓱓����Ă���΁c
				if (stat("n6pwav1.dll", &st) == 0) {
					isMustPlaySubstituteSE = 0; // ��ւ��͕K�v�Ȃ��B
					FuncPlaySoundWav( 716 );
				} else {
				}
			}
		}
	} else {
	}
}



/*
004832B6  |. 83C4 04        ADD     ESP, 4
004832B9  |. 50             PUSH    EAX                              ; /Arg4
004832BA  |. 6A 00          PUSH    0                                ; |Arg3 = 00000000
004832BC  |. 6A 03          PUSH    3                                ; |Arg2 = 00000003
004832BE  |. FF7424 10      PUSH    DWORD PTR SS:[ESP+10]            ; |Arg1 = ������10�Ԃ����������A�S�C�̉��̂悤�����A10�Ԃ�����Ƃ͌��肠�Ȃ���������Ȃ��̂Ŕ���̕K�v������B
004832C2  |. E8 AE9F0000    CALL    TENSHOU.0048D275                 ; \TENSHOU.0048D275
				�������� JMP TSMod.OnTSExeFireKonranTeppouSE �Ə��������Ă����ۂ̏���
004832C7  |. 83C4 10        ADD     ESP, 10
*/
int pTenshouExeJumpFromToOnTenshouExeFireKonranTeppouSE		=0x4832C2; // �֐��͂��̃A�h���X����AOnTenshouExeFireKonranTeppouSE�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeFireKonranTeppouSE =0x48D275; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeFireKonranTeppouSE	=0x4832C7; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeFireKonranTeppouSE() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		mov iKonranAttackSENoEAX, eax		// eax���g���̂ň�[�ێ�
		pop eax								// eax�ɍŌ��push�����l�𕜌�
		mov iCurrentKonranAttackSENo, eax	// �����ۑ�
		mov eax, iKonranAttackSENoEAX		// eax�����̒l�ɖ߂�

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
	OnTenshouExeFireKonranTeppouSEExecute();

	// �S�C�Ȃ猳�̂܂�
	if ( isShouldBePlayOriginalSE ) {

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

			push iCurrentKonranAttackSENo
			call pTenshouExeJumpCallFromToOnTenshouExeFireKonranTeppouSE // �������ŉ����Đ������

			jmp pTenshouExeReturnLblFromOnTenshouExeFireKonranTeppouSE
		}
	}

	// �|�����A��ւ��̉����K�v�Ȃ�΁c14�Ԃ�炷
	else if ( isMustPlaySubstituteSE ) {

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

			push 0xE
			call pTenshouExeJumpCallFromToOnTenshouExeFireKonranTeppouSE // �������ŉ����Đ������

			jmp pTenshouExeReturnLblFromOnTenshouExeFireKonranTeppouSE
		}

	// �t�����ʉ������Ƃ������Ă���A�|�Ȃ�΁A�ʘg�Ŗ炵�Ă���̂ŁA�֐����Đ����Ȃ��B
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

			push iCurrentKonranAttackSENo
			// call pTenshouExeJumpCallFromToOnTenshouExeFireKonranTeppouSE // �������ŉ����Đ������̂��R�����g�A�E�g���邱�ƂŁA���̌��ʉ��͂Ȃ�Ȃ�

			jmp pTenshouExeReturnLblFromOnTenshouExeFireKonranTeppouSE
		}
	}
}




char cmdOnTenshouExeJumpFromFireKonranTeppouSE[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeFireKonranTeppouSE() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeFireKonranTeppouSE;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeFireKonranTeppouSE + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeFireKonranTeppouSE  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromFireKonranTeppouSE+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeFireKonranTeppouSE), cmdOnTenshouExeJumpFromFireKonranTeppouSE, 5, NULL); //5�o�C�g�̂ݏ�������
}




