
#include "WinTarget.h"

#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"


// �S�C�֘A�̃C�x���g�̃R���g���[��
void CheckTeppouEventControll() {

	// ���C���Q�[���J�n���ɁA�S�C�̑��݂𒲐�����B
	// �������̐��ɓS�C�����݂��Ȃ��Ȃ�΁c
	if ( TSModCommand::World::cmdTheTeppouNoExists ) {

		// �S�C�`���͖����������ƂƂȂ�B
		UnsetTeppouDenrai();

		// �S�Ă̐��͂̓S�C�̒l��0�Ƃ���B
		for ( int g=0; g < GAMEDATASTRUCT_GUNDAN_NUM; g++ ) {
			nb6gundan[g].gun = 0;
		}

		// �S�Ă̕����̓S�C�̒l��0�Ƃ���B
		for ( int i=0; i < GAMEDATASTRUCT_BUSHOU_NUM; i++ ) {

			//����(0=���y,1=�R�n,2=�S�C,3=�R�n�S�C,18�܂�)
			if ( nb6bushouref[i].form == 2 ) {
				nb6bushouref[i].form = 0; // ���y��
			}
			else if ( nb6bushouref[i].form == 3 ) {
				nb6bushouref[i].form = 1; // �R�n��
			}

			nb6bushouref[i].biggun = 0;		//��C����
			nb6bushouref[i].horsegun = 0;	//�R�n�S�C����
			nb6bushouref[i].steelship = 0;	//�S�b�D����
		}

		for (int c=0; c < GAMEDATASTRUCT_CASTLE_NUM; c++) {
			nb6castle[c].bSmith = 0;		// �b�������
		}

	}
}



/*
0042B889     837C24 04 00   CMP     DWORD PTR SS:[ESP+4], 0
0042B88E   . 56             PUSH    ESI
0042B88F   . 57             PUSH    EDI

�オ�S�C�`���C�x���g�̂̍ŏ��̓��B����� RETN �Ƃ��Ă��܂��Δ������Ȃ��Ȃ�B
*/


int pTenshouExeJumpFromToOnTenshouExeTeppouDenraiEvent     =0x42B889; // �֐��͂��̃A�h���X����AOnTenshouExeTeppouDenraiJudgement�ւƃW�����v���Ă���B

char cmdOnTenshouExeJumpFromTeppouDenraiEvent[6]="\xC3\x90\x90\x90\x90"; // C3 = RETN
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeTeppouDenraiEvent() {

	if ( TSModCommand::World::cmdTheTeppouNoExists ) {
		// �S�C�`���̃C�x���g�J�n�̐擪������ׂ���RETN�ɂ��Ă��܂��B

		// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeTeppouDenraiEvent), cmdOnTenshouExeJumpFromTeppouDenraiEvent, 5, NULL); // 5�o�C�g�̂ݏ�������
	}
}





