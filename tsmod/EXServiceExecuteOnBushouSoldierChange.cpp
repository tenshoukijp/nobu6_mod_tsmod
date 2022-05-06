#include "WinTarget.h"
#include <fstream>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"
#include "ScenarioMod.h"

using namespace std;

int iCurBushouSoldier = -1;	// ���݃t�H�[�J�X�ƂȂ��Ă��镐����(�ő�\)����
short * pCurBushouID = 0L;	// ���݃t�H�[�J�X�ƂȂ��Ă��镐��ID�ւ̃|�C���^
short iCurBushouID = -1;	// ���݃t�H�[�J�X�ƂȂ��Ă��镐��ID�ւ̃|�C���^




NB6SOLDIERMAX_CUSTOM nb6soldiermax_custom[GAMEDATASTRUCT_SOLDIERMAX_NUM] = {
	0,   // �B��
	100, // �喼
	100, // �h�V
	85,  // �ƘV
	70,  // ����
	60,  // ���叫
	50,  // ���y��
};


// ���̊e�����̍ő啺�m�����A�E�ҁE���ʁE��E�Ȃǂ��܂߂ăJ�X�^�������l��Ԃ��֐��B
// @param:
//		iTargetBushouId:		�^�[�Q�b�g������ID, 
//		iCurOrgBushouSoldierMax:�^�[�Q�b�g�����̌��X�̕��m�ő吔
int getCustomSoldierMax(int iTargetBushouID, int iTargetBushouOrgSoldierMax) {

	int iTargetBushouDstSoldierMax = iTargetBushouOrgSoldierMax;
	int iBackUpBushouDstSoldierMax = iTargetBushouDstSoldierMax;

	// �Ώۂ̕����Ɋ��ʂ��E������΁A�ő啺�m���ɉe������
	if (TSModCommand::AllBushou::cmdPositionInfluenceSoldierMax != 0) {

		// ���X�̕��͂�20��������ԂŎn�߂�
		iTargetBushouDstSoldierMax -= 20;
		iBackUpBushouDstSoldierMax = iTargetBushouDstSoldierMax;

		// �Y���̕��������ʂ����邩�ǂ�������������
		for ( int iKaniID=0; iKaniID<GAMEDATASTRUCT_KANI_NUM; iKaniID++) {

			// �Y���̕��������ʂ������Ă���΁A(���ʂ̒��g�̐��l�̓v���p�e�B�[�Ȃ̂ŁAiTargetBushouID+1)
			if (nb6kani[iKaniID].attach == iTargetBushouID+1 ) {
				// ���ʂ̏㏸�l�Ɠ����l�����A�ő啺�m����������
				iTargetBushouDstSoldierMax += nb6kani[iKaniID].param;
				break;
			}
		}
 
		// �Y���̑喼�ɖ�E�����邩�ǂ�������������
		for ( int iDaimyoID=0; iDaimyoID<GAMEDATASTRUCT_DAIMYO_NUM; iDaimyoID++) {

			// �Y���̌R�c(��)����E�������Ă���Ȃ��
			if ( nb6daimyo[iDaimyoID].position ) {

				// �Y���̕��������ʂ������Ă���΁A(���ʂ̒��g�̐��l�̓v���p�e�B�[�Ȃ̂ŁAiTargetBushouID+1)
				if (nb6daimyo[iDaimyoID].attach == iTargetBushouID+1 ) {
					// ��E��ID��11����������������A�ő啺�m����������
					// �喼ID���琳�K�����ꂽ��EID�𓾂�

					iTargetBushouDstSoldierMax += (11 - getNormalizedDaimyoPosition(iDaimyoID) );
					break;
				}
			}
		}

		// �Y���̕����ɖ�E�����邩�ǂ�������������
		for ( int iGundanID=0; iGundanID<GAMEDATASTRUCT_GUNDAN_NUM; iGundanID++) {

			// �Y���̌R�c�����݂���Ȃ��
			if ( nb6gundan[iGundanID].residence != 0xFFFF ) {

				// �Y���̕��������ʂ������Ă���΁A(���ʂ̒��g�̐��l�̓v���p�e�B�[�Ȃ̂ŁAiTargetBushouID+1)
				if (nb6gundan[iGundanID].leader == iTargetBushouID+1 ) {

					// �L���Ȋ��ʂ������Ă���΁c
					if ( int(nb6gundan[iGundanID].position) > 0 ) {
						// ��E��ID��11����������������A�ő啺�m����������
						iTargetBushouDstSoldierMax += (11 - getNormalizedGundanPosition(iGundanID) );
						break;
					}
				}
			}
		}

	}

	// �Ώۂ̕������E�҂ł���΁c ���̍��ڂ͊����ŉ�����̂ōŌ�ɍs�����ƁB
	if (nb6bushouref[iTargetBushouID].job == 0x10) { // 0x10==�E�� GameDataStruct.h������

		// �E�҂͉e������̂ł���ׂ��̃R�}���h���L����Lv==1����ꍇ�c
		if (TSModCommand::AllBushou::cmdNinjaMustBeAShadow == 1 ) {
			iTargetBushouDstSoldierMax = iTargetBushouDstSoldierMax / 2;  // ���͂͌�����1/2�Ƃ���B

		// �E�҂͉e������̂ł���ׂ��̃R�}���h���L����Lv==2����ꍇ�c
		} else if (TSModCommand::AllBushou::cmdNinjaMustBeAShadow == 2 ) {
			iTargetBushouDstSoldierMax = iTargetBushouDstSoldierMax / 3;  // ���͂͌�����1/3�Ƃ���B

		// �E�҂͉e������̂ł���ׂ��̃R�}���h���L����Lv==3����ꍇ�c
		} else if (TSModCommand::AllBushou::cmdNinjaMustBeAShadow == 3 ) {
			iTargetBushouDstSoldierMax = iTargetBushouDstSoldierMax / 5;  // ���͂͌�����1/5�Ƃ���B

			// �E�҂͉e������̂ł���ׂ��̃R�}���h���L����Lv==4����ꍇ�c
		} else if (TSModCommand::AllBushou::cmdNinjaMustBeAShadow == 4 ) {
			iTargetBushouDstSoldierMax = iTargetBushouDstSoldierMax / 10;  // ���͂͌�����1/10�Ƃ���B
		}
	}

	// �����܂�100��MAX�Ƃ���B�S�̂Ƃ���10�������Ă���̂ł���ł��܂��܂��Ӗ����o��B
	if (iTargetBushouDstSoldierMax > int(nb6parammax.soldier)) {
		iTargetBushouDstSoldierMax = int(nb6parammax.soldier);
	}

	// ScenarioMod�ɂ��ďC��
	if (p_snOnRequestSoldierMax) {

		// ScenarioMod�ɍďC����₢���킹
		int ret = p_snOnRequestSoldierMax(iTargetBushouID, iBackUpBushouDstSoldierMax, iTargetBushouDstSoldierMax);
		if (ret != -1) {
			iTargetBushouDstSoldierMax = ret;
		}
	}

	return iTargetBushouDstSoldierMax;

}

