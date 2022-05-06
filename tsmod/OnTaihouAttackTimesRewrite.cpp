#include "WinTarget.h"
#include <stdlib.h>
#include <string.h>

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"




/*
 ��C�̍U���񐔂��㏑�����肷��A���S���Y��
 */

// �ȉ��͑�C�ł���B
char szIsTaihouItems[][14] = {
	"�J���o�����C",
	"�Z�[�J�[�C",
	"�����^�J�C",
	"���T�@",
	"��ԖC",
	"������",
	"�Ζ",
	"�P�C",
	"�ŒҖC",
	"���j�R",
};

// �ȉ��͑�C�ł͂Ȃ�
char szNoTaihouItems[][14] = {
	// �ȉ��͑�C�łȂ��e�̖��O
	"��䓛",
	"���O��",
	"��A���蒆��",
	"���F��",
	"�䓛",
	"���쓛",
	"���N�Z��",
	"��q����",
	"�U��",
	"�ē�",
	"�đ�",
	"�I�B��",
	"���w��",
	"�F����",
	"���g��",
	"�n�㓛",

	// �ȉ��͑�C�łȂ��ԓ���
	"�����",

	// �ȉ��͑�C�łȂ�����
	"�C�c��",

	// ����͒u���v�̖��O
	"���C",   
};


int IsItemTaihou(char *szItemName) {

	// ��C����C�łȂ����̕Ӓl���ʂ̊i�[�ꏊ
	int iReturnResult = 0;

	// �S�p�Q�����ȏ�̎��A��ԍŌ�̑S�p��������S�C��̎��ł���΁A��C�ł͂Ȃ��S�C�ł���
	if ( strlen(szItemName) >= 4) {
		char *pszLast2Character = szItemName + strlen(szItemName) - 4;

		// ��ԍŌオ�S�C�Ƃ���������ŏI����Ă������C�ł͂Ȃ�
		if ( strcmp(pszLast2Character, "�S�C")==0 ) {
			return 0;
		}
	}


	// ��C�A�C�e�����Ȃ߂āA���̕����񂪊܂܂�Ă����猩��
	for ( int ix = 0; ix <sizeof(szIsTaihouItems)/sizeof(szIsTaihouItems[0]); ix++ ) {
		// ��C�A�C�e���Ɗ��S�ɍ��v������A��C
		if ( strstr(szItemName, szIsTaihouItems[ix] ) ) {
			return 1;
		}
	}

	// ��ԍŌ�̑S�p�����P�����𔲂��o��
	char *pszLastCharacter = szItemName + strlen(szItemName) - 2;

	// �Ō�̕������"�C"�v�������́u"��"��Ȃ��C�Ƃ݂Ȃ��B
	if (strcmp("�C", pszLastCharacter) == 0 || strcmp("��", pszLastCharacter) == 0 ){
		iReturnResult = 1;
	}

	// �������A���̂悤�Ȃ��̂ł����Ă��A��C�ł͂Ȃ��A�C�e�����Ȃ߂āA���̕����񂪊܂܂�Ă������C�ł͂Ȃ�
	for ( int ix = 0; ix <sizeof(szNoTaihouItems)/sizeof(szNoTaihouItems[0]); ix++ ) {
		// ��C�ł͂Ȃ��A�C�e���Ɗ��S�ɍ��v������A��C�ł͂Ȃ�
		if ( strstr(szItemName, szNoTaihouItems[ix] ) ) {
			return 0;
		}
	}

	return iReturnResult;
}

// �A�C�e���̖��O�����C�̃��x���𓾂�B��C�łȂ���΂O�D��C�ł���΂P�A�����t���ł���΁A�Q�ȏ�̒l���Ԃ��Ă���B
int getItemTaihouCnt(char *szItemName) {
	// ��C�łȂ���΁A�J�E���g�͓��R0�ł���B
	if ( !IsItemTaihou(szItemName) ) { return 0; }

	char szCanonCount[][3] = {"��", "��", "�O", "�l", "��", "�Z", "��", "��", "��"};

	// �������v���Ȃ���΁A�ʏ�̑�C�ł������Ƃ������ƂŁA1�����B������Q�A�C�Ƃ��Ďg���̂́A��C������On�v���O�����ɂ䂾�˂�B
	int iReturnCanonCnt = 1;

	// �O�`��܂ł̂ǂꂩ�̐������܂܂�Ă���΁A���̒l��Ԃ��B
	for ( int iCnnCnt=1; iCnnCnt < sizeof(szCanonCount)/sizeof(szCanonCount[0]); iCnnCnt++ ) {

		// �Y���̑S�p���������܂܂�Ă�����A���̒l��Ԃ�
		if (strstr(szItemName, szCanonCount[iCnnCnt])) {
			iReturnCanonCnt = iCnnCnt+1;
			break;
		}
	}

	// �����Ƃ������O���t���ꍇ�́A����Ɂ{�P�J�E���g�����B
	if ( strstr(szItemName, "��") ) {
		if ( iReturnCanonCnt <= 2 ) { 
			iReturnCanonCnt = 3;
		} else {
			iReturnCanonCnt++;
		}

	}

	// ��C�ł���΁c
	return iReturnCanonCnt;
}



