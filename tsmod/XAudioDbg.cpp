#include <stdio.h>
#include <stdarg.h>
#include <windows.h>

#include "XAudioDbg.h"


void XAUDIO2_OUTDBG( const char *format, ... ) {
#ifdef DEBUG_XAUDIO2
    char szBufDebug[4096] = "";

    va_list arg;

    va_start(arg, format);
    vsprintf_s(szBufDebug, format, arg);
    va_end(arg);

    OutputDebugString( szBufDebug );
#endif
}

