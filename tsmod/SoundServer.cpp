#define  _CRT_SECURE_NO_WARNINGS

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "WriteSoundMap.h"
#include "SoundServer.h"

SOUNDHANDLE hSndHandle = 100000;

// ２Ｄで普通にファイルを再生(wav, mp3)
SOUNDHANDLE PlaySound2DStr( char *szFileName, BOOL isLoop ) {

	// 環境音の設定が有効であれば…
	if ( !isSEValid() ) {
		return 0;
	}
	// フォアグランドではない上、かつ、メッセージスピードが一番左(超早い）ならば、サウンドは鳴らさない
	if ( GetForegroundWindow()!=CommonGlobalHandle::tenshouWND && nb6settings.messagespd == 0) {
		return 0;
	}

	hSndHandle++;

	TSoundWaveParam t = {0};
	t.hSound = hSndHandle;
	strcpy(t.szCmd, "PlaySound2DStr");
	strcpy(t.szFileName, szFileName);
	t.isLoop = isLoop;
	WriteSoundFMap(&t);

	return hSndHandle;
}

// ２Ｄでリソースを再生(wav, mp3)
SOUNDHANDLE PlaySound2DResID( int iResID, HMODULE hModule, BOOL isLoop ) {

	// 環境音の設定が有効であれば…
	if ( !isSEValid() ) {
		return 0;
	}
	// フォアグランドではない上、かつ、メッセージスピードが一番左(超早い）ならば、サウンドは鳴らさない
	if ( GetForegroundWindow()!=CommonGlobalHandle::tenshouWND && nb6settings.messagespd == 0) {
		return 0;
	}


	hSndHandle++;

	TSoundWaveParam t = {0};
	t.hSound = hSndHandle;
	strcpy(t.szCmd, "PlaySound2DResID");
	t.iResID = iResID;

	char szFullDLLName[512] = "";
	// DLLハンドル(実際にはメモリ上のアドレス)⇒そやつのフルネームを得る。
	GetModuleFileNameA( hModule, szFullDLLName, sizeof(szFullDLLName)-1);
	strcpy( t.hModuleName, szFullDLLName );


	t.isLoop = isLoop;
	WriteSoundFMap(&t);

	return hSndHandle;
}

// ３Ｄで普通にファイルを再生(wav, mp3)
SOUNDHANDLE PlaySound3DStr( char *szFileName, BOOL isLoop, double px, double py, double pz,  double vx, double vy, double vz ) {

	// 環境音の設定が有効であれば…
	if ( !isSEValid() ) {
		return 0;
	}
	// フォアグランドではない上、かつ、メッセージスピードが一番左(超早い）ならば、サウンドは鳴らさない
	if ( GetForegroundWindow()!=CommonGlobalHandle::tenshouWND && nb6settings.messagespd == 0) {
		return 0;
	}


	hSndHandle++;

	TSoundWaveParam t = {0};
	t.hSound = hSndHandle;
	strcpy(t.szCmd, "PlaySound3DStr");
	strcpy(t.szFileName, szFileName);
	t.isLoop = isLoop;
	t.px = (float)px;
	t.py = (float)py;
	t.pz = (float)pz;
	t.vx = (float)vx;
	t.vy = (float)vy;
	t.vz = (float)vz;
	WriteSoundFMap(&t);

	return hSndHandle;
}

// ３Ｄでリソースを再生(wav, mp3)
SOUNDHANDLE PlaySound3DResID( int iResID, HMODULE hModule, BOOL isLoop , double px, double py, double pz,  double vx, double vy, double vz ) {

	// 環境音の設定が有効であれば…
	if ( !isSEValid() ) {
		return 0;
	}
	// フォアグランドではない上、かつ、メッセージスピードが一番左(超早い）ならば、サウンドは鳴らさない
	if ( GetForegroundWindow()!=CommonGlobalHandle::tenshouWND && nb6settings.messagespd == 0) {
		return 0;
	}


	hSndHandle++;

	TSoundWaveParam t = {0};
	t.hSound = hSndHandle;
	strcpy(t.szCmd, "PlaySound3DResID");
	t.iResID = iResID;

	char szFullDLLName[512] = "";
	// DLLハンドル(実際にはメモリ上のアドレス)⇒そやつのフルネームを得る。
	GetModuleFileNameA( hModule, szFullDLLName, sizeof(szFullDLLName)-1);
	strcpy( t.hModuleName, szFullDLLName );

	t.isLoop = isLoop;
	t.px = (float)px;
	t.py = (float)py;
	t.pz = (float)pz;
	t.vx = (float)vx;
	t.vy = (float)vy;
	t.vz = (float)vz;

	WriteSoundFMap(&t);

	return hSndHandle;
}


// リソースＩＤで再生を停止(wav, mp3)
extern SOUNDHANDLE hCurPlayingMidiSndHandle; // MIDI用のサウンドハンドル。１ラインしかない。
void StopSound( SOUNDHANDLE hSound ) {

	// Midi以外
	TSoundWaveParam t = {0};
	t.hSound = hSound;
	strcpy(t.szCmd, "StopSound");
	WriteSoundFMap(&t);
}


// 全ての効果音を停止
void StopAllSound() {
	TSoundWaveParam t = {0};
	strcpy(t.szCmd, "StopAllSound");
	WriteSoundFMap(&t);
}

// ＭＩＤＩマッパーの変更
void ChangeMidiMapper(char *szNewMidiMapper) {
	TSoundWaveParam t = {0};
	strcpy(t.szCmd, "ChangeMidiMapper");
	strcpy(t.szFileName, szNewMidiMapper);
	WriteSoundFMap(&t);
}

//--------------------以下、外部呼び出し用

// ２Ｄで普通にファイルを再生(wav, mp3)
SOUNDHANDLE WINAPI Extern_PlaySound2DStr( char *szFileName, BOOL isLoop ) {
	return PlaySound2DStr(szFileName, isLoop);
}

// ２Ｄでリソースを再生(wav, mp3)
SOUNDHANDLE WINAPI Extern_PlaySound2DResID( int iResID, HMODULE hModule, BOOL isLoop ) {
	return PlaySound2DResID(iResID, hModule, isLoop);
}

// ３Ｄで普通にファイルを再生(wav, mp3)
SOUNDHANDLE WINAPI Extern_PlaySound3DStr( char *szFileName, BOOL isLoop, double px, double py, double pz,  double vx, double vy, double vz) {
	return PlaySound3DStr(szFileName, isLoop, px, py, pz, vx, vy, vz);
}

// ３Ｄでリソースを再生(wav, mp3)
SOUNDHANDLE WINAPI Extern_PlaySound3DResID( int iResID, HMODULE hModule, BOOL isLoop , double px, double py, double pz,  double vx, double vy, double vz) {
	return PlaySound3DResID(iResID, hModule, isLoop, px, py, pz, vx, vy, vz);
}

// ハンドルで再生を停止(wav, mp3)
void WINAPI Extern_StopSound( SOUNDHANDLE hSound ) {
	StopSound(hSound);
}

// ３Ｄサラウンドの全ての再生を停止
void WINAPI Extern_StopAllSound() {
	StopAllSound();
}


// ＭＩＤＩマッパーの変更() {
void WINAPI Extern_ChangeMidiMapper(char *szNewMidiMapper) {
	ChangeMidiMapper(szNewMidiMapper);
}
