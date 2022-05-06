#include "WinTarget.h"

#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

/*
 * ��C���������Ă邩�ǂ�����
 * ���肷�邪�B�|�����Ƃ̌��ˍ����ŁA�J���~���Ă�Ƃǂ������̔�����p�b�`����B
 */
/*
0042B456   . E8 4AEEFDFF    CALL    TENSHOU.0040A2A5
0042B45B   . 83C4 04        ADD     ESP, 4
0042B45E   . 85C0           TEST    EAX, EAX
0042B460   . 75 04          JNZ     SHORT TENSHOU.0042B466
*/

// �Ώە����͉J�S�C���\���ǂ���
extern bool IsCanAmeDeppou(int iBushouID );


//-----------------------------------���̂P---------------------------------------

int iCurBushouIDOnTaihouPatch = -1;
int isHasTaihouOnTaihouPatch = 0;

// �{���ɍ��A��C�����Ă�̂��ǂ���
BOOL IsCanTaihouPatch(int iBushouID ) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// �Ώە�����(��ɋ����)�ʒu�𓾂�B
		HEX_POSITION p = GetCastleHexUnitPos( iBushouID );
		// �����̈ʒu�̖����𓾂�
		int iHexRole = GetCastleHexRole( p );

		// ��C�������Ă���
		if ( nb6bushouref[iBushouID].biggun ) {
			// �J�S�C�������Ă���΁A�V�C�ɂ�����炸�łĂ�
			if ( IsCanAmeDeppou(iBushouID) ) {
				return true;

			// �J�S�C�͎����Ă��Ȃ����J�ȊO�Ȃ̂őłĂ�
			// �V�C���𓾂� 0:���� 1:�܂� 2:�J 3:��
			} else if ( getWeather() != 2 ) {
				return true;

			// �J�~���āA�J�S�C�������ĂȂ����A�{�ۂȂ̂őłĂ܂��B
			} else if ( iHexRole == CASTLE_HEX_ROLE::�E || iHexRole == CASTLE_HEX_ROLE::�{�� ) {
				return true;

			// ���͑łĂȂ�
			} else  {
				return false;
			}

		// ��C�������Ă��Ȃ�
		} else {
			return false;
		}
	} return false;
}

void OnTenshouExeJudgeHasTaihouExecute() {
	int iBushouID = iCurBushouIDOnTaihouPatch-1; // �����h�c��

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// �|���ɂȂ��Ă���ꍇ�́A�ςȂ��ƂɂȂ��Ă���̂ŁA��C�Ɋւ��Ē�������킹�鏈���ւƗ����B
		if ( nb6bushouref[iBushouID].form == 0 || nb6bushouref[iBushouID].form == 1 ) {
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 ) {
				isHasTaihouOnTaihouPatch = IsCanTaihouPatch(iBushouID); // ��C�łĂ�łĂȂ������㏑��
			}
		}
	}
}



/*
0042B456   . E8 4AEEFDFF    CALL    TENSHOU.0040A2A5
				�������� JMP TSMod.OnTSExeJudgeHasTaihou1 �Ə��������Ă����ۂ̏���
0042B45B   . 83C4 04        ADD     ESP, 4
0042B45E   . 85C0           TEST    EAX, EAX

EDI�������ԍ�
*/
int pTenshouExeJumpFromToOnTenshouExeJudgeHasTaihou1	 =0x42B456; // �֐��͂��̃A�h���X����AOnTenshouExeJudgeHasTaihou1�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeJudgeHasTaihou1 =0x40A2A5; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeJudgeHasTaihou1	 =0x42B45B; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeJudgeHasTaihou1() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iCurBushouIDOnTaihouPatch, edi // �����ԍ����R�s�[

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeJudgeHasTaihou1

		mov isHasTaihouOnTaihouPatch, eax // ��C��������

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
	OnTenshouExeJudgeHasTaihouExecute();

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

		mov eax, isHasTaihouOnTaihouPatch // EAX�ɑ�C�\���Ƃ��ăI�[�o�[���C�h

		jmp pTenshouExeReturnLblFromOnTenshouExeJudgeHasTaihou1
	}
}




