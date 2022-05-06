#pragma once



#include <istream>
#include <ostream>
#include <sstream>
#include "WinTarget.h"
#include <tchar.h>


// ｢C4996」のワーニングの発生するコードがあっても、コンパイル時には出力されなくなる。
#pragma warning(push)
#pragma warning(disable:4996)

// OutputDebugStringへとフォーマット(printf)形式を利用して出力する。
extern char szBufOutputDebugStream[4096];

namespace stdx {

	// VC++ で STLport だと using std::char_traits; みたいなのが必要かも
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
			// flush(); // 不要らしい．http://www.tietew.jp/cppll/archive/607
			delete rdbuf();
		}

		// 特別に付け加え。OutputDebugStream(format, ...)のprintf関数表記的な感じを実現するため。
		void operator()( const char *format, ... ) {

			va_list arg;

			va_start(arg, format);
			vsprintf_s(szBufOutputDebugStream, format, arg);
			va_end(arg);

			// 最後の文字が改行ではない場合
			if ( szBufOutputDebugStream[strlen(szBufOutputDebugStream)-1] != '\n' ) {
				// 改行を足す。
				strcat( szBufOutputDebugStream, "\n");
			}
			
			// OutputDebugStringの方に出す。
			OutputDebugString( szBufOutputDebugStream );

		}
	};





	typedef basic_dbg_streambuf<_TCHAR> dbg_streambuf;
	typedef basic_dbg_ostream<_TCHAR> dbg_ostream;

	// これを定義しておくと， dout の宣言がいらなくなる．
	//static dstream dout;

}


using namespace stdx;

// 以下の変数が｢cout｣相当。シングルトン前提
extern stdx::dbg_ostream OutputDebugStream;


// 以上「C4996」のワーニングの発生するコードがあっても、コンパイル時には出力されなくなる。
#pragma warning(pop)