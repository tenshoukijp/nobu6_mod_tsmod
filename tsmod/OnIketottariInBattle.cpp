#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "ScenarioMod.h"
#include "SoundServer.h"


/*
00432921  |. 85C0           TEST    EAX, EAX�@�@�@�@ �����ꂪ�O���Ƒދ����������͕ߔ��ƂȂ�B
00432923  |. 75 0C          JNZ     SHORT TENSHOU.00432931
00432925  |. 56             PUSH    ESI
00432926  |. 57             PUSH    EDI
00432927  |. E8 6CECFFFF    CALL    TENSHOU.00431598 �� �����Ƃ����� (���ɂ����̊֐��ւƗ����ӏ��͐���������)
0043292C  |. 83C4 08        ADD     ESP, 8
0043292F  |. EB 0F          JMP     SHORT TENSHOU.00432940
00432931  |> 837D F4 00     CMP     DWORD PTR SS:[EBP-C], 0
00432935  |. 74 09          JE      SHORT TENSHOU.00432940
*/
/*
�ދp���A����Ƃ��A�ߔ��������߂鋫�ڂ̊֐�
00431598  /$ 56             PUSH    ESI
00431599  |. 57             PUSH    EDI
0043159A  |. 66:8B7424 0C   MOV     SI, WORD PTR SS:[ESP+C]
0043159F  |. 68 00020000    PUSH    200
004315A4  |. 56             PUSH    ESI
004315A5  |. E8 A88DFDFF    CALL    TENSHOU.0040A352
004315AA  |. 66:8B7C24 18   MOV     DI, WORD PTR SS:[ESP+18]
004315AF  |. 83C4 08        ADD     ESP, 8
004315B2  |. 57             PUSH    EDI
004315B3  |. 56             PUSH    ESI
004315B4  |. E8 48F9FFFF    CALL    TENSHOU.00430F01
004315B9  |. 83C4 08        ADD     ESP, 8
004315BC  |. 85C0           TEST    EAX, EAX
004315BE  |. 75 52          JNZ     SHORT TENSHOU.00431612
004315C0  |. 57             PUSH    EDI
004315C1  |. 56             PUSH    ESI
004315C2  |. E8 9EFEFFFF    CALL    TENSHOU.00431465
004315C7  |. 83C4 08        ADD     ESP, 8
004315CA  |. 85C0           TEST    EAX, EAX
004315CC  |. 75 44          JNZ     SHORT TENSHOU.00431612
004315CE  |. 56             PUSH    ESI
004315CF  |. 57             PUSH    EDI
004315D0  |. E8 63510300    CALL    TENSHOU.00466738
004315D5  |. 83C4 08        ADD     ESP, 8
004315D8  |. 85C0           TEST    EAX, EAX
004315DA  |. 75 2B          JNZ     SHORT TENSHOU.00431607
004315DC  |. 6A 00          PUSH    0                                ; /Arg2 = 00000000
004315DE  |. 56             PUSH    ESI                              ; |Arg1
004315DF  |. E8 9F79FFFF    CALL    TENSHOU.00428F83                 ; \TENSHOU.00428F83
004315E4  |. 83C4 08        ADD     ESP, 8
004315E7  |. 66:3D FFFF     CMP     AX, 0FFFF
004315EB  |. 74 0E          JE      SHORT TENSHOU.004315FB
004315ED  |. 6A 00          PUSH    0                                ; /Arg3 = 00000000
004315EF  |. 57             PUSH    EDI                              ; |Arg2
004315F0  |. 56             PUSH    ESI                              ; |Arg1
004315F1  |. E8 7FF4FFFF    CALL    TENSHOU.00430A75                 ; \TENSHOU.00430A75	�� �����߂�m��
004315F6  |. 83C4 0C        ADD     ESP, 0C
004315F9  |. EB 17          JMP     SHORT TENSHOU.00431612
004315FB  |> 6A 02          PUSH    2                                ; /Arg3 = 00000002
004315FD  |. 56             PUSH    ESI                              ; |Arg2
004315FE  |. 57             PUSH    EDI                              ; |Arg1
004315FF  |. E8 ADF2FFFF    CALL    TENSHOU.004308B1                 ; \TENSHOU.004308B1
00431604  |. 83C4 0C        ADD     ESP, 0C
00431607  |> 6A 01          PUSH    1
00431609  |. 56             PUSH    ESI
0043160A  |. E8 F7F4FFFF    CALL    TENSHOU.00430B06
0043160F  |. 83C4 08        ADD     ESP, 8
00431612  |> 5F             POP     EDI
00431613  |. 5E             POP     ESI
00431614  \. C3             RETN

*/



