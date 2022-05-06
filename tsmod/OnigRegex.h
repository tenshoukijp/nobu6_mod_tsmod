#pragma once

#include <vector>
#include <map>
#include "WinTarget.h"

#include "oniguruma/oniguruma.h"

using namespace std;

typedef map<int, string> Matches;


/*
	Matches matches;
	if ( PregMatch("能力と表現", "(能)(.+)(現)", &matches) ) {
		OutputDebugStream << "0:" << matches[0] << ",1:" << matches[1] << ",2:" << matches[2] << endl;
	}		
 */
int OnigMatch( string str_target, string str_regex, Matches* pmatches=NULL );


// 上記関数の外部参照用
int WINAPI Extern_OnigMatchAlloc(char *str_target, char *str_regex, int *pmatches);

// 基礎型しかないので、手動で解法用。外部参照が正しく解放してくれること前提
void WINAPI Extern_OnigMatchFree(int *pmatches);
