#include "WinTarget.h"

#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXBushou_All.h"

/*
 * �푈���Ƀ��j�b�g����U�����悤�Ƃ���Ƃ��ɁA�U���\���������ǂ����𔻒肷��B
 * ���̒l��傫������΁A���ɉ����̓G�ł��U�����邱�Ƃ��\���B
 */

/*
0042B428  |. 57             PUSH    EDI                              ; |Arg1
0042B429  |. E8 8A000000    CALL    TENSHOU.0042B4B8                 ; \TENSHOU.0042B4B8
0042B42E  |. 83C4 08        ADD     ESP, 8


0042B431  |. 85C0           TEST    EAX, EAX
0042B433  |. 74 14          JE      SHORT TENSHOU.0042B449
0042B435  |. 3B5D F0        CMP     EBX, DWORD PTR SS:[EBP-10] ��EDI�̕����̓S�C�̓K����EBX�ɃR�s�[
0042B438  |. 72 0F          JB      SHORT TENSHOU.0042B449
0042B43A  |. 837D EC 02     CMP     DWORD PTR SS:[EBP-14], 2 ���S�C�����ǂ����̔���
0042B43E  |. 74 06          JE      SHORT TENSHOU.0042B446
0042B440  |. 837D EC 03     CMP     DWORD PTR SS:[EBP-14], 3 ���R�n�S�C�����ǂ����̔���
0042B444  |. 75 03          JNZ     SHORT TENSHOU.0042B449  ���R�n�S�C�����ł��Ȃ��Ȃ�΁c
*/

/*
 * 
 */
//
int isBushouIDJudgeRemoteAttack = -1; // �����[�g�U���\���ǂ�������ΏۂƂȂ镐���ԍ�
int iRemoteAptituteOfJudgeTorTK = 0;
int iRemoteJohekiTsukinuke = 0;

void OnTenshouExeCanAnyAttackDistanceExecute() {

	iRemoteJohekiTsukinuke = 0; // ���ʏ�ǂ͓˂��������Ȃ�

	int iBushouID = (isBushouIDJudgeRemoteAttack & 0xFFFF) -1;

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// �Ώە�����(��ɋ����)�ʒu�𓾂�B
		HEX_POSITION p = GetFieldHexUnitPos( iBushouID );
		// �����̈ʒu�̖����𓾂�
		int iHexRole = GetFieldHexRole( p );

		// ���ȉ��ł͋��������炵�Ă��邪�A�����𑝂₷���Ƃ��\��!!
		// ���y�ŋ|���a�ȏ�ł���΁c
		if ( nb6bushouref[iBushouID].form == 0 && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3) { // ���y�ŋ|���a�ȏ�ł���΁c
			
			if ( iHexRole == FIELD_HEX_ROLE::�C || iHexRole == FIELD_HEX_ROLE::�� ) {
				iRemoteAptituteOfJudgeTorTK = 0; // �|�K���Ƃ��Ă͂R�e�܂łn�j�B
			} else if (nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi == 5 ) {
				iRemoteAptituteOfJudgeTorTK = 3; // �|�K���Ƃ��Ă͂R�e�܂łn�j�B
			} else {
				iRemoteAptituteOfJudgeTorTK = 2; // �|�K���Ƃ��Ă͂Q�e�܂łn�j�B
			}

		// �R�n�ŋ|���a�ȏ�ł���΁c
		// �R�n�K�����a�ȏ�ŁA�|�K�����a�ȏ�Ȃ�΁A�����[�g�U���\
		} else if ( nb6bushouref[iBushouID].form == 1 && nb6bushouref[iBushouID].aptitudekib >= 3 && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3) {

			if ( iHexRole == FIELD_HEX_ROLE::�C || iHexRole == FIELD_HEX_ROLE::�� ) {
				iRemoteAptituteOfJudgeTorTK = 0; // �|�K���Ƃ��Ă͂R�e�܂łn�j�B
			} else if (nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi == 5 ) {
				iRemoteAptituteOfJudgeTorTK = 3; // �|�K���Ƃ��Ă͂R�e�܂łn�j�B
			} else {
				iRemoteAptituteOfJudgeTorTK = 2; // �|�K���Ƃ��Ă͂Q�e�܂łn�j�B
			}

		// �S�C���łĂ�
		}
	}

}


/*
0042B428  |. 57             PUSH    EDI                              ; |Arg1
0042B429  |. E8 8A000000    CALL    TENSHOU.0042B4B8                 ; \TENSHOU.0042B4B8
							�������� JMP TSMod.OnTSExeCanAnyAttackDistance �Ə��������Ă����ۂ̏���
0042B42E  |. 83C4 08        ADD     ESP, 8
*/
int pTenshouExeJumpFromToOnTenshouExeCanAnyAttackDistance		=0x42B429; // �֐��͂��̃A�h���X����AOnTenshouExePreviousBattleMenuInBattle�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeCanAnyAttackDistance =0x42B4B8; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeCanAnyAttackDistance	=0x42B42E; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeCanAnyAttackDistance() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov isBushouIDJudgeRemoteAttack, EDI // EDI�Ɍ��ݑΏۂƂȂ镐���ԍ�������B

		mov iRemoteAptituteOfJudgeTorTK, EBX // �S�C�̒l

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	OnTenshouExeCanAnyAttackDistanceExecute();

	// ��Ǔ˂������n�j�Ȃ�΁c
	if ( iRemoteJohekiTsukinuke ) {

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

			call pTenshouExeJumpCallFromToOnTenshouExeCanAnyAttackDistance // �������̍s�����s���Ȃ���΁A��ǂ̍������ւƌ����Ƃ��\�ƂȂ�B

			mov EBX, iRemoteAptituteOfJudgeTorTK; // �|�ł���΁A�S�C�˒���Z�����邱�ƂŁA�Ώ�����B

			jmp pTenshouExeReturnLblFromOnTenshouExeCanAnyAttackDistance;
		}

	// ��Ǔ˂������o���Ȃ����ʂ̏��
	} else {
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

			call pTenshouExeJumpCallFromToOnTenshouExeCanAnyAttackDistance // �������̍s�����s���Ȃ���΁A��ǂ̍������ւƌ����Ƃ��\�ƂȂ�B

			mov EBX, iRemoteAptituteOfJudgeTorTK; // �|�ł���΁A�S�C�˒���Z�����邱�ƂŁA�Ώ�����B

			jmp pTenshouExeReturnLblFromOnTenshouExeCanAnyAttackDistance;
		}
	}
}





char cmdOnTenshouExeJumpFromCanAnyAttackDistance[6]="\xE9";
// ���̖��߂��T�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeCanAnyAttackDistance() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeCanAnyAttackDistance;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeCanAnyAttackDistance + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeCanAnyAttackDistance  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromCanAnyAttackDistance+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeCanAnyAttackDistance), cmdOnTenshouExeJumpFromCanAnyAttackDistance, 5, NULL); //5�o�C�g�̂ݏ�������
}


