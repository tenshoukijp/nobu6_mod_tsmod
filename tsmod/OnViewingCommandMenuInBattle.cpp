#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "TSModCommand.h"
#include "ScenarioMod.h"





//------------------------------------------------------------------------------------------------------

/*�@�@�@�@�@ ���Z�\�@�@�@�@������		 ���ە�    �@  ���ދp
004CD508  [9C D4 4C] 00 [B0 AF 4B] 00 [94 D4 4C] 00 [8C D4 4C] 00  ��L.��K.��L.��L.
			 ����O
004CD518  [84 D4 4C] 00                                      �EL..


004CD494  8C DB 95 91 00 00 00 00 8B 5A 94 5C 00 00 00 00  �ە�....�Z�\....



004BB508  A4 B4 4B 00 98 B4 4B 00 8C B4 4B 00 00 00 00 00  ��K.��K.��K.....
004BB518  [84 B4 4B] 00 [7C B4 4B] 00 [74 B4 4B] 00 [6C B4 4B] 00  ��K.|�K.t�K.l�K.
             �� �ꊅ�@�@�@������ �@     ������        ���΍U��
004BB528  [64 B4 4B] 00 [5C B4 4B] 00 [54 B4 4B] 00 00 00 00 00  d�K.\�K.T�K.....
   �@�@�@�@�@�� �ÎE	  ������		������
004BB538  4C B4 4B 00 44                                L�K.D

004CD484  8F E9 8A 4F 00 00 00 00 91 DE 8B 70 00 00 00 00  ��O....�ދp....
004CD494  8C DB 95 91 00 00 00 00 8B 5A 94 5C 00 00 00 00  �ە�....�Z�\....


004BB454  92 A7 94 AD 00 00 00 00 90 F8 93 AE 00 00 00 00  ����....����....
004BB464  88 C3 8E 45 00 00 00 00 89 CE 8D 55 82 DF 00 00  �ÎE....�΍U��..
004BB474  8D AC 97 90 00 00 00 00 92 A5 95 BA 00 00 00 00  ����....����....
004BB484  88 EA 8A 85 00 00 00 00                          �ꊅ....���
*/

// ���݂̕���ID
int iCurBushouIDOnViewingCommandMenuInBattle = -1;




#define SCENARIOMOD_BASE_CUSTOM_LABEL_NUM		12


char pBufBaseCustomMenuInBattle[SCENARIOMOD_BASE_CUSTOM_LABEL_NUM][20] = {"","","","","","","","","","","",""};

// �u�J�X�^���v���w���A�V���ȃA�h���X�T��
int iNewAdressBufBaseCustomMenuInBattle[SCENARIOMOD_BASE_CUSTOM_LABEL_NUM] = {
	(int)(pBufBaseCustomMenuInBattle[0]),
	(int)(pBufBaseCustomMenuInBattle[1]),
	(int)(pBufBaseCustomMenuInBattle[2]),
	(int)(pBufBaseCustomMenuInBattle[3]),
	(int)(pBufBaseCustomMenuInBattle[4]),
	(int)(pBufBaseCustomMenuInBattle[5]),
	(int)(pBufBaseCustomMenuInBattle[6]),
	(int)(pBufBaseCustomMenuInBattle[7]),
	(int)(pBufBaseCustomMenuInBattle[8]),
	(int)(pBufBaseCustomMenuInBattle[9]),
	(int)(pBufBaseCustomMenuInBattle[10]),
	(int)(pBufBaseCustomMenuInBattle[11]),
};


//--------------------- ���ʌn -----------------------

#define COMMAND_LEVEL_NUM 7

//--------------------- �ە��n -----------------------
char pBufBattleMenuTokukreiXInBattle[] = "����";

// �u����v���w���A�V���ȃA�h���X
int iNewAdressBufBattleTokureiMenuInBattle = (int)pBufBattleMenuTokukreiXInBattle;

// �u�ە��v���w���A���̃A�h���X
int iOrgAdressBufBattleKobuMenuInBattle = 0x4CD494;

// �u�ە��v�̕�������w���|�C���^�l�������Ă���A�h���X
int pOrgAddressBattleKobuMenuInBattle = 0x4CD510;


//--------------------- ��O�n -----------------------
// �u��O�v���w���A���̃A�h���X
int iOrgAdressBufBattleJogaiMenuInBattle = 0x4CD484;

// �u��O�v�̕�������w���|�C���^�l�������Ă���A�h���X
int pOrgAddressBattleJogaiMenuInBattle = 0x4CD518;


//--------------------- �ދp�n -----------------------
// �u�ދp�v���w���A���̃A�h���X
int iOrgAdressBufBattleTaikyakuMenuInBattle = 0x4CD48C;

// �u�ދp�v�̕�������w���|�C���^�l�������Ă���A�h���X
int pOrgAddressBattleTaikyakuMenuInBattle = 0x4CD514;



//--------------------- �����n -----------------------
// �u�����v���w���A���̃A�h���X
int iOrgAdressBufBattleNaiouMenuInBattle = 0x4BAFB0;

