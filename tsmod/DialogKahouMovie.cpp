#define _CRT_SECURE_NO_WARNINGS

#pragma pack(1)

#include <windows.h>
#include <tchar.h>
#include <vfw.h>
#include <stdio.h>

#include "CommonGlobalHandle.h"
#include "DialogKahouMovie.h"
#include "GameDataStruct.h"

#pragma comment(lib, "vfw32.lib ")  // mciSendCommand()�̂��߁BBCC�ł͕s�v




/*
�A�h���X  �f�[�^  ���� 
000000  52 49 46 46 D6 1A 4B 00 41 56 49 20 4C 49 53 54  RIFF . . K . AVI LIST 
000010  D4 07 00 00 68 64 72 6C 61 76 69 68 38 00 00 00  . . . . hdrl avih 8 . . . 
000020  31 04 01 00 00 00 00 00 5C 00 50 45 10 07 00 00  1 . . . . . . . \ . P E . . . . 
000030  CC 01 00 00 0C 00 00 00 02 00 00 00 4E A8 0A 00  . . . . . . . . . . . . N . . . 
000040  F0 00 00 00 B4 00 00 00 00 00 00 00 79 1A F7 BF  . . . . . . . . . . . . y . . . 
000050  F4 6C 00 00 FF FF FF FF 4C 49 53 54 74 00 00 00  . l . . . . . . LIST . . . . 
000060  73 74 72 6C 73 74 72 68 38 00 00 00 76 69 64 73  strl strh 8 . . . vids 
000070  69 76 33 32 30 75 00 00 00 00 00 00 00 00 00 00  iv32 . . . . . . . . . . .  
�A�h���X �\�� �R�����g 
000000 RIFF ('AVI ' <4,922,070 bytes> RIFF�t�@�C���w�b�_ 
00000C  LIST ('hdrl' <2,004 bytes> �t�H�[�}�b�g�`�����N 
000018  'avih' (56 bytes) ���C��AVI�w�b�_ 
000020  dwMicroSecPerFrame = 66,609 �t���[��������̃}�C�N���b 
000024 dwMaxBytesPerSec = 0 �b������̍ő�o�C�g�� 
000028 dwPaddingGranularity = 1,162,870,876  
00002C dwFlags = 0x0710 �t���O 
000030 dwTotalFrames = 460 �S�t���[���� 
000034 dwInitialFrames = 12 �Đ��J�n�t���[�� 
000038 dwStreams = 2  
000034 dwSuggestedBufferSize = 698,446 �����o�b�t�@�T�C�Y 
000040 dwWidth = 240 ��ʃT�C�Y 
000044 dwHeight = 180 ��ʃT�C�Y 
000048 dwReserved[4] �\�� 
*/


static HWND hMovieKahouMCI;
static char szMovieKahouNameMCI[256] = "";

int iMoveKahouMovieMCI_PreX;
int iMoveKahouMovieMCI_PreY;


// avi�̃s�N�Z����(�c��)�𓾂�
void getAviPixelSize(char *szFileName, int *witdh, int *height) {
	FILE *fp = fopen( szFileName, "r");
	if ( !fp ) {
		return;
	}
	char szBuf[256];
	fread(szBuf, 256, 1, fp);
	memcpy(witdh, szBuf+0x40, 4);
	memcpy(height, szBuf+0x44, 4);
	fclose(fp);
}

// ���[�r�[�ŉƕ�̃N���G�C�g
void CreateKahouMovieMCI(char *szKahouName, int pos_x, int pos_y) {
	if ( !hMovieKahouMCI ) {
		hMovieKahouMCI = MCIWndCreate(CommonGlobalHandle::tenshouWND , (HINSTANCE)CommonGlobalHandle::hDLLModule , 
			WS_CHILD | WS_VISIBLE | MCIWNDF_NOPLAYBAR , szKahouName); // MCI�E�B���h�E�𐶐��B�����Ȃ��^�C�v

		if ( hMovieKahouMCI ) {
			iMoveKahouMovieMCI_PreX = pos_x;
			iMoveKahouMovieMCI_PreY = pos_y;
			MoveKahouMovieMCI( pos_x , pos_y );
			strcpy(szMovieKahouNameMCI, szKahouName);

			if ( MCIWndCanPlay(hMovieKahouMCI) ) {
				MCIWndPlay(hMovieKahouMCI);
			}
		}
	}
}

// ���[�r�[�ŉƕ�̔j��
void DestroyKahouMovieMCI() {
	if ( hMovieKahouMCI ) {
		MCIWndClose(hMovieKahouMCI); // ���J���Ă���̂��N���[�Y
		MCIWndDestroy(hMovieKahouMCI); // MCI�E�B���h�E���̂̔j��
		szMovieKahouNameMCI[0] = NULL; 
	}
	hMovieKahouMCI = NULL;
}

// ���[�r�[
void MoveKahouMovieMCI(int pos_x, int pos_y ) {
	if ( hMovieKahouMCI ) {
		// ���[�r�[�̏c���̃s�N�Z���𓾂�
		int width, height;
		getAviPixelSize( szMovieKahouNameMCI, &width, &height );
		// ���S���u
		if ( width == 0 || height == 0 || width > 128 || height > 128 ) {
			width = 64; height = 64;
		}
		MoveWindow(hMovieKahouMCI , pos_x , pos_y , width , height , FALSE);

		// �O��̈ʒu�ƈ������A�ŏI�t���[���Ȃ�Đ����Ȃ���
		if ( pos_x != iMoveKahouMovieMCI_PreX || pos_y != iMoveKahouMovieMCI_PreY ) {
			if ( MCIWndGetPosition(hMovieKahouMCI) == MCIWndGetEnd(hMovieKahouMCI)) {
				if ( MCIWndCanPlay(hMovieKahouMCI) ) {
					MCIWndPlay(hMovieKahouMCI);
				}
			}
		}

		iMoveKahouMovieMCI_PreX = pos_x;
		iMoveKahouMovieMCI_PreY = pos_y;
	}
}