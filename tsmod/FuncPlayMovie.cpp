#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"

#include "FuncPlayMovie.h"
#include "GameDataStruct.h"
#include "SetExtensionMovie.h"
#include "OnigRegex.h"
#include "time.h"


int pCallFromFuncPlayMovie = 0x43CD64;
void FuncPlayMovieAsm() {
	__asm {
		PUSH    1
		PUSH    1						// ���ꂪ���[�r�[�ԍ��ł���B
		CALL    pCallFromFuncPlayMovie
		ADD     ESP, 8
	}
}

char *pOriginalAdditionalPoiter = NULL;
BOOL isMovieWithBlackBack = TRUE; // �@�u���b�N�o�b�N����ׂ��łȂ��B�f�t�H���g�ł̓u���b�N�o�b�N����̂ŁA�����FALSE
BOOL isMovieWithBGMSTOP = TRUE;	 // �@�a�f�l�͎~�߂�ׂ��łȂ��B�f�t�H���g�ł͂a�f�l�͎~�߂�̂ŁA�����FALSE

void FuncPlayMovie(string strMovie, BOOL isBlackBack, BOOL isBGMStop) {

	// �I���W�i���̂P�Ԗ�(�R�[�G�[���S)�̃��[�r�[�|�C���^���T���Ă���
	pOriginalAdditionalPoiter = nb6_moviepointer_ex[1].pAddressMovieName;

	// �U���Ă���������̃|�C���^���P�Ԗڂ̃��[�r�[�Ƃ��ăZ�b�g
	nb6_moviepointer_ex[1].pAddressMovieName = (char *)strMovie.c_str();

	isMovieWithBlackBack = isBlackBack; // �u���b�N�o�b�N���邩�ǂ������㏑������B
	isMovieWithBGMSTOP = isBGMStop; // �a�f�l���~�߂邩�ǂ������㏑������B

	FuncPlayMovieAsm(); // ���[�r�[���Đ�����B
	// �Đ����I������̂Ń|�C���^�����ւƖ߂�

	isMovieWithBlackBack = TRUE; // �u���b�N�o�b�N�������ԂւƖ߂��B
	isMovieWithBGMSTOP = TRUE; // �a�f�l���~�߂��ԂւƖ߂��B

	// ���[�r�[�̖��O�����ւƖ߂��B
	nb6_moviepointer_ex[1].pAddressMovieName = pOriginalAdditionalPoiter;

}



// ����w�肪�A���K�\���p�^�[���}�b�`�ōs���Ă���ꍇ�́A�P��I������BHDD����movie�t�H���_�����ΏۂƂȂ�B
string ChoiceMovieFromRegex(string strMovieRegexPattern) {

	// �ΏۂƂȂ郀�[�r�[�̃��X�g
	vector<string> movie_list;

	WIN32_FIND_DATA ffd;

	// movie�t�H���_�ɂ���*.avi�Ƃ������O�̃t�@�C���B
	HANDLE hFileList = FindFirstFile("movie\\*.avi", &ffd);

	// ���݂����ꍇ�A�ǂݍ��ݑΏۂƂ���
	if ( hFileList != INVALID_HANDLE_VALUE ) {

		do {
			// �����h��h�~�̂��߁A������������B
			char filename[512] = "";
			strcpy(filename, _strlwr(ffd.cFileName));

			Matches movie_matches;

			// ".avi"�̕������폜
			if ( strlen(filename)-4 >= 0 ) {
				filename[strlen(filename)-4] = NULL; // �Ō��.avi���폜
			}

			// �p�^�[���Ƀ}�b�`������̂�T��
			if ( OnigMatch( filename, strMovieRegexPattern, &movie_matches ) ) {

				// �}�b�`������̂̓��X�g�ɒǉ�
				movie_list.push_back(string(filename));
			} 

		} while (FindNextFile(hFileList, &ffd));
	}
	FindClose(hFileList);

	// nb6moviepointer_ex��0�Ԗڂ�NULL�Ȃ̂ŁA1����X�^�[�g�B
	for ( int i=1; i< GAMEDATASTRUCT_MOVIE_NUM; i++ ) {
		// �b�c�|�q�n�l�̃��[�r�[���X�g�ƃ}�b�`������A�������Ԃ��B
		// �p�^�[���Ƀ}�b�`������̂�T��
		if ( OnigMatch( nb6_moviepointer_ex[i].pAddressMovieName, strMovieRegexPattern ) ) {

			// �}�b�`������̂̓��X�g�ɒǉ�
			movie_list.push_back(string(nb6_moviepointer_ex[i].pAddressMovieName));
		}
	}

	// �p�^�[���Ƀ}�b�`������̂��P���Ȃ���΁A
	if ( movie_list.size() == 0 ) {

		// �Ή�������̂����������ꍇ�́Astring���󑊓��ɂ���B
		return string("");

	} else {
		int irandom = rand() % movie_list.size();

		return movie_list[irandom];
	}
}


// �O������̌Ăяo���p�̌��B
void WINAPI Extern_FuncPlayMovie(char *szMovieName, int iOptions) {

	// ���[�r�[�̎w��͐��K�\���p�^�[����������Ȃ��̂ŁA���ꂪ���邩�ǂ����`�F�b�N�B
	string strMovieRegexPattern = ChoiceMovieFromRegex(szMovieName);

	// �X�g�����O���󑊓���������A���[�r�[�͍Đ����Ȃ��B�s���ȏꍇ��(ChoiceMovieFromRegex)���ŋ󑊓��ɂȂ�B
	if ( strMovieRegexPattern.empty() ) { return; }

	// ���o�b�N�n��ScenarioMod����n���Ă���I�v�V�����B�r�b�g���Z���Ēl�𔽑΂ɂ���K�v������B
	int isBlackBackOption = !(iOptions & 0x01);
	// �a�f�l��~�n��ScenarioMod����n���Ă���I�v�V�����B�r�b�g���Z���Ēl�𔽑΂ɂ���K�v������B
	int isBGMSTOPOption = !(iOptions & 0x02);

	FuncPlayMovie(strMovieRegexPattern, isBlackBackOption, isBGMSTOPOption);
}