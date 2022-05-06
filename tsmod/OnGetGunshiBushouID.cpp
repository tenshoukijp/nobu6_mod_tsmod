#include "WinTarget.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"
#include "EXBushou_All.h"
#include "ScenarioMod.h"
#include "HumanRelation.h"


/*
 �R�t�����h�c���K�v�Ƃ��ꂽ��
 */


//---------------------------------------------------------------------------------
// �R�c�ԍ��������Ƃ��Ă�����Ă���̂Ŋo���Ă���
int iOriginalGundanIDOfOnGunshiBushouIDPrev = -1;

void OnTenshouExeGetGunshiBushouIDPrevExecute() {
	iOriginalGundanIDOfOnGunshiBushouIDPrev = iOriginalGundanIDOfOnGunshiBushouIDPrev & 0xFFFF; // ������������������Ȃ��̂Ńt�B���^�[

	// �R�c�ԍ��͈̔͂ł���΁AiGundanID�ɂ��Ă����B
	if ( 0 <= iOriginalGundanIDOfOnGunshiBushouIDPrev-1 && iOriginalGundanIDOfOnGunshiBushouIDPrev-1 < GAMEDATASTRUCT_GUNDAN_NUM ) {
		iOriginalGundanIDOfOnGunshiBushouIDPrev = iOriginalGundanIDOfOnGunshiBushouIDPrev-1;
	}
}



/*
00454BF1  |. FF7424 14      PUSH    DWORD PTR SS:[ESP+14]
00454BF5  |. E8 21F0FFFF    CALL    TENSHOU.00453C1B
				�������� JMP TSMod.OnTSExeGetGunshiBushouIDPrev �Ə��������Ă����ۂ̏���
00454BFA  |. 66:8BD8        MOV     BX, AX
*/
int pTenshouExeJumpFromToOnTenshouExeGetGunshiBushouIDPrev	   =0x454BF5; // �֐��͂��̃A�h���X����AOnTenshouExeGetGunshiBushouIDPrev�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeGetGunshiBushouIDPrev =0x453C1B; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeGetGunshiBushouIDPrev  =0x454BFA; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeGetGunshiBushouIDPrev() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// �R�c�ԍ����T���Ă���
		mov iOriginalGundanIDOfOnGunshiBushouIDPrev, eax

		// TENSHOU.EXE�̌��X�̏�����������
		call pTenshouExeJumpCallFromToOnTenshouExeGetGunshiBushouIDPrev

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnTenshouExeGetGunshiBushouIDPrevExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeGetGunshiBushouIDPrev
	}
}



char cmdOnTenshouExeJumpFromGetGunshiBushouIDPrev[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeGetGunshiBushouIDPrev() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeGetGunshiBushouIDPrev;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeGetGunshiBushouIDPrev + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGetGunshiBushouIDPrev  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromGetGunshiBushouIDPrev+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeGetGunshiBushouIDPrev), cmdOnTenshouExeJumpFromGetGunshiBushouIDPrev, 5, NULL); //5�o�C�g�̂ݏ�������
}





















//-----------------------------------------------------------------------------------------------------------

/*
 �����̌��ʂ��o���̂��i�[
 */

/*
00454CBC  |> 66:8BC6        MOV     AX, SI
00454CBF  |. 5D             POP     EBP
00454CC0  |. 5F             POP     EDI
00454CC1  |. 5E             POP     ESI
00454CC2  |. 5B             POP     EBX
00454CC3  \. C3             RETN
*/

WORD iOrgBushouIDOfOnGunshiBushouIDAfter = 0xFFFF;
WORD iNewBushouIDOfOnGunshiBushouIDAfter = 0xFFFF;

