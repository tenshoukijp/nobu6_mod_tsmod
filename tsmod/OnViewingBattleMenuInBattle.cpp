#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "TSModCommand.h"
#include "ScenarioMod.h"


extern bool isUseCurBushouAmeDeppou; // ���̕������J�S�C���g�����ǂ���

/*
004BB488  00 00 00 00 88 EA 90 C4 8D 55 8C 82 00 00 00 00  ....��čU��....
004BB498  91 E5 96 43 00 00 00 00 00 00 00 00 93 53 96 43  ��C........�S�C
004BB4A8  00 00 00 00 00 00 00 00 93 CB 8C 82 00 00 00 00  ........�ˌ�....
004BB4B8  8D 55 8C 82 00 00 00 00 00 00 00 00 00 00 00 00  �U��............
*/

bool isRewriteDefaultBattleCommand = false;
// �f�t�H���g�́u�ʏ�U���A��čU���A�S�C�U���A��C�U���v�Ȃǂ���������
void rewriteDefaultBattleCommand() {

	if ( isRewriteDefaultBattleCommand == false ) {

		// �u�ʏ�U���v���u�U���v�Ƃ���B
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x4BB4B8), "�U��\x00\x00\x00\x00", 8, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		// �u�S�C�U���v���u�S�C�v�Ƃ���B
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x4BB4A4), "�S�C\x00\x00\x00\x00", 8, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		// �u��C�U���v���u��C�v�Ƃ���B
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x4BB498), "��C\x00\x00\x00\x00", 8, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		isRewriteDefaultBattleCommand = true;

	}
}


/*           ���ʏ�U��    ���ˌ�(�����̃A�h���X���A�󋵂ɂ���ď���������Ηǂ�)
004BB500  B8 B4 4B 00 B0 B4 4B 00 A4 B4 4B 00 98 B4 4B 00  ��K.��K.��K.��K.
004BB510  8C B4 4B 00 00 00 00 00 84 B4 4B 00 7C B4 4B 00  ��K.....��K.|�K.
004BB520  74 B4 4B 00 6C B4 4B 00 64 B4 4B 00 5C B4 4B 00  t�K.l�K.d�K.\�K.
004BB530  54 B4 4B                                         T�K
*/

// ���j���[���o�����Ƃ��Ă��镐����ID(�z��g�p����1�傫������)
int iCurBushouIDOnViewingBattleMenuInBattle1 = -1;





#define SCENARIOMOD_BATTLE_CUSTOM_LABEL_NUM		5


char pBufBattleCustomMenuInBattle[SCENARIOMOD_BATTLE_CUSTOM_LABEL_NUM][20] = {"","","","",""};

// �u�J�X�^���v���w���A�V���ȃA�h���X�T��
int iNewAdressBufBattleCustomMenuInBattle[SCENARIOMOD_BATTLE_CUSTOM_LABEL_NUM] = {
	(int)(pBufBattleCustomMenuInBattle[0]),
	(int)(pBufBattleCustomMenuInBattle[1]),
	(int)(pBufBattleCustomMenuInBattle[2]),
	(int)(pBufBattleCustomMenuInBattle[3]),
	(int)(pBufBattleCustomMenuInBattle[4]),
};




#define COMMAND_LEVEL_NUM 7

//--------------------- �U�� -----------------------


// �V���ȃR�}���h���̔z��
char pBufBattleMenuAttackInBattle[COMMAND_LEVEL_NUM][10] = {"�U��","�U�����","�U����Q","�U����l","�U�����","6","�U�����"};

// �u�U���v���w���A�V���ȃA�h���X
int iNewAdressBufBattleAttackMenuInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufBattleMenuAttackInBattle[0]),
	(int)(pBufBattleMenuAttackInBattle[1]),
	(int)(pBufBattleMenuAttackInBattle[2]),
	(int)(pBufBattleMenuAttackInBattle[3]),
	(int)(pBufBattleMenuAttackInBattle[4]),
	(int)(pBufBattleMenuAttackInBattle[5]),
	(int)(pBufBattleMenuAttackInBattle[6]),
};

//--------------------- �U�� -----------------------

// �u�U���v���w���A���̃A�h���X
int iOrgAdressBufBattleAttackMenuInBattle = 0x4BB4B8;

// �u�U���v�̕�������w���|�C���^�l�������Ă���A�h���X
int pOrgAddressBattleAttackMenuInBattle = 0x4BB500;



//--------------------- ��čU�� -----------------------

// �V���ȃR�}���h���̔z��
char pBufBattleMenuIsseiInBattle[COMMAND_LEVEL_NUM][14] = {"��čU��","��čU�����","��čU����Q","��čU����l","��čU�����","6","��čU�����"};

