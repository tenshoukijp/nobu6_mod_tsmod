#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#include <vector>
#include <string>

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "FuncPlayMovie.h"
#include "OnigRegex.h"
#include "FuncFadeInOut.h"

using namespace std;

/*
 * �V�i���I�喼�I�����̏���
 */
bool isEnteringFromScenarioDaimyoExecute = false;

void OnTenshouExePlayScenarioDetailMovie() ;
void OnTenshouExeEnteringFromScenarioDaimyoExecute() {

	isEnteringFromScenarioDaimyoExecute = true;
}




/*
0049D814  |. 68 00040000    |PUSH    400
0049D819  |. E8 9597FCFF    |CALL    TENSHOU.00466FB3
				�������� JMP TSMod.OnTSExeEnteringFromScenarioDaimyo �Ə��������Ă����ۂ̏���
0049D81E  |. 83C4 04        |ADD     ESP, 4
*/
int pTenshouExeJumpFromToOnTenshouExeEnteringFromScenarioDaimyo	 =0x49D819; // �֐��͂��̃A�h���X����AOnTenshouExeEnteringFromScenarioDaimyo�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeEnteringFromScenarioDaimyo =0x466FB3; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeEnteringFromScenarioDaimyo  =0x49D81E; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeEnteringFromScenarioDaimyo() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeEnteringFromScenarioDaimyo

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
	OnTenshouExeEnteringFromScenarioDaimyoExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeEnteringFromScenarioDaimyo
	}
}





char cmdOnTenshouExeJumpFromEnteringFromScenarioDaimyo[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeEnteringFromScenarioDaimyo() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeEnteringFromScenarioDaimyo;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeEnteringFromScenarioDaimyo + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeEnteringFromScenarioDaimyo  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromEnteringFromScenarioDaimyo+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeEnteringFromScenarioDaimyo), cmdOnTenshouExeJumpFromEnteringFromScenarioDaimyo, 5, NULL); //5�o�C�g�̂ݏ�������
}




//----------------------------------------------------------------------------------------------------------------------
//------------------------------------------
// ��̂�����B�����́A�V�i���I�I�����s�����ʂƃ��[�h���Ƌ��ʂŗ���Ă���B�t�F�C�h�A�E�g���O�ƂȂ�B

void OnTenshouExePlayScenarioDetailMovie(); // �V�i���I����p�̓�����Đ�����B

void OnTenshouExeEnteringAfterScenarioDaimyoExecute() {

	// ���̃^�C�~���O�ŁA�V�i���I�I���������Ă����ꍇ�̂݃V�i���I������[�r�[�𗬂��B
	if ( isEnteringFromScenarioDaimyoExecute ) {
		OnTenshouExePlayScenarioDetailMovie();
	}
}

extern int iSelectingScenarioLocation; // �V�i���I�̈ʒu�̔ԍ��B�ォ��O�D��ԉ����T

