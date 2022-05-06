#pragma once
#pragma pack(1)

// 指定のファイル名のプロセス名が何個実行されいるかを得る
int GetTargetProcessCount(char* process);


// 現在実行中の自分のプロセスのウィンドウハンドルを得る。
HWND GetCurrentWindowHandle();