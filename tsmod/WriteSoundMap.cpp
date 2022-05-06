#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include "WriteSoundMap.h"
#include "SoundServer.h"

char *lpCommonMapAddress;

HANDLE hFMWrite;
extern HANDLE globalSndHANDLE;

int CreateSoundFMap() {
	// ���łɑ��݂�����A����
	if (hFMWrite) {
		CloseHandle(hFMWrite);
	}

	// CreateFileMapping�֐��Ńt�@�C���}�b�s���O�I�u�W�F�N�g���쐬���܂��B 
	hFMWrite = CreateFileMapping( (HANDLE)-1, NULL, PAGE_READWRITE, 0, sizeof(TSoundWaveParam)*SOUND_SERVER_ARRAY_CNT, "TENSHOUKI_SOUND_MAPPING"); // ���S�����āA�]���f�[�^�T�C�Y*10�܂ŁB

	if (hFMWrite == NULL) {
		return -1;
	}

	// �������݃��[�h
	lpCommonMapAddress = (LPSTR)MapViewOfFile(hFMWrite, FILE_MAP_ALL_ACCESS, 0, 0, 0);

	// ���s����
	if (lpCommonMapAddress == NULL) {
		return -2;
	}

	// �O�Ŗ��߂�
	memset(lpCommonMapAddress, NULL, sizeof(TSoundWaveParam)*SOUND_SERVER_ARRAY_CNT );

	// �������݃��[�h����
	UnmapViewOfFile(lpCommonMapAddress);

	// �����I��
	return 0;
}

// ���L�������ւ̏������݁B
// lpStr
int CleanSoundFMap() {

	if (hFMWrite == NULL)
		return -1;

	// �������݃��[�h
	lpCommonMapAddress = (LPSTR)MapViewOfFile(hFMWrite, FILE_MAP_ALL_ACCESS, 0, 0, 0);

	// ���s����
	if (lpCommonMapAddress == NULL) {
		return -2;
	}

	// �O�Ŗ��߂�
	memset(lpCommonMapAddress, NULL, sizeof(TSoundWaveParam)*SOUND_SERVER_ARRAY_CNT );
	// �������݃��[�h����
	UnmapViewOfFile(lpCommonMapAddress);

	// �����I��
	return 0;
}


// ���L�������ւ̏������݁B
// lpStr
int WriteSoundFMap(TSoundWaveParam *pWriteData) {

	if (hFMWrite == NULL) {
		return -1;
	}

	// �������݃��[�h
	lpCommonMapAddress = (LPSTR)MapViewOfFile(hFMWrite, FILE_MAP_ALL_ACCESS, 0, 0, 0);

	// ���s����
	if (lpCommonMapAddress == NULL) {
		return -2;
	}

	// ���L�}�b�v��TSoundWaveParam�^�̔z��(SOUND_SERVER_ARRAY_CNT)�Ƃ݂Ȃ��āc
	TSoundWaveParam *t = (TSoundWaveParam *)lpCommonMapAddress;

	for ( int i=0; i<SOUND_SERVER_ARRAY_CNT; i++, t++ ) {

		// �܂��f�[�^���l�܂��Ă��Ȃ��g�ł���΁A�f�[�^�������ɃR�s�[
		// t[i]->isEnteringData �� 0 ���Z�b�e�B���O����̂́A��M��
		if ( t && t->isEnteringData == 0 ) {

			t->hSound		   = pWriteData->hSound;
			t->iResID		   = pWriteData->iResID;
			strcpy( t->hModuleName, pWriteData->hModuleName);
			t->px			   = pWriteData->px;
			t->py			   = pWriteData->py;
			t->pz			   = pWriteData->pz;
			t->vx			   = pWriteData->vx;
			t->vy			   = pWriteData->vy;
			t->vz			   = pWriteData->vz;
			t->isLoop		   = pWriteData->isLoop;
			strcpy(t->szCmd     , pWriteData->szCmd );		 
			strcpy(t->szFileName, pWriteData->szFileName );		 

			t->isEnteringData = 1;
			// �P�������̂ŏI��
			break;
		}
	}

	// �������݃��[�h����
	UnmapViewOfFile(lpCommonMapAddress);

	// �����I��
	return 0;
}


int CloseSoundFMap() {
	if (hFMWrite == NULL) {
		return -1;
	}

	// �������݃��[�h
	lpCommonMapAddress = (LPSTR)MapViewOfFile(hFMWrite, FILE_MAP_ALL_ACCESS, 0, 0, 0);

	if (lpCommonMapAddress == NULL) {
		return -2;
	}

	// �O�Ŗ��߂�
	memset(lpCommonMapAddress, NULL, sizeof(TSoundWaveParam)-1 );

	// �������݃��[�h����
	UnmapViewOfFile(lpCommonMapAddress);

	// ����
	if (hFMWrite) {
		CloseHandle(hFMWrite);
	}

	// �����I��
	return 0;
}