// �u�����v�̕�������w���|�C���^�l�������Ă���A�h���X
int pOrgAddressBattleNaiouMenuInBattle = 0x4CD50C;


//--------------------- �Z�\�n -----------------------
// �V���ȃR�}���h���̔z��
char pBufCommandMenuGinouInBattle[COMMAND_LEVEL_NUM][10] = {"�Z�\���","�Z�\���","�Z�\��Q","�Z�\��l","�Z�\���","6","�Z�\��^"};

// �u�����n�v���w���A�V���ȃA�h���X
int iNewAdressBufCommandMenuGinouInBattle[8] = {
	(int)(pBufCommandMenuGinouInBattle[0]),
	(int)(pBufCommandMenuGinouInBattle[1]),
	(int)(pBufCommandMenuGinouInBattle[2]),
	(int)(pBufCommandMenuGinouInBattle[3]),
	(int)(pBufCommandMenuGinouInBattle[4]),
	(int)(pBufCommandMenuGinouInBattle[5]),
	(int)(pBufCommandMenuGinouInBattle[6]),
	(int)(pBufCommandMenuGinouInBattle[7]),
};


// �u�Z�\�v���w���A���̃A�h���X
int iOrgAdressBufBattleGinouMenuInBattle = 0x4CD49C;

// �u�Z�\�v�̕�������w���|�C���^�l�������Ă���A�h���X
int pOrgAddressBattleGinouMenuInBattle = 0x4CD508;



//--------------------- �����n -----------------------


// �V���ȃR�}���h���̔z��
char pBufCommandMenuChohatsuInBattle[COMMAND_LEVEL_NUM][10] = {"����","�������","������Q","������l","�������","6","������^"};

// �u�����n�v���w���A�V���ȃA�h���X
int iNewAdressBufCommandMenuChohatsuInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufCommandMenuChohatsuInBattle[0]),
	(int)(pBufCommandMenuChohatsuInBattle[1]),
	(int)(pBufCommandMenuChohatsuInBattle[2]),
	(int)(pBufCommandMenuChohatsuInBattle[3]),
	(int)(pBufCommandMenuChohatsuInBattle[4]),
	(int)(pBufCommandMenuChohatsuInBattle[5]),
	(int)(pBufCommandMenuChohatsuInBattle[6]),
};

// �u�����v���w���A���̃A�h���X
int iOrgAdressBufBattleChohatsuMenuInBattle = 0x4BB454;

// �u�����v�̕�������w���|�C���^�l�������Ă���A�h���X
int pOrgAddressBattleChohatsuMenuInBattle = 0x4BB530;



// �V���ȃR�}���h���̔z��
char pBufCommandMenuSendouInBattle[COMMAND_LEVEL_NUM][10] = {"����","�������","������Q","������l","�������","6","������^"};

// �u�����n�v���w���A�V���ȃA�h���X
int iNewAdressBufCommandMenuSendouInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufCommandMenuSendouInBattle[0]),
	(int)(pBufCommandMenuSendouInBattle[1]),
	(int)(pBufCommandMenuSendouInBattle[2]),
	(int)(pBufCommandMenuSendouInBattle[3]),
	(int)(pBufCommandMenuSendouInBattle[4]),
	(int)(pBufCommandMenuSendouInBattle[5]),
	(int)(pBufCommandMenuSendouInBattle[6]),
};

// �u�����v���w���A���̃A�h���X
int iOrgAdressBufBattleSendouMenuInBattle = 0x4BB45C;

// �u�����v�̕�������w���|�C���^�l�������Ă���A�h���X
int pOrgAddressBattleSendouMenuInBattle = 0x4BB52C;





//--------------------- �����n -----------------------

// �V���ȃR�}���h���̔z��
char pBufCommandMenuKonranInBattle[COMMAND_LEVEL_NUM][10] = {"����","�������","������Q","������l","�������","6","������^"};

// �u�����n�v���w���A�V���ȃA�h���X
int iNewAdressBufCommandMenuKonranInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufCommandMenuKonranInBattle[0]),
	(int)(pBufCommandMenuKonranInBattle[1]),
	(int)(pBufCommandMenuKonranInBattle[2]),
	(int)(pBufCommandMenuKonranInBattle[3]),
	(int)(pBufCommandMenuKonranInBattle[4]),
	(int)(pBufCommandMenuKonranInBattle[5]),
	(int)(pBufCommandMenuKonranInBattle[6]),
};

// �u�����v���w���A���̃A�h���X
int iOrgAdressBufBattleKonranMenuInBattle = 0x4BB474;

// �u�����v�̕�������w���|�C���^�l�������Ă���A�h���X
int pOrgAddressBattleKonranMenuInBattle = 0x4BB520;


//--------------------- �΍U�n -----------------------

// �V���ȃR�}���h���̔z��
char pBufCommandMenuHizemeInBattle[COMMAND_LEVEL_NUM][10] = {"�΍U��","�΍U���","�΍U��Q","�΍U��l","�΍U���","6","�΍U��^"};

