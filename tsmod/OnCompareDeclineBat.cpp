#include "WinTarget.h"
#include <fstream>

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "TSModCommand.h"



/*
 * ��˂��邢�͐퓬���V���Č������锻�肪����鎞
 * ���G�߂ŁA�������Ƃɐ퓬�\�̘͂V���̔N���r���Ȃ���鎞
 */
int iCompareDeclineBatBushouID = -1;

const int iCompareDeclineBatDefault = 0x32;	 // 0x32==50�ˁ@�퓬��50�Έȏ�ŘV���n�߂�

int iCompareDeclineBatValue = -1;
int iCurDeclineBatOld = -1;			 // ���݂̑Ώە����̔N��


// �Ώە����̐퓬��������N��
int getBeginDeclineBatYears(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		// �����X���b�g�^���^���X�ł���΁c
		if ( isLancelotTartare( iBushouID ) ) {
			return iCompareDeclineBatDefault + 30;	// ���̕������30�˒x���V���n�߂�

		// ��ɂł���΁c
		} else if ( isShuiHidechiyo(iBushouID) ) {
			return iCompareDeclineBatDefault + 400;	// ���̕������400�˒x���V���n�߂�

		// �E�Ƃ��R�m�ł���΁c
		} else if ( nb6bushouref[iBushouID].job == 0xA0 ) {
			return iCompareDeclineBatDefault + 20;	// ���̕������20�˒x���V���n�߂�

		// �E�Ƃ����p�t�ł���΁c
		} else if ( nb6bushouref[iBushouID].job == 0xB0 ) {
			return iCompareDeclineBatDefault + 50;	// ���̕������50�˒x���V���n�߂�

		// �E�Ƃ������ł���΁c
		} else if ( nb6bushouref[iBushouID].job == 0xC0 ) {
			return iCompareDeclineBatDefault + 100;	// ���̕������100�˒x���V���n�߂�
		} else {
			return iCompareDeclineBatDefault;
		}
	}

	return iCompareDeclineBatDefault;
}
void OnTenshouExeCompareDeclineBatExecute() {
	// �K�v������� iCompareDeclineBatValue �� TSModCommand���ɂ��ύX���邱�ƂŁA��V���n�߂�N��ύX���邱�Ƃ��\�B
	// �����P�ʂŖ��x�����̓��[�v���Ă���̂ŁA�����P�ʂŕω������邱�Ƃ��\�B

	// ����f�t�H���g�l�ɂ��邱��
	iCompareDeclineBatValue = iCompareDeclineBatDefault;  // �V���n�߂�f�t�H���g�l�͖��x���

	// ����ID�͈̓`�F�b�N
	if ( 0 <= iCompareDeclineBatBushouID-1 && iCompareDeclineBatBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM ) {
		iCompareDeclineBatValue = getBeginDeclineBatYears(iCompareDeclineBatBushouID-1);
	}

	// �V��ɂ��\�͌����Ȃ��ł���΁c
	if ( TSModCommand::AllBushou::cmdNoDeclineTalentParam ) {
		iCompareDeclineBatValue = 10000; // 10000�ˈȏ�ł���Δ\�͌���
	}

}


/*
00441DA3  |. 83FF 32        CMP     EDI, 32
				�������� JMP TSMod.OnTSExeCompareDeclineBat �Ə��������Ă����ۂ̏���
00441DA6  |. 77 28          JA      SHORT TENSHOU.00441DD0
00441DA8  |. 8D47 D3        LEA     EAX, DWORD PTR DS:[EDI-2D]
*/
int pTenshouExeJumpFromToOnTenshouExeCompareDeclineBat	     =0x441DA3; // �֐��͂��̃A�h���X����AOnTenshouExeCompareDeclineBat�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeCompareDeclineBat   =0x441DD0; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeCompareDeclineBat    =0x441DA8; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w

// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeCompareDeclineBat() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���݂̑Ώە����̔N����R�s�[
		mov iCurDeclineBatOld, edi

		// �Ώۂ̕���ID���R�s�[
		mov iCompareDeclineBatBushouID, esi

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
	OnTenshouExeCompareDeclineBatExecute();

	// ���݂̔N��A�퓬���V���n�߂�ƒ�`���ꂽ�N���菬�����ꍇ
	// 00441DA6  |. 77 28          JA      SHORT TENSHOU.00441DD0 �Ɠ������Ƃ�����B
	if ( iCurDeclineBatOld < iCompareDeclineBatValue ) {
		__asm {

			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			jmp pTenshouExeJumpCallFromToOnTenshouExeCompareDeclineBat
		}

	// 00441DA8  |. 8D47 D3        LEA     EAX, DWORD PTR DS:[EDI-2D]�@�̍s�ւƃW�����v����B
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

			jmp pTenshouExeReturnLblFromOnTenshouExeCompareDeclineBat
		}
	}
}

/*
00441DA3  |. 83FF 32        CMP     EDI, 32
				�������� JMP TSMod.OnTSExeCompareDeclineBat �Ə��������Ă����ۂ̏���
00441DA6  |. 77 28          JA      SHORT TENSHOU.00441DD0
00441DA8  |. 8D47 D3        LEA     EAX, DWORD PTR DS:[EDI-2D]
*/
char cmdOnTenshouExeJumpFromCompareDeclineBat[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeCompareDeclineBat() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeCompareDeclineBat;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeCompareDeclineBat + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeCompareDeclineBat  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromCompareDeclineBat+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeCompareDeclineBat), cmdOnTenshouExeJumpFromCompareDeclineBat, 5, NULL); //5�o�C�g�̂ݏ�������
}




