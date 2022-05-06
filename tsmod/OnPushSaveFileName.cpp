#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "ScenarioMod.h"

#include "FuncSelectMenuDialog.h"

#include <algorithm>

using namespace std;


/*
�Z�[�u�f�[�^�����[�h����ۂɁA���O(savedat.n6p)�����܂鎞�B

00479C3B  |. 68 708D4C00    PUSH    TENSHOU.004C8D70                 ;  ASCII "A:SAVEDAT.N6P"
00479C40  |. 56             PUSH    ESI
00479C41  |. E8 A7350300    CALL    TENSHOU.004AD1ED
00479C46  |. 83C4 08        ADD     ESP, 8
*/

#define SAVEDATA_MULTI_NUM  10
#define SAVEDATA_FILENAME_MAX_LEN	80
char szMultiSaveDataFileName[SAVEDATA_MULTI_NUM][SAVEDATA_FILENAME_MAX_LEN] = {
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
};

int iPushFilePointerAddressOfSaveFileName = NULL;
void OnTenshouExePushSaveFileNameExecute() {

	// �V�i���I�^�C�g�����̍X�V�BScenarioMod�̊֐����Ăяo��
	if ( p_snOnViewingScenarioTitleList ) {
		 p_snOnViewingScenarioTitleList();
	}

	// �܂��O�Ԗڂ͏�Ƀf�t�H���g�Ɠ���(A:SAVEDATA.N6P)�Ƃ���B
	strcpy( szMultiSaveDataFileName[0], ((char *)GAMEDATASTRUCT_SAVEDATAFILENAME_ADDRESS)-2 ); // A:SAVEDATA.N6P�Ȃ̂�2�������A�h���X����

	// �f�t�H���g�ł͂����I�������t�@�C�����Ƃ��Ă���
	iPushFilePointerAddressOfSaveFileName = (int)szMultiSaveDataFileName[0]; // �Z�[�u�f�[�^�t�@�C�����z���0�Ԗڂ̔z��(�f�t�H���g��)

	// savedat*.n6p�Ƃ����t�@�C������T��
	vector<string> listTargetSaveFiles;


	WIN32_FIND_DATA ffd;
	// *Mod.dll�Ƃ������O�̃t�@�C���B
	HANDLE hFileList = FindFirstFile("savedat*.n6p", &ffd);

	// ���݂����ꍇ�A�ǂݍ��ݑΏۂƂ���
	if ( hFileList != INVALID_HANDLE_VALUE ) {
		do {
			char *p = ffd.cFileName;
			_strupr(p);

			// �R�s�[�Ƃ��������񂪊܂܂ꂽsavedat.n6p�ȊO�̎�
			if ( ! strstr(p, "�R�s�[") ){ 
				string filename = string("A:")+p;
				listTargetSaveFiles.push_back(filename);
			}

		} while (FindNextFile(hFileList, &ffd));
	}
	FindClose(hFileList);

	// szMultiSaveDataFileName�̍ŏ��̂P�������ă��Z�b�g
	for ( int i=1; i<SAVEDATA_MULTI_NUM; i++) {
		strcpy( szMultiSaveDataFileName[i], "");
	}

	// ���������t�@�C���Q�̒�����A�f�t�H���g���ł���AA:SAVEDATA.N6P�������C�^���[�^
	vector<string>::iterator end_it = remove( listTargetSaveFiles.begin(), listTargetSaveFiles.end(), szMultiSaveDataFileName[0] );

	// �܂��A�����������̂�savedat.n6p���܂܂�邩�ǂ���
	int iSumOfList=1; // szMultiSaveDataFileName�ւ̔z��B�f�t�H���g�ȊO�Ȃ̂�1����X�^�[�g
	for ( vector<string>::iterator it = listTargetSaveFiles.begin(); 
		  it != end_it && iSumOfList<SAVEDATA_MULTI_NUM; // end_it �Ƃ�����Ōv�Z��������ȃC�^���[�^�܂�
		  it++, iSumOfList++ ) { // szMultiSaveDataFileName�̔z����͂ݏo���Ȃ��悤�ɁB

		strncpy( szMultiSaveDataFileName[iSumOfList], (*it).c_str(), SAVEDATA_FILENAME_MAX_LEN-2);
		szMultiSaveDataFileName[iSumOfList][SAVEDATA_FILENAME_MAX_LEN-1] = NULL; // �A�z�݂����Ȓ������O�ł�OK�Ȃ悤�ɔԕ�
	}

	// ���Ǒ��݂���Z�[�u�f�[�^���P���傫���Ȃ�΃��j���[��
	if ( iSumOfList > 1 ) {
		vector<string>  menu;
		for ( int i=0; i<SAVEDATA_MULTI_NUM; i++ ) {
			// �L���ȕ�����ł���΁c
			if ( strlen(szMultiSaveDataFileName[i]) > 3 ) {

				// ���j���[���쐬����B

				// ���j���[�p�̕�����ɉ��H
				string menu_title = ((char *)szMultiSaveDataFileName[i])+2; // �ŏ���"A:"���������邽�߁B
				menu_title = "�@" + menu_title + "�@"; // �O��ɋ�
				// �v�b�V��
				menu.push_back( menu_title );
			}
		}

		// ���j���[�\��
		int iSelect = 0;
		
		while ( 1 ) {
			iSelect = SelectMenuDialog(menu);
			if ( iSelect != 0xFFFF ) { break; } // �L���ȃ��j���[��I��

		}

		// �L���ȃ��j���[�ԍ���I��
		if ( iSelect < (int)menu.size() ) {
			// �I������������ւ̃|�C���^�𐔎��Ƃ��āApush�p�Ƃ��Ċi�[
			iPushFilePointerAddressOfSaveFileName = (int)szMultiSaveDataFileName[iSelect];

			// �Z�[�u�f�[�^�̃t�@�C����
			psznb6savedatafilename = (char *)iPushFilePointerAddressOfSaveFileName+2; // �Z�[�u�f�[�^�G�f�B�^�p�B�t�@�C���������Ȃ̂�+2

		}
	}
}



/*
00479C3B  |. 68 708D4C00    PUSH    TENSHOU.004C8D70                 ;  ASCII "A:SAVEDAT.N6P"
				�������� JMP TSMod.OnTenshouExePushSaveFileName �Ə��������Ă����ۂ̏���
00479C40  |. 56             PUSH    ESI
00479C41  |. E8 A7350300    CALL    TENSHOU.004AD1ED
00479C46  |. 83C4 08        ADD     ESP, 8
*/
int pTenshouExeJumpFromToOnTenshouExePushSaveFileName	  = 0x479C3B; // �֐��͂��̃A�h���X����AOnTenshouExePushSaveFileName�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExePushSaveFileName  = 0x479C40; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExePushSaveFileName() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	OnTenshouExePushSaveFileNameExecute();

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

		// ���Xtenshou.exe �ɂ����������������ɁB
		PUSH    iPushFilePointerAddressOfSaveFileName

		jmp pTenshouExeReturnLblFromOnTenshouExePushSaveFileName
	}
}




char cmdOnTenshouExeJumpFromPushSaveFileName[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v5�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExePushSaveFileName() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExePushSaveFileName;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExePushSaveFileName + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExePushSaveFileName  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromPushSaveFileName+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExePushSaveFileName), cmdOnTenshouExeJumpFromPushSaveFileName, 5, NULL); //5�o�C�g�̂ݏ�������
}




