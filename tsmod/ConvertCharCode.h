#pragma once
#pragma pack(1)

#include <string>

// CP932(SJIS)��UTF8�ւ̕ϊ�
std::string cp932_to_utf8(const std::string &text);

// UTF8��CP932(SJIS)�ւ̕ϊ�
std::string utf8_to_cp932(const std::string &text);