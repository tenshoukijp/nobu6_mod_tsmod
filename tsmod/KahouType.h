#pragma once

#pragma pack(1)


extern CUSTOM_KAHOU_TYPE_STRING nb6kahou_custom_type_string[GAMEDATASTRUCT_KAHOU_NUM];

// 家宝種類のタイプのカスタム文字列へのポインターを得る
char *GetOverWriteKahouTypeMeiPointer(int iKahouID);