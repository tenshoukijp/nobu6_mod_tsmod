#include <string.h>
#include <string.h>
#include <stdio.h>

#pragma warning(push)
#pragma warning(disable:4996)



void strchg(char *buf, const char *str1, const char *str2) { 

	char tmp[1024 + 1];

	char *p;

	/* ������Ȃ��Ȃ�܂ŌJ��Ԃ�
 	   p�͋�������̐擪���w���Ă��� */
	while ((p = strstr(buf, str1)) != NULL) {

		// ���̕��������������̒��O�ŋ�؂��� 
		*p = '\0';

		// �|�C���^����������̎��̕�����
		p += strlen(str1);

		// �������񂩂���ۑ�/  
		strcpy(tmp, p);  

		// �V����������̌�ɂȂ�
		strcat(buf, str2); 

		// ����Ɏc����Ȃ�/ 
		strcat(buf, tmp);
	}
}





char param_array[][10] = { "%s", "%u", "%2u", "%3u", "%4u", "%02u", "%03u", "%04u", "%d", "%02d", "%03d", "%04d", "%2d", "%3d", "%4d" };


// �����񒆂�format�^�p�����^���Ƃ肠�����A$$$�ɏC������B
void replaceParamToThreeDoll(char *szOrgPrmMsg) {
	for ( int i=0; i<sizeof(param_array)/sizeof(param_array[0]); i++) {
		strchg( szOrgPrmMsg, param_array[i], "$$$" );
	}
}

// �����񒆂�$$$�����邩�ǂ����B
char * isIncludeParameter(char *szTargetMsg) {
	return strstr( szTargetMsg, "$$$" );
}

// ���̕������܂܂�邩�ǂ����B
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


// �Ώۂ̊������ꂽ������(szVerifyMsg)�̌��̃p�����^���b�Z�[�W��(szParamMsg)���m���炵�����ǂ����B
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



// �ȏ�uC4996�v�̃��[�j���O�̔�������R�[�h�������Ă��A�R���p�C�����ɂ͏o�͂���Ȃ��Ȃ�B
#pragma warning(pop)