// �u�΍U�n�v���w���A�V���ȃA�h���X
int iNewAdressBufCommandMenuHizemeInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufCommandMenuHizemeInBattle[0]),
	(int)(pBufCommandMenuHizemeInBattle[1]),
	(int)(pBufCommandMenuHizemeInBattle[2]),
	(int)(pBufCommandMenuHizemeInBattle[3]),
	(int)(pBufCommandMenuHizemeInBattle[4]),
	(int)(pBufCommandMenuHizemeInBattle[5]),
	(int)(pBufCommandMenuHizemeInBattle[6]),
};

//--------------------- �E�҉΍U�n -----------------------

// �V���ȃR�}���h���̔z��
char pBufCommandMenuKatonInBattle[COMMAND_LEVEL_NUM][10] = {"�Γ�","�Γ٥��","�Γ٥�Q","�Γ٥�l","�Γ٥��","6","�Γ٥�^"};

// �u�Γٌn�v���w���A�V���ȃA�h���X
int iNewAdressBufCommandMenuKatonInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufCommandMenuKatonInBattle[0]),
	(int)(pBufCommandMenuKatonInBattle[1]),
	(int)(pBufCommandMenuKatonInBattle[2]),
	(int)(pBufCommandMenuKatonInBattle[3]),
	(int)(pBufCommandMenuKatonInBattle[4]),
	(int)(pBufCommandMenuKatonInBattle[5]),
	(int)(pBufCommandMenuKatonInBattle[6]),
};


// �u�΍U�v���w���A���̃A�h���X
int iOrgAdressBufBattleHizemeMenuInBattle = 0x4BB46C;

// �u�΍U�v�̕�������w���|�C���^�l�������Ă���A�h���X
int pOrgAddressBattleHizemeMenuInBattle = 0x4BB524;


//--------------------- �ÎE�n -----------------------

// �V���ȃR�}���h���̔z��
char pBufCommandMenuAnsatsuInBattle[COMMAND_LEVEL_NUM][10] = {"�ÎE","�ÎE���","�ÎE��Q","�ÎE��l","�ÎE���","6","�ÎE��^"};

// �u�ÎE�n�v���w���A�V���ȃA�h���X
int iNewAdressBufCommandMenuAnsatsuInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufCommandMenuAnsatsuInBattle[0]),
	(int)(pBufCommandMenuAnsatsuInBattle[1]),
	(int)(pBufCommandMenuAnsatsuInBattle[2]),
	(int)(pBufCommandMenuAnsatsuInBattle[3]),
	(int)(pBufCommandMenuAnsatsuInBattle[4]),
	(int)(pBufCommandMenuAnsatsuInBattle[5]),
	(int)(pBufCommandMenuAnsatsuInBattle[6]),
};

// �u�ÎE�v���w���A���̃A�h���X
int iOrgAdressBufBattleAnsatsuMenuInBattle = 0x4BB464;

// �u�ÎE�v�̕�������w���|�C���^�l�������Ă���A�h���X
int pOrgAddressBattleAnsatsuMenuInBattle = 0x4BB528;


//--------------------- �����n -----------------------

// �V���ȃR�}���h���̔z��
char pBufCommandMenuChoheiInBattle[COMMAND_LEVEL_NUM][10] = {"����","�������","������Q","������l","�������","6","������^"};

// �u�����n�v���w���A�V���ȃA�h���X
int iNewAdressBufCommandMenuChoheiInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufCommandMenuChoheiInBattle[0]),
	(int)(pBufCommandMenuChoheiInBattle[1]),
	(int)(pBufCommandMenuChoheiInBattle[2]),
	(int)(pBufCommandMenuChoheiInBattle[3]),
	(int)(pBufCommandMenuChoheiInBattle[4]),
	(int)(pBufCommandMenuChoheiInBattle[5]),
	(int)(pBufCommandMenuChoheiInBattle[6]),
};

// �u�����v���w���A���̃A�h���X
int iOrgAdressBufBattleChoheiMenuInBattle = 0x4BB47C;

// �u�����v�̕�������w���|�C���^�l�������Ă���A�h���X
int pOrgAddressBattleChoheiMenuInBattle = 0x4BB51C;


//--------------------- �ꊅ�n -----------------------

// �V���ȃR�}���h���̔z��
char pBufCommandMenuIkkatsuInBattle[COMMAND_LEVEL_NUM][10] = {"�ꊅ","�ꊅ���","�ꊅ��Q","�ꊅ��l","�ꊅ���","6","�ꊅ��^"};

// �u�ꊅ�n�v���w���A�V���ȃA�h���X
int iNewAdressBufCommandMenuIkkatsuInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufCommandMenuIkkatsuInBattle[0]),
	(int)(pBufCommandMenuIkkatsuInBattle[1]),
	(int)(pBufCommandMenuIkkatsuInBattle[2]),
	(int)(pBufCommandMenuIkkatsuInBattle[3]),
	(int)(pBufCommandMenuIkkatsuInBattle[4]),
	(int)(pBufCommandMenuIkkatsuInBattle[5]),
	(int)(pBufCommandMenuIkkatsuInBattle[6]),
};

