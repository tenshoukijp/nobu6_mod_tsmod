#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"



int iWeatherRainRangeJudge = 0;
bool isUseCurBushouAmeDeppou = false; // ���̕������J�S�C���g�����ǂ���

int iCurBushouIDOnRainRangeJudge = -1; // TENSHOU.EXE����n���Ă���Ώۂ̕���


bool isCurBushouHasAHoujutsusyo(int iBushouID) {
	// �ƕ����ʂ蒲�ׂ�
	for (int iKahouID=0; iKahouID<GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {

		// ���L�҂̕���ID����v����ꍇ�̂ݔ���̑Ώ�
		if (nb6kahou[iKahouID].attach-1 == iBushouID) {

			// �Ώۂ̉ƕ󂪁u�C�p���v�Ƃ��������A�������́u�C���v�Ƃ����������܂�ł���ꍇ true
			if ( strstr(nb6kahou[iKahouID].name, "�C�p��") ) {
				return true;
			}
			if ( strstr(nb6kahou[iKahouID].name, "�C��") ) {
				return true;
			}
		}
	}
	// ������Ȃ����false
	return false;
}

// �Ώە����͉J�S�C���\���ǂ���
bool IsCanAmeDeppou(int iBushouID ) {

	if (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		// �^���^���X�Ȃ�΂��肦�Ȃ��V�C�Ɣ�r����B���Ȃ킿�A�S�C�͕K�����Ă�B
		if (isLancelotTartare(iBushouID)) {

			iWeatherRainRangeJudge = 7;
			isUseCurBushouAmeDeppou = true;
			return true;

		// ��ɂȂ�΂��肦�Ȃ��V�C�Ɣ�r����B���Ȃ킿�A�S�C�͕K�����Ă�B
		} else if (isShuiHidechiyo(iBushouID)) {

			iWeatherRainRangeJudge = 7;
			isUseCurBushouAmeDeppou = true;
			return true;

		// �C�p���E�C���������Ă��镐�����J�S�C�����Ă���̂Ƃ���B
		} else if ( isCurBushouHasAHoujutsusyo(iBushouID) ) {

			iWeatherRainRangeJudge = 7;
			isUseCurBushouAmeDeppou = true;
			return true;

		} else {

			// �֐���DLL�Œ�`����Ă�����ACONDITION�Ƃ��ČĂяo���A���̌��ʂ�TRUE�ł���΁c �J�S�C�����Ƃ��\�B
			if ( p_snOnCustomCondition ) {
				int iAmeDeppouCanResult = ((PFNSNONCUSTOMCONDITION) p_snOnCustomCondition)("OR����::�푈::�J�S�C��", iBushouID+1, 0, 0, "", "", 0.0, 0.0);
				if ( iAmeDeppouCanResult == TRUE ) {
					iWeatherRainRangeJudge = 7;
					isUseCurBushouAmeDeppou = true;
					return true;
				}
			}

			return false;
		}
	}

	return false;
}

void OnTenshouExeRainRangeJudgeExecute() {

	//	cmp eax, 2
	// ����2��ʂ̒l�ɂ���K�v�����邩�ǂ����𔻒f����B2�ł͂Ȃ�7�Ȃǂɂ���΁A�u�J�Ƃ̔�r�ł͂Ȃ��A���݂��Ȃ��V�C�Ƃ̔�r�v���ƂɂȂ邩��A�S�C���C�����Ă�B

	// �܂��Q�D����͏󋵂ɂ������Ȃ��K�v�B
	iWeatherRainRangeJudge = 2;
	isUseCurBushouAmeDeppou = false;

	int iBushouID = iCurBushouIDOnRainRangeJudge-1;

	if (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		if (IsCanAmeDeppou(iBushouID)) {
			iWeatherRainRangeJudge = 7;
			isUseCurBushouAmeDeppou = true;
		}

		// �|���������͋|�R�n���ł���΁A�J�S�C�\�ƂȂ�
		// ���y�ŋ|���a�ȏ�ł���΁c
		if ( nb6bushouref[iBushouID].form == 0 && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3) { // ���y�ŋ|���a�ȏ�ł���΁c
			iWeatherRainRangeJudge = 7;

		// �R�n�K�����a�ȏ�ŁA�|�K�����a�ȏ�Ȃ�΁A�����[�g�U���\
		} else if ( nb6bushouref[iBushouID].form == 1 && nb6bushouref[iBushouID].aptitudekib >= 3 && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3) {
			iWeatherRainRangeJudge = 7;
		}

	}
}


/*
/*
0042B3F0  |. FE45 FC        INC     BYTE PTR SS:[EBP-4]
0042B3F3  |> E8 32AF0600    CALL    TENSHOU.0049632A		�� ������EAX�ɓV�C�������Ă��� 0������A1���܂�A2���J
				�������� JMP TSMod.OnTSExeRainRangeJudge �Ə��������Ă����ۂ̏���
0042B3F8  |. 83F8 02        CMP     EAX, 2					�� �����ŉJ���ǂ������肵�Ă���0������A1���܂�A2���J
0042B3FB  |. 72 10          JB      SHORT TENSHOU.0042B40D
0042B3FD  |. 837D F4 1B     CMP     DWORD PTR SS:[EBP-C], 1B
*/
int pTenshouExeJumpFromToOnTenshouExeRainRangeJudge		=0x42B3F3; // �֐��͂��̃A�h���X����AOnTenshouExeRainRangeJudge�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeRainRangeJudge =0x49632A; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeRainRangeJudge	=0x42B3FB; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeRainRangeJudge() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ������ID��ۑ�
		mov iCurBushouIDOnRainRangeJudge, edi

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeRainRangeJudge

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
	OnTenshouExeRainRangeJudgeExecute();

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

		cmp eax, iWeatherRainRangeJudge		// �f�t�H���g�ł́AiWeatherRainRangeJudge�ɂ͂Q(�J)�����邪�A7(���݂����Ȃ��V�C)�Ȃǂɂ���ΕK���S�C�����Ă邱�ƂƂȂ�B

		jmp pTenshouExeReturnLblFromOnTenshouExeRainRangeJudge
	}
}

/*
0049D7AA  |. 6A 03          |PUSH    3                               ; |Arg3 = 00000003
0049D7AC  |. 56             |PUSH    ESI                             ; |Arg2
0049D7AD  |. 56             |PUSH    ESI                             ; |Arg1
0049D7AE  |. E8 D3540000    |CALL    TENSHOU.004A2C86                ; \TENSHOU.004A2C86
				�������� JMP TSMod.OnTSExeRainRangeJudge �Ə��������Ă����ۂ̏���
0049D7B3  |. 83C4 1C        |ADD     ESP, 1C
0049D7B6  |. 85C0           |TEST    EAX, EAX
*/
char cmdOnTenshouExeJumpFromRainRangeJudge[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeRainRangeJudge() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeRainRangeJudge;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeRainRangeJudge + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeRainRangeJudge  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromRainRangeJudge+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeRainRangeJudge), cmdOnTenshouExeJumpFromRainRangeJudge, 5, NULL); //5�o�C�g�̂ݏ�������
}




