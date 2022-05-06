#pragma once

#pragma pack(1)

#include "WinTarget.h"

// ********************************************************* 
// コマンドライン引数を解析する
// ********************************************************* 
LPTSTR *parse_args(LPCTSTR args_t, int *argc);


// ********************************************************* 
// parse_args()で取得したコマンドライン引数を削除する
// *********************************************************
void free_args(int argc, LPTSTR *argv);
