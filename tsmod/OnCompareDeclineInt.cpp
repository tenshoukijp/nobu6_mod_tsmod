#include "WinTarget.h"
#include <fstream>

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "TSModCommand.h"



/*
 * ��˂��邢�͒q�d���V���Č������锻�肪����鎞
 * ���G�߂ŁA�������Ƃɒq�d�\�̘͂V���̔N���r���Ȃ���鎞
 */
int iCompareDeclineIntBushouID = -1;

const int iCompareDeclineIntDefault = 0x3C;	 // 0x3C==60�ˁ@�q�d��60�Έȏ�ŘV���n�߂�

int iCompareDeclineIntValue = -1;
int iCurDeclineIntOld = -1;			 // ���݂̑Ώە����̔N��

// �Ώە����̒q�d��������N��
int getBeginDeclineIntYears(int iBushouID) {

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		// �����X���b�g�^���^���X�ł���΁c
		if ( isLancelotTartare( iBushouID ) ) {
			return iCompareDeclineIntDefault + 30;	// ���̕������30�˒x���V���n�߂�

		// ��ɂł���΁c
		} else if ( isShuiHidechiyo(iBushouID) ) {
			return iCompareDeclineIntDefault + 400;	// ���̕������400�˒x���V���n�߂�

		// �E�Ƃ��R�m�ł���΁c
		} else if ( nb6bushouref[iBushouID].job == 0xA0 ) {
			return iCompareDeclineIntDefault + 20;	// ���̕������20�˒x���V���n�߂�

		// �E�Ƃ����p�t�ł���΁c
		} else if ( nb6bushouref[iBushouID].job == 0xB0 ) {
			return iCompareDeclineIntDefault + 50;	// ���̕������50�˒x���V���n�߂�

		// �E�Ƃ������ł���΁c
		} else if ( nb6bushouref[iBushouID].job == 0xC0 ) {
			return iCompareDeclineIntDefault + 100;	// ���̕������100�˒x���V���n�߂�
		} else {
			return iCompareDeclineIntDefault;
		}
	}

	return iCompareDeclineIntDefault;
}

void OnTenshouExeCompareDeclineIntExecute() {
	// �K�v������� iCompareDeclineIntValue �� TSModCommand���ɂ��ύX���邱�ƂŁA��V���n�߂�N��ύX���邱�Ƃ��\�B
	// �����P�ʂŖ��x�����̓��[�v���Ă���̂ŁA�����P�ʂŕω������邱�Ƃ��\�B

	// ����f�t�H���g�l�ɂ��邱��
	iCompareDeclineIntValue = iCompareDeclineIntDefault;  // �V���n�߂�f�t�H���g�l�͖��x���

	// ����ID�͈̓`�F�b�N
	if ( 0 <= iCompareDeclineIntBushouID-1 && iCompareDeclineIntBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM ) {
		iCompareDeclineIntValue = getBeginDeclineIntYears(iCompareDeclineIntBushouID-1);
	}

	// �V��ɂ��\�͌����Ȃ��ł���΁c
	if ( TSModCommand::AllBushou::cmdNoDeclineTalentParam ) {
		iCompareDeclineIntValue = 10000; // 10000�ˈȏ�ł���Δ\�͌���
	}
}


/*
00441DD0  |> 83FF 3C        CMP     EDI, 3C							
				�������� JMP TSMod.OnTSExeCompareDeclineInt �Ə��������Ă����ۂ̏���
00441DD3  |. 77 28          JA      SHORT TENSHOU.00441DFD							
00441DD5  |. 8D47 C9        LEA     EAX, DWORD PTR DS:[EDI-37]							
*/
int pTenshouExeJumpFromToOnTenshouExeCompareDeclineInt	     =0x441DD0; // �֐��͂��̃A�h���X����AOnTenshouExeCompareDeclineInt�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeCompareDeclineInt   =0x441DFD; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeCompareDeclineInt    =0x441DD5; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w

// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeCompareDeclineInt() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���݂̑Ώە����̔N����R�s�[
		mov iCurDeclineIntOld, edi

		// �Ώۂ̕���ID���R�s�[
		mov iCompareDeclineIntBushouID, esi

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
	OnTenshouExeCompareDeclineIntExecute();

	// ���݂̔N��A�q�d���V���n�߂�ƒ�`���ꂽ�N���菬�����ꍇ
	// 00441DD3  |. 77 28          JA      SHORT TENSHOU.00441DFD �Ɠ������Ƃ�����B
	if ( iCurDeclineIntOld < iCompareDeclineIntValue ) {
		__asm {

			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			jmp pTenshouExeJumpCallFromToOnTenshouExeCompareDeclineInt
		}

	// 00441DD5  |. 8D47 C9        LEA     EAX, DWORD PTR DS:[EDI-37]�@�̍s�ւƃW�����v����B
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

			jmp pTenshouExeReturnLblFromOnTenshouExeCompareDeclineInt
		}
	}
}

/*
00441DD0  |> 83FF 3C        CMP     EDI, 3C							
				�������� JMP TSMod.OnTSExeCompareDeclineInt �Ə��������Ă����ۂ̏���
00441DD3  |. 77 28          JA      SHORT TENSHOU.00441DFD							
00441DD5  |. 8D47 C9        LEA     EAX, DWORD PTR DS:[EDI-37]							
*/
char cmdOnTenshouExeJumpFromCompareDeclineInt[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeCompareDeclineInt() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeCompareDeclineInt;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeCompareDeclineInt + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeCompareDeclineInt  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromCompareDeclineInt+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeCompareDeclineInt), cmdOnTenshouExeJumpFromCompareDeclineInt, 5, NULL); //5�o�C�g�̂ݏ�������
}




