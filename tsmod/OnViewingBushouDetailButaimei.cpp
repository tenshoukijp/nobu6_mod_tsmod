#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

#include "SetOverWriteButaiMei.h"
#include "EXBushou_All.h"

#include "ScenarioMod.h"


/*
 * �����ڍ׉�ʂ������ۂ̕�����
 */ 


/*
00493C75  |. 83C4 04        ADD     ESP, 4
00493C78  |. FF3485 80134D0>PUSH    DWORD PTR DS:[EAX*4+4D1380]
00493C7F  |. 68 982E4D00    PUSH    TENSHOU.004D2E98                 ;  ASCII "%s��"
00493C84  |. 55             PUSH    EBP
*/

// ���X���[���A���̏ꍇ�p�̓��ʏ���
char *szLoslorienDan = "%s�c";
int pAddressSzIsLoslorienDan = (int)szLoslorienDan;
bool isLoslorienViewingBushouDetailButaimeiExecute = false;

// �����t���Ȃ��ꍇ�̓��ʏ���
char *szButaiSuffixNon = "%s";
int pAddressSzIsButaiSuffixNon = (int)szButaiSuffixNon;
bool isButaiSuffixNonViewingBushouDetailButaimeiExecute = false;

// ScenarioMod�̏ꍇ�p�̓��ʏ���
char szScenarioModCustomSuffix[11] = "%s�R�_";
int pAddressSzScenarioModCustomSuffix = (int)szScenarioModCustomSuffix;
bool isScenarioModCustomViewingBushouDetailButaimeiExecute = false;



// �\�����悤�Ƃ��Ă��镐���ԍ�
int iBushouIDOnTenshouExeViewingBushouDetailButaimeiExecute = -1;
int pOrgButaiMeiArrayOnExecute = 0x4D1380;; // ���XTENSHOU.EXE���ɂ�����CALL��

int pNewButaiMeiArrayOnExecute = -1;
int pOrgButaiFormNo = -1;

bool IsMustButaimeiDan(int iBushouID);

void OnTenshouExeViewingBushouDetailButaimeiExecute() {
	// ���xfalse�ŏ�����
	isLoslorienViewingBushouDetailButaimeiExecute = false;
	isButaiSuffixNonViewingBushouDetailButaimeiExecute = false;
	isScenarioModCustomViewingBushouDetailButaimeiExecute = false;

	// ESI�̂����A����SI�̕��������������ԍ��ɑ�������
	iBushouIDOnTenshouExeViewingBushouDetailButaimeiExecute = iBushouIDOnTenshouExeViewingBushouDetailButaimeiExecute & 0xFFFF;

	int iBushouID = iBushouIDOnTenshouExeViewingBushouDetailButaimeiExecute - 1;

	pOrgButaiMeiArrayOnExecute = 0x4D1380;

	// �������z��|�C���^���㏑������B
	int p = GetOverWriteButaiMeiArrayPointer(iBushouID);
	if ( p ) {
		pNewButaiMeiArrayOnExecute = p + pOrgButaiFormNo*4;
	} else {
		// ���������x���͕K���I���W�i���ŏ㏑�������B
		pNewButaiMeiArrayOnExecute = 0x4D1380 + pOrgButaiFormNo*4;
	}


	// ��������̎擾
	if ( p_snOnGettingButaimeiGobiSprintf ) {
		char szpSzModifyDan[11] = "";
		int ret = p_snOnGettingButaimeiGobiSprintf(NULL, szpSzModifyDan); // ���������ScenarioMod����擾
		if (ret) {
			strcpy(szScenarioModCustomSuffix, szpSzModifyDan);
			isScenarioModCustomViewingBushouDetailButaimeiExecute = true;
		}
	}

	// �c�ł���ׂ��Ȃ��
	if ( IsMustButaimeiDan(iBushouID) ) {
		isLoslorienViewingBushouDetailButaimeiExecute = true;
	}

}

