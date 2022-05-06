#pragma once

#pragma pack(1)

#include <string>
using namespace std;

extern BOOL isMovieWithBlackBack;
extern BOOL isMovieWithBGMSTOP;

void WINAPI Extern_FuncPlayMovie(char *szMovieName, int iOptions);

// 指定のムービーを再生する。デフォルトでブラックバック。デフォルトでBGMは止まる
void FuncPlayMovie(string strMovie, BOOL isBlackBack=TRUE, BOOL isBGMStop=TRUE);