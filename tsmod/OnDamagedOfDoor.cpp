#include "WinTarget.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


extern int isCurBushouHasATouken(int iBushouID);

// �G�߂��ς��x�ɂS�����ȉ��̌��ƕ�ɂ��āA�����������킹�Ă��Ȃ����������ɁA�����Ⴂ�m���œn��悤�ɂ���B
void EXServieExecute_NPCKengouBushouHasAKahouSword() {

	// ���[�h����͔������Ȃ�
	if ( nb6progressseason <= 1 ) { return; }

	// ��������ʂ�Ȃ߂�B����������T��
	for ( int iBushouID = 0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

		// ���������łȂ���Ζ����B���̐l
		if ( nb6bushouref[iBushouID].job != 0x30 ) { continue; }

		// ���S���Ă���Ζ����B���̐l
		if ( nb6bushouname[iBushouID].State == 7 ) { continue; }

		// �Y�������̐퓬�\�͂���������������������A���P�����B�������Ȃ��B
		if ( nb6bushouref[iBushouID].maxbat > 100 ) { continue; }

		// �Y�������������������Ă�����A�����B���̐l
		if ( isCurBushouHasATouken( iBushouID ) ) { continue; }

		// �v���C���[�z���̕����ł���΁A����
		if ( isPlayerBushou( iBushouID ) ) { continue; }

		// 1�l�̌���������A1/30�������Ă�m���͂Ȃ�
		if ( rand() % 30 != 0 ) { continue; }

		// �ƕ����ʂ蒲�ׂ�
		for (int iKahouID=0; iKahouID<GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {

			int iTargetKahouRank = rand() % 7; // (0�`6) 
			iTargetKahouRank += 1; // (�㏸��1�`7 = 10�����`�S����)

			// �Ώۂ̉ƕ󂪁u�����v�n�ł���B ���L�҂����l�ł���(0xFFFF = 65535)�B�����ĉƕ�̏㏸�l���AiTargetKahouRank�ƈ�v����B
			if (nb6kahou[iKahouID].type == 5 && nb6kahou[iKahouID].attach == 0xFFFF && nb6kahou[iKahouID].param == iTargetKahouRank ) {

				// ���̉ƕ���Y�������Ɏ�������
				nb6kahou[iKahouID].attach = iBushouID + 1;

				// �퓬�\�͂��Y�����l���A�b�v����B
				nb6bushouref[iBushouID].maxbat += nb6kahou[iKahouID].param;

				// 1�l�ň�C�ɉ��������Ȃ��悤��break����K�v����!!
				break;
				
			}
		}
	}

}



// return �����̏㏸�l(1�`10)
int isCurBushouHasATouken(int iBushouID) {
	int iToukenMaxRank = 0;

	// �ƕ����ʂ蒲�ׂ�
	for (int iKahouID=0; iKahouID<GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {

		// ���L�҂̕���ID����v����ꍇ�̂ݔ���̑Ώ�
		if (nb6kahou[iKahouID].attach-1 == iBushouID) {

			// �Ώۂ̉ƕ󂪁u�����v�n�ł���B
			if ( nb6kahou[iKahouID].type == 5 ) {

				// ���Ɏ����Ă����������������N��������΁A���ꂪ�{�l��������������ƕ�㏸�l
				if ( iToukenMaxRank < nb6kahou[iKahouID].param ) {
					iToukenMaxRank = nb6kahou[iKahouID].param;
				}
			}
		}
	}
	// �����N��Ԃ��B�ŏ�0(�������Ă��Ȃ�)�A�ő�10
	return iToukenMaxRank;
}

int iCurTypeDamegeOfDoor = -1;				// �h�A�̃^�C�v
int iDamageOfDoorExecute = -1;
int iCurBushouIDInDamageOfDoor = -1;		// �h�A�̏ꍇ��ebp���畐��ID���A��̏ꍇ��ebx���畐��ID���R�s�[�����B
int iCurBushouIDInDamageOfCastle = -1;		// �h�A�̏ꍇ��ebp���畐��ID���A��̏ꍇ��ebx���畐��ID���R�s�[�����B

void OnTenshouExeDamagedOfDoorExecute() {
	// ��̏ꍇ(edi==0xffff) NPC�̏ꍇ��(edi=ebx�̉��Sbit)
	if ( (iCurTypeDamegeOfDoor == 0xFFFF) || (iCurTypeDamegeOfDoor == (iCurBushouIDInDamageOfCastle & 0xFFFF) ) ) {
		// ���ʂS�o�C�g������ID�Ȃ̂ŁA�}�X�N
		iCurBushouIDInDamageOfDoor = iCurBushouIDInDamageOfCastle & 0xFFFF;
	} else {
		// ���ʂS�o�C�g������ID�Ȃ̂ŁA�}�X�N
		iCurBushouIDInDamageOfDoor = iCurBushouIDInDamageOfDoor & 0xFFFF;
	}
	// ���������͈̔͂ł��邱��
	if (0 <= iCurBushouIDInDamageOfDoor-1 && iCurBushouIDInDamageOfDoor-1 < GAMEDATASTRUCT_BUSHOU_NUM) {
		
		// �����łȂ��A�R�m�ł��Ȃ���΁A���̐l�ɔ��ւ̓���ȃ_���[�W�����͂��Ȃ�
		if ( nb6bushouref[iCurBushouIDInDamageOfDoor-1].job != 0x30 && nb6bushouref[iCurBushouIDInDamageOfDoor-1].job != 0xA0 ) { return; }

		byte iKahouParam = isCurBushouHasATouken(iCurBushouIDInDamageOfDoor-1);
		// �ƕ�������Ă���΁c
		if ( iKahouParam ) {
			iKahouParam += 2; // (�ŏ�3�`�ő�12��)
			// �h�A�̃_���[�W����Ƃ̏㏸�l�{
			iDamageOfDoorExecute = iDamageOfDoorExecute * iKahouParam;
			iDamageOfDoorExecute = iDamageOfDoorExecute / 2; //(�ŏ�1.5�{�`�ő�6.0�{)
			// �ő�l��100
			if (iDamageOfDoorExecute > 100) { iDamageOfDoorExecute = 100; }
		}
	}
}


/*
00432F14  |. 8D4406 01      LEA     EAX, DWORD PTR DS:[ESI+EAX+1]  EAX=2B
00432F18  |. 5E             POP     ESI
				�����̂Q�s�� JMP TSMod.OnTSExeDamagedOfDoor �Ə��������Ă����ۂ̏���
00432F19  \. C3             RETN
*/
int pTenshouExeJumpFromToOnTenshouExeDamagedOfDoor		=0x432F14; // �֐��͂��̃A�h���X����AOnTenshouExeDamagedOfDoor�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeDamagedOfDoor	=0x432F19; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeDamagedOfDoor() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		mov iCurTypeDamegeOfDoor, edi		// edi��0xFFFF�Ȃ��_���[�W 1�Ȃ�h�A�_���[�W�B�m�o�b�̏ꍇ�́Aedi=ebx�̉��Sbit�̏󋵂���̖͗l�B

		lea eax, DWORD PTR DS:[esi+eax+1]

		mov iCurBushouIDInDamageOfDoor, ebp			// �h�A�̏ꍇ�ɕ���ID���R�s�[�������Aebp�̉���4�o�C�g������ID�Ȃ̂ŁA�}�X�N����K�v����B
		mov iCurBushouIDInDamageOfCastle, ebx;		// ��̏ꍇ�ɕ���ID���R�s�[

		// �_���[�W��ۑ�
		mov iDamageOfDoorExecute, eax;

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
	OnTenshouExeDamagedOfDoorExecute();

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

		// �C�������_���[�W�l��������eax�ɋt�ɖ߂�
		mov eax, iDamageOfDoorExecute

		pop	esi

		jmp pTenshouExeReturnLblFromOnTenshouExeDamagedOfDoor
	}
}

/*
00432F14  |. 8D4406 01      LEA     EAX, DWORD PTR DS:[ESI+EAX+1]  EAX=2B
00432F18  |. 5E             POP     ESI
				�����̂Q�s�� JMP TSMod.OnTSExeDamagedOfDoor �Ə��������Ă����ۂ̏���
00432F19  \. C3             RETN
*/
char cmdOnTenshouExeJumpFromDamagedOfDoor[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeDamagedOfDoor() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeDamagedOfDoor;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeDamagedOfDoor + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeDamagedOfDoor  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromDamagedOfDoor+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeDamagedOfDoor), cmdOnTenshouExeJumpFromDamagedOfDoor, 5, NULL); //5�o�C�g�̂ݏ�������
}




