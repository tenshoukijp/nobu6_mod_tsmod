#pragma once;

// 自分自身のタイムスタンプをチェックする。

// タイムスタンプ修正ツールの名前
#define TIMESTAMPERFILENAME		"TimeStamper.exe"
#define TIMESTAMPERFILEOPTION	"-silent"

// タイムスタンプが定番シリーズと異なっているかあっているか。
extern int isTimeStampTeibanSeries;

void CheckTimeStampMyself();