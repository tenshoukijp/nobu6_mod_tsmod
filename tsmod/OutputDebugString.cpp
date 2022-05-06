#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"
#include <string>
#include "TSModCommand.h"

// OutputDebugString�ւƃt�H�[�}�b�g(printf)�`���𗘗p���ďo�͂���B
char szBufOutputDebugString[4096] = "";
void OutputDebugStringF( const char *format, ... ) {

	va_list arg;

	va_start(arg, format);
	vsprintf_s(szBufOutputDebugString, format, arg);
	va_end(arg);

	// �Ō�̕��������s�ł͂Ȃ��ꍇ
	if ( szBufOutputDebugString[strlen(szBufOutputDebugString)-1] != '\n' ) {
		// ���s�𑫂��B
		strcat( szBufOutputDebugString, "\n");
	}

	// OutputDebugString�̕��ɏo���B
	OutputDebugString( szBufOutputDebugString );

	// TSMod���M�̃f�o�b�O�R���\�[�����I�[�v�����Ă�����A������ɂ���������
	if ( TSModCommand::Debug::cmdDebugWindowOpen == 1) {
		printf( szBufOutputDebugString );
	}
}
