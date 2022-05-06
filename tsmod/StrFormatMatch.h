#pragma once

#pragma pack(1)

#include <string.h>

// 文字列の一部を他の文字列で置き換える
// buf:文字列  str1:置き換える対象文字列 str2:置き換えた後の文字列
void strchg(char *buf, const char *str1, const char *str2);

// 対象のszVerifyMsgが、szParamMsgのパラメータに値を入れる形のメッセージと思われる場合にtrue
// szVerify:「いいいあああおおお」、szParam:｢%dあああ%s」ならtrue
bool isTheParamMsgIsIncludeVerifyMsg( char *szVerifyMsg, char *szParamMsg );