#pragma once

/**
* http://www.tietew.jp/cppll/archive/598 ���
*
* ������O�� STDX_DSTREAM_BUFFERING �� define ����Ă���ƁC
* �f�t�H���g�Ńo�b�t�@�����O���s���D
*
* ����Ȋ����Ŏg���܂��B
* int main(int argc, char **argv)
* {
*   stdx::dstream ds;
*   ds << "�т�`��" << std::endl;
* }
*/
#include "OutputDebugStream.h"

stdx::dbg_ostream OutputDebugStream;

char szBufOutputDebugStream[4096] = "";