// �u�ꊅ�v���w���A���̃A�h���X
int iOrgAdressBufBattleIkkatsuMenuInBattle = 0x4BB484;

// �u�ꊅ�v�̕�������w���|�C���^�l�������Ă���A�h���X
int pOrgAddressBattleIkkatsuMenuInBattle = 0x4BB518;

//-------------------------------------------


void OnTenshouExeViewingCommandMenuInBattleExecute() {

	// �����͈̓`�F�b�N
	if (0 <= iCurBushouIDOnViewingCommandMenuInBattle-1 && iCurBushouIDOnViewingCommandMenuInBattle-1 < GAMEDATASTRUCT_BUSHOU_NUM) {
	} else {
		return;
	}

	if ( 1 ) {
		// �ە��̕�����̕ύX�B���̂Ƃ���A�ύX���Ă��Ȃ��̂ŁA�K���f�t�H���g�ւƖ߂��̂݁B(�O�̐l��ScenarioMod�����������Ă������̂���)
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleKobuMenuInBattle), &iOrgAdressBufBattleKobuMenuInBattle, 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		// ��O�̕�����̕ύX�B���̂Ƃ���A�ύX���Ă��Ȃ��̂ŁA�K���f�t�H���g�ւƖ߂��̂݁B(�O�̐l��ScenarioMod�����������Ă������̂���)
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleJogaiMenuInBattle), &iOrgAdressBufBattleJogaiMenuInBattle, 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		// �ދp�̕�����̕ύX�B���̂Ƃ���A�ύX���Ă��Ȃ��̂ŁA�K���f�t�H���g�ւƖ߂��̂݁B(�O�̐l��ScenarioMod�����������Ă������̂���)
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTaikyakuMenuInBattle), &iOrgAdressBufBattleTaikyakuMenuInBattle, 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		// �����̕�����̕ύX�B���̂Ƃ���A�ύX���Ă��Ȃ��̂ŁA�K���f�t�H���g�ւƖ߂��̂݁B(�O�̐l��ScenarioMod�����������Ă������̂���)
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleNaiouMenuInBattle), &iOrgAdressBufBattleNaiouMenuInBattle, 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		// �Z�\�̕�����̕ύX�B���̂Ƃ���A�ύX���Ă��Ȃ��̂ŁA�K���f�t�H���g�ւƖ߂��̂݁B(�O�̐l��ScenarioMod�����������Ă������̂���)
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleGinouMenuInBattle), &iOrgAdressBufBattleGinouMenuInBattle, 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		// �����̕�����̕ύX�B���̂Ƃ���A�ύX���Ă��Ȃ��̂ŁA�K���f�t�H���g�ւƖ߂��̂݁B(�O�̐l��ScenarioMod�����������Ă������̂���)
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleChohatsuMenuInBattle), &iOrgAdressBufBattleChohatsuMenuInBattle, 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		// ��̕�����̕ύX�B���̂Ƃ���A�ύX���Ă��Ȃ��̂ŁA�K���f�t�H���g�ւƖ߂��̂݁B(�O�̐l��ScenarioMod�����������Ă������̂���)
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleSendouMenuInBattle), &iOrgAdressBufBattleSendouMenuInBattle, 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		// �����̕�����̕ύX�B���̂Ƃ���A�ύX���Ă��Ȃ��̂ŁA�K���f�t�H���g�ւƖ߂��̂݁B(�O�̐l��ScenarioMod�����������Ă������̂���)
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleKonranMenuInBattle), &iOrgAdressBufBattleKonranMenuInBattle, 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		// �΍U�߂̕�����̕ύX�B���̂Ƃ���A�ύX���Ă��Ȃ��̂ŁA�K���f�t�H���g�ւƖ߂��̂݁B(�O�̐l��ScenarioMod�����������Ă������̂���)
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleHizemeMenuInBattle), &iOrgAdressBufBattleHizemeMenuInBattle, 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		// �ÎE�̕�����̕ύX�B���̂Ƃ���A�ύX���Ă��Ȃ��̂ŁA�K���f�t�H���g�ւƖ߂��̂݁B(�O�̐l��ScenarioMod�����������Ă������̂���)
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleAnsatsuMenuInBattle), &iOrgAdressBufBattleAnsatsuMenuInBattle, 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		// �����̕�����̕ύX�B���̂Ƃ���A�ύX���Ă��Ȃ��̂ŁA�K���f�t�H���g�ւƖ߂��̂݁B(�O�̐l��ScenarioMod�����������Ă������̂���)
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleChoheiMenuInBattle), &iOrgAdressBufBattleChoheiMenuInBattle, 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		// �ꊅ�̕�����̕ύX�B���̂Ƃ���A�ύX���Ă��Ȃ��̂ŁA�K���f�t�H���g�ւƖ߂��̂݁B(�O�̐l��ScenarioMod�����������Ă������̂���)
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleIkkatsuMenuInBattle), &iOrgAdressBufBattleIkkatsuMenuInBattle, 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
	}

	
	// �Z�\�̕�����̕ύX�B�Z�\���ɉ����āA�������ύX����B
	if ( 1 ) {
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleGinouMenuInBattle), &iOrgAdressBufBattleGinouMenuInBattle, 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
	}

	// �����X���b�g�E�^���^���X�ł���B
	if ( isLancelotTartare(iCurBushouIDOnViewingCommandMenuInBattle-1) ) {

		// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleKobuMenuInBattle), &iNewAdressBufBattleTokureiMenuInBattle, 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

	// ���͌ە�
	} else {
		// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleKobuMenuInBattle), &iOrgAdressBufBattleKobuMenuInBattle, 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
	}

	//.-----------------------------------------------------
	//�q�d�֘A���������𒴂���ꍇ�A������ɁE�����Ɂv���o��B INDEX��1�����Ă�̒���
	if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 110 &&   // �q�d��110(=220)�ȏ�
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 110) { // �q�˂�110(=220)�ȏ�

		// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleChohatsuMenuInBattle), &(iNewAdressBufCommandMenuChohatsuInBattle[4]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleKonranMenuInBattle), &(iNewAdressBufCommandMenuKonranInBattle[4]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

	//�q�d�֘A���������𒴂���ꍇ�A������l�E�����l�v���o��B INDEX��1�����Ă�̒���
	} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 90 &&   // �q�d��90(=180)�ȏ�
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 100) { // �q�˂�100(=200)�ȏ�

		// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleChohatsuMenuInBattle), &(iNewAdressBufCommandMenuChohatsuInBattle[3]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleKonranMenuInBattle), &(iNewAdressBufCommandMenuKonranInBattle[3]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g


	//�q�d�֘A���������𒴂���ꍇ�A������Q�E�����Q�v���o��B INDEX��1�����Ă�̒���
	} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 70 &&   // �q�d��70(=140)�ȏ�
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 85) { // �q�˂�85(=170)�ȏ�

		// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleChohatsuMenuInBattle), &(iNewAdressBufCommandMenuChohatsuInBattle[2]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleKonranMenuInBattle), &(iNewAdressBufCommandMenuKonranInBattle[2]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g


	//�q�d�֘A���������𒴂���ꍇ�A�������E������v���o��B INDEX��1�����Ă�̒���
	} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 50 &&   // �q�d��50(=100)�ȏ�
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 70) { // �q�˂�70(=140)�ȏ�

		// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleChohatsuMenuInBattle), &(iNewAdressBufCommandMenuChohatsuInBattle[1]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleKonranMenuInBattle), &(iNewAdressBufCommandMenuKonranInBattle[1]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

	// ���̓f�t�H�ɖ߂�
	} else {
		// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleChohatsuMenuInBattle), &iOrgAdressBufBattleChohatsuMenuInBattle, 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleKonranMenuInBattle), &iOrgAdressBufBattleKonranMenuInBattle, 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
	}


	//.-----------------------------------------------------
	//�q�d�Ɩ��͊֘A���������𒴂���ꍇ�A������Ɂv���o��B INDEX��1�����Ă�̒���
	if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 110 &&   // �q�d��110(=220)�ȏ�
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 110 && // �q�˂�110(=220)�ȏ�
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].charm >= 110		// ���͂�110�ȏ�
		) { 

		// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleChoheiMenuInBattle), &(iNewAdressBufCommandMenuChoheiInBattle[4]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

	//�q�d�Ɩ��͊֘A���������𒴂���ꍇ�A������l�v���o��B INDEX��1�����Ă�̒���
	} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 90 &&   // �q�d��90(=180)�ȏ�
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 100 &&	 // �q�˂�100(=200)�ȏ�
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].charm >= 90) {	// ���͂�90�ȏ�

		// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleChoheiMenuInBattle), &(iNewAdressBufCommandMenuChoheiInBattle[3]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g


	//�q�d�Ɩ��͊֘A���������𒴂���ꍇ�A������Q�v���o��B INDEX��1�����Ă�̒���
	} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 70 &&   // �q�d��70(=140)�ȏ�
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 85 &&	 // �q�˂�85(=170)�ȏ�
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].charm >= 70) {	// ���͂�70�ȏ�

		// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleChoheiMenuInBattle), &(iNewAdressBufCommandMenuChoheiInBattle[2]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g


	//�q�d�Ɩ��͊֘A���������𒴂���ꍇ�A�������v���o��B INDEX��1�����Ă�̒���
	} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 50 &&   // �q�d��50(=100)�ȏ�
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 70 &&  // �q�˂�70(=140)�ȏ�
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].charm >= 50) {	// ���͂�50�ȏ�

		// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleChoheiMenuInBattle), &(iNewAdressBufCommandMenuChoheiInBattle[1]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

	// ���̓f�t�H�ɖ߂�
	} else {
		// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleChoheiMenuInBattle), &iOrgAdressBufBattleChoheiMenuInBattle, 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
	}



	//.-----------------------------------------------------
	//�q�d�Ɩ�]�֘A���������𒴂���ꍇ�A��ꊅ�ɁE�����Ɂv���o��B INDEX��1�����Ă�̒���
	if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 110 &&   // �q�d��110(=220)�ȏ�
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 110 && // �q�˂�110(=220)�ȏ�
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].ambition >= 110		// ��]��110�ȏ�
		) { 

		// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleIkkatsuMenuInBattle), &(iNewAdressBufCommandMenuIkkatsuInBattle[4]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleSendouMenuInBattle), &(iNewAdressBufCommandMenuSendouInBattle[4]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

	//�q�d�Ɩ�]�֘A���������𒴂���ꍇ�A��ꊅ�l�E�����l�v���o��B INDEX��1�����Ă�̒���
	} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 90 &&   // �q�d��90(=180)�ȏ�
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 100 &&	 // �q�˂�100(=200)�ȏ�
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].ambition >= 90) {	// ��]��90�ȏ�

		// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleIkkatsuMenuInBattle), &(iNewAdressBufCommandMenuIkkatsuInBattle[3]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleSendouMenuInBattle), &(iNewAdressBufCommandMenuSendouInBattle[3]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g


	//�q�d�Ɩ�]�֘A���������𒴂���ꍇ�A��ꊅ�Q�E�����Q�v���o��B INDEX��1�����Ă�̒���
	} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 70 &&   // �q�d��70(=140)�ȏ�
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 85 &&	 // �q�˂�85(=170)�ȏ�
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].ambition >= 70) {	// ��]��70�ȏ�

		// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleIkkatsuMenuInBattle), &(iNewAdressBufCommandMenuIkkatsuInBattle[2]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleSendouMenuInBattle), &(iNewAdressBufCommandMenuSendouInBattle[2]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g


	//�q�d�Ɩ�]�֘A���������𒴂���ꍇ�A��ꊅ��E������v���o��B INDEX��1�����Ă�̒���
	} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 50 &&   // �q�d��50(=100)�ȏ�
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 70 &&  // �q�˂�70(=140)�ȏ�
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].ambition >= 50) {	// ��]��50�ȏ�

		// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleIkkatsuMenuInBattle), &(iNewAdressBufCommandMenuIkkatsuInBattle[1]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleSendouMenuInBattle), &(iNewAdressBufCommandMenuSendouInBattle[1]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

	// ���̓f�t�H�ɖ߂�
	} else {
		// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleIkkatsuMenuInBattle), &iOrgAdressBufBattleIkkatsuMenuInBattle, 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleSendouMenuInBattle), &iOrgAdressBufBattleSendouMenuInBattle, 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
	}



	//.-----------------------------------------------------
	// �E�҈ȊO�ł���΁c
	if (nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].job != 0x10 ) {

		//�q�d�֘A���������𒴂���ꍇ�A��΍U�Ɂv���o��B INDEX��1�����Ă�̒���
		if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 110 &&   // �q�d��110(=220)�ȏ�
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 110) { // �q�˂�110(=220)�ȏ�

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleHizemeMenuInBattle), &(iNewAdressBufCommandMenuHizemeInBattle[4]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		//�q�d�֘A���������𒴂���ꍇ�A��΍U�l�v���o��B INDEX��1�����Ă�̒���
		} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 90 &&   // �q�d��90(=180)�ȏ�
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 100) { // �q�˂�100(=200)�ȏ�

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleHizemeMenuInBattle), &(iNewAdressBufCommandMenuHizemeInBattle[3]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g


		//�q�d�֘A���������𒴂���ꍇ�A��΍U�Q�v���o��B INDEX��1�����Ă�̒���
		} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 70 &&   // �q�d��70(=140)�ȏ�
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 85) { // �q�˂�85(=170)�ȏ�

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleHizemeMenuInBattle), &(iNewAdressBufCommandMenuHizemeInBattle[2]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g


		//�q�d�֘A���������𒴂���ꍇ�A��΍U��v���o��B INDEX��1�����Ă�̒���
		} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 50 &&   // �q�d��50(=100)�ȏ�
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 70) { // �q�˂�70(=140)�ȏ�

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleHizemeMenuInBattle), &(iNewAdressBufCommandMenuHizemeInBattle[1]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		// ���̓f�t�H�ɖ߂�
		} else {
			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleHizemeMenuInBattle), &iOrgAdressBufBattleHizemeMenuInBattle, 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		}


		//.-----------------------------------------------------
		// �ÎE�n�c
		//�q�d�Ɩ�]���������𒴂���ꍇ�A��ÎE�l�v���o��B INDEX��1�����Ă�̒���
		if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 110 &&   // �q�d��110(=220)�ȏ�
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 110 && // �q�˂�110(=220)�ȏ�
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].ambition >= 110) { // ��]��110�ȏ�

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleAnsatsuMenuInBattle), &(iNewAdressBufCommandMenuAnsatsuInBattle[3]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		//�q�d�Ɩ�]���������𒴂���ꍇ�A��ÎE�O�v���o��B INDEX��1�����Ă�̒���
		} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 90 &&   // �q�d��90(=180)�ȏ�
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 100 && // �q�˂�100(=200)�ȏ�
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].ambition >= 90) { // ��]��90�ȏ�

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleAnsatsuMenuInBattle), &(iNewAdressBufCommandMenuAnsatsuInBattle[2]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		//�q�d�Ɩ�]���������𒴂���ꍇ�A��ÎE��v���o��B INDEX��1�����Ă�̒���
		} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 70 &&   // �q�d��70(=140)�ȏ�
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 85 && // �q�˂�85(=170)�ȏ�
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].ambition >= 70) { // ��]��70�ȏ�

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleAnsatsuMenuInBattle), &(iNewAdressBufCommandMenuAnsatsuInBattle[1]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		// ���̓f�t�H�ɖ߂�
		} else {
			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleAnsatsuMenuInBattle), &iOrgAdressBufBattleAnsatsuMenuInBattle, 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		}


	//.-----------------------------------------------------
	// �E�҂ł���΁c(�΍U�ƉΓق��قȂ�)
	} else {

		//�q�d�֘A���������𒴂���ꍇ�A��ΓًɁv���o��B INDEX��1�����Ă�̒���
		if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 110 &&   // �q�d��110(=220)�ȏ�
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 110) { // �q�˂�110(=220)�ȏ�

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleHizemeMenuInBattle), &(iNewAdressBufCommandMenuKatonInBattle[4]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		//�q�d�֘A���������𒴂���ꍇ�A��Γَl�v���o��B INDEX��1�����Ă�̒���
		} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 90 &&   // �q�d��90(=180)�ȏ�
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 100) { // �q�˂�100(=200)�ȏ�

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleHizemeMenuInBattle), &(iNewAdressBufCommandMenuKatonInBattle[3]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g


		//�q�d�֘A���������𒴂���ꍇ�A��ΓَQ�v���o��B INDEX��1�����Ă�̒���
		} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 70 &&   // �q�d��70(=140)�ȏ�
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 85) { // �q�˂�85(=170)�ȏ�

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleHizemeMenuInBattle), &(iNewAdressBufCommandMenuKatonInBattle[2]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g


		//�q�d�֘A���������𒴂���ꍇ�A��Γٓ�v���o��B INDEX��1�����Ă�̒���
		} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 50 &&   // �q�d��50(=100)�ȏ�
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 70) { // �q�˂�70(=140)�ȏ�

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleHizemeMenuInBattle), &(iNewAdressBufCommandMenuKatonInBattle[1]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		// ���̓f�t�H�ɖ߂�(�΍U�̏ꍇ�͉ΓقƂ���)
		} else {
			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleHizemeMenuInBattle), &(iNewAdressBufCommandMenuKatonInBattle[0]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		}

		//.-----------------------------------------------------
		// �ÎE�n�c
		//�q�d�Ɩ�]���������𒴂���ꍇ�A��ÎE�Ɂv���o��B INDEX��1�����Ă�̒���
		if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 110 &&   // �q�d��110(=220)�ȏ�
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 110 && // �q�˂�110(=220)�ȏ�
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].ambition >= 110) { // ��]��110�ȏ�

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleAnsatsuMenuInBattle), &(iNewAdressBufCommandMenuAnsatsuInBattle[4]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		//�q�d�Ɩ�]���������𒴂���ꍇ�A��ÎE�l�v���o��B INDEX��1�����Ă�̒���
		} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 90 &&   // �q�d��90(=180)�ȏ�
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 100 && // �q�˂�100(=200)�ȏ�
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].ambition >= 50) { // ��]��50�ȏ�

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleAnsatsuMenuInBattle), &(iNewAdressBufCommandMenuAnsatsuInBattle[3]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		//�q�d���������𒴂���ꍇ�A��ÎE�Q�v���o��B INDEX��1�����Ă�̒���
		} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 70 &&   // �q�d��70(=140)�ȏ�
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 85 ) { // �q�˂�85(=170)�ȏ�

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleAnsatsuMenuInBattle), &(iNewAdressBufCommandMenuAnsatsuInBattle[2]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		//�q�d���������𒴂���ꍇ�A��ÎE�Q�v���o��B INDEX��1�����Ă�̒���
		} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 50 &&   // �q�d��50(=100)�ȏ�
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 70 ) { // �q�˂�70(=140)�ȏ�

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleAnsatsuMenuInBattle), &(iNewAdressBufCommandMenuAnsatsuInBattle[1]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g


		// ���̓f�t�H�ɖ߂�
		} else {
			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleAnsatsuMenuInBattle), &iOrgAdressBufBattleAnsatsuMenuInBattle, 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		}


	}


	// ����ID�����͈̔�(����ȊO�̒l�������Ă��邱�Ƃ����邽�߃`�F�b�N)
	if (0 <= iCurBushouIDOnViewingCommandMenuInBattle-1 && iCurBushouIDOnViewingCommandMenuInBattle-1 < GAMEDATASTRUCT_BUSHOU_NUM) {
		// �V�i���I���b�h�̑Ώۃ��\�b�h���L���ł���΁c
		if ( p_snOnViewingCommandMenuInBattle ) {
			((PFNSNONVIEWINGCOMMANDMENUINBATTLE) p_snOnViewingCommandMenuInBattle)(iCurBushouIDOnViewingCommandMenuInBattle, 
					pBufBaseCustomMenuInBattle[0],
					pBufBaseCustomMenuInBattle[1],
					pBufBaseCustomMenuInBattle[2],
					pBufBaseCustomMenuInBattle[3],
					pBufBaseCustomMenuInBattle[4],
					pBufBaseCustomMenuInBattle[5],
					pBufBaseCustomMenuInBattle[6],
					pBufBaseCustomMenuInBattle[7],
					pBufBaseCustomMenuInBattle[8],
					pBufBaseCustomMenuInBattle[9],
					pBufBaseCustomMenuInBattle[10],
					pBufBaseCustomMenuInBattle[11]
			);
			for ( int n=0; n<SCENARIOMOD_BASE_CUSTOM_LABEL_NUM; n++ ) {
				if ( strlen( pBufBaseCustomMenuInBattle[n] ) > 0 ) {
					if ( n==0 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleKobuMenuInBattle), &(iNewAdressBufBaseCustomMenuInBattle[n]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
					} else if ( n==1 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleJogaiMenuInBattle), &(iNewAdressBufBaseCustomMenuInBattle[n]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
					} else if ( n==2 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTaikyakuMenuInBattle), &(iNewAdressBufBaseCustomMenuInBattle[n]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
					} else if ( n==3 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleNaiouMenuInBattle), &(iNewAdressBufBaseCustomMenuInBattle[n]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
					} else if ( n==4 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleGinouMenuInBattle), &(iNewAdressBufBaseCustomMenuInBattle[n]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
					} else if ( n==5 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleChohatsuMenuInBattle), &(iNewAdressBufBaseCustomMenuInBattle[n]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
					} else if ( n==6 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleSendouMenuInBattle), &(iNewAdressBufBaseCustomMenuInBattle[n]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
					} else if ( n==7 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleKonranMenuInBattle), &(iNewAdressBufBaseCustomMenuInBattle[n]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
					} else if ( n==8 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleHizemeMenuInBattle), &(iNewAdressBufBaseCustomMenuInBattle[n]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
					} else if ( n==9 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleAnsatsuMenuInBattle), &(iNewAdressBufBaseCustomMenuInBattle[n]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
					} else if ( n==10 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleChoheiMenuInBattle), &(iNewAdressBufBaseCustomMenuInBattle[n]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
					} else if ( n==11 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleIkkatsuMenuInBattle), &(iNewAdressBufBaseCustomMenuInBattle[n]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
					}
				}
			}
		}
	}

}