// �O���Q�Ɨp
int WINAPI Extern_getCustomSoldierMax(int iBushouID) {

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		//�@���݂̐g���𓾂�
		int position = nb6bushouname[iBushouID].position;

		//�@�g������ő啺���𓾂�
		int soldiermax = nb6soldiermax[position].soldiermax;

		// �ő啺�m���́A�E�҂������芯�ʁE��E���ɂ���ăt�B���^�[��������
		soldiermax = getCustomSoldierMax( iBushouID, soldiermax );

		return soldiermax;
	}

	return 0;
}



// (�\�ő�)���m���ɑ΂��āA����ȃt�B���^�[��(���̓s�x)�ݒ肷��B
void checkEXFilterOfSoldierMax() {
	iCurBushouID =  *(pCurBushouID+4);
	iCurBushouID = iCurBushouID - 1;  // ���ۂ̔ԍ���1�������l(�����ɂ́A�z��Ŏg�p����̂�-1�̒l�A�e��v���p�e�B�ɓ����Ă���l�͂��̂܂܂̒l)

	// �����Ńt�B���^�[��������
	iCurBushouSoldier = getCustomSoldierMax(iCurBushouID, iCurBushouSoldier);
}

/*
0040BFD8  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]			�����̎���ESP�̐�ɕ�����INDEX(�^���^���X�Ȃ�189=BD)�������Ă���B
0040BFDC  |. E8 F7E3FFFF    CALL    TENSHOU.0040A3D8
				�������� JMP TSMod.OnTSExeBushouSoldierMax �Ƃ���̂��ړI�@�����āA���X���̍s�ɂ����� ���e�͊֐����ŏ���
0040BFE1  |. 0FB70445 A0B74>MOVZX   EAX, WORD PTR DS:[EAX*2+4BB7A0]	��������EAX���ő啺�m��
0040BFE9  |. 83C4 04        ADD     ESP, 4
0040BFEC  \. C3             RETN
*/