// �u��čU���v���w���A�V���ȃA�h���X
int iNewAdressBufBattleIsseiMenuInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufBattleMenuIsseiInBattle[0]),
	(int)(pBufBattleMenuIsseiInBattle[1]),
	(int)(pBufBattleMenuIsseiInBattle[2]),
	(int)(pBufBattleMenuIsseiInBattle[3]),
	(int)(pBufBattleMenuIsseiInBattle[4]),
	(int)(pBufBattleMenuIsseiInBattle[5]),
	(int)(pBufBattleMenuIsseiInBattle[6]),
};

//--------------------- ��čU�� -----------------------

// �u��čU���v���w���A���̃A�h���X
int iOrgAdressBufBattleIsseiMenuInBattle = 0x4BB48C;

// �u��čU���v�̕�������w���|�C���^�l�������Ă���A�h���X
int pOrgAddressBattleIsseiMenuInBattle = 0x4BB510;




//--------------------- �R�n�ˌ��n -----------------------

// �V���ȃR�}���h���̔z��
char pBufBattleMenuKiTotsuInBattle[COMMAND_LEVEL_NUM][10] = {"�R��","�R�˥��","�R�˥�Q","�R�˥�l","�R�˥��","6","�R�S�˥��"};

// �u�R�ˋɁv���w���A�V���ȃA�h���X
int iNewAdressBufBattleKiTotsuMenuInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufBattleMenuKiTotsuInBattle[0]),
	(int)(pBufBattleMenuKiTotsuInBattle[1]),
	(int)(pBufBattleMenuKiTotsuInBattle[2]),
	(int)(pBufBattleMenuKiTotsuInBattle[3]),
	(int)(pBufBattleMenuKiTotsuInBattle[4]),
	(int)(pBufBattleMenuKiTotsuInBattle[5]),
	(int)(pBufBattleMenuKiTotsuInBattle[6]),
};

//--------------------- �ˌ��n -----------------------

// �V���ȃR�}���h���̔z��
char pBufBattleMenuTotsugekiInBattle[COMMAND_LEVEL_NUM][10] = {"�ˌ�","�ˌ����","�ˌ���Q","�ˌ���l","�ˌ����","6","�ˌ���^"};

// �u�R�ˋɁv���w���A�V���ȃA�h���X
int iNewAdressBufBattleTotsugekiMenuInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufBattleMenuTotsugekiInBattle[0]),
	(int)(pBufBattleMenuTotsugekiInBattle[1]),
	(int)(pBufBattleMenuTotsugekiInBattle[2]),
	(int)(pBufBattleMenuTotsugekiInBattle[3]),
	(int)(pBufBattleMenuTotsugekiInBattle[4]),
	(int)(pBufBattleMenuTotsugekiInBattle[5]),
	(int)(pBufBattleMenuTotsugekiInBattle[6]),
};

//--------------------- �ˌ����� -----------------------

// �u�ˌ��v���w���A���̃A�h���X
int iOrgAdressBufBattleTotsugekiMenuInBattle = 0x4BB4B0;

// �u�ˌ��v�̕�������w���|�C���^�l�������Ă���A�h���X
int pOrgAddressBattleTotsugekiMenuInBattle = 0x4BB504;


//--------------------- �S�C�n -----------------------

// �V���ȃR�}���h���̔z��
char pBufBattleMenuTeppouInBattle[COMMAND_LEVEL_NUM][10] = {"�S�C","�S�C���","�S�C��Q","�S�C��l","�S�C���","6","�S�C��^"};

// �u�S�C�n�v���w���A�V���ȃA�h���X
int iNewAdressBufBattleTeppouMenuInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufBattleMenuTeppouInBattle[0]),
	(int)(pBufBattleMenuTeppouInBattle[1]),
	(int)(pBufBattleMenuTeppouInBattle[2]),
	(int)(pBufBattleMenuTeppouInBattle[3]),
	(int)(pBufBattleMenuTeppouInBattle[4]),
	(int)(pBufBattleMenuTeppouInBattle[5]),
	(int)(pBufBattleMenuTeppouInBattle[6]),
};

//--------------------- �J�S�C�n -----------------------

// �V���ȃR�}���h���̔z��
char pBufBattleMenuAmeTeppouInBattle[COMMAND_LEVEL_NUM][10] = {"�J�S�C","�J�S�C���","�J�S�C��Q","�J�S�C��l","�J�S�C���","6","�J�S�C��^"};

