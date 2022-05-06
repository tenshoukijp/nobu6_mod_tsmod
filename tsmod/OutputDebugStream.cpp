#pragma once

/**
* http://www.tietew.jp/cppll/archive/598 より
*
* これより前に STDX_DSTREAM_BUFFERING が define されていると，
* デフォルトでバッファリングを行う．
*
* こんな感じで使います。
* int main(int argc, char **argv)
* {
*   stdx::dstream ds;
*   ds << "びよ～ん" << std::endl;
* }
*/
#include "OutputDebugStream.h"

stdx::dbg_ostream OutputDebugStream;

char szBufOutputDebugStream[4096] = "";