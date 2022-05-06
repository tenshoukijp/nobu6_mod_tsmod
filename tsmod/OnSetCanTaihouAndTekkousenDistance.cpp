#include "WinTarget.h"

#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXBushou_All.h"

/*
 * ��C�̋����i���ۂɂ͓K���l�j�K�����Z�b�g����B
 * �|���ŉ����Ă��܂�����{�����������ł����Ȃ����̂��B
 */


extern int isBushouIDJudgeRemoteAttack; // �����[�g�U���\���ǂ�������ΏۂƂȂ镐���ԍ�

int iCanTaihouAndTekkousenDistance = -1;
int iCanTaihouAndTekkousenDistanceCompareVariable = -1;
void OnTenshouExeSetCanTaihouAndTekkousenDistanceExecute() {
	// �S�C�K���Ƒ�C�}�X�����̊֌W
	/*
	S 5
	A 4
	B 3
	C 3
	D 2
	E 2
	*/
	int TaihouDistanceArray[] = {2,2,3,3,4,5,5,6,6,7,7}; // �S�C�K���Ƒ勗���e�ڂ̊֌W�z��
	int iBushouID = (isBushouIDJudgeRemoteAttack & 0xFFFF)-1;

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		// �S�C�̓K���𓾂�
		int iTeppouAptitude = getBushouAptitude( iBushouID, APTITUDE_TEPPOU );

		// ���̏ꍇ
		if ( Is_FieldWar() ) {

			// �Ώە�����(��ɋ����)�ʒu�𓾂�B
			HEX_POSITION p = GetFieldHexUnitPos( iBushouID );
			// �����̈ʒu�̖����𓾂�
			int iHexRole = GetFieldHexRole( p );

			// ��C�������Ă��邩�c �������́A�S�b�D�������ĂĊC�̏�
			if ( (nb6bushouref[iBushouID].biggun) || (nb6bushouref[iBushouID].steelship && (iHexRole==FIELD_HEX_ROLE::�C||iHexRole==FIELD_HEX_ROLE::��) ) ) {

				if ( nb6bushouref[iBushouID].form == 0 && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3) { // ���y�ŋ|���a�ȏ�ł���΁c
					// �S�C�K������A��C�����}�X�ڂ��擾
					iCanTaihouAndTekkousenDistance = TaihouDistanceArray[iTeppouAptitude]; // 
				}

				// �|�R�n�̎�
				else if ( nb6bushouref[iBushouID].form == 1 && nb6bushouref[iBushouID].aptitudekib >= 3 && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3) {
					// �S�C�K������A��C�����}�X�ڂ��擾
					iCanTaihouAndTekkousenDistance = TaihouDistanceArray[iTeppouAptitude]; // 
				}

				// �C���ŁA���C�Ȃ�΋��������Z�c
				if ( nb6bushouref[iBushouID].job == 0x50 && (iHexRole==FIELD_HEX_ROLE::�C||iHexRole==FIELD_HEX_ROLE::��) ) {
					iCanTaihouAndTekkousenDistance += (getBushouAptitude(iBushouID, APTITUDE_SUIGUN)+1)/2; // �K���ɉ����ĉ��Z����B
				}

				// �^���^���X���C��œS�b�D��ԂȂ�΁A���������l��ύX����B
				if ( isLancelotTartare(iBushouID) && (nb6bushouref[iBushouID].steelship && (iHexRole==FIELD_HEX_ROLE::�C||iHexRole==FIELD_HEX_ROLE::��) ) ) {
					iCanTaihouAndTekkousenDistance = 2 + getBushouAptitude(iBushouID, APTITUDE_SUIGUN)*2 + getBushouAptitude(iBushouID, APTITUDE_TEPPOU)*2;
				}

				// ��ɂ��C��œS�b�D��ԂȂ�΁A���������l��ύX����B
				if (isShuiHidechiyo(iBushouID) && (nb6bushouref[iBushouID].steelship && (iHexRole == FIELD_HEX_ROLE::�C || iHexRole == FIELD_HEX_ROLE::��))) {
					iCanTaihouAndTekkousenDistance = 2 + getBushouAptitude(iBushouID, APTITUDE_SUIGUN) * 2 + getBushouAptitude(iBushouID, APTITUDE_TEPPOU) * 2;
				}

			}

			// �U���̏ꍇ
		} else if ( Is_CastleWar() ) {

			// �Ώە�����(��ɋ����)�ʒu�𓾂�B
			HEX_POSITION p = GetCastleHexUnitPos( iBushouID );
			// �����̈ʒu�̖����𓾂�
			int iHexRole = GetCastleHexRole( p );

			if ( nb6bushouref[iBushouID].biggun ) { // ��C�������Ă��邩�A�������́A(���S�b�D=�S�|�D�������Ă��āA�C�̏�)�Ȃ�΁c

				// �z��ɃA�N�Z�X�ł������ł���΁c
				if ( iTeppouAptitude < sizeof(TaihouDistanceArray)/sizeof(TaihouDistanceArray[0]) ) {

					if ( nb6bushouref[iBushouID].form == 0 && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3) { // ���y�ŋ|���a�ȏ�ł���΁c
						// �S�C�K������A��C�����}�X�ڂ��擾
						iCanTaihouAndTekkousenDistance = TaihouDistanceArray[iTeppouAptitude]; // 

						if ( iHexRole == CASTLE_HEX_ROLE::�E || iHexRole == CASTLE_HEX_ROLE::�{�� ) {
							iCanTaihouAndTekkousenDistance++;
						}
					}

					// �|�R�n�̎�
					else if ( nb6bushouref[iBushouID].form == 1 && nb6bushouref[iBushouID].aptitudekib >= 3 && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3) {
						// �S�C�K������A��C�����}�X�ڂ��擾
						iCanTaihouAndTekkousenDistance = TaihouDistanceArray[iTeppouAptitude]; // 

						if ( iHexRole == CASTLE_HEX_ROLE::�E || iHexRole == CASTLE_HEX_ROLE::�{�� ) {
							iCanTaihouAndTekkousenDistance++;
						}
					}
				}
			}
		}

		if ( iCanTaihouAndTekkousenDistance < 9 ) {
			iCanTaihouAndTekkousenDistance++; // �C���N�������g ����TENSHOU.EXE�Ɠ������Ƃ�����Ă����B
		}

	}
}

/*
��C�������Ă�����A�˒����P���₷�Ƃ����b�Ȃ̂Łc
0042B449  |> 837D F0 02     CMP     DWORD PTR SS:[EBP-10], 2	����C�������Ă���
0042B44D  |. 7C 1A          JL      SHORT TENSHOU.0042B469
0042B44F  |. 43             INC     EBX							�������Ă���Ύ˒����P���₷�B�����ŋ|���Ȃ�{���̎˒���EBX�ɏ㏑������Ηǂ��B
							�������� JMP TSMod.OnTSExeSetCanTaihouAndTekkousenDistance �Ə��������Ă����ۂ̏���
0042B450  |. 3B5D F0        CMP     EBX, DWORD PTR SS:[EBP-10]
0042B453  |. 72 14          JB      SHORT TENSHOU.0042B469		
0042B455  |. 57             PUSH    EDI
*/
int pTenshouExeJumpFromToOnTenshouExeSetCanTaihouAndTekkousenDistance		=0x42B44F; // �֐��͂��̃A�h���X����AOnTenshouExePreviousBattleMenuInBattle�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeSetCanTaihouAndTekkousenDistance   =0x42B4B8; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLbl1FromOnTenshouExeSetCanTaihouAndTekkousenDistance	=0x42B469; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
int pTenshouExeReturnLbl2FromOnTenshouExeSetCanTaihouAndTekkousenDistance	=0x42B455; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeSetCanTaihouAndTekkousenDistance() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		
		mov iCanTaihouAndTekkousenDistance, ebx // �����EBX���R�s�[

		mov ebx, DWORD PTR SS:[EBP-0x10] // ��r�Ώۂ�ebx�ɃR�s�[

		mov iCanTaihouAndTekkousenDistanceCompareVariable, ebx // ������R�s�[

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	OnTenshouExeSetCanTaihouAndTekkousenDistanceExecute();

	if ( iCanTaihouAndTekkousenDistance < iCanTaihouAndTekkousenDistanceCompareVariable ) { // CMP   EBX, DWORD PTR SS:[EBP-0x10]

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

			mov ebx, iCanTaihouAndTekkousenDistance // EBX�ɏ㏑��

			jmp pTenshouExeReturnLbl1FromOnTenshouExeSetCanTaihouAndTekkousenDistance
		}

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

			mov ebx, iCanTaihouAndTekkousenDistance // EBX�ɏ㏑��

			jmp pTenshouExeReturnLbl2FromOnTenshouExeSetCanTaihouAndTekkousenDistance;
		}
	}
}





char cmdOnTenshouExeJumpFromSetCanTaihouAndTekkousenDistance[7]="\xE9\x90\x90\x90\x90\x90";
// ���̖��߂��U�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�U�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeSetCanTaihouAndTekkousenDistance() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeSetCanTaihouAndTekkousenDistance;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSetCanTaihouAndTekkousenDistance + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeSetCanTaihouAndTekkousenDistance  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromSetCanTaihouAndTekkousenDistance+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSetCanTaihouAndTekkousenDistance), cmdOnTenshouExeJumpFromSetCanTaihouAndTekkousenDistance, 6, NULL); //6�o�C�g�̂ݏ�������
}


