#include <string.h>
#include <string.h>
#include <stdio.h>

#pragma warning(push)
#pragma warning(disable:4996)



void strchg(char *buf, const char *str1, const char *str2) { 

	char tmp[1024 + 1];

	char *p;

	/* 見つからなくなるまで繰り返す
 	   pは旧文字列の先頭を指している */
	while ((p = strstr(buf, str1)) != NULL) {

		// 元の文字列を旧文字列の直前で区切って 
		*p = '\0';

		// ポインタを旧文字列の次の文字へ
		p += strlen(str1);

		// 旧文字列から後を保存/  
		strcpy(tmp, p);  

		// 新文字列をその後につなぎ
		strcat(buf, str2); 

		// さらに残りをつなぐ/ 
		strcat(buf, tmp);
	}
}





char param_array[][10] = { "%s", "%u", "%2u", "%3u", "%4u", "%02u", "%03u", "%04u", "%d", "%02d", "%03d", "%04d", "%2d", "%3d", "%4d" };


// 文字列中のformat型パラメタをとりあえず、$$$に修正する。
void replaceParamToThreeDoll(char *szOrgPrmMsg) {
	for ( int i=0; i<sizeof(param_array)/sizeof(param_array[0]); i++) {
		strchg( szOrgPrmMsg, param_array[i], "$$$" );
	}
}

// 文字列中に$$$があるかどうか。
char * isIncludeParameter(char *szTargetMsg) {
	return strstr( szTargetMsg, "$$$" );
}

// 元の文字が含まれるかどうか。
bool isTheMsgIsWsprintFormatOf( char *szVerifyMsg, char *szParamMsg ) {
    char *tp = strtok( szParamMsg, "$$$" );

	if ( !strstr( szVerifyMsg, tp ) ) {	
		return false;
	}

	while ( tp != NULL ) {
		tp = strtok( NULL,"$$$" );
			
		if ( tp && !strstr( szVerifyMsg, tp ) ) {	
			return false;
		}
    }
        
	return true;
}


// 対象の完成された文字列(szVerifyMsg)の元のパラメタメッセージは(szParamMsg)が確からしいかどうか。
bool isTheParamMsgIsIncludeVerifyMsg( char *szVerifyMsg, char *szParamMsg ) {

	if ( szVerifyMsg == NULL || szParamMsg == NULL ) { return false; }
	if ( szVerifyMsg[0] == NULL || szParamMsg[0] == NULL ) { return false; }

	char szParamedMsg[512] = "";
	strcpy( szParamedMsg, szParamMsg );

	replaceParamToThreeDoll( szParamedMsg );
	if ( isIncludeParameter( szParamedMsg ) ) {
		if ( isTheMsgIsWsprintFormatOf ( szVerifyMsg, szParamedMsg ) ) {
			return true;
		}
	}

	return false;
}



// 以上「C4996」のワーニングの発生するコードがあっても、コンパイル時には出力されなくなる。
#pragma warning(pop)