#include "CommonGlobalHandle.h"
#include "Menu.h"

#define SOUNDSERVER_FILENAME  "SNDmm.dll"


// �R�c�����T�[�o�[���N��
void OnTenshouExeCallSoundModServer() {
	// SoundMM���Ă΂�Ă�H
	HMODULE h = GetModuleHandle(SOUNDSERVER_FILENAME);

	// ���������Ăяo���Ă��Ȃ��̂ŁA�������ă��[�h
	HINSTANCE irtnValue = ShellExecute(NULL, NULL, "rundll32.exe", SOUNDSERVER_FILENAME " EntryPoint", NULL, SW_HIDE);

	if ( (int)irtnValue > 32 ) { // �֐�����������ƁA32 ���傫���l���Ԃ�܂��B�֐������s����ƁA32 �ȉ��̒l���Ԃ�܂��B
		// AddModuleItem( (HMODULE)OnTenshouExeCallSoundModServer, "sndmm" ); // (HMODULE)CheckINMMModule�Ƃ����̂̓��j�[�N�Ȕԍ����m�ۂ��邽�߁B
	}

}