int pTenshouExeJumpFromToOnTenshouExeBushouSoldierMax    =0x40BFDC; // �֐��͂��̃A�h���X����AOnTenshouExeBushouSoldierMax�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeBushouSoldierMax =0x40BFE9; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
int pTenshouExeCallLblFromOnTenshouExeBushouSoldierMax   =0x40A3D8; // TENSHOU.EXE���ɂ��颐g������畺���𓾂�֐�
// ���̊֐���TENSHOU.EXE�������̕��m����ύX���悤�Ƃ���^�C�~���O�ŁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeBushouSoldierMax() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// esp�̓|�C���^�B�w���͕̂���ID(�����ɂ͕���ID�ɂP���������̖̂͗l)
		mov pCurBushouID, esp

		// tenshou.exe�Ō����炠��������
		call pTenshouExeCallLblFromOnTenshouExeBushouSoldierMax
		movzx eax, word ptr ds:[eax*2+4BB7A0h]

		// eax���ő�\�����ƂȂ�̂ŁA���̐��l����[�ۑ�����
		mov iCurBushouSoldier, eax

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// �������s��
	checkEXFilterOfSoldierMax();

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

		mov eax, iCurBushouSoldier

		jmp pTenshouExeReturnLblFromOnTenshouExeBushouSoldierMax
	}
}

/*
0040BFD8  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]     �����̎���ESP�̐�ɕ�����INDEX(�^���^���X�Ȃ�189=BD)�������Ă���B
0040BFDC  |. E8 F7E3FFFF    CALL    TENSHOU.0040A3D8
				�������� JMP TSMod.OnTSExeBushouSoldierMax �Ƃ���̂��ړI�@�����āA���X���̍s�ɂ����� ���e�͊֐����ŏ���
0040BFE1  |. 0FB70445 A0B74>MOVZX   EAX, WORD PTR DS:[EAX*2+4BB7A0]	��������EAX���ő啺�m��
0040BFE9  |. 83C4 04        ADD     ESP, 4
0040BFEC  \. C3             RETN
*/
char cmdOnTenshouExeJumpFromBushouSoldierMax[6]="\xE9";
// ���̖��߂�6�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂ŁANOP���Ō�ɕt��������Ă���B

typedef void (WINAPI *PFNONTENSHOUEXEBUSHOUSOLDIERMAX)();
PFNONTENSHOUEXEBUSHOUSOLDIERMAX iOnTenshouExeBushouSoldierMax;

// ���m�����ύX����悤�Ƃ������ɁA�Ăяo�����
void WriteAsmJumperOnTenshouExeBushouSoldierMax() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeBushouSoldierMax;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeBushouSoldierMax + 5 );
		// �T�Ƃ����̂́A0040BFDC  -E9 ????????  JMP TSMod.OnTSExeBushouSoldierMax  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromBushouSoldierMax+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeBushouSoldierMax), cmdOnTenshouExeJumpFromBushouSoldierMax, 5, NULL); // 5�o�C�g�̂ݏ������� ���߂�5�o�C�g�B
}

