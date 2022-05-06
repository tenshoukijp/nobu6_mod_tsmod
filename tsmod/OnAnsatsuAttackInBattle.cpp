#include "WinTarget.h"
#include <time.h>

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "FuncPlayResourceWav.h"
#include "TSModCommand.h"
#include "WaitTimeProcedure.h"
#include "ScenarioMod.h"

int isAnsatsuSuccessOnAttackInBattleExecute = -1;			// �ÎE�������������ǂ���,eax���W�X�^�̒l���㏑�����邽�߂ɂ���
int iOrgBushouIDOfAnsatsuSuruGawa    = -1;					// ���W�X�^����킽���Ă����ÎE���鑤�̕���ID
int iOrgBushouIDOfAnsatsuSareruGawa  = -1;					// ���W�X�^����킽���Ă����ÎE����鑤�̕���ID
int iNewBushouIDOfAnsatsuSuruGawa	 = -1;					// �ÎE���鑤�̕���ID(���H�p)
int iNewBushouIDOfAnsatsuSareruGawa  = -1;					// �ÎE����鑤�̕���ID(���H�p)
int iStatusAntiAnsatsuMessageModeOnAttackInBattleExecute = -1;   // �A���`�ÎE���b�Z�[�W���[�h

BOOL iStatusAntiAnsatsIsOverWriteByScenarioMod = FALSE;

// �G�߂̕ς��ځi����̕ς��ځj�ŌĂяo����A�G�߂̈ÎE�J�E���g�����Z�b�g�����B
void EXSuperService_AnsatsuCntReset() {

	// �S�����̈ÎE�J�E���g�����Z�b�g�B
	for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {
		// �Y�������̍��G�߂̈ÎE���s�J�E���g��+1
		nb6savedata_ex.bushou_ex[iBushouID].ansatsu_cnt = 0;
	}
}


