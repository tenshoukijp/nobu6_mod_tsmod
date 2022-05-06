#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"



/*
00496331  |. 8B7424 08      MOV     ESI, DWORD PTR SS:[ESP+8]		�� ��������V�L�ԍ����R�s�[�����B
00496335  |. 56             PUSH    ESI								�� ���̓V�󂪓V�C�ԍ�
00496336  |. E8 09C4FFFF    CALL    TENSHOU.00492744
0049633B  |. 83C4 04        ADD     ESP, 4
*/

/*
 �V���ȓV�󂪃Z�b�g����鎞
 */
//  0:���� 1:�܂� 2:�J 3:��


vector<TReservedTenki> iReservedWeatherOnChangeWeather;  // �\��V�C
int iWeatherOnChangeWeather = -1;  // �ݒ肳���\��̖{���̓V�C

void OnTenshouExeChangeWeatherExecute() {
	// �V�󂪗\�񂳂�Ă���Ȃ�΁c
	if ( !iReservedWeatherOnChangeWeather.empty() ) {

		if ( 0<=iReservedWeatherOnChangeWeather[0].iTenki && iReservedWeatherOnChangeWeather[0].iTenki <= 3 ) {

			iWeatherOnChangeWeather = iReservedWeatherOnChangeWeather[0].iTenki; // ���̓V��ŏ㏑������

			// �\�񂵂Ă���iContinueTurn�^�[��������������ʂ��؂ꂽ�B
			if ( getRemainTurnInBattle() <= iReservedWeatherOnChangeWeather[0].iRemainTurn -  iReservedWeatherOnChangeWeather[0].iContinueTurn ) {
				iReservedWeatherOnChangeWeather.clear(); // �g�����̂ŗ\��͉���
			} else if ( getRemainTurnInBattle() == 0 ) {
				iReservedWeatherOnChangeWeather.clear(); // �g�����̂ŗ\��͉���
			}
		}
	}

	// �J�X�^�������ŁA�V��ύXAS����
	if ( p_snOnCalcWeatherInBattle ) {

		// �V�C�ԍ�																		      �����ݗ\�肳��Ă���V�C�ԍ�   ���c��^�[����
		int isWarWeatherResult = ((PFNSNONCALCWEATHERINBATTLE) p_snOnCalcWeatherInBattle)(iWeatherOnChangeWeather, getRemainTurnInBattle() );
		if ( isWarWeatherResult != -1 ) {

			// �V�C���㏑������B
			iWeatherOnChangeWeather = isWarWeatherResult;
		}
	}

}


/*
00496331  |. 8B7424 08      MOV     ESI, DWORD PTR SS:[ESP+8]		�� ��������V�L�ԍ����R�s�[�����B
00496335  |. 56             PUSH    ESI								�� ���̓V�󂪓V�C�ԍ�
00496336  |. E8 09C4FFFF    CALL    TENSHOU.00492744
				�������� JMP TSMod.OnTSExeChangeWeather �Ə��������Ă����ۂ̏���
0049633B  |. 83C4 04        ADD     ESP, 4
*/
int pTenshouExeJumpFromToOnTenshouExeChangeWeather	     =0x496336; // �֐��͂��̃A�h���X����AOnTenshouExeChangeWeather�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeChangeWeather   =0x492744; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeChangeWeather    =0x49633B; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w

// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeChangeWeather() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iWeatherOnChangeWeather, esi   // �{���̓V����R�s�[���Ă����B

		pop esi								// �V��̃v�b�V���͂Ȃ��������Ƃɂ���B

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
	OnTenshouExeChangeWeatherExecute();

	__asm {

		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		mov esi, iWeatherOnChangeWeather   //�@�V����㏑������B

		push esi // ���X��TENSHOU.EXE���̋L�q

		// ���XTENSHOU.EXE���ŌĂяo���Ă����֐����Ăяo��
		call pTenshouExeJumpCallFromToOnTenshouExeChangeWeather

		jmp pTenshouExeReturnLblFromOnTenshouExeChangeWeather
	}
}

/*
*/
char cmdOnTenshouExeJumpFromChangeWeather[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeChangeWeather() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeChangeWeather;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeChangeWeather + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeChangeWeather  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromChangeWeather+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeChangeWeather), cmdOnTenshouExeJumpFromChangeWeather, 5, NULL); //5�o�C�g�̂ݏ�������
}




