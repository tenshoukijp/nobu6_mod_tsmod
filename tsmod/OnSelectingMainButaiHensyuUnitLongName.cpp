#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

#include "SetOverWriteButaiMei.h"


/*
 * �����ҏW���悤�Ƃ������A���j�b�g�̕�����ނ�I�����Ē������O���v�����ꂽ
 */ 


/*
00460150  |. 6A 00          PUSH    0
00460152  |. 68 80134D00    PUSH    TENSHOU.004D1380
00460157  |. FF7424 28      PUSH    DWORD PTR SS:[ESP+28]
*/

// �\�����悤�Ƃ��Ă��镐���ԍ�
int iBushouIDOnTenshouExeSelectingMainButaiHensyuUnitExecute = -1;
extern int pOrgButaiMeiArrayOnExecute; // ���XTENSHOU.EXE���ɂ�����CALL��

extern int pNewButaiMeiArrayOnExecute;
extern int pOrgButaiFormNo;

// �ŏI�I�Ɍ�����p�̔z��
char szCmd0ButaiLongNameAshigaru[19] = "���y";
char szCmd1ButaiLongNameKiba[19] = "�R�n";
char szCmd0ButaiLongNameTeppou[19] = "�S�C";
char szCmd0ButaiLongNameTeppouKiba[19] = "�R�n�S�C";

int iNewAddressCmdsSelectingMainButaiHensyuUnitExecute[4] = {
	(int)szCmd0ButaiLongNameAshigaru,
	(int)szCmd1ButaiLongNameKiba,
	(int)szCmd0ButaiLongNameTeppou,
	(int)szCmd0ButaiLongNameTeppouKiba,
};


void OnTenshouExeSelectingMainButaiHensyuUnitExecute() {
	// ESI�̂����A����SI�̕��������������ԍ��ɑ�������
	iBushouIDOnTenshouExeSelectingMainButaiHensyuUnitExecute = iBushouIDOnTenshouExeSelectingMainButaiHensyuUnitExecute & 0xFFFF;

	int iBushouID = iBushouIDOnTenshouExeSelectingMainButaiHensyuUnitExecute - 1;

	// �K�����́A���y�A�R�n�A�S�C�A�S�C�R�n�̕����ŏ�����
	// �I���W�i�����̂��R�s�[���Ă����āc
	pOrgButaiMeiArrayOnExecute = 0x4D1380;

	char **pOrg = (char **)pOrgButaiMeiArrayOnExecute;
	strcpy(szCmd0ButaiLongNameAshigaru,   pOrg[0]);
	strcpy(szCmd1ButaiLongNameKiba,	      pOrg[1]);
	strcpy(szCmd0ButaiLongNameTeppou,     pOrg[2]);
	strcpy(szCmd0ButaiLongNameTeppouKiba, pOrg[3]);


	// �S�񂻂ꂼ��̕��ԂŃV���~���[�V�������Ă݂�B
	int iOriginalForm = nb6bushouref[iBushouID].form;

	// �܂����y�ŃV���~���[�g
	nb6bushouref[iBushouID].form = 0;
	// �������z��|�C���^���㏑������B
	int iAddress = GetOverWriteButaiMeiArrayPointer(iBushouID);
	if ( iAddress ) {
		char **p = (char **)iAddress;
		strcpy(szCmd0ButaiLongNameAshigaru, p[0]);
	}

	// ���ɋR�n�łŃV���~���[�g
	nb6bushouref[iBushouID].form = 1;
	// �������z��|�C���^���㏑������B
	iAddress = GetOverWriteButaiMeiArrayPointer(iBushouID);
	if ( iAddress ) {
		char **p = (char **)iAddress;
		strcpy(szCmd1ButaiLongNameKiba, p[1]);
	}

	// ���ɓS�C�łŃV���~���[�g
	nb6bushouref[iBushouID].form = 2;
	// �������z��|�C���^���㏑������B
	iAddress = GetOverWriteButaiMeiArrayPointer(iBushouID);
	if ( iAddress ) {
		char **p = (char **)iAddress;
		strcpy(szCmd0ButaiLongNameTeppou, p[2]);
	}

	// �Ō�ɋR�n�S�C�łŃV���~���[�g
	nb6bushouref[iBushouID].form = 3;
	// �������z��|�C���^���㏑������B
	iAddress = GetOverWriteButaiMeiArrayPointer(iBushouID);
	if ( iAddress ) {
		char **p = (char **)iAddress;
		strcpy(szCmd0ButaiLongNameTeppouKiba, p[3]);
	}

	// ���Ԃ����ւƖ߂�
	nb6bushouref[iBushouID].form = iOriginalForm;

	// �\�����ׂ��A����int�z��̃A�h���X���A�����Ƃ��ēn��
	pNewButaiMeiArrayOnExecute = int(&iNewAddressCmdsSelectingMainButaiHensyuUnitExecute);
}



/*
// ���̒i�K��EAX�ɕ����ԍ��������Ă��邪�A���ۂ̕����ԍ���AX����(���ʃr�b�g)
00460150  |. 6A 00          PUSH    0
00460152  |. 68 80134D00    PUSH    TENSHOU.004D1380
				�������� JMP TSMod.OnTSExeSelectingMainButaiHensyuUnit �Ə��������Ă����ۂ̏���
00460157  |. FF7424 28      PUSH    DWORD PTR SS:[ESP+28]
*/
int pTenshouExeJumpFromToOnTenshouExeSelectingMainButaiHensyuUnit	  =0x460152; // �֐��͂��̃A�h���X����AOnTenshouExeSelectingMainButaiHensyuUnit�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeSelectingMainButaiHensyuUnit  =0x460157; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeSelectingMainButaiHensyuUnit() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// �����ԍ��̕ۑ�
		mov iBushouIDOnTenshouExeSelectingMainButaiHensyuUnitExecute, eax

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
	OnTenshouExeSelectingMainButaiHensyuUnitExecute();

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

		// pNewButaiMeiArrayOnExecute�͏��Ȃ��Ƃ��I���W�i�����A�㏑�����ꂽ�Achar *[4]�̔z��B
		push pNewButaiMeiArrayOnExecute

		jmp pTenshouExeReturnLblFromOnTenshouExeSelectingMainButaiHensyuUnit
	}
}




char cmdOnTenshouExeJumpFromSelectingMainButaiHensyuUnit[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v5�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeSelectingMainButaiHensyuUnit() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeSelectingMainButaiHensyuUnit;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSelectingMainButaiHensyuUnit + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeSelectingMainButaiHensyuUnit  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromSelectingMainButaiHensyuUnit+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSelectingMainButaiHensyuUnit), cmdOnTenshouExeJumpFromSelectingMainButaiHensyuUnit, 5, NULL); //5�o�C�g�̂ݏ�������
}




