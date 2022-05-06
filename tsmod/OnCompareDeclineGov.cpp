#include "WinTarget.h"
#include <fstream>

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "TSModCommand.h"


/*
 * ��˂��邢�͐������V���Č������锻�肪����鎞
 * ���G�߂ŁA�������Ƃɐ����\�̘͂V���̔N���r���Ȃ���鎞
 */
int iCompareDeclineGovBushouID = -1;

const int iCompareDeclineGovDefault = 0x46;	 // 0x46==70�ˁ@������70�Έȏ�ŘV���n�߂�

int iCompareDeclineGovValue = -1;
int iCurDeclineGovOld = -1;			 // ���݂̑Ώە����̔N��


// �Ώە����̐�����������N��
int getBeginDeclineGovYears(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		// �����X���b�g�^���^���X�ł���΁c
		if ( isLancelotTartare( iBushouID ) ) {
			return iCompareDeclineGovDefault + 30;	// ���̕������30�˒x���V���n�߂�

		// ��ɂł���΁c
		} else if ( isShuiHidechiyo(iBushouID) ) {
			return iCompareDeclineGovDefault + 400;	// ���̕������400�˒x���V���n�߂�

		// �E�Ƃ��R�m�ł���΁c
		} else if ( nb6bushouref[iBushouID].job == 0xA0 ) {
			return iCompareDeclineGovDefault + 20;	// ���̕������20�˒x���V���n�߂�

		// �E�Ƃ����p�t�ł���΁c
		} else if ( nb6bushouref[iBushouID].job == 0xB0 ) {
			return iCompareDeclineGovDefault + 50;	// ���̕������50�˒x���V���n�߂�

		// �E�Ƃ������ł���΁c
		} else if ( nb6bushouref[iBushouID].job == 0xC0 ) {
			return iCompareDeclineGovDefault + 100;	// ���̕������100�˒x���V���n�߂�
		} else {
			return iCompareDeclineGovDefault;
		}
	}

	return iCompareDeclineGovDefault;
}

void OnTenshouExeCompareDeclineGovExecute() {
	// �K�v������� iCompareDeclineGovValue �� TSModCommand���ɂ��ύX���邱�ƂŁA��V���n�߂�N��ύX���邱�Ƃ��\�B
	// �����P�ʂŖ��x�����̓��[�v���Ă���̂ŁA�����P�ʂŕω������邱�Ƃ��\�B

	// ����f�t�H���g�l�ɂ��邱��
	iCompareDeclineGovValue = iCompareDeclineGovDefault;  // �V���n�߂�f�t�H���g�l�͖��x���

	// ����ID�͈̓`�F�b�N
	if ( 0 <= iCompareDeclineGovBushouID-1 && iCompareDeclineGovBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM ) {
		iCompareDeclineGovValue = getBeginDeclineGovYears(iCompareDeclineGovBushouID-1);
	}

	// �V��ɂ��\�͌����Ȃ��ł���΁c
	if ( TSModCommand::AllBushou::cmdNoDeclineTalentParam ) {
		iCompareDeclineGovValue = 10000; // 10000�ˈȏ�ł���Δ\�͌���
	}

}


/*
00441DFD  |> 83FF 46        CMP     EDI, 46
				�������� JMP TSMod.OnTSExeCompareDeclineGov �Ə��������Ă����ۂ̏���
00441E00  |. 77 28          JA      SHORT TENSHOU.00441E2A
00441E02  |. 83EF 41        SUB     EDI, 41
*/
int pTenshouExeJumpFromToOnTenshouExeCompareDeclineGov	     =0x441DFD; // �֐��͂��̃A�h���X����AOnTenshouExeCompareDeclineGov�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeCompareDeclineGov   =0x441E2A; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeCompareDeclineGov    =0x441E02; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w

// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeCompareDeclineGov() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���݂̑Ώە����̔N����R�s�[
		mov iCurDeclineGovOld, edi

		// �Ώۂ̕���ID���R�s�[
		mov iCompareDeclineGovBushouID, esi

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
	OnTenshouExeCompareDeclineGovExecute();

	// ���݂̔N��A�������V���n�߂�ƒ�`���ꂽ�N���菬�����ꍇ
	// 00441E00  |. 77 28          JA      SHORT TENSHOU.00441E2A �Ɠ������Ƃ�����B
	if ( iCurDeclineGovOld < iCompareDeclineGovValue ) {
		__asm {

			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			jmp pTenshouExeJumpCallFromToOnTenshouExeCompareDeclineGov
		}

	// 00441E02  |. 83EF 41        SUB     EDI, 41�@�̍s�ւƃW�����v����B
	} else {

		__asm {

			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			jmp pTenshouExeReturnLblFromOnTenshouExeCompareDeclineGov
		}
	}
}

/*
00441DD0  |> 83FF 3C        CMP     EDI, 3C							
				�������� JMP TSMod.OnTSExeCompareDeclineGov �Ə��������Ă����ۂ̏���
00441DD3  |. 77 28          JA      SHORT TENSHOU.00441DFD							
00441DD5  |. 8D47 C9        LEA     EAX, DWORD PTR DS:[EDI-37]							
*/
char cmdOnTenshouExeJumpFromCompareDeclineGov[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeCompareDeclineGov() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeCompareDeclineGov;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeCompareDeclineGov + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeCompareDeclineGov  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromCompareDeclineGov+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeCompareDeclineGov), cmdOnTenshouExeJumpFromCompareDeclineGov, 5, NULL); //5�o�C�g�̂ݏ�������
}