// "�c"�ł���ׂ���
bool IsMustButaimeiDan(int iBushouID) {

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		//�@���X���[���A���Ȃ�A"��"�ł͂Ȃ�"�c"�ɂ��邽��
		if ( isLancelotTartare(iBushouID ) || IsLoslorien(iBushouID) ) {
			return true;
		}
		// �R�m�n�ƂȂ邽�߁B
		if ( nb6bushouref[iBushouID].job == 0xA0 && Is_FieldWar() ) {
			// ���퓬���Ă��āA�C�̏�ɂ���
			HEX_POSITION p = GetFieldHexUnitPos( iBushouID );
			// �����̈ʒu�̖����𓾂�
			int iHexRole = GetFieldHexRole( p );

			// �C�̏�
			if ( iHexRole != FIELD_HEX_ROLE::�C && iHexRole != FIELD_HEX_ROLE::�� ) {
				// �M���o���h�ӊO�Ȃ�΁A�c
				if ( strcmp(nb6bushouname[iBushouID].familyname, "�g�h�i")!=0 ) {
					return true;
				}
			}
		}
	}

	return false;
}


/*
// ���̒i�K��ESI�ɕ����ԍ��������Ă��邪�A���ۂ̕����ԍ���SI����(���ʃr�b�g)
00493C70  |. E8 1462F7FF    CALL    TENSHOU.00409E89
				�������� JMP TSMod.OnTSExeViewingBushouDetailButaimei �Ə��������Ă����ۂ̏���
00493C75  |. 83C4 04        ADD     ESP, 4
00493C78  |. FF3485 80134D0>PUSH    DWORD PTR DS:[EAX*4+4D1380]				eax ��nb6bushou[iBushouID].form �Ɠ��l
00493C7F  |. 68 982E4D00    PUSH    TENSHOU.004D2E98                 ;  ASCII "%s��"
00493C84  |. 55             PUSH    EBP

*/
int pTenshouExeJumpFromToOnTenshouExeViewingBushouDetailButaimei	 =0x493C70; // �֐��͂��̃A�h���X����AOnTenshouExeViewingBushouDetailButaimei�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeViewingBushouDetailButaimei =0x409E89; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeViewingBushouDetailButaimei  =0x493C84; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeViewingBushouDetailButaimei() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���̏����������ɋL��
		call pTenshouExeJumpCallFromToOnTenshouExeViewingBushouDetailButaimei

		// ���̏���
		ADD     ESP, 4

		// nb6bushou[iBushouID].form ������ۑ�
		mov pOrgButaiFormNo, eax

		// �����ԍ��̕ۑ�
		mov iBushouIDOnTenshouExeViewingBushouDetailButaimeiExecute, esi

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
	OnTenshouExeViewingBushouDetailButaimeiExecute();

	// ScenarioMod�ɂ���ďC������Ă���ꍇ
	if ( isScenarioModCustomViewingBushouDetailButaimeiExecute ) {
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
			mov eax, pNewButaiMeiArrayOnExecute
			PUSH    DWORD PTR DS:[eax] 

			// eax �������̒l�ɖ߂�
			mov eax, pOrgButaiFormNo

			// TENSHOU.EXE �Ɍ��X����������
			push pAddressSzScenarioModCustomSuffix

			jmp pTenshouExeReturnLblFromOnTenshouExeViewingBushouDetailButaimei
		}

	// ���X���[���A������("�`�c")
	} else if ( isLoslorienViewingBushouDetailButaimeiExecute ) {
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
			mov eax, pNewButaiMeiArrayOnExecute
			PUSH    DWORD PTR DS:[eax] 

			// eax �������̒l�ɖ߂�
			mov eax, pOrgButaiFormNo

			// TENSHOU.EXE �Ɍ��X����������
			push pAddressSzIsLoslorienDan

			jmp pTenshouExeReturnLblFromOnTenshouExeViewingBushouDetailButaimei
		}

	// �ʏ�̕���("�`��")
	} else {
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
			mov eax, pNewButaiMeiArrayOnExecute
			PUSH    DWORD PTR DS:[eax] 

			// eax �������̒l�ɖ߂�
			mov eax, pOrgButaiFormNo

			// TENSHOU.EXE �Ɍ��X����������
			push 0x4D2E98

			jmp pTenshouExeReturnLblFromOnTenshouExeViewingBushouDetailButaimei
		}
	}
}




char cmdOnTenshouExeJumpFromViewingBushouDetailButaimei[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v5�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeViewingBushouDetailButaimei() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeViewingBushouDetailButaimei;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingBushouDetailButaimei + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingBushouDetailButaimei  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromViewingBushouDetailButaimei+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingBushouDetailButaimei), cmdOnTenshouExeJumpFromViewingBushouDetailButaimei, 5, NULL); //5�o�C�g�̂ݏ�������
}




