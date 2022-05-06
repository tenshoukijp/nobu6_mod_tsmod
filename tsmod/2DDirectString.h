#pragma once

#include "2DPosition.h"
#include <string>

using namespace std;


// ベクトルを文字列化するが、東西南北系（８方位）で文字列化。
string get８方位文字列(D2D_VECTOR v);


// ベクトルを文字列化するが、東西南北系（４方位）で文字列化。
string get４方位文字列(D2D_VECTOR v);