// �u�J�S�C�n�v���w���A�V���ȃA�h���X
int iNewAdressBufBattleAmeTeppouMenuInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufBattleMenuAmeTeppouInBattle[0]),
	(int)(pBufBattleMenuAmeTeppouInBattle[1]),
	(int)(pBufBattleMenuAmeTeppouInBattle[2]),
	(int)(pBufBattleMenuAmeTeppouInBattle[3]),
	(int)(pBufBattleMenuAmeTeppouInBattle[4]),
	(int)(pBufBattleMenuAmeTeppouInBattle[5]),
	(int)(pBufBattleMenuAmeTeppouInBattle[6]),
};

//--------------------- �|�n -----------------------

// �V���ȃR�}���h���̔z��
char pBufBattleMenuYumiInBattle[COMMAND_LEVEL_NUM][10] = {"�|","�|���","�|��Q","�|��l","�|���","6","�|��^"};

// �u�|�n�v���w���A�V���ȃA�h���X
int iNewAdressBufBattleYumiMenuInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufBattleMenuYumiInBattle[0]),
	(int)(pBufBattleMenuYumiInBattle[1]),
	(int)(pBufBattleMenuYumiInBattle[2]),
	(int)(pBufBattleMenuYumiInBattle[3]),
	(int)(pBufBattleMenuYumiInBattle[4]),
	(int)(pBufBattleMenuYumiInBattle[5]),
	(int)(pBufBattleMenuYumiInBattle[6]),
};


//--------------------- �J�|�n -----------------------

// �V���ȃR�}���h���̔z��
char pBufBattleMenuAmeYumiInBattle[COMMAND_LEVEL_NUM][10] = {"�J�|","�J�|���","�J�|��Q","�J�|��l","�J�|���","6","�J�|��^"};

// �u�J�|�n�v���w���A�V���ȃA�h���X
int iNewAdressBufBattleAmeYumiMenuInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufBattleMenuAmeYumiInBattle[0]),
	(int)(pBufBattleMenuAmeYumiInBattle[1]),
	(int)(pBufBattleMenuAmeYumiInBattle[2]),
	(int)(pBufBattleMenuAmeYumiInBattle[3]),
	(int)(pBufBattleMenuAmeYumiInBattle[4]),
	(int)(pBufBattleMenuAmeYumiInBattle[5]),
	(int)(pBufBattleMenuAmeYumiInBattle[6]),
};


//--------------------- �S�C���ʌn -----------------------

// �u�S�C�U���v���w���A���̃A�h���X
int iOrgAdressBufBattleTeppouMenuInBattle = 0x4BB4A4;

// �u�S�C�U���v�̕�������w���|�C���^�l�������Ă���A�h���X
int pOrgAddressBattleTeppouMenuInBattle = 0x4BB508;



//--------------------- ��C�n -----------------------

// �V���ȃR�}���h���̔z��
char pBufBattleMenuTaihouInBattle[COMMAND_LEVEL_NUM][10] = {"��C","��C���","��C��Q","��C��l","��C���","6","��C��^"};

// �u��C�n�v���w���A�V���ȃA�h���X
int iNewAdressBufBattleTaihouMenuInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufBattleMenuTaihouInBattle[0]),
	(int)(pBufBattleMenuTaihouInBattle[1]),
	(int)(pBufBattleMenuTaihouInBattle[2]),
	(int)(pBufBattleMenuTaihouInBattle[3]),
	(int)(pBufBattleMenuTaihouInBattle[4]),
	(int)(pBufBattleMenuTaihouInBattle[5]),
	(int)(pBufBattleMenuTaihouInBattle[6]),
};

//--------------------- �J��C�n -----------------------


// �V���ȃR�}���h���̔z��
char pBufBattleMenuAmeTaihouInBattle[COMMAND_LEVEL_NUM][10] = {"�J��C","�J��C���","�J��C��Q","�J��C��l","�J��C���","6","�J��C��^"};

// �u�J��C�n�v���w���A�V���ȃA�h���X
int iNewAdressBufBattleAmeTaihouMenuInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufBattleMenuAmeTaihouInBattle[0]),
	(int)(pBufBattleMenuAmeTaihouInBattle[1]),
	(int)(pBufBattleMenuAmeTaihouInBattle[2]),
	(int)(pBufBattleMenuAmeTaihouInBattle[3]),
	(int)(pBufBattleMenuAmeTaihouInBattle[4]),
	(int)(pBufBattleMenuAmeTaihouInBattle[5]),
	(int)(pBufBattleMenuAmeTaihouInBattle[6]),
};

//--------------------- ��C���ʌn -----------------------

// �u��C�U���v���w���A���̃A�h���X
int iOrgAdressBufBattleTaihouMenuInBattle = 0x4BB498;

// �u��C�U���v�̕�������w���|�C���^�l�������Ă���A�h���X
int pOrgAddressBattleTaihouMenuInBattle = 0x4BB50C;

