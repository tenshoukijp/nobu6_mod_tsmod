#pragma once

#include "resource.h"
#pragma once
#pragma pack(1)

typedef int SOUNDHANDLE;

#define SOUND_SERVER_ARRAY_CNT	8

struct TSoundWaveParam {
	int isEnteringData;		// 排他制御フラグ。サウンド情報が格納されると１が立ち、再生しおえると０となる。
	SOUNDHANDLE hSound;			// サウンド停止用などのUniqueID;
	int iResID;				// リソースＩＤによる再生用
	char hModuleName[256];		// リソースＩＤ時のファイル名用
	float px;				// ３Ｄ再生時用のX座標
	float py;				// ３Ｄ再生時用のY座標
	float pz;				// ３Ｄ再生時用のZ座標
	float vx;				// ３Ｄ再生時用のエミッタ―のX運動
	float vy;				// ３Ｄ再生時用のエミッタ―のY運動
	float vz;				// ３Ｄ再生時用のエミッタ―のZ運動
	BOOL isLoop;			// 繰り返しフラグ
	char szCmd[64];			// コマンド文字列
	char szFileName[256];	// ファイル名
};


// ２Ｄで普通にファイルを再生(wav, mp3)
SOUNDHANDLE PlaySound2DStr( char *szFileName, BOOL isLoop );

// ２Ｄでリソースを再生(wav, mp3)
SOUNDHANDLE PlaySound2DResID( int iResID, HMODULE hModule, BOOL isLoop );

// ３Ｄで普通にファイルを再生(wav, mp3)
SOUNDHANDLE PlaySound3DStr( char *szFileName, BOOL isLoop, double px, double py, double pz,  double vx, double vy, double vz);

// ３Ｄでリソースを再生(wav, mp3)
SOUNDHANDLE PlaySound3DResID( int iResID, HMODULE hModule, BOOL isLoop , double px, double py, double pz,  double vx, double vy, double vz);

// リソースＩＤで再生を停止(wav, mp3)
void StopSound( SOUNDHANDLE hSound );

// ３Ｄサラウンドの全ての再生を停止
void StopAllSound();