// �A���`�ÎE�V�X�e���A����сA�ÎE�u���b�N�V�X�e��
void OnTenshouExeAnsatsuAttackInBattleExecute() {

	// ���̒l���R�s�[
	iNewBushouIDOfAnsatsuSuruGawa	= iOrgBushouIDOfAnsatsuSuruGawa;
	iNewBushouIDOfAnsatsuSareruGawa = iOrgBushouIDOfAnsatsuSareruGawa;

	// ������ʉ߂���x�ɋt�ÎE���b�Z�[�W���[�h�̓��Z�b�g����B
	iStatusAntiAnsatsuMessageModeOnAttackInBattleExecute = 0;

	// ScenarioMod�ɂ���ăI�[�o�[���C�h���ꂽ���Ƃ̓��Z�b�g����B
	iStatusAntiAnsatsIsOverWriteByScenarioMod = FALSE;

	// �����Ƃ������ԍ������K�Ȕԍ��Ɏ��܂��Ă��鎞�B���̏��������낢��Ƃ�₱�����̂ŁAScenarioMod�̏������ɏo���āA
	// �ύX���Ȃ�������A�������s���B
	if ( 0 <= iNewBushouIDOfAnsatsuSuruGawa-1 && iNewBushouIDOfAnsatsuSuruGawa-1 < GAMEDATASTRUCT_BUSHOU_NUM  &&
		0 <= iNewBushouIDOfAnsatsuSareruGawa-1 && iNewBushouIDOfAnsatsuSareruGawa-1 < GAMEDATASTRUCT_BUSHOU_NUM 
		) {
		// ScenarioMod�̃J�X�^�������ŌR�t��ݒ肷��
		if ( p_snOnCustomCondition ) {
			// ���C����ʂł̈ÎE�̐���																									�����s��	�@���󓮎�			�����ݗ\�肳��Ă��錋��         ��BOOL�l�ւ̕ϊ�
			int isScenarioModDeathResult = ((PFNSNONCUSTOMCONDITION) p_snOnCustomCondition)("AS����::�푈::�ÎE�󓮎Ҏ��S��", iNewBushouIDOfAnsatsuSuruGawa, iNewBushouIDOfAnsatsuSareruGawa, (isAnsatsuSuccessOnAttackInBattleExecute!=0), "", "", 0.0, 0.0);
			if ( isScenarioModDeathResult != CUSTOM_CONDITION_NOMODIFY ) {
				// �ÎE�̐��ۂ��㏑������B
				isAnsatsuSuccessOnAttackInBattleExecute = isScenarioModDeathResult;
				iStatusAntiAnsatsIsOverWriteByScenarioMod = TRUE;
			}
		}
	}

	// �����Ƃ������ԍ������K�Ȕԍ��Ɏ��܂��Ă��鎞�B���AScenarioMod�ŃI�[�o�[���C�h����Ă��Ȃ���
	if ( iStatusAntiAnsatsIsOverWriteByScenarioMod == FALSE &&
		0 <= iNewBushouIDOfAnsatsuSuruGawa-1 && iNewBushouIDOfAnsatsuSuruGawa-1 < GAMEDATASTRUCT_BUSHOU_NUM  &&
		0 <= iNewBushouIDOfAnsatsuSareruGawa-1 && iNewBushouIDOfAnsatsuSareruGawa-1 < GAMEDATASTRUCT_BUSHOU_NUM 
		) {

		// �Y�������̍��G�߂̈ÎE���s�J�E���g��+1
		nb6savedata_ex.bushou_ex[iNewBushouIDOfAnsatsuSuruGawa-1].ansatsu_cnt += 1;

		// �ÎE����鑤�������X���b�g�E�^���^���X�ł���B
		if (isLancelotTartare(iNewBushouIDOfAnsatsuSareruGawa - 1)) {

			// �ÎE���鑤�Ƃ���鑤�����ւ���
			int tmp = iNewBushouIDOfAnsatsuSuruGawa;
			iNewBushouIDOfAnsatsuSuruGawa = iNewBushouIDOfAnsatsuSareruGawa;
			iNewBushouIDOfAnsatsuSareruGawa = tmp;

			// �t�ÎE�͕K����������B
			isAnsatsuSuccessOnAttackInBattleExecute = 1;

			// �t�ÎE���b�Z�[�W���[�h�X�^�[�g
			iStatusAntiAnsatsuMessageModeOnAttackInBattleExecute = 1;

			{
				// ����
				FuncPlaySoundWav(617);
			}

		// �ÎE����鑤����ɏG���ł���B
		} else if (isShuiHidechiyo(iNewBushouIDOfAnsatsuSareruGawa - 1)) {

			// �ÎE���鑤�Ƃ���鑤�����ւ���
			int tmp = iNewBushouIDOfAnsatsuSuruGawa;
			iNewBushouIDOfAnsatsuSuruGawa = iNewBushouIDOfAnsatsuSareruGawa;
			iNewBushouIDOfAnsatsuSareruGawa = tmp;

			// �t�ÎE�͕K����������B
			isAnsatsuSuccessOnAttackInBattleExecute = 1;

			// �t�ÎE���b�Z�[�W���[�h�X�^�[�g
			iStatusAntiAnsatsuMessageModeOnAttackInBattleExecute = 1;

			{
				// ����
				FuncPlaySoundWav(617);
			}


		// �ÎE����鑤���R�m�E���p�t�E�����Ȃ�΁c
		} else if ( nb6bushouref[iNewBushouIDOfAnsatsuSareruGawa-1].job == 0xA0 ||
					nb6bushouref[iNewBushouIDOfAnsatsuSareruGawa-1].job == 0xB0 || 
					nb6bushouref[iNewBushouIDOfAnsatsuSareruGawa-1].job == 0xC0 ) {

			// �ÎE���������邱�Ƃ��Ȃ�
			isAnsatsuSuccessOnAttackInBattleExecute = 0;

		// �ÎE���鑤�������X���b�g�^���^���X�Ȃ�΁c
		} else if ( isLancelotTartare(iNewBushouIDOfAnsatsuSuruGawa-1) ) {

			;
			// �����t�B���^�[������A���`�V�X�e���Ȃ�

		// �ÎE���鑤���R�m�E���p�t�E�����Ȃ�΁c
		} else if ( 
			nb6bushouref[iNewBushouIDOfAnsatsuSuruGawa-1].job == 0xA0 ||
			nb6bushouref[iNewBushouIDOfAnsatsuSuruGawa-1].job == 0xB0 ||
			nb6bushouref[iNewBushouIDOfAnsatsuSuruGawa-1].job == 0xC0 ) {

			;
			// �����t�B���^�[������A���`�V�X�e���Ȃ�

		// �ÎE���鑤����ɂȂ�΁c
		}
		else if (isShuiHidechiyo(iNewBushouIDOfAnsatsuSuruGawa - 1)) {

			;
			// �����t�B���^�[������A���`�V�X�e���Ȃ�

		// ���̑���ʕ������m
		} else {

			// �ÎE�Ԃ�����шÎE�������̃I�v�V�������L���Ȃ�΁c
			if ( TSModCommand::AllBushou::cmdAntiAssassinationInBattle ) {

				// n=�Y�������̈ÎE���s��
				int n = nb6savedata_ex.bushou_ex[iNewBushouIDOfAnsatsuSuruGawa-1].ansatsu_cnt;

				// n��1�ȏ�A�t�ÎE���������n�߂�B
				// �ÎE���J��Ԃ��قǁA�t�ÎE���������₷���Ȃ�B
				if ( n >= 1 && rand() % 50 <= n ) {
					// �ÎE���鑤�Ƃ���鑤�����ւ���
					int tmp = iNewBushouIDOfAnsatsuSuruGawa;
					iNewBushouIDOfAnsatsuSuruGawa = iNewBushouIDOfAnsatsuSareruGawa;
					iNewBushouIDOfAnsatsuSareruGawa = tmp;

					// �t�ÎE�͕K����������B
					isAnsatsuSuccessOnAttackInBattleExecute = 1;

					// �t�ÎE���b�Z�[�W���[�h�X�^�[�g
					iStatusAntiAnsatsuMessageModeOnAttackInBattleExecute = 1;
					{
						// ����
						FuncPlaySoundWav( 617 );
					}


				// n��2�ȏ�ŁA1/n�̊m���ňÎE�t�B���^�[��������Ȃ��B����ȊO�͈ÎE���s�B(rand()%n==0�̎������ÎE�����0�t�B���^�[���|���Ȃ�)
				// �ÎE���J��Ԃ��قǁA�ÎE�͎��s���₷���Ȃ�B
				} else if ( n >= 2 && (rand() % n != 0) ) {
					// �ÎE���������邱�Ƃ��Ȃ�
					isAnsatsuSuccessOnAttackInBattleExecute = 0;

				} 
			}
		}
	}
}