char cmdOnTenshouExeJumpFromJudgeHasTaihou1[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeJudgeHasTaihou1() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeJudgeHasTaihou1;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeJudgeHasTaihou1 + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeJudgeHasTaihou1  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromJudgeHasTaihou1+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeJudgeHasTaihou1), cmdOnTenshouExeJumpFromJudgeHasTaihou1, 5, NULL); //5�o�C�g�̂ݏ�������
}



//-----------------------------------���̂Q---------------------------------------





/*
0046960D  |. E8 930CFAFF    CALL    TENSHOU.0040A2A5
				�������� JMP TSMod.OnTSExeJudgeHasTaihou2 �Ə��������Ă����ۂ̏���
00469612  |. 83C4 04        ADD     ESP, 4
00469615  |. 85C0           TEST    EAX, EAX

EDI�������ԍ�
*/
int pTenshouExeJumpFromToOnTenshouExeJudgeHasTaihou2	 =0x46960D; // �֐��͂��̃A�h���X����AOnTenshouExeJudgeHasTaihou2�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeJudgeHasTaihou2 =0x40A2A5; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeJudgeHasTaihou2	 =0x469612; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeJudgeHasTaihou2() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iCurBushouIDOnTaihouPatch, edi // �����ԍ����R�s�[

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeJudgeHasTaihou2

		mov isHasTaihouOnTaihouPatch, eax // ��C��������

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
	OnTenshouExeJudgeHasTaihouExecute();

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

		mov eax, isHasTaihouOnTaihouPatch // EAX�ɑ�C�\���Ƃ��ăI�[�o�[���C�h

		jmp pTenshouExeReturnLblFromOnTenshouExeJudgeHasTaihou2
	}
}




char cmdOnTenshouExeJumpFromJudgeHasTaihou2[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeJudgeHasTaihou2() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeJudgeHasTaihou2;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeJudgeHasTaihou2 + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeJudgeHasTaihou2  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromJudgeHasTaihou2+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeJudgeHasTaihou2), cmdOnTenshouExeJumpFromJudgeHasTaihou2, 5, NULL); //5�o�C�g�̂ݏ�������
}



//-----------------------------------���̂R---------------------------------------





/*
0042B202  |. E8 9EF0FDFF    CALL    TENSHOU.0040A2A5
				�������� JMP TSMod.OnTSExeJudgeHasTaihou3 �Ə��������Ă����ۂ̏���
0042B207  |. 83C4 04        ADD     ESP, 4
0042B20A  |. 85C0           TEST    EAX, EAX

���̏u�Ԃ�ESI�������ԍ�
*/
int pTenshouExeJumpFromToOnTenshouExeJudgeHasTaihou3	 =0x42B202; // �֐��͂��̃A�h���X����AOnTenshouExeJudgeHasTaihou3�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeJudgeHasTaihou3 =0x40A2A5; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeJudgeHasTaihou3	 =0x42B207; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeJudgeHasTaihou3() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iCurBushouIDOnTaihouPatch, esi // �����ԍ����R�s�[

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeJudgeHasTaihou3

		mov isHasTaihouOnTaihouPatch, eax // ��C��������

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
	OnTenshouExeJudgeHasTaihouExecute();

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

		mov eax, isHasTaihouOnTaihouPatch // EAX�ɑ�C�\���Ƃ��ăI�[�o�[���C�h

		jmp pTenshouExeReturnLblFromOnTenshouExeJudgeHasTaihou3
	}
}




char cmdOnTenshouExeJumpFromJudgeHasTaihou3[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeJudgeHasTaihou3() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeJudgeHasTaihou3;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeJudgeHasTaihou3 + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeJudgeHasTaihou3  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromJudgeHasTaihou3+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeJudgeHasTaihou3), cmdOnTenshouExeJumpFromJudgeHasTaihou3, 5, NULL); //5�o�C�g�̂ݏ�������
}




//-----------------------------------���̂S---------------------------------------






