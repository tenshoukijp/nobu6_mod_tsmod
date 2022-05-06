#pragma once

#include <windows.h>

// フルパス名からファイル名だけを得る。
// (元文字列のポインタ示してるだけなので、元の文字列の存在が必須なので気を付けること)
char *GetFileNameWithOutDirName(char *lpszPath);