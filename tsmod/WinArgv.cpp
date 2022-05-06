

#include <tchar.h>
#include "WinTarget.h"
#include <shellapi.h>


#ifndef UNICODE

#include <stdlib.h>
#include <winnls.h>


/** char文字列をWCHARに変換する */
static LPWSTR atow(LPCSTR src)
{
	LPWSTR buf;
	int dst_size, rc;

	rc = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0);
	if (rc == 0) {
		return NULL;
	}

	dst_size = rc + 1;
	buf = (LPWSTR) malloc(sizeof(WCHAR) * dst_size);
	if (buf == NULL) {
		return NULL;
	}

	rc = MultiByteToWideChar(CP_ACP, 0, src, -1, buf, dst_size);
	if (rc == 0) {
		free(buf);
		return NULL;
	}
	buf[rc] = L'\0';

	return buf;
}

/** WCHAR文字列をcharに変換する */
static LPSTR wtoa(LPCWSTR src)
{
	LPSTR buf;
	int dst_size, rc;

	rc = WideCharToMultiByte(CP_ACP, 0, src, -1, NULL, 0, NULL, NULL);
	if (rc == 0) {
		return NULL;
	}

	dst_size = rc + 1;
	buf = (LPSTR) malloc(dst_size);
	if (buf == NULL) {
		return NULL;
	}

	rc = WideCharToMultiByte(CP_ACP, 0, src, -1, buf, dst_size, NULL, NULL);
	if (rc == 0) {
		free(buf);
		return NULL;
	}
	buf[rc] = '\0';

	return buf;
}
#endif /* ndef UNICODE */

// ********************************************************* 
// コマンドライン引数を解析する
// ********************************************************* 
LPTSTR *parse_args(LPCTSTR args_t, int *argc)
{
	LPCWSTR args_w;
	LPWSTR *argv_w;

	if (args_t[0] == _T('\0')) {
		*argc = 0;
		return NULL;
	}

#ifdef UNICODE
	args_w = args_t;
#else
	/* 引数文字列をWCHARに変換 */
	args_w = (LPCWSTR) atow(args_t);
	if (args_w == NULL) {
		return NULL;
	}
#endif

	/* パースする（WCHAR用のAPIしか用意されていない模様） */
	argv_w = CommandLineToArgvW(args_w, argc);

#ifdef UNICODE
	return argv_w;
#else
	free((void *) args_w);
	if (argv_w == NULL) {
		return NULL;
	}

	{
		LPSTR *argv_c = NULL;
		int i, j;

		/* パース結果はWCHARなので、char型に変換する必要がある */
		argv_c = (LPSTR *) malloc(sizeof(argv_c[0]) * (*argc + 1));
		if (argv_c == NULL) {
			goto DONE;
		}
		for (i = 0; i < *argc; ++i) {
			argv_c[i] = wtoa(argv_w[i]);
			if (argv_c[i] == NULL) {
				for (j = 0; j < i; ++j) {
					free(argv_c[j]);
				}
				free(argv_c);
				argv_c = NULL;
				goto DONE;
			}
		}
		argv_c[i] = NULL;

DONE:
		(void) LocalFree((HLOCAL) argv_w);
		return argv_c;
	}
#endif
}

// ********************************************************* 
// parse_args()で取得したコマンドライン引数を削除する
// *********************************************************
void free_args(int argc, LPTSTR *argv)
{
#ifdef UNICODE
	(void) argc;
	(void) LocalFree((HLOCAL) argv);
#else
	int i;

	for (i = 0; i < argc; ++i) {
		free(argv[i]);
	}
	free(argv);
#endif
}




