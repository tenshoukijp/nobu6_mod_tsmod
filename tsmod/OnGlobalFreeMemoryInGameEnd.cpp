#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "SoundServer.h"
#include "TimeStamp.h"
#include "ScenarioMod.h"
#include "PluginMod.h"

#include "FuncImagePngDialog.h"

#include "TenShougiMod.h"


extern int iSelectingScenarioLocation;
extern int iSelectingScenarioID;

void OnTenshouExeGlobalFreeMemoryInGameEndExecute() {
	CommonGlobalHandle::isNobunaganoYabouGameEnd = true;

	iSelectingScenarioLocation = -1;
	iSelectingScenarioID = -1;

	// �S�Ẳ����~�߂�B
	StopAllSound();


	// PluginMod��Finalize���\�b�h
	// �ǂݍ���DLL�Q�ɑ΂��āAOnFinalize������΁A��������s�Ȃ���Ή��
	for ( vector<HMODULE>::iterator it = listOfTargetDLL.begin(); it != listOfTargetDLL.end(); it++ ) {

		// DLL���ǂ߂��̂ł���΁AOnFinalize�֐������݂��邩���`�F�b�N�B
		PFNTARGETMODFINALISE p_OnFinalize = (PFNTARGETMODFINALISE)GetProcAddress(*it, "OnFinalize");

		if ( !p_OnFinalize ) { continue; }

		// OnFinalize�����݂���Ύ��s
		((PFNTARGETMODFINALISE) p_OnFinalize)();
	}


	// ScenarioMod�̃f�X�g���N�^
	if ( p_snOnDestructor ) {
		p_snOnDestructor();
	}

	// �V�i���IMod�I���̃��\�b�h
	if ( p_snOnDllMainEnd ) {
		p_snOnDllMainEnd();
	}

	// �����V����
	TenShougiModEnd();


	// �������g�̃^�C���X�^���v����ԃV���[�Y�ƈقȂ�΁c)
	if ( !isTimeStampTeibanSeries ) {
		ShellExecute(NULL, NULL, TIMESTAMPERFILENAME, TIMESTAMPERFILEOPTION, NULL, SW_SHOWNORMAL);
	}
}


/*
00496B82  /$ C705 30C24B00 >MOV     DWORD PTR DS:[4BC230], 1
00496B8C  |. E8 30FEFFFF    CALL    TENSHOU.004969C1
				�������� JMP TSMod.OnTSExeGlobalFreeMemoryInGameEnd �Ə��������Ă����ۂ̏���
00496B91  |. B9 54684E00    MOV     ECX, TENSHOU.004E6854
*/
int pTenshouExeJumpFromToOnTenshouExeGlobalFreeMemoryInGameEnd		=0x496B8C; // �֐��͂��̃A�h���X����AOnTenshouExeGlobalFreeMemoryInGameEnd�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeGlobalFreeMemoryInGameEnd  =0x4969C1; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeGlobalFreeMemoryInGameEnd	=0x496B91; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeGlobalFreeMemoryInGameEnd() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeGlobalFreeMemoryInGameEnd

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
	OnTenshouExeGlobalFreeMemoryInGameEndExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeGlobalFreeMemoryInGameEnd
	}
}

/*
00496B82  /$ C705 30C24B00 >MOV     DWORD PTR DS:[4BC230], 1
00496B8C  |. E8 30FEFFFF    CALL    TENSHOU.004969C1
				�������� JMP TSMod.OnTSExeGlobalFreeMemoryInGameEnd �Ə��������Ă����ۂ̏���
00496B91  |. B9 54684E00    MOV     ECX, TENSHOU.004E6854
*/
char cmdOnTenshouExeJumpFromGlobalFreeMemoryInGameEnd[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeGlobalFreeMemoryInGameEnd() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeGlobalFreeMemoryInGameEnd;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeGlobalFreeMemoryInGameEnd + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGlobalFreeMemoryInGameEnd  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromGlobalFreeMemoryInGameEnd+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeGlobalFreeMemoryInGameEnd), cmdOnTenshouExeJumpFromGlobalFreeMemoryInGameEnd, 5, NULL); //5�o�C�g�̂ݏ�������
}




