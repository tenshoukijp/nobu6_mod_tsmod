#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "PluginMod.h"
#include "ScenarioMod.h"

/*
 �g�b�v���j���[�̂ǂꂩ��I��������
 */

// �N�E�G�߂��ǂ����Ő錾����Ă���B
extern int nb6progressseason;

// �P�O�̋G�߂̏��
extern char pre_check_season;

// �I�����Ă���V�i���I�ԍ�
extern int iSelectingScenarioLocation;
extern int iSelectingScenarioID;



void OnTenshouExeViewingTopMenuExecute() {

	// �G�ߑ����J�E���g�����Z�b�g
	nb6progressseason = 1L;

	// �P�O�̋G�߂����蓾�Ȃ����l�ɂ��Ă���
	pre_check_season = 100;

	iSelectingScenarioLocation = -1;
	iSelectingScenarioID = -1;

	// �g���f�[�^���������Z�b�g
	memset( (void *)&nb6savedata_ex, 0, sizeof(nb6savedata_ex) );
	memset( (void *)&nb6savedata_ex_common, 0, sizeof(nb6savedata_ex_common) );

	// �����ɗ��邽�тɃG���f�B���O���[�h�I��
	CommonGlobalHandle::isEndingMode = false;

	CommonGlobalHandle::isEndingModeAfter = false;

	// �v���O�C��Mod�̃��\�b�h
	// ���s�ΏۂƂȂ��Ă���DLL�����ɑ{�����āc
	for ( vector<HMODULE>::iterator it = listOfTargetDLL.begin(); it != listOfTargetDLL.end(); it++ ) {

		// �Ώۂ�DLL��OnSelectingScenarioDaimyoStart�֐������݂��邩���`�F�b�N�B
		PFNTARGETMODVIEWINGTOPMENU p_ViewingTopMenu = (PFNTARGETMODVIEWINGTOPMENU)GetProcAddress(*it, "OnViewingTopMenu");

		if ( !p_ViewingTopMenu ) {
			continue;
		}

		((PFNTARGETMODVIEWINGTOPMENU) p_ViewingTopMenu)();
	}

	// �V�i���IMod�̓��l�̃��\�b�h
	if ( p_snOnViewingTopMenu ) {
		p_snOnViewingTopMenu();
	}
}


/*
0049D7AA  |. 6A 03          |PUSH    3                               ; |Arg3 = 00000003
0049D7AC  |. 56             |PUSH    ESI                             ; |Arg2
0049D7AD  |. 56             |PUSH    ESI                             ; |Arg1
0049D7AE  |. E8 D3540000    |CALL    TENSHOU.004A2C86                ; \TENSHOU.004A2C86
				�������� JMP TSMod.OnTSExeViewingTopMenu �Ə��������Ă����ۂ̏���
0049D7B3  |. 83C4 1C        |ADD     ESP, 1C
0049D7B6  |. 85C0           |TEST    EAX, EAX
*/
int pTenshouExeJumpFromToOnTenshouExeViewingTopMenu		=0x49D7AE; // �֐��͂��̃A�h���X����AOnTenshouExeViewingTopMenu�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeViewingTopMenu =0x4A2C86; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeViewingTopMenu	=0x49D7B3; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeViewingTopMenu() {
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
	OnTenshouExeViewingTopMenuExecute();

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
		call pTenshouExeJumpCallFromToOnTenshouExeViewingTopMenu

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingTopMenu
	}
}

/*
0049D7AA  |. 6A 03          |PUSH    3                               ; |Arg3 = 00000003
0049D7AC  |. 56             |PUSH    ESI                             ; |Arg2
0049D7AD  |. 56             |PUSH    ESI                             ; |Arg1
0049D7AE  |. E8 D3540000    |CALL    TENSHOU.004A2C86                ; \TENSHOU.004A2C86
				�������� JMP TSMod.OnTSExeViewingTopMenu �Ə��������Ă����ۂ̏���
0049D7B3  |. 83C4 1C        |ADD     ESP, 1C
0049D7B6  |. 85C0           |TEST    EAX, EAX
*/
char cmdOnTenshouExeJumpFromViewingTopMenu[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeViewingTopMenu() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeViewingTopMenu;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingTopMenu + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingTopMenu  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromViewingTopMenu+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingTopMenu), cmdOnTenshouExeJumpFromViewingTopMenu, 5, NULL); //5�o�C�g�̂ݏ�������
}