/*
0046A7FA  |. 68 08D54C00    PUSH    TENSHOU.004CD508
0046A7FF  |. 56             PUSH    ESI
0046A800  |. BE FFFF0000    MOV     ESI, 0FFFF
				�������� JMP TSMod.OnTSExeViewingCommandMenuInBattle �Ə��������Ă����ۂ̏���
0046A805  |. 56             PUSH    ESI
0046A806  |. 56             PUSH    ESI
0046A807  |. E8 BF970300    CALL    TENSHOU.004A3FCB
0046A80C  |. 83C4 18        ADD     ESP, 18
0046A80F  |. 3BC6           CMP     EAX, ESI
*/
int pTenshouExeJumpFromToOnTenshouExeViewingCommandMenuInBattle		=0x46A800; // �֐��͂��̃A�h���X����AOnTenshouExeViewingCommandMenuInBattle�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeViewingCommandMenuInBattle	=0x46A805; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeViewingCommandMenuInBattle() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		mov  iCurBushouIDOnViewingCommandMenuInBattle, ebx;

		// ���XTENSHOU.EXE���ɂ��������̂�������
		mov esi, 0xFFFF

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// �R�}���h������̒u����
	OnTenshouExeViewingCommandMenuInBattleExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingCommandMenuInBattle
	}
}

/*
0046A7FA  |. 68 08D54C00    PUSH    TENSHOU.004CD508
0046A7FF  |. 56             PUSH    ESI
0046A800  |. BE FFFF0000    MOV     ESI, 0FFFF
				�������� JMP TSMod.OnTSExeViewingCommandMenuInBattle �Ə��������Ă����ۂ̏���
0046A805  |. 56             PUSH    ESI
0046A806  |. 56             PUSH    ESI
0046A807  |. E8 BF970300    CALL    TENSHOU.004A3FCB
0046A80C  |. 83C4 18        ADD     ESP, 18
0046A80F  |. 3BC6           CMP     EAX, ESI
*/
char cmdOnTenshouExeJumpFromViewingCommandMenuInBattle[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �^�[�����`�F���W�����^�C�~���O
void WriteAsmJumperOnTenshouExeViewingCommandMenuInBattle() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeViewingCommandMenuInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingCommandMenuInBattle + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingCommandMenuInBattle  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromViewingCommandMenuInBattle+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingCommandMenuInBattle), cmdOnTenshouExeJumpFromViewingCommandMenuInBattle, 5, NULL); //5�o�C�g�̂ݏ�������
}


