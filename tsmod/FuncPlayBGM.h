#pragma once

#pragma pack(1)


// 指定のＢＧＭを再生する
void FuncPlayBGM(int iBGM);

// ＢＧＭを再生中か？
int isPlayBGM();

// 再生中のBGMのIDを得る。0xFFFFだと再生中ではない。
int getPlayingBGM();

// 再生中のＢＧＭを止める
void FuncStopBGM();