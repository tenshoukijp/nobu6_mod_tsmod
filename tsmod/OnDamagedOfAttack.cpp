#include "WinTarget.h"
#include <time.h>

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "FuncPlayResourceWav.h"
#include "ScenarioMod.h"



// 00431FB8  |. 85C0           TEST    EAX, EAX	�� �c�蕺���̔���B�O��������ߔ��֐��֔��


// 00432022  |> E8 71F5FFFF    CALL    TENSHOU.00431598		�� �ߔ��֐�


/*
 * �_���[�W��H������ۂ̔�_���[�W���T���߂ɂ��鏈��
 */
// �U����������Ă鑤�̕���ID���R�s�[
int iDamagedOfAttackBushouID = -1;

// �U����������Ă��镐���̎c�蕺��
int iRemainOfNumOfSoldier = -1;


// ���݂̐��ɂ�����^�[���o�ߐ��B���ȊO�ł�-1�ƂȂ��Ă���B
extern int iCurIncrementTurnInBattle;

void OnTenshouExeDamagedOfAttackExecute() {

	// �푈���łȂ���Ή������Ȃ�
	if ( !CommonGlobalHandle::isBattleMode ) { return; }

	// �ȉ��A�푈���Ƃ��������̉��A
	// ����ID�`�F�b�N
	if ( 0 <= iDamagedOfAttackBushouID-1 && iDamagedOfAttackBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM ) {

		int iDefBushouID = iDamagedOfAttackBushouID-1;

		// ���������o���B
		byte iCurrSoldierNum = nb6bushouref[iDefBushouID].soldier;
		byte iPrevSoldierNum = nb6bushoucopy[iDefBushouID].soldier;

		// �_���[�W����
		int iDamagedValueu = iPrevSoldierNum - iCurrSoldierNum;

		if ( isLancelotTartare( iDefBushouID ) ) {


			if ( iCurrSoldierNum >= 1 && iPrevSoldierNum >= 2 ) {

				// �����������S�ȏ㌸���Ă�����A������R�ɗ}����
				if ( iPrevSoldierNum - iCurrSoldierNum >= 4 ) {

					// ���ۂɐ��t�B�[���h�ɂ��鎞�̂ݓS�ǉ����o�� ( 50% )�B(���C���}�b�v��ł͓S�Ǐ����͍s�����A���͏o���Ȃ��j
					if ( iCurIncrementTurnInBattle >= 0 && rand() % 2 == 0 ) {
						// �S�ǉ�
						FuncPlaySoundWav(717);
					}

					int iModifySoldierNum = iPrevSoldierNum - 3;
					iDamagedValueu = 3; // ���ւƓ`�B����_���[�W�l��ύX����

					if (iModifySoldierNum < 0 ) { iModifySoldierNum = 0; }
					nb6bushouref[iDefBushouID].soldier = iModifySoldierNum;
					nb6bushoucopy[iDefBushouID].soldier = nb6bushouref[iDefBushouID].soldier;

				}
			}
		}

		else if (isShuiHidechiyo(iDefBushouID)) {


			if (iCurrSoldierNum >= 1 && iPrevSoldierNum >= 2) {

				// �����������S�ȏ㌸���Ă�����A������R�ɗ}����
				if (iPrevSoldierNum - iCurrSoldierNum >= 4) {

					// ���ۂɐ��t�B�[���h�ɂ��鎞�̂݃V���V���V�������o�� ( 50% )�B(���C���}�b�v��ł͓S�Ǐ����͍s�����A���͏o���Ȃ��j
					if (iCurIncrementTurnInBattle >= 0 && rand() % 2 == 0) {
						// �V���V���V����
						FuncPlaySoundWav(617); // �������̉��̕������Ⴄ�I
					}

					int iModifySoldierNum = iPrevSoldierNum - 3;
					iDamagedValueu = 3; // ���ւƓ`�B����_���[�W�l��ύX����

					if (iModifySoldierNum < 0) { iModifySoldierNum = 0; }
					nb6bushouref[iDefBushouID].soldier = iModifySoldierNum;
					nb6bushoucopy[iDefBushouID].soldier = nb6bushouref[iDefBushouID].soldier;

				}
			}
		}



		if ( p_snOnBattleDamaged ) {
			p_snOnBattleDamaged( iDefBushouID+1, iDamagedValueu , NULL );
		}

		nb6bushoucopy[iDefBushouID].soldier = nb6bushouref[iDefBushouID].soldier;

		// �ύX���ꂽ�c�蕺����������ɂ��R�s�[
		iRemainOfNumOfSoldier = nb6bushouref[iDefBushouID].soldier;
	}
}


/*
������1�񓖂���̔�U���Ō����Ă䂭����(GAMEDATA_STRUCT�̐�)��������x�A�w����}����ΓS�ǂ͊�������B
00409C37  |. E8 6E320A00    CALL    TENSHOU.004ACEAA
00409C3C  |. 83C4 08        ADD     ESP, 8
00409C3F  |. 8846 1D        MOV     BYTE PTR DS:[ESI+1D], AL
00409C42  |. 0FB6F0         MOVZX   ESI, AL
				�����̂Q�s JMP TSMod.OnTSExeDamagedOfAttack �Ə��������Ă����ۂ̏���
00409C45  |. 85F6           TEST    ESI, ESI					ESI�͓ˌ�������邽�тɌ����Ă䂭�BESI�𑀍삵�Ă��Ӗ��͂Ȃ��B
00409C47  |. 75 21          JNZ     SHORT TENSHOU.00409C6A

*/
int pTenshouExeJumpFromToOnTenshouExeDamagedOfAttack	   =0x409C3F; // �֐��͂��̃A�h���X����AOnTenshouExeDamagedOfAttack�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeDamagedOfAttack    =0x409C45; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w

// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeDamagedOfAttack() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���X�̏���
		mov byte ptr ds:[esi+0x1D], al

		// ���X�̏���
		movzx   esi, al

		// �U����������Ă鑤�̕���ID���R�s�[
		mov iDamagedOfAttackBushouID, edi

		// ������R�s�[���Ă���
		mov iRemainOfNumOfSoldier, esi

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeDamagedOfAttackExecute();

	__asm {

		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		// �ꉞ���̐��l��esi�ɏ㏑�����Ă����B��������0����[�������Ȃ̂ŕs�v�ȋC�����邪�c
		mov esi, iRemainOfNumOfSoldier

		jmp pTenshouExeReturnLblFromOnTenshouExeDamagedOfAttack
	}
}

/*
0040A185     E8 44320A00    CALL    TENSHOU.004AD3CE
				�������� JMP TSMod.OnTSExeDamagedOfAttack �Ə��������Ă����ۂ̏���
0040A18A  |. 0FB7C0         MOVZX   EAX, AX
*/
char cmdOnTenshouExeJumpFromDamagedOfAttack[7]="\xE9\x90\x90\x90\x90\x90";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeDamagedOfAttack() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeDamagedOfAttack;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeDamagedOfAttack + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeDamagedOfAttack  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromDamagedOfAttack+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeDamagedOfAttack), cmdOnTenshouExeJumpFromDamagedOfAttack, 6, NULL); //6�o�C�g�̂ݏ�������
}




