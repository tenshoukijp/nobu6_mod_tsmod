#pragma once

#include "resource.h"
#pragma once
#pragma pack(1)

typedef int SOUNDHANDLE;

#define SOUND_SERVER_ARRAY_CNT	8

struct TSoundWaveParam {
	int isEnteringData;		// �r������t���O�B�T�E���h��񂪊i�[�����ƂP�������A�Đ���������ƂO�ƂȂ�B
	SOUNDHANDLE hSound;			// �T�E���h��~�p�Ȃǂ�UniqueID;
	int iResID;				// ���\�[�X�h�c�ɂ��Đ��p
	char hModuleName[256];		// ���\�[�X�h�c���̃t�@�C�����p
	float px;				// �R�c�Đ����p��X���W
	float py;				// �R�c�Đ����p��Y���W
	float pz;				// �R�c�Đ����p��Z���W
	float vx;				// �R�c�Đ����p�̃G�~�b�^�\��X�^��
	float vy;				// �R�c�Đ����p�̃G�~�b�^�\��Y�^��
	float vz;				// �R�c�Đ����p�̃G�~�b�^�\��Z�^��
	BOOL isLoop;			// �J��Ԃ��t���O
	char szCmd[64];			// �R�}���h������
	char szFileName[256];	// �t�@�C����
};


// �Q�c�ŕ��ʂɃt�@�C�����Đ�(wav, mp3)
SOUNDHANDLE PlaySound2DStr( char *szFileName, BOOL isLoop );

// �Q�c�Ń��\�[�X���Đ�(wav, mp3)
SOUNDHANDLE PlaySound2DResID( int iResID, HMODULE hModule, BOOL isLoop );

// �R�c�ŕ��ʂɃt�@�C�����Đ�(wav, mp3)
SOUNDHANDLE PlaySound3DStr( char *szFileName, BOOL isLoop, double px, double py, double pz,  double vx, double vy, double vz);

// �R�c�Ń��\�[�X���Đ�(wav, mp3)
SOUNDHANDLE PlaySound3DResID( int iResID, HMODULE hModule, BOOL isLoop , double px, double py, double pz,  double vx, double vy, double vz);

// ���\�[�X�h�c�ōĐ����~(wav, mp3)
void StopSound( SOUNDHANDLE hSound );

// �R�c�T���E���h�̑S�Ă̍Đ����~
void StopAllSound();
