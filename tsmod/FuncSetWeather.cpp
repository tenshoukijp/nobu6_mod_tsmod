#include "WinTarget.h"

#include "GameDataStruct.h"

/*
00436101  |> 56             PUSH    ESI						�� �V�C�ԍ�
00436102  |. E8 29020600    CALL    TENSHOU.00496330		�� �V�C��ύX���āA�`����X�V�̊֐�
00436107  |. 83C4 04        ADD     ESP, 4
*/

int pCallFromFuncSetWeather = 0x496330; // ���XTENSHOU.EXE���ɂ�����CALL��

int iWeatherIDInFuncSetWeather = 1L;    // ���S����ID

// �V�C�𑦍��ɕύX(�������A�V����͍X�V����Ȃ��B���o�p)
void FuncSetWeatherAsm() {
	__asm {
		push iWeatherIDInFuncSetWeather
		call pCallFromFuncSetWeather
		add esp, 0x4
	}
}


// �V�C�𑦍��ɕύX(�������A�V����͍X�V����Ȃ��B���o�p)
void FuncSetWeather(int iWeatherID) {

	// �֐��p�̂�́A1�����̂ŁA��������
	iWeatherIDInFuncSetWeather = iWeatherID;

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
	FuncSetWeatherAsm();

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
