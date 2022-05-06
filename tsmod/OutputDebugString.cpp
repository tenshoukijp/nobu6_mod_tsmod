#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"
#include <string>
#include "TSModCommand.h"

// OutputDebugStringへとフォーマット(printf)形式を利用して出力する。
char szBufOutputDebugString[4096] = "";
void OutputDebugStringF( const char *format, ... ) {

	va_list arg;

	va_start(arg, format);
	vsprintf_s(szBufOutputDebugString, format, arg);
	va_end(arg);

	// 最後の文字が改行ではない場合
	if ( szBufOutputDebugString[strlen(szBufOutputDebugString)-1] != '\n' ) {
		// 改行を足す。
		strcat( szBufOutputDebugString, "\n");
	}

	// OutputDebugStringの方に出す。
	OutputDebugString( szBufOutputDebugString );

	// TSMod自信のデバッグコンソールがオープンしていたら、そちらにも書き込み
	if ( TSModCommand::Debug::cmdDebugWindowOpen == 1) {
		printf( szBufOutputDebugString );
	}
}
