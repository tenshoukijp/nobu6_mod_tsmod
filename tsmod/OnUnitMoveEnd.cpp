#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"
#include "ScenarioMod.h"

/*
 �Ƃ��郆�j�b�g�̈ړ����I���������B�o�b���j�b�g�A�m�o�b���j�b�g����
 */


/*
00448B5F   E8 8D24FEFF      CALL    TENSHOU.0042AFF1		// ���j�b�g���ړ�����������ꏊ�́u�g�d�w�����v��EAX�ɓ���Ă���B
00448B64   83C4 04          ADD     ESP, 4
00448B67  |. 83F8 1B        CMP     EAX, 1B					// EAX����{�ۂ��ǂ����B
00448B6A  |. 75 2C          JNZ     SHORT TENSHOU.00448B98
00448B6C   56               PUSH    ESI

*/

extern int iCheckRojoBushouNum; // �ď鑤�̏���ɋ���l��
extern int iNextTurnCastleID;   // �\���ď�풆�̏�ԍ�(+1�̕�)

int isUnitMoveEndRakujoCondition = 0;	// �V���[�g�J�b�g��������𖞂����B
int iUnitMoveEndBushou = -1;
int iUnitMoveEndNum = -1;
void OnTenshouExeUnitMoveEndExecute() {
	isUnitMoveEndRakujoCondition = 0; // ���x�������Ȃ��ŏ㏑������B
	
	if ( 0 <= iUnitMoveEndBushou-1 && iUnitMoveEndBushou-1 < GAMEDATASTRUCT_BUSHOU_NUM ) {
		iUnitMoveEndBushou = iUnitMoveEndBushou - 1; // iBushouID (-1���������z��̓Y�����p�̔ԍ��Ƃ��Ă���)
	}


	// �V���[�g�J�b�g����̒l���L���ł���B
	if ( TSModCommand::AllCastle::cmdShortCutFallCastle ) {
		// �U��풆�ł���B(�����āA�ď鑤�̐l���͂O�ł���B�� ���Ȃ����ƂƂ����B)
		if ( Is_CastleWar() /* && iCheckRojoBushouNum==0*/ ) {
			/*
			�o�w���Ă���A�S�Ă̕����𓾂�֐�
			GetSyutujinBushouArray() �̃��X�g��
			�ď钆�̏�ɏ������Ă��镐����������ƁA���X�o�w���Ă��Ȃ������̃��X�g���o��B
			*/
			// �U�߂��Ă����
			int iCastleID = iNextTurnCastleID-1;
			int iJokakuValue = 50;
			if ( 0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM ) {
				iJokakuValue = nb6castle[iCastleID].build;
			}

			// �U�߂��Ă����ɏ������Ă��āA�o�w���Ă��Ȃ��҂̕����ꗗ
			vector<int> vCatstleBushouArray;
			for ( int iBushouID = 0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {
				// �Y���̏�ɏ�������Ȃ�΁c
				if ( nb6bushouref[iBushouID].castle-1 == iCastleID ) {
					// �}�b�v�S�̂ŏo�w���Ă��镐��
					vector<int> vSyutujinBushouArray = GetSyutujinBushouArray();

					bool isSyutujinZumi = false; // ��ɏ������Ă��邪���݂ǂ����ɏo�w���Ă��邩�H
					for ( vector<int>::iterator it = vSyutujinBushouArray.begin(); it != vSyutujinBushouArray.end(); it++ ) {
						if ( iBushouID == *it ) {
							isSyutujinZumi = true;
						}
					}
					// �o�w�ς݂łȂ��ꍇ����������
					if ( !isSyutujinZumi ) {
						vCatstleBushouArray.push_back(iBushouID);
					}
				}
			}

			// ��ɂ������Ă��镐���̓����T��B
			int iSumDefenceSoldier = 0; // �����̍��v
			for ( vector<int>::iterator it = vCatstleBushouArray.begin(); it != vCatstleBushouArray.end(); it++ ) {
				iSumDefenceSoldier += nb6bushouref[*it].soldier;
			}

	
			// ���݂����ď�}�b�v�Ɏ��ۂɃ��j�b�g�o�w���Ă��镐�����X�g
			vector<int> vCurMapSyutujinBushouArray = GetCurMapSyutujinBushouArray();

			int iShozokuDaimyo = -1;
			int iSumAttackSoldier = 0; // �����̍��v
			for ( vector<int>::iterator it = vCurMapSyutujinBushouArray.begin(); it != vCurMapSyutujinBushouArray.end(); it++ ) {
				int iBushouID = *it;

				if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
					// 1���ځB�ŏ��̐l�ŏ����喼�����߂�B
					if (iShozokuDaimyo == -1) {
						iShozokuDaimyo = nb6bushouref[iBushouID].attach;
					}
					// ���̐l�S�����ŏ��̐l�̑喼�Ɠ����ł���΁A���邵���ƌ��Ȃ��B
					// ��v���Ȃ���΁A���邵���Ƃ͌��Ȃ��Ȃ��B�����喼��-2����Ă����B
					else if (iShozokuDaimyo != nb6bushouref[iBushouID].attach ) {
						iShozokuDaimyo = -2;
						break;
					}
					// �����̕����̍��v�����Z���Ă���
					iSumAttackSoldier += nb6bushouref[iBushouID].soldier;
				}
			}

			// �����喼���O�ȏ�̒l�Ƃ��Ďc���Ă���΁A���ݕ\�������ď��}�b�v�ɋ���ʎq�������喼�����Ƃ������ƁB
			// ����āA���邵���ƌ��Ȃ��B
			// (�U�����̕����͂��̂R�{�ȏ�͂���B�������͓G���ď�҂O) ���A�U�����͏�s�l��1/10�͍Œ�ł����͂�L���Ă��邱�ƁB
			if ( iShozokuDaimyo >= 0 && ((iSumDefenceSoldier*3 < iSumAttackSoldier) || iCheckRojoBushouNum==0 ) && (iSumAttackSoldier*10 > iJokakuValue) ) {

				int iBushouID = iUnitMoveEndBushou;
				HEX_POSITION my2DPos = GetCastleHexUnitPos(iBushouID); // �����̈ʒu
				int iRole = GetCastleHexRole(my2DPos); // ���̕����̖���
				D3D_POSITION my3DPos = CastleHexPos_To_WorldPos(my2DPos); // �����̈ʒu(3D��)

				// ����ɓ����Ă����(����m��͍������Q�ȏ�)�A�������́A�������P�ȏゾ���A���炩�ɏ���ɋ���B(�������P�������肾�ƁA���ɓ����������łł������P�ƂȂ��Ă��܂�����)
				if ( (my3DPos.y >= 2) || (my3DPos.y >= 1 && (iRole==CASTLE_HEX_ROLE::������n||iRole==CASTLE_HEX_ROLE::������n||iRole==CASTLE_HEX_ROLE::�E)) ) {
					isUnitMoveEndRakujoCondition = 1;
				}
			}
		}
	}

	if ( iUnitMoveEndNum == CASTLE_HEX_ROLE::�{�� ) { 
		isUnitMoveEndRakujoCondition = 1; // �������Ŗ������B
	}

	/*
	// ScenarioMod�̃J�X�^�������Ő푈::�ړ��㗎��ۂ�ݒ肷��
	if ( Is_CastleWar() ) {
		if ( p_snOnCustomCondition ) {
			int iDefenceCastleID = iNextTurnCastleID;	  // �Ώۂ̏�̔ԍ�(+1�̕�)
			int iMovedBushouID  = iUnitMoveEndBushou + 1; // �s�������������ԍ�(+1�̕�)

			// ��������̐���																							�����s��	     ���Ώۂ̏�	    �����ݗ\�肳��Ă��闎��̉ۂ̌��� 
			int isCastleFallDownResult = ((PFNSNONCUSTOMCONDITION) p_snOnCustomCondition)("AS����::�푈::�ړ��㗎���", iMovedBushouID+1, iDefenceCastleID+1, isUnitMoveEndRakujoCondition, "", "", 0.0, 0.0);
			if ( isCastleFallDownResult != CUSTOM_CONDITION_NOMODIFY ) {

				// ����������㏑������B
				isUnitMoveEndRakujoCondition = isCastleFallDownResult;
			}
		}
	}
	*/
} 
/* 
00448B5F   E8 8D24FEFF      CALL    TENSHOU.0042AFF1		// ���j�b�g���ړ�����������ꏊ�́u�g�d�w�����v��EAX�ɓ���Ă���B
00448B64   83C4 04          ADD     ESP, 4
00448B67  |. 83F8 1B        CMP     EAX, 1B					// EAX����{�ۂ��ǂ����B
00448B6A  |. 75 2C          JNZ     SHORT TENSHOU.00448B98
00448B6C   56               PUSH    ESI
*/ 
int pTenshouExeJumpFromToOnTenshouExeUnitMoveEnd    =0x448B5F; 
int pTenshouExeJumpCallFromToOnTenshouExeUnitMoveEnd=0x42AFF1; 
int pTenshouExeReturnLblFromOnTenshouExeUnitMoveEnd =0x448B6A; 
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeUnitMoveEnd() { 
	__asm { 

		// TENSHOU.EXE����call
		call pTenshouExeJumpCallFromToOnTenshouExeUnitMoveEnd

		ADD     ESP, 4

		mov iUnitMoveEndNum, eax  // �ړ����������ꏊ�̖����ԍ���ۑ����Ă����B
		mov iUnitMoveEndBushou, esi // �����h�c��ۑ����Ă����B

		push eax 
		push ebx 
		push ecx 
		push edx 
		push esp 
		push ebp 
		push esi 
		push edi 
	} 

	OnTenshouExeUnitMoveEndExecute(); 

	__asm { 
		pop edi 
		pop esi 
		pop ebp 
		pop esp 
		pop edx 
		pop ecx 
		pop ebx 
		pop eax 

		CMP     isUnitMoveEndRakujoCondition, 1			// ��r�������Ă����ăt���O���W�X�^�ɒl�i�[�B
		jmp pTenshouExeReturnLblFromOnTenshouExeUnitMoveEnd  // �g�킸�W�����v
	} 
} 
char cmdOnTenshouExeJumpFromUnitMoveEnd[6]="\xE9"; 
void WriteAsmJumperOnTenshouExeUnitMoveEnd() { 
	int iAddress = (int)OnTenshouExeUnitMoveEnd; 
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeUnitMoveEnd + 5 ) ; 
	memcpy(cmdOnTenshouExeJumpFromUnitMoveEnd+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B 
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeUnitMoveEnd), cmdOnTenshouExeJumpFromUnitMoveEnd, 5, NULL); // �ꉞ���S�̂��� 
} 


