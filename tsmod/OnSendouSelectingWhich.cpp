#include "WinTarget.h"
#include <time.h>

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


/*
 * �����̂ǂ��I�����邩�̃^�C�~���O
 */

/*
A	�_��			
9	����	�^�[��������		
8	���s			
7	�Z�\�K��	�Z�\��Y��邱�Ƃ��L��		
6	��܂�	�܂łɑS��		
5	���s			
4	����			
3	�����B���s���邱�Ƃ�������			�܂�ɑS��
2	�G����			
1	�N���T�̐l�ꊅ(�G�����܂ށj			
0	�N���卬��			�܂�ɑS��
*/

int iSendouSelectingWhich = -1;

// �������s�������s�����h�c
int iSendouSelectingWhichBushouID = -1;

// �����̍ő吔(�J�X�^���l��)
extern int getCustomSoldierMax(int, int);

// �O��̐������ɍ��������J�E���g
extern int iSendouKonranTargetConranCnt;


void OnTenshouExeSendouSelectingWhichExecute() {

	// �O��̐����ō��������l���������ŔO�̂��߃��Z�b�g
	iSendouKonranTargetConranCnt = 0;

	// �����Q�ȏ�ł���΁A�����̑I�������������B

	// ����ID�����͈̔�(����ȊO�̒l�������Ă��邱�Ƃ����邽�߃`�F�b�N)
	if (0 <= iSendouSelectingWhichBushouID-1 && iSendouSelectingWhichBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM) {

		if (getBushouInt(iSendouSelectingWhichBushouID-1) >= 70 &&   // �q�d��70(=140)�ȏ�
			nb6bushouref[iSendouSelectingWhichBushouID-1].maxint >= 85 &&	 // �q�˂�85(=170)�ȏ�
			nb6bushouref[iSendouSelectingWhichBushouID-1].ambition >= 70) {	// ��]��70�ȏ�

			// �G�𒧔��͎��s�Ȃ̂ŁA�卬����
			if ( iSendouSelectingWhich == 2 ) {
				iSendouSelectingWhich = 0;


			// �R�̒����A�������́A�T�̎��s�́A�卬����
			} else if ( iSendouSelectingWhich == 3 || iSendouSelectingWhich == 5) {

				//�@���݂̐g���𓾂�
				int position = nb6bushouname[iSendouSelectingWhichBushouID-1].position;

				//�@�g������ő啺���𓾂�
				int soldiermax = nb6soldiermax[position].soldiermax;

				// �ő啺�m���́A�E�҂������芯�ʁE��E���ɂ���ăt�B���^�[��������
				soldiermax = getCustomSoldierMax( iSendouSelectingWhichBushouID-1, soldiermax );

				// �܂��ő啺���łȂ��Ȃ�΁c
				if ( nb6bushouref[iSendouSelectingWhichBushouID-1].soldier < soldiermax ) {
					// ������	
					iSendouSelectingWhich = 3;
				} else {
					// �卬����
					iSendouSelectingWhich = 0;
				}

			// �U�̌ە��A�������́A�W�͎��s�Ȃ̂ŁA�ە��o����Ȃ�ە���
			} else if ( iSendouSelectingWhich == 6 || iSendouSelectingWhich == 8 ) {
				// �m�C���܂�MAX�łȂ��Ȃ�Υ��
				if ( nb6bushouref[iSendouSelectingWhichBushouID-1].solloyal < 100 ) {
					// �ە�������
					iSendouSelectingWhich = 6;
				} else {
					//�@���݂̐g���𓾂�
					int position = nb6bushouname[iSendouSelectingWhichBushouID-1].position;

					//�@�g������ő啺���𓾂�
					int soldiermax = nb6soldiermax[position].soldiermax;

					// �ő啺�m���́A�E�҂������芯�ʁE��E���ɂ���ăt�B���^�[��������
					soldiermax = getCustomSoldierMax( iSendouSelectingWhichBushouID-1, soldiermax );

					// �܂��ő啺���łȂ��Ȃ�΁c
					if ( nb6bushouref[iSendouSelectingWhichBushouID-1].soldier < soldiermax ) {
						// ������	
						iSendouSelectingWhich = 3;
					} else {
						// �卬����
						iSendouSelectingWhich = 0;
					}
				}

			// �_���͎��s�Ȃ̂ŁA������
			} else if ( iSendouSelectingWhich == 0x0A ) {
				// �m�C���܂�MAX�łȂ��Ȃ�Υ��
				if ( nb6bushouref[iSendouSelectingWhichBushouID-1].solloyal < 100 ) {
					// �ە�������
					iSendouSelectingWhich = 6;
				} else {
					//�@���݂̐g���𓾂�
					int position = nb6bushouname[iSendouSelectingWhichBushouID-1].position;

					//�@�g������ő啺���𓾂�
					int soldiermax = nb6soldiermax[position].soldiermax;

					// �ő啺�m���́A�E�҂������芯�ʁE��E���ɂ���ăt�B���^�[��������
					soldiermax = getCustomSoldierMax( iSendouSelectingWhichBushouID-1, soldiermax );

					// �܂��ő啺���łȂ��Ȃ�΁c
					if ( nb6bushouref[iSendouSelectingWhichBushouID-1].soldier < soldiermax ) {
						// ������	
						iSendouSelectingWhich = 3;
					} else {
						// �卬����
						iSendouSelectingWhich = 0;
					}
				}

			}
		}
	}

	// �����Ɉȏ�ł���΁A
	// �����ƈꊅ���獬���ւƂȂ��ꍞ��
	if (0 <= iSendouSelectingWhichBushouID-1 && iSendouSelectingWhichBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM) {
		if (getBushouInt(iSendouSelectingWhichBushouID-1) >= 110 &&   // �q�d��110(=220)�ȏ�
			nb6bushouref[iSendouSelectingWhichBushouID-1].maxint >= 110 &&	 // �q�˂�110(=220)�ȏ�
			nb6bushouref[iSendouSelectingWhichBushouID-1].ambition >= 110) {	// ��]��110�ȏ�

			// �����ƈꊅ���獬���ƂȂ�
			if ( iSendouSelectingWhich == 1 || iSendouSelectingWhich == 2 ) {
				// �卬����
				iSendouSelectingWhich = 0;
			}
		}
		
	}

	// �Ȃ��͂������A0x0B�ȏ�ɂȂ��Ă���ꍇ�́A�卬���ɂ���B
	if ( iSendouSelectingWhich >= 11 ) {
		iSendouSelectingWhich = 0;
	}
}


