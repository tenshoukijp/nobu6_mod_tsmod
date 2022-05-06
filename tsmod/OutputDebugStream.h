#pragma once



#include <istream>
#include <ostream>
#include <sstream>
#include "WinTarget.h"
#include <tchar.h>


// �C4996�v�̃��[�j���O�̔�������R�[�h�������Ă��A�R���p�C�����ɂ͏o�͂���Ȃ��Ȃ�B
#pragma warning(push)
#pragma warning(disable:4996)

// OutputDebugString�ւƃt�H�[�}�b�g(printf)�`���𗘗p���ďo�͂���B
extern char szBufOutputDebugStream[4096];

namespace stdx {

	// VC++ �� STLport ���� using std::char_traits; �݂����Ȃ̂��K�v����
	template <typename Ch_T, typename Tr_T = std::char_traits<Ch_T> >
	class basic_dbg_streambuf: public std::basic_stringbuf<Ch_T, Tr_T> {
	public:
		basic_dbg_streambuf() {
#ifndef STDX_DSTREAM_BUFFERING
			setbuf(0,0);
#endif
		}

		virtual ~basic_dbg_streambuf() {
			sync();
		}

	protected:
		int sync(void) {
			dbg_out(str().c_str());
			pbump(static_cast<int>(pbase() - pptr()));
			return 0;
		}

		void dbg_out(const Ch_T*);
	};

	template <>
	inline void basic_dbg_streambuf<char>::dbg_out(const char *str)	{
		::OutputDebugStringA(str);
	}

	template <>
	inline void basic_dbg_streambuf<wchar_t>::dbg_out(const wchar_t *str) {
		::OutputDebugStringW(str);
	}

	template <typename Ch_T, typename Tr_T = std::char_traits<Ch_T> >
	class basic_dbg_ostream: public std::basic_ostream<Ch_T, Tr_T> {
	public:
		basic_dbg_ostream() : std::basic_ostream<Ch_T, Tr_T>(new basic_dbg_streambuf<Ch_T, Tr_T>())	{
		}

		virtual ~basic_dbg_ostream() {
			// flush(); // �s�v�炵���Dhttp://www.tietew.jp/cppll/archive/607
			delete rdbuf();
		}

		// ���ʂɕt�������BOutputDebugStream(format, ...)��printf�֐��\�L�I�Ȋ������������邽�߁B
		void operator()( const char *format, ... ) {

			va_list arg;

			va_start(arg, format);
			vsprintf_s(szBufOutputDebugStream, format, arg);
			va_end(arg);

			// �Ō�̕��������s�ł͂Ȃ��ꍇ
			if ( szBufOutputDebugStream[strlen(szBufOutputDebugStream)-1] != '\n' ) {
				// ���s�𑫂��B
				strcat( szBufOutputDebugStream, "\n");
			}
			
			// OutputDebugString�̕��ɏo���B
			OutputDebugString( szBufOutputDebugStream );

		}
	};





	typedef basic_dbg_streambuf<_TCHAR> dbg_streambuf;
	typedef basic_dbg_ostream<_TCHAR> dbg_ostream;

	// ������`���Ă����ƁC dout �̐錾������Ȃ��Ȃ�D
	//static dstream dout;

}


using namespace stdx;

// �ȉ��̕ϐ����cout������B�V���O���g���O��
extern stdx::dbg_ostream OutputDebugStream;


// �ȏ�uC4996�v�̃��[�j���O�̔�������R�[�h�������Ă��A�R���p�C�����ɂ͏o�͂���Ȃ��Ȃ�B
#pragma warning(pop)