void OnTenshouExeGetGunshiBushouIDAfterExecute() {

	// �㏑���p�B�����I�ɂ́A��Ɏ󂯎�����l���̂܂�
	iNewBushouIDOfOnGunshiBushouIDAfter = iOrgBushouIDOfOnGunshiBushouIDAfter; 

	int iBushouID = iOrgBushouIDOfOnGunshiBushouIDAfter-1; // AX���������������ԍ�

	// �����łȂ�������݁B���Ȃ�������0xFFFF�ɂ��ǂ��B
	if ( ! (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) ) {
		iBushouID = 0xFFFF;
	}

	// �w��̌R�c�͂���B
	int iGundanID = iOriginalGundanIDOfOnGunshiBushouIDPrev;

	
	// ���̂Q����\�͂Ɋւ�炸���ʂ��㏑������B
	if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {

		// ���X���[���A���n����
		// �v������Ă���R�c�̃��[�_�[
		int iLeaderBushouID = nb6gundan[iGundanID].leader - 1;

		// �����Ƌ�����c
		if ( 0 <= iLeaderBushouID && iLeaderBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

			// �w��R�c�̃��[�_�[�̓����X���b�g�^���^���X�B
			// �o�[���[�t�H����T��
			if ( isLancelotTartare( iLeaderBushouID ) ) {
				for ( int i=0; i<GAMEDATASTRUCT_BUSHOU_NUM; i++) {
					// �o�[���[�t�H�������݂���B
					if ( strcmp( nb6bushouname[i].familyname, "�I�J�K" )==0 && strcmp( nb6bushouname[i].fastname, "�L�M�N" )==0 ) {
						// �o�[���[�t�H���ƃ^���^���X�������R�c��
						if ( nb6bushouref[i].gundan == nb6bushouref[iLeaderBushouID].gundan ) {
							iNewBushouIDOfOnGunshiBushouIDAfter = i+1; // �o�[���[�t�H����iBushouID�������ԍ���

							break;
						}
					}
				}
			}

			else {
				// ��ʓI�ȌR�t�A���S���Y����ύX����B
				vector<int> GundanBushouList;
				vector<int> GundanBushouParamList;
				for (int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

					// �Y���̌R�c�ɏ������Ă���B���A��Ԃ������ł���B(�喼��R�c���A�B����Q�l�E�P���ł͂Ȃ��B)
					if ( nb6bushouref[iBushouID].gundan-1 == iGundanID && nb6bushouname[iBushouID].State == 2 ) {

						// �����{�q�d��200�ȏ�A�������́A����+�퓬+�q�d�̍��v�l��250�ȏ�
						if ( (getBushouGov( iBushouID ) + getBushouInt( iBushouID ))  >= 100 ||
							 (getBushouGov( iBushouID ) + getBushouBat( iBushouID ) + getBushouInt( iBushouID ) ) >= 125 ) {
							// �����ǉ�
							GundanBushouList.push_back( iBushouID );

							// �\�͍��v�l(���̕]���l)
							int sum_param = getBushouGov( iBushouID ) + getBushouBat( iBushouID ) + getBushouInt( iBushouID );

							// �R�c���ƑΏە����͌����ł���B�������A�q���ł͂Ȃ��B
							if ( is�����͑���̌���(iBushouID, iLeaderBushouID ) && !is�����͑���̎q(iBushouID, iLeaderBushouID ) ) {
								sum_param += int(sum_param * 0.2); // 20���]�����Z
							}

							// �d���N�����Q�Ŋ������l�́��𑫂����ށB
							sum_param += int((sum_param * nb6bushouref[iBushouID].work)/200);

							// �h�V����20���A�ƘV����15���A��������10�� ���叫����5�� �̉��Z�B
							sum_param += sum_param * (6-nb6bushouname[iBushouID].position) * 5 / 100;

							// �\�͒ǉ�
							GundanBushouParamList.push_back( sum_param );
						}
					}
				}

				// �Y���҂O�Ȃ牽�����Ȃ��B�{���̏����̂܂܂ł��n�j�D
				// �Y���҂��P�l�ł�����΁c
				if ( GundanBushouList.size() > 0 ) {

					// �N����Ԕ\�͂������̂��{��
					int iGunshiBushou = 0xFFFF;
					int iMaxSumParam = 0;

					for ( int i=0; i<(int)GundanBushouList.size(); i++ ) {
						// �ő�̐l
						if ( GundanBushouParamList[i] > iMaxSumParam ) {
							iGunshiBushou = GundanBushouList[i];
							iMaxSumParam = GundanBushouParamList[i];
						}
					}

					iNewBushouIDOfOnGunshiBushouIDAfter = iGunshiBushou+1;
				}

			}

			// ScenarioMod�̃J�X�^�������ŌR�t��ݒ肷��
			if ( p_snOnCustomCondition ) {															              //���R�c�ԍ�   �����R�t�����ԍ�  
				int isNewGunshiBushouID = ((PFNSNONCUSTOMCONDITION) p_snOnCustomCondition)("AS����::�R�t�����ԍ�", iGundanID+1, iOrgBushouIDOfOnGunshiBushouIDAfter, 0, "", "", 0.0, 0.0);
				// �R�t���ύX�ƂȂ��Ă���B
				if ( isNewGunshiBushouID != CUSTOM_CONDITION_NOMODIFY ) {
					iNewBushouIDOfOnGunshiBushouIDAfter = isNewGunshiBushouID;
				}
			}

		}

	}
}



/*
00454CBC  |> 66:8BC6        MOV     AX, SI
				�������� JMP TSMod.OnTSExeGetGunshiBushouIDAfter �Ə��������Ă����ۂ̏���
00454CBF  |. 5D             POP     EBP
00454CC0  |. 5F             POP     EDI
00454CC1  |. 5E             POP     ESI
00454CC2  |. 5B             POP     EBX
00454CC3  \. C3             RETN
*/
int pTenshouExeJumpFromToOnTenshouExeGetGunshiBushouIDAfter	   =0x454CBC; // �֐��͂��̃A�h���X����AOnTenshouExeGetGunshiBushouIDAfter�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeGetGunshiBushouIDAfter  =0x454CC1; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeGetGunshiBushouIDAfter() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// ���Xtenshou.exe�ɂ�����������������
		MOV     AX, SI

		mov iOrgBushouIDOfOnGunshiBushouIDAfter, AX

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnTenshouExeGetGunshiBushouIDAfterExecute();

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

		// �㏑�����ׂ��Ȃ�㏑������B
		MOV     AX, iNewBushouIDOfOnGunshiBushouIDAfter

		// ���XTENSHOU.EXE���ɂ��������̂�������
		POP     EBP
		POP     EDI

		jmp pTenshouExeReturnLblFromOnTenshouExeGetGunshiBushouIDAfter
	}
}



char cmdOnTenshouExeJumpFromGetGunshiBushouIDAfter[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeGetGunshiBushouIDAfter() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeGetGunshiBushouIDAfter;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeGetGunshiBushouIDAfter + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeGetGunshiBushouIDAfter  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromGetGunshiBushouIDAfter+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeGetGunshiBushouIDAfter), cmdOnTenshouExeJumpFromGetGunshiBushouIDAfter, 5, NULL); //5�o�C�g�̂ݏ�������
}


