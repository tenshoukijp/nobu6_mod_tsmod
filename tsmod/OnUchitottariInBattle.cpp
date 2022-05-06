#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "ScenarioMod.h"
#include "SoundServer.h"


/*
004316CA  |. 66:8B75 08     MOV     SI, WORD PTR SS:[EBP+8]
004316CE  |. 57             PUSH    EDI                              ; /Arg3
004316CF  |. 56             PUSH    ESI                              ; |Arg2
004316D0  |. 68 62100000    PUSH    1062                             ; |Arg1 = 00001062
004316D5  |. E8 A5AD0300    CALL    TENSHOU.0046C47F                 ; \TENSHOU.0046C47F
004316DA  |. 83C4 0C        ADD     ESP, 0C
004316DD  |. 50             PUSH    EAX
004316DE  |. 56             PUSH    ESI
004316DF  |. E8 638F0600    CALL    TENSHOU.0049A647
004316E4  |. 83C4 08        ADD     ESP, 8
004316E7  |. 56             PUSH    ESI                              ; /Arg3
004316E8  |. 57             PUSH    EDI                              ; |Arg2
004316E9  |. 68 68100000    PUSH    1068                             ; |Arg1 = 00001068
004316EE  |. E8 8CAD0300    CALL    TENSHOU.0046C47F                 ; \TENSHOU.0046C47F
004316F3  |. 83C4 0C        ADD     ESP, 0C
004316F6  |. 50             PUSH    EAX
004316F7  |. 57             PUSH    EDI
004316F8  |. E8 4A8F0600    CALL    TENSHOU.0049A647
004316FD  |. 83C4 08        ADD     ESP, 8


00431747  |. E8 068CFDFF    CALL    TENSHOU.0040A352
0043174C  |. 83C4 08        ADD     ESP, 8
0043174F  |. 5F             POP     EDI
00431750  |. 5E             POP     ESI
*/



/*
 */

int iUchitottariNoudouInBattleBushouID = -1; // (�����������)�̕���ID
int iUchitottariJyudouInBattleBushouID = -1; // (�������ꂽ��)�̕���ID

