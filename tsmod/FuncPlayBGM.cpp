#include "WinTarget.h"

#include "FuncPlayBGM.h"
#include "GameDataStruct.h"

/*
�a�f�l���Đ������
0042BA2A   . 6A 08          PUSH    8
0042BA2C   . E8 B8190600    CALL    TENSHOU.0048D3E9
0042BA31   . 83C4 08        ADD     ESP, 8

*/


//---------------------�a�f�l��~-------------------------
int pCallFromFuncStopBGM = 0x48D483; // ���XTENSHOU.EXE���ɂ�����CALL��

void FuncStopBGMAsm() {

	__asm {
		call pCallFromFuncStopBGM
	}
}

// �w��̂a�f�l���~����
void FuncStopBGM() {

	__asm {
		push eax
		push ebx
		push ecx
		push edx
		push esp
		push esi
		push edi
	}

	// TENSHOU.EXE���̊֐��Ăяo��
	FuncStopBGMAsm();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

}



//---------------------�a�f�l�Đ�-------------------------
int pCallFromFuncPlayBGM = 0x48D3E9; // ���XTENSHOU.EXE���ɂ�����CALL��

int iBgmIdFuncPlayBGM = -1;

void FuncPlayBGMAsm() {

	__asm {
		push 0x1  // ���Ȃ̂��͂킩���c

		push iBgmIdFuncPlayBGM

		call pCallFromFuncPlayBGM

		add esp, 8
	}

}

// �w��̂a�f�l���Đ�����
void FuncPlayBGM(int iBGM) {
	// TSMod��_�p�b�`�x�[�X�̔ԍ���Tenshouki�����ԍ��ɒ����B
	if ( 1 <= iBGM && iBGM < 0xFFFF ) {
		iBGM--;
	}
	iBgmIdFuncPlayBGM = iBGM;

	__asm {
		push eax
		push ebx
		push ecx
		push edx
		push esp
		push esi
		push edi
	}

	// TENSHOU.EXE���̊֐��Ăяo��
	FuncPlayBGMAsm();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}


}


// �a�f�l���Đ������H
int isPlayBGM() {
	WORD *pBGMID = (WORD *)GAMEDATASTRUCT_PLAYING_BGM_ID_ADDRESS;
	return 1 <= *pBGMID && *pBGMID != 0xFFFF;
}

// �Đ�����BGM��ID�𓾂�B0xFFFF���ƍĐ����ł͂Ȃ��B
int getPlayingBGM() {
	WORD *pBGMID = (WORD *)GAMEDATASTRUCT_PLAYING_BGM_ID_ADDRESS;
	// tenshouki�����ԍ����_�p�b�`�ԍ��ɒ���
	if ( 1 <= *pBGMID && *pBGMID != 0xFFFF ) {
		return (*pBGMID)+1;
	} else {
		return 0xFFFF;
	}
}