/*
0043483A  |. 83FA 0B             |CMP     EDX, 0B
0043483D  |.^72 ED               \JB      SHORT TENSHOU.0043482C
0043483F  |> 57                  PUSH    EDI
				�������� JMP TSMod.OnTSExeSendouSelectingWhich �Ə��������Ă����ۂ̏���
00434840  |. FF1495 A8FB4B0>CALL    DWORD PTR DS:[EDX*4+4BFBA8]									�� ������EDX�l�Ő����̎�ނ����肳���B									
00434847  |. 83C4 04        ADD     ESP, 4									�@�@10(=A)�Ȃ�_���A1�Ȃ�ꊅ�A�ȂǁB									
*/
int pTenshouExeJumpFromToOnTenshouExeSendouSelectingWhich	 =0x43483F; // �֐��͂��̃A�h���X����AOnTenshouExeSendouSelectingWhich�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeSendouSelectingWhich =0x43482C; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeSendouSelectingWhich  =0x434847; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeSendouSelectingWhich() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// �Ώۂ̕���ID
		mov iSendouSelectingWhichBushouID, edi

		// �����̎�ޔԍ�
		mov iSendouSelectingWhich, edx

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
	OnTenshouExeSendouSelectingWhichExecute();

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

		// edx���㏑��
		mov edx, iSendouSelectingWhich

		// ���XTENSHOU.EXE�ł���������
		push edi

		// ���XTENSHOU.EXE�ł���������(�����R�}���h�̎��s) EDX�����ۂ̑I�����ԍ�
		call dword ptr ds:[edx*4+0x4BFBA8]

		// �ȉ��Q��ڂ̐��������s����Ȃ�΁A�ȉ��̂S�s�̃R�����g�A�E�g���͂����Ηǂ��悤�����A�ǂ��`�F�b�N���Ȃ��Ɣ�Ԃ�������Ȃ��B
		/*
		// ���ꂽ���W�X�g���𕜊�
		mov edx, iSendouSelectingWhich;

		add esp, 4
		push edi
		// �Q��ڂ̎��s
		call dword ptr ds:[edx*4+0x4BFBA8]
		*/

		// ���ւƃW�����v
		jmp pTenshouExeReturnLblFromOnTenshouExeSendouSelectingWhich
	}
}

/*
0043483A  |. 83FA 0B             |CMP     EDX, 0B
0043483D  |.^72 ED               \JB      SHORT TENSHOU.0043482C
0043483F  |> 57                  PUSH    EDI
00434840  |. FF1495 A8FB4B0>CALL    DWORD PTR DS:[EDX*4+4BFBA8]									�� ������EDX�l�Ő����̎�ނ����肳���B									
				�������� JMP TSMod.OnTSExeSendouSelectingWhich �Ə��������Ă����ۂ̏���
00434847  |. 83C4 04        ADD     ESP, 4									�@�@10(=A)�Ȃ�_���A1�Ȃ�ꊅ�A�ȂǁB									
*/
char cmdOnTenshouExeJumpFromSendouSelectingWhich[9]="\xE9\x90\x90\x90\x90\x90\x90\x90";
// ���̖��߂�6�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v6�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeSendouSelectingWhich() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeSendouSelectingWhich;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSendouSelectingWhich + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeSendouSelectingWhich  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromSendouSelectingWhich+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSendouSelectingWhich), cmdOnTenshouExeJumpFromSendouSelectingWhich, 8, NULL); //5�o�C�g�̂ݏ�������
}