void OnTenshouExeUchitottariInBattleExecute() { 
	int iBushouID = iUchitottariNoudouInBattleBushouID-1; 
	int iTargetID = iUchitottariJyudouInBattleBushouID-1; 

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM && 0 <= iTargetID && iTargetID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// position 	(�B��=0, �喼=1, �h�V=2, �ƘV=3, ����=4, ���叫=5, ���y��=6)
		// State		0�喼,1�R��,2����,3�B��,4�Q�l,5�P,6?,7���S
		// ���肪�A�喼�A�h�V�A�ƘV�A�������́A�R�c��
		// ���邢�͈ꗬ�̐�ꕐ��
		if ( nb6bushouname[iTargetID].position <= 3 || nb6bushouname[iTargetID].State == 0 || nb6bushouname[iTargetID].State == 1 ||
			( (nb6bushouref[iTargetID-1].maxbat + nb6bushouref[iTargetID-1].maxint) >= 160 )
		 ) {

			// �����͂����̖ʎq�����߂āA���̑S�Ă̈ʒu�̒��S�_�����B
			// ����ɂ͂��ꂽ���̕����̈ʒu���W�߂�
			// ���̒��S�_����A���ۂ�iBushouID�̕����ւ̕����x�N�g���𓾂�
			// �R�`�T���قǂ̋����ւƎ��������ĉ���炷�B

			// ���ݐ��ɂ���ʎq
			vector<int> curMembers = GetCurMapSyutujinBushouArray();
			vector<int> posMembers;
			for ( vector<int>::iterator it = curMembers.begin(); it != curMembers.end(); it++ ) {
				// �����̖͂ʎq�݂̂̃��X�g�Ƃ���B
				if ( nb6bushouref[*it].attach == nb6bushouref[iBushouID].attach ) {
					posMembers.push_back(*it);
				}
			}
			// iBushouID���T�C�Y�������A����Ă������ƂŁA���ɉ��l���悤�Ƃ��A�Y���L�����̈ʒu�Ƀt�H�[�J�X��������B
			int size = posMembers.size();
			for ( int i=0; i<size; i++ ) {
				posMembers.push_back(iBushouID);
			}

			// ���
			if ( Is_FieldWar() ) {
				//
				HEX_POSITION my2DPos = GetFieldHexUnitPos(iBushouID); // �����̈ʒu
				D3D_POSITION my3DPos = FieldHexPos_To_WorldPos(my2DPos); // �����̈ʒu

				D3D_POSITION cnt3DPos(0,0,0); // (posMembers)�S���̒��S�p
				
				// �S���̒��S�𑫂���
				for ( vector<int>::iterator it = posMembers.begin(); it != posMembers.end(); it++ ) {
					HEX_POSITION it2DPos = GetFieldHexUnitPos(*it);
					D3D_POSITION it3DPos = FieldHexPos_To_WorldPos(it2DPos);
					cnt3DPos.x += it3DPos.x;
					cnt3DPos.y += it3DPos.y;
					cnt3DPos.z += it3DPos.z;
				}
				// �l���Ŋ��邱�ƂŁA���S�_�ƂȂ�B
				if ( posMembers.size() > 0 ) {
					cnt3DPos.x /= posMembers.size();
					cnt3DPos.y /= posMembers.size();
					cnt3DPos.z /= posMembers.size();
				}

				D3D_VECTOR vDir = my3DPos-cnt3DPos; // ���S�������ւ̃x�N�g�������߂�B
				vDir /= 3;
				if ( vDir.length() >= 3 ) {
					vDir.normalize(); // �P�ʃx�N�g�������āA�����R�܂łɏk�߂�
					vDir *= 3;
				}

				// �R��ޗp�ӂ��Ă���̂Ń����_���ŁB�����m����1/2
				int irand = rand() % 6;
				if ( irand == 1 ) {
					// �u�G���A�ł��������v�̌��ʉ�����ԍĐ��B
					PlaySound3DResID( 751, CommonGlobalHandle::hResourceWavMod, false , vDir.x, vDir.y, vDir.z,  0, 0, 0 );
				} else if ( irand == 2 ) {
					// �u�G���A�ł��������v�̌��ʉ�����ԍĐ��B
					PlaySound3DResID( 751, CommonGlobalHandle::hResourceWavMod2, false , vDir.x, vDir.y, vDir.z,  0, 0, 0 );
				} else if ( irand == 3 ) {
					// �u�G���A�ł��������v�̌��ʉ�����ԍĐ��B
					PlaySound3DResID( 751, CommonGlobalHandle::hResourceWavMod3, false , vDir.x, vDir.y, vDir.z,  0, 0, 0 );
				}

			// �U���(���S�ɓ����A���S���Y���B�ꏊ��Castle�p�֐��Ȃ����B)
			} else if ( Is_CastleWar() ) {
				//
				HEX_POSITION my2DPos = GetCastleHexUnitPos(iBushouID); // �����̈ʒu
				D3D_POSITION my3DPos = CastleHexPos_To_WorldPos(my2DPos); // �����̈ʒu

				D3D_POSITION cnt3DPos(0,0,0); // (posMembers)�S���̒��S�p
				
				// �S���̒��S�𑫂���
				for ( vector<int>::iterator it = posMembers.begin(); it != posMembers.end(); it++ ) {
					HEX_POSITION it2DPos = GetCastleHexUnitPos(*it);
					D3D_POSITION it3DPos = CastleHexPos_To_WorldPos(it2DPos);
					cnt3DPos.x += it3DPos.x;
					cnt3DPos.y += it3DPos.y;
					cnt3DPos.z += it3DPos.z;
				}

				if ( posMembers.size() > 0 ) {
					// �l���Ŋ��邱�ƂŁA���S�_�ƂȂ�B
					cnt3DPos.x /= posMembers.size();
					cnt3DPos.y /= posMembers.size();
					cnt3DPos.z /= posMembers.size();
				}
				D3D_VECTOR vDir = my3DPos-cnt3DPos; // ���S�������ւ̃x�N�g�������߂�B
				vDir /= 3;
				if ( vDir.length() >= 3 ) {
					vDir.normalize(); // �P�ʃx�N�g�������āA�����R�܂łɏk�߂�
					vDir *= 3;
				}

				// �R��ޗp�ӂ��Ă���̂Ń����_���ŁB�����m����1/2
				int irand = rand() % 6;
				if ( irand == 1 ) {
					// �u�G���A�ł��������v�̌��ʉ�����ԍĐ��B
					PlaySound3DResID( 751, CommonGlobalHandle::hResourceWavMod, false , vDir.x, vDir.y, vDir.z,  0, 0, 0 );
				} else if ( irand == 2 ) {
					// �u�G���A�ł��������v�̌��ʉ�����ԍĐ��B
					PlaySound3DResID( 751, CommonGlobalHandle::hResourceWavMod2, false , vDir.x, vDir.y, vDir.z,  0, 0, 0 );
				} else if ( irand == 3 ) {
					// �u�G���A�ł��������v�̌��ʉ�����ԍĐ��B
					PlaySound3DResID( 751, CommonGlobalHandle::hResourceWavMod3, false , vDir.x, vDir.y, vDir.z,  0, 0, 0 );
				}


			}
		} 
	}

} 
/* 
004316E7  |. 56             PUSH    ESI                              ; /Arg3	�� �U�����ꂽ��(�ł����ꂽ��)
004316E8  |. 57             PUSH    EDI                              ; |Arg2	�� �U��������
004316E9  |. 68 68100000    PUSH    1068                             ; |Arg1 = 00001068
004316EE  |. E8 8CAD0300    CALL    TENSHOU.0046C47F                 ; \TENSHOU.0046C47F
004316F3  |. 83C4 0C        ADD     ESP, 0C

004316F6  |. 50             PUSH    EAX
004316F7  |. 57             PUSH    EDI
004316F8  |. E8 4A8F0600    CALL    TENSHOU.0049A647
004316FD  |. 83C4 08        ADD     ESP, 8

*/ 
int pTenshouExeJumpFromToOnTenshouExeUchitottariInBattle    =0x4316F8; 
int pTenshouExeJumpCallFromToOnTenshouExeUchitottariInBattle=0x49A647; 
int pTenshouExeReturnLblFromOnTenshouExeUchitottariInBattle =0x4316FD; 
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeUchitottariInBattle() { 
	__asm { 
		mov iUchitottariNoudouInBattleBushouID, edi
		mov iUchitottariJyudouInBattleBushouID, esi

		// TENSHOU.EXE����call
		call pTenshouExeJumpCallFromToOnTenshouExeUchitottariInBattle 

		push eax 
		push ebx 
		push ecx 
		push edx 
		push esp 
		push ebp 
		push esi 
		push edi 
	} 

	OnTenshouExeUchitottariInBattleExecute(); 

	__asm { 
		pop edi 
		pop esi 
		pop ebp 
		pop esp 
		pop edx 
		pop ecx 
		pop ebx 
		pop eax 

		jmp pTenshouExeReturnLblFromOnTenshouExeUchitottariInBattle 
	} 
} 
char cmdOnTenshouExeJumpFromUchitottariInBattle[6]="\xE9"; 
void WriteAsmJumperOnTenshouExeUchitottariInBattle() { 
	int iAddress = (int)OnTenshouExeUchitottariInBattle; 
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeUchitottariInBattle + 5 ) ; 
	memcpy(cmdOnTenshouExeJumpFromUchitottariInBattle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B 
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeUchitottariInBattle), cmdOnTenshouExeJumpFromUchitottariInBattle, 5, NULL); // �ꉞ���S�̂��� 
} 