void OnTenshouExePlayScenarioDetailMovie() {

	// �V�i���I�̈ʒu�ԍ��������Ȕ͈͂ɂ��邩�H
	int iSnNo = iSelectingScenarioLocation+1;
	
	if ( 1 <= iSnNo && iSnNo <= 6 ) {

		bool isScenarioMoviePlay = false; // �t�F�C�h�A�E�g�̂��߁B
		// --------------------------------------------------
		// �܂��̓V�i���I�ŗL��(SN*_OPENING.avi)���Đ�����B
		struct stat  st;

		char szMovieName[256] = "";
		wsprintf(szMovieName, "SN%d_OPENING", iSnNo);

		char szMovieFullName[256] = "";
		wsprintf(szMovieFullName, "Movie\\SN%d_OPENING.avi", iSnNo);

		// ���̃��[�r�[�̓n�[�h�f�B�X�N�ɑ��݂��邩
		if (stat(szMovieFullName, &st) == 0) {
			FuncPlayMovie(szMovieName);
			isScenarioMoviePlay = true;
		}


		// --------------------------------------------------
		// ���ɃV�i���I�ŗL��(SN*_RANDOM??.avi)�A���ŁA�V�i���I���ʂ�(OPENING_SN_RND??.avi)���Đ�����B
		
		vector<string> ident_scenario_movie_file; // ����̃V�i���I�̒��ł̃����_���Đ��t�@�C��
		vector<string> common_scenario_movie_file; // ����̃V�i���I�̒��ł̃����_���Đ��t�@�C��

		WIN32_FIND_DATA ffd;
		// OPENING_SN*.avi�Ƃ������O�̃t�@�C���B
		HANDLE hFileList = FindFirstFile("movie\\*.avi", &ffd);

		// ���݂����ꍇ�A�ǂݍ��ݑΏۂƂ��邪�A���K�\���Ń`�F�b�N�B
		if ( hFileList != INVALID_HANDLE_VALUE ) {

			do {
				char filename[512] = "";
				strcpy(filename, _strlwr(ffd.cFileName));
				char szSnNo[5] = "";
				_itoa( iSnNo, szSnNo,10 );

				// ����̃V�i���I�p�̃����_�����[�r�[
				string strIdentMovieRegex = string("^(sn")+string(szSnNo)+string("_random.+?).avi$");

				// �V�i���I�p�ɂ������Ȃ����ʂ̃����_�����[�r�[
				string strCommonMovieRegex = string("^(sn")+string("_random.+?).avi$");

				Matches movie_matches;
				// ����̃V�i���I�p�̃t�@�C�����̐��K�\���p�^�[���𒲂ׂ�
				if ( OnigMatch( filename, strIdentMovieRegex, &movie_matches ) ) {
					// ���݂��Ă����Ȃ�A�ǂ񂶂�����킯�ł��āA��ő������邽�ߓo�^���Ă����B
					ident_scenario_movie_file.push_back(string(movie_matches[1]));
				} 

				// �V�i���I�p�ɂ������Ȃ����ʂ̃����_���̃t�@�C�����̐��K�\���p�^�[���𒲂ׂ�
				if ( OnigMatch( filename, strCommonMovieRegex, &movie_matches ) ) {
					// ���݂��Ă����Ȃ�A�ǂ񂶂�����킯�ł��āA��ő������邽�ߓo�^���Ă����B
					common_scenario_movie_file.push_back(string(movie_matches[1]));
				}

			} while (FindNextFile(hFileList, &ffd));
		}
		FindClose(hFileList);

		// ����̃V�i���I�p�̃����_�����[�r�[(SN*_RANDOM??.avi)���Đ�����B
		if ( ident_scenario_movie_file.size() > 0 ) {
			int irand = rand() % ident_scenario_movie_file.size(); // �傫�Ȑ��������ɑ΂��āA�Y���̐��K�\�������Ă͂܂������[�r�[�̌��̗]�肪�����ƂȂ�B
			FuncPlayMovie( ident_scenario_movie_file[irand] ); // �����_���ōĐ�

			isScenarioMoviePlay = true;
		}

		// --------------------------------------------------
		// ���ɃV�i���I���ʂ�(SN_RANDOM.avi)���Đ�����B
		if ( common_scenario_movie_file.size() > 0 ) {
			int irand = rand() % common_scenario_movie_file.size(); // �傫�Ȑ��������ɑ΂��āA�Y���̐��K�\�������Ă͂܂������[�r�[�̌��̗]�肪�����ƂȂ�B
			FuncPlayMovie( common_scenario_movie_file[irand] ); // �����_���ōĐ�

			isScenarioMoviePlay = true;
		}

		// ���[�r�[���Đ����Ă���ƁA�t�F�C�h�A�E�g���Ă��Ă��A�p�b�Ɖ�ʂ��o�Ă��܂��̂ŁA���߂ăt�F�C�h�A�E�g�B
		if ( isScenarioMoviePlay ) {
			FuncFadeOut();
		}
	}
}



/*
0049D81E  |. 83C4 04        |ADD     ESP, 4
0049D821  |> E8 C85AF8FF    |CALL    TENSHOU.004232EE
				�������� JMP TSMod.OnTSExeEnteringAfterScenarioDaimyo �Ə��������Ă����ۂ̏���
0049D826  |. 8D4D A4        |LEA     ECX, DWORD PTR SS:[EBP-5C]
*/
int pTenshouExeJumpFromToOnTenshouExeEnteringAfterScenarioDaimyo	 =0x49D821; // �֐��͂��̃A�h���X����AOnTenshouExeEnteringAfterScenarioDaimyo�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeEnteringAfterScenarioDaimyo =0x4232EE; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeEnteringAfterScenarioDaimyo  =0x49D826; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeEnteringAfterScenarioDaimyo() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeEnteringAfterScenarioDaimyo // ����͎�����t�F�C�h�A�E�g

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
	OnTenshouExeEnteringAfterScenarioDaimyoExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeEnteringAfterScenarioDaimyo
	}
}



char cmdOnTenshouExeJumpFromEnteringAfterScenarioDaimyo[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeEnteringAfterScenarioDaimyo() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeEnteringAfterScenarioDaimyo;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeEnteringAfterScenarioDaimyo + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeEnteringAfterScenarioDaimyo  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromEnteringAfterScenarioDaimyo+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeEnteringAfterScenarioDaimyo), cmdOnTenshouExeJumpFromEnteringAfterScenarioDaimyo, 5, NULL); //5�o�C�g�̂ݏ�������
}