void OnTenshouExeViewingBattleMenuInBattle1Execute() {

	// ����ID�����͈̔�(����ȊO�̒l�������Ă��邱�Ƃ����邽�߃`�F�b�N)
	if (0 <= iCurBushouIDOnViewingBattleMenuInBattle1-1 && iCurBushouIDOnViewingBattleMenuInBattle1-1 < GAMEDATASTRUCT_BUSHOU_NUM) {
	} else {
		return;
	}


	// (�ʏ�)�̕�����̕ύX�B���̂Ƃ���A�ύX���Ă��Ȃ��̂ŁA�K���f�t�H���g�ւƖ߂��̂݁B(�O�̐l��ScenarioMod�����������Ă������̂���)
	if ( 1 ) {
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleAttackMenuInBattle), &iOrgAdressBufBattleAttackMenuInBattle, 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleIsseiMenuInBattle), &iOrgAdressBufBattleIsseiMenuInBattle, 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTotsugekiMenuInBattle), &iOrgAdressBufBattleTotsugekiMenuInBattle, 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &iOrgAdressBufBattleTeppouMenuInBattle, 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTaihouMenuInBattle), &iOrgAdressBufBattleTaihouMenuInBattle, 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
	}


	// �R�n�����R�n�S�C���ł���
	if (nb6bushouref[iCurBushouIDOnViewingBattleMenuInBattle1-1].form==1 ||nb6bushouref[iCurBushouIDOnViewingBattleMenuInBattle1-1].form==3) {

		// ��˂�120(=240)�𒴂���ꍇ�A��R�ː^�v���o��B INDEX��1�����Ă�̒���
		if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 120 &&			// �퓬��240�ȏ�
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_KIBA) >= 5 &&
			isLancelotTartare(iCurBushouIDOnViewingBattleMenuInBattle1-1) ) {

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTotsugekiMenuInBattle), &(iNewAdressBufBattleKiTotsuMenuInBattle[6]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		//��˂�110(=220)�𒴂���ꍇ�A��R�ˋɁv���o��B INDEX��1�����Ă�̒���
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 110 &&   // �퓬��110(=220)�ȏ�
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_KIBA) >= 5 ) {   // �R�n���r����

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTotsugekiMenuInBattle), &(iNewAdressBufBattleKiTotsuMenuInBattle[4]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		//��˂�90(=180)�𒴂���ꍇ�A��R�ˎl�v���o��B INDEX��1�����Ă�̒���
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 90 &&   // �퓬��90(=180)�ȏ�
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_KIBA) >= 5 ) {   // �R�n���r����

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTotsugekiMenuInBattle), &(iNewAdressBufBattleKiTotsuMenuInBattle[3]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		//��˂�80(=160)�𒴂���ꍇ�A��R�ˎQ�v���o��B INDEX��1�����Ă�̒���
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 70 &&   // �퓬��70(=140)�ȏ�
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_KIBA) >= 4 ) {   // �R�n���`����

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTotsugekiMenuInBattle), &(iNewAdressBufBattleKiTotsuMenuInBattle[2]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		//��˂�70(=140)�𒴂���ꍇ�A��R�˓�v���o��B INDEX��1�����Ă�̒���
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 50 &&   // �퓬��50(=100)�ȏ�
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_KIBA) >= 2 ) {   // �R�n���b����

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTotsugekiMenuInBattle), &(iNewAdressBufBattleKiTotsuMenuInBattle[1]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		// �قȂ�ꍇ�͋R�n�ˌ��Ƃ���B
		} else {	 

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTotsugekiMenuInBattle), &(iNewAdressBufBattleKiTotsuMenuInBattle[0]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		}

	// ���y�����S�C���ł���
	} else if (nb6bushouref[iCurBushouIDOnViewingBattleMenuInBattle1-1].form==0 ||nb6bushouref[iCurBushouIDOnViewingBattleMenuInBattle1-1].form==2) {


		//��˂�110(=220)�𒴂���ꍇ�A��ˌ��Ɂv���o��B INDEX��1�����Ă�̒���
		if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 110 &&   // �퓬��110(=220)�ȏ�
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_ASIGARU) >= 5 ) {   // ���y���r����

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTotsugekiMenuInBattle), &(iNewAdressBufBattleTotsugekiMenuInBattle[4]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		//��˂�90(=180)�𒴂���ꍇ�A��ˌ��l�v���o��B INDEX��1�����Ă�̒���
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 90 &&   // �퓬��90(=180)�ȏ�
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_ASIGARU) >= 5 ) {   // ���y���r����

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTotsugekiMenuInBattle), &(iNewAdressBufBattleTotsugekiMenuInBattle[3]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		//��˂�80(=160)�𒴂���ꍇ�A��ˌ��Q�v���o��B INDEX��1�����Ă�̒���
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 70 &&   // �퓬��70(=140)�ȏ�
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_ASIGARU) >= 4 ) {   // ���y���`����

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTotsugekiMenuInBattle), &(iNewAdressBufBattleTotsugekiMenuInBattle[2]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		//��˂�70(=140)�𒴂���ꍇ�A��ˌ���v���o��B INDEX��1�����Ă�̒���
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 50 &&   // �퓬��50(=100)�ȏ�
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_ASIGARU) >= 2 ) {   // ���y���b����

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTotsugekiMenuInBattle), &(iNewAdressBufBattleTotsugekiMenuInBattle[1]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		// �قȂ�ꍇ�͓ˌ��ƌ��ւƖ߂��B
		} else {	 

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTotsugekiMenuInBattle), &(iNewAdressBufBattleTotsugekiMenuInBattle[0]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		}


	// �قȂ�ꍇ�͌��ւƖ߂�
	} else {

		// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTotsugekiMenuInBattle), &iOrgAdressBufBattleTotsugekiMenuInBattle, 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
	}


	//�@�J�S�C���g�������������Ă��āA���V�C���J�̏ꍇ(�S�C���������͋R�n�S�C��)
	if ( isUseCurBushouAmeDeppou && getWeather() == 2 && (nb6bushouref[iCurBushouIDOnViewingBattleMenuInBattle1-1].form == 2||nb6bushouref[iCurBushouIDOnViewingBattleMenuInBattle1-1].form == 3)) {

		// ��˂�110(=220)�𒴂���ꍇ�A��S�C�Ɂv���o��B INDEX��1�����Ă�̒���
		if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 110 &&    // �퓬��110(=220)�ȏ�
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_TEPPOU) >= 5 ) {  // �S�C���r����

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleAmeTeppouMenuInBattle[4]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		// ��˂�90(=180)�𒴂���ꍇ�A��S�C�Ɂv���o��B INDEX��1�����Ă�̒���
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 90 &&    // �퓬��90(=180)�ȏ�
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_TEPPOU) >= 5 ) {  // �S�C���r����

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleAmeTeppouMenuInBattle[3]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		// ��˂�80(=160)�𒴂���ꍇ�A��S�C�Q�v���o��B INDEX��1�����Ă�̒���
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 70 &&    // �퓬��70(=140)�ȏ�
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_TEPPOU) >= 4 ) {  // �S�C���`����

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleAmeTeppouMenuInBattle[2]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		// ��˂�70(=140)�𒴂���ꍇ�A��S�C��v���o��B INDEX��1�����Ă�̒���
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 50 &&    // �퓬��50(=100)�ȏ�
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_TEPPOU) >= 2 ) {  // �S�C���b����

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleAmeTeppouMenuInBattle[1]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g


		// �قȂ�ꍇ�͌��ւƖ߂�
		} else {

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleAmeTeppouMenuInBattle[0]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		}
	}


	//------------------------�|�n(�S�C�ɂ�鏑���������ēx�ύX����̂ł����ɓ���邱��)

	else if ( (nb6bushouref[iCurBushouIDOnViewingBattleMenuInBattle1-1].form == 0||nb6bushouref[iCurBushouIDOnViewingBattleMenuInBattle1-1].form == 1) ) {
		// �J�ȊO
		if ( getWeather() != 2 ) {

			// ��˂�110(=220)�𒴂���ꍇ�A��|�Ɂv���o��B INDEX��1�����Ă�̒���
			if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 110 &&    // �퓬��110(=220)�ȏ�
				nb6savedata_ex.bushou_ex[iCurBushouIDOnViewingBattleMenuInBattle1-1].aptitudeyumi >= 5 ) {  // �|���r����

				// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
				WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleYumiMenuInBattle[4]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

			// ��˂�90(=180)�𒴂���ꍇ�A��|�Ɂv���o��B INDEX��1�����Ă�̒���
			} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 90 &&    // �퓬��90(=180)�ȏ�
				nb6savedata_ex.bushou_ex[iCurBushouIDOnViewingBattleMenuInBattle1-1].aptitudeyumi >= 5 ) {   // �|���r����

				// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
				WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleYumiMenuInBattle[3]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

			// ��˂�80(=160)�𒴂���ꍇ�A��|�Q�v���o��B INDEX��1�����Ă�̒���
			} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 70 &&    // �퓬��70(=140)�ȏ�
				nb6savedata_ex.bushou_ex[iCurBushouIDOnViewingBattleMenuInBattle1-1].aptitudeyumi >= 4 ) {  // �|���`����

				// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
				WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleYumiMenuInBattle[2]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

			// ��˂�70(=140)�𒴂���ꍇ�A��|��v���o��B INDEX��1�����Ă�̒���
			} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 50 &&    // �퓬��50(=100)�ȏ�
				nb6savedata_ex.bushou_ex[iCurBushouIDOnViewingBattleMenuInBattle1-1].aptitudeyumi >= 3 ) {  // �|���a����

				// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
				WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleYumiMenuInBattle[1]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

			// �قȂ�ꍇ�͌��ւƖ߂�
			} else {

				// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
				WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleYumiMenuInBattle[0]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
			}

		// �J�̎�
		} else {
			// ��˂�110(=220)�𒴂���ꍇ�A��J�|�Ɂv���o��B INDEX��1�����Ă�̒���
			if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 110 &&    // �퓬��110(=220)�ȏ�
				nb6savedata_ex.bushou_ex[iCurBushouIDOnViewingBattleMenuInBattle1-1].aptitudeyumi >= 5 ) {  // �J�|���r����

				// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
				WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleAmeYumiMenuInBattle[4]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

			// ��˂�90(=180)�𒴂���ꍇ�A��J�|�Ɂv���o��B INDEX��1�����Ă�̒���
			} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 90 &&    // �퓬��90(=180)�ȏ�
				nb6savedata_ex.bushou_ex[iCurBushouIDOnViewingBattleMenuInBattle1-1].aptitudeyumi >= 5 ) {   // �J�|���r����

				// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
				WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleAmeYumiMenuInBattle[3]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

			// ��˂�80(=160)�𒴂���ꍇ�A��J�|�Q�v���o��B INDEX��1�����Ă�̒���
			} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 70 &&    // �퓬��70(=140)�ȏ�
				nb6savedata_ex.bushou_ex[iCurBushouIDOnViewingBattleMenuInBattle1-1].aptitudeyumi >= 4 ) {  // �J�|���`����

				// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
				WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleAmeYumiMenuInBattle[2]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

			// ��˂�70(=140)�𒴂���ꍇ�A��J�|��v���o��B INDEX��1�����Ă�̒���
			} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 50 &&    // �퓬��50(=100)�ȏ�
				nb6savedata_ex.bushou_ex[iCurBushouIDOnViewingBattleMenuInBattle1-1].aptitudeyumi >= 3 ) {  // �J�|���a����

				// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
				WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleAmeYumiMenuInBattle[1]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

			// �قȂ�ꍇ�͌��ւƖ߂�
			} else {

				// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
				WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleAmeYumiMenuInBattle[0]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
			}
		}

	// ���ʂɓS�C���g����󋵂̏ꍇ
	} else {

		// ��˂�110(=220)�𒴂���ꍇ�A��S�C�Ɂv���o��B INDEX��1�����Ă�̒���
		if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 110 &&    // �퓬��110(=220)�ȏ�
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_TEPPOU) >= 5 ) {  // �S�C���r����

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleTeppouMenuInBattle[4]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		// ��˂�90(=180)�𒴂���ꍇ�A��S�C�Ɂv���o��B INDEX��1�����Ă�̒���
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 90 &&    // �퓬��90(=180)�ȏ�
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_TEPPOU) >= 5 ) {  // �S�C���r����

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleTeppouMenuInBattle[3]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		// ��˂�80(=160)�𒴂���ꍇ�A��S�C�Q�v���o��B INDEX��1�����Ă�̒���
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 70 &&    // �퓬��70(=140)�ȏ�
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_TEPPOU) >= 4 ) {  // �S�C���`����

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleTeppouMenuInBattle[2]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		// ��˂�70(=140)�𒴂���ꍇ�A��S�C��v���o��B INDEX��1�����Ă�̒���
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 50 &&    // �퓬��50(=100)�ȏ�
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_TEPPOU) >= 2 ) {  // �S�C���b����

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleTeppouMenuInBattle[1]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g


		// �قȂ�ꍇ�͌��ւƖ߂�
		} else {

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &iOrgAdressBufBattleTeppouMenuInBattle, 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		}

	}


		//�@�J��C���g�������������Ă��āA���V�C���J�̏ꍇ
	if ( isUseCurBushouAmeDeppou && getWeather() == 2 ) {

		// ��˂�110(=220)�𒴂���ꍇ�A��S�C�Ɂv���o��B INDEX��1�����Ă�̒���
		if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 110 &&    // �퓬��110(=220)�ȏ�
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_TEPPOU) >= 5 ) {  // �S�C���r����

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTaihouMenuInBattle), &(iNewAdressBufBattleAmeTaihouMenuInBattle[4]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		// ��˂�90(=180)�𒴂���ꍇ�A��S�C�Ɂv���o��B INDEX��1�����Ă�̒���
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 90 &&    // �퓬��90(=180)�ȏ�
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_TEPPOU) >= 5 ) {  // �S�C���r����

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTaihouMenuInBattle), &(iNewAdressBufBattleAmeTaihouMenuInBattle[3]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		// ��˂�80(=160)�𒴂���ꍇ�A��S�C�Q�v���o��B INDEX��1�����Ă�̒���
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 70 &&    // �퓬��70(=140)�ȏ�
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_TEPPOU) >= 4 ) {  // �S�C���`����

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTaihouMenuInBattle), &(iNewAdressBufBattleAmeTaihouMenuInBattle[2]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		// ��˂�70(=140)�𒴂���ꍇ�A��S�C��v���o��B INDEX��1�����Ă�̒���
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 50 &&    // �퓬��50(=100)�ȏ�
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_TEPPOU) >= 2 ) {  // �S�C���b����

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTaihouMenuInBattle), &(iNewAdressBufBattleAmeTaihouMenuInBattle[1]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g


		// �قȂ�ꍇ�͌��ւƖ߂�
		} else {

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTaihouMenuInBattle), &(iNewAdressBufBattleAmeTaihouMenuInBattle[0]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		}

	// ���ʂɑ�C���g����󋵂̏ꍇ
	} else {

		// ��˂�110(=220)�𒴂���ꍇ�A��S�C�Ɂv���o��B INDEX��1�����Ă�̒���
		if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 110 &&    // �퓬��110(=220)�ȏ�
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_TEPPOU) >= 5 ) {  // �S�C���r����

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTaihouMenuInBattle), &(iNewAdressBufBattleTaihouMenuInBattle[4]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		// ��˂�90(=180)�𒴂���ꍇ�A��S�C�Ɂv���o��B INDEX��1�����Ă�̒���
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 90 &&    // �퓬��90(=180)�ȏ�
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_TEPPOU) >= 5 ) {  // �S�C���r����

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTaihouMenuInBattle), &(iNewAdressBufBattleTaihouMenuInBattle[3]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		// ��˂�80(=160)�𒴂���ꍇ�A��S�C�Q�v���o��B INDEX��1�����Ă�̒���
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 70 &&    // �퓬��70(=140)�ȏ�
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_TEPPOU) >= 4 ) {  // �S�C���`����

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTaihouMenuInBattle), &(iNewAdressBufBattleTaihouMenuInBattle[2]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g

		// ��˂�70(=140)�𒴂���ꍇ�A��S�C��v���o��B INDEX��1�����Ă�̒���
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 50 &&    // �퓬��50(=100)�ȏ�
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_TEPPOU) >= 2 ) {  // �S�C���b����

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTaihouMenuInBattle), &(iNewAdressBufBattleTaihouMenuInBattle[1]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g


		// �قȂ�ꍇ�͌��ւƖ߂�
		} else {

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTaihouMenuInBattle), &iOrgAdressBufBattleTaihouMenuInBattle, 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
		}

	}


	// ����ID�����͈̔�(����ȊO�̒l�������Ă��邱�Ƃ����邽�߃`�F�b�N)
	if (0 <= iCurBushouIDOnViewingBattleMenuInBattle1-1 && iCurBushouIDOnViewingBattleMenuInBattle1-1 < GAMEDATASTRUCT_BUSHOU_NUM) {
		// �V�i���I���b�h�̑Ώۃ��\�b�h���L���ł���΁c
		if ( p_snOnViewingBattleMenuInBattle ) {

			((PFNSNONVIEWINGBATTLEMENUINBATTLE) p_snOnViewingBattleMenuInBattle)(iCurBushouIDOnViewingBattleMenuInBattle1,
					pBufBattleCustomMenuInBattle[0],
					pBufBattleCustomMenuInBattle[1],
					pBufBattleCustomMenuInBattle[2],
					pBufBattleCustomMenuInBattle[3],
					pBufBattleCustomMenuInBattle[4]);

			for ( int n=0; n<SCENARIOMOD_BATTLE_CUSTOM_LABEL_NUM; n++ ) {
				if ( strlen( pBufBattleCustomMenuInBattle[n] ) > 0 ) {
					if ( n==0 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleAttackMenuInBattle), &(iNewAdressBufBattleCustomMenuInBattle[n]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
					} else if ( n==1 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleIsseiMenuInBattle), &(iNewAdressBufBattleCustomMenuInBattle[n]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
					} else if ( n==2 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTotsugekiMenuInBattle), &(iNewAdressBufBattleCustomMenuInBattle[n]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
					} else if ( n==3 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleCustomMenuInBattle[n]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
					} else if ( n==4 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTaihouMenuInBattle), &(iNewAdressBufBattleCustomMenuInBattle[n]), 4, NULL); // �A�h���X�Ȃ̂�4�o�C�g
					}
				}
			}
		}
	}

}