/*
 */

int iIketottariNoudouInBattleBushouID = -1; // (�����������)�̕���ID
int iIketottariJyudouInBattleBushouID = -1; // (�������ꂽ��)�̕���ID

void OnTenshouExeIketottariInBattleExecute() { 
	int iBushouID = iIketottariNoudouInBattleBushouID-1; 
	int iTargetID = iIketottariJyudouInBattleBushouID-1; 

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
					// �u�G���A�����߂�����v�̌��ʉ�����ԍĐ��B
					PlaySound3DResID( 750, CommonGlobalHandle::hResourceWavMod, false , vDir.x, vDir.y, vDir.z,  0, 0, 0 );
				} else if ( irand == 2 ) {
					// �u�G���A�����߂�����v�̌��ʉ�����ԍĐ��B
					PlaySound3DResID( 750, CommonGlobalHandle::hResourceWavMod2, false , vDir.x, vDir.y, vDir.z,  0, 0, 0 );
				} else if ( irand == 3 ) {
					// �u�G���A�����߂�����v�̌��ʉ�����ԍĐ��B
					PlaySound3DResID( 750, CommonGlobalHandle::hResourceWavMod3, false , vDir.x, vDir.y, vDir.z,  0, 0, 0 );
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
					// �u�G���A�����߂�����v�̌��ʉ�����ԍĐ��B
					PlaySound3DResID( 750, CommonGlobalHandle::hResourceWavMod, false , vDir.x, vDir.y, vDir.z,  0, 0, 0 );
				} else if ( irand == 2 ) {
					// �u�G���A�����߂�����v�̌��ʉ�����ԍĐ��B
					PlaySound3DResID( 750, CommonGlobalHandle::hResourceWavMod2, false , vDir.x, vDir.y, vDir.z,  0, 0, 0 );
				} else if ( irand == 3 ) {
					// �u�G���A�����߂�����v�̌��ʉ�����ԍĐ��B
					PlaySound3DResID( 750, CommonGlobalHandle::hResourceWavMod3, false , vDir.x, vDir.y, vDir.z,  0, 0, 0 );
				}


			}
		} 
	}

} 
/* 
004315ED  |. 6A 00          PUSH    0                                ; /Arg3 = 00000000
004315EF  |. 57             PUSH    EDI                              ; |Arg2	�� EDI ���U��������(�����������)
004315F0  |. 56             PUSH    ESI                              ; |Arg1	�� ESI �����ꂽ��(�������ꂽ��)
004315F1  |. E8 7FF4FFFF    CALL    TENSHOU.00430A75                 ; \TENSHOU.00430A75	�� �����߂�m��
004315F6  |. 83C4 0C        ADD     ESP, 0C
*/ 
int pTenshouExeJumpFromToOnTenshouExeIketottariInBattle    =0x4315F1; 
int pTenshouExeJumpCallFromToOnTenshouExeIketottariInBattle=0x430A75; 
int pTenshouExeReturnLblFromOnTenshouExeIketottariInBattle =0x4315F6; 
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeIketottariInBattle() { 
	__asm { 
		mov iIketottariNoudouInBattleBushouID, edi
		mov iIketottariJyudouInBattleBushouID, esi

		// TENSHOU.EXE����call
		call pTenshouExeJumpCallFromToOnTenshouExeIketottariInBattle 

		push eax 
		push ebx 
		push ecx 
		push edx 
		push esp 
		push ebp 
		push esi 
		push edi 
	} 

	OnTenshouExeIketottariInBattleExecute(); 

	__asm { 
		pop edi 
		pop esi 
		pop ebp 
		pop esp 
		pop edx 
		pop ecx 
		pop ebx 
		pop eax 

		jmp pTenshouExeReturnLblFromOnTenshouExeIketottariInBattle 
	} 
} 
char cmdOnTenshouExeJumpFromIketottariInBattle[6]="\xE9"; 
void WriteAsmJumperOnTenshouExeIketottariInBattle() { 
	int iAddress = (int)OnTenshouExeIketottariInBattle; 
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeIketottariInBattle + 5 ) ; 
	memcpy(cmdOnTenshouExeJumpFromIketottariInBattle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B 
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeIketottariInBattle), cmdOnTenshouExeJumpFromIketottariInBattle, 5, NULL); // �ꉞ���S�̂��� 
} 