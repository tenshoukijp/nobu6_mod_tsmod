
#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"
#include "PluginMod.h"
#include "SetBushouAvgParam.h"
#include "EXBushou_All.h"


extern bool isEnteringFromScenarioDaimyoExecute;
extern bool isEnteringFromLoadDialogExecute;
extern void OnTenshouExeEnteringFromScenarioDaimyoExecute();
extern void EXBushou_All_CheckLifeTimes();

extern int iSelectingScenarioLocation; // �V�i���I�̈ʒu�̔ԍ��B�ォ��O�D��ԉ����T

extern int iEnterOfClickMainScreenMainCommand;


extern int iIsGameStartFlagEAX = 0; // ���[�h��Q�[���V�K�J�n�Ȃǎ���1�A�����ł͂Ȃ��A�G�߂��ڂ�ς���������̎���0

void OnTenshouExeMainGameStartExecute() {

	CommonGlobalHandle::isMainGameStart = true;

	// �V�i���I�I���̃��[�h�ł͂Ȃ��̂͊m���Ȃ̂ŁA���̃t���O���~�낷
	CommonGlobalHandle::isModeScenario = false;

	// ���C����ʂ̃R�}���h�ɐN�����Ă邩�ǂ����B
	iEnterOfClickMainScreenMainCommand = 0;

	// �V�i���I�I�����炫���A�������̓��[�h��ʂ��炫��
	if ( isEnteringFromLoadDialogExecute || isEnteringFromScenarioDaimyoExecute ) {

		// ���̃^�C�~���O�ŁA�V�i���I�I���������Ă����ꍇ�̂݃V�i���I������[�r�[�𗬂��B
		if ( isEnteringFromScenarioDaimyoExecute ) {
			// �V�i���I�I���������Ă����t���O�������B
			isEnteringFromScenarioDaimyoExecute = false;
		} else if (isEnteringFromLoadDialogExecute)  {
			isEnteringFromLoadDialogExecute = false;

			// �V�i���I�I���ȊO�������Ă����ꍇ��(���ʂ̓��[�h�o�R�̂�)�Anb6savedata_ex.bushou_ex�ŕK�v�ȃp�����^������B
			SetBushouAvgParam(2); // 2�Ƃ����̂̓��[�h���̂��ƁB
			SetExBushouAvgParam(-1); // -1�͑S���ɑ΂��čs���Ƃ������ƁB���ꕐ���p(�V�i���I�I�����o�R�͎��s�ς݂Ȃ̂ł͂����Ă���)
		}

		// �V�i���I�I������ł����[�h����ł��A�ȉ��͋��ʂ̏���
		copyAllBushouSoldiersOriginToCopy();
	
		EXSuperService_OnMainGameStart();
	}

	// ���[�h���Q�[���J�n���ȂǁA�u���߂Ă̂��̋G�߁v�̎���������B
	if (iIsGameStartFlagEAX) {
		// ;
	}

	// ���G�߂���B�������A���[�h���Ȃ������ۂ͗��Ȃ��B
	else {
		// �~����t�ɕς�鎞
		if (nb6year.season == 3) {

			// �I�v�V�����Ɋւ�炸�AEX�������΂����̂��x������
			EXBushou_All_CheckLifeTimes();

			// �����������΂����̂����{�x�������Ȃ�c
			EXSuperService_IfLifesIsTheTimes();

		}
	}
}


/*
0044193D  /$ 83EC 10        SUB     ESP, 10				���������烁�C���Q�[���X�^�[�g�ƍl���ėǂ�
00441940  |. 56             PUSH    ESI
00441941  |. 57             PUSH    EDI
00441942  |. 68 00020000    PUSH    200
00441947  |. E8 55560200    CALL    TENSHOU.00466FA1
				�������� JMP TSMod.OnTSExeMainGameStart �Ə��������Ă����ۂ̏���
0044194C  |. 83C4 04        ADD     ESP, 4
*/
int pTenshouExeJumpFromToOnTenshouExeMainGameStart		=0x441947; // �֐��͂��̃A�h���X����AOnTenshouExeMainGameStart�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeMainGameStart  =0x466FA1; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeMainGameStart	=0x44194C; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeMainGameStart() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeMainGameStart

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi

		mov iIsGameStartFlagEAX, eax    // ���[�h��Q�[���V�K�J�n�Ȃǎ���1�A�����ł͂Ȃ��A�G�߂��ڂ�ς���������̎���0
	}
	
	OnTenshouExeMainGameStartExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeMainGameStart
	}
}


char cmdOnTenshouExeJumpFromMainGameStart[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeMainGameStart() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeMainGameStart;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeMainGameStart + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeMainGameStart  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromMainGameStart+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeMainGameStart), cmdOnTenshouExeJumpFromMainGameStart, 5, NULL); //5�o�C�g�̂ݏ�������
}