//----------------------------------------------------------------------------------------------------------

// �Y����������C�������Ă��邩�ǂ����B
int isCurBushouHasTaihou(int iBushouID) {

	int iMaxTaihouLv = 0;

	// �ƕ����ʂ蒲�ׂ�B��C�������Ă��邩�ǂ����A�����Ă�����ő�ƂȂ��C��T���B
	for (int iKahouID=0; iKahouID<GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {

		// ���L�҂̕���ID����v����ꍇ�̂ݔ���̑Ώ�
		if (nb6kahou[iKahouID].attach-1 == iBushouID) {

			// �Y���A�C�e���̑�C���x����T��
			int iTaihouLv =  getItemTaihouCnt(nb6kahou[iKahouID].name);

			// ���ꂪ�����̔���������C�������Ă���΁A�����V���ȏ�����C���x���Ƃ���B
			if ( iMaxTaihouLv < iTaihouLv ) { iMaxTaihouLv = iTaihouLv; }
		}
	}
	// 
	return iMaxTaihouLv;
}


// ��C�����Ƃ��Ƃ��Ă��镐����ID
int iTaihouAttackTimesRewriteBushouID = -1;

// ��C�̔��ˉ�
int iTaihouAttackTimesRewriteCanonFireCnt = 1;

void OnTenshouExeTaihouAttackTimesRewriteExecute() {

	// ��C�̔��ˉ񐔂͖���1�Ń��Z�b�g����B
	iTaihouAttackTimesRewriteCanonFireCnt = 1;

	// ���������͈̔͂ł��邱��
	if (0 <= iTaihouAttackTimesRewriteBushouID-1 && iTaihouAttackTimesRewriteBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM) {
		
		// ��C�������ǂ����A�܂���C�����ł���Δ��ˉ񐔂�
		int iTaihouCnt = isCurBushouHasTaihou( iTaihouAttackTimesRewriteBushouID-1 );

		// ��C���������Ă镐���ł���ꍇ
		if ( iTaihouCnt >= 1 ) {

			// �f�t�H���g�̑�C�̏ꍇ�́A���ˉ񐔂��Q��Ƃ���B
			if ( iTaihouCnt == 1 ) { iTaihouCnt = 2; } 

			// �A�C�e���̑�C���x���������ݒ���傫���ꍇ�́A����𔭎ˉ񐔂Ƃ���B
			if ( iTaihouCnt > iTaihouAttackTimesRewriteCanonFireCnt ) {
				iTaihouAttackTimesRewriteCanonFireCnt = iTaihouCnt;
			}
		}
	}

}


/*
00432B3B     B8 01000000    MOV     EAX, 1									��C�̉�		��C�̉񐔂��R�ɂ���ƂR��ƂȂ�B
				�������� JMP TSMod.OnTSExeTaihouAttackTimesRewrite �Ə��������Ă����ۂ̏���
00432B40  |> 85C0           TEST    EAX, EAX											
 */
int pTenshouExeJumpFromToOnTenshouExeTaihouAttackTimesRewrite    =0x432B3B; // �֐��͂��̃A�h���X����AOnTenshouExeTaihouAttackTimesRewrite�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeTaihouAttackTimesRewrite =0x432B40; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeTaihouAttackTimesRewrite() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// ��C�����Ƃ��Ƃ��Ă��镐����ID
		mov iTaihouAttackTimesRewriteBushouID, esi;

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
	OnTenshouExeTaihouAttackTimesRewriteExecute();

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

		// ���XTENSHOU.EXE���ɂ��������̂�������
		// ���ˉ񐔂��v�Z�ɂ����̂ŏ㏑���B
		mov eax, iTaihouAttackTimesRewriteCanonFireCnt

		jmp pTenshouExeReturnLblFromOnTenshouExeTaihouAttackTimesRewrite
	}
}

/*
00432B3B     B8 01000000    MOV     EAX, 1									��C�̉�		��C�̉񐔂��R�ɂ���ƂR��ƂȂ�B
				�������� JMP TSMod.OnTSExeTaihouAttackTimesRewrite �Ə��������Ă����ۂ̏���
00432B40  |> 85C0           TEST    EAX, EAX											
*/
char cmdOnTenshouExeJumpFromTaihouAttackTimesRewrite[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeTaihouAttackTimesRewrite() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeTaihouAttackTimesRewrite;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeTaihouAttackTimesRewrite + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeTaihouAttackTimesRewrite  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromTaihouAttackTimesRewrite+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeTaihouAttackTimesRewrite), cmdOnTenshouExeJumpFromTaihouAttackTimesRewrite, 5, NULL); //5�o�C�g�̂ݏ�������
}




