#pragma once

#pragma pack(1)

#include <string>
using namespace std;

extern BOOL isMovieWithBlackBack;
extern BOOL isMovieWithBGMSTOP;

void WINAPI Extern_FuncPlayMovie(char *szMovieName, int iOptions);

// �w��̃��[�r�[���Đ�����B�f�t�H���g�Ńu���b�N�o�b�N�B�f�t�H���g��BGM�͎~�܂�
void FuncPlayMovie(string strMovie, BOOL isBlackBack=TRUE, BOOL isBGMStop=TRUE);