/*
���̏u��
ESI�F�ÎE���鑤
EDI�F�ÎE����鑤
00433992  |. E8 F4950700    CALL    TENSHOU.004ACF8B
				�������� JMP TSMod.OnTSExeAnsatsuAttackInBattle �Ə��������Ă����ۂ̏���
00433997  |. 83C4 04        ADD     ESP, 4
0043399A  |. 85C0           TEST    EAX, EAX
*/
int pTenshouExeJumpFromToOnTenshouExeAnsatsuAttackInBattle		=0x433992; // �֐��͂��̃A�h���X����AOnTenshouExeAnsatsuAttackInBattle�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeAnsatsuAttackInBattle  =0x4ACF8B; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeAnsatsuAttackInBattle	=0x433997; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeAnsatsuAttackInBattle() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeAnsatsuAttackInBattle

		mov isAnsatsuSuccessOnAttackInBattleExecute, eax // ���ݗ\�肳��Ă��錋��
		mov iOrgBushouIDOfAnsatsuSuruGawa, esi 
		mov iOrgBushouIDOfAnsatsuSareruGawa, edi

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
	OnTenshouExeAnsatsuAttackInBattleExecute();

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

		// �ÎE�̐��ۏ����㏑��
		mov eax, isAnsatsuSuccessOnAttackInBattleExecute
		// 
		mov esi, iNewBushouIDOfAnsatsuSuruGawa
		mov edi, iNewBushouIDOfAnsatsuSareruGawa

		jmp pTenshouExeReturnLblFromOnTenshouExeAnsatsuAttackInBattle
	}
}

/*
004487DB   . 57             PUSH    EDI
004487DC   . 57             PUSH    EDI
004487DD   . 57             PUSH    EDI
004487DE   . 6A 0A          PUSH    0A
004487E0   > E8 044C0400    CALL    TENSHOU.0048D3E9
				�������� JMP TSMod.OnTSExeAnsatsuAttackInBattle �Ə��������Ă����ۂ̏���
004487E5   . 83C4 10        ADD     ESP, 10
004487E8   . EB 0B          JMP     SHORT TENSHOU.004487F5
*/
char cmdOnTenshouExeJumpFromAnsatsuAttackInBattle[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeAnsatsuAttackInBattle() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeAnsatsuAttackInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeAnsatsuAttackInBattle + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeAnsatsuAttackInBattle  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromAnsatsuAttackInBattle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeAnsatsuAttackInBattle), cmdOnTenshouExeJumpFromAnsatsuAttackInBattle, 5, NULL); //5�o�C�g�̂ݏ�������
}




