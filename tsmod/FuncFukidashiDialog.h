#pragma once

#pragma pack(1)

// 吹き出しオープン
void FuncLeftNormalFukidashiMessage(int iBushouID, char *szMessage);
void FuncRightNormalFukidashiMessage(int iBushouID, char *szMessage);
// 吹き出しの更新
void FuncRightNormalFukidashiUpdate(int iBushouID, char *szMessage);
void FuncLeftNormalFukidashiUpdate(int iBushouID, char *szMessage);
// 吹き出し閉じる
void FuncLeftNormalFukidashiDelete();
void FuncRightNormalFukidashiDelete();

/*
void FuncCenterNormalFukidashiMessage(int iBushouID, char *szMessage);
*/