#define  _CRT_SECURE_NO_WARNINGS

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "WriteSoundMap.h"
#include "SoundServer.h"

SOUNDHANDLE hSndHandle = 100000;

// �Q�c�ŕ��ʂɃt�@�C�����Đ�(wav, mp3)
SOUNDHANDLE PlaySound2DStr( char *szFileName, BOOL isLoop ) {

	// �����̐ݒ肪�L���ł���΁c
	if ( !isSEValid() ) {
		return 0;
	}
	// �t�H�A�O�����h�ł͂Ȃ���A���A���b�Z�[�W�X�s�[�h����ԍ�(�������j�Ȃ�΁A�T�E���h�͖炳�Ȃ�
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

// �Q�c�Ń��\�[�X���Đ�(wav, mp3)
SOUNDHANDLE PlaySound2DResID( int iResID, HMODULE hModule, BOOL isLoop ) {

	// �����̐ݒ肪�L���ł���΁c
	if ( !isSEValid() ) {
		return 0;
	}
	// �t�H�A�O�����h�ł͂Ȃ���A���A���b�Z�[�W�X�s�[�h����ԍ�(�������j�Ȃ�΁A�T�E���h�͖炳�Ȃ�
	if ( GetForegroundWindow()!=CommonGlobalHandle::tenshouWND && nb6settings.messagespd == 0) {
		return 0;
	}


	hSndHandle++;

	TSoundWaveParam t = {0};
	t.hSound = hSndHandle;
	strcpy(t.szCmd, "PlaySound2DResID");
	t.iResID = iResID;

	char szFullDLLName[512] = "";
	// DLL�n���h��(���ۂɂ̓�������̃A�h���X)�˂���̃t���l�[���𓾂�B
	GetModuleFileNameA( hModule, szFullDLLName, sizeof(szFullDLLName)-1);
	strcpy( t.hModuleName, szFullDLLName );


	t.isLoop = isLoop;
	WriteSoundFMap(&t);

	return hSndHandle;
}

// �R�c�ŕ��ʂɃt�@�C�����Đ�(wav, mp3)
SOUNDHANDLE PlaySound3DStr( char *szFileName, BOOL isLoop, double px, double py, double pz,  double vx, double vy, double vz ) {

	// �����̐ݒ肪�L���ł���΁c
	if ( !isSEValid() ) {
		return 0;
	}
	// �t�H�A�O�����h�ł͂Ȃ���A���A���b�Z�[�W�X�s�[�h����ԍ�(�������j�Ȃ�΁A�T�E���h�͖炳�Ȃ�
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

// �R�c�Ń��\�[�X���Đ�(wav, mp3)
SOUNDHANDLE PlaySound3DResID( int iResID, HMODULE hModule, BOOL isLoop , double px, double py, double pz,  double vx, double vy, double vz ) {

	// �����̐ݒ肪�L���ł���΁c
	if ( !isSEValid() ) {
		return 0;
	}
	// �t�H�A�O�����h�ł͂Ȃ���A���A���b�Z�[�W�X�s�[�h����ԍ�(�������j�Ȃ�΁A�T�E���h�͖炳�Ȃ�
	if ( GetForegroundWindow()!=CommonGlobalHandle::tenshouWND && nb6settings.messagespd == 0) {
		return 0;
	}


	hSndHandle++;

	TSoundWaveParam t = {0};
	t.hSound = hSndHandle;
	strcpy(t.szCmd, "PlaySound3DResID");
	t.iResID = iResID;

	char szFullDLLName[512] = "";
	// DLL�n���h��(���ۂɂ̓�������̃A�h���X)�˂���̃t���l�[���𓾂�B
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


// ���\�[�X�h�c�ōĐ����~(wav, mp3)
extern SOUNDHANDLE hCurPlayingMidiSndHandle; // MIDI�p�̃T�E���h�n���h���B�P���C�������Ȃ��B
void StopSound( SOUNDHANDLE hSound ) {

	// Midi�ȊO
	TSoundWaveParam t = {0};
	t.hSound = hSound;
	strcpy(t.szCmd, "StopSound");
	WriteSoundFMap(&t);
}


// �S�Ă̌��ʉ����~
void StopAllSound() {
	TSoundWaveParam t = {0};
	strcpy(t.szCmd, "StopAllSound");
	WriteSoundFMap(&t);
}

// �l�h�c�h�}�b�p�[�̕ύX
void ChangeMidiMapper(char *szNewMidiMapper) {
	TSoundWaveParam t = {0};
	strcpy(t.szCmd, "ChangeMidiMapper");
	strcpy(t.szFileName, szNewMidiMapper);
	WriteSoundFMap(&t);
}

//--------------------�ȉ��A�O���Ăяo���p

// �Q�c�ŕ��ʂɃt�@�C�����Đ�(wav, mp3)
SOUNDHANDLE WINAPI Extern_PlaySound2DStr( char *szFileName, BOOL isLoop ) {
	return PlaySound2DStr(szFileName, isLoop);
}

// �Q�c�Ń��\�[�X���Đ�(wav, mp3)
SOUNDHANDLE WINAPI Extern_PlaySound2DResID( int iResID, HMODULE hModule, BOOL isLoop ) {
	return PlaySound2DResID(iResID, hModule, isLoop);
}

// �R�c�ŕ��ʂɃt�@�C�����Đ�(wav, mp3)
SOUNDHANDLE WINAPI Extern_PlaySound3DStr( char *szFileName, BOOL isLoop, double px, double py, double pz,  double vx, double vy, double vz) {
	return PlaySound3DStr(szFileName, isLoop, px, py, pz, vx, vy, vz);
}

// �R�c�Ń��\�[�X���Đ�(wav, mp3)
SOUNDHANDLE WINAPI Extern_PlaySound3DResID( int iResID, HMODULE hModule, BOOL isLoop , double px, double py, double pz,  double vx, double vy, double vz) {
	return PlaySound3DResID(iResID, hModule, isLoop, px, py, pz, vx, vy, vz);
}

// �n���h���ōĐ����~(wav, mp3)
void WINAPI Extern_StopSound( SOUNDHANDLE hSound ) {
	StopSound(hSound);
}

// �R�c�T���E���h�̑S�Ă̍Đ����~
void WINAPI Extern_StopAllSound() {
	StopAllSound();
}


// �l�h�c�h�}�b�p�[�̕ύX() {
void WINAPI Extern_ChangeMidiMapper(char *szNewMidiMapper) {
	ChangeMidiMapper(szNewMidiMapper);
}