/*
0042B482  |. 33F6           XOR     ESI, ESI
0042B484  |. B9 00B54B00    MOV     ECX, TENSHOU.004BB500
				�������� JMP TSMod.OnTSExeViewingBattleMenuInBattle1 �Ə��������Ă����ۂ̏���
0042B489  |. 8B55 14        MOV     EDX, DWORD PTR SS:[EBP+14]
*/
int pTenshouExeJumpFromToOnTenshouExeViewingBattleMenuInBattle1		=0x42B484; // �֐��͂��̃A�h���X����AOnTenshouExeViewingBattleMenuInBattle1�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeViewingBattleMenuInBattle1	=0x42B489; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeViewingBattleMenuInBattle1() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ������ID��ۑ����Ă���
		mov  iCurBushouIDOnViewingBattleMenuInBattle1, edi;

		// ���XTENSHOU.EXE���ɂ��������̂�������
		mov  ecx, 4BB500h

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// ���j���[�̒��g�̕������u����������
	OnTenshouExeViewingBattleMenuInBattle1Execute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingBattleMenuInBattle1
	}
}

/*
0042B482  |. 33F6           XOR     ESI, ESI
0042B484  |. B9 00B54B00    MOV     ECX, TENSHOU.004BB500
				�������� JMP TSMod.OnTSExeViewingBattleMenuInBattle1 �Ə��������Ă����ۂ̏���
0042B489  |. 8B55 14        MOV     EDX, DWORD PTR SS:[EBP+14]
*/
char cmdOnTenshouExeJumpFromViewingBattleMenuInBattle1[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �^�[�����`�F���W�����^�C�~���O
void WriteAsmJumperOnTenshouExeViewingBattleMenuInBattle1() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeViewingBattleMenuInBattle1;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingBattleMenuInBattle1 + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingBattleMenuInBattle1  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromViewingBattleMenuInBattle1+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingBattleMenuInBattle1), cmdOnTenshouExeJumpFromViewingBattleMenuInBattle1, 5, NULL); //5�o�C�g�̂ݏ�������
}




//------------------------------------------------------------------------------------------------------



void OnTenshouExeViewingBattleMenuInBattle2Execute() {
	// (���͉����Ȃ�)
	;
}

/*
0044848B  |. 74 10          JE      SHORT TENSHOU.0044849D
0044848D  |. A1 00B54B00    MOV     EAX, DWORD PTR DS:[4BB500]
				�������� JMP TSMod.OnTSExeViewingBattleMenuInBattle2 �Ə��������Ă����ۂ̏���
00448492  |. 8D5E 01        LEA     EBX, DWORD PTR DS:[ESI+1]
*/
int pTenshouExeJumpFromToOnTenshouExeViewingBattleMenuInBattle2		=0x44848D; // �֐��͂��̃A�h���X����AOnTenshouExeViewingBattleMenuInBattle2�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeViewingBattleMenuInBattle2	=0x448492; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeViewingBattleMenuInBattle2() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		mov  eax, DWORD PTR DS:[4BB500h]

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// ������ł͌����͉������Ȃ�
	OnTenshouExeViewingBattleMenuInBattle2Execute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingBattleMenuInBattle2
	}
}

/*
0044848B  |. 74 10          JE      SHORT TENSHOU.0044849D
0044848D  |. A1 00B54B00    MOV     EAX, DWORD PTR DS:[4BB500]
				�������� JMP TSMod.OnTSExeViewingBattleMenuInBattle2 �Ə��������Ă����ۂ̏���
00448492  |. 8D5E 01        LEA     EBX, DWORD PTR DS:[ESI+1]
*/
char cmdOnTenshouExeJumpFromViewingBattleMenuInBattle2[6]="\xE9";
// ���̖��߂�6�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂ŁANOP���Ō�ɕt��������Ă���B


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeViewingBattleMenuInBattle2() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeViewingBattleMenuInBattle2;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingBattleMenuInBattle2 + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingBattleMenuInBattle2  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromViewingBattleMenuInBattle2+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingBattleMenuInBattle2), cmdOnTenshouExeJumpFromViewingBattleMenuInBattle2, 5, NULL); // 6�o�C�g�̂ݏ������� ���̖��߂�6�o�C�g�A�V�K�̖��߂��T�o�C�g�Ȃ̂ŁANOP���t��������Ă���B
}


