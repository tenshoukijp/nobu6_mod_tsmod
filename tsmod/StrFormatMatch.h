#pragma once

#pragma pack(1)

#include <string.h>

// ������̈ꕔ�𑼂̕�����Œu��������
// buf:������  str1:�u��������Ώە����� str2:�u����������̕�����
void strchg(char *buf, const char *str1, const char *str2);

// �Ώۂ�szVerifyMsg���AszParamMsg�̃p�����[�^�ɒl������`�̃��b�Z�[�W�Ǝv����ꍇ��true
// szVerify:�u�������������������v�AszParam:�%d������%s�v�Ȃ�true
bool isTheParamMsgIsIncludeVerifyMsg( char *szVerifyMsg, char *szParamMsg );